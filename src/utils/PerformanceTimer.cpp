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

#include "PerformanceTimer.hpp"

namespace ktl {
	//
	// CreateNativeClassPerformanceTimer
	//
#define TJS_NATIVE_CLASSID_NAME PerformanceTimerClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassPerformanceTimer, PerformanceTimer, PerformanceTimer)
	//
	//	SUMMARY: 操作系メソッド
	//
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(PerformanceTimer, restart);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(PerformanceTimer, elapsed, elapsed);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_performance_timer
// unregist_ktl_performance_timer
//
static void regist_ktl_performance_timer() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:PerformanceTimer:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddNewMember(global.get(), SPRIG_KRKR_TJS_W("PerformanceTimer"), ktl::CreateNativeClassPerformanceTimer());
	}
}
static void unregist_ktl_performance_timer() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:PerformanceTimer:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("PerformanceTimer"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_performance_timer);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_performance_timer);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:PerformanceTimer:information"));
