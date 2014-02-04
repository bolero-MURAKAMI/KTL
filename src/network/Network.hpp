/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_NETWORK_NETWORK_HPP
#define SRC_NETWORK_NETWORK_HPP

#include <algorithm>
#include <string>
#include <iostream>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "NetworkCommon.hpp"
#include "NetworkDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Network:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Network:error")

namespace ktl {
	//
	// NativeNetwork
	//
	NativeNetwork::NativeNetwork() {}
	//
	//	SUMMARY: I/Oサービス系メソッド
	//
	KTL_INLINE void NativeNetwork::run() {
		NetworkService::get_mutable_instance().run();
	}
	KTL_INLINE void NativeNetwork::run(size_type new_thread_count) {
		NetworkService::get_mutable_instance().run(new_thread_count);
	}
	KTL_INLINE void NativeNetwork::runWithHardwareConcurrency() {
		NetworkService::get_mutable_instance().run_with_hardware_concurrency();
	}
	KTL_INLINE void NativeNetwork::runAtLeast(size_type new_thread_count) {
		NetworkService::get_mutable_instance().run_at_least(new_thread_count);
	}
	KTL_INLINE void NativeNetwork::runAtMost(size_type new_thread_count) {
		NetworkService::get_mutable_instance().run_at_most(new_thread_count);
	}
	KTL_INLINE void NativeNetwork::finish() {
		NetworkService::get_mutable_instance().finish();
	}
	KTL_INLINE void NativeNetwork::stop() {
		NetworkService::get_mutable_instance().stop();
	}
	KTL_INLINE NativeNetwork::size_type NativeNetwork::threadCount() {
		return NetworkService::get_const_instance().thread_count();
	}
	//
	//	SUMMARY: ユーティリティ系メソッド
	//
	KTL_INLINE bool NativeNetwork::downloadHTTP(tjs_char const* url, tjs_char const* storage) {
		return NetworkUtils::downloadHTTP(url, storage);
	}
	KTL_INLINE NativeNetwork::flag_type NativeNetwork::getInternetConnetionState() {
		return NetworkUtils::getInternetConnetionState();
	}
	KTL_INLINE bool NativeNetwork::isInternetConnected() {
		return NetworkUtils::isInternetConnected();
	}
	KTL_INLINE tTJSVariant NativeNetwork::hostName() {
		boost::system::error_code error;
		std::string result = boost::asio::ip::host_name(error);
		if (error) {
			return tTJSVariant();
		}
		return tTJSVariant(result.c_str());
	}
	//
	//	SUMMARY: スレッド系メソッド
	//
	KTL_INLINE NativeNetwork::size_type NativeNetwork::getThreadHardwareConcurrency() {
		return boost::thread::hardware_concurrency();
	}
	//
	//	SUMMARY: クロック系メソッド
	//
	KTL_INLINE tTJSVariant NativeNetwork::getClockTime() {
		boost::system::error_code error;
		time_point_type time_point = clock_type::now(error);
		if (error) {
			return tTJSVariant();
		}
		return tTVInteger(boost::chrono::duration_cast<milliseconds_type>(time_point.time_since_epoch()).count());
	}
	//
	//	SUMMARY: 終了時処理系メソッド
	//
	KTL_INLINE NativeNetwork::flag_type NativeNetwork::getOnKillFlag() {
		return NetworkService::get_const_instance().get_on_kill_flag();
	}
	KTL_INLINE void NativeNetwork::setOnKillFlag(flag_type on_kill_flag) {
		NetworkService::get_mutable_instance().set_on_kill_flag(
			static_cast<NetworkService::on_kill_flag_type>(on_kill_flag)
			);
	}

	//
	// Network
	//
	Network::Network() {}
	tjs_error TJS_INTF_METHOD Network::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Network::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeNetwork>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Network::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Network::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: I/Oサービス系メソッド
	//
	KTL_INLINE void Network::run() {
		NativeNetwork::run();
	}
	KTL_INLINE void Network::run(tTVInteger new_thread_count) {
		NativeNetwork::run(
			boost::numeric_cast<NativeNetwork::size_type>(new_thread_count)
			);
	}
	KTL_INLINE void Network::runWithHardwareConcurrency() {
		NativeNetwork::runWithHardwareConcurrency();
	}
	KTL_INLINE void Network::runAtLeast(tTVInteger new_thread_count) {
		NativeNetwork::runAtLeast(
			boost::numeric_cast<NativeNetwork::size_type>(new_thread_count)
			);
	}
	KTL_INLINE void Network::runAtMost(tTVInteger new_thread_count) {
		NativeNetwork::runAtMost(
			boost::numeric_cast<NativeNetwork::size_type>(new_thread_count)
			);
	}
	KTL_INLINE void Network::finish() {
		NativeNetwork::finish();
	}
	KTL_INLINE void Network::stop() {
		NativeNetwork::stop();
	}
	KTL_INLINE tTVInteger Network::threadCount() {
		return NativeNetwork::threadCount();
	}
	//
	//	SUMMARY: ユーティリティ系メソッド
	//
	KTL_INLINE bool Network::downloadHTTP(tTJSVariantString const* url, tTJSVariantString const* storage) {
		return NativeNetwork::downloadHTTP(
			sprig::krkr::tjs::as_c_str(url),
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger Network::getInternetConnetionState() {
		return NativeNetwork::getInternetConnetionState();
	}
	KTL_INLINE bool Network::isInternetConnected() {
		return NativeNetwork::isInternetConnected();
	}
	KTL_INLINE tTJSVariant Network::hostName() {
		return NativeNetwork::hostName();
	}
	//
	//	SUMMARY: スレッド系メソッド
	//
	KTL_INLINE tTVInteger Network::getThreadHardwareConcurrency() {
		return NativeNetwork::getThreadHardwareConcurrency();
	}
	//
	//	SUMMARY: クロック系メソッド
	//
	KTL_INLINE tTJSVariant Network::getClockTime() {
		return NativeNetwork::getClockTime();
	}
	//
	//	SUMMARY: 終了時処理系メソッド
	//
	KTL_INLINE tTVInteger Network::getOnKillFlag() {
		return NativeNetwork::getOnKillFlag();
	}
	KTL_INLINE void Network::setOnKillFlag(tTVInteger on_kill_flag) {
		NativeNetwork::setOnKillFlag(
			sprig::numeric::bit_cast<NativeNetwork::flag_type>(on_kill_flag)
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_NETWORK_NETWORK_HPP
