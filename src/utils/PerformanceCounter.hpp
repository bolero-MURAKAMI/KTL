/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_UTILS_PERFORMANCE_COUNTER_HPP
#define SRC_UTILS_PERFORMANCE_COUNTER_HPP

#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "PerformanceCounterDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:PerformanceCounter:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:PerformanceCounter:error")

namespace ktl {
	//
	// NativePerformanceCounter
	//
	NativePerformanceCounter::NativePerformanceCounter()
		: enable_(false)
	{}
	//
	//	SUMMARY: 出力系メソッド
	//
	KTL_INLINE void NativePerformanceCounter::begin() {
		if (!enable_) {
			return;
		}
		performance_timer_.restart();
	}
	KTL_INLINE void NativePerformanceCounter::end() {
		if (!enable_) {
			return;
		}
		performance_times_.push_back(performance_timer_.elapsed());
		if (unit_performance_timer_.elapsed() > 1.0) {
			unit_performance_timer_.restart();
			sprig::performance_timer::value_type average = 0;
			BOOST_FOREACH(sprig::performance_timer::value_type const& e, performance_times_) {
				average += e;
			}
			average /= performance_times_.size();
			performance_times_.clear();
			SPRIG_KRKR_OUTPUT_VALUE(
				SPRIG_KRKR_TJS_W("KTL:PerformanceCounter:performance"),
				boost::basic_format<tjs_char>(SPRIG_KRKR_TJS_W("%f[ms]")) % (average * 1000),
				SPRIG_KRKR_LOG_LEVEL_IMPORTANT
				);
		}
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool NativePerformanceCounter::getEnable() const {
		return enable_;
	}
	KTL_INLINE void NativePerformanceCounter::setEnable(bool v) {
		enable_ = v;
	}

	//
	// PerformanceCounter
	//
	PerformanceCounter::PerformanceCounter() {}
	tjs_error TJS_INTF_METHOD PerformanceCounter::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("PerformanceCounter::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativePerformanceCounter>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD PerformanceCounter::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("PerformanceCounter::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 出力系メソッド
	//
	KTL_INLINE void PerformanceCounter::begin() {
		instance_->begin();
	}
	KTL_INLINE void PerformanceCounter::end() {
		instance_->end();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool PerformanceCounter::getEnable() const {
		return instance_->getEnable();
	}
	KTL_INLINE void PerformanceCounter::setEnable(bool v) {
		instance_->setEnable(v);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_UTILS_PERFORMANCE_COUNTER_HPP
