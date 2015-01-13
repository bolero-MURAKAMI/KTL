/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "SplashWindow.hpp"

namespace ktl {
	//
	// CreateNativeClassSplashWindow
	//
#define TJS_NATIVE_CLASSID_NAME SplashWindowClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassSplashWindow, SplashWindow, SplashWindow)
	//
	//	SUMMARY: 初期化系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(initialize)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SplashWindow);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_CHECK(2, tvtString);
			this_->initialize(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_STRING(2));
		} else {
			this_->initialize(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(initialize);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(SplashWindow, uninitialize);
	//
	//	SUMMARY: 読み込み系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadImages)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SplashWindow);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->loadImages(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadImages);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadImagesFromOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SplashWindow);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->loadImagesFromOctet(SPRIG_KRKR_ARG_OCTET(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadImagesFromOctet);
	//
	//	SUMMARY: プロパテイ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SplashWindow, visible, getVisible, setVisible);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SplashWindow, topMost, getTopMost, setTopMost);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SplashWindow, imageWidth, getImageWidth);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SplashWindow, imageHeight, getImageHeight);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SplashWindow, left, getLeft);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SplashWindow, top, getTop);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SplashWindow, width, getWidth);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SplashWindow, height, getHeight);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SplashWindow, identity, getIdentity);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SplashWindow, initialized, initialized);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

namespace ktl {
	Splash::instance_map_type Splash::instance_map_;
}	// namespace ktl

//
// regist_ktl_splash_window
// unregist_ktl_splash_window
//
static void regist_ktl_splash_window() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:SplashWindow:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("SplashWindow"), ktl::CreateNativeClassSplashWindow());
	}
}
static void unregist_ktl_splash_window() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:SplashWindow:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("SplashWindow"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_splash_window);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_splash_window);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:SplashWindow:information"));
