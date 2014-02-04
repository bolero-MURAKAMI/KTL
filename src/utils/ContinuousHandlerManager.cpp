/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
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

#include "ContinuousHandlerManager.hpp"

namespace ktl {
	//
	// KTL_CONTINUOUS_HANDLER_MANAGER_NATIVE_GETTER_METHOD_DECL
	// KTL_CONTINUOUS_HANDLER_MANAGER_NATIVE_SETTER_METHOD_DECL
	//
#define KTL_CONTINUOUS_HANDLER_MANAGER_NATIVE_GETTER_METHOD_DECL(getter_name) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getter_name) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, ContinuousHandlerManager); \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_OPTIONAL_RESULT_SET(this_->getter_name(SPRIG_KRKR_ARG_VARIANT(0))); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getter_name)
#define KTL_CONTINUOUS_HANDLER_MANAGER_NATIVE_SETTER_METHOD_DECL(setter_name) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setter_name) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, ContinuousHandlerManager); \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		this_->setter_name(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1)); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setter_name)
	//
	// KTL_CONTINUOUS_HANDLER_MANAGER_NATIVE_PROP_METHOD_DECL
	//
#define KTL_CONTINUOUS_HANDLER_MANAGER_NATIVE_PROP_METHOD_DECL(getter_name, setter_name) \
	KTL_CONTINUOUS_HANDLER_MANAGER_NATIVE_GETTER_METHOD_DECL(getter_name) \
	KTL_CONTINUOUS_HANDLER_MANAGER_NATIVE_SETTER_METHOD_DECL(setter_name)

	//
	// CreateNativeClassContinuousHandlerManager
	//
#define TJS_NATIVE_CLASSID_NAME ContinuousHandlerManagerClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassContinuousHandlerManager, ContinuousHandlerManager, ContinuousHandlerManager)
	//
	//	SUMMARY: ハンドラ管理系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insert)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ContinuousHandlerManager);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		SPRIG_KRKR_INVOKE_RESULT_SET(this_->insert(
			SPRIG_KRKR_ARG_VARIANT(0),
			SPRIG_KRKR_ARG_VARIANT(1),
			SPRIG_KRKR_ARG(2)
			));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(insert);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VARIANT_RES_VOID(ContinuousHandlerManager, erase);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(ContinuousHandlerManager, clear);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VARIANT(ContinuousHandlerManager, size);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(exist)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ContinuousHandlerManager);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_RESULT_SET(this_->exist(SPRIG_KRKR_ARG_VARIANT(0)));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(exist);
	KTL_CONTINUOUS_HANDLER_MANAGER_NATIVE_PROP_METHOD_DECL(getPriority, setPriority);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(find)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ContinuousHandlerManager);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_RESULT_SET(this_->find(SPRIG_KRKR_ARG_VARIANT(0)));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(find);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(ContinuousHandlerManager, run, getRun, setRun);
	//
	//	SUMMARY: デバッグ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(ContinuousHandlerManager, enablePerformanceTimer, getEnablePerformanceTimer, setEnablePerformanceTimer);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_continuous_handler_manager
// unregist_ktl_continuous_handler_manager
//
static void regist_ktl_continuous_handler_manager() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:ContinuousHandlerManager:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("ContinuousHandlerManager"), ktl::CreateNativeClassContinuousHandlerManager());
	}
}
static void unregist_ktl_continuous_handler_manager() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:ContinuousHandlerManager:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("ContinuousHandlerManager"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_continuous_handler_manager);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_continuous_handler_manager);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:ContinuousHandlerManager:information"));
