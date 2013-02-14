
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/ntl/error.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "CppInt.hpp"

namespace ktl {
	//
	// CreateNativeClassCppInt
	//
#define TJS_NATIVE_CLASSID_NAME CppIntClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassCppInt, CppInt, CppInt)
	//
	//	SUMMARY: 内部系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(CppInt, instance, getInstance);
	//
	// KTL_CPP_INT_NATIVE_METHOD_OPERATION_DECL
	//
#define KTL_CPP_INT_NATIVE_METHOD_OPERATION_DECL(FUNC_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(FUNC_NAME) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		if (result) { \
			sprig::krkr::tjs::object_type obj = CppInt::FUNC_NAME(numparams, param); \
			*result = tTJSVariant(obj.get(), obj.get()); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(FUNC_NAME);
	//
	//	SUMMARY: 演算系メソッド
	//
	KTL_CPP_INT_NATIVE_METHOD_OPERATION_DECL(opAdd);
	KTL_CPP_INT_NATIVE_METHOD_OPERATION_DECL(opSub);
	KTL_CPP_INT_NATIVE_METHOD_OPERATION_DECL(opMul);
	KTL_CPP_INT_NATIVE_METHOD_OPERATION_DECL(opDiv);
	KTL_CPP_INT_NATIVE_METHOD_OPERATION_DECL(opMod);
	KTL_CPP_INT_NATIVE_METHOD_OPERATION_DECL(opOr);
	KTL_CPP_INT_NATIVE_METHOD_OPERATION_DECL(opAnd);
	KTL_CPP_INT_NATIVE_METHOD_OPERATION_DECL(opXor);
	//
	// KTL_CPP_INT_NATIVE_METHOD_COMPARISON_DECL
	//
#define KTL_CPP_INT_NATIVE_METHOD_COMPARISON_DECL(FUNC_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(FUNC_NAME) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		if (result) { \
			*result = CppInt::FUNC_NAME(numparams, param); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(FUNC_NAME);
	//
	//	SUMMARY: 同定系メソッド
	//
	KTL_CPP_INT_NATIVE_METHOD_COMPARISON_DECL(opEq);
	KTL_CPP_INT_NATIVE_METHOD_COMPARISON_DECL(opNEq);
	//
	//	SUMMARY: 比較系メソッド
	//
	KTL_CPP_INT_NATIVE_METHOD_COMPARISON_DECL(opLs);
	KTL_CPP_INT_NATIVE_METHOD_COMPARISON_DECL(opLsEq);
	KTL_CPP_INT_NATIVE_METHOD_COMPARISON_DECL(opGr);
	KTL_CPP_INT_NATIVE_METHOD_COMPARISON_DECL(opGrEq);
	//
	// KTL_CPP_INT_NATIVE_METHOD_UNARY_OPERATION_DECL
	//
#define KTL_CPP_INT_NATIVE_METHOD_UNARY_OPERATION_DECL(FUNC_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(FUNC_NAME) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		if (result) { \
			sprig::krkr::tjs::object_type obj = CppInt::FUNC_NAME(SPRIG_KRKR_ARG_VARIANT(0)); \
			*result = tTJSVariant(obj.get(), obj.get()); \
		} \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(FUNC_NAME);
	//
	// KTL_CPP_INT_NATIVE_METHOD_BINARY_OPERATION_DECL
	//
#define KTL_CPP_INT_NATIVE_METHOD_BINARY_OPERATION_DECL(FUNC_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(FUNC_NAME) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		if (result) { \
			sprig::krkr::tjs::object_type obj = CppInt::FUNC_NAME( \
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
	KTL_CPP_INT_NATIVE_METHOD_UNARY_OPERATION_DECL(opPlus);
	KTL_CPP_INT_NATIVE_METHOD_UNARY_OPERATION_DECL(opMinus);
	//
	//	SUMMARY: 算術系メソッド
	//
	KTL_CPP_INT_NATIVE_METHOD_UNARY_OPERATION_DECL(abs);
	KTL_CPP_INT_NATIVE_METHOD_BINARY_OPERATION_DECL(pow);
	KTL_CPP_INT_NATIVE_METHOD_BINARY_OPERATION_DECL(gcd);
	KTL_CPP_INT_NATIVE_METHOD_BINARY_OPERATION_DECL(max);
	KTL_CPP_INT_NATIVE_METHOD_BINARY_OPERATION_DECL(min);
	//
	//	SUMMARY: 代入系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assign)
	{
		TJS_GET_NATIVE_INSTANCE(this_, CppInt);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		this_->assign(SPRIG_KRKR_ARG_VARIANT(0));
		if (result) {
			*result = tTJSVariant(objthis, objthis);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assign);
	//
	// KTL_CPP_INT_NATIVE_METHOD_ASSIGN_DECL
	//
#define KTL_CPP_INT_NATIVE_METHOD_ASSIGN_DECL(FUNC_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(FUNC_NAME) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CppInt); \
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
	KTL_CPP_INT_NATIVE_METHOD_ASSIGN_DECL(assignAdd);
	KTL_CPP_INT_NATIVE_METHOD_ASSIGN_DECL(assignSub);
	KTL_CPP_INT_NATIVE_METHOD_ASSIGN_DECL(assignMul);
	KTL_CPP_INT_NATIVE_METHOD_ASSIGN_DECL(assignDiv);
	KTL_CPP_INT_NATIVE_METHOD_ASSIGN_DECL(assignMod);
	KTL_CPP_INT_NATIVE_METHOD_ASSIGN_DECL(assignOr);
	KTL_CPP_INT_NATIVE_METHOD_ASSIGN_DECL(assignAnd);
	KTL_CPP_INT_NATIVE_METHOD_ASSIGN_DECL(assignXor);
	//
	//	SUMMARY: 変換系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(CppInt, asInt, asInt);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(CppInt, asReal, asReal);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(CppInt, asString, asString);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(CppInt, asOctet, asOctet);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(CppInt, sign, sign);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_cpp_int
// unregist_ktl_cpp_int
//
static void regist_ktl_cpp_int() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:CppInt:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("CppInt"), ktl::CreateNativeClassCppInt());
	}
}
static void unregist_ktl_cpp_int() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:CppInt:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("CppInt"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_cpp_int);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_cpp_int);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:CppInt:information"));

//
// enable_sprig_ntl_error_handling
//
static void enable_sprig_ntl_error_handling() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Sprig:ntl:error_handling:enable"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::ntl::error_handling::enable();
}
NCB_PRE_REGIST_CALLBACK(enable_sprig_ntl_error_handling);
