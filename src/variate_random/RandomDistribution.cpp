/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "RandomDistribution.hpp"

namespace ktl {
	//
	// RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS
	//
#define RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(FUNCTION_NAME, CLASS_NAME, ENTRY_NAME) \
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(FUNCTION_NAME, CLASS_NAME, ENTRY_NAME) \
	/**/ \
	/*	SUMMARY: 内部系メソッド */ \
	/**/ \
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(CLASS_NAME, base, getBase); \
	/**/ \
	/*	SUMMARY: 取得系メソッド */ \
	/**/ \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(random) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject); \
		SPRIG_KRKR_INVOKE_RESULT_SET( \
			this_->random(SPRIG_KRKR_ARG_OBJECT(0)) \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(random); \
	/**/ \
	/*	SUMMARY: 初期化系メソッド */ \
	/**/ \
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(CLASS_NAME, reset); \
	/**/ \
	/*	SUMMARY: シリアライズ系メソッド */ \
	/**/ \
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VARIANT(CLASS_NAME, serialize); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(deserialize) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		this_->deserialize(SPRIG_KRKR_ARG_STRING(0)); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(deserialize); \
	/**/ \
	/*	SUMMARY: 取得プロパティ系メソッド */ \
	/**/ \
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(CLASS_NAME, min, min); \
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(CLASS_NAME, max, max); \
		\
	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();

	//
	// CreateNativeClassUniformSmallint
	//
#define TJS_NATIVE_CLASSID_NAME UniformSmallintClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassUniformSmallint, RandomDistribution<ktl::RandomUtils::uniform_smallint>, UniformSmallint);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassUniformIntDistribution
	//
#define TJS_NATIVE_CLASSID_NAME UniformIntDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassUniformIntDistribution, RandomDistribution<ktl::RandomUtils::uniform_int_distribution>, UniformIntDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassUniform01
	//
#define TJS_NATIVE_CLASSID_NAME Uniform01ClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassUniform01, RandomDistribution<ktl::RandomUtils::uniform_01>, Uniform01);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassUniformRealDistribution
	//
#define TJS_NATIVE_CLASSID_NAME UniformRealDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassUniformRealDistribution, RandomDistribution<ktl::RandomUtils::uniform_real_distribution>, UniformRealDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassBernoulliDistribution
	//
#define TJS_NATIVE_CLASSID_NAME BernoulliDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassBernoulliDistribution, RandomDistribution<ktl::RandomUtils::bernoulli_distribution>, BernoulliDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassBinomialDistribution
	//
#define TJS_NATIVE_CLASSID_NAME BinomialDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassBinomialDistribution, RandomDistribution<ktl::RandomUtils::binomial_distribution>, BinomialDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassGeometricDistribution
	//
#define TJS_NATIVE_CLASSID_NAME GeometricDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassGeometricDistribution, RandomDistribution<ktl::RandomUtils::geometric_distribution>, GeometricDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassNegativeBinomialDistribution
	//
#define TJS_NATIVE_CLASSID_NAME NegativeBinomialDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassNegativeBinomialDistribution, RandomDistribution<ktl::RandomUtils::negative_binomial_distribution>, NegativeBinomialDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassPoissonDistribution
	//
#define TJS_NATIVE_CLASSID_NAME PoissonDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassPoissonDistribution, RandomDistribution<ktl::RandomUtils::poisson_distribution>, PoissonDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassExponentialDistribution
	//
#define TJS_NATIVE_CLASSID_NAME ExponentialDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassExponentialDistribution, RandomDistribution<ktl::RandomUtils::exponential_distribution>, ExponentialDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassGammaDistribution
	//
#define TJS_NATIVE_CLASSID_NAME GammaDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassGammaDistribution, RandomDistribution<ktl::RandomUtils::gamma_distribution>, GammaDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassWeibullDistribution
	//
#define TJS_NATIVE_CLASSID_NAME WeibullDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassWeibullDistribution, RandomDistribution<ktl::RandomUtils::weibull_distribution>, WeibullDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassExtremeValueDistribution
	//
#define TJS_NATIVE_CLASSID_NAMEExtremeValueDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassExtremeValueDistribution, RandomDistribution<ktl::RandomUtils::extreme_value_distribution>, ExtremeValueDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassNormalDistribution
	//
#define TJS_NATIVE_CLASSID_NAME NormalDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassNormalDistribution, RandomDistribution<ktl::RandomUtils::normal_distribution>, NormalDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassLognormalDistribution
	//
#define TJS_NATIVE_CLASSID_NAME LognormalDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassLognormalDistribution, RandomDistribution<ktl::RandomUtils::lognormal_distribution>, LognormalDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassChiSquaredDistribution
	//
#define TJS_NATIVE_CLASSID_NAME ChiSquaredDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassChiSquaredDistribution, RandomDistribution<ktl::RandomUtils::chi_squared_distribution>, ChiSquaredDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassCauchyDistribution
	//
#define TJS_NATIVE_CLASSID_NAME CauchyDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassCauchyDistribution, RandomDistribution<ktl::RandomUtils::cauchy_distribution>, CauchyDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassFisherFDistribution
	//
#define TJS_NATIVE_CLASSID_NAME FisherFDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassFisherFDistribution, RandomDistribution<ktl::RandomUtils::fisher_f_distribution>, FisherFDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassStudentTDistribution
	//
#define TJS_NATIVE_CLASSID_NAME StudentTDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassStudentTDistribution, RandomDistribution<ktl::RandomUtils::student_t_distribution>, StudentTDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassDiscreteDistribution
	//
#define TJS_NATIVE_CLASSID_NAME DiscreteDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassDiscreteDistribution, RandomDistribution<ktl::RandomUtils::discrete_distribution>, DiscreteDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassPiecewiseConstantDistribution
	//
#define TJS_NATIVE_CLASSID_NAME PiecewiseConstantDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassPiecewiseConstantDistribution, RandomDistribution<ktl::RandomUtils::piecewise_constant_distribution>, PiecewiseConstantDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassPiecewiseLinearDistribution
	//
#define TJS_NATIVE_CLASSID_NAME PiecewiseLinearDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassPiecewiseLinearDistribution, RandomDistribution<ktl::RandomUtils::piecewise_linear_distribution>, PiecewiseLinearDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassTriangleDistribution
	//
#define TJS_NATIVE_CLASSID_NAME TriangleDistributionClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassTriangleDistribution, RandomDistribution<ktl::RandomUtils::triangle_distribution>, TriangleDistribution);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassUniformOnSphere
	//
#define TJS_NATIVE_CLASSID_NAME UniformOnSphereClassID
	RANDOM_DISTRIBUTION_CREATE_NATIVE_CLASS(CreateNativeClassUniformOnSphere, RandomDistribution<ktl::RandomUtils::uniform_on_sphere>, UniformOnSphere);
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_random_distribution
// unregist_ktl_random_distribution
//
static void regist_ktl_random_distribution() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:RandomDistribution:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("UniformSmallint"), ktl::CreateNativeClassUniformSmallint());
		{
			sprig::krkr::tjs::object_type obj(ktl::CreateNativeClassUniformIntDistribution(), false);
			sprig::krkr::tjs::AddMemberNoRelease(global.get(), SPRIG_KRKR_TJS_W("UniformIntDistribution"), obj.get());
			sprig::krkr::tjs::AddMemberNoRelease(global.get(), SPRIG_KRKR_TJS_W("UniformInt"), obj.get());
		}
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Uniform01"), ktl::CreateNativeClassUniform01());
		{
			sprig::krkr::tjs::object_type obj(ktl::CreateNativeClassUniformRealDistribution(), false);
			sprig::krkr::tjs::AddMemberNoRelease(global.get(), SPRIG_KRKR_TJS_W("UniformRealDistribution"), obj.get());
			sprig::krkr::tjs::AddMemberNoRelease(global.get(), SPRIG_KRKR_TJS_W("UniformReal"), obj.get());
		}
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("BernoulliDistribution"), ktl::CreateNativeClassBernoulliDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("BinomialDistribution"), ktl::CreateNativeClassBinomialDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("GeometricDistribution"), ktl::CreateNativeClassGeometricDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("NegativeBinomialDistribution"), ktl::CreateNativeClassNegativeBinomialDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("PoissonDistribution"), ktl::CreateNativeClassPoissonDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("ExponentialDistribution"), ktl::CreateNativeClassExponentialDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("GammaDistribution"), ktl::CreateNativeClassGammaDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("WeibullDistribution"), ktl::CreateNativeClassWeibullDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("ExtremeValueDistribution"), ktl::CreateNativeClassExtremeValueDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("NormalDistribution"), ktl::CreateNativeClassNormalDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LognormalDistribution"), ktl::CreateNativeClassLognormalDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("ChiSquaredDistribution"), ktl::CreateNativeClassChiSquaredDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("CauchyDistribution"), ktl::CreateNativeClassCauchyDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("FisherFDistribution"), ktl::CreateNativeClassFisherFDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("StudentTDistribution"), ktl::CreateNativeClassStudentTDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("DiscreteDistribution"), ktl::CreateNativeClassDiscreteDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("PiecewiseConstantDistribution"), ktl::CreateNativeClassPiecewiseConstantDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("PiecewiseLinearDistribution"), ktl::CreateNativeClassPiecewiseLinearDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("TriangleDistribution"), ktl::CreateNativeClassTriangleDistribution());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("UniformOnSphere"), ktl::CreateNativeClassUniformOnSphere());
	}
}
static void unregist_ktl_random_distribution() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:RandomDistribution:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("UniformSmallint"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("UniformIntDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("UniformInt"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Uniform01"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("UniformRealDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("UniformReal"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("BernoulliDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("BinomialDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("GeometricDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("NegativeBinomialDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("PoissonDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("ExponentialDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("GammaDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("WeibullDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("ExtremeValueDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("NormalDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LognormalDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("ChiSquaredDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("CauchyDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("FisherFDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("StudentTDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("DiscreteDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("PiecewiseConstantDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("PiecewiseLinearDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("TriangleDistribution"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("UniformOnSphere"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_random_distribution);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_random_distribution);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:RandomDistribution:information"));
