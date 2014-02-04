/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef KTL_INFORMATION_HPP
#define KTL_INFORMATION_HPP

#include <sprig/config/config.hpp>

#ifdef SPRIG_USING_PRAGMA_ONCE
#	pragma once
#endif	// #ifdef SPRIG_USING_PRAGMA_ONCE

#include <ktl/config.hpp>

#include <boost/preprocessor/cat.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>

//
// KTL_INFORMATION_MODE
//
#ifdef NDEBUG
#	define KTL_INFORMATION_MODE SPRIG_KRKR_TJS_W("Release")
#else	// #ifdef NDEBUG
#	define KTL_INFORMATION_MODE SPRIG_KRKR_TJS_W("Debug")
#endif	// #ifdef NDEBUG

//
// KTL_INFORMATION_FUNCTION
//
#define KTL_INFORMATION_FUNCTION BOOST_PP_CAT(ktl_information_, __LINE__)

//
// KTL_INFORMATION
//
#define KTL_INFORMATION(SECTION) \
	static void KTL_INFORMATION_FUNCTION() { \
		SPRIG_KRKR_SECTION((SECTION), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION); \
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("compiled-date"), SPRIG_KRKR_TJS_W(__DATE__), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION); \
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("compiled-time"), SPRIG_KRKR_TJS_W(__TIME__), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION); \
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("last-updated-timestamp"), SPRIG_KRKR_TJS_W(__TIMESTAMP__), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION); \
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("mode"), KTL_INFORMATION_MODE, SPRIG_KRKR_LOG_LEVEL_NOTIFICATION); \
	} \
	NCB_PRE_REGIST_CALLBACK(KTL_INFORMATION_FUNCTION)

#endif	// #ifndef KTL_INFORMATION_HPP
