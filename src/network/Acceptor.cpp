/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "Config.hpp"

#include <sprig/config/lib/openssl.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>
#include <ktl/thread_callback.hpp>

#include "Acceptor.hpp"

namespace ktl {
	//
	// CreateNativeClassAcceptor
	//
#define TJS_NATIVE_CLASSID_NAME AcceptorClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassAcceptor, Acceptor, Acceptor)
	//
	//	SUMMARY: 初期化系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(reset)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Acceptor);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->reset()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(reset);
	//
	//	SUMMARY: 制御系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(cancel)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Acceptor);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->cancel()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(cancel);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Acceptor, cancelled, cancelled);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(close)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Acceptor);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->close()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(close);
	//
	//	SUMMARY: 接続待ち系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(monitor)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Acceptor);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->monitor(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->monitor(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(monitor);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(monitorSSL)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Acceptor);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->monitorSSL(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->monitorSSL(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(monitorSSL);
	//
	//	SUMMARY: 取得系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(popAccepted)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Acceptor);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->popAccepted()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(popAccepted);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Acceptor, isProcessing, isProcessing);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Acceptor, acceptedCount, acceptedCount);
	//
	//	SUMMARY: コールバック系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(Acceptor, onFinished, getOnFinished, setOnFinished);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_acceptor
// unregist_ktl_acceptor
//
static void regist_ktl_acceptor() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Acceptor:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddNewMember(global, SPRIG_KRKR_TJS_W("Acceptor"), ktl::CreateNativeClassAcceptor());
	}
}
static void unregist_ktl_acceptor() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Acceptor:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global, SPRIG_KRKR_TJS_W("Acceptor"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_acceptor);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_acceptor);

NCB_PRE_REGIST_CALLBACK(ktl_thread_callback_setup);
NCB_POST_UNREGIST_CALLBACK(ktl_thread_callback_teardown);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Acceptor:information"));
