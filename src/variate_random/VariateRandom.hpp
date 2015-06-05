/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_VARIATE_RANDOM_VARIATE_RANDOM_HPP
#define SRC_VARIATE_RANDOM_VARIATE_RANDOM_HPP

#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/exception.hpp>
#include <sprig/section.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "VariateRandomCommon.hpp"
#include "VariateRandomDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:VariateRandom:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:VariateRandom:error")

namespace ktl {
	//
	// NativeVariateRandom
	//
	NativeVariateRandom::NativeVariateRandom()
		: generator_(sprig::random::null_engine<>(), sprig::random::null_distribution<RandomResult>())
	{}
	NativeVariateRandom::NativeVariateRandom(iTJSDispatch2* eng_obj)
		: engine_(eng_obj)
		, generator_(
			boost::apply_visitor(
				sprig::random::generate_random_visitor<variate_generator_type>(),
				RandomEngineBase::getBase(eng_obj)->variant(),
				distribution_variant_type()
				)
			)
	{}
	NativeVariateRandom::NativeVariateRandom(iTJSDispatch2* eng_obj, iTJSDispatch2* dis_obj)
		: engine_(eng_obj)
		, distribution_(dis_obj)
		, generator_(
			boost::apply_visitor(
				sprig::random::generate_random_visitor<variate_generator_type>(),
				RandomEngineBase::getBase(eng_obj)->variant(),
				RandomDistributionBase::getBase(dis_obj)->variant()
				)
			)
	{}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE RandomResult NativeVariateRandom::random() {
		return generator_();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type NativeVariateRandom::engine() const {
		return engine_;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeVariateRandom::distribution() const {
		return distribution_;
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	KTL_INLINE RandomResult NativeVariateRandom::min() const {
		return generator_.min();
	}
	KTL_INLINE RandomResult NativeVariateRandom::max() const {
		return generator_.max();
	}

	//
	// VariateRandom
	//
	VariateRandom::VariateRandom() {}
	tjs_error TJS_INTF_METHOD VariateRandom::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("VariateRandom::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
			SPRIG_KRKR_TYPE_CHECK(1, tvtObject);
			instance_ = boost::make_shared<NativeVariateRandom>(
				param[0]->AsObjectNoAddRef(),
				param[1]->AsObjectNoAddRef()
				);
		} else if (numparams >= 1) {
			SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
			instance_ = boost::make_shared<NativeVariateRandom>(
				param[0]->AsObjectNoAddRef()
				);
		} else {
			instance_ = boost::make_shared<NativeVariateRandom>();
		}
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD VariateRandom::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("VariateRandom::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE tTJSVariant VariateRandom::random() {
		return instance_->random();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tTJSVariant VariateRandom::engine() const {
		sprig::krkr::tjs::object_type result = instance_->engine();
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant VariateRandom::distribution() const {
		sprig::krkr::tjs::object_type result = instance_->distribution();
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	KTL_INLINE tTJSVariant VariateRandom::min() const {
		return instance_->min();
	}
	KTL_INLINE tTJSVariant VariateRandom::max() const {
		return instance_->max();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_VARIATE_RANDOM_VARIATE_RANDOM_HPP
