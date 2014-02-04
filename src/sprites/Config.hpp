/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_SPRITES_CONFIG_HPP
#define SRC_SPRITES_CONFIG_HPP

#if defined(_MSC_VER)
#	pragma warning(disable : 4819)
#	pragma warning(disable : 4996)
#endif

#pragma warning(disable : 4819)
#pragma warning(disable : 4996)

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#ifndef _WIN32_WINNT
#	define _WIN32_WINNT 0x0501
#endif	// #ifndef _WIN32_WINNT
#ifndef WINVER
#	define WINVER 0x0501
#endif	// #ifndef WINVER

#define SPRIG_CONFIG_LIBINFO_MESSAGE
#define SPRIG_CONFIG_MUST_NOMINMAX

//	CONFIG: D3D
#define SPRIG_CONFIG_LIB_D3DX9_USING_RELEASE_BINARY

//	CONFIG: Sprig.DG
//#define SPRIG_DG_CONFIG_DISABLE_THROW_EXCEPTION
//#define SPRIG_DG_CONFIG_DEVICE_MANAGER_ENABLE_OUTPUT_DEVICE_CAPS

//	CONFIG: KTL.Sprites
//#define KTL_SPRITES_CONFIG_USING_STATIC_POINTER_CAST
//#define KTL_SPRITES_CONFIG_PERFORMANCE_TIMER_ENABLE_DEFAULT

//	CONFIG: user config
#define SPRIG_CONFIG_LIB_USERCONFIG

#endif	// #ifndef SRC_SPRITES_CONFIG_HPP
