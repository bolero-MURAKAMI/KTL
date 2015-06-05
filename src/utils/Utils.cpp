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

#include "Utils.hpp"

namespace ktl {
	//
	// CreateNativeClassUtils
	//
#define TJS_NATIVE_CLASSID_NAME UtilsClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassUtils, Utils, Utils)
	//
	//	SUMMARY: 定数系プロパティ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(sizeofWChar, Utils::sizeofWChar);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(sizeofNChar, Utils::sizeofNChar);
	//
	//	SUMMARY: オブジェクト系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getCii)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		if (numparams >= 2) {
			SPRIG_KRKR_NUMPARAMS_CHECK_FOR(1, 2);
			SPRIG_KRKR_RESULT_SET(
				Utils::getCii(SPRIG_KRKR_ARG_OBJECT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Utils::getCii(SPRIG_KRKR_ARG_OBJECT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getCii);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getCount)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_RESULT_SET(
			Utils::getCount(SPRIG_KRKR_ARG_CLOSURE(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getCount);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getContext)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_RESULT_SET(
			Utils::getContext(SPRIG_KRKR_ARG_CLOSURE(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getContext);
	//
	//	SUMMARY: オクテット系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getOctetData)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		SPRIG_KRKR_RESULT_SET(
			Utils::getOctetData(SPRIG_KRKR_ARG_OCTET(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getOctetData);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getOctetSize)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		SPRIG_KRKR_RESULT_SET(
			Utils::getOctetSize(SPRIG_KRKR_ARG_OCTET(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getOctetSize);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(beginOctet)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		SPRIG_KRKR_RESULT_SET(
			Utils::beginOctet(SPRIG_KRKR_ARG_OCTET(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(beginOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(endOctet)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		SPRIG_KRKR_RESULT_SET(
			Utils::endOctet(SPRIG_KRKR_ARG_OCTET(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(endOctet);
	//
	//	SUMMARY: 文字列系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getStringData)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			Utils::getStringData(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getStringData);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getStringSize)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			Utils::getStringSize(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getStringSize);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(beginString)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			Utils::beginString(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(beginString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(endString)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			Utils::endString(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(endString);
	//
	//	SUMMARY: 変換系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(toString)
	{
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(1);
		SPRIG_KRKR_RESULT_SET(
			Utils::toString(SPRIG_KRKR_ARG(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(toString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(toReadableString)
	{
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_NUMPARAMS_CHECK_FOR(1, 2);
			SPRIG_KRKR_RESULT_SET(
				Utils::toReadableString(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Utils::toReadableString(SPRIG_KRKR_ARG(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(toReadableString);
	//
	//	SUMMARY: ストレージ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadStorageToOctet)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			Utils::loadStorageToOctet(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadStorageToOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveStorageFromOctet)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtOctet);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			Utils::saveStorageFromOctet(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OCTET(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveStorageFromOctet);
	//
	//	SUMMARY: 列挙系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(enumMembers)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_RESULT_SET(
			Utils::enumMembers(SPRIG_KRKR_ARG_CLOSURE(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(enumMembers);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(enumMembersName)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_RESULT_SET(
			Utils::enumMembersName(SPRIG_KRKR_ARG_CLOSURE(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(enumMembersName);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_utils, unregist_ktl_utils
//
static void regist_ktl_utils() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Utils:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddNewMember(global, SPRIG_KRKR_TJS_W("Utils"), ktl::CreateNativeClassUtils());
	}
}
static void unregist_ktl_utils() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Utils:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global, SPRIG_KRKR_TJS_W("Utils"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_utils);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_utils);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Utils:information"));
