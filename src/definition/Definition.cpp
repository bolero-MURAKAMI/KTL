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

#include "Definition.hpp"

namespace ktl {
	//
	// CreateNativeClassDefinition
	//
#define TJS_NATIVE_CLASSID_NAME DefinitionClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassDefinition, Definition, Definition)
	//
	// 定義系メソッド
	//
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(Definition, class_, class_);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassClassDefinition
	//
#define TJS_NATIVE_CLASSID_NAME ClassDefinitionClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassClassDefinition, ClassDefinition, ClassDefinition)
	//
	// 定義系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(ClassDefinition, endDefinition, endDefinition);
	//
	// クラス定義系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(name)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ClassDefinition);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->name(SPRIG_KRKR_ARG_STRING(0));
		if (result) {
			*result = tTJSVariant(objthis, objthis);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(name);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(extends_)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ClassDefinition);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		this_->extends_(SPRIG_KRKR_ARG_OBJECT(0));
		if (result) {
			*result = tTJSVariant(objthis, objthis);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(extends_);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(funcCall)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ClassDefinition);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		this_->funcCall(SPRIG_KRKR_ARG_OBJECT(0));
		if (result) {
			*result = tTJSVariant(objthis, objthis);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(funcCall);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(propGetByNum)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ClassDefinition);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		this_->propGetByNum(SPRIG_KRKR_ARG_OBJECT(0));
		if (result) {
			*result = tTJSVariant(objthis, objthis);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(propGetByNum);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(propSetByNum)
	{
		TJS_GET_NATIVE_INSTANCE(this_, ClassDefinition);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		this_->propSetByNum(SPRIG_KRKR_ARG_OBJECT(0));
		if (result) {
			*result = tTJSVariant(objthis, objthis);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(propSetByNum);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_definition, unregist_ktl_definition
//
static void regist_ktl_definition() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Definition:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddNewMember(global, SPRIG_KRKR_TJS_W("Definition"), ktl::CreateNativeClassDefinition());
		sprig::krkr::tjs::AddNewMember(global, SPRIG_KRKR_TJS_W("ClassDefinition"), ktl::CreateNativeClassClassDefinition());
	}
}
static void unregist_ktl_definition() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Definition:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global, SPRIG_KRKR_TJS_W("Definition"));
		sprig::krkr::tjs::DeleteMember(global, SPRIG_KRKR_TJS_W("ClassDefinition"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_definition);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_definition);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Definition:information"));
