#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeCppInt
	//
	class NativeCppInt {
	public:
		typedef boost::multiprecision::cpp_int big_int_type;
	private:
		big_int_type value_;
	public:
		static big_int_type convertValue(tTJSVariant const& v);
		static big_int_type convertValue(tTJSVariant const& v, int sign);
		static big_int_type convertValuePtr(tTJSVariant const* v);
		static sprig::krkr::tjs::intptr_type getInstance(iTJSDispatch2* obj);
		static sprig::krkr::tjs::object_type createNew(tjs_int numparams, tTJSVariant** param);
	public:
		NativeCppInt();
		explicit NativeCppInt(big_int_type const& value);
		explicit NativeCppInt(tTJSVariant const& v);
		NativeCppInt(tTJSVariant const& v, int sign);
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
		NativeCppInt& assign(tTJSVariant const& v);
		//
		//	SUMMARY: 演算代入系メソッド
		//
		NativeCppInt& assignAdd(tjs_int numparams, tTJSVariant** param);
		NativeCppInt& assignSub(tjs_int numparams, tTJSVariant** param);
		NativeCppInt& assignMul(tjs_int numparams, tTJSVariant** param);
		NativeCppInt& assignDiv(tjs_int numparams, tTJSVariant** param);
		NativeCppInt& assignMod(tjs_int numparams, tTJSVariant** param);
		NativeCppInt& assignOr(tjs_int numparams, tTJSVariant** param);
		NativeCppInt& assignAnd(tjs_int numparams, tTJSVariant** param);
		NativeCppInt& assignXor(tjs_int numparams, tTJSVariant** param);
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
	// CppInt
	//
	class CppInt
		: public tTJSNativeInstance
	{
	public:
		typedef NativeCppInt::big_int_type big_int_type;
	private:
		boost::shared_ptr<NativeCppInt> instance_;
	public:
		CppInt();
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
		CppInt& assign(tTJSVariant const& v);
		//
		//	SUMMARY: 演算代入系メソッド
		//
		CppInt& assignAdd(tjs_int numparams, tTJSVariant** param);
		CppInt& assignSub(tjs_int numparams, tTJSVariant** param);
		CppInt& assignMul(tjs_int numparams, tTJSVariant** param);
		CppInt& assignDiv(tjs_int numparams, tTJSVariant** param);
		CppInt& assignMod(tjs_int numparams, tTJSVariant** param);
		CppInt& assignOr(tjs_int numparams, tTJSVariant** param);
		CppInt& assignAnd(tjs_int numparams, tTJSVariant** param);
		CppInt& assignXor(tjs_int numparams, tTJSVariant** param);
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
