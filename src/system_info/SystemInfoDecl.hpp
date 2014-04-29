/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_SYSTEM_INFO_SYSTEM_INFO_DECL_HPP
#define SRC_SYSTEM_INFO_SYSTEM_INFO_DECL_HPP

#include <boost/cstdint.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeSystemInfo
	//
	class NativeSystemInfo {
	public:
		typedef boost::uint_fast32_t flag_type;
	public:
		static tTJSString getLocalName(tjs_char const* path);
	public:
		NativeSystemInfo();
	public:
		//
		//	SUMMARY: ドライブ系フラグ
		//
		static flag_type const dfUnknown = DRIVE_UNKNOWN;
		static flag_type const dfNoRootDir = DRIVE_NO_ROOT_DIR;
		static flag_type const dfRemovable = DRIVE_REMOVABLE;
		static flag_type const dfFixed = DRIVE_FIXED;
		static flag_type const dfRemote = DRIVE_REMOTE;
		static flag_type const dfCDROM = DRIVE_CDROM;
		static flag_type const dfRAMDisk = DRIVE_RAMDISK;
		//
		//	SUMMARY: ファイルシステム系フラグ
		//
		static flag_type const fsfCaseIsPreserved = FS_CASE_IS_PRESERVED;
		static flag_type const fsfCaseSensitive = FS_CASE_SENSITIVE;
		static flag_type const fsfUnicodeStoredOnDisk = FS_UNICODE_STORED_ON_DISK;
		static flag_type const fsfPersistentACLs = FS_PERSISTENT_ACLS;
		static flag_type const fsfFileCompression = FS_FILE_COMPRESSION;
		static flag_type const fsfVolIsCompressed = FS_VOL_IS_COMPRESSED;
		static flag_type const fsfNamedStreams = FILE_NAMED_STREAMS;
		static flag_type const fsfSupportsEncryption = FILE_SUPPORTS_ENCRYPTION;
		static flag_type const fsfSupportsObjectIDs = FILE_SUPPORTS_OBJECT_IDS;
		static flag_type const fsfSupportsRepersePoints = FILE_SUPPORTS_REPARSE_POINTS;
		static flag_type const fsfSupportsSparseFiles = FILE_SUPPORTS_SPARSE_FILES;
		static flag_type const fsfVolumeQuotas = FILE_VOLUME_QUOTAS;
		//
		//	SUMMARY: CPU機能系フラグ
		//
		static flag_type const cffFPU = TVP_CPU_HAS_FPU;
		static flag_type const cffMMX = TVP_CPU_HAS_MMX;
		static flag_type const cff3DN = TVP_CPU_HAS_3DN;
		static flag_type const cffSSE = TVP_CPU_HAS_SSE;
		static flag_type const cffCMOVcc = TVP_CPU_HAS_CMOV;
		static flag_type const cffE3DN = TVP_CPU_HAS_E3DN;
		static flag_type const cffEMMX = TVP_CPU_HAS_EMMX;
		static flag_type const cffSSE2 = TVP_CPU_HAS_SSE2;
		static flag_type const cffTSC = TVP_CPU_HAS_TSC;
		static flag_type const cffMask = TVP_CPU_FEATURE_MASK;
		//
		//	SUMMARY: CPUベンダ系フラグ
		//
		static flag_type const cvfIntel = TVP_CPU_IS_INTEL;
		static flag_type const cvfAMD = TVP_CPU_IS_AMD;
		static flag_type const cvfIDT = TVP_CPU_IS_IDT;
		static flag_type const cvfCyrix = TVP_CPU_IS_CYRIX;
		static flag_type const cvfNexGen = TVP_CPU_IS_NEXGEN;
		static flag_type const cvfRise = TVP_CPU_IS_RISE;
		static flag_type const cvfUMC = TVP_CPU_IS_UMC;
		static flag_type const cvfTransmeta = TVP_CPU_IS_TRANSMETA;
		static flag_type const cvfUnknown = TVP_CPU_IS_UNKNOWN;
		static flag_type const cvfMask = TVP_CPU_VENDOR_MASK;
	public:
		//
		// コンピュータ名系メソッド
		//
		static tTJSString computerName();
		//
		// システムディレクトリ情報系メソッド 
		//
		static tTJSString windowsDirectory();
		static tTJSString systemWindowsDirectory();
		static tTJSString systemDirectory();
		//
		// メモリ情報系メソッド
		//
		static tTJSVariant globalMemoryStatus();
		//
		// ディスクドライブ情報系メソッド
		//
		static tTJSVariant logicalDriveStrings();
		static flag_type getDriveType(tjs_char const* root_path_name);
		static tTJSString getDriveTypeString(tjs_char const* root_path_name);
		static tTJSVariant getVolumeInformation(tjs_char const* root_path_name);
		//
		// システムメトリック情報系メソッド
		//
		static int cleanBoot();
		static int monitors();
		static int mouseButtons();
		//
		// CPU機能系メソッド
		//
		static flag_type CPUType();
	};

	//
	// SystemInfo
	//
	class SystemInfo
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeSystemInfo> instance_;
	public:
		SystemInfo();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: ドライブ系フラグ
		//
		static tTVInteger const dfUnknown = NativeSystemInfo::dfUnknown;
		static tTVInteger const dfNoRootDir = NativeSystemInfo::dfNoRootDir;
		static tTVInteger const dfRemovable = NativeSystemInfo::dfRemovable;
		static tTVInteger const dfFixed = NativeSystemInfo::dfFixed;
		static tTVInteger const dfRemote = NativeSystemInfo::dfRemote;
		static tTVInteger const dfCDROM = NativeSystemInfo::dfCDROM;
		static tTVInteger const dfRAMDisk = NativeSystemInfo::dfRAMDisk;
		//
		//	SUMMARY: ファイルシステム系フラグ
		//
		static tTVInteger const fsfCaseIsPreserved = NativeSystemInfo::fsfCaseIsPreserved;
		static tTVInteger const fsfCaseSensitive = NativeSystemInfo::fsfCaseSensitive;
		static tTVInteger const fsfUnicodeStoredOnDisk = NativeSystemInfo::fsfUnicodeStoredOnDisk;
		static tTVInteger const fsfPersistentACLs = NativeSystemInfo::fsfPersistentACLs;
		static tTVInteger const fsfFileCompression = NativeSystemInfo::fsfFileCompression;
		static tTVInteger const fsfVolIsCompressed = NativeSystemInfo::fsfVolIsCompressed;
		static tTVInteger const fsfNamedStreams = NativeSystemInfo::fsfNamedStreams;
		static tTVInteger const fsfSupportsEncryption = NativeSystemInfo::fsfSupportsEncryption;
		static tTVInteger const fsfSupportsObjectIDs = NativeSystemInfo::fsfSupportsObjectIDs;
		static tTVInteger const fsfSupportsRepersePoints = NativeSystemInfo::fsfSupportsRepersePoints;
		static tTVInteger const fsfSupportsSparseFiles = NativeSystemInfo::fsfSupportsSparseFiles;
		static tTVInteger const fsfVolumeQuotas = NativeSystemInfo::fsfVolumeQuotas;
		//
		//	SUMMARY: CPU機能系フラグ
		//
		static tTVInteger const cffFPU = NativeSystemInfo::cffFPU;
		static tTVInteger const cffMMX = NativeSystemInfo::cffMMX;
		static tTVInteger const cff3DN = NativeSystemInfo::cff3DN;
		static tTVInteger const cffSSE = NativeSystemInfo::cffSSE;
		static tTVInteger const cffCMOVcc = NativeSystemInfo::cffCMOVcc;
		static tTVInteger const cffE3DN = NativeSystemInfo::cffE3DN;
		static tTVInteger const cffEMMX = NativeSystemInfo::cffEMMX;
		static tTVInteger const cffSSE2 = NativeSystemInfo::cffSSE2;
		static tTVInteger const cffTSC = NativeSystemInfo::cffTSC;
		static tTVInteger const cffMask = NativeSystemInfo::cffMask;
		//
		//	SUMMARY: CPUベンダ系フラグ
		//
		static tTVInteger const cvfIntel = NativeSystemInfo::cvfIntel;
		static tTVInteger const cvfAMD = NativeSystemInfo::cvfAMD;
		static tTVInteger const cvfIDT = NativeSystemInfo::cvfIDT;
		static tTVInteger const cvfCyrix = NativeSystemInfo::cvfCyrix;
		static tTVInteger const cvfNexGen = NativeSystemInfo::cvfNexGen;
		static tTVInteger const cvfRise = NativeSystemInfo::cvfRise;
		static tTVInteger const cvfUMC = NativeSystemInfo::cvfUMC;
		static tTVInteger const cvfTransmeta = NativeSystemInfo::cvfTransmeta;
		static tTVInteger const cvfUnknown = NativeSystemInfo::cvfUnknown;
		static tTVInteger const cvfMask = NativeSystemInfo::cvfMask;
	public:
		//
		// コンピュータ名系メソッド
		//
		static tTJSString computerName();
		//
		// システムディレクトリ情報系メソッド 
		//
		static tTJSString windowsDirectory();
		static tTJSString systemWindowsDirectory();
		static tTJSString systemDirectory();
		//
		// メモリ情報系メソッド
		//
		static tTJSVariant globalMemoryStatus();
		//
		// ディスクドライブ情報系メソッド
		//
		static tTJSVariant logicalDriveStrings();
		static tTVInteger getDriveType(tTJSVariantString const* root_path_name);
		static tTJSString getDriveTypeString(tTJSVariantString const* root_path_name);
		static tTJSVariant getVolumeInformation(tTJSVariantString const* root_path_name);
		//
		// システムメトリック情報系メソッド
		//
		static tTVInteger cleanBoot();
		static tTVInteger monitors();
		static tTVInteger mouseButtons();
		//
		// CPU機能系メソッド
		//
		static tTVInteger CPUType();
	};
}	// namespace ktl

#endif	// #ifndef SRC_SYSTEM_INFO_SYSTEM_INFO_DECL_HPP
