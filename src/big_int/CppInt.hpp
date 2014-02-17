/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_BIG_INT_CPP_INT_HPP
#define SRC_BIG_INT_CPP_INT_HPP

#include <iterator>
#include <vector>
#include <algorithm>
#include <string>
#include <boost/multiprecision/number.hpp>
#include <boost/math/common_factor.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/ref.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/range/algorithm_ext/is_sorted.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/mpl/identity.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/get_pointer.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/algorithm.hpp>
#include <sprig/exception.hpp>
#include <sprig/section.hpp>
#include <sprig/ntl/big_int.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "CppIntDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:CppInt:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:CppInt:error")

namespace ktl {
	//
	// NativeCppInt
	//
	KTL_INLINE NativeCppInt::big_int_type NativeCppInt::convertValue(tTJSVariant const& v) {
		switch (v.Type()) {
		case tvtVoid:
			return big_int_type();
		case tvtObject:
			if (sprig::krkr::tjs::IsInstanceObjectOf(v.AsObjectNoAddRef(), SPRIG_KRKR_TJS_W("CppInt"))) {
				return reinterpret_cast<CppInt*>(getInstance(v.AsObjectNoAddRef()))->get();
			} else {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("対応しないオブジェクトの引数は無効です"),
					sprig::krkr::bad_argument
					);
				return big_int_type();
			}
		case tvtString:
			return big_int_type(sprig::str_cast<std::string>(sprig::krkr::tjs::as_c_str(v.AsStringNoAddRef())));
		case tvtOctet:
			{
				tTJSVariantOctet const* v_octet = v.AsOctetNoAddRef();
				return big_int_type(
					sprig::str_cast<std::string>(
						sprig::big_int(sprig::krkr::tjs::octet_data(v_octet), sprig::krkr::tjs::octet_length(v_octet))
						)
					);
			}
		case tvtInteger:
			return big_int_type(static_cast<tTVInteger>(v));
		case tvtReal:
			return big_int_type(static_cast<tTVReal>(v));
		}
		return big_int_type();
	}
	KTL_INLINE NativeCppInt::big_int_type NativeCppInt::convertValue(tTJSVariant const& v, int sign) {
		switch (v.Type()) {
		case tvtOctet:
			{
				sprig::big_int::sign s = sign < 0
					? sprig::big_int::sign_minus
					: sign > 0
						? sprig::big_int::sign_plus
						: sprig::big_int::sign_zero
					;
				tTJSVariantOctet const* v_octet = v.AsOctetNoAddRef();
				return big_int_type(
					sprig::str_cast<std::string>(
						sprig::big_int(sprig::krkr::tjs::octet_data(v_octet), sprig::krkr::tjs::octet_length(v_octet), s)
						)
					);
			}
		default:
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("符号付変換はオクテット列のみ対応します"),
				sprig::krkr::bad_argument
				);
			return big_int_type();
		}
	}
	KTL_INLINE NativeCppInt::big_int_type NativeCppInt::convertValuePtr(tTJSVariant const* v) {
		return convertValue(*v);
	}
	KTL_INLINE sprig::krkr::tjs::intptr_type NativeCppInt::getInstance(iTJSDispatch2* obj) {
		return sprig::krkr::tjs::GetPropValue<sprig::krkr::tjs::intptr_type>(
			sprig::krkr::tjs::GetMemberNoAddRef(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("CppInt")),
				SPRIG_KRKR_TJS_W("instance")
				),
			obj
			);
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeCppInt::createNew(tjs_int numparams, tTJSVariant** param) {
		iTJSDispatch2* result_obj = 0;
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("CppInt")),
			&result_obj, numparams, param, 0
			);
		return sprig::krkr::tjs::object_type(result_obj, false);
	}
	NativeCppInt::NativeCppInt() {}
	NativeCppInt::NativeCppInt(big_int_type const& value)
		: value_(value)
	{}
	NativeCppInt::NativeCppInt(tTJSVariant const& v)
		: value_(convertValue(v))
	{}
	NativeCppInt::NativeCppInt(tTJSVariant const& v, int sign)
		: value_(convertValue(v, sign))
	{}
	KTL_INLINE void NativeCppInt::reset(big_int_type const& value) {
		value_ = value;
	}
	KTL_INLINE NativeCppInt::big_int_type const& NativeCppInt::get() const {
		return value_;
	}
	KTL_INLINE NativeCppInt::big_int_type& NativeCppInt::ref() {
		return value_;
	}
	//
	// KTL_CPP_INT_INTERIOR_OPERATION_DECL
	//
