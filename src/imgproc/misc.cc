#include "misc.h"
#include "../core/Point.h"
#include "../core/Scalar.h"
#include "../core/Rect.h"

namespace ncv {

  namespace imgproc {

    NAN_METHOD(AdaptiveThreshold) {
      FUNCTION_REQUIRE_ARGUMENTS(7);
      Nan::HandleScope scope;
      ASSERT_INPUTARRAY_FROM_ARGS(src, 0);
      ASSERT_OUTPUTARRAY_FROM_ARGS(dst, 1);
      ASSERT_DOUBLE_FROM_ARGS(maxval, 2);
      ASSERT_INT_FROM_ARGS(adaptiveMethod, 3);
      ASSERT_INT_FROM_ARGS(adaptiveType, 4);
      ASSERT_INT_FROM_ARGS(blockSize, 5);   
      ASSERT_DOUBLE_FROM_ARGS(C, 6);
      TRY_CATCH_THROW_OPENCV(cv::adaptiveThreshold(src, dst, maxval, adaptiveMethod, adaptiveType, blockSize, C));
    }

    NAN_METHOD(CvtColor) {
      FUNCTION_REQUIRE_ARGUMENTS_RANGE(3, 4);
      Nan::HandleScope scope;
      ASSERT_INPUTARRAY_FROM_ARGS(src, 0);
      ASSERT_OUTPUTARRAY_FROM_ARGS(dst, 1);
      ASSERT_INT_FROM_ARGS(code, 2);
      DEFAULT_INT_FROM_ARGS(dstCn, 3, 0);
      TRY_CATCH_THROW_OPENCV(cv::cvtColor(src, dst, code, dstCn));
    }

    NAN_METHOD(DistanceTransform) {
		FUNCTION_REQUIRE_ARGUMENTS_RANGE(5, 6);
		ASSERT_INPUTARRAY_FROM_ARGS(src, 0);
		ASSERT_OUTPUTARRAY_FROM_ARGS(dst, 1);
		ASSERT_OUTPUTARRAY_FROM_ARGS(labels, 2);
		ASSERT_INPUTARRAY_FROM_ARGS(distanceType, 3);
		ASSERT_INPUTARRAY_FROM_ARGS(maskSize, 4);
		DEFAULT_INPUTARRAY_FROM_ARGS(labelType, 5, cv::DIST_LABEL_CCOMP);   // cv:: is not available
		TRY_CATCH_THROW_OPENCV(cv::DistanceTransform(src, dst, labels, distanceType, maskSize, labelType));
    }

    NAN_METHOD(FloodFill) {
		FUNCTION_REQUIRE_ARGUMENTS_RANGE(3, 7);
		ASSERT_INPUTOUTPUTARRAY_FROM_ARGS(image, 0);
		cv::_InputOutputArray mask;
		int argumentsOffset = 0;
		if (Point::HasInstance(info[1])) {
			ASSERT_INPUTOUTPUTARRAY_FROM_ARGS(_mask, 1);
			mask = _mask;
		}
		ASSERT_POINT_FROM_ARGS(seedPoint, 1 + argumentsOffset);
		ASSERT_SCALAR_FROM_ARGS(newVal, 2 + argumentsOffset);
		DEFAULT_SCALAR_FROM_ARGS(loDiff, 3 + argumentsOffset, cv::Scalar());
		DEFAULT_SCALAR_FROM_ARGS(upDiff, 4 + argumentsOffset, cv::Scalar());
		DEFAULT_INT_FROM_ARGS(flags, 5 + argumentsOffset, 4);

		cv::Rect rect;
		int result;
		TRY_CATCH_THROW_OPENCV(result = cv::floodFill(image, mask, seedPoint, newVal, &rect, loDiff, upDiff, flags));

		Local<Object> ret = Nan::New<Object>();
		Nan::Set(ret, Nan::New<String>("result").ToLocalChecked(), Nan::New<Number>(result));
		Nan::Set(ret, Nan::New<String>("rect").ToLocalChecked(), Rect::NewInstance(rect));

		info.GetReturnValue().Set(ret);
    }

