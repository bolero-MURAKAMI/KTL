/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_ODEINT_ODEINT_DECL_HPP
#define SRC_ODEINT_ODEINT_DECL_HPP

#include <cstddef>
#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeOdeint
	//
	class NativeOdeint {
	public:
		typedef tTVReal value_type;
		typedef tTVReal time_type;
		typedef std::vector<value_type> state_type;
	public:
		class Vector
			: public tTJSDispatch
		{
		private:
			state_type* s_;
		public:
			explicit Vector(state_type* s)
				: s_(s)
			{}
		public:
			tjs_error TJS_INTF_METHOD iTJSDispatch2::PropGet(
				tjs_uint32 flag,
				tjs_char const* membername,
				tjs_uint32* hint,
				tTJSVariant* result,
				iTJSDispatch2* objthis
				);
			tjs_error TJS_INTF_METHOD PropGetByNum(
				tjs_uint32 flag,
				tjs_int num,
				tTJSVariant* result,
				iTJSDispatch2* objthis
				);
			tjs_error TJS_INTF_METHOD PropSetByNum(
				tjs_uint32 flag,
				tjs_int num,
				tTJSVariant const* param,
				iTJSDispatch2* objthis
				);
		};
		class ConstVector
			: public tTJSDispatch
		{
		private:
			state_type const* s_;
		public:
			explicit ConstVector(state_type const* s)
				: s_(s)
			{}
		public:
			tjs_error TJS_INTF_METHOD iTJSDispatch2::PropGet(
				tjs_uint32 flag,
				tjs_char const* membername,
				tjs_uint32* hint,
				tTJSVariant* result,
				iTJSDispatch2* objthis
				);
			tjs_error TJS_INTF_METHOD PropGetByNum(
				tjs_uint32 flag,
				tjs_int num,
				tTJSVariant* result,
				iTJSDispatch2* objthis
				);
		};
		class System {
		private:
			tTJSVariantClosure closure_;
		public:
			explicit System(tTJSVariantClosure const& closure)
				: closure_(closure)
			{}
			void operator()(state_type const& x, state_type& dxdt, time_type t) const;
		};
		class Observer {
		private:
			tTJSVariantClosure closure_;
		public:
			explicit Observer(tTJSVariantClosure const& closure)
				: closure_(closure)
			{}
			void operator()(state_type const& state, time_type t) const;
		};
	public:
		NativeOdeint();
	public:
		//
		//	SUMMARY: 積分系メソッド
		//
		static std::size_t integrate(
			tTJSVariantClosure const& system, tTJSVariantClosure const& start_state, time_type start_time, time_type end_time, time_type dt
			);
		static std::size_t integrate(
			tTJSVariantClosure const& system, tTJSVariantClosure const& start_state, time_type start_time, time_type end_time, time_type dt,
			tTJSVariantClosure const& observer
			);
	};

	//
	// Odeint
	//
	class Odeint
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeOdeint> instance_;
	public:
		Odeint();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: 積分系メソッド
		//
		static tTVInteger integrate(
			tTJSVariantClosure const& system, tTJSVariantClosure const& start_state, tTVReal start_time, tTVReal end_time, tTVReal dt
			);
		static tTVInteger integrate(
			tTJSVariantClosure const& system, tTJSVariantClosure const& start_state, tTVReal start_time, tTVReal end_time, tTVReal dt,
			tTJSVariantClosure const& observer
			);
	};
}	// namespace ktl

#endif	// #ifndef SRC_ODEINT_ODEINT_DECL_HPP