#define KTL_CPP_INT_INTERIOR_OPERATION_DECL(FUNC_NAME, OP) \
	KTL_INLINE sprig::krkr::tjs::object_type NativeCppInt::FUNC_NAME(tjs_int numparams, tTJSVariant** param) { \
		if (numparams < 2) { \
			KTL_ERROR( \
				KTL_ERROR_SECTION, \
				SPRIG_KRKR_TJS_W("引数が不足しています"), \
				sprig::krkr::bad_argument \
				); \
			return 0; \
		} \
		sprig::krkr::tjs::object_type result = createNew(1, param); \
		CppInt* ptr = reinterpret_cast<CppInt*>(getInstance(sprig::get_pointer(result))); \
		for (int i = 1; i != numparams; ++i) { \
			ptr->ref() OP convertValue(*param[i]); \
		} \
		return result; \
	}
	//
	//	SUMMARY: 演算系メソッド
	//
	KTL_CPP_INT_INTERIOR_OPERATION_DECL(opAdd, +=);
	KTL_CPP_INT_INTERIOR_OPERATION_DECL(opSub, -=);
	KTL_CPP_INT_INTERIOR_OPERATION_DECL(opMul, *=);
	KTL_CPP_INT_INTERIOR_OPERATION_DECL(opDiv, /=);
	KTL_CPP_INT_INTERIOR_OPERATION_DECL(opMod, %=);
	KTL_CPP_INT_INTERIOR_OPERATION_DECL(opOr, |=);
	KTL_CPP_INT_INTERIOR_OPERATION_DECL(opAnd, &=);
	KTL_CPP_INT_INTERIOR_OPERATION_DECL(opXor, ^=);
	//
	//	SUMMARY: 同定系メソッド
	//
	KTL_INLINE bool NativeCppInt::opEq(tjs_int numparams, tTJSVariant** param) {
		if (numparams < 2) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("引数が不足しています"),
				sprig::krkr::bad_argument
				);
			return 0;
		}
		std::vector<big_int_type> seq;
		std::transform(
			param,
			param + numparams,
			std::back_inserter(seq),
			&convertValuePtr
			);
		return sprig::is_equality(seq);
	}
	KTL_INLINE bool NativeCppInt::opNEq(tjs_int numparams, tTJSVariant** param) {
		if (numparams < 2) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("引数が不足しています"),
				sprig::krkr::bad_argument
				);
			return 0;
		}
		std::vector<big_int_type> seq;
		std::transform(
			param,
			param + numparams,
			std::back_inserter(seq),
			&convertValuePtr
			);
		return sprig::is_unique(seq);
	}
	//
	// KTL_CPP_INT_INTERIOR_COMPARISON_DECL
	//
#define KTL_CPP_INT_INTERIOR_COMPARISON_DECL(FUNC_NAME, OP) \
	KTL_INLINE bool NativeCppInt::FUNC_NAME(tjs_int numparams, tTJSVariant** param) { \
		if (numparams < 2) { \
			KTL_ERROR( \
				KTL_ERROR_SECTION, \
				SPRIG_KRKR_TJS_W("引数が不足しています"), \
				sprig::krkr::bad_argument \
				); \
			return 0; \
		} \
		std::vector<big_int_type> seq; \
		std::transform( \
			param, \
			param + numparams, \
			std::back_inserter(seq), \
			&convertValuePtr \
			); \
			return boost::range::is_sorted(seq, (boost::lambda::_1 OP boost::lambda::_2)); \
	}
	//
	//	SUMMARY: 比較系メソッド
	//
	KTL_CPP_INT_INTERIOR_COMPARISON_DECL(opLs, <);
	KTL_CPP_INT_INTERIOR_COMPARISON_DECL(opLsEq, <=);
	KTL_CPP_INT_INTERIOR_COMPARISON_DECL(opGr, >);
	KTL_CPP_INT_INTERIOR_COMPARISON_DECL(opGrEq, >=);
	//
	// KTL_CPP_INT_INTERIOR_UNARY_OPERATION_DECL
	//
