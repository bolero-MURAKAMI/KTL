/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef KTL_RC_VERSION_H
#define KTL_RC_VERSION_H

#include <WinVer.h>

#ifndef KTL_RC_PROJECT_NAME
#	error Requires: define KTL_RC_PROJECT_NAME
#endif	// #ifndef KTL_RC_PROJECT_NAME

#ifndef KTL_RC_MODULE_NAME
#	error Requires: define KTL_RC_MODULE_NAME
#endif	// #ifndef KTL_RC_MODULE_NAME

#ifndef KTL_RC_VERSION
#	define KTL_RC_VERSION					0,0,0,0
#endif	// #ifndef KTL_RC_VERSION

#define KTL_FULL_PROJECT_NAME				"ktl_" KTL_RC_PROJECT_NAME
#define KTL_FULL_MODULE_NAME				"KTL." KTL_RC_MODULE_NAME

#ifdef NDEBUG
#	define KTL_COMMENTS_BUILD				"(Release)"
#else
	#define KTL_COMMENTS_BUILD				"(Debug)"
#endif

#if defined(KTL_RC_TARGET)
#	define KTL_COMMENTS_TARGET				"(" KTL_RC_TARGET ")"
#else
#	define KTL_COMMENTS_TARGET				"(Unknown)"
#endif

#if defined(KTL_RC_COMPILER)
#	define KTL_COMMENTS_COMPILER			"(" KTL_RC_COMPILER ")"
#else
#	define KTL_COMMENTS_COMPILER			"(Unknown)"
#endif

#if defined(KTL_RC_USELIB_0)
#	define KTL_COMMENTS_USELIB_0			"(" KTL_RC_USELIB_0 ")"
#else
#	define KTL_COMMENTS_USELIB_0			""
#endif
#if defined(KTL_RC_USELIB_1)
#	define KTL_COMMENTS_USELIB_1			"(" KTL_RC_USELIB_1 ")"
#else
#	define KTL_COMMENTS_USELIB_1			""
#endif
#if defined(KTL_RC_USELIB_2)
#	define KTL_COMMENTS_USELIB_2			"(" KTL_RC_USELIB_2 ")"
#else
#	define KTL_COMMENTS_USELIB_2			""
#endif
#if defined(KTL_RC_USELIB_3)
#	define KTL_COMMENTS_USELIB_3			"(" KTL_RC_USELIB_3 ")"
#else
#	define KTL_COMMENTS_USELIB_3			""
#endif
#if defined(KTL_RC_USELIB_4)
#	define KTL_COMMENTS_USELIB_4			"(" KTL_RC_USELIB_4 ")"
#else
#	define KTL_COMMENTS_USELIB_4			""
#endif
#if defined(KTL_RC_USELIB_5)
#	define KTL_COMMENTS_USELIB_5			"(" KTL_RC_USELIB_5 ")"
#else
#	define KTL_COMMENTS_USELIB_5			""
#endif
#if defined(KTL_RC_USELIB_6)
#	define KTL_COMMENTS_USELIB_6			"(" KTL_RC_USELIB_6 ")"
#else
#	define KTL_COMMENTS_USELIB_6			""
#endif
#if defined(KTL_RC_USELIB_7)
#	define KTL_COMMENTS_USELIB_7			"(" KTL_RC_USELIB_7 ")"
#else
#	define KTL_COMMENTS_USELIB_7			""
#endif
#if defined(KTL_RC_USELIB_8)
#	define KTL_COMMENTS_USELIB_8			"(" KTL_RC_USELIB_8 ")"
#else
#	define KTL_COMMENTS_USELIB_8			""
#endif
#if defined(KTL_RC_USELIB_9)
#	define KTL_COMMENTS_USELIB_9			"(" KTL_RC_USELIB_9 ")"
#else
#	define KTL_COMMENTS_USELIB_9			""
#endif
#define KTL_COMMENTS_LIBRARIES				KTL_COMMENTS_USELIB_0 \
											KTL_COMMENTS_USELIB_1 \
											KTL_COMMENTS_USELIB_2 \
											KTL_COMMENTS_USELIB_3 \
											KTL_COMMENTS_USELIB_4 \
											KTL_COMMENTS_USELIB_5 \
											KTL_COMMENTS_USELIB_6 \
											KTL_COMMENTS_USELIB_7 \
											KTL_COMMENTS_USELIB_8 \
											KTL_COMMENTS_USELIB_9

#define KTL_STRINGIZE_I(TEXT)				#TEXT
#define KTL_STRINGIZE(TEXT)					KTL_STRINGIZE_I(TEXT)

#define KTL_FILEVERSION						KTL_RC_VERSION
#define KTL_PRODUCTVERSION					KTL_RC_VERSION
#define KTL_FILEFLAGSMASK					VS_FFI_FILEFLAGSMASK
#ifdef NDEBUG
	#define KTL_FILEFLAGS					(0x0L | VS_FF_PRERELEASE)
#else
	#define KTL_FILEFLAGS					(0x0L | VS_FF_PRERELEASE | VS_FF_DEBUG)
#endif
#define KTL_FILEOS							VOS_NT_WINDOWS32
#define KTL_FILETYPE						VFT_DLL
#define KTL_FILESUBTYPE						VFT2_UNKNOWN

#define KTL_COMMENTS						KTL_FULL_MODULE_NAME \
											" " KTL_STRINGIZE(KTL_RC_VERSION) "\n" \
											" " KTL_COMMENTS_BUILD "\n" \
											" / Target: " KTL_COMMENTS_TARGET "\n" \
											" / Compiler: " KTL_COMMENTS_COMPILER "\n" \
											" / Libraries: " KTL_COMMENTS_LIBRARIES "\n" \
											"\0"
#define KTL_COMPANY_NAME					"Bolero MURAKAMI\0"
#define KTL_FILE_DESCRIPTION				KTL_FULL_MODULE_NAME " (KiriKiri Plug-in DLL)\0"
#define KTL_FILE_VERSION					KTL_STRINGIZE(KTL_RC_VERSION) "\0"
#define KTL_INTERNAL_NAME					KTL_FULL_PROJECT_NAME "\0"
#define KTL_LEGAL_COPYRIGHT					"Copyright (C) 2009-2014 Bolero MURAKAMI.\0"
#define KTL_LEGAL_TRADEMARKS				"\0"
#ifdef NDEBUG
#	define KTL_ORIGINAL_FILENAME			KTL_FULL_PROJECT_NAME ".dll\0"
#else
#	define KTL_ORIGINAL_FILENAME			KTL_FULL_PROJECT_NAME "-d.dll\0"
#endif
#define KTL_PRODUCT_NAME					KTL_FULL_MODULE_NAME "\0"
#define KTL_PRODUCT_VERSION					KTL_STRINGIZE(KTL_RC_VERSION) "\0"
#define KTL_PRIVATE_BUILD					"\0"
#define KTL_SPECIAL_BUILD					"\0"

#endif	// #ifndef KTL_RC_VERSION_H
