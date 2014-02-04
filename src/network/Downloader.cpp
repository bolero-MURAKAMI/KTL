/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
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

#include "Downloader.hpp"

namespace ktl {
	//
	// CreateNativeClassDownloader
	//
#define TJS_NATIVE_CLASSID_NAME DownloaderClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassDownloader, Downloader, Downloader)
	//
	//	SUMMARY: ダウンロード系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(dhfAnalyHTTPHeader, Downloader::dhfAnalyHTTPHeader);
	//
	//	SUMMARY: 初期化系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(reset)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->reset()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(reset);
	//
	//	SUMMARY: ダウンロード系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(downloadHTTP)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->downloadHTTP(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->downloadHTTP(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(downloadHTTP);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncDownloadHTTP)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncDownloadHTTP(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncDownloadHTTP(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncDownloadHTTP);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(cancel)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->cancel()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(cancel);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, isProcessing, isProcessing);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, failed, failed);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, cancelled, cancelled);
	//
	//	SUMMARY: 情報系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, transferredSize, transferredSize);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, contentLength, contentLength);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, chunked, chunked);
	//
	//	SUMMARY: バッファダウンロード系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(downloadHTTPToBuffer)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->downloadHTTPToBuffer(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->downloadHTTPToBuffer(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(downloadHTTPToBuffer);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncDownloadHTTPToBuffer)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncDownloadHTTPToBuffer(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncDownloadHTTPToBuffer(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncDownloadHTTPToBuffer);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(clearBuffer)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->clearBuffer()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(clearBuffer);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, bufferEnable, bufferEnable);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, bufferSize, bufferSize);
	//
	//	SUMMARY: バッファ取得系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getBufferAsOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->getBufferAsOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->getBufferAsOctet(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->getBufferAsOctet()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getBufferAsOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getBufferAsString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->getBufferAsString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->getBufferAsString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->getBufferAsString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getBufferAsString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getBufferAsNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->getBufferAsNarrowString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->getBufferAsNarrowString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->getBufferAsNarrowString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getBufferAsNarrowString);
	//
	//	SUMMARY: タイムアウト系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(waitTimeoutFromNow)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->waitTimeoutFromNow(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(waitTimeoutFromNow)
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(waitTimeoutAt)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->waitTimeoutAt(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(waitTimeoutAt);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncWaitTimeoutFromNow)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncWaitTimeoutFromNow(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncWaitTimeoutFromNow);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncWaitTimeoutAt)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncWaitTimeoutAt(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncWaitTimeoutAt);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, isWaitingTimeout, isWaitingTimeout);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, expiresTimeoutFromNow, expiresTimeoutFromNow);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, expiresTimeoutAt, expiresTimeoutAt);
	//
	//	SUMMARY: 待機系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(join)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->join()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(join)
	//
	//	SUMMARY: ポスト系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(addPostDataEncoded)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		switch (SPRIG_KRKR_ARG_TYPE(0)) {
		case tvtString:
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addPostDataEncoded(SPRIG_KRKR_ARG_STRING(0))
				);
			break;
		case tvtOctet:
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addPostDataEncoded(SPRIG_KRKR_ARG_OCTET(0))
				);
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(addPostDataEncoded);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(clearPostData)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->clearPostData()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(clearPostData);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, postDataEnable, postDataEnable);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, postDataSize, postDataSize);
	//
	//	SUMMARY: URL情報系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, hostName, hostName);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, serviceName, serviceName);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, contentPath, contentPath);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, URL, URL);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, storageLocalName, storageLocalName);
	//
	//	SUMMARY: HTTPレスポンス系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, HTTPVersion, HTTPVersion);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, statusCode, statusCode);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Downloader, reasonPhrase, reasonPhrase);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getField)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			this_->getField(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getField);
	//
	//	SUMMARY: コールバック系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(Downloader, onFinished, getOnFinished, setOnFinished);
	//
	//	SUMMARY: カスタムヘッダ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setCustomHeader)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		switch (SPRIG_KRKR_ARG_TYPE(0)) {
		case tvtString:
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setCustomHeader(SPRIG_KRKR_ARG_STRING(0))
				);
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setCustomHeader);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(clearCustomHeader)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Downloader);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->clearCustomHeader()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(clearCustomHeader);
	//
	//	SUMMARY: 処理設定系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(Downloader, processStatusCodeError, getProcessStatusCodeError, setProcessStatusCodeError);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_downloader
// unregist_ktl_downloader
//
static void regist_ktl_downloader() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Downloader:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Downloader"), ktl::CreateNativeClassDownloader());
	}
}
static void unregist_ktl_downloader() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Downloader:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Downloader"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_downloader);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_downloader);

NCB_PRE_REGIST_CALLBACK(ktl_thread_callback_setup);
NCB_POST_UNREGIST_CALLBACK(ktl_thread_callback_teardown);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Downloader:information"));
