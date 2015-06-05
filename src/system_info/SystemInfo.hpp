/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_SYSTEM_INFO_SYSTEM_INFO_HPP
#define SRC_SYSTEM_INFO_SYSTEM_INFO_HPP

#include <vector>
#include <algorithm>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <sprig/external/windows.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/get_pointer.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "SystemInfoDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:SystemInfo:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:SystemInfo:error")

namespace ktl {
	//
	// NativeSystemInfo
	//
	KTL_INLINE tTJSString NativeSystemInfo::getLocalName(tjs_char const* path) {
		tTJSString result(::TVPNormalizeStorageName(path));
		::TVPGetLocalName(result);
		return result;
	}
	NativeSystemInfo::NativeSystemInfo() {}
	//
	// コンピュータ名系メソッド
	//
	KTL_INLINE tTJSString NativeSystemInfo::computerName() {
		DWORD size = 0;
		::GetComputerNameEx(ComputerNameNetBIOS, 0, &size);
		std::vector<TCHAR> buffer(size + 1);
		if (!::GetComputerNameEx(ComputerNameNetBIOS, &buffer[0], &size)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("error in GetComputerNameEx()"),
				sprig::krkr::internal_error
				);
		}
		return &buffer[0];
	}
	//
	// システムディレクトリ情報系メソッド
	//
	KTL_INLINE tTJSString NativeSystemInfo::windowsDirectory() {
		UINT const default_size = MAX_PATH;
		UINT size = default_size;
		std::vector<TCHAR> buffer(size);
		if (!(size = ::GetWindowsDirectory(&buffer[0], size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("error in GetWindowsDirectory()"),
				sprig::krkr::internal_error
				);
		}
		if (size > default_size) {
			buffer.resize(size);
			if (!::GetWindowsDirectory(&buffer[0], size)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("error in GetWindowsDirectory()"),
					sprig::krkr::internal_error
					);
			}
		}
		return &buffer[0];
	}
	KTL_INLINE tTJSString NativeSystemInfo::systemWindowsDirectory() {
		UINT const default_size = MAX_PATH;
		UINT size = default_size;
		std::vector<TCHAR> buffer(size);
		if (!(size = ::GetSystemWindowsDirectory(&buffer[0], size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("error in GetSystemWindowsDirectory()"),
				sprig::krkr::internal_error
				);
		}
		if (size > default_size) {
			buffer.resize(size);
			if (!::GetSystemWindowsDirectory(&buffer[0], size)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("error in GetSystemWindowsDirectory()"),
					sprig::krkr::internal_error
					);
			}
		}
		return &buffer[0];
	}
	KTL_INLINE tTJSString NativeSystemInfo::systemDirectory() {
		UINT const default_size = MAX_PATH;
		UINT size = default_size;
		std::vector<TCHAR> buffer(size);
		if (!(size = ::GetSystemDirectory(&buffer[0], size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("error in GetSystemDirectory()"),
				sprig::krkr::internal_error
				);
		}
		if (size > default_size) {
			buffer.resize(size);
			if (!::GetSystemDirectory(&buffer[0], size)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("error in GetSystemDirectory()"),
					sprig::krkr::internal_error
					);
			}
		}
		return &buffer[0];
	}
	//
	// メモリ情報系メソッド
	//
	KTL_INLINE tTJSVariant NativeSystemInfo::globalMemoryStatus() {
		MEMORYSTATUSEX status;
		status.dwLength = sizeof(MEMORYSTATUSEX);
		if (!::GlobalMemoryStatusEx(&status)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("error in GlobalMemoryStatusEx()"),
				sprig::krkr::internal_error
				);
			return tTJSVariant();
		}
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
			0, 0, 0
			);
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("memoryLoad"), tTJSVariant(boost::numeric_cast<tTVInteger>(status.dwMemoryLoad)));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("totalPhys"), tTJSVariant(boost::numeric_cast<tTVInteger>(status.ullTotalPhys)));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("availPhys"), tTJSVariant(boost::numeric_cast<tTVInteger>(status.ullAvailPhys)));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("totalPageFile"), tTJSVariant(boost::numeric_cast<tTVInteger>(status.ullTotalPageFile)));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("availPageFile"), tTJSVariant(boost::numeric_cast<tTVInteger>(status.ullAvailPageFile)));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("totalVirtual"), tTJSVariant(boost::numeric_cast<tTVInteger>(status.ullTotalVirtual)));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("availVirtual"), tTJSVariant(boost::numeric_cast<tTVInteger>(status.ullAvailVirtual)));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("availExtendedVirtual"), tTJSVariant(boost::numeric_cast<tTVInteger>(status.ullAvailExtendedVirtual)));
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	//
	// ディスクドライブ情報系メソッド
	//
	KTL_INLINE tTJSVariant NativeSystemInfo::logicalDriveStrings() {
		DWORD const default_size = 4 * 16 + 1;
		DWORD size = default_size;
		std::vector<TCHAR> buffer(size);
		if (!(size = ::GetLogicalDriveStrings(size, &buffer[0]))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("error in GetLogicalDriveStrings()"),
				sprig::krkr::internal_error
				);
			return tTJSVariant();
		}
		if (size > default_size) {
			buffer.resize(size);
			if (!::GetLogicalDriveStrings(size, &buffer[0])) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("error in GetLogicalDriveStrings()"),
					sprig::krkr::internal_error
					);
				return tTJSVariant();
			}
		}
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			0, 0, 0
			);
		{
			tjs_int num = 0;
			for (std::vector<TCHAR>::const_iterator it = buffer.begin(), last = buffer.end(); *it != TCHAR(); it = std::find(it, last, TCHAR()) + 1) {
				sprig::krkr::tjs::AddMember(result, num, tTJSVariant(&*it));
				++num;
			}
		}
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE NativeSystemInfo::flag_type NativeSystemInfo::getDriveType(tjs_char const* root_path_name) {
		return ::GetDriveType(getLocalName(root_path_name).c_str());
	}
	KTL_INLINE tTJSString NativeSystemInfo::getDriveTypeString(tjs_char const* root_path_name) {
		UINT drive_type = ::GetDriveType(getLocalName(root_path_name).c_str());
		switch (drive_type) {
		case DRIVE_NO_ROOT_DIR:
			return SPRIG_KRKR_TJS_W("NoRootDir");
		case DRIVE_REMOVABLE:
			return SPRIG_KRKR_TJS_W("Removable");
		case DRIVE_FIXED:
			return SPRIG_KRKR_TJS_W("Fixed");
		case DRIVE_REMOTE:
			return SPRIG_KRKR_TJS_W("Remote");
		case DRIVE_CDROM:
			return SPRIG_KRKR_TJS_W("CDROM");
		case DRIVE_RAMDISK:
			return SPRIG_KRKR_TJS_W("RAMDisk");
		}
		return SPRIG_KRKR_TJS_W("Unknown");
	}
	KTL_INLINE tTJSVariant NativeSystemInfo::getVolumeInformation(tjs_char const* root_path_name) {
		std::vector<TCHAR> volumeName(256);
		DWORD serialNumber = 0;
		DWORD maximumComponentLength = 0;
		DWORD fileSystemFlags = 0;
		std::vector<TCHAR> fileSystemName(256);
		if (!::GetVolumeInformation(
			getLocalName(root_path_name).c_str(),
			&volumeName[0], volumeName.size(), &serialNumber, &maximumComponentLength, &fileSystemFlags, &fileSystemName[0], fileSystemName.size()
			))
		{
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("error in GetVolumeInformation()"),
				sprig::krkr::internal_error
				);
			return tTJSVariant();
		}
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
			0, 0, 0
			);
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("volumeName"), tTJSVariant(&volumeName[0]));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("serialNumber"), tTJSVariant(boost::numeric_cast<tTVInteger>(serialNumber)));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("maximumComponentLength"), tTJSVariant(boost::numeric_cast<tTVInteger>(maximumComponentLength)));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("fileSystemFlags"), tTJSVariant(boost::numeric_cast<tTVInteger>(fileSystemFlags)));
		sprig::krkr::tjs::AddMember(result, SPRIG_KRKR_TJS_W("fileSystemName"), tTJSVariant(&fileSystemName[0]));
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	//
	// システムメトリック情報系メソッド
	//
	KTL_INLINE int NativeSystemInfo::cleanBoot() {
		int val = ::GetSystemMetrics(SM_CLEANBOOT);
		return tTJSVariant(val);
	}
	KTL_INLINE int NativeSystemInfo::monitors() {
		int val = ::GetSystemMetrics(SM_CMONITORS);
		return tTJSVariant(val);
	}
	KTL_INLINE int NativeSystemInfo::mouseButtons() {
		int val = ::GetSystemMetrics(SM_CMOUSEBUTTONS);
		return tTJSVariant(val);
	}
	//
	// CPU機能系メソッド
	//
	KTL_INLINE NativeSystemInfo::flag_type NativeSystemInfo::CPUType() {
		return ::TVPGetCPUType();
	}

	//
	// SystemInfo
	//
	SystemInfo::SystemInfo() {}
	tjs_error TJS_INTF_METHOD SystemInfo::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("SystemInfo::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeSystemInfo>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD SystemInfo::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("SystemInfo::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	// コンピュータ名系メソッド
	//
	KTL_INLINE tTJSString SystemInfo::computerName() {
		return NativeSystemInfo::computerName();
	}
	//
	// システムディレクトリ情報系メソッド 
	//
	KTL_INLINE tTJSString SystemInfo::windowsDirectory() {
		return NativeSystemInfo::windowsDirectory();
	}
	KTL_INLINE tTJSString SystemInfo::systemWindowsDirectory() {
		return NativeSystemInfo::systemWindowsDirectory();
	}
	KTL_INLINE tTJSString SystemInfo::systemDirectory() {
		return NativeSystemInfo::systemDirectory();
	}
	//
	// メモリ情報系メソッド
	//
	KTL_INLINE tTJSVariant SystemInfo::globalMemoryStatus() {
		return NativeSystemInfo::globalMemoryStatus();
	}
	//
	// ディスクドライブ情報系メソッド
	//
	KTL_INLINE tTJSVariant SystemInfo::logicalDriveStrings() {
		return NativeSystemInfo::logicalDriveStrings();
	}
	KTL_INLINE tTVInteger SystemInfo::getDriveType(tTJSVariantString const* root_path_name) {
		return NativeSystemInfo::getDriveType(
			sprig::krkr::tjs::as_c_str(root_path_name)
			);
	}
	KTL_INLINE tTJSString SystemInfo::getDriveTypeString(tTJSVariantString const* root_path_name) {
		return NativeSystemInfo::getDriveTypeString(
			sprig::krkr::tjs::as_c_str(root_path_name)
			);
	}
	KTL_INLINE tTJSVariant SystemInfo::getVolumeInformation(tTJSVariantString const* root_path_name) {
		return NativeSystemInfo::getVolumeInformation(
			sprig::krkr::tjs::as_c_str(root_path_name)
			);
	}
	//
	// システムメトリック情報系メソッド
	//
	KTL_INLINE tTVInteger SystemInfo::cleanBoot() {
		return NativeSystemInfo::cleanBoot();
	}
	KTL_INLINE tTVInteger SystemInfo::monitors() {
		return NativeSystemInfo::monitors();
	}
	KTL_INLINE tTVInteger SystemInfo::mouseButtons() {
		return NativeSystemInfo::mouseButtons();
	}
	//
	// CPU機能系メソッド
	//
	KTL_INLINE tTVInteger SystemInfo::CPUType() {
		return NativeSystemInfo::CPUType();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_SYSTEM_INFO_SYSTEM_INFO_HPP
