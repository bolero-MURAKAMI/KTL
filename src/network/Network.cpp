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

#include "Network.hpp"

namespace ktl {
	//
	// CreateNativeClassNetwork
	//
#define TJS_NATIVE_CLASSID_NAME NetworkClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassNetwork, Network, Network)
	//
	//	SUMMARY: インターネット接続系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(cifModem, Network::cifModem);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(cifLAN, Network::cifLAN);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(cifProxy, Network::cifProxy);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(cifModemBusy, Network::cifModemBusy);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(cifRASInstalled, Network::cifRASInstalled);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(cifOffline, Network::cifOffline);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(cifConfigured, Network::cifConfigured);
	//
	//	SUMMARY: 終了時処理系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(okfStop, Network::okfStop);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(okfFinish, Network::okfFinish);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(okfDoNothing, Network::okfDoNothing);
	//
	//	SUMMARY: プロトコル系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfIPv4, Network::pfIPv4);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfIPv6, Network::pfIPv6);
	//
	//	SUMMARY: 完了条件系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ccfTransferAtLeast, Network::ccfTransferAtLeast);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ccfTransferExactly, Network::ccfTransferExactly);
	//
	//	SUMMARY: I/Oサービス系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(run)
	{
		if (numparams >= 1) {
			Network::run(SPRIG_KRKR_ARG_VARIANT(0));
		} else {
			Network::run();
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(run)
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(runWithHardwareConcurrency)
	{
		Network::runWithHardwareConcurrency();
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(runWithHardwareConcurrency)
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(runAtLeast)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		Network::runAtLeast(SPRIG_KRKR_ARG_VARIANT(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(runAtLeast)
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(runAtMost)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		Network::runAtMost(SPRIG_KRKR_ARG_VARIANT(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(runAtMost)
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(finish)
	{
		Network::finish();
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(finish)
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(stop)
	{
		Network::stop();
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(stop)
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(Network, threadCount, threadCount);
	//
	//	SUMMARY: ユーティリティ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(downloadHTTP)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			Network::downloadHTTP(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(downloadHTTP)
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(Network, internetConnetionState, getInternetConnetionState);
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(Network, isInternetConnected, isInternetConnected);
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(Network, hostName, hostName);
	//
	//	SUMMARY: スレッド系メソッド
	//
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(Network, threadHardwareConcurrency, getThreadHardwareConcurrency);
	//
	//	SUMMARY: クロック系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getClockTime)
	{
		SPRIG_KRKR_RESULT_SET(
			Network::getClockTime()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getClockTime)
	//
	//	SUMMARY: 終了時処理系メソッド
	//
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT(Network, onKillFlag, getOnKillFlag, setOnKillFlag);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS()
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_network
// unregist_ktl_network
//
static void regist_ktl_network() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Network:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
#if defined(KTL_NETWORK_CONFIG_ON_REGIST_RUN)
	ktl::NetworkService::get_mutable_instance().run();
#endif
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Network"), ktl::CreateNativeClassNetwork());
	}
}
static void unregist_ktl_network() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Network:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
#if defined(KTL_NETWORK_CONFIG_ON_UNREGIST_KILL)
	ktl::NetworkService::get_mutable_instance().kill();
#endif
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Network"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_network);
NCB_POST_UNREGIST_CALLBACK(unregist_ktl_network);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Network:information"));
