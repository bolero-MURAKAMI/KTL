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

#include "Serialization.hpp"

namespace ktl {
	//
	// CreateNativeClassSerializationObject
	//
#define TJS_NATIVE_CLASSID_NAME SerializationObjectClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassSerializationObject, SerializationObject, SerializationObject)
	//
	//	SUMMARY: シリアライザ管理系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(insert)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SerializationObject);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_INVOKE_RESULT_SET(this_->insert(
			SPRIG_KRKR_ARG_VARIANT(0),
			SPRIG_KRKR_ARG(1)
			));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(insert);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VARIANT_RES_VOID(SerializationObject, erase);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(SerializationObject, clear);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VARIANT(SerializationObject, size);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(exist)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SerializationObject);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_RESULT_SET(this_->exist(SPRIG_KRKR_ARG_VARIANT(0)));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(exist);
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(save)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SerializationObject);
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(3);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		if (numparams >= 4) {
			SPRIG_KRKR_TYPE_CHECK(3, tvtString);
			this_->save(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG(2), static_cast<tTJSString>(SPRIG_KRKR_ARG_VARIANT(3)));
		} else {
			this_->save(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG(2));
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(save);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(load)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SerializationObject);
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		if (numparams >= 3) {
			this_->load(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG(2), result);
		} else {
			this_->load(SPRIG_KRKR_ARG(0), SPRIG_KRKR_ARG_VARIANT(1), 0, result);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(load);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_serialization
// unregist_ktl_serialization
//
static void regist_ktl_serialization() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Serialization:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::object_type serialization_object(ktl::CreateNativeClassSerializationObject(), false);
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("SerializationObject"), tTJSVariant(serialization_object.get()));
		{
			sprig::krkr::tjs::object_type serialization(
				sprig::krkr::tjs::CreateNewObject(
					serialization_object.get(),
					0, 0, 0
					),
				false
				);
			sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Serialization"), tTJSVariant(serialization.get(), serialization.get()));
		}
	}
}
static void unregist_ktl_serialization() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Serialization:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Serialization"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("SerializationObject"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_serialization);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_serialization);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Serialization:information"));
