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

#include "RandomEngine.hpp"

namespace ktl {
	//
	// STATIC_RANDOM_ENGINE_CREATE_NATIVE_CLASS
	//
#define STATIC_RANDOM_ENGINE_CREATE_NATIVE_CLASS(FUNCTION_NAME, CLASS_NAME, ENTRY_NAME) \
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
		SPRIG_KRKR_INVOKE_RESULT_SET( \
			CLASS_NAME::static_random() \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(random); \
	/**/ \
	/*	SUMMARY: シード系メソッド */ \
	/**/ \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(seed) \
	{ \
		CLASS_NAME::static_seed(numparams, param); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(seed); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(randomize) \
	{ \
		CLASS_NAME::static_seed(numparams, param); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(randomize); \
	/**/ \
	/*	SUMMARY: シリアライズ系メソッド */ \
	/**/ \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(serialize) \
	{ \
		SPRIG_KRKR_INVOKE_RESULT_SET( \
			CLASS_NAME::static_serialize() \
			); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(serialize); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(deserialize) \
	{ \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_TYPE_CHECK(0, tvtString); \
		CLASS_NAME::static_deserialize(SPRIG_KRKR_ARG_STRING(0)); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(deserialize); \
	/**/ \
	/*	SUMMARY: 取得プロパティ系メソッド */ \
	/**/ \
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(CLASS_NAME, min, static_min); \
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(CLASS_NAME, max, static_max); \
		\
	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();

	//
	// RANDOM_ENGINE_CREATE_NATIVE_CLASS
	//
#define RANDOM_ENGINE_CREATE_NATIVE_CLASS(FUNCTION_NAME, CLASS_NAME, ENTRY_NAME) \
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(FUNCTION_NAME, CLASS_NAME, ENTRY_NAME) \
	/**/ \
	/*	SUMMARY: 内部系メソッド */ \
	/**/ \
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(CLASS_NAME, base, getBase); \
	/**/ \
	/*	SUMMARY: 取得系メソッド */ \
	/**/ \
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VARIANT(CLASS_NAME, random); \
	/**/ \
	/*	SUMMARY: シード系メソッド */ \
	/**/ \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(seed) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		this_->seed(numparams, param); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(seed); \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(randomize) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, CLASS_NAME); \
		this_->seed(numparams, param); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(randomize); \
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
	// CreateNativeClassMinstdRand0
	//
#define TJS_NATIVE_CLASSID_NAME MinstdRand0ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassMinstdRand0, RandomEngine<ktl::RandomUtils::minstd_rand0>, MinstdRand0);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassMinstdRand
	//
#define TJS_NATIVE_CLASSID_NAME MinstdRandClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassMinstdRand, RandomEngine<ktl::RandomUtils::minstd_rand>, MinstdRand);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassRand48
	//
#define TJS_NATIVE_CLASSID_NAME Rand48ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRand48, RandomEngine<ktl::RandomUtils::rand48>, Rand48);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassEcuyer1988
	//
#define TJS_NATIVE_CLASSID_NAME Ecuyer1988ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassEcuyer1988, RandomEngine<ktl::RandomUtils::ecuyer1988>, Ecuyer1988);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassKnuthB
	//
#define TJS_NATIVE_CLASSID_NAME KnuthBClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassKnuthB, RandomEngine<ktl::RandomUtils::knuth_b>, KnuthB);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassKreutzer1986
	//
#define TJS_NATIVE_CLASSID_NAME Kreutzer1986ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassKreutzer1986, RandomEngine<ktl::RandomUtils::kreutzer1986>, Kreutzer1986);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassTaus88
	//
#define TJS_NATIVE_CLASSID_NAME Taus88ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassTaus88, RandomEngine<ktl::RandomUtils::taus88>, Taus88);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassHellekalek1995
	//
#define TJS_NATIVE_CLASSID_NAME Hellekalek1995ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassHellekalek1995, RandomEngine<ktl::RandomUtils::hellekalek1995>, Hellekalek1995);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassMT11213b
	//
#define TJS_NATIVE_CLASSID_NAME MT11213bClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassMT11213b, RandomEngine<ktl::RandomUtils::mt11213b>, MT11213b);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassMT19937
	//
#define TJS_NATIVE_CLASSID_NAME MT19937ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassMT19937, RandomEngine<ktl::RandomUtils::mt19937>, MT19937);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassMT19937_64
	//
#define TJS_NATIVE_CLASSID_NAME MT19937_64ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassMT19937_64, RandomEngine<ktl::RandomUtils::mt19937_64>, MT19937_64);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassLaggedFibonacci607
	//
#define TJS_NATIVE_CLASSID_NAME LaggedFibonacci607ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassLaggedFibonacci607, RandomEngine<ktl::RandomUtils::lagged_fibonacci607>, LaggedFibonacci607);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassLaggedFibonacci1279
	//
#define TJS_NATIVE_CLASSID_NAME LaggedFibonacci1279ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassLaggedFibonacci1279, RandomEngine<ktl::RandomUtils::lagged_fibonacci1279>, LaggedFibonacci1279);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassLaggedFibonacci2281
	//
#define TJS_NATIVE_CLASSID_NAME LaggedFibonacci2281ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassLaggedFibonacci2281, RandomEngine<ktl::RandomUtils::lagged_fibonacci2281>, LaggedFibonacci2281);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassLaggedFibonacci3217
	//
#define TJS_NATIVE_CLASSID_NAME LaggedFibonacci3217ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassLaggedFibonacci3217, RandomEngine<ktl::RandomUtils::lagged_fibonacci3217>, LaggedFibonacci3217);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassLaggedFibonacci4423
	//
#define TJS_NATIVE_CLASSID_NAME LaggedFibonacci4423ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassLaggedFibonacci4423, RandomEngine<ktl::RandomUtils::lagged_fibonacci4423>, LaggedFibonacci4423);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassLaggedFibonacci9689
	//
#define TJS_NATIVE_CLASSID_NAME LaggedFibonacci9689ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassLaggedFibonacci9689, RandomEngine<ktl::RandomUtils::lagged_fibonacci9689>, LaggedFibonacci9689);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassLaggedFibonacci9937
	//
#define TJS_NATIVE_CLASSID_NAME LaggedFibonacci9937ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassLaggedFibonacci9937, RandomEngine<ktl::RandomUtils::lagged_fibonacci19937>, LaggedFibonacci9937);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassLaggedFibonacci23209
	//
#define TJS_NATIVE_CLASSID_NAME LaggedFibonacci23209ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassLaggedFibonacci23209, RandomEngine<ktl::RandomUtils::lagged_fibonacci23209>, LaggedFibonacci23209);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassLaggedFibonacci44497
	//
#define TJS_NATIVE_CLASSID_NAME LaggedFibonacci44497ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassLaggedFibonacci44497, RandomEngine<ktl::RandomUtils::lagged_fibonacci44497>, LaggedFibonacci44497);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassRanlux3
	//
#define TJS_NATIVE_CLASSID_NAME Ranlux3ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRanlux3, RandomEngine<ktl::RandomUtils::ranlux3>, Ranlux3);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassRanlux4
	//
#define TJS_NATIVE_CLASSID_NAME Ranlux4ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRanlux4, RandomEngine<ktl::RandomUtils::ranlux4>, Ranlux4);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassRanlux64_3
	//
#define TJS_NATIVE_CLASSID_NAME Ranlux64_3ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRanlux64_3, RandomEngine<ktl::RandomUtils::ranlux64_3>, Ranlux64_3);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassRanlux64_4
	//
#define TJS_NATIVE_CLASSID_NAME Ranlux64_4ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRanlux64_4, RandomEngine<ktl::RandomUtils::ranlux64_4>, Ranlux64_4);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassRanlux3_01
	//
#define TJS_NATIVE_CLASSID_NAME Ranlux3_01ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRanlux3_01, RandomEngine<ktl::RandomUtils::ranlux3_01>, Ranlux3_01);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassRanlux4_01
	//
#define TJS_NATIVE_CLASSID_NAME Ranlux4_01ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRanlux4_01, RandomEngine<ktl::RandomUtils::ranlux4_01>, Ranlux4_01);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassRanlux64_3_01
	//
#define TJS_NATIVE_CLASSID_NAME Ranlux64_3_01ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRanlux64_3_01, RandomEngine<ktl::RandomUtils::ranlux64_3_01>, Ranlux64_3_01);
#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassRanlux64_4_01
	//
#define TJS_NATIVE_CLASSID_NAME Ranlux64_4_01ClassID
	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRanlux64_4_01, RandomEngine<ktl::RandomUtils::ranlux64_4_01>, Ranlux64_4_01);
#undef TJS_NATIVE_CLASSID_NAME

//	CHECK: fatal error C1060
//	//
//	// CreateNativeClassRanlux24
//	//
//#define TJS_NATIVE_CLASSID_NAME Ranlux24ClassID
//	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRanlux24, RandomEngine<ktl::RandomUtils::ranlux24>, Ranlux24);
//#undef TJS_NATIVE_CLASSID_NAME
//
//	//
//	// CreateNativeClassRanlux48
//	//
//#define TJS_NATIVE_CLASSID_NAME Ranlux48ClassID
//	RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRanlux48, RandomEngine<ktl::RandomUtils::ranlux48>, Ranlux48);
//#undef TJS_NATIVE_CLASSID_NAME

	//
	// CreateNativeClassRandomDevice
	//
#define TJS_NATIVE_CLASSID_NAME RandomDeviceClassID
	STATIC_RANDOM_ENGINE_CREATE_NATIVE_CLASS(CreateNativeClassRandomDevice, RandomEngine<ktl::RandomUtils::random_device>, RandomDevice);
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_random_engine
// unregist_ktl_random_engine
//
static void regist_ktl_random_engine() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:RandomEngine:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("MinstdRand0"), ktl::CreateNativeClassMinstdRand0());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("MinstdRand"), ktl::CreateNativeClassMinstdRand());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Rand48"), ktl::CreateNativeClassRand48());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ecuyer1988"), ktl::CreateNativeClassEcuyer1988());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("KnuthB"), ktl::CreateNativeClassKnuthB());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Kreutzer1986"), ktl::CreateNativeClassKreutzer1986());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Taus88"), ktl::CreateNativeClassTaus88());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Hellekalek1995"), ktl::CreateNativeClassHellekalek1995());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("MT11213b"), ktl::CreateNativeClassMT11213b());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("MT19937"), ktl::CreateNativeClassMT19937());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("MT19937_64"), ktl::CreateNativeClassMT19937_64());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci607"), ktl::CreateNativeClassLaggedFibonacci607());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci1279"), ktl::CreateNativeClassLaggedFibonacci1279());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci2281"), ktl::CreateNativeClassLaggedFibonacci2281());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci3217"), ktl::CreateNativeClassLaggedFibonacci3217());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci4423"), ktl::CreateNativeClassLaggedFibonacci4423());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci9689"), ktl::CreateNativeClassLaggedFibonacci9689());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci9937"), ktl::CreateNativeClassLaggedFibonacci9937());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci23209"), ktl::CreateNativeClassLaggedFibonacci23209());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci44497"), ktl::CreateNativeClassLaggedFibonacci44497());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux3"), ktl::CreateNativeClassRanlux3());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux4"), ktl::CreateNativeClassRanlux4());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux64_3"), ktl::CreateNativeClassRanlux64_3());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux64_4"), ktl::CreateNativeClassRanlux64_4());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux3_01"), ktl::CreateNativeClassRanlux3_01());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux4_01"), ktl::CreateNativeClassRanlux4_01());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux64_3_01"), ktl::CreateNativeClassRanlux64_3_01());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux64_4_01"), ktl::CreateNativeClassRanlux64_4_01());
		//	CHECK: fatal error C1060
		//sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux24"), ktl::CreateNativeClassRanlux24());
		//sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux48"), ktl::CreateNativeClassRanlux48());
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("RandomDevice"), ktl::CreateNativeClassRandomDevice());
	}
}
static void unregist_ktl_random_engine() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:RandomEngine:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("MinstdRand0"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("MinstdRand"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Rand48"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ecuyer1988"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("KnuthB"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Kreutzer1986"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Taus88"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Hellekalek1995"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("MT11213b"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("MT19937"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("MT19937_64"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci607"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci1279"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci2281"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci3217"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci4423"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci9689"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci9937"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci23209"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LaggedFibonacci44497"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux3"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux4"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux64_3"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux64_4"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux3_01"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux4_01"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux64_3_01"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux64_4_01"));
		//	CHECK: fatal error C1060
		//sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux24"));
		//sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Ranlux48"));
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("RandomDevice"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_random_engine);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_random_engine);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:RandomEngine:information"));
