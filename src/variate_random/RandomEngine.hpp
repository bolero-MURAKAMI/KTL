/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_VARIATE_RANDOM_RANDOM_ENGINE_HPP
#define SRC_VARIATE_RANDOM_RANDOM_ENGINE_HPP

#include <vector>
#include <sstream>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/exception.hpp>
#include <sprig/section.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "VariateRandomCommon.hpp"
#include "RandomEngineDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:RandomEngine:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:RandomEngine:error")

namespace ktl {
	//
	// NativeStaticRandomEngine
	//
	//
	//	SUMMARY: 取得系メソッド
	//
	template<typename Impl>
	SPRIG_INLINE RandomResult NativeStaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_random() {
		return impl_type::static_random();
	}
	//
	//	SUMMARY: シード系メソッド
	//
	template<typename Impl>
	SPRIG_INLINE void NativeStaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_seed(
		tjs_int numparams,
		tTJSVariant** param
		)
	{}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	template<typename Impl>
	SPRIG_INLINE tTJSVariant NativeStaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_serialize() {
		std::basic_ostringstream<tjs_char> oss;
		impl_type::static_out(oss);
		return tTJSString(oss.str().c_str());
	}
	template<typename Impl>
	SPRIG_INLINE void NativeStaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_deserialize(tjs_char const* data) {
		std::basic_istringstream<tjs_char> iss(data ? data : SPRIG_KRKR_TJS_W(""));
		impl_type::static_in(iss);
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	template<typename Impl>
	SPRIG_INLINE RandomResult NativeStaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_min() {
		return impl_type::static_min();
	}
	template<typename Impl>
	SPRIG_INLINE RandomResult NativeStaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_max() {
		return impl_type::static_max();
	}
	//
	// NativeRandomEngine
	//
	template<typename Impl>
	template<typename IntType, typename Base>
	SPRIG_INLINE void NativeRandomEngine<Impl>::initialize_int_seed(
		Base& base,
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		typedef Base base_type;
		if (numparams == 0) {
			base.seed();
		} else if (numparams == 1) {
			base.seed(boost::numeric_cast<IntType>(param[0]->AsInteger()));
		} else {
			std::vector<IntType> v;
			BOOST_FOREACH(tTJSVariant* e, boost::make_iterator_range(param, param + numparams)) {
				v.push_back(boost::numeric_cast<IntType>(e->AsInteger()));
			}
			base.seed(v.begin(), v.end());
		}
	}
	template<typename Impl>
	template<typename Base>
	SPRIG_INLINE void NativeRandomEngine<Impl>::initialize_linear_congruential(
		Base& base,
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_int_seed<typename Base::result_type>(base, numparams, param);
	}
	template<typename Impl>
	template<typename Base>
	SPRIG_INLINE void NativeRandomEngine<Impl>::initialize_additive_combine(
		Base& base,
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_int_seed<typename Base::result_type>(base, numparams, param);
	}
	template<typename Impl>
	template<typename Base>
	SPRIG_INLINE void NativeRandomEngine<Impl>::initialize_shuffle_order(
		Base& base,
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_int_seed<typename Base::result_type>(base, numparams, param);
	}
	template<typename Impl>
	template<typename Base>
	SPRIG_INLINE void NativeRandomEngine<Impl>::initialize_xor_combine(
		Base& base,
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_int_seed<typename Base::result_type>(base, numparams, param);
	}
	template<typename Impl>
	template<typename Base>
	SPRIG_INLINE void NativeRandomEngine<Impl>::initialize_inversive_congruential(
		Base& base,
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_int_seed<typename Base::result_type>(base, numparams, param);
	}
	template<typename Impl>
	template<typename Base>
	SPRIG_INLINE void NativeRandomEngine<Impl>::initialize_mersenne_twister(
		Base& base,
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_int_seed<typename Base::result_type>(base, numparams, param);
	}
	template<typename Impl>
	template<typename Base>
	SPRIG_INLINE void NativeRandomEngine<Impl>::initialize_lagged_fibonacci(
		Base& base,
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_int_seed<boost::uint32_t>(base, numparams, param);
	}
	template<typename Impl>
	template<typename Base>
	SPRIG_INLINE void NativeRandomEngine<Impl>::initialize_ranlux(
		Base& base,
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_int_seed<boost::uint32_t>(base, numparams, param);
	}
	template<typename Impl>
	KTL_INLINE void NativeRandomEngine<Impl>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		);
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::minstd_rand0>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_linear_congruential(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::minstd_rand>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_linear_congruential(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::rand48>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_int_seed<ktl::RandomUtils::rand48::result_type>(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ecuyer1988>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_additive_combine(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::knuth_b>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_shuffle_order(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::kreutzer1986>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_shuffle_order(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::taus88>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_xor_combine(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::hellekalek1995>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_inversive_congruential(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::mt11213b>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_mersenne_twister(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::mt19937>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_mersenne_twister(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::mt19937_64>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_mersenne_twister(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::lagged_fibonacci607>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_lagged_fibonacci(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::lagged_fibonacci1279>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_lagged_fibonacci(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::lagged_fibonacci2281>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_lagged_fibonacci(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::lagged_fibonacci3217>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_lagged_fibonacci(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::lagged_fibonacci4423>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_lagged_fibonacci(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::lagged_fibonacci9689>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_lagged_fibonacci(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::lagged_fibonacci19937>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_lagged_fibonacci(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::lagged_fibonacci23209>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_lagged_fibonacci(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::lagged_fibonacci44497>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_lagged_fibonacci(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ranlux3>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_ranlux(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ranlux4>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_ranlux(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ranlux64_3>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_ranlux(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ranlux64_4>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_ranlux(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ranlux3_01>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_ranlux(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ranlux4_01>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_ranlux(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ranlux64_3_01>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_ranlux(impl_.base(), numparams, param);
	}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ranlux64_4_01>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize_ranlux(impl_.base(), numparams, param);
	}
	//	CHECK: fatal error C1060
	//template<>
	//KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ranlux24>::initialize(
	//	tjs_int numparams,
	//	tTJSVariant** param
	//	)
	//{
	//	initialize_ranlux(impl_.base(), numparams, param);
	//}
	//template<>
	//KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::ranlux48>::initialize(
	//	tjs_int numparams,
	//	tTJSVariant** param
	//	)
	//{
	//	initialize_ranlux(impl_.base(), numparams, param);
	//}
	template<>
	KTL_INLINE void NativeRandomEngine<ktl::RandomUtils::random_device>::initialize(
		tjs_int numparams,
		tTJSVariant** param
		)
	{}
	template<typename Impl>
	NativeRandomEngine<Impl>::NativeRandomEngine()
		: impl_(boost::make_shared<base_type>())
	{}
	template<typename Impl>
	NativeRandomEngine<Impl>::NativeRandomEngine(
		tjs_int numparams,
		tTJSVariant** param
		)
		: impl_(boost::make_shared<base_type>())
	{
		initialize(numparams, param);
	}
	template<typename Impl>
	NativeRandomEngine<Impl>::NativeRandomEngine(wrapped_type const& impl)
		: impl_(impl)
	{}
	template<typename Impl>
	typename NativeRandomEngine<Impl>::variant_type NativeRandomEngine<Impl>::variant() const {
		return variant_type(impl_);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	template<typename Impl>
	KTL_INLINE RandomResult NativeRandomEngine<Impl>::random() {
		return impl_();
	}
	//
	//	SUMMARY: シード系メソッド
	//
	template<typename Impl>
	KTL_INLINE void NativeRandomEngine<Impl>::seed(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		initialize(numparams, param);
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	template<typename Impl>
	KTL_INLINE tTJSVariant NativeRandomEngine<Impl>::serialize() const {
		std::basic_ostringstream<tjs_char> oss;
		oss << impl_;
		return tTJSString(oss.str().c_str());
	}
	template<typename Impl>
	KTL_INLINE void NativeRandomEngine<Impl>::deserialize(tjs_char const* data) {
		std::basic_istringstream<tjs_char> iss(data ? data : SPRIG_KRKR_TJS_W(""));
		iss >> impl_;
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	template<typename Impl>
	KTL_INLINE RandomResult NativeRandomEngine<Impl>::min() const {
		return impl_.min();
	}
	template<typename Impl>
	KTL_INLINE RandomResult NativeRandomEngine<Impl>::max() const {
		return impl_.max();
	}

	//
	// StaticRandomEngine
	//
	//
	//	SUMMARY: 取得系メソッド
	//
	template<typename Impl>
	SPRIG_INLINE tTJSVariant StaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_random() {
		return NativeRandomEngine<impl_type>::static_random();
	}
	//
	//	SUMMARY: シード系メソッド
	//
	template<typename Impl>
	SPRIG_INLINE void StaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_seed(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		NativeRandomEngine<impl_type>::static_seed(numparams, param);
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	template<typename Impl>
	SPRIG_INLINE tTJSVariant StaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_serialize() {
		return NativeRandomEngine<impl_type>::static_serialize();
	}
	template<typename Impl>
	SPRIG_INLINE void StaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_deserialize(tTJSVariantString const* data) {
		NativeRandomEngine<impl_type>::static_deserialize(sprig::krkr::tjs::as_c_str(data));
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	template<typename Impl>
	SPRIG_INLINE tTJSVariant StaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_min() {
		return NativeRandomEngine<impl_type>::static_min();
	}
	template<typename Impl>
	SPRIG_INLINE tTJSVariant StaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type>::static_max() {
		return NativeRandomEngine<impl_type>::static_max();
	}
	//
	// RandomEngine
	//
	template<typename Impl>
	RandomEngine<Impl>::RandomEngine() {}
	template<typename Impl>
	tjs_error TJS_INTF_METHOD RandomEngine<Impl>::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("RandomEngine::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeRandomEngine<Impl> >(numparams, param);
		return TJS_S_OK;
	}
	template<typename Impl>
	void TJS_INTF_METHOD RandomEngine<Impl>::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("RandomEngine::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 内部系メソッド
	//
	template<typename Impl>
	KTL_INLINE sprig::krkr::tjs::intptr_type RandomEngine<Impl>::getBase() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(
			static_cast<RandomEngineBase*>(instance_.get())
			);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	template<typename Impl>
	KTL_INLINE tTJSVariant RandomEngine<Impl>::random() {
		return instance_->random();
	}
	//
	//	SUMMARY: シード系メソッド
	//
	template<typename Impl>
	KTL_INLINE void RandomEngine<Impl>::seed(
		tjs_int numparams,
		tTJSVariant** param
		)
	{
		instance_->seed(numparams, param);
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	template<typename Impl>
	KTL_INLINE tTJSVariant RandomEngine<Impl>::serialize() const {
		return instance_->serialize();
	}
	template<typename Impl>
	KTL_INLINE void RandomEngine<Impl>::deserialize(tTJSVariantString const* data) {
		instance_->deserialize(sprig::krkr::tjs::as_c_str(data));
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	template<typename Impl>
	KTL_INLINE tTJSVariant RandomEngine<Impl>::min() const {
		return instance_->min();
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant RandomEngine<Impl>::max() const {
		return instance_->max();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_VARIATE_RANDOM_RANDOM_ENGINE_HPP
