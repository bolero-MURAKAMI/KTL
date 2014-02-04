/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_VARIATE_RANDOM_RANDOM_ENGINE_DECL_HPP
#define SRC_VARIATE_RANDOM_RANDOM_ENGINE_DECL_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/utility/enable_if.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

#include "VariateRandomCommon.hpp"

namespace ktl {
	//
	// NativeStaticRandomEngine
	//
	template<typename Impl, typename Enable = void>
	class NativeStaticRandomEngine {};
	template<typename Impl>
	class NativeStaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type> {
	public:
		typedef Impl impl_type;
	public:
		//
		//	SUMMARY: 取得系メソッド
		//
		static RandomResult static_random();
		//
		//	SUMMARY: シード系メソッド
		//
		static void static_seed(
			tjs_int numparams,
			tTJSVariant** param
			);
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		static tTJSVariant static_serialize();
		static void static_deserialize(tjs_char const* data);
		//
		//	SUMMARY: 取得プロパティ系メソッド
		//
		static RandomResult static_min();
		static RandomResult static_max();
	};
	//
	// NativeRandomEngine
	//
	template<typename Impl>
	class NativeRandomEngine
		: public RandomEngineBase
		, public NativeStaticRandomEngine<Impl>
	{
	public:
		typedef Impl impl_type;
		typedef typename RandomUtils::engine_wrap<impl_type>::type wrapped_type;
		typedef typename wrapped_type::base_type base_type;
		typedef RandomEngineBase::variant_type variant_type;
	private:
		template<typename IntType, typename Base>
		static void initialize_int_seed(
			Base& base,
			tjs_int numparams,
			tTJSVariant** param
			);
		template<typename Base>
		static void initialize_uint_seed(
			Base& base,
			tjs_int numparams,
			tTJSVariant** param
			);
		template<typename Base>
		static void initialize_linear_congruential(
			Base& base,
			tjs_int numparams,
			tTJSVariant** param
			);
		template<typename Base>
		static void initialize_additive_combine(
			Base& base,
			tjs_int numparams,
			tTJSVariant** param
			);
		template<typename Base>
		static void initialize_shuffle_order(
			Base& base,
			tjs_int numparams,
			tTJSVariant** param
			);
		template<typename Base>
		static void initialize_xor_combine(
			Base& base,
			tjs_int numparams,
			tTJSVariant** param
			);
		template<typename Base>
		static void initialize_inversive_congruential(
			Base& base,
			tjs_int numparams,
			tTJSVariant** param
			);
		template<typename Base>
		static void initialize_mersenne_twister(
			Base& base,
			tjs_int numparams,
			tTJSVariant** param
			);
		template<typename Base>
		static void initialize_lagged_fibonacci(
			Base& base,
			tjs_int numparams,
			tTJSVariant** param
			);
		template<typename Base>
		static void initialize_ranlux(
			Base& base,
			tjs_int numparams,
			tTJSVariant** param
			);
	private:
		wrapped_type impl_;
	private:
		void initialize(
			tjs_int numparams,
			tTJSVariant** param
			);
	public:
		NativeRandomEngine();
		NativeRandomEngine(
			tjs_int numparams,
			tTJSVariant** param
			);
		explicit NativeRandomEngine(wrapped_type const& impl);
		variant_type variant() const;
	public:
		//
		//	SUMMARY: 取得系メソッド
		//
		RandomResult random();
		//
		//	SUMMARY: シード系メソッド
		//
		void seed(
			tjs_int numparams,
			tTJSVariant** param
			);
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		tTJSVariant serialize() const;
		void deserialize(tjs_char const* data);
		//
		//	SUMMARY: 取得プロパティ系メソッド
		//
		RandomResult min() const;
		RandomResult max() const;
	};

	//
	// StaticRandomEngine
	//
	template<typename Impl, typename Enable = void>
	class StaticRandomEngine {};
	template<typename Impl>
	class StaticRandomEngine<Impl, typename RandomUtils::enable_static_random_engine<Impl>::type> {
	public:
		typedef Impl impl_type;
	public:
		//
		//	SUMMARY: 取得系メソッド
		//
		static tTJSVariant static_random();
		//
		//	SUMMARY: シード系メソッド
		//
		static void static_seed(
			tjs_int numparams,
			tTJSVariant** param
			);
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		static tTJSVariant static_serialize();
		static void static_deserialize(tTJSVariantString const* data);
		//
		//	SUMMARY: 取得プロパティ系メソッド
		//
		static tTJSVariant static_min();
		static tTJSVariant static_max();
	};
	//
	// RandomEngine
	//
	template<typename Impl>
	class RandomEngine
		: public tTJSNativeInstance
		, public StaticRandomEngine<Impl>
	{
	public:
		typedef Impl impl_type;
		typedef typename NativeRandomEngine<impl_type>::wrapped_type wrapped_type;
	private:
		boost::shared_ptr<NativeRandomEngine<impl_type> > instance_;
	public:
		RandomEngine();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: 内部系メソッド
		//
		sprig::krkr::tjs::intptr_type getBase();
		//
		//	SUMMARY: 取得系メソッド
		//
		tTJSVariant random();
		//
		//	SUMMARY: シード系メソッド
		//
		void seed(
			tjs_int numparams,
			tTJSVariant** param
			);
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		tTJSVariant serialize() const;
		void deserialize(tTJSVariantString const* data);
		//
		//	SUMMARY: 取得プロパティ系メソッド
		//
		tTJSVariant min() const;
		tTJSVariant max() const;
	};
}	// namespace ktl

#endif	// #ifndef SRC_VARIATE_RANDOM_RANDOM_ENGINE_DECL_HPP
