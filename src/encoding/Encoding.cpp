
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "Encoding.hpp"

namespace ktl {
	//
	// CreateNativeClassEncoding
	//
#define TJS_NATIVE_CLASSID_NAME EncodingClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassEncoding, Encoding, Encoding)
	//
	//	SUMMARY: URLエンコード系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(uefSpaceToPlus, Encoding::uefSpaceToPlus);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(uefEncodeTilde, Encoding::uefEncodeTilde);
	//
	//	SUMMARY:  URLエンコード系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(URLEncodeToOctet)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		switch (SPRIG_KRKR_ARG_TYPE(1)) {
		case tvtString:
			if (numparams >= 3) {
				SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
				SPRIG_KRKR_RESULT_SET(
					Encoding::URLEncodeToOctet(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
					);
			} else {
				SPRIG_KRKR_RESULT_SET(
					Encoding::URLEncodeToOctet(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
					);
			}
			break;
		case tvtOctet:
			SPRIG_KRKR_NUMPARAMS_CHECK(3);
			SPRIG_KRKR_TYPE_CHECK(2, tvtString);
			if (numparams >= 4) {
				SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
				SPRIG_KRKR_RESULT_SET(
					Encoding::URLEncodeToOctet(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_STRING(2), SPRIG_KRKR_ARG_VARIANT(3))
					);
			} else {
				SPRIG_KRKR_RESULT_SET(
					Encoding::URLEncodeToOctet(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_STRING(2))
					);
			}
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(URLEncodeToOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(URLEncodeToString)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		switch (SPRIG_KRKR_ARG_TYPE(1)) {
		case tvtString:
			if (numparams >= 3) {
				SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
				SPRIG_KRKR_RESULT_SET(
					Encoding::URLEncodeToString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
					);
			} else {
				SPRIG_KRKR_RESULT_SET(
					Encoding::URLEncodeToString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
					);
			}
			break;
		case tvtOctet:
			SPRIG_KRKR_NUMPARAMS_CHECK(3);
			SPRIG_KRKR_TYPE_CHECK(2, tvtString);
			if (numparams >= 4) {
				SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
				SPRIG_KRKR_RESULT_SET(
					Encoding::URLEncodeToString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_STRING(2), SPRIG_KRKR_ARG_VARIANT(3))
					);
			} else {
				SPRIG_KRKR_RESULT_SET(
					Encoding::URLEncodeToString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_STRING(2))
					);
			}
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(URLEncodeToString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(URLDecodeToOctet)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		switch (SPRIG_KRKR_ARG_TYPE(1)) {
		case tvtString:
			SPRIG_KRKR_RESULT_SET(
				Encoding::URLDecodeToOctet(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
				);
			break;
		case tvtOctet:
			SPRIG_KRKR_NUMPARAMS_CHECK(3);
			SPRIG_KRKR_TYPE_CHECK(2, tvtString);
			SPRIG_KRKR_RESULT_SET(
				Encoding::URLDecodeToOctet(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_STRING(2))
				);
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(URLDecodeToOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(URLDecodeToString)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		switch (SPRIG_KRKR_ARG_TYPE(1)) {
		case tvtString:
			SPRIG_KRKR_RESULT_SET(
				Encoding::URLDecodeToString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
				);
			break;
		case tvtOctet:
			SPRIG_KRKR_NUMPARAMS_CHECK(3);
			SPRIG_KRKR_TYPE_CHECK(2, tvtString);
			SPRIG_KRKR_RESULT_SET(
				Encoding::URLDecodeToString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_STRING(2))
				);
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(URLDecodeToString);
	//
	//	SUMMARY: Base64エンコード系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(base64EncodeToOctet)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				Encoding::base64EncodeToOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Encoding::base64EncodeToOctet(SPRIG_KRKR_ARG_OCTET(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(base64EncodeToOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(base64EncodeToString)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				Encoding::base64EncodeToString(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Encoding::base64EncodeToString(SPRIG_KRKR_ARG_OCTET(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(base64EncodeToString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(base64DecodeToOctet)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		switch (SPRIG_KRKR_ARG_TYPE(0)) {
		case tvtString:
			SPRIG_KRKR_RESULT_SET(
				Encoding::base64DecodeToOctet(SPRIG_KRKR_ARG_STRING(0))
				);
			break;
		case tvtOctet:
			SPRIG_KRKR_RESULT_SET(
				Encoding::base64DecodeToOctet(SPRIG_KRKR_ARG_OCTET(0))
				);
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(base64DecodeToOctet);
	//
	//	SUMMARY: エンコード系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(encodeToOctet)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtOctet);
			SPRIG_KRKR_TYPE_CHECK(2, tvtString);
			SPRIG_KRKR_RESULT_SET(
				Encoding::encodeToOctet(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_STRING(2))
				);
		} else {
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_RESULT_SET(
				Encoding::encodeToOctet(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(encodeToOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(encodeToString)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		SPRIG_KRKR_RESULT_SET(
			Encoding::encodeToString(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_STRING(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(encodeToString);
	//
	//	SUMMARY: 比較系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(sameCharsetAlias)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		SPRIG_KRKR_RESULT_SET(
			Encoding::sameCharsetAlias(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(sameCharsetAlias);
	//
	//	SUMMARY: 検出系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(detect)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		if (numparams >= 3) {
			SPRIG_KRKR_RESULT_SET(
				Encoding::detect(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				Encoding::detect(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Encoding::detect(SPRIG_KRKR_ARG_OCTET(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(detect);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(detectAll)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		if (numparams >= 3) {
			SPRIG_KRKR_RESULT_SET(
				Encoding::detectAll(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				Encoding::detectAll(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Encoding::detectAll(SPRIG_KRKR_ARG_OCTET(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(detectAll);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(checkDetected)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtOctet);
		if (numparams >= 4) {
			SPRIG_KRKR_RESULT_SET(
				Encoding::checkDetected(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3))
				);
		} else if (numparams >= 3) {
			SPRIG_KRKR_RESULT_SET(
				Encoding::checkDetected(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Encoding::checkDetected(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(checkDetected);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_encoding
// unregist_ktl_encoding
//
void regist_ktl_encoding() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Encoding:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Encoding"), ktl::CreateNativeClassEncoding());
	}
}
void unregist_ktl_encoding() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Encoding:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Encoding"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_encoding);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_encoding);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Encoding:information"));
