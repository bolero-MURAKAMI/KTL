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

#include "VariateRandom.hpp"

namespace ktl {
	//
	// CreateNativeClassVariateRandom
	//
#define TJS_NATIVE_CLASSID_NAME VariateRandomClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassVariateRandom, VariateRandom, VariateRandom)
	//
	//	SUMMARY: 取得系メソッド
	//
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VARIANT(VariateRandom, random);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(VariateRandom, engine, engine);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(VariateRandom, distribution, distribution);
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(VariateRandom, min, min);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(VariateRandom, max, max);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_variate_random
// unregist_ktl_variate_random
//
static void regist_ktl_variate_random() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:VariateRandom:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("VariateRandom"), ktl::CreateNativeClassVariateRandom());
	}
}
static void unregist_ktl_variate_random() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:VariateRandom:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("VariateRandom"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_variate_random);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_variate_random);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:VariateRandom:information"));
