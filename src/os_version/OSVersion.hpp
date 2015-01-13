/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_OS_VERSION_OS_VERSION_HPP
#define SRC_OS_VERSION_OS_VERSION_HPP

#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/windows.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "OSVersionDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:OSVersion:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:OSVersion:error")

namespace ktl {
	//
	// NativeOSVersion
	//
	//
	// NativeOSVersion::OSVersionInfo
	//
	NativeOSVersion::OSVersionInfo::OSVersionInfo()
		: initialized_(false)
	{}
	KTL_INLINE void NativeOSVersion::OSVersionInfo::initialize() {
		if (initialized_) {
			return;
		}
		OSVERSIONINFOEX os_version_info;
		os_version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
		if (!::GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&os_version_info))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("error in GetVersionEx()"),
				sprig::krkr::internal_error
				);
			return;
		}
		major_version_ = os_version_info.dwMajorVersion;
		minor_version_ = os_version_info.dwMinorVersion;
		build_number_ = os_version_info.dwBuildNumber;
		platform_id_ = os_version_info.dwPlatformId;
		csd_version_ = sprig::str_cast<string_type>(os_version_info.szCSDVersion);
		service_pack_major_ = os_version_info.wServicePackMajor;
		service_pack_minor_ = os_version_info.wServicePackMinor;
		suite_mask_ = os_version_info.wSuiteMask;
		product_type_ = os_version_info.wProductType;
		initialized_ = true;
		return;
	}
	KTL_INLINE NativeOSVersion::dword_type NativeOSVersion::OSVersionInfo::major_version() const {
		return major_version_;
	}
	KTL_INLINE NativeOSVersion::dword_type NativeOSVersion::OSVersionInfo::minor_version() const {
		return minor_version_;
	}
	KTL_INLINE NativeOSVersion::dword_type NativeOSVersion::OSVersionInfo::build_number() const {
		return build_number_;
	}
	KTL_INLINE NativeOSVersion::dword_type NativeOSVersion::OSVersionInfo::platform_id() const {
		return platform_id_;
	}
	KTL_INLINE NativeOSVersion::string_type const& NativeOSVersion::OSVersionInfo::csd_version() const {
		return csd_version_;
	}
	KTL_INLINE NativeOSVersion::word_type NativeOSVersion::OSVersionInfo::service_pack_major() const {
		return service_pack_major_;
	}
	KTL_INLINE NativeOSVersion::word_type NativeOSVersion::OSVersionInfo::service_pack_minor() const {
		return service_pack_minor_;
	}
	KTL_INLINE NativeOSVersion::word_type NativeOSVersion::OSVersionInfo::suite_mask() const {
		return suite_mask_;
	}
	KTL_INLINE NativeOSVersion::word_type NativeOSVersion::OSVersionInfo::product_type() const {
		return product_type_;
	}

	NativeOSVersion::NativeOSVersion() {}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	KTL_INLINE NativeOSVersion::dword_type NativeOSVersion::majorVersion() {
		OSVersionInfo::get_mutable_instance().initialize();
		return OSVersionInfo::get_const_instance().major_version();
	}
	KTL_INLINE NativeOSVersion::dword_type NativeOSVersion::minorVersion() {
		OSVersionInfo::get_mutable_instance().initialize();
		return OSVersionInfo::get_const_instance().minor_version();
	}
	KTL_INLINE NativeOSVersion::dword_type NativeOSVersion::buildNumber() {
		OSVersionInfo::get_mutable_instance().initialize();
		return OSVersionInfo::get_const_instance().build_number();
	}
	KTL_INLINE NativeOSVersion::dword_type NativeOSVersion::platformId() {
		OSVersionInfo::get_mutable_instance().initialize();
		return OSVersionInfo::get_const_instance().platform_id();
	}
	KTL_INLINE NativeOSVersion::string_type const& NativeOSVersion::CSDVersion() {
		OSVersionInfo::get_mutable_instance().initialize();
		return OSVersionInfo::get_const_instance().csd_version();
	}
	KTL_INLINE NativeOSVersion::word_type NativeOSVersion::servicePackMajor() {
		OSVersionInfo::get_mutable_instance().initialize();
		return OSVersionInfo::get_const_instance().service_pack_major();
	}
	KTL_INLINE NativeOSVersion::word_type NativeOSVersion::servicePackMinor() {
		OSVersionInfo::get_mutable_instance().initialize();
		return OSVersionInfo::get_const_instance().service_pack_minor();
	}
	KTL_INLINE NativeOSVersion::word_type NativeOSVersion::suiteMask() {
		OSVersionInfo::get_mutable_instance().initialize();
		return OSVersionInfo::get_const_instance().suite_mask();
	}
	KTL_INLINE NativeOSVersion::word_type NativeOSVersion::productType() {
		OSVersionInfo::get_mutable_instance().initialize();
		return OSVersionInfo::get_const_instance().product_type();
	}
	//
	//	SUMMARY: 拡張取得プロパティ系メソッド
	//
	KTL_INLINE NativeOSVersion::flag_type NativeOSVersion::versionFlag() {
		return majorVersion() * 0x100 + minorVersion();
	}
	//
	//	SUMMARY: チェック系メソッド
	//
	KTL_INLINE NativeOSVersion::int_type NativeOSVersion::checkVersion(flag_type flag) {
		return static_cast<int_type>(versionFlag()) - static_cast<int_type>(flag);
	}

	//
	// OSVersion
	//
	OSVersion::OSVersion() {}
	tjs_error TJS_INTF_METHOD OSVersion::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("OSVersion::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeOSVersion>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD OSVersion::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("OSVersion::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	KTL_INLINE tTVInteger OSVersion::majorVersion() {
		return NativeOSVersion::majorVersion();
	}
	KTL_INLINE tTVInteger OSVersion::minorVersion() {
		return NativeOSVersion::minorVersion();
	}
	KTL_INLINE tTVInteger OSVersion::buildNumber() {
		return NativeOSVersion::buildNumber();
	}
	KTL_INLINE tTVInteger OSVersion::platformId() {
		return NativeOSVersion::platformId();
	}
	KTL_INLINE tTJSString OSVersion::CSDVersion() {
		return NativeOSVersion::CSDVersion().c_str();
	}
	KTL_INLINE tTVInteger OSVersion::servicePackMajor() {
		return NativeOSVersion::servicePackMajor();
	}
	KTL_INLINE tTVInteger OSVersion::servicePackMinor() {
		return NativeOSVersion::servicePackMinor();
	}
	KTL_INLINE tTVInteger OSVersion::suiteMask() {
		return NativeOSVersion::suiteMask();
	}
	KTL_INLINE tTVInteger OSVersion::productType() {
		return NativeOSVersion::productType();
	}
	//
	//	SUMMARY: 拡張取得プロパティ系メソッド
	//
	KTL_INLINE tTVInteger OSVersion::versionFlag() {
		return NativeOSVersion::versionFlag();
	}
	//
	//	SUMMARY: チェック系メソッド
	//
	KTL_INLINE tTVInteger OSVersion::checkVersion(tTVInteger flag) {
		return NativeOSVersion::checkVersion(
			sprig::numeric::bit_cast<NativeOSVersion::flag_type>(flag)
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_OS_VERSION_CONFIG_HPP
