/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_VARIATE_RANDOM_VARIATE_RANDOM_COMMON_HPP
#define SRC_VARIATE_RANDOM_VARIATE_RANDOM_COMMON_HPP

#include <boost/foreach.hpp>
#include <boost/variant.hpp>
#include <boost/random.hpp>
#include <boost/range/reference.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/transform.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/utility/swap.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/random/adapter.hpp>
#include <sprig/random/null.hpp>
#include <sprig/random/any.hpp>
#include <sprig/random/variant.hpp>
#include <sprig/random/nondet_random.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>

//
// KTL_VARIATE_RANDOM_SEQ_AT_GEN
//
#define KTL_VARIATE_RANDOM_SEQ_AT_GEN(z, n, d) boost::mpl::at_c<d, n>::type

namespace ktl {
	namespace patch {
		//
		// geometric_distribution
		//	for: Boost 1.47
		//
		template<
			typename IntType = boost::random::geometric_distribution<>::result_type,
			typename RealType = boost::random::geometric_distribution<>::input_type
			>
		class geometric_distribution
			: public boost::random::geometric_distribution<IntType, RealType>
		{
		private:
			typedef boost::random::geometric_distribution<IntType, RealType> base_type;
		public:
			explicit geometric_distribution(typename base_type::input_type const& p = typename base_type::input_type(0.5))
				: base_type(p)
			{}
			explicit geometric_distribution(typename base_type::param_type const& param)
				: base_type(param)
			{}
			typename base_type::result_type min() const {
				return typename base_type::result_type(0);
			}
		};
	}	// namespace patch
	//
	// RandomResult
	//
	class RandomResult {
	private:
		tTJSVariant t_;
	public:
		RandomResult() {}
		RandomResult(RandomResult const& other)
			: t_(other.t_)
		{}
		RandomResult(tTJSVariant const& t)
			: t_(t)
		{}
		template<typename T>
		RandomResult(
			T t,
			typename boost::enable_if<boost::is_unsigned<T> >::type* = 0
			)
			: t_(static_cast<tTVInteger>(t))
		{}
		RandomResult(boost::random::uniform_on_sphere<tTVReal>::result_type const& t) {
			typedef boost::random::uniform_on_sphere<tTVReal>::result_type const range_type;
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			sprig::krkr::tjs::object_type obj(result_obj, false);
			{
				tjs_int num = 0;
				BOOST_FOREACH(boost::range_reference<range_type>::type e, t) {
					tTJSVariant param(e);
					sprig::krkr::tjs::PropSetByNum(
						obj.get(),
						TJS_MEMBERENSURE,
						num,
						&param,
						obj.get()
						);
					++num;
				}
			}
			t_ = tTJSVariant(obj.get(), obj.get());
		}
		RandomResult& operator=(RandomResult const& other) {
			RandomResult temp(other);
			temp.swap(*this);
			return *this;
		}
		RandomResult& operator=(tTJSVariant const& t) {
			RandomResult temp(t);
			temp.swap(*this);
			return *this;
		}
		RandomResult& operator=(boost::random::uniform_on_sphere<tTVReal>::result_type const& t) {
			RandomResult temp(t);
			temp.swap(*this);
			return *this;
		}
		void swap(RandomResult& other) {
			boost::swap(t_, other.t_);
		}
		operator tTJSVariant const&() const {
			return t_;
		}
		tTJSVariant const& get() const {
			return t_;
		}
	};
	//
	// RandomUtils
	//
	struct RandomUtils {
	public:
		BOOST_MPL_HAS_XXX_TRAIT_DEF(is_static_random_engine);
		template<typename Engine>
		struct enable_static_random_engine
			: public boost::enable_if<has_is_static_random_engine<Engine> >
		{};
		template<typename Engine>
		struct engine_wrap
			: public boost::mpl::identity<sprig::random::shared_engine_adapter<Engine, true> >
		{};
		template<typename Distribution>
		struct distribution_wrap
			: public boost::mpl::identity<sprig::random::shared_distribution_adapter<Distribution> >
		{};
	public:
		//	CHECK: fatal error C1060
#define KTL_VARIATE_RANDOM_ENGINE_SEQ_NUM 29/*31*/
#define KTL_VARIATE_RANDOM_DISTRIBUTION_SEQ_NUM 24

