#pragma once

#include <string>
#include <boost/cstdint.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/singleton.hpp>

namespace ktl {
	//
	// NativeOSVersion
	//
	class NativeOSVersion {
	public:
		typedef boost::uint32_t dword_type;
		typedef boost::uint16_t word_type;
		typedef std::basic_string<tjs_char> string_type;
		typedef boost::int_fast32_t int_type;
		typedef boost::uint_fast32_t flag_type;
	public:
		//
		// OSVersionInfo
		//
		class OSVersionInfo
			: public sprig::singleton<OSVersionInfo>
		{
			SPRIG_FRIEND_SINGLETON(OSVersionInfo);
		private:
			dword_type major_version_;
			dword_type minor_version_;
			dword_type build_number_;
			dword_type platform_id_;
			string_type csd_version_;
			word_type service_pack_major_;
			word_type service_pack_minor_;
			word_type suite_mask_;
			word_type product_type_;
			bool initialized_;
		private:
			OSVersionInfo();
		public:
			void initialize();
			dword_type major_version() const;
			dword_type minor_version() const;
			dword_type build_number() const;
			dword_type platform_id() const;
			string_type const& csd_version() const;
			word_type service_pack_major() const;
			word_type service_pack_minor() const;
			word_type suite_mask() const;
			word_type product_type() const;
		};
	public:
		NativeOSVersion();
	public:
		//
		//	SUMMARY: バージョン系フラグ
		//
		static flag_type const vfWindows2000 = 0x500;
		static flag_type const vfWindowsXP = 0x501;
		static flag_type const vfWindowsVista = 0x600;
		static flag_type const vfWindows7 = 0x601;
	public:
		//
		//	SUMMARY: 取得プロパティ系メソッド
		//
		static dword_type majorVersion();
		static dword_type minorVersion();
		static dword_type buildNumber();
		static dword_type platformId();
		static string_type const& CSDVersion();
		static word_type servicePackMajor();
		static word_type servicePackMinor();
		static word_type suiteMask();
		static word_type productType();
		//
		//	SUMMARY: 拡張取得プロパティ系メソッド
		//
		static flag_type versionFlag();
		//
		//	SUMMARY: チェック系メソッド
		//
		static int_type checkVersion(flag_type flag);
	};

	//
	// OSVersion
	//
	class OSVersion
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeOSVersion> instance_;
	public:
		//
		//	SUMMARY: バージョン系フラグ
		//
		static tTVInteger const vfWindows2000 = NativeOSVersion::vfWindows2000;
		static tTVInteger const vfWindowsXP = NativeOSVersion::vfWindowsXP;
		static tTVInteger const vfWindowsVista = NativeOSVersion::vfWindowsVista;
		static tTVInteger const vfWindows7 = NativeOSVersion::vfWindows7;
	public:
		OSVersion();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: 取得プロパティ系メソッド
		//
		static tTVInteger majorVersion();
		static tTVInteger minorVersion();
		static tTVInteger buildNumber();
		static tTVInteger platformId();
		static tTJSString CSDVersion();
		static tTVInteger servicePackMajor();
		static tTVInteger servicePackMinor();
		static tTVInteger suiteMask();
		static tTVInteger productType();
		//
		//	SUMMARY: 拡張取得プロパティ系メソッド
		//
		static tTVInteger versionFlag();
		//
		//	SUMMARY: チェック系メソッド
		//
		static tTVInteger checkVersion(tTVInteger flag);
	};
}	// namespace ktl
