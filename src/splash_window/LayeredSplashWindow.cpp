
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "LayeredSplashWindow.hpp"

namespace ktl {
	//
	// CreateNativeClassLayeredSplashWindow
	//
#define TJS_NATIVE_CLASSID_NAME LayeredSplashWindowClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassLayeredSplashWindow, LayeredSplashWindow, LayeredSplashWindow)
	//
	//	SUMMARY: 初期化系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(initialize)
	{
		TJS_GET_NATIVE_INSTANCE(this_, LayeredSplashWindow);
		if (numparams >= 1) {
			SPRIG_KRKR_TYPE_CHECK(0, tvtString);
			this_->initialize(SPRIG_KRKR_ARG_STRING(0));
		} else {
			this_->initialize();
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(initialize);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(LayeredSplashWindow, uninitialize);
	//
	//	SUMMARY: 読み込み系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadImages)
	{
		TJS_GET_NATIVE_INSTANCE(this_, LayeredSplashWindow);
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
		TJS_GET_NATIVE_INSTANCE(this_, LayeredSplashWindow);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->loadImagesFromOctet(SPRIG_KRKR_ARG_OCTET(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadImagesFromOctet);
	//
	//	SUMMARY: 更新系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(update)
	{
		TJS_GET_NATIVE_INSTANCE(this_, LayeredSplashWindow);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->update(
				SPRIG_KRKR_ENABLE_ARG(0)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(0))
					: boost::none
					,
				SPRIG_KRKR_ENABLE_ARG(1)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(1))
					: boost::none
					,
				SPRIG_KRKR_ENABLE_ARG(2)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(2))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(update);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(updateToCenter)
	{
		TJS_GET_NATIVE_INSTANCE(this_, LayeredSplashWindow);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->updateToCenter(
				SPRIG_KRKR_ENABLE_ARG(0)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(0))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(updateToCenter);
	//
	//	SUMMARY: プロパテイ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(LayeredSplashWindow, visible, getVisible, setVisible);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(LayeredSplashWindow, topMost, getTopMost, setTopMost);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(LayeredSplashWindow, imageWidth, getImageWidth);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(LayeredSplashWindow, imageHeight, getImageHeight);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(LayeredSplashWindow, left, getLeft);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(LayeredSplashWindow, top, getTop);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(LayeredSplashWindow, width, getWidth);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(LayeredSplashWindow, height, getHeight);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(LayeredSplashWindow, opacity, getOpacity);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(LayeredSplashWindow, identity, getIdentity);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(LayeredSplashWindow, initialized, initialized);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

namespace ktl {
	LayeredSplash::instance_map_type LayeredSplash::instance_map_;
}	// namespace ktl

//
// regist_ktl_layered_window
// unregist_ktl_layered_window
//
static void regist_ktl_layered_window() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:LayeredSplashWindow:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LayeredSplashWindow"), ktl::CreateNativeClassLayeredSplashWindow());
	}
}
static void unregist_ktl_layered_window() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:LayeredSplashWindow:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LayeredSplashWindow"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_layered_window);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_layered_window);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:LayeredSplashWindow:information"));
