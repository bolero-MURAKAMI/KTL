/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_FUNCTOR_FUNCTOR_HPP
#define SRC_FUNCTOR_FUNCTOR_HPP

#include <boost/utility/swap.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/str_compare.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "FunctorDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Functor:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Functor:error")

namespace ktl {
	//
	// FunctorStruct
	//
	FunctorStruct::FunctorStruct() {}
	FunctorStruct::FunctorStruct(tTJSString const& source)
		: source_(source.AsVariantStringNoAddRef())
	{
		tTJSVariant func_v;
		::TVPExecuteExpression(source, &func_v);
		if (func_v.Type() != tvtObject) {
			::TVPThrowExceptionMessage(SPRIG_KRKR_TJS_W("評価結果がオブジェクトではありません"));
		}
		tTJSVariantClosure const& func_c = func_v.AsObjectClosureNoAddRef();
		if (!func_c.Object) {
			::TVPThrowExceptionMessage(SPRIG_KRKR_TJS_W("評価結果がnullです"));
		}
		func_.reset(func_c.Object);
		context_.reset(func_c.ObjThis);
	}
	tTJSString const& FunctorStruct::source() const {
		return source_;
	}
	tTJSString& FunctorStruct::source() {
		return source_;
	}
	sprig::krkr::tjs::object_type const& FunctorStruct::func() const {
		return func_;
	}
	sprig::krkr::tjs::object_type& FunctorStruct::func() {
		return func_;
	}
	sprig::krkr::tjs::object_type const& FunctorStruct::context() const {
		return context_;
	}
	sprig::krkr::tjs::object_type& FunctorStruct::context() {
		return context_;
	}
	void FunctorStruct::swap(FunctorStruct& other) {
		boost::swap(source_, other.source_);
		boost::swap(func_, other.func_);
		boost::swap(context_, other.context_);
	}
	void swap(FunctorStruct& lhs, FunctorStruct& rhs) {
		lhs.swap(rhs);
	}

