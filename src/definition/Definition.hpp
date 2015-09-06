/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_DEFINITION_DEFINITION_HPP
#define SRC_DEFINITION_DEFINITION_HPP

#include <boost/lexical_cast.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/str_compare.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "DefinitionDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Definition:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Definition:error")
#if 0
#define KTL_DEBUG_LOG_LEVEL SPRIG_KRKR_LOG_LEVEL_NORMAL
#else
#define KTL_DEBUG_LOG_LEVEL SPRIG_KRKR_LOG_LEVEL_IGNORE
#endif

namespace ktl {
	//
	// DefinitionClass
	//
	KTL_INLINE tjs_error DefinitionClass::callPropGetByNum(
		tjs_uint32 flag,
		tjs_int num,
		tTJSVariant* result,
		iTJSDispatch2* objthis
		)
	{
		tjs_error error = TJS_S_OK;
		tTJSVariant par[] = {
			num
		};
		tTJSVariant* p_par[] = {
			par + 0
		};
		return PropGetByNum(flag, num, result, objthis);
	}
	KTL_INLINE void DefinitionClass::name(std::basic_string<tjs_char> const& s) {
		name_ = s;
	}
	KTL_INLINE std::basic_string<tjs_char> const& DefinitionClass::name() const {
		return name_;
	}
	KTL_INLINE void DefinitionClass::base(sprig::krkr::tjs::object_type const& obj) {
		base_ = obj;
	}
	KTL_INLINE sprig::krkr::tjs::object_type DefinitionClass::base() const {
		return base_;
	}
	KTL_INLINE void DefinitionClass::func(sprig::krkr::tjs::object_type const& obj) {
		func_ = obj;
	}
	KTL_INLINE sprig::krkr::tjs::object_type DefinitionClass::func() const {
		return func_;
	}
	KTL_INLINE void DefinitionClass::propGetByNum(sprig::krkr::tjs::object_type const& obj) {
		propGetByNum_ = obj;
	}
	KTL_INLINE sprig::krkr::tjs::object_type DefinitionClass::propGetByNum() const {
		return propGetByNum_;
	}
	KTL_INLINE void DefinitionClass::propSetByNum(sprig::krkr::tjs::object_type const& obj) {
		propSetByNum_ = obj;
	}
	KTL_INLINE sprig::krkr::tjs::object_type DefinitionClass::propSetByNum() const {
		return propSetByNum_;
	}
	KTL_INLINE DefinitionClass::DefinitionClass() {}
	KTL_INLINE DefinitionClass::DefinitionClass(DefinitionClass const& dc, iTJSDispatch2* base_obj)
		: name_(dc.name_)
		, base_(base_obj, false)
		, func_(dc.func_)
		, propGetByNum_(dc.propGetByNum_)
		, propSetByNum_(dc.propSetByNum_)
	{}
	tjs_error TJS_INTF_METHOD DefinitionClass::FuncCall(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::FuncCall"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		if (membername) {
			if (propGetByNum_) {
				tjs_int num = -1;
				if (boost::conversion::try_lexical_convert(membername, num)) {
					return FuncCallByNum(flag, num, result, numparams, param, objthis);
				}
			}
		} else {
			if (func_) {
				return func_->FuncCall(flag, membername, hint, result, numparams, param, objthis);
			}
		}
		return base_ ? base_->FuncCall(flag, membername, hint, result, numparams, param, objthis)
			: tTJSDispatch::FuncCall(flag, membername, hint, result, numparams, param, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::FuncCallByNum(
		tjs_uint32 flag,
		tjs_int num,
		tTJSVariant* result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::FuncCallByNum"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("num"), num, KTL_DEBUG_LOG_LEVEL);
		if (propGetByNum_) {
			tjs_error error = TJS_S_OK;
			tTJSVariant val;
			if (TJS_FAILED(error = callPropGetByNum(flag, num, &val, objthis))) {
				return error;
			}
			return val.AsObjectClosureNoAddRef().FuncCall(0, 0, 0, result, numparams, param, 0);
		}
		return base_ ? base_->FuncCallByNum(flag, num, result, numparams, param, objthis)
			: tTJSDispatch::FuncCallByNum(flag, num, result, numparams, param, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::PropGet(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::PropGet"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		if (membername && propGetByNum_) {
			tjs_int num = -1;
			if (boost::conversion::try_lexical_convert(membername, num)) {
				return PropGetByNum(flag, num, result, objthis);
			}
		}
		return base_ ? base_->PropGet(flag, membername, hint, result, objthis)
			: tTJSDispatch::PropGet(flag, membername, hint, result, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::PropGetByNum(
		tjs_uint32 flag,
		tjs_int num,
		tTJSVariant* result,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::PropGetByNum"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("num"), num, KTL_DEBUG_LOG_LEVEL);
		if (propGetByNum_) {
			tTJSVariant par[] = {
				num
			};
			tTJSVariant* p_par[] = {
				par + 0
			};
			return propGetByNum_->FuncCall(0, 0, 0, result, 1, p_par, objthis);
		}
		return base_ ? base_->PropGetByNum(flag, num, result, objthis)
			: tTJSDispatch::PropGetByNum(flag, num, result, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::PropSet(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant const* param,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::PropSet"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		if (membername && propSetByNum_) {
			tjs_int num = -1;
			if (boost::conversion::try_lexical_convert(membername, num)) {
				return PropSetByNum(flag, num, param, objthis);
			}
		}
		return base_ ? base_->PropSet(flag, membername, hint, param, objthis)
			: tTJSDispatch::PropSet(flag, membername, hint, param, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::PropSetByNum(
		tjs_uint32 flag,
		tjs_int num,
		tTJSVariant const* param,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::PropSetByNum"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("num"), num, KTL_DEBUG_LOG_LEVEL);
		if (propSetByNum_) {
			tTJSVariant par[] = {
				num, *param
			};
			tTJSVariant* p_par[] = {
				par + 0, par + 1
			};
			return propSetByNum_->FuncCall(0, 0, 0, 0, 2, p_par, objthis);
		}
		return base_ ? base_->PropSetByNum(flag, num, param, objthis)
			: tTJSDispatch::PropSetByNum(flag, num, param, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::GetCount(
		tjs_int* result,
		tjs_char const* membername,
		tjs_uint32* hint,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::GetCount"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		if (membername && propGetByNum_) {
			tjs_int num = -1;
			if (boost::conversion::try_lexical_convert(membername, num)) {
				return GetCountByNum(result, num, objthis);
			}
		}
		return base_ ? base_->GetCount(result, membername, hint, objthis)
			: tTJSDispatch::GetCount(result, membername, hint, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::GetCountByNum(
		tjs_int* result,
		tjs_int num,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::GetCountByNum"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("num"), num, KTL_DEBUG_LOG_LEVEL);
		if (propGetByNum_) {
			tjs_error error = TJS_S_OK;
			tTJSVariant val;
			if (TJS_FAILED(error = callPropGetByNum(0, num, &val, objthis))) {
				return error;
			}
			return val.AsObjectClosureNoAddRef().GetCount(result, 0, 0, 0);
		}
		return base_ ? base_->GetCountByNum(result, num, objthis)
			: tTJSDispatch::GetCountByNum(result, num, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::PropSetByVS(
		tjs_uint32 flag,
		tTJSVariantString* membername,
		tTJSVariant const* param,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::PropSetByVS"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		return base_ ? base_->PropSetByVS(flag, membername, param, objthis)
			: tTJSDispatch::PropSetByVS(flag, membername, param, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::EnumMembers(
		tjs_uint32 flag,
		tTJSVariantClosure* callback,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::EnumMembers"), KTL_DEBUG_LOG_LEVEL);
		return base_ ? base_->EnumMembers(flag, callback, objthis)
			: tTJSDispatch::EnumMembers(flag, callback, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::DeleteMember(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::DeleteMember"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		if (membername && propGetByNum_) {
			tjs_int num = -1;
			if (boost::conversion::try_lexical_convert(membername, num)) {
				return DeleteMemberByNum(flag, num, objthis);
			}
		}
		return base_ ? base_->DeleteMember(flag, membername, hint, objthis)
			: tTJSDispatch::DeleteMember(flag, membername, hint, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::DeleteMemberByNum(
		tjs_uint32 flag,
		tjs_int num,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::DeleteMemberByNum"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("num"), num, KTL_DEBUG_LOG_LEVEL);
		if (propGetByNum_) {
			tjs_error error = TJS_S_OK;
			tTJSVariant val;
			if (TJS_FAILED(error = callPropGetByNum(flag, num, &val, objthis))) {
				return error;
			}
			return val.AsObjectClosureNoAddRef().DeleteMember(0, 0, 0, 0);
		}
		return base_ ? base_->DeleteMemberByNum(flag, num, objthis)
			: tTJSDispatch::DeleteMemberByNum(flag, num, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::Invalidate(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::Invalidate"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		if (membername && propGetByNum_) {
			tjs_int num = -1;
			if (boost::conversion::try_lexical_convert(membername, num)) {
				return InvalidateByNum(flag, num, objthis);
			}
		}
		return base_ ? base_->Invalidate(flag, membername, hint, objthis)
			: tTJSDispatch::Invalidate(flag, membername, hint, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::InvalidateByNum(
		tjs_uint32 flag,
		tjs_int num,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::InvalidateByNum"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("num"), num, KTL_DEBUG_LOG_LEVEL);
		if (propGetByNum_) {
			tjs_error error = TJS_S_OK;
			tTJSVariant val;
			if (TJS_FAILED(error = callPropGetByNum(flag, num, &val, objthis))) {
				return error;
			}
			return val.AsObjectClosureNoAddRef().Invalidate(0, 0, 0, 0);
		}
		return base_ ? base_->InvalidateByNum(flag, num, objthis)
			: tTJSDispatch::InvalidateByNum(flag, num, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::IsValid(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::IsValid"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		if (membername && propGetByNum_) {
			tjs_int num = -1;
			if (boost::conversion::try_lexical_convert(membername, num)) {
				return IsValidByNum(flag, num, objthis);
			}
		}
		return base_ ? base_->IsValid(flag, membername, hint, objthis)
			: tTJSDispatch::IsValid(flag, membername, hint, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::IsValidByNum(
		tjs_uint32 flag,
		tjs_int num,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::IsValidByNum"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("num"), num, KTL_DEBUG_LOG_LEVEL);
		if (propGetByNum_) {
			tjs_error error = TJS_S_OK;
			tTJSVariant val;
			if (TJS_FAILED(error = callPropGetByNum(flag, num, &val, objthis))) {
				return error;
			}
			return val.AsObjectClosureNoAddRef().IsValid(0, 0, 0, 0);
		}
		return base_ ? base_->IsValidByNum(flag, num, objthis)
			: tTJSDispatch::IsValidByNum(flag, num, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::CreateNew(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		iTJSDispatch2** result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::CreateNew"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		if (membername) {
			if (propGetByNum_) {
				tjs_int num = -1;
				if (boost::conversion::try_lexical_convert(membername, num)) {
					return CreateNewByNum(flag, num, result, numparams, param, objthis);
				}
			}
		} else {
			tjs_error error = TJS_S_OK;
			if (base_) {
				iTJSDispatch2* base_obj;
				error = base_->CreateNew(flag, membername, hint, &base_obj, numparams, param, objthis);
				if (TJS_FAILED(error)) {
					return error;
				}
				if (!name_.empty()) {
					tTJSVariant cii = name_.c_str();
					error = base_obj->ClassInstanceInfo(TJS_CII_ADD, 0, &cii);
					if (TJS_FAILED(error)) {
						return error;
					}
				}
				*result = new DefinitionClass(*this, base_obj);
				return error;
			}
			*result = new DefinitionClass(*this);
			return error;
		}
		return base_ ? base_->CreateNew(flag, membername, hint, result, numparams, param, objthis)
			: tTJSDispatch::CreateNew(flag, membername, hint, result, numparams, param, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::CreateNewByNum(
		tjs_uint32 flag,
		tjs_int num,
		iTJSDispatch2** result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::CreateNewByNum"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("num"), num, KTL_DEBUG_LOG_LEVEL);
		if (propGetByNum_) {
			tjs_error error = TJS_S_OK;
			tTJSVariant val;
			if (TJS_FAILED(error = callPropGetByNum(flag, num, &val, objthis))) {
				return error;
			}
			return val.AsObjectClosureNoAddRef().CreateNew(0, 0, 0, result, numparams, param, 0);
		}
		return base_ ? base_->CreateNewByNum(flag, num, result, numparams, param, objthis)
			: tTJSDispatch::CreateNewByNum(flag, num, result, numparams, param, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::IsInstanceOf(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tjs_char const* classname,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::IsInstanceOf"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		if (membername && propGetByNum_) {
			tjs_int num = -1;
			if (boost::conversion::try_lexical_convert(membername, num)) {
				return IsInstanceOfByNum(flag, num, classname, objthis);
			}
		}
		return base_ ? base_->IsInstanceOf(flag, membername, hint, classname, objthis)
			: tTJSDispatch::IsInstanceOf(flag, membername, hint, classname, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::IsInstanceOfByNum(
		tjs_uint32 flag,
		tjs_int num,
		tjs_char const* classname,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::IsInstanceOfByNum"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("num"), num, KTL_DEBUG_LOG_LEVEL);
		if (propGetByNum_) {
			tjs_error error = TJS_S_OK;
			tTJSVariant val;
			if (TJS_FAILED(error = callPropGetByNum(flag, num, &val, objthis))) {
				return error;
			}
			return val.AsObjectClosureNoAddRef().IsInstanceOf(0, 0, 0, classname, 0);
		}
		return base_ ? base_->IsInstanceOfByNum(flag, num, classname, objthis)
			: tTJSDispatch::IsInstanceOfByNum(flag, num, classname, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::Operation(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		tTJSVariant const* param,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::Operation"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("membername"), sprig::krkr::tjs::as_c_str(membername), KTL_DEBUG_LOG_LEVEL);
		if (membername && propGetByNum_) {
			tjs_int num = -1;
			if (boost::conversion::try_lexical_convert(membername, num)) {
				return OperationByNum(flag, num, result, param, objthis);
			}
		}
		return base_ ? base_->Operation(flag, membername, hint, result, param, objthis)
			: tTJSDispatch::Operation(flag, membername, hint, result, param, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::OperationByNum(
		tjs_uint32 flag,
		tjs_int num,
		tTJSVariant* result,
		tTJSVariant const* param,
		iTJSDispatch2* objthis
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::OperationByNum"), KTL_DEBUG_LOG_LEVEL);
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("num"), num, KTL_DEBUG_LOG_LEVEL);
		if (propGetByNum_) {
			tjs_error error = TJS_S_OK;
			tTJSVariant val;
			if (TJS_FAILED(error = callPropGetByNum(flag, num, &val, objthis))) {
				return error;
			}
			return val.AsObjectClosureNoAddRef().Operation(0, 0, 0, result, param, 0);
		}
		return base_ ? base_->OperationByNum(flag, num, result, param, objthis)
			: tTJSDispatch::OperationByNum(flag, num, result, param, objthis)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::NativeInstanceSupport(
		tjs_uint32 flag,
		tjs_int32 classid,
		iTJSNativeInstance** pointer
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::NativeInstanceSupport"), KTL_DEBUG_LOG_LEVEL);
		return base_ ? base_->NativeInstanceSupport(flag, classid, pointer)
			: tTJSDispatch::NativeInstanceSupport(flag, classid, pointer)
			;
	}
	tjs_error TJS_INTF_METHOD DefinitionClass::ClassInstanceInfo(
		tjs_uint32 flag,
		tjs_uint num,
		tTJSVariant* value
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("DefinitionClass::ClassInstanceInfo"), KTL_DEBUG_LOG_LEVEL);
		return base_ ? base_->ClassInstanceInfo(flag, num, value)
			: tTJSDispatch::ClassInstanceInfo(flag, num, value)
			;
	}

	//
	// NativeDefinition
	//
	NativeDefinition::NativeDefinition() {}
	//
	//	SUMMARY: 定義系メソッド
	//
	KTL_INLINE tTJSVariant NativeDefinition::class_() {
		return sprig::krkr::tjs::as_object_closure_variant(
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("ClassDefinition")),
				0, 0, 0
				)
			);
	}

	//
	// NativeClassDefinition
	//
	NativeClassDefinition::NativeClassDefinition()
		: def_(new DefinitionClass(), false)
	{}
	//
	//	SUMMARY: 定義系メソッド
	//
	KTL_INLINE tTJSVariant NativeClassDefinition::endDefinition() {
		return sprig::krkr::tjs::as_object_closure_variant(def_);
	}
	//
	//	SUMMARY: クラス定義系メソッド
	//
	KTL_INLINE void NativeClassDefinition::name(tjs_char const* s) {
		def_->name(s);
	}
	KTL_INLINE void NativeClassDefinition::extends_(iTJSDispatch2* obj) {
		def_->base(obj);
	}
	KTL_INLINE void NativeClassDefinition::funcCall(iTJSDispatch2* obj) {
		def_->func(obj);
	}
	KTL_INLINE void NativeClassDefinition::propGetByNum(iTJSDispatch2* obj) {
		def_->propGetByNum(obj);
	}
	KTL_INLINE void NativeClassDefinition::propSetByNum(iTJSDispatch2* obj) {
		def_->propSetByNum(obj);
	}

	//
	// Definition
	//
	Definition::Definition() {}
	tjs_error TJS_INTF_METHOD Definition::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Definition::Construct"), KTL_DEBUG_LOG_LEVEL);
		instance_ = boost::make_shared<NativeDefinition>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Definition::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Definition::Invalidate"), KTL_DEBUG_LOG_LEVEL);
		instance_.reset();
	}
	//
	//	SUMMARY: 定義系メソッド
	//
	KTL_INLINE tTJSVariant Definition::class_() {
		return NativeDefinition::class_();
	}

	//
	// ClassDefinition
	//
	ClassDefinition::ClassDefinition() {}
	tjs_error TJS_INTF_METHOD ClassDefinition::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("ClassDefinition::Construct"), KTL_DEBUG_LOG_LEVEL);
		instance_ = boost::make_shared<NativeClassDefinition>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD ClassDefinition::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("ClassDefinition::Invalidate"), KTL_DEBUG_LOG_LEVEL);
		instance_.reset();
	}
	//
	//	SUMMARY: 定義系メソッド
	//
	KTL_INLINE tTJSVariant ClassDefinition::endDefinition() {
		return instance_->endDefinition();
	}
	//
	//	SUMMARY: クラス定義系メソッド
	//
	KTL_INLINE void ClassDefinition::name(tTJSVariantString const* s) {
		instance_->name(sprig::krkr::tjs::as_c_str(s));
	}
	KTL_INLINE void ClassDefinition::extends_(iTJSDispatch2* obj) {
		instance_->extends_(obj);
	}
	KTL_INLINE void ClassDefinition::funcCall(iTJSDispatch2* obj) {
		instance_->funcCall(obj);
	}
	KTL_INLINE void ClassDefinition::propGetByNum(iTJSDispatch2* obj) {
		instance_->propGetByNum(obj);
	}
	KTL_INLINE void ClassDefinition::propSetByNum(iTJSDispatch2* obj) {
		instance_->propSetByNum(obj);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
#undef KTL_DEBUG_LOG_LEVEL

#endif	// #ifndef SRC_DEFINITION_DEFINITION_HPP
