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

#include "StorageStream.hpp"

namespace ktl {
	//
	// CreateNativeClassStorageStream
	//
#define TJS_NATIVE_CLASSID_NAME StorageStreamClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassStorageStream, StorageStream, StorageStream)
	//
	//	SUMMARY: アクセス系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(afRead, StorageStream::afRead);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(afWrite, StorageStream::afWrite);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(afAppend, StorageStream::afAppend);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(afUpdate, StorageStream::afUpdate);
	//
	//	SUMMARY: シーク系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(sfSet, StorageStream::sfSet);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(sfCur, StorageStream::sfCur);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(sfEnd, StorageStream::sfEnd);
	//
	//	SUMMARY: 開閉系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(open)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->open(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(open);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(close)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->close()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(close);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(StorageStream, isOpen, isOpen);
	//
	//	SUMMARY: 読込系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readOctet(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readOctet()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readNarrowString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readNarrowString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readNarrowString);
	//
	//	SUMMARY: 書込系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeOctet(SPRIG_KRKR_ARG_OCTET(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeString(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeNarrowString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeNarrowString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeNarrowString(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeBlock)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->writeBlock(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeBlock);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeBytes)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->writeBytes(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeBytes);
	//
	//	SUMMARY: シーク系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(seek)
	{
		TJS_GET_NATIVE_INSTANCE(this_, StorageStream);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->seek(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(seek);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(StorageStream, current, current);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(StorageStream, size, size);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_storage_stream
// unregist_ktl_storage_stream
//
static void regist_ktl_storage_stream() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:StorageStream:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("StorageStream"), ktl::CreateNativeClassStorageStream());
	}
}
static void unregist_ktl_storage_stream() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:StorageStream:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("StorageStream"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_storage_stream);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_storage_stream);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:StorageStream:information"));
