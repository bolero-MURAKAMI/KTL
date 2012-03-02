#pragma once

#include <string>
#include <boost/cstdint.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

#include "NetworkCommon.hpp"

namespace ktl {
	//
	// NativeNetwork
	//
	class NativeNetwork {
	public:
		typedef NetworkService::size_type size_type;
		typedef NetworkUtils::impl_string_type impl_string_type;
		typedef NetworkUtils::flag_type flag_type;
		typedef NetworkUtils::clock_type clock_type;
		typedef NetworkUtils::time_point_type time_point_type;
		typedef NetworkUtils::rep_type rep_type;
		typedef NetworkUtils::milliseconds_type milliseconds_type;
	public:
		NativeNetwork();
	public:
		//
		//	SUMMARY: インターネット接続系フラグ
		//
		static flag_type const cifModem = INTERNET_CONNECTION_MODEM;
		static flag_type const cifLAN = INTERNET_CONNECTION_LAN;
		static flag_type const cifProxy = INTERNET_CONNECTION_PROXY;
		static flag_type const cifModemBusy = INTERNET_CONNECTION_MODEM_BUSY;
		static flag_type const cifRASInstalled = INTERNET_RAS_INSTALLED;
		static flag_type const cifOffline = INTERNET_CONNECTION_OFFLINE;
		static flag_type const cifConfigured = INTERNET_CONNECTION_CONFIGURED;
		//
		//	SUMMARY: 終了時処理系フラグ
		//
		static flag_type const okfStop = NetworkService::stop_on_kill;
		static flag_type const okfFinish = NetworkService::finish_on_kill;
		static flag_type const okfDoNothing = NetworkService::do_nothing_on_kill;
		//
		//	SUMMARY: プロトコル系フラグ
		//
		static flag_type const pfIPv4 = NetworkUtils::pfIPv4;
		static flag_type const pfIPv6 = NetworkUtils::pfIPv6;
		//
		//	SUMMARY: 完了条件系フラグ
		//
		static flag_type const ccfTransferAtLeast = NetworkUtils::ccfTransferAtLeast;
		static flag_type const ccfTransferExactly = NetworkUtils::ccfTransferExactly;
	public:
		//
		//	SUMMARY: I/Oサービス系メソッド
		//
		static void run();
		static void run(size_type new_thread_count);
		static void runAtLeast(size_type new_thread_count);
		static void runAtMost(size_type new_thread_count);
		static void finish();
		static void stop();
		static size_type threadCount();
		//
		//	SUMMARY: ユーティリティ系メソッド
		//
		static bool downloadHTTP(tjs_char const* url, tjs_char const* storage);
		static flag_type getInternetConnetionState();
		static bool isInternetConnected();
		static tTJSVariant hostName();
		//
		//	SUMMARY: クロック系メソッド
		//
		static tTJSVariant getClockTime();
		//
		//	SUMMARY: 終了時処理系メソッド
		//
		static flag_type getOnKillFlag();
		static void setOnKillFlag(flag_type on_kill_flag);
	};

	//
	// Network
	//
	class Network
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeNetwork> instance_;
	public:
		Network();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: インターネット接続系フラグ
		//
		static tTVInteger const cifModem = NativeNetwork::cifModem;
		static tTVInteger const cifLAN = NativeNetwork::cifLAN;
		static tTVInteger const cifProxy = NativeNetwork::cifProxy;
		static tTVInteger const cifModemBusy = NativeNetwork::cifModemBusy;
		static tTVInteger const cifRASInstalled = NativeNetwork::cifRASInstalled;
		static tTVInteger const cifOffline = NativeNetwork::cifOffline;
		static tTVInteger const cifConfigured = NativeNetwork::cifConfigured;
		//
		//	SUMMARY: 終了時処理系フラグ
		//
		static tTVInteger const okfStop = NativeNetwork::okfStop;
		static tTVInteger const okfFinish = NativeNetwork::okfFinish;
		static tTVInteger const okfDoNothing = NativeNetwork::okfDoNothing;
		//
		//	SUMMARY: プロトコル系フラグ
		//
		static tTVInteger const pfIPv4 = NativeNetwork::pfIPv4;
		static tTVInteger const pfIPv6 = NativeNetwork::pfIPv6;
		//
		//	SUMMARY: 完了条件系フラグ
		//
		static tTVInteger const ccfTransferAtLeast = NativeNetwork::ccfTransferAtLeast;
		static tTVInteger const ccfTransferExactly = NativeNetwork::ccfTransferExactly;
	public:
		//
		//	SUMMARY: I/Oサービス系メソッド
		//
		static void run();
		static void run(tTVInteger new_thread_count);
		static void runAtLeast(tTVInteger new_thread_count);
		static void runAtMost(tTVInteger new_thread_count);
		static void finish();
		static void stop();
		static tTVInteger threadCount();
		//
		//	SUMMARY: ユーティリティ系メソッド
		//
		static bool downloadHTTP(tTJSVariantString const* url, tTJSVariantString const* storage);
		static tTVInteger getInternetConnetionState();
		static bool isInternetConnected();
		static tTJSVariant hostName();
		//
		//	SUMMARY: クロック系メソッド
		//
		static tTJSVariant getClockTime();
		//
		//	SUMMARY: 終了時処理系メソッド
		//
		static tTVInteger getOnKillFlag();
		static void setOnKillFlag(tTVInteger on_kill_flag);
	};
}	// namespace ktl
