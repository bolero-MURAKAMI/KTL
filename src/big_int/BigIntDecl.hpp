/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_BIG_INT_BIG_INT_DECL_HPP
#define SRC_BIG_INT_BIG_INT_DECL_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/ntl/big_int.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeBigInt
	//
	class NativeBigInt {
	public:
		typedef sprig::big_int big_int_type;
	private:
		big_int_type value_;
	public:
		static big_int_type convertValue(tTJSVariant const& v);
		static big_int_type convertValue(tTJSVariant const& v, int sign);
		static big_int_type convertValuePtr(tTJSVariant const* v);
		static sprig::krkr::tjs::intptr_type getInstance(iTJSDispatch2* obj);
		static sprig::krkr::tjs::object_type createNew(tjs_int numparams, tTJSVariant** param);
	public:
		NativeBigInt();
		explicit NativeBigInt(big_int_type const& value);
		explicit NativeBigInt(tTJSVariant const& v);
		NativeBigInt(tTJSVariant const& v, int sign);
		void reset(big_int_type const& value);
		big_int_type const& get() const;
		big_int_type& ref();
	public:
		//
		//	SUMMARY: 演算系メソッド
		//
		static sprig::krkr::tjs::object_type opAdd(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opSub(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opMul(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opDiv(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opMod(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opOr(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opAnd(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opXor(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: 同定系メソッド
		//
		static bool opEq(tjs_int numparams, tTJSVariant** param);
		static bool opNEq(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: 比較系メソッド
		//
		static bool opLs(tjs_int numparams, tTJSVariant** param);
		static bool opLsEq(tjs_int numparams, tTJSVariant** param);
		static bool opGr(tjs_int numparams, tTJSVariant** param);
		static bool opGrEq(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: 単項演算系メソッド
		//
		static sprig::krkr::tjs::object_type opPlus(tTJSVariant const& v);
		static sprig::krkr::tjs::object_type opMinus(tTJSVariant const& v);
		//
		//	SUMMARY: 算術系メソッド
		//
		static sprig::krkr::tjs::object_type abs(tTJSVariant const& v);
		static sprig::krkr::tjs::object_type pow(tTJSVariant const& v, int e);
		static sprig::krkr::tjs::object_type gcd(tTJSVariant const& v1, tTJSVariant const& v2);
		static sprig::krkr::tjs::object_type max(tTJSVariant const& v1, tTJSVariant const& v2);
		static sprig::krkr::tjs::object_type min(tTJSVariant const& v1, tTJSVariant const& v2);
	public:
		//
		//	SUMMARY: 代入系メソッド
		//
		NativeBigInt& assign(tTJSVariant const& v);
		//
		//	SUMMARY: 演算代入系メソッド
		//
		NativeBigInt& assignAdd(tjs_int numparams, tTJSVariant** param);
		NativeBigInt& assignSub(tjs_int numparams, tTJSVariant** param);
		NativeBigInt& assignMul(tjs_int numparams, tTJSVariant** param);
		NativeBigInt& assignDiv(tjs_int numparams, tTJSVariant** param);
		NativeBigInt& assignMod(tjs_int numparams, tTJSVariant** param);
		NativeBigInt& assignOr(tjs_int numparams, tTJSVariant** param);
		NativeBigInt& assignAnd(tjs_int numparams, tTJSVariant** param);
		NativeBigInt& assignXor(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: 変換系メソッド
		//
		tTVInteger asInt() const;
		tTVReal asReal() const;
		tTJSString asString() const;
		tTJSVariant asOctet() const;
		//
		//	SUMMARY: プロパティ系メソッド
		//
		int sign() const;
	};

	//
	// BigInt
	//
	class BigInt
		: public tTJSNativeInstance
	{
	public:
		typedef NativeBigInt::big_int_type big_int_type;
	private:
		boost::shared_ptr<NativeBigInt> instance_;
	public:
		BigInt();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
		void reset(big_int_type const& value);
		big_int_type const& get() const;
		big_int_type& ref();
	public:
		//
		//	SUMMARY: 演算系メソッド
		//
		static sprig::krkr::tjs::object_type opAdd(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opSub(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opMul(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opDiv(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opMod(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opOr(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opAnd(tjs_int numparams, tTJSVariant** param);
		static sprig::krkr::tjs::object_type opXor(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: 同定系メソッド
		//
		static bool opEq(tjs_int numparams, tTJSVariant** param);
		static bool opNEq(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: 比較系メソッド
		//
		static bool opLs(tjs_int numparams, tTJSVariant** param);
		static bool opLsEq(tjs_int numparams, tTJSVariant** param);
		static bool opGr(tjs_int numparams, tTJSVariant** param);
		static bool opGrEq(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: 単項演算系メソッド
		//
		static sprig::krkr::tjs::object_type opPlus(tTJSVariant const& v);
		static sprig::krkr::tjs::object_type opMinus(tTJSVariant const& v);
		//
		//	SUMMARY: 算術系メソッド
		//
		static sprig::krkr::tjs::object_type abs(tTJSVariant const& v);
		static sprig::krkr::tjs::object_type pow(tTJSVariant const& v, tTVInteger e);
		static sprig::krkr::tjs::object_type gcd(tTJSVariant const& v1, tTJSVariant const& v2);
		static sprig::krkr::tjs::object_type max(tTJSVariant const& v1, tTJSVariant const& v2);
		static sprig::krkr::tjs::object_type min(tTJSVariant const& v1, tTJSVariant const& v2);
	public:
		//
		//	SUMMARY: 内部系メソッド
		//
		sprig::krkr::tjs::intptr_type getInstance();
		//
		//	SUMMARY: 代入系メソッド
		//
		BigInt& assign(tTJSVariant const& v);
		//
		//	SUMMARY: 演算代入系メソッド
		//
		BigInt& assignAdd(tjs_int numparams, tTJSVariant** param);
		BigInt& assignSub(tjs_int numparams, tTJSVariant** param);
		BigInt& assignMul(tjs_int numparams, tTJSVariant** param);
		BigInt& assignDiv(tjs_int numparams, tTJSVariant** param);
		BigInt& assignMod(tjs_int numparams, tTJSVariant** param);
		BigInt& assignOr(tjs_int numparams, tTJSVariant** param);
		BigInt& assignAnd(tjs_int numparams, tTJSVariant** param);
		BigInt& assignXor(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: 変換系メソッド
		//
		tTVInteger asInt() const;
		tTVReal asReal() const;
		tTJSString asString() const;
		tTJSVariant asOctet() const;
		//
		//	SUMMARY: プロパティ系メソッド
		//
		tTVInteger sign() const;
	};
}	// namespace ktl

#endif	// #ifndef SRC_BIG_INT_BIG_INT_DECL_HPP