		typedef sprig::random::any_variate_generator<RandomResult> variate_generator_type;
		typedef boost::random::minstd_rand0 minstd_rand0;
		typedef boost::random::minstd_rand minstd_rand;
		typedef boost::random::rand48 rand48;
		typedef boost::random::ecuyer1988 ecuyer1988;
		typedef boost::random::knuth_b knuth_b;
		typedef boost::random::kreutzer1986 kreutzer1986;
		typedef boost::random::taus88 taus88;
		typedef boost::random::hellekalek1995 hellekalek1995;
		typedef boost::random::mt11213b mt11213b;
		typedef boost::random::mt19937 mt19937;
		typedef boost::random::mt19937_64 mt19937_64;
		typedef boost::random::lagged_fibonacci607 lagged_fibonacci607;
		typedef boost::random::lagged_fibonacci1279 lagged_fibonacci1279;
		typedef boost::random::lagged_fibonacci2281 lagged_fibonacci2281;
		typedef boost::random::lagged_fibonacci3217 lagged_fibonacci3217;
		typedef boost::random::lagged_fibonacci4423 lagged_fibonacci4423;
		typedef boost::random::lagged_fibonacci9689 lagged_fibonacci9689;
		typedef boost::random::lagged_fibonacci19937 lagged_fibonacci19937;
		typedef boost::random::lagged_fibonacci23209 lagged_fibonacci23209;
		typedef boost::random::lagged_fibonacci44497 lagged_fibonacci44497;
		typedef boost::random::ranlux3 ranlux3;
		typedef boost::random::ranlux4 ranlux4;
		typedef boost::random::ranlux64_3 ranlux64_3;
		typedef boost::random::ranlux64_4 ranlux64_4;
		typedef boost::random::ranlux3_01 ranlux3_01;
		typedef boost::random::ranlux4_01 ranlux4_01;
		typedef boost::random::ranlux64_3_01 ranlux64_3_01;
		typedef boost::random::ranlux64_4_01 ranlux64_4_01;
		typedef boost::random::ranlux24 ranlux24;
		typedef boost::random::ranlux48 ranlux48;
		typedef sprig::random::singleton_random_device random_device;
		typedef boost::mpl::BOOST_PP_CAT(list, KTL_VARIATE_RANDOM_ENGINE_SEQ_NUM)<
			minstd_rand0,
			minstd_rand,
			rand48,
			ecuyer1988,
			knuth_b,
			kreutzer1986,
			taus88,
			hellekalek1995,
			mt11213b,
			mt19937,
			mt19937_64,
			lagged_fibonacci607,
			lagged_fibonacci1279,
			lagged_fibonacci2281,
			lagged_fibonacci3217,
			lagged_fibonacci4423,
			lagged_fibonacci9689,
			lagged_fibonacci19937,
			lagged_fibonacci23209,
			lagged_fibonacci44497,
			ranlux3,
			ranlux4,
			ranlux64_3,
			ranlux64_4,
			ranlux3_01,
			ranlux4_01,
			ranlux64_3_01,
			ranlux64_4_01,
			//	CHECK: fatal error C1060
			//ranlux24,
			//ranlux48,
			random_device
		> engine_seq_type;
		typedef boost::random::uniform_smallint<tTVInteger> uniform_smallint;
		typedef boost::random::uniform_int_distribution<tTVInteger> uniform_int_distribution;
		typedef boost::random::uniform_01<tTVReal, tTVReal> uniform_01;
		typedef boost::random::uniform_real_distribution<tTVReal> uniform_real_distribution;
		typedef boost::random::bernoulli_distribution<tTVReal> bernoulli_distribution;
		//	CHECK: binomial_distribution (Boost 1.47)
		typedef boost::random::binomial_distribution</*tTVInteger*/tjs_int, tTVReal> binomial_distribution;
		typedef boost::random::geometric_distribution<tTVInteger, tTVReal> geometric_distribution;
		typedef boost::random::negative_binomial_distribution<tTVInteger, tTVReal> negative_binomial_distribution;
		typedef boost::random::poisson_distribution<tTVInteger, tTVReal> poisson_distribution;
		typedef boost::random::exponential_distribution<tTVReal> exponential_distribution;
		typedef boost::random::gamma_distribution<tTVReal> gamma_distribution;
		typedef boost::random::weibull_distribution<tTVReal> weibull_distribution;
		typedef boost::random::extreme_value_distribution<tTVReal> extreme_value_distribution;
		typedef boost::random::normal_distribution<tTVReal> normal_distribution;
		typedef boost::random::lognormal_distribution<tTVReal> lognormal_distribution;
		typedef boost::random::chi_squared_distribution<tTVReal> chi_squared_distribution;
		typedef boost::random::cauchy_distribution<tTVReal> cauchy_distribution;
		typedef boost::random::fisher_f_distribution<tTVReal> fisher_f_distribution;
		typedef boost::random::student_t_distribution<tTVReal> student_t_distribution;
		typedef boost::random::discrete_distribution<tTVInteger, tTVReal> discrete_distribution;
		typedef boost::random::piecewise_constant_distribution<tTVReal, tTVReal> piecewise_constant_distribution;
		typedef boost::random::piecewise_linear_distribution<tTVReal> piecewise_linear_distribution;
		typedef boost::random::triangle_distribution<tTVReal> triangle_distribution;
		typedef boost::random::uniform_on_sphere<tTVReal> uniform_on_sphere;
		typedef boost::mpl::BOOST_PP_CAT(list, KTL_VARIATE_RANDOM_DISTRIBUTION_SEQ_NUM)<
			uniform_smallint,
			uniform_int_distribution,
			uniform_01,
			uniform_real_distribution,
			bernoulli_distribution,
			binomial_distribution,
			geometric_distribution,
			negative_binomial_distribution,
			poisson_distribution,
			exponential_distribution,
			gamma_distribution,
			weibull_distribution,
			extreme_value_distribution,
			normal_distribution,
			lognormal_distribution,
			chi_squared_distribution,
			cauchy_distribution,
			fisher_f_distribution,
			student_t_distribution,
			discrete_distribution,
			piecewise_constant_distribution,
			piecewise_linear_distribution,
			triangle_distribution,
			uniform_on_sphere
		> distribution_seq_type;
		typedef boost::mpl::transform<
			engine_seq_type,
			boost::mpl::quote1<engine_wrap>
		>::type wrapped_engine_seq_type;
		typedef boost::mpl::transform<
			distribution_seq_type,
			boost::mpl::quote1<distribution_wrap>
		>::type wrapped_distribution_seq_type;
		typedef boost::variant<
			sprig::random::null_engine<>,
			BOOST_PP_ENUM(KTL_VARIATE_RANDOM_ENGINE_SEQ_NUM, KTL_VARIATE_RANDOM_SEQ_AT_GEN, wrapped_engine_seq_type)
		> engine_variant_type;
		typedef boost::variant<
			sprig::random::null_distribution<RandomResult>,
			BOOST_PP_ENUM(KTL_VARIATE_RANDOM_DISTRIBUTION_SEQ_NUM, KTL_VARIATE_RANDOM_SEQ_AT_GEN, wrapped_distribution_seq_type)
		> distribution_variant_type;
	private:
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size<engine_seq_type>, boost::mpl::size_t<KTL_VARIATE_RANDOM_ENGINE_SEQ_NUM> >));
		BOOST_MPL_ASSERT((boost::mpl::equal_to<boost::mpl::size<distribution_seq_type>, boost::mpl::size_t<KTL_VARIATE_RANDOM_DISTRIBUTION_SEQ_NUM> >));
	public:
		static sprig::krkr::tjs::intptr_type getBase(iTJSDispatch2* obj) {
			tTJSVariant result;
			sprig::krkr::tjs::MemberGet(
				obj,
				SPRIG_KRKR_TJS_W("base"),
				&result
				);
			return static_cast<sprig::krkr::tjs::intptr_type>(result);
		}
	};
	//
	// RandomEngineBase
	//
	class RandomEngineBase {
	public:
		typedef RandomUtils::engine_variant_type variant_type;
	public:
		static RandomEngineBase* getBase(iTJSDispatch2* obj) {
			return reinterpret_cast<RandomEngineBase*>(RandomUtils::getBase(obj));
		}
	public:
		virtual ~RandomEngineBase() {}
		virtual variant_type variant() const = 0;
	};
	//
	// RandomDistributionBase
	//
	class RandomDistributionBase {
	public:
		typedef RandomUtils::distribution_variant_type variant_type;
	public:
		static RandomDistributionBase* getBase(iTJSDispatch2* obj) {
			return reinterpret_cast<RandomDistributionBase*>(RandomUtils::getBase(obj));
		}
	public:
		virtual ~RandomDistributionBase() {}
		virtual variant_type variant() const = 0;
	};
}	// namespace ktl

#endif	// #ifndef SRC_VARIATE_RANDOM_VARIATE_RANDOM_COMMON_HPP
