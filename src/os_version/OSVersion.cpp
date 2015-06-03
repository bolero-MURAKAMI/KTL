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

#include "OSVersion.hpp"

//
// KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL
//
#define KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(PROP_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_PROP_DECL(PROP_NAME) \
	{ \
		SPRIG_KRKR_BEGIN_NATIVE_PROP_GETTER() \
		{ \
			SPRIG_KRKR_RESULT_SET(ktl::OSVersion::PROP_NAME()); \
			return TJS_S_OK; \
		} \
		SPRIG_KRKR_END_NATIVE_PROP_GETTER(); \
		TJS_DENY_NATIVE_PROP_SETTER; \
	} \
	SPRIG_KRKR_END_NATIVE_PROP_DECL(PROP_NAME);

namespace ktl {
	//
	// CreateNativeClassOSVersion
	//
#define TJS_NATIVE_CLASSID_NAME OSVersionClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassOSVersion, OSVersion, OSVersion)
	//
	//	SUMMARY: バージョン系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vfWindows2000, OSVersion::vfWindows2000);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vfWindowsXP, OSVersion::vfWindowsXP);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vfWindowsXPProX64, OSVersion::vfWindowsXPProX64);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vfWindowsVista, OSVersion::vfWindowsVista);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vfWindows7, OSVersion::vfWindows7);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vfWindows8_1, OSVersion::vfWindows8_1);
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(majorVersion);
	KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(minorVersion);
	KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(buildNumber);
	KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(platformId);
	KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(CSDVersion);
	KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(servicePackMajor);
	KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(servicePackMinor);
	KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(suiteMask);
	KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(productType);
	//
	//	SUMMARY: 拡張取得プロパティ系メソッド
	//
	KTL_OS_VERSION_NATIVE_STATIC_PROP_DECL(versionFlag);
	//
	//	SUMMARY: チェック系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(checkVersion)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			OSVersion::checkVersion(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(checkVersion);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_os_version, unregist_ktl_os_version
//
void regist_ktl_os_version() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:OSVersion:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddNewMember(global.get(), SPRIG_KRKR_TJS_W("OSVersion"), ktl::CreateNativeClassOSVersion());
	}
}
void unregist_ktl_os_version() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:OSVersion:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("OSVersion"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_os_version);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_os_version);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:OSVersion:information"));
