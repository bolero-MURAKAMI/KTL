/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
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

#include "SystemInfo.hpp"

namespace ktl {
	//
	// CreateNativeClassSystemInfo
	//
#define TJS_NATIVE_CLASSID_NAME SystemInfoClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassSystemInfo, SystemInfo, SystemInfo)
	//
	//	SUMMARY: ドライブ系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(dfUnknown, SystemInfo::dfUnknown);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(dfNoRootDir, SystemInfo::dfNoRootDir);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(dfRemovable, SystemInfo::dfRemovable);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(dfFixed, SystemInfo::dfFixed);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(dfRemote, SystemInfo::dfRemote);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(dfCDROM, SystemInfo::dfCDROM);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(dfRAMDisk, SystemInfo::dfRAMDisk);
	//
	//	SUMMARY: ファイルシステム系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfCaseIsPreserved, SystemInfo::fsfCaseIsPreserved);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfCaseSensitive, SystemInfo::fsfCaseSensitive);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfUnicodeStoredOnDisk, SystemInfo::fsfUnicodeStoredOnDisk);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfPersistentACLs, SystemInfo::fsfPersistentACLs);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfFileCompression, SystemInfo::fsfFileCompression);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfVolIsCompressed, SystemInfo::fsfVolIsCompressed);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfNamedStreams, SystemInfo::fsfNamedStreams);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfSupportsEncryption, SystemInfo::fsfSupportsEncryption);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfSupportsObjectIDs, SystemInfo::fsfSupportsObjectIDs);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfSupportsRepersePoints, SystemInfo::fsfSupportsRepersePoints);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfSupportsSparseFiles, SystemInfo::fsfSupportsSparseFiles);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(fsfVolumeQuotas, SystemInfo::fsfVolumeQuotas);
	//
	// コンピュータ名系メソッド
	//
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(SystemInfo, computerName, computerName);
	//
	// システムディレクトリ情報系メソッド 
	//
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(SystemInfo, windowsDirectory, windowsDirectory);
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(SystemInfo, systemWindowsDirectory, systemWindowsDirectory);
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(SystemInfo, systemDirectory, systemDirectory);
	//
	// メモリ情報系メソッド
	//
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(SystemInfo, globalMemoryStatus, globalMemoryStatus);
	//
	// ディスクドライブ情報系メソッド
	//
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(SystemInfo, logicalDriveStrings, logicalDriveStrings);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getDriveType)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SystemInfo::getDriveType(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getDriveType);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getDriveTypeString)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SystemInfo::getDriveTypeString(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getDriveTypeString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getVolumeInformation)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SystemInfo::getVolumeInformation(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getVolumeInformation);
	//
	// システムメトリック情報系メソッド
	//
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(SystemInfo, cleanBoot, cleanBoot);
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(SystemInfo, monitors, monitors);
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(SystemInfo, mouseButtons, mouseButtons);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_system_info
// unregist_ktl_system_info
//
static void regist_ktl_system_info() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:SystemInfo:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("SystemInfo"), ktl::CreateNativeClassSystemInfo());
	}
}
static void unregist_ktl_system_info() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:SystemInfo:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("SystemInfo"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_system_info);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_system_info);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:SystemInfo:information"));
