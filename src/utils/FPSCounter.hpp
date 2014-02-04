/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_UTILS_FPS_COUNTER_HPP
#define SRC_UTILS_FPS_COUNTER_HPP

#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "FPSCounterDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:FPSCounter:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:FPSCounter:error")

namespace ktl {
	//
	// NativeFPSCounter
	//
	NativeFPSCounter::NativeFPSCounter()
		: before_time_(0.0)
		, enable_(false)
		, run_(false)
		, unit_time_(1.0)
	{}
	NativeFPSCounter::~NativeFPSCounter() throw() {
		if (run_) {
			::TVPRemoveContinuousEventHook(this);
		}
	}
	void TJS_INTF_METHOD NativeFPSCounter::OnContinuousCallback(tjs_uint64 tick) {
		try {
			if (enable_) {
				process();
			}
		} catch (sprig::exception const& e) {
			sprig::krkr::tjs::rethrow_exception_to_tjs_exception(e);
		} catch (boost::exception const& e) {
			sprig::krkr::tjs::rethrow_exception_to_tjs_exception(e);
		} catch (std::exception const& e) {
			sprig::krkr::tjs::rethrow_exception_to_tjs_exception(e);
		}
	}
	//
	//	SUMMARY: 出力系メソッド
	//
	KTL_INLINE NativeFPSCounter::value_type NativeFPSCounter::getTime() const {
		return time_counter_.get();
	}
	KTL_INLINE std::size_t NativeFPSCounter::getFPS() const {
		return container_.size();
	}
	KTL_INLINE void NativeFPSCounter::push(value_type const time) {
		container_.push_back(time);
	}
	KTL_INLINE void NativeFPSCounter::truncate(value_type const time) {
		value_type const border_time = time - unit_time_;
		container_type::size_type border_count = 0;
		for (container_type::size_type i =0, last = container_.size(); i != last; ++i) {
			if (container_.at(i) < border_time) {
				++border_count;
			} else {
				break;
			}
		}
		for (container_type::size_type i = 0, last = border_count; i != last; ++i) {
			container_.pop_front();
		}
	}
	KTL_INLINE void NativeFPSCounter::clear() {
		container_.clear();
	}
	KTL_INLINE void NativeFPSCounter::resetBeforeTime(value_type const time) {
		before_time_ = time;
	}
	KTL_INLINE void NativeFPSCounter::process() {
		value_type const time = getTime();
		push(time);
		truncate(time);
		if (time - unit_time_ > before_time_) {
			SPRIG_KRKR_OUTPUT_VALUE(
				SPRIG_KRKR_TJS_W("KTL:FPSCounter:FPS"),
				getFPS(),
				SPRIG_KRKR_LOG_LEVEL_IMPORTANT
				);
			resetBeforeTime(time);
		}
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool NativeFPSCounter::getEnable() const {
		return enable_;
	}
	KTL_INLINE void NativeFPSCounter::setEnable(bool v) {
		enable_ = v;
	}
	KTL_INLINE bool NativeFPSCounter::getRun() const {
		return run_;
	}
	KTL_INLINE void NativeFPSCounter::setRun(bool v) {
		if (!run_ && v) {
			::TVPAddContinuousEventHook(this);
			clear();
			resetBeforeTime(getTime());
		} else if (run_ && !v) {
			::TVPRemoveContinuousEventHook(this);
		}
		run_ = v;
	}

	//
	// FPSCounter
	//
	FPSCounter::FPSCounter() {}
	tjs_error TJS_INTF_METHOD FPSCounter::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("FPSCounter::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeFPSCounter>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD FPSCounter::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("FPSCounter::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool FPSCounter::getEnable() const {
		return instance_->getEnable();
	}
	KTL_INLINE void FPSCounter::setEnable(bool v) {
		instance_->setEnable(v);
	}
	KTL_INLINE bool FPSCounter::getRun() const {
		return instance_->getRun();
	}
	KTL_INLINE void FPSCounter::setRun(bool v) {
		instance_->setRun(v);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_UTILS_FPS_COUNTER_HPP
