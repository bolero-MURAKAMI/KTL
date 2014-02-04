/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_BIG_INT_BIG_INT_HPP
#define SRC_BIG_INT_BIG_INT_HPP

#include <iterator>
#include <vector>
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

#include "BigIntDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:BigInt:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:BigInt:error")

namespace ktl {
	//
	// NativeBigInt
	//
	KTL_INLINE NativeBigInt::big_int_type NativeBigInt::convertValue(tTJSVariant const& v) {
		switch (v.Type()) {
		case tvtVoid:
			return big_int_type::zero();
		case tvtObject:
			if (sprig::krkr::tjs::IsInstanceObjectOf(v.AsObjectNoAddRef(), SPRIG_KRKR_TJS_W("BigInt"))) {
				return reinterpret_cast<BigInt*>(getInstance(v.AsObjectNoAddRef()))->get();
			} else {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("対応しないオブジェクトの引数は無効です"),
					sprig::krkr::bad_argument
					);
				return big_int_type::zero();
			}
		case tvtString:
			return static_cast<tTJSString>(v).c_str();
		case tvtOctet:
			{
				tTJSVariantOctet const* v_octet = v.AsOctetNoAddRef();
				return big_int_type(sprig::krkr::tjs::octet_data(v_octet), sprig::krkr::tjs::octet_length(v_octet));
			}
		case tvtInteger:
			return static_cast<tTVInteger>(v);
		case tvtReal:
			return static_cast<tTVReal>(v);
		}
		return big_int_type::zero();
	}
	KTL_INLINE NativeBigInt::big_int_type NativeBigInt::convertValue(tTJSVariant const& v, int sign) {
		switch (v.Type()) {
		case tvtOctet:
			{
				big_int_type::sign s = sign < 0
					? big_int_type::sign_minus
					: sign > 0
						? big_int_type::sign_plus
						: big_int_type::sign_zero
					;
				tTJSVariantOctet const* v_octet = v.AsOctetNoAddRef();
				return big_int_type(sprig::krkr::tjs::octet_data(v_octet), sprig::krkr::tjs::octet_length(v_octet), s);
			}
		default:
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("符号付変換はオクテット列のみ対応します"),
				sprig::krkr::bad_argument
				);
			return big_int_type::zero();
		}
	}
	KTL_INLINE NativeBigInt::big_int_type NativeBigInt::convertValuePtr(tTJSVariant const* v) {
		return convertValue(*v);
	}
	KTL_INLINE sprig::krkr::tjs::intptr_type NativeBigInt::getInstance(iTJSDispatch2* obj) {
		return sprig::krkr::tjs::GetPropValue<sprig::krkr::tjs::intptr_type>(
			sprig::krkr::tjs::GetMemberNoAddRef(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("BigInt")),
				SPRIG_KRKR_TJS_W("instance")
				),
			obj
			);
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeBigInt::createNew(tjs_int numparams, tTJSVariant** param) {
		iTJSDispatch2* result_obj = 0;
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("BigInt")),
			&result_obj,
			numparams,
			param,
			0
			);
		return sprig::krkr::tjs::object_type(result_obj, false);
	}
	NativeBigInt::NativeBigInt() {}
	NativeBigInt::NativeBigInt(big_int_type const& value)
		: value_(value)
	{}
	NativeBigInt::NativeBigInt(tTJSVariant const& v)
		: value_(convertValue(v))
	{}
	NativeBigInt::NativeBigInt(tTJSVariant const& v, int sign)
		: value_(convertValue(v, sign))
	{}
	KTL_INLINE void NativeBigInt::reset(big_int_type const& value) {
		value_ = value;
	}
	KTL_INLINE NativeBigInt::big_int_type const& NativeBigInt::get() const {
		return value_;
	}
	KTL_INLINE NativeBigInt::big_int_type& NativeBigInt::ref() {
		return value_;
	}
	//
	// KTL_BIG_INT_INTERIOR_OPERATION_DECL
	//
