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

#include "Checksum.hpp"

namespace ktl {
	//
	// CHECKSUM_CREATE_NATIVE_CLASS
	//
#define CHECKSUM_CREATE_NATIVE_CLASS(FUNCTION_NAME, CLASS_NAME, ENTRY_NAME) \
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(FUNCTION_NAME, CLASS_NAME, ENTRY_NAME) \
	/**/ \
	/*	SUMMARY: 計算系メソッド */ \
	/**/ \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateBlock) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger); \
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateBlock(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateBlock); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateBytes) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateBytes(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateBytes); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateOctet) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateOctet(SPRIG_KRKR_ARG_OCTET(0)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateOctet); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateString) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateString(SPRIG_KRKR_ARG_STRING(0)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateString); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateNarrowString) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateNarrowString(SPRIG_KRKR_ARG_STRING(0)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateNarrowString); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateStorage) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateStorage(SPRIG_KRKR_ARG_STRING(0)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateStorage); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateBlockAsOctet) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger); \
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateBlockAsOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateBlockAsOctet); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateBytesAsOctet) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateBytesAsOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateBytesAsOctet); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateOctetAsOctet) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateOctetAsOctet(SPRIG_KRKR_ARG_OCTET(0)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateOctetAsOctet); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateStringAsOctet) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateStringAsOctet(SPRIG_KRKR_ARG_STRING(0)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateStringAsOctet); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateNarrowStringAsOctet) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateNarrowStringAsOctet(SPRIG_KRKR_ARG_STRING(0)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateNarrowStringAsOctet); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateStorageAsOctet) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		SPRIG_KRKR_RESULT_SET( \
			CLASS_NAME::calculateStorageAsOctet(SPRIG_KRKR_ARG_STRING(0)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateStorageAsOctet); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateBlockAsHex) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger); \
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger); \
		if (numparams >= 3) { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateBlockAsHex(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_BOOL(2)) \
				); \
		} else { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateBlockAsHex(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1)) \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateBlockAsHex); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateBytesAsHex) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger); \
		if (numparams >= 3) { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateBytesAsHex(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_BOOL(2)) \
				); \
		} else { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateBytesAsHex(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1)) \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateBytesAsHex); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateOctetAsHex) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet); \
		if (numparams >= 2) { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateOctetAsHex(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_BOOL(1)) \
				); \
		} else { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateOctetAsHex(SPRIG_KRKR_ARG_OCTET(0)) \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateOctetAsHex); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateStringAsHex) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		if (numparams >= 2) { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateStringAsHex(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_BOOL(1)) \
				); \
		} else { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateStringAsHex(SPRIG_KRKR_ARG_STRING(0)) \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateStringAsHex); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateNarrowStringAsHex) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		if (numparams >= 2) { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateNarrowStringAsHex(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_BOOL(1)) \
				); \
		} else { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateNarrowStringAsHex(SPRIG_KRKR_ARG_STRING(0)) \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateNarrowStringAsHex); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(calculateStorageAsHex) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		if (numparams >= 2) { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateStorageAsHex(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_BOOL(1)) \
				); \
		} else { \
			SPRIG_KRKR_RESULT_SET( \
				CLASS_NAME::calculateStorageAsHex(SPRIG_KRKR_ARG_STRING(0)) \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(calculateStorageAsHex); \
	/**/ \
	/*	SUMMARY: 処理系メソッド */ \
	/**/ \
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(CLASS_NAME, reset); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processBlock) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger); \
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger); \
		SPRIG_KRKR_INVOKE_RESULT_SET( \
			this_->processBlock(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processBlock); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processBytes) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger); \
		SPRIG_KRKR_INVOKE_RESULT_SET( \
			this_->processBytes(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processBytes); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processByte) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_INVOKE_RESULT_SET( \
			this_->processByte(SPRIG_KRKR_ARG_VARIANT(0)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processByte); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processOctet) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet); \
		if (numparams >= 3) { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2)) \
				); \
		} else if (numparams >= 2) { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1)) \
				); \
		} else { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processOctet(SPRIG_KRKR_ARG_OCTET(0)) \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processOctet); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processString) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		if (numparams >= 3) { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2)) \
				); \
		} else if (numparams >= 2) { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1)) \
				); \
		} else { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processString(SPRIG_KRKR_ARG_STRING(0)) \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processString); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processNarrowString) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		if (numparams >= 3) { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processNarrowString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2)) \
				); \
		} else if (numparams >= 2) { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processNarrowString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1)) \
				); \
		} else { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processNarrowString(SPRIG_KRKR_ARG_STRING(0)) \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processNarrowString); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(processStorage) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		if (numparams >= 3) { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processStorage(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2)) \
				); \
		} else if (numparams >= 2) { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processStorage(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1)) \
				); \
		} else { \
			SPRIG_KRKR_INVOKE_RESULT_SET( \
				this_->processStorage(SPRIG_KRKR_ARG_STRING(0)) \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(processStorage); \
	/**/ \
	/*	SUMMARY: 取得系メソッド */ \
	/**/ \
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VARIANT(CLASS_NAME, checksum); \
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VARIANT(CLASS_NAME, checksumOctet); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(checksumHex) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		if (numparams >= 1) { \
			SPRIG_KRKR_RESULT_SET( \
				this_->checksumHex(SPRIG_KRKR_ARG_BOOL(0)) \
				); \
		} else { \
			SPRIG_KRKR_RESULT_SET( \
				this_->checksumHex() \
				); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(checksumHex); \
		\
	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();

	//
	// CreateNativeClassCRC_16
	//
#define TJS_NATIVE_CLASSID_NAME CRC_16ClassID
	CHECKSUM_CREATE_NATIVE_CLASS(CreateNativeClassCRC_16, Checksum<boost::crc_16_type>, CRC_16);
#undef TJS_NATIVE_CLASSID_NAME
	//
	// CreateNativeClassCRC_CCITT
	//
#define TJS_NATIVE_CLASSID_NAME CRC_CCITTClassID
	CHECKSUM_CREATE_NATIVE_CLASS(CreateNativeClassCRC_CCITT, Checksum<boost::crc_ccitt_type>, CRC_CCITT);
#undef TJS_NATIVE_CLASSID_NAME
	//
	// CreateNativeClassCRC_XMODEM
	//
#define TJS_NATIVE_CLASSID_NAME CRC_XMODEMClassID
	CHECKSUM_CREATE_NATIVE_CLASS(CreateNativeClassCRC_XMODEM, Checksum<boost::crc_xmodem_type>, CRC_XMODEM);
#undef TJS_NATIVE_CLASSID_NAME
	//
	// CreateNativeClassCRC_32
	//
#define TJS_NATIVE_CLASSID_NAME CRC_32ClassID
	CHECKSUM_CREATE_NATIVE_CLASS(CreateNativeClassCRC_32, Checksum<boost::crc_32_type>, CRC_32);
#undef TJS_NATIVE_CLASSID_NAME
	//
	// CreateNativeClassMD5
	//
#define TJS_NATIVE_CLASSID_NAME MD5ClassID
	CHECKSUM_CREATE_NATIVE_CLASS(CreateNativeClassMD5, Checksum<hamigaki::checksum::md5>, MD5);
#undef TJS_NATIVE_CLASSID_NAME
	//
	// CreateNativeClassSHA1
	//
#define TJS_NATIVE_CLASSID_NAME SHA1ClassID
	CHECKSUM_CREATE_NATIVE_CLASS(CreateNativeClassSHA1, Checksum<hamigaki::checksum::sha1>, SHA1);
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_checksum
// unregist_ktl_checksum
//
static void regist_ktl_checksum() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Checksum:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("CRC_16"), ktl::CreateNativeClassCRC_16());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("CRC_CCITT"), ktl::CreateNativeClassCRC_CCITT());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("CRC_XMODEM"), ktl::CreateNativeClassCRC_XMODEM());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("CRC_32"), ktl::CreateNativeClassCRC_32());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("MD5"), ktl::CreateNativeClassMD5());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("SHA1"), ktl::CreateNativeClassSHA1());
	}
}
static void unregist_ktl_checksum() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Checksum:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("SHA1"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("MD5"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("CRC_32"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("CRC_XMODEM"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("CRC_CCITT"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("CRC_16"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_checksum);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_checksum);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Checksum:information"));
