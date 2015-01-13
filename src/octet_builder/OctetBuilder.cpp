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

#include "OctetBuilder.hpp"

namespace ktl {
	//
	// CreateNativeClassOctetBuilder
	//
#define TJS_NATIVE_CLASSID_NAME OctetBuilderClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassOctetBuilder, OctetBuilder, OctetBuilder)
	//
	//	SUMMARY: タイプ系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfMask, OctetBuilder::vtfMask);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfByte, OctetBuilder::vtfByte);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfInt8, OctetBuilder::vtfInt8);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfInt16, OctetBuilder::vtfInt16);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfInt32, OctetBuilder::vtfInt32);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfInt64, OctetBuilder::vtfInt64);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfUInt8, OctetBuilder::vtfUInt8);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfUInt16, OctetBuilder::vtfUInt16);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfUInt32, OctetBuilder::vtfUInt32);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfUInt64, OctetBuilder::vtfUInt64);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfFloat, OctetBuilder::vtfFloat);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfDouble, OctetBuilder::vtfDouble);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfOctet, OctetBuilder::vtfOctet);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfString, OctetBuilder::vtfString);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfNarrowString, OctetBuilder::vtfNarrowString);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vtfBuilder, OctetBuilder::vtfBuilder);
	//
	//	SUMMARY: エンディアン系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vefMask, OctetBuilder::vefMask);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vefBigEndian, OctetBuilder::vefBigEndian);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(vefLittleEndian, OctetBuilder::vefLittleEndian);
	//
	//	SUMMARY: 生成系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(fromHex)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (result) {
			sprig::krkr::tjs::object_type obj = OctetBuilder::fromHex(SPRIG_KRKR_ARG_STRING(0));
			*result = tTJSVariant(obj.get(), obj.get());
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(fromHex);
	//
	//	SUMMARY: 内部系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(OctetBuilder, instance, getInstance);
	//
	//	SUMMARY: 書込系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(set)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->set(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->set(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(set);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->setByte(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setBuilder)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_INSTANCEOF_CHECK(1, SPRIG_KRKR_TJS_W("OctetBuilder"));
		if (numparams >= 4) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setBuilder(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OBJECT(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3))
				);
		} else if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setBuilder(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OBJECT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setBuilder(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OBJECT(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setBuilder);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(1, tvtOctet);
		if (numparams >= 4) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3))
				);
		} else if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OCTET(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		if (numparams >= 4) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3))
				);
		} else if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		if (numparams >= 4) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setNarrowString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3))
				);
		} else if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setNarrowString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setNarrowString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setStorage)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		if (numparams >= 4) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setStorage(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3))
				);
		} else if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setStorage(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->setStorage(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setStorage);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setBlock)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->setBlock(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setBlock);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setBytes)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->setBytes(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setBytes);
	//
	//	SUMMARY: 割当系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assign)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			this_->assign(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1));
		} else {
			this_->assign(SPRIG_KRKR_ARG(0));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assign);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assignByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		this_->assignByte(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assignByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assignBuilder)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INSTANCEOF_CHECK(0, SPRIG_KRKR_TJS_W("OctetBuilder"));
		if (numparams >= 3) {
			this_->assignBuilder(SPRIG_KRKR_ARG_OBJECT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else if (numparams >= 2) {
			this_->assignBuilder(SPRIG_KRKR_ARG_OBJECT(0), SPRIG_KRKR_ARG_VARIANT(1));
		} else {
			this_->assignBuilder(SPRIG_KRKR_ARG_OBJECT(0));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assignBuilder);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assignOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		if (numparams >= 3) {
			this_->assignOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else if (numparams >= 2) {
			this_->assignOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1));
		} else {
			this_->assignOctet(SPRIG_KRKR_ARG_OCTET(0));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assignOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assignString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			this_->assignString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else if (numparams >= 2) {
			this_->assignString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1));
		} else {
			this_->assignString(SPRIG_KRKR_ARG_STRING(0));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assignString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assignNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			this_->assignNarrowString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else if (numparams >= 2) {
			this_->assignNarrowString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1));
		} else {
			this_->assignNarrowString(SPRIG_KRKR_ARG_STRING(0));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assignNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assignStorage)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			this_->assignStorage(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else if (numparams >= 2) {
			this_->assignStorage(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1));
		} else {
			this_->assignStorage(SPRIG_KRKR_ARG_STRING(0));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assignStorage);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assignBlock)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		this_->assignBlock(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assignBlock);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assignBytes)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
		this_->assignBytes(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assignBytes);
	//
	//	SUMMARY: 挿入系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insert)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
			this_->insert(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else {
			this_->insert(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG(1));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(insert);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insertByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		this_->insertByte(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(insertByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insertBuilder)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_INSTANCEOF_CHECK(1, SPRIG_KRKR_TJS_W("OctetBuilder"));
		if (numparams >= 4) {
			this_->insertBuilder(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OBJECT(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3));
		} else if (numparams >= 3) {
			this_->insertBuilder(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OBJECT(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else {
			this_->insertBuilder(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OBJECT(1));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(insertBuilder);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insertOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(1, tvtOctet);
		if (numparams >= 4) {
			this_->insertOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3));
		} else if (numparams >= 3) {
			this_->insertOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OCTET(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else {
			this_->insertOctet(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_OCTET(1));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(insertOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insertString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		if (numparams >= 4) {
			this_->insertString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3));
		} else if (numparams >= 3) {
			this_->insertString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else {
			this_->insertString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(insertString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insertNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		if (numparams >= 4) {
			this_->insertNarrowString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3));
		} else if (numparams >= 3) {
			this_->insertNarrowString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else {
			this_->insertNarrowString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(insertNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insertStorage)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		if (numparams >= 4) {
			this_->insertStorage(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3));
		} else if (numparams >= 3) {
			this_->insertStorage(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2));
		} else {
			this_->insertStorage(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_STRING(1));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setStorage);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insertBlock)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
		this_->insertBlock(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(insertBlock);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insertBytes)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		this_->insertBytes(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(insertBytes);
	//
	//	SUMMARY: 追加系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(add)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->add(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->add(SPRIG_KRKR_ARG(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(add);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(addByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->addByte(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(addByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(addBuilder)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INSTANCEOF_CHECK(0, SPRIG_KRKR_TJS_W("OctetBuilder"));
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addBuilder(SPRIG_KRKR_ARG_OBJECT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addBuilder(SPRIG_KRKR_ARG_OBJECT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addBuilder(SPRIG_KRKR_ARG_OBJECT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(addBuilder);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(addOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addOctet(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addOctet(SPRIG_KRKR_ARG_OCTET(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(addOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(addString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addString(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(addString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(addNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addNarrowString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addNarrowString(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addNarrowString(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(addNarrowString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(addStorage)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addStorage(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addStorage(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->addStorage(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(addStorage);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(addBlock)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		this_->addBlock(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(addBlock);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(addBytes)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
		this_->addBytes(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(addBytes);
	//
	//	SUMMARY: 初期化系メソッド
	//
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(OctetBuilder, clear);
	//
	//	SUMMARY: 削除系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(erase)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			this_->erase(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
		} else {
			this_->erase(SPRIG_KRKR_ARG_VARIANT(0));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(erase);
	//
	//	SUMMARY: 前後削除系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(pop)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->pop(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->pop()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(pop);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(shift)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		if (numparams >= 1) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->shift(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->shift()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(shift);
	//
	//	SUMMARY: 変換系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(as)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_RESULT_SET(
				this_->as(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->as(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(as);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asBuilder)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->asBuilder(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->asBuilder(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->asBuilder()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asBuilder);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
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
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->asString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->asString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->asString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(asNarrowString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->asNarrowString(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_RESULT_SET(
				this_->asNarrowString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->asNarrowString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(asNarrowString);
	//
	//	SUMMARY: ストレージ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveStorage)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
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
	//
	//	SUMMARY: 演算系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(transformAndByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->transformAndByte(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(transformAndByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(transformOrByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->transformOrByte(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(transformOrByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(transformXorByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->transformXorByte(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(transformXorByte);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(transformNotByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->transformNotByte()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(transformNotByte);
	//
	//	SUMMARY: 検索系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(findByte)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		if (numparams >= 3) {
			SPRIG_KRKR_RESULT_SET(
				this_->findByte(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_RESULT_SET(
				this_->findByte(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->findByte(SPRIG_KRKR_ARG_VARIANT(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(findByte);
	//
	//	SUMMARY: メモリ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(copy)
	{
		TJS_GET_NATIVE_INSTANCE(this_, OctetBuilder);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		this_->copy(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(copy);
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(OctetBuilder, back, back);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(OctetBuilder, front, front);
	//
	//	SUMMARY: ポインタ取得プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(OctetBuilder, data, begin);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(OctetBuilder, begin, begin);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(OctetBuilder, end, end);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(OctetBuilder, empty, empty);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(OctetBuilder, size, size, resize);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(OctetBuilder, length, size, resize);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(OctetBuilder, count, size, resize);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_octet_builder
// unregist_ktl_octet_builder
//
static void regist_ktl_octet_builder() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:OctetBuilder:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("OctetBuilder"), ktl::CreateNativeClassOctetBuilder());
	}
}
static void unregist_ktl_octet_builder() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:OctetBuilder:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("OctetBuilder"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_octet_builder);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_octet_builder);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:OctetBuilder:information"));
