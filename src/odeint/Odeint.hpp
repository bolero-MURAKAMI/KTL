/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_ODEINT_ODEINT_HPP
#define SRC_ODEINT_ODEINT_HPP

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/numeric/odeint/integrate/integrate.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/get_pointer.hpp>
#include <sprig/str_compare.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "OdeintDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Odeint:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Odeint:error")

namespace ktl {
	tjs_error TJS_INTF_METHOD NativeOdeint::Vector::PropGet(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		iTJSDispatch2* objthis
		)
	{
		if (!membername) {
			return TJS_E_NOTIMPL;
		}
		if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("asArray")) == 0) {
			if (result) {
				sprig::krkr::tjs::object_type res(
					sprig::krkr::tjs::CreateNewObject(
						sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
						0, 0, 0
						),
					false
					);
				{
					tjs_int num = 0;
					for (state_type::const_iterator it = s_->begin(), last = s_->end(); it != last; ++it) {
						sprig::krkr::tjs::AddMember(sprig::get_pointer(res), num, tTJSVariant(*it));
						++num;
					}
				}
				*result = tTJSVariant(res.get(), res.get());
			}
		} else if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("count")) == 0) {
			if (result) {
				*result = static_cast<tjs_int>(s_->size());
			}
		} else {
			return TJS_E_MEMBERNOTFOUND;
		}
		return TJS_S_OK;
	}
	tjs_error TJS_INTF_METHOD NativeOdeint::Vector::PropGetByNum(
		tjs_uint32 flag,
		tjs_int num,
		tTJSVariant* result,
		iTJSDispatch2* objthis
		)
	{
		if (num < 0 || num >= static_cast<tjs_int>(s_->size())) {
			return TJS_E_INVALIDPARAM;
		}
		if (!result) {
			return TJS_S_OK;
		}
		*result = (*s_)[num];
		return TJS_S_OK;
	}
	tjs_error TJS_INTF_METHOD NativeOdeint::Vector::PropSetByNum(
		tjs_uint32 flag,
		tjs_int num,
		tTJSVariant const* param,
		iTJSDispatch2* objthis
		)
	{
		if (num < 0 || num >= static_cast<tjs_int>(s_->size())) {
			return TJS_E_INVALIDPARAM;
		}
		(*s_)[num] = *param;
		return TJS_S_OK;
	}
	tjs_error TJS_INTF_METHOD NativeOdeint::ConstVector::PropGet(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		iTJSDispatch2* objthis
		)
	{
		if (!membername) {
			return TJS_E_NOTIMPL;
		}
		if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("asArray")) == 0) {
			if (result) {
				sprig::krkr::tjs::object_type res(
					sprig::krkr::tjs::CreateNewObject(
						sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
						0, 0, 0
						),
					false
					);
				{
					tjs_int num = 0;
					for (state_type::const_iterator it = s_->begin(), last = s_->end(); it != last; ++it) {
						sprig::krkr::tjs::AddMember(sprig::get_pointer(res), num, tTJSVariant(*it));
						++num;
					}
				}
				*result = tTJSVariant(res.get(), res.get());
			}
		} else if (sprig::str_compare(membername, SPRIG_KRKR_TJS_W("count")) == 0) {
			if (result) {
				*result = static_cast<tjs_int>(s_->size());
			}
		} else {
			return TJS_E_MEMBERNOTFOUND;
		}
		return TJS_S_OK;
	}
	tjs_error TJS_INTF_METHOD NativeOdeint::ConstVector::PropGetByNum(
		tjs_uint32 flag,
		tjs_int num,
		tTJSVariant* result,
		iTJSDispatch2* objthis
		)
	{
		if (num < 0 || num >= static_cast<tjs_int>(s_->size())) {
			return TJS_E_INVALIDPARAM;
		}
		if (!result) {
			return TJS_S_OK;
		}
		*result = (*s_)[num];
		return TJS_S_OK;
	}
	KTL_INLINE void NativeOdeint::System::operator()(state_type const& x, state_type& dxdt, time_type t) const {
		sprig::krkr::tjs::object_type x_object(new ConstVector(&x), false);
		sprig::krkr::tjs::object_type dxdt_object(new Vector(&dxdt), false);
		tTJSVariant result;
		tTJSVariant param[] = {
			x_object.get(), dxdt_object.get(), t
		};
		tTJSVariant* p_param[] = {
			param + 0, param + 1, param + 2
		};
		sprig::krkr::tjs::FuncObjectCall(
			closure_.Object,
			&result,
			3,
			p_param,
			closure_.ObjThis
			);
	}
	KTL_INLINE void NativeOdeint::Observer::operator()(state_type const& state, time_type t) const {
		sprig::krkr::tjs::object_type state_object(new ConstVector(&state), false);
		tTJSVariant result;
		tTJSVariant param[] = {
			state_object.get(), t
		};
		tTJSVariant* p_param[] = {
			param + 0, param + 1
		};
		sprig::krkr::tjs::FuncObjectCall(
			closure_.Object,
			&result,
			2,
			p_param,
			closure_.ObjThis
			);
	}
	//
	// NativeOdeint
	//
	NativeOdeint::NativeOdeint() {}
	//
	//	SUMMARY: 積分系メソッド
	//
	KTL_INLINE std::size_t NativeOdeint::integrate(
		tTJSVariantClosure const& system, tTJSVariantClosure const& start_state, time_type start_time, time_type end_time, time_type dt
		)
	{
		System sys(system);
		state_type state;
		tjs_int const count = sprig::krkr::tjs::GetPropValue<tjs_int>(start_state, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			state.push_back(
				sprig::krkr::tjs::GetPropValue<value_type>(start_state, i)
				);
		}
		std::size_t ret = boost::numeric::odeint::integrate(sys, state, start_time, end_time, dt);
		for (tjs_int i = 0; i != count; ++i) {
			sprig::krkr::tjs::AddMember(start_state.Object, i, tTJSVariant(state[i]));
		}
		return ret;
	}
	KTL_INLINE std::size_t NativeOdeint::integrate(
		tTJSVariantClosure const& system, tTJSVariantClosure const& start_state, time_type start_time, time_type end_time, time_type dt,
		tTJSVariantClosure const& observer
		)
	{
		System sys(system);
		Observer obs(observer);
		state_type state;
		tjs_int const count = sprig::krkr::tjs::GetPropValue<tjs_int>(start_state, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			state.push_back(
				sprig::krkr::tjs::GetPropValue<value_type>(start_state, i)
				);
		}
		std::size_t ret = boost::numeric::odeint::integrate(sys, state, start_time, end_time, dt, obs);
		for (tjs_int i = 0; i != count; ++i) {
			sprig::krkr::tjs::AddMember(start_state.Object, i, tTJSVariant(state[i]));
		}
		return ret;
	}

	//
	// Odeint
	//
	Odeint::Odeint() {}
	tjs_error TJS_INTF_METHOD Odeint::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Odeint::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeOdeint>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Odeint::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Odeint::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 積分系メソッド
	//
	KTL_INLINE tTVInteger Odeint::integrate(
		tTJSVariantClosure const& system, tTJSVariantClosure const& start_state, tTVReal start_time, tTVReal end_time, tTVReal dt
		)
	{
		return NativeOdeint::integrate(system, start_state, start_time, end_time, dt);
	}
	KTL_INLINE tTVInteger Odeint::integrate(
		tTJSVariantClosure const& system, tTJSVariantClosure const& start_state, tTVReal start_time, tTVReal end_time, tTVReal dt,
		tTJSVariantClosure const& observer
		)
	{
		return NativeOdeint::integrate(system, start_state, start_time, end_time, dt, observer);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_ODEINT_ODEINT_HPP
