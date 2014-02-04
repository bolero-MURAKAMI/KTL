/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_UTILS_PERFORMANCE_TIMER_HPP
#define SRC_UTILS_PERFORMANCE_TIMER_HPP

#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "PerformanceTimerDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:PerformanceTimer:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:PerformanceTimer:error")

namespace ktl {
	//
	// NativePerformanceTimer
	//
	NativePerformanceTimer::NativePerformanceTimer() {}
	//
	//	SUMMARY: 操作系メソッド
	//
	KTL_INLINE void NativePerformanceTimer::restart() {
		performance_timer_.restart();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE NativePerformanceTimer::value_type NativePerformanceTimer::elapsed() {
		return performance_timer_.elapsed();
	}

	//
	// PerformanceTimer
	//
	PerformanceTimer::PerformanceTimer() {}
	tjs_error TJS_INTF_METHOD PerformanceTimer::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("PerformanceTimer::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_= boost::make_shared<NativePerformanceTimer>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD PerformanceTimer::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("PerformanceTimer::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 操作系メソッド
	//
	KTL_INLINE void PerformanceTimer::restart() {
		instance_->restart();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tjs_real PerformanceTimer::elapsed() {
		return instance_->elapsed();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_UTILS_PERFORMANCE_TIMER_HPP
