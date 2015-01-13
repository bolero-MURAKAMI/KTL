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

#include "Functor.hpp"

//
// regist_ktl_functor, unregist_ktl_functor
//
static void regist_ktl_functor() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Functor:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("FunctorClass"), new ktl::FunctorClass());
	}
}
static void unregist_ktl_functor() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Functor:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("FunctorClass"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_functor);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_functor);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Functor:information"));