#define KTL_CPP_INT_INTERIOR_UNARY_OPERATION_DECL(FUNC_NAME, OP) \
	KTL_INLINE sprig::krkr::tjs::object_type NativeCppInt::FUNC_NAME(tTJSVariant const& v) { \
		sprig::krkr::tjs::object_type result = createNew(0, 0); \
		CppInt* ptr = reinterpret_cast<CppInt*>(getInstance(sprig::get_pointer(result))); \
		NativeCppInt::big_int_type t = OP convertValue(v); \
		ptr->ref().swap(t); \
		return result; \
	}
	//
	//	SUMMARY: 単項演算系メソッド
	//
	KTL_CPP_INT_INTERIOR_UNARY_OPERATION_DECL(opPlus, +);
	KTL_CPP_INT_INTERIOR_UNARY_OPERATION_DECL(opMinus, -);
	//
	//	SUMMARY: 算術系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type NativeCppInt::abs(tTJSVariant const& v) {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		CppInt* ptr = reinterpret_cast<CppInt*>(getInstance(sprig::get_pointer(result)));
		ptr->ref() = boost::multiprecision::abs(convertValue(v));
		return result;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeCppInt::pow(tTJSVariant const& v, int e) {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		CppInt* ptr = reinterpret_cast<CppInt*>(getInstance(sprig::get_pointer(result)));
		ptr->ref() = boost::multiprecision::pow(convertValue(v), e);
		return result;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeCppInt::gcd(tTJSVariant const& v1, tTJSVariant const& v2) {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		CppInt* ptr = reinterpret_cast<CppInt*>(getInstance(sprig::get_pointer(result)));
		ptr->ref() = boost::math::gcd(convertValue(v1), convertValue(v2));
		return result;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeCppInt::max(tTJSVariant const& v1, tTJSVariant const& v2) {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		CppInt* ptr = reinterpret_cast<CppInt*>(getInstance(sprig::get_pointer(result)));
		ptr->ref() = std::max(convertValue(v1), convertValue(v2));
		return result;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeCppInt::min(tTJSVariant const& v1, tTJSVariant const& v2) {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		CppInt* ptr = reinterpret_cast<CppInt*>(getInstance(sprig::get_pointer(result)));
		ptr->ref() = std::min(convertValue(v1), convertValue(v2));
		return result;
	}
	//
	//	SUMMARY: 代入系メソッド
	//
	KTL_INLINE NativeCppInt& NativeCppInt::assign(tTJSVariant const& v) {
		value_.swap(convertValue(v));
		return *this;
	}
	//
	// KTL_CPP_INT_INTERIOR_ASSIGN_DECL
	//
#define KTL_CPP_INT_INTERIOR_ASSIGN_DECL(FUNC_NAME, OP) \
	KTL_INLINE NativeCppInt& NativeCppInt::FUNC_NAME(tjs_int numparams, tTJSVariant** param) { \
		if (numparams < 1) { \
			KTL_ERROR( \
				KTL_ERROR_SECTION, \
				SPRIG_KRKR_TJS_W("引数が不足しています"), \
				sprig::krkr::bad_argument \
				); \
			return *this; \
		} \
		for (int i = 0; i != numparams; ++i) { \
			value_ OP convertValue(*param[i]); \
		} \
		return *this; \
	}
	//
	//	SUMMARY: 演算代入系メソッド
	//
	KTL_CPP_INT_INTERIOR_ASSIGN_DECL(assignAdd, +=);
	KTL_CPP_INT_INTERIOR_ASSIGN_DECL(assignSub, -=);
	KTL_CPP_INT_INTERIOR_ASSIGN_DECL(assignMul, *=);
	KTL_CPP_INT_INTERIOR_ASSIGN_DECL(assignDiv, /=);
	KTL_CPP_INT_INTERIOR_ASSIGN_DECL(assignMod, %=);
	KTL_CPP_INT_INTERIOR_ASSIGN_DECL(assignOr, |=);
	KTL_CPP_INT_INTERIOR_ASSIGN_DECL(assignAnd, &=);
	KTL_CPP_INT_INTERIOR_ASSIGN_DECL(assignXor, ^=);
	//
	//	SUMMARY: 変換系メソッド
	//
	KTL_INLINE tTVInteger NativeCppInt::asInt() const {
		return boost::lexical_cast<tTVInteger>(value_.str());
	}
	KTL_INLINE tTVReal NativeCppInt::asReal() const {
		return boost::lexical_cast<tTVReal>(value_.str());
	}
	KTL_INLINE tTJSString NativeCppInt::asString() const {
		return sprig::str_cast<tTJSString>(value_);
	}
	KTL_INLINE tTJSVariant NativeCppInt::asOctet() const {
		std::vector<tjs_uint8> bytes;
		sprig::get_bytes(sprig::big_int(value_.str()), std::back_inserter(bytes));
		return tTJSVariant(bytes.empty() ? 0 : &bytes[0], bytes.size());
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE int NativeCppInt::sign() const {
		return value_.sign();
	}

	//
	// CppInt
	//
	CppInt::CppInt() {}
	tjs_error TJS_INTF_METHOD CppInt::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("CppInt::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		if (numparams >= 2) {
			instance_ = boost::make_shared<NativeCppInt>(*param[0], *param[1]);
		} else if (numparams >= 1) {
			instance_ = boost::make_shared<NativeCppInt>(*param[0]);
		} else {
			instance_ = boost::make_shared<NativeCppInt>();
		}
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD CppInt::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("CppInt::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	KTL_INLINE void CppInt::reset(big_int_type const& value) {
		instance_->reset(value);
	}
	KTL_INLINE CppInt::big_int_type const& CppInt::get() const {
		return instance_->get();
	}
	KTL_INLINE CppInt::big_int_type& CppInt::ref() {
		return instance_->ref();
	}
	//
	//	SUMMARY: 内部系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type CppInt::getInstance() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(this);
	}
	//
	//	SUMMARY: 演算系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::opAdd(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opAdd(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::opSub(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opSub(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::opMul(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opMul(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::opDiv(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opDiv(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::opMod(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opMod(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::opOr(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opOr(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::opAnd(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opAnd(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::opXor(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opXor(numparams, param);
	}
	//
	//	SUMMARY: 同定系メソッド
	//
	KTL_INLINE bool CppInt::opEq(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opEq(numparams, param);
	}
	KTL_INLINE bool CppInt::opNEq(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opNEq(numparams, param);
	}
	//
	//	SUMMARY: 比較系メソッド
	//
	KTL_INLINE bool CppInt::opLs(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opLs(numparams, param);
	}
	KTL_INLINE bool CppInt::opLsEq(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opLsEq(numparams, param);
	}
	KTL_INLINE bool CppInt::opGr(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opGr(numparams, param);
	}
	KTL_INLINE bool CppInt::opGrEq(tjs_int numparams, tTJSVariant** param) {
		return NativeCppInt::opGrEq(numparams, param);
	}
	//
	//	SUMMARY: 単項演算系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::opPlus(tTJSVariant const& v) {
		return NativeCppInt::opPlus(v);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::opMinus(tTJSVariant const& v) {
		return NativeCppInt::opMinus(v);
	}
	//
	//	SUMMARY: 算術系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::abs(tTJSVariant const& v) {
		return NativeCppInt::abs(v);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::pow(tTJSVariant const& v, tTVInteger e) {
		return NativeCppInt::pow(v, boost::numeric_cast<int>(e));
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::gcd(tTJSVariant const& v1, tTJSVariant const& v2) {
		return NativeCppInt::gcd(v1, v2);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::max(tTJSVariant const& v1, tTJSVariant const& v2) {
		return NativeCppInt::max(v1, v2);
	}
	KTL_INLINE sprig::krkr::tjs::object_type CppInt::min(tTJSVariant const& v1, tTJSVariant const& v2) {
		return NativeCppInt::min(v1, v2);
	}
	//
	//	SUMMARY: 代入系メソッド
	//
	KTL_INLINE CppInt& CppInt::assign(tTJSVariant const& v) {
		instance_->assign(v);
		return *this;
	}
	//
	//	SUMMARY: 演算代入系メソッド
	//
	KTL_INLINE CppInt& CppInt::assignAdd(tjs_int numparams, tTJSVariant** param) {
		instance_->assignAdd(numparams, param);
		return *this;
	}
	KTL_INLINE CppInt& CppInt::assignSub(tjs_int numparams, tTJSVariant** param) {
		instance_->assignSub(numparams, param);
		return *this;
	}
	KTL_INLINE CppInt& CppInt::assignMul(tjs_int numparams, tTJSVariant** param) {
		instance_->assignMul(numparams, param);
		return *this;
	}
	KTL_INLINE CppInt& CppInt::assignDiv(tjs_int numparams, tTJSVariant** param) {
		instance_->assignDiv(numparams, param);
		return *this;
	}
	KTL_INLINE CppInt& CppInt::assignMod(tjs_int numparams, tTJSVariant** param) {
		instance_->assignMod(numparams, param);
		return *this;
	}
	KTL_INLINE CppInt& CppInt::assignOr(tjs_int numparams, tTJSVariant** param) {
		instance_->assignOr(numparams, param);
		return *this;
	}
	KTL_INLINE CppInt& CppInt::assignAnd(tjs_int numparams, tTJSVariant** param) {
		instance_->assignAnd(numparams, param);
		return *this;
	}
	KTL_INLINE CppInt& CppInt::assignXor(tjs_int numparams, tTJSVariant** param) {
		instance_->assignXor(numparams, param);
		return *this;
	}
	//
	//	SUMMARY: 変換系メソッド
	//
	KTL_INLINE tTVInteger CppInt::asInt() const {
		return instance_->asInt();
	}
	KTL_INLINE tTVReal CppInt::asReal() const {
		return instance_->asReal();
	}
	KTL_INLINE tTJSString CppInt::asString() const {
		return instance_->asString();
	}
	KTL_INLINE tTJSVariant CppInt::asOctet() const {
		return instance_->asOctet();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tTVInteger CppInt::sign() const {
		return instance_->sign();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_BIG_INT_CPP_INT_HPP
