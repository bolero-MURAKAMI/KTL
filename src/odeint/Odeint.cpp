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

#include "Odeint.hpp"

namespace ktl {
	//
	// CreateNativeClassOdeint
	//
#define TJS_NATIVE_CLASSID_NAME OdeintClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassOdeint, Odeint, Odeint)
	//
	//	SUMMARY: Ï•ªŒnƒƒ\ƒbƒh
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(integrate)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(5);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtObject);
		if (numparams >= 6) {
			SPRIG_KRKR_TYPE_CHECK(5, tvtObject);
			SPRIG_KRKR_RESULT_SET(
				Odeint::integrate(
					SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_CLOSURE(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3), SPRIG_KRKR_ARG_VARIANT(4),
					SPRIG_KRKR_ARG_CLOSURE(5)
					)
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Odeint::integrate(
					SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_CLOSURE(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3), SPRIG_KRKR_ARG_VARIANT(4)
					)
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(integrate);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_odeint
// unregist_ktl_odeint
//
static void regist_ktl_odeint() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Odeint:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Odeint"), ktl::CreateNativeClassOdeint());
	}
}
static void unregist_ktl_odeint() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Odeint:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Odeint"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_odeint);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_odeint);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Odeint:information"));