    NAN_METHOD(GrabCut) {
		FUNCTION_REQUIRE_ARGUMENTS_RANGE(6, 7);
		ASSERT_INPUTARRAY_FROM_ARGS(img, 0);
		ASSERT_INPUTOUTPUTARRAY_FROM_ARGS(mask, 1);
		ASSERT_RECT_FROM_ARGS(rect, 2);
		ASSERT_INPUTOUTPUTARRAY_FROM_ARGS(bgdModel, 3);
		ASSERT_INPUTOUTPUTARRAY_FROM_ARGS(fgdModel, 4);
		ASSERT_INT_FROM_ARGS(iterCount, 5);
		DEFAULT_INT_FROM_ARGS(mode, 6, cv::GC_EVAL);
		TRY_CATCH_THROW_OPENCV(cv::grabCut(img, mask, rect, bgdModel, fgdModel, iterCount, mode));
		
    }

    NAN_METHOD(Integral) {
		FUNCTION_REQUIRE_ARGUMENTS_RANGE(2, 6);
		ASSERT_INPUTARRAY_FROM_ARGS(src, 0);
		ASSERT_OUTPUTARRAY_FROM_ARGS(sum, 1);
		cv::_OutputArray sqsum = cv::noArray(), tilted = cv::noArray();
		int argumentOffset = 0;
		if (info.Length() > 2 && !info[2]->IsNumber()) {
			ASSERT_OUTPUTARRAY_FROM_ARGS(_sqsum, 2);
			sqsum = _sqsum;
			argumentOffset += 1;
		}
		if (info.Length() > 3 && !info[3]->IsNumber()) {
			ASSERT_OUTPUTARRAY_FROM_ARGS(_tilted, 3);
			tilted = _tilted;
			argumentOffset += 1;
		}
		DEFAULT_INT_FROM_ARGS(sdepth, 2 + argumentOffset, -1);
		DEFAULT_INT_FROM_ARGS(sdepth, 3 + argumentOffset, -1);
		TRY_CATCH_THROW_OPENCV(cv::Integral(src, sum, sqsum, tilted, sdepth, sqdepth));
		/*
		//Type2
		FUCNTION_REQUIRE_ARGUMENTS_RANGE(3, 5);
		ASSERT_INPUTARRAY_FROM_ARGS(src,0);
		ASSERT_OUTPUTARRAY_FROM_ARGS(sum,1);
		ASSERT_OUTPUTARRAY_FROM_ARGS(sqsum,2);
		DEFAULT_INT_FROM_ARGS(sdepth,3,cv::sdepth(-1));
		DEFAULT_INT_FROM_ARGS(sdepth,3,cv::sqdepth(-1));
		TRY_CATCH_THROW_OPENCV(cv::Integral(src, sum, sqsum, sdepth, sqdepth));

		*/
		/*
		//Type 3
		FUNCTION_REQUIRE_ARGUMENTS_RANGE(4, 6);
		ASSERT_INPUTARRAY_FROM_ARGS(src ,0);
	    ASSERT_OUTPUTARRAY_FROM_ARGS(sum ,1);
		ASSERT_OUTPUTARRAY_FROM_ARGS(sqsum ,2);
		ASSERT_OUTPUTARRAY_FROM_ARGS(tilted, 3);
		DEFALUT_INT_FROM_ARGS(sdepth, 4, cv::sdepth(-1));
		DEFALUT_INT_FROM_ARGS(sqdepth, 4, cv::sqdepth(-1));
		TRY_CATCH_THROW_OPENCV(cv::Integral(src, sum, sqsum, tilted, sdepth, sqdepth ));

		
		*/



		
		
		
		
		
		
		
		
	}

    NAN_METHOD(Threshold) {
      FUNCTION_REQUIRE_ARGUMENTS(5);
      Nan::HandleScope scope;
      ASSERT_INPUTARRAY_FROM_ARGS(src, 0);
      ASSERT_OUTPUTARRAY_FROM_ARGS(dst, 1);
      ASSERT_DOUBLE_FROM_ARGS(thresh, 2);
      ASSERT_DOUBLE_FROM_ARGS(maxval, 3);
      ASSERT_INT_FROM_ARGS(type, 4);
      TRY_CATCH_THROW_OPENCV(info.GetReturnValue().Set(Nan::New<Number>(cv::threshold(src, dst, thresh, maxval, type))));
    }

