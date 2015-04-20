/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_MATH_EX_MATH_EX_DECL_HPP
#define SRC_MATH_EX_MATH_EX_DECL_HPP

#include <functional>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/array.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeMathEx
	//
	class NativeMathEx {
	private:
		//
		// RealCompare
		//
		class RealCompare
			: public std::binary_function<tTJSVariant const*, tTJSVariant const*, bool>
		{
		public:
			bool operator()(tTJSVariant const* lhs, tTJSVariant const* rhs) const;
		};
	private:
		static bool seedTime();
	public:
		NativeMathEx();
	public:
		//
		//	SUMMARY: 数値系メソッド
		//
		static tTVReal bernoulliB2n(int i);
		static tTVReal tangentT2n(int i);
		static boost::uint32_t prime(unsigned n);
		//
		//	SUMMARY: 算術系メソッド
		//
		static tTVReal abs(tTVReal x);
		static tTVReal ceil(tTVReal x);
		static tTVReal floor(tTVReal x);
		static tTVReal sqrt(tTVReal x);
		static tTVReal cbrt(tTVReal x);
		static tTVReal sqrt1Pm1(tTVReal x);
		static tTVReal hypot(tTVReal x, tTVReal y);
		//
		//	SUMMARY: 浮動小数点数系メソッド
		//
		static tTVReal round(tTVReal x);
		static tTVReal trunc(tTVReal x);
		static tTJSVariant modf(tTVReal x);
		static tTVReal fpart(tTVReal x);
		static tTVReal ldexp(tTVReal x, int exponent);
		static tTJSVariant frexp(tTVReal x);
		//
		//	SUMMARY: 浮動小数点数比較系メソッド
		//
		static bool equals(tTVReal a, tTVReal b);
		static bool notEquals(tTVReal a, tTVReal b);
		static bool less(tTVReal a, tTVReal b);
		static bool greater(tTVReal a, tTVReal b);
		static bool lessEqual(tTVReal a, tTVReal b);
		static bool greaterEqual(tTVReal a, tTVReal b);
		static int compare(tTVReal a, tTVReal b);
		//
		//	SUMMARY: 三角関数系メソッド
		//
		static tTVReal sin(tTVReal x);
		static tTVReal cos(tTVReal x);
		static tTVReal tan(tTVReal x);
		static tTVReal asin(tTVReal x);
		static tTVReal acos(tTVReal x);
		static tTVReal atan(tTVReal x);
		static tTVReal atan2(tTVReal y, tTVReal x);
		//
		//	SUMMARY: 双曲線関数系メソッド
		//
		static tTVReal sinh(tTVReal x);
		static tTVReal cosh(tTVReal x);
		static tTVReal tanh(tTVReal x);
		static tTVReal asinh(tTVReal x);
		static tTVReal acosh(tTVReal x);
		static tTVReal atanh(tTVReal x);
		//
		//	SUMMARY: 指数対数系メソッド
		//
		static tTVReal pow(tTVReal base, tTVReal exponent);
		static tTVReal pow10(tTVReal x);
		static tTVReal pow2(tTVReal x);
		static tTVReal exp(tTVReal x);
		static tTVReal log(tTVReal x);
		static tTVReal log10(tTVReal x);
		static tTVReal log2(tTVReal x);
		static tTVReal logN(tTVReal base, tTVReal x);
		static tTVReal powm1(tTVReal x, tTVReal y);
		static tTVReal expm1(tTVReal x);
		static tTVReal log1P(tTVReal x);
		//
		//	SUMMARY: ガンマ関数系メソッド
		//
		static tTVReal tgamma(tTVReal x);
		static tTVReal tgamma1Pm1(tTVReal x);
		static tTVReal lgamma(tTVReal x);
		static tTVReal digamma(tTVReal x);
		static tTVReal trigamma(tTVReal x);
		static tTVReal polygamma(int n, tTVReal x);
		static tTVReal tgammaRatio(tTVReal x, tTVReal y);
		static tTVReal tgammaDeltaRatio(tTVReal x, tTVReal delta);
		static tTVReal gammaP(tTVReal a, tTVReal x);
		static tTVReal gammaQ(tTVReal a, tTVReal x);
		static tTVReal tgammaLower(tTVReal a, tTVReal x);
		static tTVReal tgamma(tTVReal a, tTVReal x);
		static tTVReal gammaPInv(tTVReal a, tTVReal p);
		static tTVReal gammaQInv(tTVReal a, tTVReal q);
		static tTVReal gammaPInva(tTVReal a, tTVReal p);
		static tTVReal gammaQInva(tTVReal a, tTVReal q);
		static tTVReal gammaPDerivative(tTVReal a, tTVReal x);
		//
		//	SUMMARY: ベータ関数系メソッド
		//
		static tTVReal beta(tTVReal a, tTVReal b);
		static tTVReal ibeta(tTVReal a, tTVReal b, tTVReal x);
		static tTVReal ibetac(tTVReal a, tTVReal b, tTVReal x);
		static tTVReal beta(tTVReal a, tTVReal b, tTVReal x);
		static tTVReal betac(tTVReal a, tTVReal b, tTVReal x);
		static tTVReal ibetaInv(tTVReal a, tTVReal b, tTVReal p);
		static tTVReal ibetacInv(tTVReal a, tTVReal b, tTVReal q);
		static tTVReal ibetaInva(tTVReal a, tTVReal b, tTVReal p);
		static tTVReal ibetacInva(tTVReal a, tTVReal b, tTVReal q);
		static tTVReal ibetaInvb(tTVReal a, tTVReal b, tTVReal p);
		static tTVReal ibetacInvb(tTVReal a, tTVReal b, tTVReal q);
		static tTVReal ibetaDerivative(tTVReal a, tTVReal b, tTVReal x);
		//
		//	SUMMARY:誤差関数系メソッド
		//
		static tTVReal erf(tTVReal x);
		static tTVReal erfc(tTVReal x);
		static tTVReal erfInv(tTVReal p);
		static tTVReal erfcInv(tTVReal p);
		//
		//	SUMMARY:ルジャンドル多項式系メソッド
		//
		static tTVReal legendreP(int l, tTVReal x);
		static tTVReal legendreP(int l, int m, tTVReal x);
		static tTVReal legendreQ(unsigned l, tTVReal x);
		static tTVReal legendreNext(unsigned l, tTVReal x, tTVReal pl, tTVReal plm1);
		static tTVReal legendreNext(unsigned l, unsigned m, tTVReal x, tTVReal pl, tTVReal plm1);
		static tTVReal laguerre(int n, tTVReal x);
		static tTVReal laguerre(int n, int m, tTVReal x);
		static tTVReal laguerreNext(unsigned n, tTVReal x, tTVReal ln, tTVReal lnm1);
		static tTVReal laguerreNext(unsigned n, unsigned l, tTVReal x, tTVReal ln, tTVReal lnm1);
		static tTVReal hermite(unsigned n, tTVReal x);
		static tTVReal hermiteNext(unsigned n, tTVReal x, tTVReal hn, tTVReal hnm1);
		static tTJSVariant sphericalHarmonic(unsigned n, int m, tTVReal theta, tTVReal phi);
		static tTVReal sphericalHarmonicR(unsigned n, int m, tTVReal theta, tTVReal phi);
		static tTVReal sphericalHarmonicI(unsigned n, int m, tTVReal theta, tTVReal phi);
		//
		//	SUMMARY: ベッセル関数系メソッド
		//
		static tTVReal cylBesselJ(tTVReal v, tTVReal x);
		static tTVReal cylNeumann(tTVReal v, tTVReal x);
		static tTVReal cylBesselJZero(tTVReal v, int m);
		static tTVReal cylNeumannZero(tTVReal v, int m);
		static tTVReal cylBesselI(tTVReal v, tTVReal x);
		static tTVReal cylBesselK(tTVReal v, tTVReal x);
		static tTVReal sphBessel(unsigned v, tTVReal x);
		static tTVReal sphNeumann(unsigned v, tTVReal x);
		static tTVReal cylBesselJPrime(tTVReal v, tTVReal x);
		static tTVReal cylNeumannPrime(tTVReal v, tTVReal x);
		static tTVReal cylBesselIPrime(tTVReal v, tTVReal x);
		static tTVReal cylBesselKPrime(tTVReal v, tTVReal x);
		static tTVReal sphBesselPrime(unsigned v, tTVReal x);
		static tTVReal sphNeumannPrime(unsigned v, tTVReal x);
		//
		//	SUMMARY: ハンケル関数系メソッド
		//
		static std::complex<tTVReal> cylHankel1(tTVReal v, tTVReal x);
		static std::complex<tTVReal> cylHankel2(tTVReal v, tTVReal x);
		static std::complex<tTVReal> sphHankel1(tTVReal v, tTVReal x);
		static std::complex<tTVReal> sphHankel2(tTVReal v, tTVReal x);
		//
		//	SUMMARY: エアリー関数系メソッド
		//
		static tTVReal airyAi(tTVReal x);
		static tTVReal airyBi(tTVReal x);
		static tTVReal airyAiPrime(tTVReal x);
		static tTVReal airyBiPrime(tTVReal x);
		//
		//	SUMMARY: ゼータ関数系メソッド
		//
		static tTVReal zeta(tTVReal z);
		//
		//	SUMMARY: シンク関数系メソッド
		//
		static tTVReal sincPi(tTVReal x);
		static tTVReal sinhcPi(tTVReal x);
		//
		//	SUMMARY: オーウェンのT関数系メソッド
		//
		static tTVReal owensT(tTVReal h, tTVReal a);
		//
		//	SUMMARY: 楕円積分系メソッド
		//
		static tTVReal ellintRf(tTVReal x, tTVReal y, tTVReal z);
		static tTVReal ellintRd(tTVReal x, tTVReal y, tTVReal z);
		static tTVReal ellintRj(tTVReal x, tTVReal y, tTVReal z, tTVReal p);
		static tTVReal ellintRc(tTVReal x, tTVReal y);
		static tTVReal ellint1(tTVReal k, tTVReal phi);
		static tTVReal ellint1(tTVReal k);
		static tTVReal ellint2(tTVReal k, tTVReal phi);
		static tTVReal ellint2(tTVReal k);
		static tTVReal ellint3(tTVReal k, tTVReal n, tTVReal phi);
		static tTVReal ellint3(tTVReal k, tTVReal n);
		static tTVReal ellintD(tTVReal k, tTVReal phi);
		static tTVReal ellintD(tTVReal k);
		static tTVReal jacobiZeta(tTVReal k, tTVReal phi);
		static tTVReal heumanLambda(tTVReal k, tTVReal phi);
		//
		//	SUMMARY: ヤコビの楕円関数系メソッド
		//
		static boost::array<tTVReal, 3> jacobiElliptic(tTVReal k, tTVReal u);
		static tTVReal jacobiCd(tTVReal k, tTVReal u);
		static tTVReal jacobiCn(tTVReal k, tTVReal u);
		static tTVReal jacobiCs(tTVReal k, tTVReal u);
		static tTVReal jacobiDc(tTVReal k, tTVReal u);
		static tTVReal jacobiDn(tTVReal k, tTVReal u);
		static tTVReal jacobiDs(tTVReal k, tTVReal u);
		static tTVReal jacobiNc(tTVReal k, tTVReal u);
		static tTVReal jacobiNd(tTVReal k, tTVReal u);
		static tTVReal jacobiNs(tTVReal k, tTVReal u);
		static tTVReal jacobiSc(tTVReal k, tTVReal u);
		static tTVReal jacobiSd(tTVReal k, tTVReal u);
		static tTVReal jacobiSn(tTVReal k, tTVReal u);
		//
		//	SUMMARY: 指数積分系メソッド
		//
		static tTVReal expint(unsigned n, tTVReal z);
		static tTVReal expint(tTVReal z);
		//
		//	SUMMARY: 階乗系メソッド
		//
		static tTVReal factorial(unsigned i);
		static tTVReal doubleFactorial(unsigned i);
		static tTVReal risingFactorial(tTVReal x, int n);
		static tTVReal fallingFactorial(tTVReal x, unsigned i);
		static tTVReal binomialCoefficient(unsigned n, unsigned k);
		//
		//	SUMMARY: 公倍公約系メソッド
		//
		static tTVInteger gcd(tTVInteger a, tTVInteger b);
		static tTVInteger lcm(tTVInteger a, tTVInteger b);
		//
		//	SUMMARY: 比較系メソッド
		//
		static tTVReal min(tjs_int numparams, tTJSVariant** param);
		static tTVReal max(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: ランダム系メソッド
		//
		static tTVReal random();
		static tTVReal randomDevice();
		//
		//	SUMMARY: 定数系プロパティ
		//
		static tTVReal pi();
		static tTVReal e();
		static tTVReal ln10();
		static tTVReal ln2();
		static tTVReal log10e();
		static tTVReal log2e();
		static tTVReal sqrt2();
		static tTVReal sqrt1_2();
	};

	//
	// MathEx
	//
	class MathEx
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeMathEx> instance_;
	public:
		MathEx();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: 数値系メソッド
		//
		static tTVReal bernoulliB2n(tTVInteger i);
		static tTVReal tangentT2n(tTVInteger i);
		static tTVInteger prime(tTVInteger n);
		//
		//	SUMMARY: 算術系メソッド
		//
		static tTVReal abs(tTVReal x);
		static tTVReal ceil(tTVReal x);
		static tTVReal floor(tTVReal x);
		static tTVReal sqrt(tTVReal x);
		static tTVReal cbrt(tTVReal x);
		static tTVReal sqrt1Pm1(tTVReal x);
		static tTVReal hypot(tTVReal x, tTVReal y);
		//
		//	SUMMARY: 浮動小数点数系メソッド
		//
		static tTVReal round(tTVReal x);
		static tTVReal trunc(tTVReal x);
		static tTJSVariant modf(tTVReal x);
		static tTVReal fpart(tTVReal x);
		static tTVReal ldexp(tTVReal x, tTVInteger exponent);
		static tTJSVariant frexp(tTVReal x);
		//
		//	SUMMARY: 浮動小数点数比較系メソッド
		//
		static bool equals(tTVReal a, tTVReal b);
		static bool notEquals(tTVReal a, tTVReal b);
		static bool less(tTVReal a, tTVReal b);
		static bool greater(tTVReal a, tTVReal b);
		static bool lessEqual(tTVReal a, tTVReal b);
		static bool greaterEqual(tTVReal a, tTVReal b);
		static tTVInteger compare(tTVReal a, tTVReal b);
		//
		//	SUMMARY: 三角関数系メソッド
		//
		static tTVReal sin(tTVReal x);
		static tTVReal cos(tTVReal x);
		static tTVReal tan(tTVReal x);
		static tTVReal asin(tTVReal x);
		static tTVReal acos(tTVReal x);
		static tTVReal atan(tTVReal x);
		static tTVReal atan2(tTVReal y, tTVReal x);
		//
		//	SUMMARY: 双曲線関数系メソッド
		//
		static tTVReal sinh(tTVReal x);
		static tTVReal cosh(tTVReal x);
		static tTVReal tanh(tTVReal x);
		static tTVReal asinh(tTVReal x);
		static tTVReal acosh(tTVReal x);
		static tTVReal atanh(tTVReal x);
		//
		//	SUMMARY: 指数対数系メソッド
		//
		static tTVReal pow(tTVReal base, tTVReal exponent);
		static tTVReal exp(tTVReal x);
		static tTVReal pow10(tTVReal x);
		static tTVReal pow2(tTVReal x);
		static tTVReal log(tTVReal x);
		static tTVReal log10(tTVReal x);
		static tTVReal log2(tTVReal x);
		static tTVReal logN(tTVReal base, tTVReal x);
		static tTVReal powm1(tTVReal x, tTVReal y);
		static tTVReal expm1(tTVReal x);
		static tTVReal log1P(tTVReal x);
		//
		//	SUMMARY: ガンマ関数系メソッド
		//
		static tTVReal tgamma(tTVReal x);
		static tTVReal tgamma1Pm1(tTVReal x);
		static tTVReal lgamma(tTVReal x);
		static tTVReal digamma(tTVReal x);
		static tTVReal trigamma(tTVReal x);
		static tTVReal polygamma(tTVInteger n, tTVReal x);
		static tTVReal tgammaRatio(tTVReal x, tTVReal y);
		static tTVReal tgammaDeltaRatio(tTVReal x, tTVReal delta);
		static tTVReal gammaP(tTVReal a, tTVReal x);
		static tTVReal gammaQ(tTVReal a, tTVReal x);
		static tTVReal tgammaLower(tTVReal a, tTVReal x);
		static tTVReal tgamma(tTVReal a, tTVReal x);
		static tTVReal gammaPInv(tTVReal a, tTVReal p);
		static tTVReal gammaQInv(tTVReal a, tTVReal q);
		static tTVReal gammaPInva(tTVReal a, tTVReal p);
		static tTVReal gammaQInva(tTVReal a, tTVReal q);
		static tTVReal gammaPDerivative(tTVReal a, tTVReal x);
		//
		//	SUMMARY: ベータ関数系メソッド
		//
		static tTVReal beta(tTVReal a, tTVReal b);
		static tTVReal ibeta(tTVReal a, tTVReal b, tTVReal x);
		static tTVReal ibetac(tTVReal a, tTVReal b, tTVReal x);
		static tTVReal beta(tTVReal a, tTVReal b, tTVReal x);
		static tTVReal betac(tTVReal a, tTVReal b, tTVReal x);
		static tTVReal ibetaInv(tTVReal a, tTVReal b, tTVReal p);
		static tTVReal ibetacInv(tTVReal a, tTVReal b, tTVReal q);
		static tTVReal ibetaInva(tTVReal a, tTVReal b, tTVReal p);
		static tTVReal ibetacInva(tTVReal a, tTVReal b, tTVReal q);
		static tTVReal ibetaInvb(tTVReal a, tTVReal b, tTVReal p);
		static tTVReal ibetacInvb(tTVReal a, tTVReal b, tTVReal q);
		static tTVReal ibetaDerivative(tTVReal a, tTVReal b, tTVReal x);
		//
		//	SUMMARY:誤差関数系メソッド
		//
		static tTVReal erf(tTVReal x);
		static tTVReal erfc(tTVReal x);
		static tTVReal erfInv(tTVReal p);
		static tTVReal erfcInv(tTVReal p);
		//
		//	SUMMARY:ルジャンドル多項式系メソッド
		//
		static tTVReal legendreP(tTVInteger l, tTVReal x);
		static tTVReal legendreP(tTVInteger l, tTVInteger m, tTVReal x);
		static tTVReal legendreQ(tTVInteger l, tTVReal x);
		static tTVReal legendreNext(tTVInteger l, tTVReal x, tTVReal pl, tTVReal plm1);
		static tTVReal legendreNext(tTVInteger l, tTVInteger m, tTVReal x, tTVReal pl, tTVReal plm1);
		static tTVReal laguerre(tTVInteger n, tTVReal x);
		static tTVReal laguerre(tTVInteger n, tTVInteger m, tTVReal x);
		static tTVReal laguerreNext(tTVInteger n, tTVReal x, tTVReal ln, tTVReal lnm1);
		static tTVReal laguerreNext(tTVInteger n, tTVInteger l, tTVReal x, tTVReal ln, tTVReal lnm1);
		static tTVReal hermite(tTVInteger n, tTVReal x);
		static tTVReal hermiteNext(tTVInteger n, tTVReal x, tTVReal hn, tTVReal hnm1);
		static tTJSVariant sphericalHarmonic(tTVInteger n, tTVInteger m, tTVReal theta, tTVReal phi);
		static tTVReal sphericalHarmonicR(tTVInteger n, tTVInteger m, tTVReal theta, tTVReal phi);
		static tTVReal sphericalHarmonicI(tTVInteger n, tTVInteger m, tTVReal theta, tTVReal phi);
		//
		//	SUMMARY: ベッセル関数系メソッド
		//
		static tTVReal cylBesselJ(tTVReal v, tTVReal x);
		static tTVReal cylNeumann(tTVReal v, tTVReal x);
		static tTVReal cylBesselJZero(tTVReal v, tTVInteger m);
		static tTVReal cylNeumannZero(tTVReal v, tTVInteger m);
		static tTVReal cylBesselI(tTVReal v, tTVReal x);
		static tTVReal cylBesselK(tTVReal v, tTVReal x);
		static tTVReal sphBessel(tTVInteger v, tTVReal x);
		static tTVReal sphNeumann(tTVInteger v, tTVReal x);
		static tTVReal cylBesselJPrime(tTVReal v, tTVReal x);
		static tTVReal cylNeumannPrime(tTVReal v, tTVReal x);
		static tTVReal cylBesselIPrime(tTVReal v, tTVReal x);
		static tTVReal cylBesselKPrime(tTVReal v, tTVReal x);
		static tTVReal sphBesselPrime(tTVInteger v, tTVReal x);
		static tTVReal sphNeumannPrime(tTVInteger v, tTVReal x);
		//
		//	SUMMARY: ハンケル関数系メソッド
		//
		static tTJSVariant cylHankel1(tTVReal v, tTVReal x);
		static tTJSVariant cylHankel2(tTVReal v, tTVReal x);
		static tTJSVariant sphHankel1(tTVReal v, tTVReal x);
		static tTJSVariant sphHankel2(tTVReal v, tTVReal x);
		//
		//	SUMMARY: エアリー関数系メソッド
		//
		static tTVReal airyAi(tTVReal x);
		static tTVReal airyBi(tTVReal x);
		static tTVReal airyAiPrime(tTVReal x);
		static tTVReal airyBiPrime(tTVReal x);
		//
		//	SUMMARY: ゼータ関数系メソッド
		//
		static tTVReal zeta(tTVReal z);
		//
		//	SUMMARY: シンク関数系メソッド
		//
		static tTVReal sincPi(tTVReal x);
		static tTVReal sinhcPi(tTVReal x);
		//
		//	SUMMARY: オーウェンのT関数系メソッド
		//
		static tTVReal owensT(tTVReal h, tTVReal a);
		//
		//	SUMMARY: 楕円積分系メソッド
		//
		static tTVReal ellintRf(tTVReal x, tTVReal y, tTVReal z);
		static tTVReal ellintRd(tTVReal x, tTVReal y, tTVReal z);
		static tTVReal ellintRj(tTVReal x, tTVReal y, tTVReal z, tTVReal p);
		static tTVReal ellintRc(tTVReal x, tTVReal y);
		static tTVReal ellint1(tTVReal k, tTVReal phi);
		static tTVReal ellint1(tTVReal k);
		static tTVReal ellint2(tTVReal k, tTVReal phi);
		static tTVReal ellint2(tTVReal k);
		static tTVReal ellint3(tTVReal k, tTVReal n, tTVReal phi);
		static tTVReal ellint3(tTVReal k, tTVReal n);
		static tTVReal ellintD(tTVReal k, tTVReal phi);
		static tTVReal ellintD(tTVReal k);
		static tTVReal jacobiZeta(tTVReal k, tTVReal phi);
		static tTVReal heumanLambda(tTVReal k, tTVReal phi);
		//
		//	SUMMARY: ヤコビの楕円関数系メソッド
		//
		static tTJSVariant jacobiElliptic(tTVReal k, tTVReal u);
		static tTVReal jacobiCd(tTVReal k, tTVReal u);
		static tTVReal jacobiCn(tTVReal k, tTVReal u);
		static tTVReal jacobiCs(tTVReal k, tTVReal u);
		static tTVReal jacobiDc(tTVReal k, tTVReal u);
		static tTVReal jacobiDn(tTVReal k, tTVReal u);
		static tTVReal jacobiDs(tTVReal k, tTVReal u);
		static tTVReal jacobiNc(tTVReal k, tTVReal u);
		static tTVReal jacobiNd(tTVReal k, tTVReal u);
		static tTVReal jacobiNs(tTVReal k, tTVReal u);
		static tTVReal jacobiSc(tTVReal k, tTVReal u);
		static tTVReal jacobiSd(tTVReal k, tTVReal u);
		static tTVReal jacobiSn(tTVReal k, tTVReal u);
		//
		//	SUMMARY: 指数積分系メソッド
		//
		static tTVReal expint(tTVInteger n, tTVReal z);
		static tTVReal expint(tTVReal z);
		//
		//	SUMMARY: 階乗系メソッド
		//
		static tTVReal factorial(tTVInteger i);
		static tTVReal doubleFactorial(tTVInteger i);
		static tTVReal risingFactorial(tTVReal x, tTVInteger n);
		static tTVReal fallingFactorial(tTVReal x, tTVInteger i);
		static tTVReal binomialCoefficient(tTVInteger n, tTVInteger k);
		//
		//	SUMMARY: 公倍公約系メソッド
		//
		static tTVInteger gcd(tTVInteger a, tTVInteger b);
		static tTVInteger lcm(tTVInteger a, tTVInteger b);
		//
		//	SUMMARY: 比較系メソッド
		//
		static tTVReal min(tjs_int numparams, tTJSVariant** param);
		static tTVReal max(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: ランダム系メソッド
		//
		static tTVReal random();
		static tTVReal randomDevice();
		//
		//	SUMMARY: 定数系プロパティ
		//
		static tTVReal pi();
		static tTVReal e();
		static tTVReal ln10();
		static tTVReal ln2();
		static tTVReal log10e();
		static tTVReal log2e();
		static tTVReal sqrt2();
		static tTVReal sqrt1_2();
	};
}	// namespace ktl

#endif	// #ifndef SRC_MATH_EX_MATH_EX_DECL_HPP
