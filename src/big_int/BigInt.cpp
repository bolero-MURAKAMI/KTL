/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/ntl/error.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "BigInt.hpp"

namespace ktl {
	//
	// CreateNativeClassBigInt
	//
#define TJS_NATIVE_CLASSID_NAME BigIntClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassBigInt, BigInt, BigInt)
	//
	//	SUMMARY: 内部系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(BigInt, instance, getInstance);
	//
	// KTL_BIG_INT_NATIVE_METHOD_OPERATION_DECL
	//
#define KTL_BIG_INT_NATIVE_METHOD_OPERATION_DECL(FUNC_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(FUNC_NAME) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		if (result) { \
			sprig::krkr::tjs::object_type obj = BigInt::FUNC_NAME(numparams, param); \
			*result = tTJSVariant(obj.get(), obj.get()); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(FUNC_NAME);
	//
	//	SUMMARY: 演算系メソッド
	//
	KTL_BIG_INT_NATIVE_METHOD_OPERATION_DECL(opAdd);
	KTL_BIG_INT_NATIVE_METHOD_OPERATION_DECL(opSub);
	KTL_BIG_INT_NATIVE_METHOD_OPERATION_DECL(opMul);
	KTL_BIG_INT_NATIVE_METHOD_OPERATION_DECL(opDiv);
	KTL_BIG_INT_NATIVE_METHOD_OPERATION_DECL(opMod);
	KTL_BIG_INT_NATIVE_METHOD_OPERATION_DECL(opOr);
	KTL_BIG_INT_NATIVE_METHOD_OPERATION_DECL(opAnd);
	KTL_BIG_INT_NATIVE_METHOD_OPERATION_DECL(opXor);
	//
	// KTL_BIG_INT_NATIVE_METHOD_COMPARISON_DECL
	//
#define KTL_BIG_INT_NATIVE_METHOD_COMPARISON_DECL(FUNC_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(FUNC_NAME) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		if (result) { \
			*result = BigInt::FUNC_NAME(numparams, param); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(FUNC_NAME);
	//
	//	SUMMARY: 同定系メソッド
	//
	KTL_BIG_INT_NATIVE_METHOD_COMPARISON_DECL(opEq);
	KTL_BIG_INT_NATIVE_METHOD_COMPARISON_DECL(opNEq);
	//
	//	SUMMARY: 比較系メソッド
	//
	KTL_BIG_INT_NATIVE_METHOD_COMPARISON_DECL(opLs);
	KTL_BIG_INT_NATIVE_METHOD_COMPARISON_DECL(opLsEq);
	KTL_BIG_INT_NATIVE_METHOD_COMPARISON_DECL(opGr);
	KTL_BIG_INT_NATIVE_METHOD_COMPARISON_DECL(opGrEq);
	//
	// KTL_BIG_INT_NATIVE_METHOD_UNARY_OPERATION_DECL
	//
#define KTL_BIG_INT_NATIVE_METHOD_UNARY_OPERATION_DECL(FUNC_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(FUNC_NAME) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		if (result) { \
			sprig::krkr::tjs::object_type obj = BigInt::FUNC_NAME(SPRIG_KRKR_ARG_VARIANT(0)); \
			*result = tTJSVariant(obj.get(), obj.get()); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(FUNC_NAME);
	//
	// KTL_BIG_INT_NATIVE_METHOD_BINARY_OPERATION_DECL
	//
#define KTL_BIG_INT_NATIVE_METHOD_BINARY_OPERATION_DECL(FUNC_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(FUNC_NAME) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		if (result) { \
			sprig::krkr::tjs::object_type obj = BigInt::FUNC_NAME( \
				SPRIG_KRKR_ARG_VARIANT(0), \
				SPRIG_KRKR_ARG_VARIANT(1) \
				); \
			*result = tTJSVariant(obj.get(), obj.get()); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(FUNC_NAME);
	//
	//	SUMMARY: 単項演算系メソッド
	//
	KTL_BIG_INT_NATIVE_METHOD_UNARY_OPERATION_DECL(opPlus);
	KTL_BIG_INT_NATIVE_METHOD_UNARY_OPERATION_DECL(opMinus);
	//
	//	SUMMARY: 算術系メソッド
	//
	KTL_BIG_INT_NATIVE_METHOD_UNARY_OPERATION_DECL(abs);
	KTL_BIG_INT_NATIVE_METHOD_BINARY_OPERATION_DECL(pow);
	KTL_BIG_INT_NATIVE_METHOD_BINARY_OPERATION_DECL(gcd);
	KTL_BIG_INT_NATIVE_METHOD_BINARY_OPERATION_DECL(max);
	KTL_BIG_INT_NATIVE_METHOD_BINARY_OPERATION_DECL(min);
	//
	//	SUMMARY: 代入系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assign)
	{
		TJS_GET_NATIVE_INSTANCE(this_, BigInt);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		this_->assign(SPRIG_KRKR_ARG_VARIANT(0));
		if (result) {
			*result = tTJSVariant(objthis, objthis);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assign);
	//
	// KTL_BIG_INT_NATIVE_METHOD_ASSIGN_DECL
	//
#define KTL_BIG_INT_NATIVE_METHOD_ASSIGN_DECL(FUNC_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(FUNC_NAME) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, BigInt); \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		this_->FUNC_NAME(numparams, param); \
		if (result) { \
			*result = tTJSVariant(objthis, objthis); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(FUNC_NAME);
	//
	//	SUMMARY: 演算代入系メソッド
	//
	KTL_BIG_INT_NATIVE_METHOD_ASSIGN_DECL(assignAdd);
	KTL_BIG_INT_NATIVE_METHOD_ASSIGN_DECL(assignSub);
	KTL_BIG_INT_NATIVE_METHOD_ASSIGN_DECL(assignMul);
	KTL_BIG_INT_NATIVE_METHOD_ASSIGN_DECL(assignDiv);
	KTL_BIG_INT_NATIVE_METHOD_ASSIGN_DECL(assignMod);
	KTL_BIG_INT_NATIVE_METHOD_ASSIGN_DECL(assignOr);
	KTL_BIG_INT_NATIVE_METHOD_ASSIGN_DECL(assignAnd);
	KTL_BIG_INT_NATIVE_METHOD_ASSIGN_DECL(assignXor);
	//
	//	SUMMARY: 変換系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(BigInt, asInt, asInt);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(BigInt, asReal, asReal);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(BigInt, asString, asString);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(BigInt, asOctet, asOctet);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(BigInt, sign, sign);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_big_int
// unregist_ktl_big_int
//
static void regist_ktl_big_int() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:BigInt:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("BigInt"), ktl::CreateNativeClassBigInt());
	}
}
static void unregist_ktl_big_int() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:BigInt:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("BigInt"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_big_int);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_big_int);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:BigInt:information"));

//
// enable_sprig_ntl_error_handling
//
static void enable_sprig_ntl_error_handling() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Sprig:ntl:error_handling:enable"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::ntl::error_handling::enable();
}
NCB_PRE_REGIST_CALLBACK(enable_sprig_ntl_error_handling);
