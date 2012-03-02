
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "TJSEngine.hpp"

namespace ktl {
	//
	// CreateNativeClassTJSEngine
	//
#define TJS_NATIVE_CLASSID_NAME TJSEngineClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassTJSEngine, TJSEngine, TJSEngine)
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_PROP_DECL(global)
	{
		SPRIG_KRKR_BEGIN_NATIVE_PROP_GETTER()
		{
			TJS_GET_NATIVE_INSTANCE(this_, TJSEngine);
			sprig::krkr::tjs::object_type obj = this_->global();
			*result = tTJSVariant(obj.get());
			return TJS_S_OK;
		}
		SPRIG_KRKR_END_NATIVE_PROP_GETTER();
		TJS_DENY_NATIVE_PROP_SETTER
	}
	SPRIG_KRKR_END_NATIVE_PROP_DECL(global);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_tjs_engine, unregist_ktl_tjs_engine
//
static void regist_ktl_tjs_engine() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:TJSEngine:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("TJSEngine"), ktl::CreateNativeClassTJSEngine());
	}
}
static void unregist_ktl_tjs_engine() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:TJSEngine:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("TJSEngine"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_tjs_engine);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_tjs_engine);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:TJSEngine:information"));
