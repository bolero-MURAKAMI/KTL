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

#include "Socket.hpp"

namespace ktl {
	//
	// CreateNativeClassSocket
	//
#define TJS_NATIVE_CLASSID_NAME SocketClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassSocket, Socket, Socket)
	//
	//	SUMMARY: 内部系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, instance, getInstance);
	//
	//	SUMMARY: プロトコル系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfIPv4, Socket::pfIPv4);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfIPv6, Socket::pfIPv6);
	//
	//	SUMMARY: 完了条件系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ccfTransferAtLeast, Socket::ccfTransferAtLeast);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ccfTransferExactly, Socket::ccfTransferExactly);
	//
	//	SUMMARY: 初期化系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(reset)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
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
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->cancel()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(cancel);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, cancelled, cancelled);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(close)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->close()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(close);
	//
	//	SUMMARY: 名前解決系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(resolve)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->resolve(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(resolve);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncResolve)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncResolve(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncResolve);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, resolvingFinished, resolvingFinished);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, resolvingSucceeded, resolvingSucceeded);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VARIANT(Socket, getEndpointList);
	//
	//	SUMMARY: 接続系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(connect)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->connect(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->connect()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(connect);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(connectSSL)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->connectSSL(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->connectSSL()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(connectSSL);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(connectLoopback)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->connectLoopback(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->connectLoopback(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(connectLoopback);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(connectLoopbackSSL)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->connectLoopbackSSL(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->connectLoopbackSSL(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(connectLoopbackSSL);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncConnect)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncConnect(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncConnect()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncConnect);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncConnectSSL)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncConnectSSL(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncConnectSSL()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncConnectSSL);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncConnectLoopback)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncConnectLoopback(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncConnectLoopback(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncConnectLoopback);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncConnectLoopbackSSL)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncConnectLoopbackSSL(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncConnectLoopbackSSL(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncConnectLoopbackSSL);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, connectingFinished, connectingFinished);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, connectingSucceeded, connectingSucceeded);
	//
	//	SUMMARY: 接続待ち系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(accept)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->accept(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->accept(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(accept);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(acceptSSL)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->acceptSSL(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->acceptSSL(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(acceptSSL);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncAccept)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncAccept(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncAccept(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncAccept);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncAcceptSSL)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncAcceptSSL(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncAcceptSSL(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncAcceptSSL);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, acceptingFinished, acceptingFinished);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, acceptingSucceeded, acceptingSucceeded);
	//
	//	SUMMARY: ハンドシェイク系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(handshake)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->handshake()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(handshake);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncHandshake)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncHandshake()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncHandshake);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, handshakingFinished, handshakingFinished);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, handshakingSucceeded, handshakingSucceeded);
	//
	//	SUMMARY: 送信系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(pushByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		this_->pushByte(SPRIG_KRKR_ARG_VARIANT(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(pushByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(pushString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->pushString(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(pushString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(pushNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->pushNarrowString(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(pushNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(pushOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		this_->pushOctet(SPRIG_KRKR_ARG_OCTET(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(pushOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(pushLine)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->pushLine(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(pushLine);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(write)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->write()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(write);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncWrite)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncWrite()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncWrite);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, writingFinished, writingFinished);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, writingSucceeded, writingSucceeded);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, writingBufferSize, writingBufferSize);
	//
	//	SUMMARY: 送信バッファ取得系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getWritingBufferAsOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->getWritingBufferAsOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->getWritingBufferAsOctet(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->getWritingBufferAsOctet()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getWritingBufferAsOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getWritingBufferAsString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->getWritingBufferAsString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->getWritingBufferAsString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->getWritingBufferAsString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getWritingBufferAsString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getWritingBufferAsNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->getWritingBufferAsNarrowString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->getWritingBufferAsNarrowString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->getWritingBufferAsNarrowString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getWritingBufferAsNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(consumeWritingBuffer)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->consumeWritingBuffer(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->consumeWritingBuffer()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(consumeWritingBuffer);
	//
	//	SUMMARY: 受信系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(popByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->popByte()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(popByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(popString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->popString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->popString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(popString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(popNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->popNarrowString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->popNarrowString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(popNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(popOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->popOctet(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->popOctet()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(popOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(popLine)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->popLine()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(popLine);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(read)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->read(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->read(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->read()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(read);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readUntilByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->readUntilByte(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readUntilByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readUntilString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->readUntilString(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readUntilString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readUntilNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->readUntilNarrowString(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readUntilNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readUntilOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->readUntilOctet(SPRIG_KRKR_ARG_OCTET(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readUntilOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readUntil)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->readUntil(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readUntil);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readUntilRegex)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->readUntilRegex(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readUntilRegex);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncRead)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncRead(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncRead(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->asyncRead()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncRead);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncReadUntilByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncReadUntilByte(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncReadUntilByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncReadUntilString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncReadUntilString(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncReadUntilString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncReadUntilNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncReadUntilNarrowString(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncReadUntilNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncReadUntilOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncReadUntilOctet(SPRIG_KRKR_ARG_OCTET(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncReadUntilOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncReadUntil)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncReadUntil(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncReadUntil);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncReadUntilRegex)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncReadUntilRegex(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncReadUntilRegex);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, readingFinished, readingFinished);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, readingSucceeded, readingSucceeded);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, eof, eof);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, readingBufferSize, readingBufferSize);
	//
	//	SUMMARY: 受信バッファ取得系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getReadingBufferAsOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->getReadingBufferAsOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->getReadingBufferAsOctet(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->getReadingBufferAsOctet()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getReadingBufferAsOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getReadingBufferAsString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->getReadingBufferAsString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->getReadingBufferAsString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->getReadingBufferAsString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getReadingBufferAsString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getReadingBufferAsNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->getReadingBufferAsNarrowString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->getReadingBufferAsNarrowString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->getReadingBufferAsNarrowString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getReadingBufferAsNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(consumeReadingBuffer)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->consumeReadingBuffer(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->consumeReadingBuffer()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(consumeReadingBuffer);
	//
	//	SUMMARY: タイムアウト系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(waitTimeoutFromNow)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->waitTimeoutFromNow(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(waitTimeoutFromNow);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(waitTimeoutAt)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->waitTimeoutAt(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(waitTimeoutAt);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncWaitTimeoutFromNow)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncWaitTimeoutFromNow(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncWaitTimeoutFromNow);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asyncWaitTimeoutAt)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->asyncWaitTimeoutAt(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asyncWaitTimeoutAt);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, isWaitingTimeout, isWaitingTimeout);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, expiresTimeoutFromNow, expiresTimeoutFromNow);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, expiresTimeoutAt, expiresTimeoutAt);
	//
	//	SUMMARY: 待機系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(join)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Socket);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->join()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(join)
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, isProcessing, isProcessing);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, isOpen, isOpen);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Socket, isSSL, isSSL);
	//
	//	SUMMARY: コールバック系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(Socket, onFinished, getOnFinished, setOnFinished);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_socket
// unregist_ktl_socket
//
static void regist_ktl_socket() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Socket:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Socket"), ktl::CreateNativeClassSocket());
	}
}
static void unregist_ktl_socket() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Socket:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Socket"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_socket);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_socket);

NCB_PRE_REGIST_CALLBACK(ktl_thread_callback_setup);
NCB_POST_UNREGIST_CALLBACK(ktl_thread_callback_teardown);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Socket:information"));
