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

#include "FPSCounter.hpp"

namespace ktl {
	//
	// CreateNativeClassFPSCounter
	//
#define TJS_NATIVE_CLASSID_NAME FPSCounterClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassFPSCounter, FPSCounter, FPSCounter)
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(FPSCounter, enable, getEnable, setEnable);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(FPSCounter, run, getRun, setRun);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_fps_counter
// unregist_ktl_fps_counter
//
static void regist_ktl_fps_counter() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:FPSCounter:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("FPSCounter"), ktl::CreateNativeClassFPSCounter());
	}
}
static void unregist_ktl_fps_counter() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:FPSCounter:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("FPSCounter"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_fps_counter);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_fps_counter);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:FPSCounter:information"));
