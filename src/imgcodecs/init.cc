#include "common.h"

#include "../core/Matrix.h"


class HybridAsyncWorker : public Nan::AsyncWorker {
public:
  HybridAsyncWorker() : AsyncWorker{ nullptr } {}

  virtual Local<Value> GetResult() = 0;

  bool HasError() const {
    return ErrorMessage() != NULL;
  }

  Local<Value> Error() const {
    Nan::EscapableHandleScope scope;

    return scope.Escape(Nan::Error(ErrorMessage()));
  }

protected:
  void HandleOKCallback() override final {
    Nan::HandleScope scope;

    Local<Value> result = GetResult();

    Local<Value> callback_or_resolver = GetFromPersistent(0u);
    if (callback_or_resolver->IsFunction()) {
      Local<Value> argv[2] = { Nan::Undefined(), result };

      Nan::TryCatch try_catch;
      Local<Function>::Cast(callback_or_resolver)->Call(Nan::GetCurrentContext()->Global(), 2, argv);
      if (try_catch.HasCaught()) {
        Nan::FatalException(try_catch);
      }
    } else {
      Local<Promise::Resolver>::Cast(callback_or_resolver)->Resolve(result);
      Isolate::GetCurrent()->RunMicrotasks();
    }
  }

  void HandleErrorCallback() override final {
    Nan::HandleScope scope;

    Local<Value> error = Error();

    Local<Value> callback_or_resolver = GetFromPersistent(0u);
    if (callback_or_resolver->IsFunction()) {
      Local<Value> argv[1] { error };

      Nan::TryCatch try_catch;
      Nan::CallAsFunction(callback_or_resolver->ToObject(), Nan::GetCurrentContext()->Global(), 1, argv);
      if (try_catch.HasCaught()) {
        Nan::FatalException(try_catch);
      }
    } else {
      Local<Promise::Resolver>::Cast(callback_or_resolver)->Reject(error);
      Isolate::GetCurrent()->RunMicrotasks();
    }
  }
};

class BaseImageReadingAsyncWorker : public HybridAsyncWorker {
public:
  BaseImageReadingAsyncWorker() : HybridAsyncWorker() {}

  void SetImreadMode(cv::ImreadModes mode) {
    this->mode = mode;
  }

protected:
  cv::ImreadModes mode = cv::IMREAD_COLOR;
};

class ReadImageAsyncWorker : public BaseImageReadingAsyncWorker {
public:
  ReadImageAsyncWorker(std::string const path) : BaseImageReadingAsyncWorker(), path(path) {}
  ReadImageAsyncWorker(unsigned const length, uint8_t* const data) : BaseImageReadingAsyncWorker(), length(length), data(data) {}

  void Execute() override {
    try {
      if (data == nullptr) {
        mat = cv::imread(path, mode);
      } else {
        cv::Mat mbuf(length, 1, CV_64FC1, data);
        mat = cv::imdecode(mbuf, mode);
      }
    } catch (cv::Exception& e) {
      return SetErrorMessage(e.what());
    }

    if (mat.empty()) {
      SetErrorMessage("Could not open or find the image");
    }
  }

  Local<Value> GetResult() {
    Nan::EscapableHandleScope scope;
    return scope.Escape(Matrix::NewInstance(mat));
  }

private:
  std::string const path = "";

  unsigned const length = 0;
  uint8_t* const data = nullptr;

  cv::Mat mat;
};

