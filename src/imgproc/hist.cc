#include "hist.h"

namespace ncv {

  namespace imgproc {

    NAN_METHOD(CalcBackProject) {
      NotImplemented(info);
    }

    NAN_METHOD(CalcHist) {
      NotImplemented(info);
    }

    NAN_METHOD(CompareHist) {
		FUNCTION_REQUIRE_ARGUMENTS(3);
		ASSERT_INPUTARRAY_FROM_ARGS(H1, 0);
		ASSERT_INPUTARRAY_FROM_ARGS(H2, 1);
		ASSERT_INTARRAY_FROM_ARGS(method, 2);
		TRY_CATCH_THROW_OPENCV(cv:compareHist(H1, H2, method));
    }

    NAN_METHOD(EMD) {
      NotImplemented(info);
    }

    NAN_METHOD(EqualizeHist) {
      FUNCTION_REQUIRE_ARGUMENTS(2);
      ASSERT_INPUTARRAY_FROM_ARGS(src, 0);
      ASSERT_OUTPUTARRAY_FROM_ARGS(dst, 1);
      TRY_CATCH_THROW_OPENCV(cv::equalizeHist(src, dst));
    }

    NAN_METHOD(WrapperEMD) {
		FUCTION_REQUIRED_ARGUMENTS_RANGE(3, 6);
		ASSERT_INPUTARRAY_FROM_ARGS(signature1, 0);
		ASSERT_INPUTARRAY_FROM_ARGS(signature2, 1);
		ASSERT_INT_FROM_ARGS(distType, 2);
		DEFAULT_INPUTARRAY_FROM_ARGS(cost, 3, cv::cost = noArray());
		//
    }

    void HistInit(Local<Object> &target) {
      // hist comp methods
      DEFINE_CONST_ENUM(HISTCMP_CORREL);
      DEFINE_CONST_ENUM(HISTCMP_CHISQR);
      DEFINE_CONST_ENUM(HISTCMP_INTERSECT);
      DEFINE_CONST_ENUM(HISTCMP_BHATTACHARYYA);
      DEFINE_CONST_ENUM(HISTCMP_HELLINGER);
      DEFINE_CONST_ENUM(HISTCMP_CHISQR_ALT);
      DEFINE_CONST_ENUM(HISTCMP_KL_DIV);

      Nan::SetMethod(target, "calcBackProject", CalcBackProject);
      Nan::SetMethod(target, "calcHist", CalcHist);
      Nan::SetMethod(target, "compareHist", CompareHist);
      Nan::SetMethod(target, "emd", EMD);
      Nan::SetMethod(target, "equalizeHist", EqualizeHist);
      Nan::SetMethod(target, "wrapperEMD", WrapperEMD);
    }

  }

}
