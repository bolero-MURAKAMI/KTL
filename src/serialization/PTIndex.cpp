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

#include "PTIndex.hpp"

//
// regist_ktl_pt_index, unregist_ktl_pt_index
//
static void regist_ktl_pt_index() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:PTIndex:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddNewMember(global.get(), SPRIG_KRKR_TJS_W("PTIndex"), new ktl::PTIndex());
	}
}
static void unregist_ktl_pt_index() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:PTIndex:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("PTIndex"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_pt_index);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_pt_index);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:PTIndex:information"));
