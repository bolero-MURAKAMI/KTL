/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_VARIATE_RANDOM_RANDOM_DITRIBUTION_DECL_HPP
#define SRC_VARIATE_RANDOM_RANDOM_DITRIBUTION_DECL_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

#include "VariateRandomCommon.hpp"

namespace ktl {
	//
	// NativeRandomDistribution
	//
	template<typename Impl>
	class NativeRandomDistribution
		: public RandomDistributionBase
	{
	public:
		typedef Impl impl_type;
		typedef typename RandomUtils::distribution_wrap<impl_type>::type wrapped_type;
		typedef typename wrapped_type::base_type base_type;
		typedef RandomDistributionBase::variant_type variant_type;
	private:
		wrapped_type impl_;
	private:
		void initialize(
			tjs_int numparams,
			tTJSVariant** param
			);
	public:
		NativeRandomDistribution();
		NativeRandomDistribution(
			tjs_int numparams,
			tTJSVariant** param
			);
		explicit NativeRandomDistribution(wrapped_type const& impl);
		variant_type variant() const;
	public:
		//
		//	SUMMARY: 取得系メソッド
		//
		RandomResult random(iTJSDispatch2* obj);
		//
		//	SUMMARY: 初期化系メソッド
		//
		void reset();
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
	// RandomDistribution
	//
	template<typename Impl>
	class RandomDistribution
		: public tTJSNativeInstance
	{
	public:
		typedef typename NativeRandomDistribution<Impl>::wrapped_type wrapped_type;
	private:
		boost::shared_ptr<NativeRandomDistribution<Impl> > instance_;
	public:
		RandomDistribution();
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
		tTJSVariant random(iTJSDispatch2* obj);
		//
		//	SUMMARY: 初期化系メソッド
		//
		void reset();
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

#endif	// #ifndef SRC_VARIATE_RANDOM_RANDOM_DITRIBUTION_DECL_HPP