    NAN_METHOD(Watershed) {
      FUNCTION_REQUIRE_ARGUMENTS(2);
      Nan::HandleScope scope;
      ASSERT_INPUTARRAY_FROM_ARGS(image, 0);
      ASSERT_INPUTOUTPUTARRAY_FROM_ARGS(markers, 1);
      TRY_CATCH_THROW_OPENCV(cv::watershed(image, markers));
    }

    void MiscInit(Local<Object> &target) {
      // adaptive threshold types
      DEFINE_CONST_ENUM(ADAPTIVE_THRESH_MEAN_C);
      DEFINE_CONST_ENUM(ADAPTIVE_THRESH_GAUSSIAN_C);

      // color conversion codes
      DEFINE_CONST_ENUM(COLOR_BGR2BGRA);
      DEFINE_CONST_ENUM(COLOR_RGB2RGBA);
      DEFINE_CONST_ENUM(COLOR_BGRA2BGR);
      DEFINE_CONST_ENUM(COLOR_RGBA2RGB);
      DEFINE_CONST_ENUM(COLOR_BGR2RGBA);
      DEFINE_CONST_ENUM(COLOR_RGB2BGRA);
      DEFINE_CONST_ENUM(COLOR_RGBA2BGR);
      DEFINE_CONST_ENUM(COLOR_BGRA2RGB);
      DEFINE_CONST_ENUM(COLOR_BGR2RGB);
      DEFINE_CONST_ENUM(COLOR_RGB2BGR);
      DEFINE_CONST_ENUM(COLOR_BGRA2RGBA);
      DEFINE_CONST_ENUM(COLOR_RGBA2BGRA);
      DEFINE_CONST_ENUM(COLOR_BGR2GRAY);
      DEFINE_CONST_ENUM(COLOR_RGB2GRAY);
      DEFINE_CONST_ENUM(COLOR_GRAY2BGR);
      DEFINE_CONST_ENUM(COLOR_GRAY2RGB);
      DEFINE_CONST_ENUM(COLOR_GRAY2BGRA);
      DEFINE_CONST_ENUM(COLOR_GRAY2RGBA);
      DEFINE_CONST_ENUM(COLOR_BGRA2GRAY);
      DEFINE_CONST_ENUM(COLOR_RGBA2GRAY);
      DEFINE_CONST_ENUM(COLOR_BGR2BGR565);
      DEFINE_CONST_ENUM(COLOR_RGB2BGR565);
      DEFINE_CONST_ENUM(COLOR_BGR5652BGR);
      DEFINE_CONST_ENUM(COLOR_BGR5652RGB);
      DEFINE_CONST_ENUM(COLOR_BGRA2BGR565);
      DEFINE_CONST_ENUM(COLOR_RGBA2BGR565);
      DEFINE_CONST_ENUM(COLOR_BGR5652BGRA);
      DEFINE_CONST_ENUM(COLOR_BGR5652RGBA);
      DEFINE_CONST_ENUM(COLOR_GRAY2BGR565);
      DEFINE_CONST_ENUM(COLOR_BGR5652GRAY);
      DEFINE_CONST_ENUM(COLOR_BGR2BGR555);
      DEFINE_CONST_ENUM(COLOR_RGB2BGR555);
      DEFINE_CONST_ENUM(COLOR_BGR5552BGR);
      DEFINE_CONST_ENUM(COLOR_BGR5552RGB);
      DEFINE_CONST_ENUM(COLOR_BGRA2BGR555);
      DEFINE_CONST_ENUM(COLOR_RGBA2BGR555);
      DEFINE_CONST_ENUM(COLOR_BGR5552BGRA);
      DEFINE_CONST_ENUM(COLOR_BGR5552RGBA);
      DEFINE_CONST_ENUM(COLOR_GRAY2BGR555);
      DEFINE_CONST_ENUM(COLOR_BGR5552GRAY);
      DEFINE_CONST_ENUM(COLOR_BGR2XYZ);
      DEFINE_CONST_ENUM(COLOR_RGB2XYZ);
      DEFINE_CONST_ENUM(COLOR_XYZ2BGR);
      DEFINE_CONST_ENUM(COLOR_XYZ2RGB);
      DEFINE_CONST_ENUM(COLOR_BGR2YCrCb);
      DEFINE_CONST_ENUM(COLOR_RGB2YCrCb);
      DEFINE_CONST_ENUM(COLOR_YCrCb2BGR);
      DEFINE_CONST_ENUM(COLOR_YCrCb2RGB);
      DEFINE_CONST_ENUM(COLOR_BGR2HSV);
      DEFINE_CONST_ENUM(COLOR_RGB2HSV);
      DEFINE_CONST_ENUM(COLOR_BGR2Lab);
      DEFINE_CONST_ENUM(COLOR_RGB2Lab);
      DEFINE_CONST_ENUM(COLOR_BGR2Luv);
      DEFINE_CONST_ENUM(COLOR_RGB2Luv);
      DEFINE_CONST_ENUM(COLOR_BGR2HLS);
      DEFINE_CONST_ENUM(COLOR_RGB2HLS);
      DEFINE_CONST_ENUM(COLOR_HSV2BGR);
      DEFINE_CONST_ENUM(COLOR_HSV2RGB);
      DEFINE_CONST_ENUM(COLOR_Lab2BGR);
      DEFINE_CONST_ENUM(COLOR_Lab2RGB);
      DEFINE_CONST_ENUM(COLOR_Luv2BGR);
      DEFINE_CONST_ENUM(COLOR_Luv2RGB);
      DEFINE_CONST_ENUM(COLOR_HLS2BGR);
      DEFINE_CONST_ENUM(COLOR_HLS2RGB);
      DEFINE_CONST_ENUM(COLOR_BGR2HSV_FULL);
      DEFINE_CONST_ENUM(COLOR_RGB2HSV_FULL);
      DEFINE_CONST_ENUM(COLOR_BGR2HLS_FULL);
      DEFINE_CONST_ENUM(COLOR_RGB2HLS_FULL);
      DEFINE_CONST_ENUM(COLOR_HSV2BGR_FULL);
      DEFINE_CONST_ENUM(COLOR_HSV2RGB_FULL);
      DEFINE_CONST_ENUM(COLOR_HLS2BGR_FULL);
      DEFINE_CONST_ENUM(COLOR_HLS2RGB_FULL);
      DEFINE_CONST_ENUM(COLOR_LBGR2Lab);
      DEFINE_CONST_ENUM(COLOR_LRGB2Lab);
      DEFINE_CONST_ENUM(COLOR_LBGR2Luv);
      DEFINE_CONST_ENUM(COLOR_LRGB2Luv);
      DEFINE_CONST_ENUM(COLOR_Lab2LBGR);
      DEFINE_CONST_ENUM(COLOR_Lab2LRGB);
      DEFINE_CONST_ENUM(COLOR_Luv2LBGR);
      DEFINE_CONST_ENUM(COLOR_Luv2LRGB);
      DEFINE_CONST_ENUM(COLOR_BGR2YUV);
      DEFINE_CONST_ENUM(COLOR_RGB2YUV);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_NV12);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_NV12);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_NV21);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_NV21);
      DEFINE_CONST_ENUM(COLOR_YUV420sp2RGB);
      DEFINE_CONST_ENUM(COLOR_YUV420sp2BGR);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_NV12);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_NV12);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_NV21);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_NV21);
      DEFINE_CONST_ENUM(COLOR_YUV420sp2RGBA);
      DEFINE_CONST_ENUM(COLOR_YUV420sp2BGRA);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_YV12);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_YV12);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_IYUV);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_IYUV);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_I420);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_I420);
      DEFINE_CONST_ENUM(COLOR_YUV420p2RGB);
      DEFINE_CONST_ENUM(COLOR_YUV420p2BGR);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_YV12);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_YV12);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_IYUV);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_IYUV);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_I420);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_I420);
      DEFINE_CONST_ENUM(COLOR_YUV420p2RGBA);
      DEFINE_CONST_ENUM(COLOR_YUV420p2BGRA);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_420);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_NV21);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_NV12);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_YV12);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_IYUV);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_I420);
      DEFINE_CONST_ENUM(COLOR_YUV420sp2GRAY);
      DEFINE_CONST_ENUM(COLOR_YUV420p2GRAY);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_UYVY);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_UYVY);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_Y422);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_Y422);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_UYNV);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_UYNV);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_UYVY);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_UYVY);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_Y422);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_Y422);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_UYNV);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_UYNV);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_YUY2);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_YUY2);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_YVYU);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_YVYU);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_YUYV);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_YUYV);
      DEFINE_CONST_ENUM(COLOR_YUV2RGB_YUNV);
      DEFINE_CONST_ENUM(COLOR_YUV2BGR_YUNV);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_YUY2);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_YUY2);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_YVYU);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_YVYU);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_YUYV);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_YUYV);
      DEFINE_CONST_ENUM(COLOR_YUV2RGBA_YUNV);
      DEFINE_CONST_ENUM(COLOR_YUV2BGRA_YUNV);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_UYVY);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_YUY2);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_Y422);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_UYNV);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_YVYU);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_YUYV);
      DEFINE_CONST_ENUM(COLOR_YUV2GRAY_YUNV);
      DEFINE_CONST_ENUM(COLOR_RGBA2mRGBA);
      DEFINE_CONST_ENUM(COLOR_mRGBA2RGBA);
      DEFINE_CONST_ENUM(COLOR_RGB2YUV_I420);
      DEFINE_CONST_ENUM(COLOR_BGR2YUV_I420);
      DEFINE_CONST_ENUM(COLOR_RGB2YUV_IYUV);
      DEFINE_CONST_ENUM(COLOR_BGR2YUV_IYUV);
      DEFINE_CONST_ENUM(COLOR_RGBA2YUV_I420);
      DEFINE_CONST_ENUM(COLOR_BGRA2YUV_I420);
      DEFINE_CONST_ENUM(COLOR_RGBA2YUV_IYUV);
      DEFINE_CONST_ENUM(COLOR_BGRA2YUV_IYUV);
      DEFINE_CONST_ENUM(COLOR_RGB2YUV_YV12);
      DEFINE_CONST_ENUM(COLOR_BGR2YUV_YV12);
      DEFINE_CONST_ENUM(COLOR_RGBA2YUV_YV12);
      DEFINE_CONST_ENUM(COLOR_BGRA2YUV_YV12);
      DEFINE_CONST_ENUM(COLOR_BayerBG2BGR);
      DEFINE_CONST_ENUM(COLOR_BayerGB2BGR);
      DEFINE_CONST_ENUM(COLOR_BayerRG2BGR);
      DEFINE_CONST_ENUM(COLOR_BayerGR2BGR);
      DEFINE_CONST_ENUM(COLOR_BayerBG2RGB);
      DEFINE_CONST_ENUM(COLOR_BayerGB2RGB);
      DEFINE_CONST_ENUM(COLOR_BayerRG2RGB);
      DEFINE_CONST_ENUM(COLOR_BayerGR2RGB);
      DEFINE_CONST_ENUM(COLOR_BayerBG2GRAY);
      DEFINE_CONST_ENUM(COLOR_BayerGB2GRAY);
      DEFINE_CONST_ENUM(COLOR_BayerRG2GRAY);
      DEFINE_CONST_ENUM(COLOR_BayerGR2GRAY);
      DEFINE_CONST_ENUM(COLOR_BayerBG2BGR_VNG);
      DEFINE_CONST_ENUM(COLOR_BayerGB2BGR_VNG);
      DEFINE_CONST_ENUM(COLOR_BayerRG2BGR_VNG);
      DEFINE_CONST_ENUM(COLOR_BayerGR2BGR_VNG);
      DEFINE_CONST_ENUM(COLOR_BayerBG2RGB_VNG);
      DEFINE_CONST_ENUM(COLOR_BayerGB2RGB_VNG);
      DEFINE_CONST_ENUM(COLOR_BayerRG2RGB_VNG);
      DEFINE_CONST_ENUM(COLOR_BayerGR2RGB_VNG);
      DEFINE_CONST_ENUM(COLOR_BayerBG2BGR_EA);
      DEFINE_CONST_ENUM(COLOR_BayerGB2BGR_EA);
      DEFINE_CONST_ENUM(COLOR_BayerRG2BGR_EA);
      DEFINE_CONST_ENUM(COLOR_BayerGR2BGR_EA);
      DEFINE_CONST_ENUM(COLOR_BayerBG2RGB_EA);
      DEFINE_CONST_ENUM(COLOR_BayerGB2RGB_EA);
      DEFINE_CONST_ENUM(COLOR_BayerRG2RGB_EA);
      DEFINE_CONST_ENUM(COLOR_BayerGR2RGB_EA);
      DEFINE_CONST_ENUM(COLOR_BayerBG2BGRA);
      DEFINE_CONST_ENUM(COLOR_BayerGB2BGRA);
      DEFINE_CONST_ENUM(COLOR_BayerRG2BGRA);
      DEFINE_CONST_ENUM(COLOR_BayerGR2BGRA);
      DEFINE_CONST_ENUM(COLOR_BayerBG2RGBA);
      DEFINE_CONST_ENUM(COLOR_BayerGB2RGBA);
      DEFINE_CONST_ENUM(COLOR_BayerRG2RGBA);
      DEFINE_CONST_ENUM(COLOR_BayerGR2RGBA);
      DEFINE_CONST_ENUM(COLOR_COLORCVT_MAX);

      // distance transform label types
      DEFINE_CONST_ENUM(DIST_LABEL_CCOMP);
      DEFINE_CONST_ENUM(DIST_LABEL_PIXEL);

      // distance transform masks
      DEFINE_CONST_ENUM(DIST_MASK_3);
      DEFINE_CONST_ENUM(DIST_MASK_5);
      DEFINE_CONST_ENUM(DIST_MASK_PRECISE);

      // distance types
      DEFINE_CONST_ENUM(DIST_USER);
      DEFINE_CONST_ENUM(DIST_L1);
      DEFINE_CONST_ENUM(DIST_L2);
      DEFINE_CONST_ENUM(DIST_C);
      DEFINE_CONST_ENUM(DIST_L12);
      DEFINE_CONST_ENUM(DIST_FAIR);
      DEFINE_CONST_ENUM(DIST_WELSCH);
      DEFINE_CONST_ENUM(DIST_HUBER);

      // flood fill flags
      DEFINE_CONST_ENUM(FLOODFILL_FIXED_RANGE);
      DEFINE_CONST_ENUM(FLOODFILL_MASK_ONLY);

      // grab cut classes
      DEFINE_CONST_ENUM(GC_BGD);
      DEFINE_CONST_ENUM(GC_FGD);
      DEFINE_CONST_ENUM(GC_PR_BGD);
      DEFINE_CONST_ENUM(GC_PR_FGD);

      // grab cut modes
      DEFINE_CONST_ENUM(GC_INIT_WITH_RECT);
      DEFINE_CONST_ENUM(GC_INIT_WITH_MASK);
      DEFINE_CONST_ENUM(GC_EVAL);

      // threshold types
      DEFINE_CONST_ENUM(THRESH_BINARY);
      DEFINE_CONST_ENUM(THRESH_BINARY_INV);
      DEFINE_CONST_ENUM(THRESH_TRUNC);
      DEFINE_CONST_ENUM(THRESH_TOZERO);
      DEFINE_CONST_ENUM(THRESH_TOZERO_INV);
      DEFINE_CONST_ENUM(THRESH_MASK);
      DEFINE_CONST_ENUM(THRESH_OTSU);
      DEFINE_CONST_ENUM(THRESH_TRIANGLE);

      // undistort types
      DEFINE_CONST_ENUM(PROJ_SPHERICAL_ORTHO);
      DEFINE_CONST_ENUM(PROJ_SPHERICAL_EQRECT);

      Nan::SetMethod(target, "adaptiveThreshold", AdaptiveThreshold);
      Nan::SetMethod(target, "cvtColor", CvtColor);
      Nan::SetMethod(target, "distanceTransform", DistanceTransform);
      Nan::SetMethod(target, "floodFill", FloodFill);
      Nan::SetMethod(target, "grabCut", GrabCut);
      Nan::SetMethod(target, "integral", Integral);
      Nan::SetMethod(target, "threshold", Threshold);
      Nan::SetMethod(target, "watershed", Watershed);
    }

  }

}