#define KTL_BIG_INT_INTERIOR_OPERATION_DECL(FUNC_NAME, OP) \
	KTL_INLINE sprig::krkr::tjs::object_type NativeBigInt::FUNC_NAME(tjs_int numparams, tTJSVariant** param) { \
		if (numparams < 2) { \
			KTL_ERROR( \
				KTL_ERROR_SECTION, \
				SPRIG_KRKR_TJS_W("引数が不足しています"), \
				sprig::krkr::bad_argument \
				); \
			return 0; \
		} \
		sprig::krkr::tjs::object_type result = createNew(1, param); \
		BigInt* ptr = reinterpret_cast<BigInt*>(getInstance(sprig::get_pointer(result))); \
		for (int i = 1; i != numparams; ++i) { \
			ptr->ref() OP convertValue(*param[i]); \
		} \
		return result; \
	}
	//
	//	SUMMARY: 演算系メソッド
	//
	KTL_BIG_INT_INTERIOR_OPERATION_DECL(opAdd, +=);
	KTL_BIG_INT_INTERIOR_OPERATION_DECL(opSub, -=);
	KTL_BIG_INT_INTERIOR_OPERATION_DECL(opMul, *=);
	KTL_BIG_INT_INTERIOR_OPERATION_DECL(opDiv, /=);
	KTL_BIG_INT_INTERIOR_OPERATION_DECL(opMod, %=);
	KTL_BIG_INT_INTERIOR_OPERATION_DECL(opOr, |=);
	KTL_BIG_INT_INTERIOR_OPERATION_DECL(opAnd, &=);
	KTL_BIG_INT_INTERIOR_OPERATION_DECL(opXor, ^=);
	//
	//	SUMMARY: 同定系メソッド
	//
	KTL_INLINE bool NativeBigInt::opEq(tjs_int numparams, tTJSVariant** param) {
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
	KTL_INLINE bool NativeBigInt::opNEq(tjs_int numparams, tTJSVariant** param) {
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
	// KTL_BIG_INT_INTERIOR_COMPARISON_DECL
	//
#define KTL_BIG_INT_INTERIOR_COMPARISON_DECL(FUNC_NAME, OP) \
	KTL_INLINE bool NativeBigInt::FUNC_NAME(tjs_int numparams, tTJSVariant** param) { \
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
	KTL_BIG_INT_INTERIOR_COMPARISON_DECL(opLs, <);
	KTL_BIG_INT_INTERIOR_COMPARISON_DECL(opLsEq, <=);
	KTL_BIG_INT_INTERIOR_COMPARISON_DECL(opGr, >);
	KTL_BIG_INT_INTERIOR_COMPARISON_DECL(opGrEq, >=);
	//
	// KTL_BIG_INT_INTERIOR_UNARY_OPERATION_DECL
	//
#define KTL_BIG_INT_INTERIOR_UNARY_OPERATION_DECL(FUNC_NAME, OP) \
	KTL_INLINE sprig::krkr::tjs::object_type NativeBigInt::FUNC_NAME(tTJSVariant const& v) { \
		sprig::krkr::tjs::object_type result = createNew(0, 0); \
		BigInt* ptr = reinterpret_cast<BigInt*>(getInstance(sprig::get_pointer(result))); \
		ptr->ref().swap(OP convertValue(v)); \
		return result; \
	}
	//
	//	SUMMARY: 単項演算系メソッド
	//
	KTL_BIG_INT_INTERIOR_UNARY_OPERATION_DECL(opPlus, +);
	KTL_BIG_INT_INTERIOR_UNARY_OPERATION_DECL(opMinus, -);
	//
	//	SUMMARY: 算術系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type NativeBigInt::abs(tTJSVariant const& v) {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		BigInt* ptr = reinterpret_cast<BigInt*>(getInstance(sprig::get_pointer(result)));
		sprig::abs(ptr->ref(), convertValue(v));
		return result;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeBigInt::pow(tTJSVariant const& v, int e) {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		BigInt* ptr = reinterpret_cast<BigInt*>(getInstance(sprig::get_pointer(result)));
		sprig::pow(ptr->ref(), convertValue(v), e);
		return result;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeBigInt::gcd(tTJSVariant const& v1, tTJSVariant const& v2) {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		BigInt* ptr = reinterpret_cast<BigInt*>(getInstance(sprig::get_pointer(result)));
		sprig::gcd(ptr->ref(), convertValue(v1), convertValue(v2));
		return result;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeBigInt::max(tTJSVariant const& v1, tTJSVariant const& v2) {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		BigInt* ptr = reinterpret_cast<BigInt*>(getInstance(sprig::get_pointer(result)));
		sprig::max(ptr->ref(), convertValue(v1), convertValue(v2));
		return result;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeBigInt::min(tTJSVariant const& v1, tTJSVariant const& v2) {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		BigInt* ptr = reinterpret_cast<BigInt*>(getInstance(sprig::get_pointer(result)));
		sprig::min(ptr->ref(), convertValue(v1), convertValue(v2));
		return result;
	}
	//
	//	SUMMARY: 代入系メソッド
	//
	KTL_INLINE NativeBigInt& NativeBigInt::assign(tTJSVariant const& v) {
		value_.swap(convertValue(v));
		return *this;
	}
	//
	// KTL_BIG_INT_INTERIOR_ASSIGN_DECL
	//
#define KTL_BIG_INT_INTERIOR_ASSIGN_DECL(FUNC_NAME, OP) \
	KTL_INLINE NativeBigInt& NativeBigInt::FUNC_NAME(tjs_int numparams, tTJSVariant** param) { \
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
	KTL_BIG_INT_INTERIOR_ASSIGN_DECL(assignAdd, +=);
	KTL_BIG_INT_INTERIOR_ASSIGN_DECL(assignSub, -=);
	KTL_BIG_INT_INTERIOR_ASSIGN_DECL(assignMul, *=);
	KTL_BIG_INT_INTERIOR_ASSIGN_DECL(assignDiv, /=);
	KTL_BIG_INT_INTERIOR_ASSIGN_DECL(assignMod, %=);
	KTL_BIG_INT_INTERIOR_ASSIGN_DECL(assignOr, |=);
	KTL_BIG_INT_INTERIOR_ASSIGN_DECL(assignAnd, &=);
	KTL_BIG_INT_INTERIOR_ASSIGN_DECL(assignXor, ^=);
	//
	//	SUMMARY: 変換系メソッド
	//
	KTL_INLINE tTVInteger NativeBigInt::asInt() const {
		return sprig::big_int_cast<tTVInteger>(value_);
	}
	KTL_INLINE tTVReal NativeBigInt::asReal() const {
		return sprig::big_int_cast<tTVReal>(value_);
	}
	KTL_INLINE tTJSString NativeBigInt::asString() const {
		return sprig::str_cast<tTJSString>(value_);
	}
	KTL_INLINE tTJSVariant NativeBigInt::asOctet() const {
		std::vector<tjs_uint8> bytes;
		sprig::get_bytes(value_, std::back_inserter(bytes));
		return tTJSVariant(bytes.empty() ? 0 : &bytes[0], bytes.size());
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE int NativeBigInt::sign() const {
		return sprig::sign(value_);
	}

	//
	// BigInt
	//
	BigInt::BigInt() {}
	tjs_error TJS_INTF_METHOD BigInt::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("BigInt::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		if (numparams >= 2) {
			instance_ = boost::make_shared<NativeBigInt>(*param[0], *param[1]);
		} else if (numparams >= 1) {
			instance_ = boost::make_shared<NativeBigInt>(*param[0]);
		} else {
			instance_ = boost::make_shared<NativeBigInt>();
		}
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD BigInt::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("BigInt::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	KTL_INLINE void BigInt::reset(big_int_type const& value) {
		instance_->reset(value);
	}
	KTL_INLINE BigInt::big_int_type const& BigInt::get() const {
		return instance_->get();
	}
	KTL_INLINE BigInt::big_int_type& BigInt::ref() {
		return instance_->ref();
	}
	//
	//	SUMMARY: 内部系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type BigInt::getInstance() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(this);
	}
	//
	//	SUMMARY: 演算系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::opAdd(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opAdd(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::opSub(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opSub(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::opMul(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opMul(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::opDiv(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opDiv(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::opMod(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opMod(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::opOr(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opOr(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::opAnd(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opAnd(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::opXor(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opXor(numparams, param);
	}
	//
	//	SUMMARY: 同定系メソッド
	//
	KTL_INLINE bool BigInt::opEq(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opEq(numparams, param);
	}
	KTL_INLINE bool BigInt::opNEq(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opNEq(numparams, param);
	}
	//
	//	SUMMARY: 比較系メソッド
	//
	KTL_INLINE bool BigInt::opLs(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opLs(numparams, param);
	}
	KTL_INLINE bool BigInt::opLsEq(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opLsEq(numparams, param);
	}
	KTL_INLINE bool BigInt::opGr(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opGr(numparams, param);
	}
	KTL_INLINE bool BigInt::opGrEq(tjs_int numparams, tTJSVariant** param) {
		return NativeBigInt::opGrEq(numparams, param);
	}
	//
	//	SUMMARY: 単項演算系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::opPlus(tTJSVariant const& v) {
		return NativeBigInt::opPlus(v);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::opMinus(tTJSVariant const& v) {
		return NativeBigInt::opMinus(v);
	}
	//
	//	SUMMARY: 算術系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::abs(tTJSVariant const& v) {
		return NativeBigInt::abs(v);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::pow(tTJSVariant const& v, tTVInteger e) {
		return NativeBigInt::pow(v, boost::numeric_cast<int>(e));
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::gcd(tTJSVariant const& v1, tTJSVariant const& v2) {
		return NativeBigInt::gcd(v1, v2);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::max(tTJSVariant const& v1, tTJSVariant const& v2) {
		return NativeBigInt::max(v1, v2);
	}
	KTL_INLINE sprig::krkr::tjs::object_type BigInt::min(tTJSVariant const& v1, tTJSVariant const& v2) {
		return NativeBigInt::min(v1, v2);
	}
	//
	//	SUMMARY: 代入系メソッド
	//
	KTL_INLINE BigInt& BigInt::assign(tTJSVariant const& v) {
		instance_->assign(v);
		return *this;
	}
	//
	//	SUMMARY: 演算代入系メソッド
	//
	KTL_INLINE BigInt& BigInt::assignAdd(tjs_int numparams, tTJSVariant** param) {
		instance_->assignAdd(numparams, param);
		return *this;
	}
	KTL_INLINE BigInt& BigInt::assignSub(tjs_int numparams, tTJSVariant** param) {
		instance_->assignSub(numparams, param);
		return *this;
	}
	KTL_INLINE BigInt& BigInt::assignMul(tjs_int numparams, tTJSVariant** param) {
		instance_->assignMul(numparams, param);
		return *this;
	}
	KTL_INLINE BigInt& BigInt::assignDiv(tjs_int numparams, tTJSVariant** param) {
		instance_->assignDiv(numparams, param);
		return *this;
	}
	KTL_INLINE BigInt& BigInt::assignMod(tjs_int numparams, tTJSVariant** param) {
		instance_->assignMod(numparams, param);
		return *this;
	}
	KTL_INLINE BigInt& BigInt::assignOr(tjs_int numparams, tTJSVariant** param) {
		instance_->assignOr(numparams, param);
		return *this;
	}
	KTL_INLINE BigInt& BigInt::assignAnd(tjs_int numparams, tTJSVariant** param) {
		instance_->assignAnd(numparams, param);
		return *this;
	}
	KTL_INLINE BigInt& BigInt::assignXor(tjs_int numparams, tTJSVariant** param) {
		instance_->assignXor(numparams, param);
		return *this;
	}
	//
	//	SUMMARY: 変換系メソッド
	//
	KTL_INLINE tTVInteger BigInt::asInt() const {
		return instance_->asInt();
	}
	KTL_INLINE tTVReal BigInt::asReal() const {
		return instance_->asReal();
	}
	KTL_INLINE tTJSString BigInt::asString() const {
		return instance_->asString();
	}
	KTL_INLINE tTJSVariant BigInt::asOctet() const {
		return instance_->asOctet();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tTVInteger BigInt::sign() const {
		return instance_->sign();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_BIG_INT_BIG_INT_HPP