NAN_METHOD(ImageRead) {
  FUNCTION_REQUIRE_ARGUMENTS_RANGE(1, 3);
  Nan::HandleScope scope;
  ASSERT_STRING_FROM_ARGS(filename, 0);
  int argumentOffset = 1;
  INT_FROM_ARGS(mode, 1) else {
    mode = cv::IMREAD_ANYCOLOR;
    argumentOffset = 0;
  }

  bool isCallback = false;
  if (info.Length() > 1 + argumentOffset) {
    if (!info[1 + argumentOffset]->IsFunction()) {
      return THROW_INVALID_ARGUMENT_TYPE(1 + argumentOffset, "a function");
    }

    isCallback = true;
  }

  ReadImageAsyncWorker* worker = new ReadImageAsyncWorker(filename);
  worker->SetImreadMode(static_cast<cv::ImreadModes>(mode));

  if (isCallback) {
    worker->SaveToPersistent(0u, info[1 + argumentOffset]);
  } else {
    Local<Promise::Resolver> resolver = Promise::Resolver::New(Nan::GetCurrentContext()).ToLocalChecked();
    worker->SaveToPersistent(0u, resolver);
    info.GetReturnValue().Set(resolver->GetPromise());
  }

  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(ImageReadSync) {
  FUNCTION_REQUIRE_ARGUMENTS_RANGE(1, 2);
  Nan::HandleScope scope;
  ASSERT_STRING_FROM_ARGS(filename, 0);
  DEFAULT_INT_FROM_ARGS(mode, 1, cv::IMREAD_ANYCOLOR);

  ReadImageAsyncWorker worker(filename);
  worker.SetImreadMode(static_cast<cv::ImreadModes>(mode));
  worker.Execute();

  if (worker.HasError()) {
    return Nan::ThrowError(worker.Error());
  }

  info.GetReturnValue().Set(worker.GetResult());
}

NAN_METHOD(ImageDecode) {
  FUNCTION_REQUIRE_ARGUMENTS_RANGE(1, 3);
  Nan::EscapableHandleScope scope;
  if (!Buffer::HasInstance(info[0])) {
    return THROW_INVALID_ARGUMENT_TYPE(0, "a Buffer");
  }

  int argumentOffset = 1;
  INT_FROM_ARGS(mode, 1) else {
    mode = cv::IMREAD_ANYCOLOR;
    argumentOffset = 0;
  }

  bool isCallback = false;
  if (info.Length() > 1 + argumentOffset) {
    if (!info[1 + argumentOffset]->IsFunction()) {
      return THROW_INVALID_ARGUMENT_TYPE(1 + argumentOffset, "a function");
    }

    isCallback = true;
  }

  ReadImageAsyncWorker* worker = new ReadImageAsyncWorker(Buffer::Length(info[0]->ToObject()), reinterpret_cast<uint8_t *>(Buffer::Data(info[0]->ToObject())));
  worker->SetImreadMode(static_cast<cv::ImreadModes>(mode));

  if (isCallback) {
    worker->SaveToPersistent(0u, info[1 + argumentOffset]);
  } else {
    Local<Promise::Resolver> resolver = Promise::Resolver::New(Nan::GetCurrentContext()).ToLocalChecked();
    worker->SaveToPersistent(0u, resolver);
    info.GetReturnValue().Set(resolver->GetPromise());
  }

  Nan::AsyncQueueWorker(worker);
}

NAN_METHOD(ImageDecodeSync) {
  FUNCTION_REQUIRE_ARGUMENTS_RANGE(1, 2);
  Nan::HandleScope scope;
  if (!Buffer::HasInstance(info[0])) {
    return THROW_INVALID_ARGUMENT_TYPE(0, "a Buffer");
  }
  Local<Object> buffer = info[0]->ToObject();
  DEFAULT_INT_FROM_ARGS(mode, 1, cv::IMREAD_ANYCOLOR);

  ReadImageAsyncWorker worker(Buffer::Length(buffer), reinterpret_cast<uint8_t *>(Buffer::Data(buffer)));
  worker.SetImreadMode(static_cast<cv::ImreadModes>(mode));
  worker.Execute();

  if (worker.HasError()) {
    return Nan::ThrowError(worker.Error());
  }

  info.GetReturnValue().Set(worker.GetResult());
}

#if CV_MAJOR_VERSION >= 3
class ReadImageMultiAsyncWorker : public BaseImageReadingAsyncWorker {
public:
  ReadImageMultiAsyncWorker(const std::string &path) : BaseImageReadingAsyncWorker(), path(path) {}

  void Execute() override {
    try {
      cv::imreadmulti(path, mats, mode);
    } catch (cv::Exception& e) {
      return SetErrorMessage(e.what());
    }

    if (mats.empty()) {
      SetErrorMessage("Error loading file");
    }
  }

  Local<Value> GetResult() {
    Nan::HandleScope scope;

    Local<Array> out = Nan::New<Array>();
    for (cv::Mat &mat : mats) {
      Nan::Set(out, out->Length(), Matrix::NewInstance(mat));
    }

    return out;
  }

private:
  const std::string path = nullptr;

  std::vector<cv::Mat> mats;
};
#endif

NAN_METHOD(ImageReadMulti) {
#if CV_MAJOR_VERSION >= 3
  FUNCTION_REQUIRE_ARGUMENTS_RANGE(1, 3);
  Nan::EscapableHandleScope scope;

  int argumentOffset = 1;
  INT_FROM_ARGS(mode, 1) else {
    mode = cv::IMREAD_ANYCOLOR;
    argumentOffset = 0;
  }

  bool isCallback = false;
  if (info.Length() > 1 + argumentOffset) {
    if (!info[1 + argumentOffset]->IsFunction()) {
      return THROW_INVALID_ARGUMENT_TYPE(1 + argumentOffset, "a function");
    }

    isCallback = true;
  }

  ASSERT_STRING_FROM_ARGS(path, 0);

  ReadImageMultiAsyncWorker* worker = new ReadImageMultiAsyncWorker(path);

  worker->SetImreadMode(static_cast<cv::ImreadModes>(mode));

  if (isCallback) {
    worker->SaveToPersistent(0u, info[1 + argumentOffset]);
  } else {
    Local<Promise::Resolver> resolver = Promise::Resolver::New(Nan::GetCurrentContext()).ToLocalChecked();
    worker->SaveToPersistent(0u, resolver);
    info.GetReturnValue().Set(resolver->GetPromise());
  }

  Nan::AsyncQueueWorker(worker);
#else
  Nan::ThrowError("only supported in opencv 3.x")
#endif
}

NAN_METHOD(ImageReadMultiSync) {
#if CV_MAJOR_VERSION >= 3
  FUNCTION_REQUIRE_ARGUMENTS_RANGE(1, 2);
  Nan::EscapableHandleScope scope;
  ASSERT_STRING_FROM_ARGS(path, 0);
  DEFAULT_INT_FROM_ARGS(mode, 1, cv::IMREAD_ANYCOLOR);

  ReadImageMultiAsyncWorker worker(path);
  worker.SetImreadMode(static_cast<cv::ImreadModes>(mode));
  worker.Execute();

  if (worker.HasError()) {
    return Nan::ThrowError(worker.Error());
  }

  info.GetReturnValue().Set(worker.GetResult());
#else
  Nan::ThrowError("only supported in opencv 3.x")
#endif
}

extern "C" void init(Local<Object> target) {
  Nan::HandleScope scope;

  // imread modes
  DEFINE_CONST_ENUM(IMREAD_UNCHANGED);
  DEFINE_CONST_ENUM(IMREAD_GRAYSCALE);
  DEFINE_CONST_ENUM(IMREAD_COLOR);
  DEFINE_CONST_ENUM(IMREAD_ANYDEPTH);
  DEFINE_CONST_ENUM(IMREAD_ANYCOLOR);
  DEFINE_CONST_ENUM(IMREAD_LOAD_GDAL);
  DEFINE_CONST_ENUM(IMREAD_REDUCED_GRAYSCALE_2);
  DEFINE_CONST_ENUM(IMREAD_REDUCED_COLOR_2);
  DEFINE_CONST_ENUM(IMREAD_REDUCED_GRAYSCALE_4);
  DEFINE_CONST_ENUM(IMREAD_REDUCED_COLOR_4);
  DEFINE_CONST_ENUM(IMREAD_REDUCED_GRAYSCALE_8);
  DEFINE_CONST_ENUM(IMREAD_REDUCED_COLOR_8);
  DEFINE_CONST_ENUM(IMREAD_IGNORE_ORIENTATION);

  // imwrite flags
  DEFINE_CONST_ENUM(IMWRITE_JPEG_QUALITY);
  DEFINE_CONST_ENUM(IMWRITE_JPEG_PROGRESSIVE);
  DEFINE_CONST_ENUM(IMWRITE_JPEG_OPTIMIZE);
  DEFINE_CONST_ENUM(IMWRITE_JPEG_RST_INTERVAL);
  DEFINE_CONST_ENUM(IMWRITE_JPEG_LUMA_QUALITY);
  DEFINE_CONST_ENUM(IMWRITE_JPEG_CHROMA_QUALITY);
  DEFINE_CONST_ENUM(IMWRITE_PNG_COMPRESSION);
  DEFINE_CONST_ENUM(IMWRITE_PNG_STRATEGY);
  DEFINE_CONST_ENUM(IMWRITE_PNG_BILEVEL);
  DEFINE_CONST_ENUM(IMWRITE_PXM_BINARY);
  DEFINE_CONST_ENUM(IMWRITE_WEBP_QUALITY);
  DEFINE_CONST_ENUM(IMWRITE_PAM_TUPLETYPE);

  // imwrite pam flags
  DEFINE_CONST_ENUM(IMWRITE_PAM_FORMAT_NULL);
  DEFINE_CONST_ENUM(IMWRITE_PAM_FORMAT_BLACKANDWHITE);
  DEFINE_CONST_ENUM(IMWRITE_PAM_FORMAT_GRAYSCALE);
  DEFINE_CONST_ENUM(IMWRITE_PAM_FORMAT_GRAYSCALE_ALPHA);
  DEFINE_CONST_ENUM(IMWRITE_PAM_FORMAT_RGB);
  DEFINE_CONST_ENUM(IMWRITE_PAM_FORMAT_RGB_ALPHA);

  // imwrite png flags
  DEFINE_CONST_ENUM(IMWRITE_PNG_STRATEGY_DEFAULT);
  DEFINE_CONST_ENUM(IMWRITE_PNG_STRATEGY_FILTERED);
  DEFINE_CONST_ENUM(IMWRITE_PNG_STRATEGY_HUFFMAN_ONLY);
  DEFINE_CONST_ENUM(IMWRITE_PNG_STRATEGY_RLE);
  DEFINE_CONST_ENUM(IMWRITE_PNG_STRATEGY_FIXED);

  Nan::SetMethod(target, "imread", ImageRead);
  Nan::SetMethod(target, "imreadSync", ImageReadSync);
  Nan::SetMethod(target, "imdecode", ImageDecode);
  Nan::SetMethod(target, "imdecodeSync", ImageDecodeSync);
  Nan::SetMethod(target, "imencode", NotImplemented);
  Nan::SetMethod(target, "imencodeSync", NotImplemented);
  Nan::SetMethod(target, "imreadmulti", ImageReadMulti);
  Nan::SetMethod(target, "imreadmultiSync", ImageReadMultiSync);
  Nan::SetMethod(target, "imwrite", NotImplemented);
  Nan::SetMethod(target, "imwriteSync", NotImplemented);
}

NODE_MODULE(imgcodec, init);
