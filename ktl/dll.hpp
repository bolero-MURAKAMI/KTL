/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef KTL_DLL_HPP
#define KTL_DLL_HPP

#include <sprig/config/config.hpp>

#ifdef SPRIG_USING_PRAGMA_ONCE
#	pragma once
#endif	// #ifdef SPRIG_USING_PRAGMA_ONCE

#include <ktl/config.hpp>

#include <boost/preprocessor/wstringize.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/information.hpp>

//
// KTL_VERSION_NUMBER
// KTL_VERSION_STRING
//
#define KTL_VERSION_NUMBER(major, minor, patch, patch_minor) \
	((((major) % 100) * 10000000) + (((minor) % 100) * 100000) + (((patch) % 100) * 1000) + ((patch_minor) % 1000))
#define KTL_VERSION_STRING(major, minor, patch, patch_minor) \
	BOOST_PP_WSTRINGIZE(major) SPRIG_KRKR_TJS_W(".") \
	BOOST_PP_WSTRINGIZE(minor) SPRIG_KRKR_TJS_W(".") \
	BOOST_PP_WSTRINGIZE(patch) SPRIG_KRKR_TJS_W(".") \
	BOOST_PP_WSTRINGIZE(patch_minor)
//
// KTL_VERSION_NUMBER_X
// KTL_VERSION_STRING_X
//
#define KTL_VERSION_NUMBER_X_I(x) KTL_VERSION_NUMBER x
#define KTL_VERSION_NUMBER_X(x) \
	KTL_VERSION_NUMBER_X_I((x))
#define KTL_VERSION_STRING_X_I(x) KTL_VERSION_STRING x
#define KTL_VERSION_STRING_X(x) \
	KTL_VERSION_STRING_X_I((x))

//
// KTL_DLL_DEBUG
//
#ifdef NDEBUG
#	define KTL_DLL_DEBUG 0
#else	// #ifdef NDEBUG
#	define KTL_DLL_DEBUG 1
#endif	// #ifdef NDEBUG
//
// KTL_DLL_PRODUCT
//
#ifdef NDEBUG
#	define KTL_DLL_PRODUCT \
		SPRIG_KRKR_TJS_W("KTL.") SPRIG_KRKR_TJS_W(KTL_RC_MODULE_NAME) \
		SPRIG_KRKR_TJS_W(" version ") KTL_VERSION_STRING_X(KTL_RC_VERSION) \
		SPRIG_KRKR_TJS_W(" Release mode")
#else	// #ifdef NDEBUG
#	define KTL_DLL_PRODUCT \
		SPRIG_KRKR_TJS_W("KTL.") SPRIG_KRKR_TJS_W(KTL_RC_MODULE_NAME) \
		SPRIG_KRKR_TJS_W(" version ") KTL_VERSION_STRING_X(KTL_RC_VERSION) \
		SPRIG_KRKR_TJS_W(" Debug mode")
#endif	// #ifdef NDEBUG

//
// regist_ktl_local_files
// unregist_ktl_local_files
//
static void regist_ktl_dll() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:DLL:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("product"), KTL_DLL_PRODUCT, SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::object_type ktl_dll;
		{
			tTJSVariant var;
			tjs_error error = global->PropGet(TJS_IGNOREPROP, SPRIG_KRKR_TJS_W("ktldll"), 0, &var, global.get());
			if (error != TJS_E_MEMBERNOTFOUND && TJS_FAILED(error)) {
				::TVPThrowExceptionMessage(SPRIG_KRKR_TJS_W("error in iTJSDispatch2::PropGet()"));
			}
			if (var.Type() != tvtVoid) {
				ktl_dll = sprig::krkr::tjs::object_type(var.AsObjectNoAddRef());
			}
		}
		if (!ktl_dll) {
			{
				iTJSDispatch2* obj = 0;
				sprig::krkr::tjs::CreateNewObject(
					sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
					&obj, 0, 0, 0
					);
				ktl_dll = sprig::krkr::tjs::object_type(obj, false);
			}
			{
				tTJSVariant var(ktl_dll.get());
				sprig::krkr::tjs::AddMemberNoRelease(global.get(), SPRIG_KRKR_TJS_W("ktldll"), ktl_dll.get());
			}
		}
		sprig::krkr::tjs::object_type module;
		{
			iTJSDispatch2* obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
				&obj, 0, 0, 0
				);
			module = sprig::krkr::tjs::object_type(obj, false);
		}
		{
			tTJSVariant var(KTL_VERSION_NUMBER_X(KTL_RC_VERSION));
			sprig::krkr::tjs::AddMember(module.get(), SPRIG_KRKR_TJS_W("version"), &var);
		}
		{
			tTJSVariant var(KTL_DLL_DEBUG);
			sprig::krkr::tjs::AddMember(module.get(), SPRIG_KRKR_TJS_W("debug"), &var);
		}
		{
			tTJSVariant var(SPRIG_KRKR_TJS_W(__DATE__));
			sprig::krkr::tjs::AddMember(module.get(), SPRIG_KRKR_TJS_W("date"), &var);
		}
		{
			tTJSVariant var(SPRIG_KRKR_TJS_W(__TIME__));
			sprig::krkr::tjs::AddMember(module.get(), SPRIG_KRKR_TJS_W("time"), &var);
		}
		{
			tTJSVariant var(KTL_DLL_PRODUCT);
			sprig::krkr::tjs::AddMember(module.get(), SPRIG_KRKR_TJS_W("product"), &var);
		}
		sprig::krkr::tjs::AddMemberNoRelease(ktl_dll.get(), SPRIG_KRKR_TJS_W(KTL_RC_PROJECT_NAME), module.get());
	}
}
static void unregist_ktl_dll() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:DLL:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("product"), KTL_DLL_PRODUCT, SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
}
NCB_POST_REGIST_CALLBACK(regist_ktl_dll);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_dll);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:DLL:information"));

#endif	// #ifndef KTL_DLL_HPP
