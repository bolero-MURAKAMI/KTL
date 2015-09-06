/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef KTL_CONFIG_LIB_KTL_TJS_ADAPTER_HPP
#define KTL_CONFIG_LIB_KTL_TJS_ADAPTER_HPP

#ifdef SPRIG_CONFIG_LIB_USERCONFIG
#	include "../../../config/libconfig/ktl_tjs_adapter.hpp"
#endif	// #ifdef SPRIG_CONFIG_LIB_USERCONFIG

#include <sprig/config/pragma_once.hpp>

#ifdef SPRIG_USING_PRAGMA_ONCE
#	pragma once
#endif	// #ifdef SPRIG_USING_PRAGMA_ONCE

#ifdef SPRIG_CONFIG_LIB_AUTO_LINK

#ifndef KTL_LIB_TJS_ADAPTER_BINARY
#	if defined(_MT) && defined(_DLL)
#		ifdef NDEBUG
#			ifdef KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT
#				define KTL_LIB_TJS_ADAPTER_BINARY KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT
#			endif	// #ifdef KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT
#		else	// #ifdef NDEBUG
#			ifdef KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT_GD
#				define KTL_LIB_TJS_ADAPTER_BINARY KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT_GD
#			endif	// #ifdef KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT_GD
#		endif	// #ifdef NDEBUG
#	elif defined(_MT)
#		ifdef NDEBUG
#			ifdef KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT_S
#				define KTL_LIB_TJS_ADAPTER_BINARY KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT_S
#			endif	// #ifdef KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT_S
#		else	// #ifdef NDEBUG
#			ifdef KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT_SGD
#				define KTL_LIB_TJS_ADAPTER_BINARY KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT_SGD
#			endif	// #ifdef KTL_LIBCONFIG_TJS_ADAPTER_LIB_MT_SGD
#		endif	// #ifdef NDEBUG
#	endif
#endif	// #ifndef KTL_LIB_TJS_ADAPTER_BINARY

#include <boost/preprocessor/stringize.hpp>
#include <sprig/config/pragma_comment.hpp>

#ifndef KTL_LIB_BUILD
#	ifdef SPRIG_HAS_PRAGMA_COMMENT
#		ifdef KTL_LIB_TJS_ADAPTER_BINARY
#			pragma comment(lib, BOOST_PP_STRINGIZE(KTL_LIB_TJS_ADAPTER_BINARY))
#		endif	// #ifdef KTL_LIB_TJS_ADAPTER_BINARY
#	endif	// #ifdef SPRIG_HAS_PRAGMA_COMMENT
#endif	// #ifndef KTL_LIB_BUILD

#include <sprig/config/pragma_message.hpp>

#ifdef SPRIG_HAS_PRAGMA_MESSAGE
#	pragma message("ktl/tjs_adapter.hpp")
#	ifdef KTL_LIB_TJS_ADAPTER_BINARY
#		pragma message("    lib=" BOOST_PP_STRINGIZE(KTL_LIB_TJS_ADAPTER_BINARY))
#	endif	// #ifdef KTL_LIB_TJS_ADAPTER_BINARY
#endif	// #ifdef SPRIG_HAS_PRAGMA_MESSAGE

#endif	// #ifdef SPRIG_CONFIG_LIB_AUTO_LINK

#endif	// #ifndef KTL_CONFIG_LIB_KTL_TJS_ADAPTER_HPP
