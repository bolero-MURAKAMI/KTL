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

#include "PerformanceCounter.hpp"

namespace ktl {
	//
	// CreateNativeClassPerformanceCounter
	//
#define TJS_NATIVE_CLASSID_NAME PerformanceCounterClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassPerformanceCounter, PerformanceCounter, PerformanceCounter)
	//
	//	SUMMARY: 出力系メソッド
	//
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(PerformanceCounter, begin);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(PerformanceCounter, end);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(PerformanceCounter, enable, getEnable, setEnable);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_performance_counter
// unregist_ktl_performance_counter
//
static void regist_ktl_performance_counter() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:PerformanceCounter:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddNewMember(global.get(), SPRIG_KRKR_TJS_W("PerformanceCounter"), ktl::CreateNativeClassPerformanceCounter());
	}
}
static void unregist_ktl_performance_counter() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:PerformanceCounter:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("PerformanceCounter"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_performance_counter);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_performance_counter);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:PerformanceCounter:information"));
