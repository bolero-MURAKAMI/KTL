#pragma once

#include <boost/lexical_cast.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/str_compare.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "PTIndexDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:PTIndex:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:PTIndex:error")

namespace ktl {
	//
	// PTIndex
	//
	KTL_INLINE void PTIndex::reserve(size_type size) {
		if (size < 0) {
			return;
		}
		index_table_.reserve(size);
	}
	KTL_INLINE void PTIndex::resize(size_type size) {
		if (size < 0) {
			return;
		}
		index_table_.resize(size);
	}
	tjs_error TJS_INTF_METHOD PTIndex::FuncCall(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis
		)
	{
		if (!membername) {
			return TJS_E_NOTIMPL;
		}
		if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("reserve")) == 0) {
			if (numparams < 1) {
				return TJS_E_BADPARAMCOUNT;
			}
			size_type size = *param[0];
			if (size < 0) {
				return TJS_E_INVALIDPARAM;
			}
			reserve(size);
			if (result) {
				*result = size;
			}
		} else if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("resize")) == 0) {
			if (numparams < 1) {
				return TJS_E_BADPARAMCOUNT;
			}
			size_type size = *param[0];
			if (size < 0) {
				return TJS_E_INVALIDPARAM;
			}
			resize(size);
			if (result) {
				*result = size;
			}
		} else {
			return TJS_E_MEMBERNOTFOUND;
		}
		return TJS_S_OK;
	}
	tjs_error TJS_INTF_METHOD PTIndex::PropGetByNum(
		tjs_uint32 flag,
		tjs_int num,
		tTJSVariant* result,
		iTJSDispatch2* objthis
		)
	{
		if (num < 0) {
			return TJS_E_INVALIDPARAM;
		}
		reserve(num + 1);
		if (!result) {
			return TJS_S_OK;
		}
		*result = index_table_[num];
		return TJS_S_OK;
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
