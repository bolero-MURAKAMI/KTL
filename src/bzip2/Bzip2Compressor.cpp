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

#include "Bzip2Compressor.hpp"

namespace ktl {
	//
	// CreateNativeClassBzip2Compressor
	//
#define TJS_NATIVE_CLASSID_NAME Bzip2CompressorClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassBzip2Compressor, Bzip2Compressor, Bzip2Compressor)
	//
	//	SUMMARY: セットアップ系メソッド
	//
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(Bzip2Compressor, reset);
	SPRIG_KRKR_NATIVE_METHOD_DECL_INVOKE_PARAM_VOID_RES_VARIANT(Bzip2Compressor, setup);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Bzip2Compressor, setupCompleted, setupCompleted);
	SPRIG_KRKR_NATIVE_METHOD_DECL_INVOKE_PARAM_VOID_RES_VARIANT(Bzip2Compressor, finish);
	SPRIG_KRKR_NATIVE_METHOD_DECL_INVOKE_PARAM_VOID_RES_VARIANT(Bzip2Compressor, flush);
	//
	//	SUMMARY: パラメータ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(Bzip2Compressor, blockSize, getBlockSize, setBlockSize);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(Bzip2Compressor, workFactor, getWorkFactor, setWorkFactor);
	//
	//	SUMMARY: 処理系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processBlock)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Bzip2Compressor);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->processBlock(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processBlock);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processBytes)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Bzip2Compressor);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->processBytes(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processBytes);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Bzip2Compressor);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->processByte(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Bzip2Compressor);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processOctet(SPRIG_KRKR_ARG_OCTET(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Bzip2Compressor);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processString(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Bzip2Compressor);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processNarrowString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processNarrowString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processNarrowString(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processStorage)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Bzip2Compressor);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processStorage(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processStorage(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->processStorage(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processStorage);
	//
	//	SUMMARY: 取得系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Bzip2Compressor, size, size);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Bzip2Compressor);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->asOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->asOctet(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->asOctet()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asOctet);
	//
	//	SUMMARY: ストレージ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveStorage)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Bzip2Compressor);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->saveStorage(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->saveStorage(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->saveStorage(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveStorage);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_deflate
// unregist_ktl_deflate
//
static void regist_ktl_deflate() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Bzip2Compressor:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Bzip2Compressor"), ktl::CreateNativeClassBzip2Compressor());
	}
}
static void unregist_ktl_deflate() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Bzip2Compressor:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Bzip2Compressor"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_deflate);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_deflate);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Bzip2Compressor:information"));
