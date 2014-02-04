/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_UTILS_PERFORMANCE_TIMER_DECL_HPP
#define SRC_UTILS_PERFORMANCE_TIMER_DECL_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/performance_timer.hpp>
#include <sprig/timer.hpp>

namespace ktl {
	//
	// NativePerformanceTimer
	//
	class NativePerformanceTimer {
	public:
		typedef sprig::performance_timer::value_type value_type;
	private:
		sprig::performance_timer performance_timer_;
	public:
		NativePerformanceTimer();
	public:
		//
		//	SUMMARY: 操作系メソッド
		//
		void restart();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		value_type elapsed();
	};

	//
	// PerformanceTimer
	//
	class PerformanceTimer
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativePerformanceTimer> instance_;
	public:
		PerformanceTimer();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
		//
		//	SUMMARY: 操作系メソッド
		//
		void restart();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		tjs_real elapsed();
	};
}	// namespace ktl

#endif	// #ifndef SRC_UTILS_PERFORMANCE_TIMER_DECL_HPP
