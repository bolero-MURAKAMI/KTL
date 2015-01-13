/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_VARIATE_RANDOM_RANDOM_DITRIBUTION_HPP
#define SRC_VARIATE_RANDOM_RANDOM_DITRIBUTION_HPP

#include <vector>
#include <sstream>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/range/iterator_range.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/exception.hpp>
#include <sprig/section.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "VariateRandomCommon.hpp"
#include "RandomDistributionDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:RandomDistribution:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:RandomDistribution:error")

namespace ktl {
	//
	// NativeRandomDistribution
	//
	template<typename Impl>
	KTL_INLINE void NativeRandomDistribution<Impl>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		);
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::uniform_smallint>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::uniform_int_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::uniform_01>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		impl_ = boost::make_shared<base_type>();
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::uniform_real_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::bernoulli_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<tTVReal>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	//	CHECK: binomial_distribution (Boost 1.47)
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::binomial_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast</*tTVInteger*/tjs_int>(*param[0]),
				static_cast<tTVReal>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast</*tTVInteger*/tjs_int>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::geometric_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::input_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::negative_binomial_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<tTVInteger>(*param[0]),
				static_cast<tTVReal>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<tTVInteger>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::poisson_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::input_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::exponential_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::gamma_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::weibull_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::extreme_value_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::normal_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::lognormal_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::chi_squared_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::cauchy_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::fisher_f_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::student_t_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::discrete_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		std::vector<base_type::input_type> weights;
		BOOST_FOREACH(tTJSVariant* e, boost::make_iterator_range(param, param + numparams)) {
			weights.push_back(static_cast<base_type::input_type>(*e));
		}
		impl_ = boost::make_shared<base_type>(weights);
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::piecewise_constant_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		tjs_int const intervals_num = numparams / 2 + numparams % 2;
		tjs_int const weights_num = intervals_num - 1;
		std::vector<base_type::result_type> intervals;
		BOOST_FOREACH(tTJSVariant* e, boost::make_iterator_range(param, param + intervals_num)) {
			intervals.push_back(static_cast<base_type::result_type>(*e));
		}
		std::vector<base_type::result_type> weights;
		if (intervals.size() >= 1) {
			BOOST_FOREACH(tTJSVariant* e, boost::make_iterator_range(param + intervals_num, param + intervals_num + weights_num)) {
				weights.push_back(static_cast<base_type::result_type>(*e));
			}
		}
		impl_ = boost::make_shared<base_type>(intervals, weights);
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::piecewise_linear_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		tjs_int const intervals_num = numparams / 2;
		tjs_int const weights_num = intervals_num;
		std::vector<base_type::result_type> intervals;
		BOOST_FOREACH(tTJSVariant* e, boost::make_iterator_range(param, param + intervals_num)) {
			intervals.push_back(static_cast<base_type::result_type>(*e));
		}
		std::vector<base_type::result_type> weights;
		BOOST_FOREACH(tTJSVariant* e, boost::make_iterator_range(param + intervals_num, param + intervals_num + weights_num)) {
			weights.push_back(static_cast<base_type::result_type>(*e));
		}
		impl_ = boost::make_shared<base_type>(intervals, weights);
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::triangle_distribution>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 3) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1]),
				static_cast<base_type::result_type>(*param[2])
				);
		} else if (numparams >= 2) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0]),
				static_cast<base_type::result_type>(*param[1])
				);
		} else if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<base_type::result_type>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<>
	KTL_INLINE void NativeRandomDistribution<ktl::RandomUtils::uniform_on_sphere>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		if (numparams >= 1) {
			impl_ = boost::make_shared<base_type>(
				static_cast<int>(*param[0])
				);
		} else {
			impl_ = boost::make_shared<base_type>();
		}
	}
	template<typename Impl>
	NativeRandomDistribution<Impl>::NativeRandomDistribution()
		: impl_(boost::make_shared<base_type>())
	{}
	template<typename Impl>
	NativeRandomDistribution<Impl>::NativeRandomDistribution(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize(numparams, param);
	}
	template<typename Impl>
	NativeRandomDistribution<Impl>::NativeRandomDistribution(wrapped_type const& impl)
		: impl_(impl)
	{}
	template<typename Impl>
	typename NativeRandomDistribution<Impl>::variant_type NativeRandomDistribution<Impl>::variant() const {
		return variant_type(impl_);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	template<typename Impl>
	KTL_INLINE RandomResult NativeRandomDistribution<Impl>::random(iTJSDispatch2* obj) {
		RandomEngineBase::variant_type eng(RandomEngineBase::getBase(obj)->variant());
		sprig::random::distribution_variate_visitor<RandomResult, wrapped_type> visitor(impl_);
		return boost::apply_visitor(visitor, eng);
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	template<typename Impl>
	KTL_INLINE void NativeRandomDistribution<Impl>::reset() {
		impl_.reset();
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	template<typename Impl>
	KTL_INLINE tTJSVariant NativeRandomDistribution<Impl>::serialize() const {
		std::basic_ostringstream<tjs_char> oss;
		oss << impl_;
		return tTJSString(oss.str().c_str());
	}
	template<typename Impl>
	KTL_INLINE void NativeRandomDistribution<Impl>::deserialize(tjs_char const* data) {
		std::basic_istringstream<tjs_char> iss(data ? data : SPRIG_KRKR_TJS_W(""));
		iss >> impl_;
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	template<typename Impl>
	KTL_INLINE RandomResult NativeRandomDistribution<Impl>::min() const {
		return impl_.min();
	}
	template<typename Impl>
	KTL_INLINE RandomResult NativeRandomDistribution<Impl>::max() const {
		return impl_.max();
	}

	//
	// RandomDistribution
	//
	template<typename Impl>
	RandomDistribution<Impl>::RandomDistribution() {}
	template<typename Impl>
	tjs_error TJS_INTF_METHOD RandomDistribution<Impl>::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("RandomDistribution::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeRandomDistribution<Impl> >(numparams, param);
		return TJS_S_OK;
	}
	template<typename Impl>
	void TJS_INTF_METHOD RandomDistribution<Impl>::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("RandomDistribution::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 内部系メソッド
	//
	template<typename Impl>
	KTL_INLINE sprig::krkr::tjs::intptr_type RandomDistribution<Impl>::getBase() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(
			static_cast<RandomDistributionBase*>(instance_.get())
			);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	template<typename Impl>
	KTL_INLINE tTJSVariant RandomDistribution<Impl>::random(iTJSDispatch2* obj) {
		return instance_->random(obj);
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	template<typename Impl>
	KTL_INLINE void RandomDistribution<Impl>::reset() {
		instance_->reset();
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	template<typename Impl>
	KTL_INLINE tTJSVariant RandomDistribution<Impl>::serialize() const {
		return instance_->serialize();
	}
	template<typename Impl>
	KTL_INLINE void RandomDistribution<Impl>::deserialize(tTJSVariantString const* data) {
		instance_->deserialize(sprig::krkr::tjs::as_c_str(data));
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	template<typename Impl>
	KTL_INLINE tTJSVariant RandomDistribution<Impl>::min() const {
		return instance_->min();
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant RandomDistribution<Impl>::max() const {
		return instance_->max();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_VARIATE_RANDOM_RANDOM_DITRIBUTION_HPP
