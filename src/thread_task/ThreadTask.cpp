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

#include "ThreadTask.hpp"

namespace ktl {
	//
	// CreateNativeClassThreadTask
	//
#define TJS_NATIVE_CLASSID_NAME ThreadTaskClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassThreadTask, ThreadTask, ThreadTask)
	//
	//	SUMMARY: 制御系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(start)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ThreadTask);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->start(SPRIG_KRKR_ARG_CLOSURE(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(start);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(cancel)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ThreadTask);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->cancel()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(cancel);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(ThreadTask, result, result);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(ThreadTask, isProcessing, isProcessing);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(ThreadTask, cancelled, cancelled);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
	//
	// CreateNativeClassThreadControl
	//
#define TJS_NATIVE_CLASSID_NAME ThreadControlClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassThreadControl, ThreadControl, ThreadControl)
	//
	//	SUMMARY: 内部系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(ThreadControl, instance, getInstance);
	//
	//	SUMMARY: 制御系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(yield)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ThreadControl);
		this_->yield();
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(yield);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(sleep)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ThreadControl);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		this_->sleep(SPRIG_KRKR_ARG_VARIANT(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(sleep);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(ThreadControl, cancelled, cancelled);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_thread_task
// unregist_ktl_thread_task
//
static void regist_ktl_thread_task() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:ThreadTask:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("ThreadTask"), ktl::CreateNativeClassThreadTask());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("ThreadControl"), ktl::CreateNativeClassThreadControl());
	}
}
static void unregist_ktl_thread_task() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:ThreadTask:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("ThreadControl"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("ThreadTask"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_thread_task);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_thread_task);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:ThreadTask:information"));