	//
	// FunctorClass
	//
	FunctorClass::FunctorClass()
		: compile_(new compile(), false)
	{}
	tjs_error TJS_INTF_METHOD FunctorClass::FuncCall(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis
		)
	{
		if (flag) {
			return TJS_E_NOTIMPL;
		}
		if (!membername) {
			return TJS_E_NOTIMPL;
		}
		if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("compile")) == 0) {
			return compile_->FuncCall(
				flag,
				0,
				hint,
				result,
				numparams,
				param,
				objthis
				);
		} else {
			return TJS_E_MEMBERNOTFOUND;
		}
		return TJS_S_OK;
	}
	tjs_error TJS_INTF_METHOD FunctorClass::PropGet(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		iTJSDispatch2* objthis
		)
	{
		if (flag) {
			return TJS_E_NOTIMPL;
		}
		if (!membername) {
			return TJS_E_NOTIMPL;
		}
		if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("compile")) == 0) {
			if (result) {
				*result = compile_.get();
			}
		} else {
			return TJS_E_MEMBERNOTFOUND;
		}
		return TJS_S_OK;
	}
	tjs_error TJS_INTF_METHOD FunctorClass::PropSet(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant const* param,
		iTJSDispatch2* objthis
		)
	{
		if (flag) {
			return TJS_E_NOTIMPL;
		}
		if (!membername) {
			return TJS_E_NOTIMPL;
		}
		if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("compile")) == 0) {
			compile_.reset(param->AsObjectNoAddRef());
		} else {
			return TJS_E_MEMBERNOTFOUND;
		}
		return TJS_S_OK;
	}
	tjs_error TJS_INTF_METHOD FunctorClass::IsInstanceOf(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tjs_char const* classname,
		iTJSDispatch2* objthis
		)
	{
		if (flag) {
			return TJS_E_NOTIMPL;
		}
		if (membername) {
			if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("compile")) == 0) {
				return compile_->IsInstanceOf(
					flag,
					membername,
					hint,
					classname,
					objthis
					);
			} else {
				return TJS_E_MEMBERNOTFOUND;
			}
		}
		if (!classname) {
			return TJS_E_INVALIDPARAM;
		}
		return sprig::str_compare(classname, SPRIG_KRKR_TJS_W("FunctorClass")) == 0
			|| sprig::str_compare(classname, SPRIG_KRKR_TJS_W("Object")) == 0
			? TJS_S_TRUE
			: TJS_S_FALSE
			;
	}
	tjs_error TJS_INTF_METHOD FunctorClass::ClassInstanceInfo(
		tjs_uint32 flag,
		tjs_uint num,
		tTJSVariant* value
		)
	{
		if (flag != TJS_CII_GET) {
			return TJS_E_NOTIMPL;
		}
		if (num != 0) {
			return TJS_E_NOTIMPL;
		}
		if (!value) {
			return TJS_E_INVALIDPARAM;
		}
		*value = SPRIG_KRKR_TJS_W("FunctorClass");
		return TJS_S_OK;
	}

	//
	// FunctorClass::compile
	//
	tjs_error TJS_INTF_METHOD FunctorClass::compile::FuncCall(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis
		)
	{
		if (flag) {
			return TJS_E_NOTIMPL;
		}
		if (numparams < 1) {
			return TJS_E_BADPARAMCOUNT;
		}
		if (param[0]->Type() != tvtString) {
			return TJS_E_INVALIDPARAM;
		}
		FunctorStruct s(*param[0]);
		sprig::krkr::tjs::object_type functor(
			new Functor(s),
			false
			);
		if (result) {
			*result = functor.get();
		}
		return TJS_S_OK;
	}

	//
	// Functor
	//
	Functor::Functor(FunctorStruct const& s)
		: s_(s)
		, compile_(new FunctorClass::compile(), false)
	{}
	tjs_error TJS_INTF_METHOD Functor::FuncCall(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis
		)
	{
		if (flag) {
			return TJS_E_NOTIMPL;
		}
		if (membername) {
			if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("compile")) == 0) {
				tTJSVariant functor;
				tjs_error error = compile_->FuncCall(
					flag,
					0,
					hint,
					&functor,
					numparams,
					param,
					objthis
					);
				if (TJS_FAILED(error)) {
					return error;
				}
				boost::swap(
					s_,
					dynamic_cast<Functor*>(functor.AsObjectNoAddRef())->s_
					);
				if (result) {
					*result = this;
				}
				return error;
			} else if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("func")) == 0) {
				return s_.func()->FuncCall(
					flag,
					0,
					hint,
					result,
					numparams,
					param,
					s_.context().get()
					);
			} else if (
				sprig::str_compare(membername, SPRIG_KRKR_TJS_W("source")) == 0
				|| sprig::str_compare(membername, SPRIG_KRKR_TJS_W("context")) == 0
				)
			{
				return TJS_E_INVALIDTYPE;
			} else {
				return TJS_E_MEMBERNOTFOUND;
			}
		}
		return s_.func()->FuncCall(
			flag,
			membername,
			hint,
			result,
			numparams,
			param,
			s_.context().get()
			);
	}
	tjs_error TJS_INTF_METHOD Functor::PropGet(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		iTJSDispatch2* objthis
		)
	{
		if (flag) {
			return TJS_E_NOTIMPL;
		}
		if (!membername) {
			return TJS_E_NOTIMPL;
		}
		if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("source")) == 0) {
			if (result) {
				*result = s_.source();
			}
		} else if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("func")) == 0) {
			if (result) {
				*result = s_.func().get();
			}
		} else if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("context")) == 0) {
			if (result) {
				*result = s_.context().get();
			}
		} else if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("compile")) == 0) {
			if (result) {
				*result = compile_.get();
			}
		} else {
			return TJS_E_MEMBERNOTFOUND;
		}
		return TJS_S_OK;
	}
	tjs_error TJS_INTF_METHOD Functor::PropSet(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant const* param,
		iTJSDispatch2* objthis
		)
	{
		if (flag) {
			return TJS_E_NOTIMPL;
		}
		if (!membername) {
			return TJS_E_NOTIMPL;
		}
		if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("compile")) == 0) {
			compile_.reset(param->AsObjectNoAddRef());
		} else if (
			sprig::str_compare(membername, SPRIG_KRKR_TJS_W("source")) == 0
			|| sprig::str_compare(membername, SPRIG_KRKR_TJS_W("func")) == 0
			|| sprig::str_compare(membername, SPRIG_KRKR_TJS_W("context")) == 0
			)
		{
			return TJS_E_ACCESSDENYED;
		} else {
			return TJS_E_MEMBERNOTFOUND;
		}
		return TJS_S_OK;
	}
	tjs_error TJS_INTF_METHOD Functor::IsInstanceOf(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tjs_char const* classname,
		iTJSDispatch2* objthis
		)
	{
		if (flag) {
			return TJS_E_NOTIMPL;
		}
		if (membername) {
			if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("compile")) == 0) {
				return compile_->IsInstanceOf(
					flag,
					0,
					hint,
					classname,
					objthis
					);
			} else if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("func")) == 0) {
				return s_.func()->IsInstanceOf(
					flag,
					0,
					hint,
					classname,
					objthis
					);
			} else if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("context")) == 0) {
				return s_.context()->IsInstanceOf(
					flag,
					0,
					hint,
					classname,
					objthis
					);
			} else if (
				sprig::str_compare(membername, SPRIG_KRKR_TJS_W("source")) == 0
				)
			{
				return sprig::str_compare(classname, SPRIG_KRKR_TJS_W("String")) == 0
					|| sprig::str_compare(classname, SPRIG_KRKR_TJS_W("Object")) == 0
					;
			} else {
				return TJS_E_MEMBERNOTFOUND;
			}
		}
		if (!classname) {
			return TJS_E_INVALIDPARAM;
		}
		return sprig::str_compare(classname, SPRIG_KRKR_TJS_W("Functor")) == 0
			|| sprig::str_compare(classname, SPRIG_KRKR_TJS_W("Function")) == 0
			|| sprig::str_compare(classname, SPRIG_KRKR_TJS_W("Object")) == 0
			? TJS_S_TRUE
			: TJS_S_FALSE
			;
	}
	tjs_error TJS_INTF_METHOD Functor::ClassInstanceInfo(
		tjs_uint32 flag,
		tjs_uint num,
		tTJSVariant* value
		)
	{
		if (flag != TJS_CII_GET) {
			return TJS_E_NOTIMPL;
		}
		if (num != 0) {
			return TJS_E_NOTIMPL;
		}
		if (!value) {
			return TJS_E_INVALIDPARAM;
		}
		*value = SPRIG_KRKR_TJS_W("Functor");
		return TJS_S_OK;
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_FUNCTOR_FUNCTOR_HPP
