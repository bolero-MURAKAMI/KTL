/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_FUNCTOR_FUNCTOR_DECL_HPP
#define SRC_FUNCTOR_FUNCTOR_DECL_HPP

#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// FunctorStruct
	//
	class FunctorStruct {
	private:
		tTJSString source_;
		sprig::krkr::tjs::object_type func_;
		sprig::krkr::tjs::object_type context_;
	public:
		FunctorStruct();
		explicit FunctorStruct(tTJSString const& source);
		tTJSString const& source() const;
		tTJSString& source();
		sprig::krkr::tjs::object_type const& func() const;
		sprig::krkr::tjs::object_type& func();
		sprig::krkr::tjs::object_type const& context() const;
		sprig::krkr::tjs::object_type& context();
		void swap(FunctorStruct& other);
	};
	void swap(FunctorStruct& lhs, FunctorStruct& rhs);

	//
	// FunctorClass
	//
	class FunctorClass
		: public tTJSDispatch
	{
	public:
		class compile
			: public tTJSDispatch
		{
		public:
			tjs_error TJS_INTF_METHOD FuncCall(
				tjs_uint32 flag,
				tjs_char const* membername,
				tjs_uint32* hint,
				tTJSVariant* result,
				tjs_int numparams,
				tTJSVariant** param,
				iTJSDispatch2* objthis
				);
		};
	private:
		sprig::krkr::tjs::object_type compile_;
	public:
		FunctorClass();
		tjs_error TJS_INTF_METHOD FuncCall(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD PropGet(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant* result,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD PropSet(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant const* param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD IsInstanceOf(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tjs_char const* classname,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD ClassInstanceInfo(
			tjs_uint32 flag,
			tjs_uint num,
			tTJSVariant* value
			);
	};

	//
	// Functor
	//
	class Functor
		: public tTJSDispatch
	{
	private:
		FunctorStruct s_;
		sprig::krkr::tjs::object_type compile_;
	public:
		explicit Functor(FunctorStruct const& s);
		tjs_error TJS_INTF_METHOD FuncCall(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD PropGet(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant* result,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD PropSet(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant const* param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD IsInstanceOf(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tjs_char const* classname,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD ClassInstanceInfo(
			tjs_uint32 flag,
			tjs_uint num,
			tTJSVariant* value
			);
	};
}	// namespace ktl

#endif	// #ifndef SRC_FUNCTOR_FUNCTOR_DECL_HPP
