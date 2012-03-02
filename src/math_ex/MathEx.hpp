#pragma once

#include <cmath>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/common_factor_rt.hpp>
#include <boost/math/special_functions/asinh.hpp>
#include <boost/math/special_functions/acosh.hpp>
#include <boost/math/special_functions/atanh.hpp>
#include <boost/math/special_functions/round.hpp>
#include <boost/math/special_functions/trunc.hpp>
#include <boost/math/special_functions/modf.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <boost/math/special_functions/digamma.hpp>
#include <boost/math/special_functions/beta.hpp>
#include <boost/math/special_functions/erf.hpp>
#include <boost/math/special_functions/legendre.hpp>
#include <boost/math/special_functions/laguerre.hpp>
#include <boost/math/special_functions/hermite.hpp>
#include <boost/math/special_functions/spherical_harmonic.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <boost/math/special_functions/zeta.hpp>
#include <boost/math/special_functions/sinc.hpp>
#include <boost/math/special_functions/sinhc.hpp>
#include <boost/math/special_functions/ellint_rf.hpp>
#include <boost/math/special_functions/ellint_rd.hpp>
#include <boost/math/special_functions/ellint_rj.hpp>
#include <boost/math/special_functions/ellint_rc.hpp>
#include <boost/math/special_functions/ellint_1.hpp>
#include <boost/math/special_functions/ellint_2.hpp>
#include <boost/math/special_functions/ellint_3.hpp>
#include <boost/math/special_functions/expint.hpp>
#include <boost/math/special_functions/factorials.hpp>
#include <boost/math/special_functions/binomial.hpp>
#include <boost/math/special_functions/cbrt.hpp>
#include <boost/math/special_functions/sqrt1pm1.hpp>
#include <boost/math/special_functions/hypot.hpp>
#include <boost/math/special_functions/powm1.hpp>
#include <boost/math/special_functions/expm1.hpp>
#include <boost/math/special_functions/log1P.hpp>
#include <boost/geometry/util/math.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/random/random_device.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "MathExDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:MathEx:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:MathEx:error")

namespace ktl {
	//
	// NativeMathEx
	//
	bool NativeMathEx::RealCompare::operator()(tTJSVariant const* lhs, tTJSVariant const* rhs) const {
		return lhs->AsReal() < rhs->AsReal();
	}
	KTL_INLINE bool NativeMathEx::seedTime() {
		std::time_t time;
		std::time(&time);
		if (time != std::time_t(-1)) {
			return false;
		}
		std::srand(static_cast<unsigned>(time));
		return true;
	}
	NativeMathEx::NativeMathEx() {}
	//
	//	SUMMARY: 算術系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::abs(tTVReal x) {
		return std::abs(x);
	}
	KTL_INLINE tTVReal NativeMathEx::ceil(tTVReal x) {
		return std::ceil(x);
	}
	KTL_INLINE tTVReal NativeMathEx::floor(tTVReal x) {
		return std::floor(x);
	}
	KTL_INLINE tTVReal NativeMathEx::sqrt(tTVReal x) {
		return std::sqrt(x);
	}
	KTL_INLINE tTVReal NativeMathEx::cbrt(tTVReal x) {
		return boost::math::cbrt(x);
	}
	KTL_INLINE tTVReal NativeMathEx::sqrt1Pm1(tTVReal x) {
		return boost::math::sqrt1pm1(x);
	}
	KTL_INLINE tTVReal NativeMathEx::hypot(tTVReal x, tTVReal y) {
		return boost::math::hypot(x, y);
	}
	//
	//	SUMMARY: 浮動小数点数系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::round(tTVReal x) {
		return boost::math::round(x);
	}
	KTL_INLINE tTVReal NativeMathEx::trunc(tTVReal x) {
		return boost::math::trunc(x);
	}
	KTL_INLINE tTJSVariant NativeMathEx::modf(tTVReal x) {
		tTVReal i_part;
		tTVReal f_part = boost::math::modf(x, &i_part);
		iTJSDispatch2* result_obj = 0;
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			&result_obj,
			0,
			0,
			0
			);
		sprig::krkr::tjs::object_type obj(result_obj, false);
		{
			tTJSVariant param = i_part;
			sprig::krkr::tjs::PropSetByNum(
				obj.get(),
				TJS_MEMBERENSURE,
				0,
				&param,
				obj.get()
				);
			param = f_part;
			sprig::krkr::tjs::PropSetByNum(
				obj.get(),
				TJS_MEMBERENSURE,
				1,
				&param,
				obj.get()
				);
		}
		return tTJSVariant(obj.get(), obj.get());
	}
	KTL_INLINE tTVReal NativeMathEx::fpart(tTVReal x) {
		return x - boost::math::trunc(x);
	}
	KTL_INLINE tTVReal NativeMathEx::ldexp(tTVReal x, int exponent) {
		return std::ldexp(x, exponent);
	}
	KTL_INLINE tTJSVariant NativeMathEx::frexp(tTVReal x) {
		int exponent;
		tTVReal mantissa = std::frexp(x, &exponent);
		iTJSDispatch2* result_obj = 0;
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			&result_obj,
			0,
			0,
			0
			);
		sprig::krkr::tjs::object_type obj(result_obj, false);
		{
			tTJSVariant param = mantissa;
			sprig::krkr::tjs::PropSetByNum(
				obj.get(),
				TJS_MEMBERENSURE,
				0,
				&param,
				obj.get()
				);
			param = exponent;
			sprig::krkr::tjs::PropSetByNum(
				obj.get(),
				TJS_MEMBERENSURE,
				1,
				&param,
				obj.get()
				);
		}
		return tTJSVariant(obj.get(), obj.get());
	}
	//
	//	SUMMARY: 浮動小数点数比較系メソッド
	//
	KTL_INLINE bool NativeMathEx::equals(tTVReal a, tTVReal b) {
		return boost::geometry::math::equals(a, b);
	}
	KTL_INLINE bool NativeMathEx::notEquals(tTVReal a, tTVReal b) {
		return !boost::geometry::math::equals(a, b);
	}
	KTL_INLINE bool NativeMathEx::less(tTVReal a, tTVReal b) {
		return !boost::geometry::math::equals(a, b) && a < b;
	}
	KTL_INLINE bool NativeMathEx::greater(tTVReal a, tTVReal b) {
		return !boost::geometry::math::equals(a, b) && a > b;
	}
	KTL_INLINE bool NativeMathEx::lessEqual(tTVReal a, tTVReal b) {
		return boost::geometry::math::equals(a, b) || a < b;
	}
	KTL_INLINE bool NativeMathEx::greaterEqual(tTVReal a, tTVReal b) {
		return boost::geometry::math::equals(a, b) || a > b;
	}
	KTL_INLINE int NativeMathEx::compare(tTVReal a, tTVReal b) {
		return boost::geometry::math::equals(a, b)
			? 0
			: a > b
				? 1
				: -1
			;
	}
	//
	//	SUMMARY: 三角関数系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::sin(tTVReal x) {
		return std::sin(x);
	}
	KTL_INLINE tTVReal NativeMathEx::cos(tTVReal x) {
		return std::cos(x);
	}
	KTL_INLINE tTVReal NativeMathEx::tan(tTVReal x) {
		return std::tan(x);
	}
	KTL_INLINE tTVReal NativeMathEx::asin(tTVReal x) {
		return std::asin(x);
	}
	KTL_INLINE tTVReal NativeMathEx::acos(tTVReal x) {
		return std::acos(x);
	}
	KTL_INLINE tTVReal NativeMathEx::atan(tTVReal x) {
		return std::atan(x);
	}
	KTL_INLINE tTVReal NativeMathEx::atan2(tTVReal y, tTVReal x) {
		return std::atan2(y, x);
	};
	//
	//	SUMMARY: 双曲線関数系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::sinh(tTVReal x) {
		return std::sinh(x);
	}
	KTL_INLINE tTVReal NativeMathEx::cosh(tTVReal x) {
		return std::cosh(x);
	}
	KTL_INLINE tTVReal NativeMathEx::tanh(tTVReal x) {
		return std::tanh(x);
	}
	KTL_INLINE tTVReal NativeMathEx::asinh(tTVReal x) {
		return boost::math::asinh(x);
	}
	KTL_INLINE tTVReal NativeMathEx::acosh(tTVReal x) {
		return boost::math::acosh(x);
	}
	KTL_INLINE tTVReal NativeMathEx::atanh(tTVReal x) {
		return boost::math::atanh(x);
	}
	//
	//	SUMMARY: 指数対数系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::pow(tTVReal base, tTVReal exponent) {
		return std::pow(base, exponent);
	}
	KTL_INLINE tTVReal NativeMathEx::exp(tTVReal x) {
		return std::exp(x);
	};
	KTL_INLINE tTVReal NativeMathEx::pow10(tTVReal x) {
		return std::pow(10.0, x);
	}
	KTL_INLINE tTVReal NativeMathEx::pow2(tTVReal x) {
		return std::pow(2.0, x);
	}
	KTL_INLINE tTVReal NativeMathEx::log(tTVReal x) {
		return std::log(x);
	};
	KTL_INLINE tTVReal NativeMathEx::log10(tTVReal x) {
		return std::log10(x);
	};
	KTL_INLINE tTVReal NativeMathEx::log2(tTVReal x) {
		return std::log(x) / boost::math::constants::ln_two<tTVReal>();
	};
	KTL_INLINE tTVReal NativeMathEx::logN(tTVReal base, tTVReal x) {
		return std::log(x) / std::log(base);
	};
	KTL_INLINE tTVReal NativeMathEx::powm1(tTVReal x, tTVReal y) {
		return boost::math::powm1(x, y);
	}
	KTL_INLINE tTVReal NativeMathEx::expm1(tTVReal x) {
		return boost::math::expm1(x);
	}
	KTL_INLINE tTVReal NativeMathEx::log1P(tTVReal x) {
		return boost::math::log1p(x);
	}
	//
	//	SUMMARY: ガンマ関数系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::tgamma(tTVReal x) {
		return boost::math::tgamma(x);
	}
	KTL_INLINE tTVReal NativeMathEx::tgamma1Pm1(tTVReal x) {
		return boost::math::tgamma1pm1(x);
	}
	KTL_INLINE tTVReal NativeMathEx::lgamma(tTVReal x) {
		return boost::math::lgamma(x);
	}
	KTL_INLINE tTVReal NativeMathEx::digamma(tTVReal x) {
		return boost::math::digamma(x);
	}
	KTL_INLINE tTVReal NativeMathEx::tgammaRatio(tTVReal x, tTVReal y) {
		return boost::math::tgamma_ratio(x, y);
	}
	KTL_INLINE tTVReal NativeMathEx::tgammaDeltaRatio(tTVReal x, tTVReal delta) {
		return boost::math::tgamma_delta_ratio(x, delta);
	}
	KTL_INLINE tTVReal NativeMathEx::gammaP(tTVReal a, tTVReal x) {
		return boost::math::gamma_p(a, x);
	}
	KTL_INLINE tTVReal NativeMathEx::gammaQ(tTVReal a, tTVReal x) {
		return boost::math::gamma_q(a, x);
	}
	KTL_INLINE tTVReal NativeMathEx::tgammaLower(tTVReal a, tTVReal x) {
		return boost::math::tgamma_lower(a, x);
	}
	KTL_INLINE tTVReal NativeMathEx::tgamma(tTVReal a, tTVReal x) {
		return boost::math::tgamma(a, x);
	}
	KTL_INLINE tTVReal NativeMathEx::gammaPInv(tTVReal a, tTVReal p) {
		return boost::math::gamma_p_inv(a, p);
	}
	KTL_INLINE tTVReal NativeMathEx::gammaQInv(tTVReal a, tTVReal q) {
		return boost::math::gamma_q_inv(a, q);
	}
	KTL_INLINE tTVReal NativeMathEx::gammaPInva(tTVReal a, tTVReal p) {
		return boost::math::gamma_p_inva(a, p);
	}
	KTL_INLINE tTVReal NativeMathEx::gammaQInva(tTVReal a, tTVReal q) {
		return boost::math::gamma_q_inva(a, q);
	}
	KTL_INLINE tTVReal NativeMathEx::gammaPDerivative(tTVReal a, tTVReal x) {
		return boost::math::gamma_p_derivative(a, x);
	}
	//
	//	SUMMARY: ベータ関数系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::beta(tTVReal a, tTVReal b) {
		return boost::math::beta(a, b);
	}
	KTL_INLINE tTVReal NativeMathEx::ibeta(tTVReal a, tTVReal b, tTVReal x) {
		return boost::math::ibeta(a, b, x);
	}
	KTL_INLINE tTVReal NativeMathEx::ibetac(tTVReal a, tTVReal b, tTVReal x) {
		return boost::math::ibetac(a, b, x);
	}
	KTL_INLINE tTVReal NativeMathEx::beta(tTVReal a, tTVReal b, tTVReal x) {
		return boost::math::beta(a, b, x);
	}
	KTL_INLINE tTVReal NativeMathEx::betac(tTVReal a, tTVReal b, tTVReal x) {
		return boost::math::betac(a, b, x);
	}
	KTL_INLINE tTVReal NativeMathEx::ibetaInv(tTVReal a, tTVReal b, tTVReal p) {
		return boost::math::ibeta_inv(a, b, p);
	}
	KTL_INLINE tTVReal NativeMathEx::ibetacInv(tTVReal a, tTVReal b, tTVReal q) {
		return boost::math::ibetac_inv(a, b, q);
	}
	KTL_INLINE tTVReal NativeMathEx::ibetaInva(tTVReal a, tTVReal b, tTVReal p) {
		return boost::math::ibeta_inva(a, b, p);
	}
	KTL_INLINE tTVReal NativeMathEx::ibetacInva(tTVReal a, tTVReal b, tTVReal q) {
		return boost::math::ibetac_inva(a, b, q);
	}
	KTL_INLINE tTVReal NativeMathEx::ibetaInvb(tTVReal a, tTVReal b, tTVReal p) {
		return boost::math::ibeta_invb(a, b, p);
	}
	KTL_INLINE tTVReal NativeMathEx::ibetacInvb(tTVReal a, tTVReal b, tTVReal q) {
		return boost::math::ibetac_invb(a, b, q);
	}
	KTL_INLINE tTVReal NativeMathEx::ibetaDerivative(tTVReal a, tTVReal b, tTVReal x) {
		return boost::math::ibeta_derivative(a, b, x);
	}
	//
	//	SUMMARY:誤差関数系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::erf(tTVReal x) {
		return boost::math::erf(x);
	}
	KTL_INLINE tTVReal NativeMathEx::erfc(tTVReal x) {
		return boost::math::erfc(x);
	}
	KTL_INLINE tTVReal NativeMathEx::erfInv(tTVReal p) {
		return boost::math::erf_inv(p);
	}
	KTL_INLINE tTVReal NativeMathEx::erfcInv(tTVReal p) {
		return boost::math::erfc_inv(p);
	}
	//
	//	SUMMARY:ルジャンドル多項式系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::legendreP(int l, tTVReal x) {
		return boost::math::legendre_p(l, x);
	}
	KTL_INLINE tTVReal NativeMathEx::legendreP(int l, int m, tTVReal x) {
		return boost::math::legendre_p(l, m, x);
	}
	KTL_INLINE tTVReal NativeMathEx::legendreQ(unsigned l, tTVReal x) {
		return boost::math::legendre_q(l, x);
	}
	KTL_INLINE tTVReal NativeMathEx::legendreNext(unsigned l, tTVReal x, tTVReal pl, tTVReal plm1) {
		return boost::math::legendre_next(l, x, pl, plm1);
	}
	KTL_INLINE tTVReal NativeMathEx::legendreNext(unsigned l, unsigned m, tTVReal x, tTVReal pl, tTVReal plm1) {
		return boost::math::legendre_next(l, m, x, pl, plm1);
	}
	KTL_INLINE tTVReal NativeMathEx::laguerre(int n, tTVReal x) {
		return boost::math::laguerre(n, x);
	}
	KTL_INLINE tTVReal NativeMathEx::laguerre(int n, int m, tTVReal x) {
		return boost::math::laguerre(n, m, x);
	}
	KTL_INLINE tTVReal NativeMathEx::laguerreNext(unsigned n, tTVReal x, tTVReal ln, tTVReal lnm1) {
		return boost::math::laguerre_next(n, x, ln, lnm1);
	}
	KTL_INLINE tTVReal NativeMathEx::laguerreNext(unsigned n, unsigned l, tTVReal x, tTVReal ln, tTVReal lnm1) {
		return boost::math::laguerre_next(n, l, x, ln, lnm1);
	}
	KTL_INLINE tTVReal NativeMathEx::hermite(unsigned n, tTVReal x) {
		return boost::math::hermite(n, x);
	}
	KTL_INLINE tTVReal NativeMathEx::hermiteNext(unsigned n, tTVReal x, tTVReal hn, tTVReal hnm1) {
		return boost::math::hermite_next(n, x, hn, hnm1);
	}
	KTL_INLINE tTJSVariant NativeMathEx::sphericalHarmonic(unsigned n, int m, tTVReal theta, tTVReal phi) {
		std::complex<tTVReal> result(boost::math::spherical_harmonic(n, m, theta, phi));
		iTJSDispatch2* result_obj = 0;
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			&result_obj,
			0,
			0,
			0
			);
		sprig::krkr::tjs::object_type obj(result_obj, false);
		{
			tTJSVariant param = result.real();
			sprig::krkr::tjs::PropSetByNum(
				obj.get(),
				TJS_MEMBERENSURE,
				0,
				&param,
				obj.get()
				);
			param = result.imag();
			sprig::krkr::tjs::PropSetByNum(
				obj.get(),
				TJS_MEMBERENSURE,
				1,
				&param,
				obj.get()
				);
		}
		return tTJSVariant(obj.get(), obj.get());
	}
	KTL_INLINE tTVReal NativeMathEx::sphericalHarmonicR(unsigned n, int m, tTVReal theta, tTVReal phi) {
		return boost::math::spherical_harmonic_r(n, m, theta, phi);
	}
	KTL_INLINE tTVReal NativeMathEx::sphericalHarmonicI(unsigned n, int m, tTVReal theta, tTVReal phi) {
		return boost::math::spherical_harmonic_i(n, m, theta, phi);
	}
	//
	//	SUMMARY: ベッセル関数系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::cylBesselJ(tTVReal v, tTVReal x) {
		return boost::math::cyl_bessel_j(v, x);
	}
	KTL_INLINE tTVReal NativeMathEx::cylNeumann(tTVReal v, tTVReal x) {
		return boost::math::cyl_neumann(v, x);
	}
	KTL_INLINE tTVReal NativeMathEx::cylBesselI(tTVReal v, tTVReal x) {
		return boost::math::cyl_bessel_i(v, x);
	}
	KTL_INLINE tTVReal NativeMathEx::cylBesselK(tTVReal v, tTVReal x) {
		return boost::math::cyl_bessel_k(v, x);
	}
	KTL_INLINE tTVReal NativeMathEx::sphBessel(unsigned v, tTVReal x) {
		return boost::math::sph_bessel(v, x);
	}
	KTL_INLINE tTVReal NativeMathEx::sphNeumann(unsigned v, tTVReal x) {
		return boost::math::sph_neumann(v, x);
	}
	//
	//	SUMMARY: ゼータ関数系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::zeta(tTVReal z) {
		return boost::math::zeta(z);
	}
	//
	//	SUMMARY: シンク関数系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::sincPi(tTVReal x) {
		return boost::math::sinc_pi(x);
	}
	KTL_INLINE tTVReal NativeMathEx::sinhcPi(tTVReal x) {
		return boost::math::sinhc_pi(x);
	}
	//
	//	SUMMARY: 楕円積分系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::ellintRf(tTVReal x, tTVReal y, tTVReal z) {
		return boost::math::ellint_rf(x, y, z);
	}
	KTL_INLINE tTVReal NativeMathEx::ellintRd(tTVReal x, tTVReal y, tTVReal z) {
		return boost::math::ellint_rd(x, y, z);
	}
	KTL_INLINE tTVReal NativeMathEx::ellintRj(tTVReal x, tTVReal y, tTVReal z, tTVReal p) {
		return boost::math::ellint_rj(x, y, z, p);
	}
	KTL_INLINE tTVReal NativeMathEx::ellintRc(tTVReal x, tTVReal y) {
		return boost::math::ellint_rc(x, y);
	}
	KTL_INLINE tTVReal NativeMathEx::ellint1(tTVReal k, tTVReal phi) {
		return boost::math::ellint_1(k, phi);
	}
	KTL_INLINE tTVReal NativeMathEx::ellint1(tTVReal k) {
		return boost::math::ellint_1(k);
	}
	KTL_INLINE tTVReal NativeMathEx::ellint2(tTVReal k, tTVReal phi) {
		return boost::math::ellint_2(k, phi);
	}
	KTL_INLINE tTVReal NativeMathEx::ellint2(tTVReal k) {
		return boost::math::ellint_2(k);
	}
	KTL_INLINE tTVReal NativeMathEx::ellint3(tTVReal k, tTVReal n, tTVReal phi) {
		return boost::math::ellint_3(k, n, phi);
	}
	KTL_INLINE tTVReal NativeMathEx::ellint3(tTVReal k, tTVReal n) {
		return boost::math::ellint_3(k, n);
	}
	//
	//	SUMMARY: 指数積分系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::expint(unsigned n, tTVReal z) {
		return boost::math::expint(n, z);
	}
	KTL_INLINE tTVReal NativeMathEx::expint(tTVReal z) {
		return boost::math::expint(z);
	}
	//
	//	SUMMARY: 階乗系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::factorial(unsigned i) {
		return boost::math::factorial<tTVReal>(i);
	}
	KTL_INLINE tTVReal NativeMathEx::doubleFactorial(unsigned i) {
		return boost::math::double_factorial<tTVReal>(i);
	}
	KTL_INLINE tTVReal NativeMathEx::risingFactorial(tTVReal x, int n) {
		return boost::math::rising_factorial(x, n);
	}
	KTL_INLINE tTVReal NativeMathEx::fallingFactorial(tTVReal x, unsigned i) {
		return boost::math::falling_factorial(x, i);
	}
	KTL_INLINE tTVReal NativeMathEx::binomialCoefficient(unsigned n, unsigned k) {
		return boost::math::binomial_coefficient<tTVReal>(n, k);
	}
	//
	//	SUMMARY: 公倍公約系メソッド
	//
	KTL_INLINE tTVInteger NativeMathEx::gcd(tTVInteger a, tTVInteger b) {
		return boost::math::gcd(a, b);
	}
	KTL_INLINE tTVInteger NativeMathEx::lcm(tTVInteger a, tTVInteger b) {
		return boost::math::lcm(a, b);
	}
	//
	//	SUMMARY: 比較系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::min(tjs_int numparams, tTJSVariant** param) {
		return **std::min_element(param, param + numparams, RealCompare());
	}
	KTL_INLINE tTVReal NativeMathEx::max(tjs_int numparams, tTJSVariant** param) {
		return **std::max_element(param, param + numparams, RealCompare());
	}
	//
	//	SUMMARY: ランダム系メソッド
	//
	KTL_INLINE tTVReal NativeMathEx::random() {
		static bool const initialized = seedTime();
		return tTVReal(std::rand()) / RAND_MAX;
	};
	KTL_INLINE tTVReal NativeMathEx::randomDevice() {
		static boost::random::random_device random;
		return tTVReal(random()) / random.max();
	};
	//
	//	SUMMARY: 定数系プロパティ
	//
	KTL_INLINE tTVReal NativeMathEx::pi() {
		return boost::math::constants::pi<tTVReal>();
	}
	KTL_INLINE tTVReal NativeMathEx::e() {
		return boost::math::constants::e<tTVReal>();
	}
	KTL_INLINE tTVReal NativeMathEx::ln10() {
		return 2.3025850929940456840179914546844;
	}
	KTL_INLINE tTVReal NativeMathEx::ln2() {
		return boost::math::constants::ln_two<tTVReal>();
	}
	KTL_INLINE tTVReal NativeMathEx::log10e() {
		return 0.43429448190325182765112891891661;
	}
	KTL_INLINE tTVReal NativeMathEx::log2e() {
		return 1.4426950408889634073599246810019;
	}
	KTL_INLINE tTVReal NativeMathEx::sqrt2() {
		return boost::math::constants::root_two<tTVReal>();
	}
	KTL_INLINE tTVReal NativeMathEx::sqrt1_2() {
		return boost::math::constants::half_root_two<tTVReal>();
	}

	//
	// MathEx
	//
	MathEx::MathEx() {}
	tjs_error TJS_INTF_METHOD MathEx::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("MathEx::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeMathEx>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD MathEx::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("MathEx::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 算術系メソッド
	//
	KTL_INLINE tTVReal MathEx::abs(tTVReal x) {
		return NativeMathEx::abs(x);
	}
	KTL_INLINE tTVReal MathEx::ceil(tTVReal x) {
		return NativeMathEx::ceil(x);
	}
	KTL_INLINE tTVReal MathEx::floor(tTVReal x) {
		return NativeMathEx::floor(x);
	}
	KTL_INLINE tTVReal MathEx::sqrt(tTVReal x) {
		return NativeMathEx::sqrt(x);
	}
	KTL_INLINE tTVReal MathEx::cbrt(tTVReal x) {
		return NativeMathEx::cbrt(x);
	}
	KTL_INLINE tTVReal MathEx::sqrt1Pm1(tTVReal x) {
		return NativeMathEx::sqrt1Pm1(x);
	}
	KTL_INLINE tTVReal MathEx::hypot(tTVReal x, tTVReal y) {
		return NativeMathEx::hypot(x, y);
	}
	//
	//	SUMMARY: 浮動小数点数系メソッド
	//
	KTL_INLINE tTVReal MathEx::round(tTVReal x) {
		return NativeMathEx::round(x);
	}
	KTL_INLINE tTVReal MathEx::trunc(tTVReal x) {
		return NativeMathEx::trunc(x);
	}
	KTL_INLINE tTJSVariant MathEx::modf(tTVReal x) {
		return NativeMathEx::modf(x);
	}
	KTL_INLINE tTVReal MathEx::fpart(tTVReal x) {
		return NativeMathEx::fpart(x);
	}
	KTL_INLINE tTVReal MathEx::ldexp(tTVReal x, tTVInteger exponent) {
		return NativeMathEx::ldexp(x, boost::numeric_cast<int>(exponent));
	}
	KTL_INLINE tTJSVariant MathEx::frexp(tTVReal x) {
		return NativeMathEx::frexp(x);
	}
	//
	//	SUMMARY: 浮動小数点数比較系メソッド
	//
	KTL_INLINE bool MathEx::equals(tTVReal a, tTVReal b) {
		return NativeMathEx::equals(a, b);
	}
	KTL_INLINE bool MathEx::notEquals(tTVReal a, tTVReal b) {
		return NativeMathEx::notEquals(a, b);
	}
	KTL_INLINE bool MathEx::less(tTVReal a, tTVReal b) {
		return NativeMathEx::less(a, b);
	}
	KTL_INLINE bool MathEx::greater(tTVReal a, tTVReal b) {
		return NativeMathEx::greater(a, b);
	}
	KTL_INLINE bool MathEx::lessEqual(tTVReal a, tTVReal b) {
		return NativeMathEx::lessEqual(a, b);
	}
	KTL_INLINE bool MathEx::greaterEqual(tTVReal a, tTVReal b) {
		return NativeMathEx::greaterEqual(a, b);
	}
	KTL_INLINE tTVInteger MathEx::compare(tTVReal a, tTVReal b) {
		return NativeMathEx::compare(a, b);
	}
	//
	//	SUMMARY: 三角関数系メソッド
	//
	KTL_INLINE tTVReal MathEx::sin(tTVReal x) {
		return NativeMathEx::sin(x);
	}
	KTL_INLINE tTVReal MathEx::cos(tTVReal x) {
		return NativeMathEx::cos(x);
	}
	KTL_INLINE tTVReal MathEx::tan(tTVReal x) {
		return NativeMathEx::tan(x);
	}
	KTL_INLINE tTVReal MathEx::asin(tTVReal x) {
		return NativeMathEx::asin(x);
	}
	KTL_INLINE tTVReal MathEx::acos(tTVReal x) {
		return NativeMathEx::acos(x);
	}
	KTL_INLINE tTVReal MathEx::atan(tTVReal x) {
		return NativeMathEx::atan(x);
	}
	KTL_INLINE tTVReal MathEx::atan2(tTVReal y, tTVReal x) {
		return NativeMathEx::atan2(y, x);
	};
	//
	//	SUMMARY: 双曲線関数系メソッド
	//
	KTL_INLINE tTVReal MathEx::sinh(tTVReal x) {
		return NativeMathEx::sinh(x);
	}
	KTL_INLINE tTVReal MathEx::cosh(tTVReal x) {
		return NativeMathEx::cosh(x);
	}
	KTL_INLINE tTVReal MathEx::tanh(tTVReal x) {
		return NativeMathEx::tanh(x);
	}
	KTL_INLINE tTVReal MathEx::asinh(tTVReal x) {
		return NativeMathEx::asinh(x);
	}
	KTL_INLINE tTVReal MathEx::acosh(tTVReal x) {
		return NativeMathEx::acosh(x);
	}
	KTL_INLINE tTVReal MathEx::atanh(tTVReal x) {
		return NativeMathEx::atanh(x);
	}
	//
	//	SUMMARY: 指数対数系メソッド
	//
	KTL_INLINE tTVReal MathEx::pow(tTVReal base, tTVReal exponent) {
		return NativeMathEx::pow(base, exponent);
	}
	KTL_INLINE tTVReal MathEx::exp(tTVReal x) {
		return NativeMathEx::exp(x);
	};
	KTL_INLINE tTVReal MathEx::pow10(tTVReal x) {
		return NativeMathEx::pow10(x);
	}
	KTL_INLINE tTVReal MathEx::pow2(tTVReal x) {
		return NativeMathEx::pow2(x);
	}
	KTL_INLINE tTVReal MathEx::log(tTVReal x) {
		return NativeMathEx::log(x);
	};
	KTL_INLINE tTVReal MathEx::log10(tTVReal x) {
		return NativeMathEx::log10(x);
	};
	KTL_INLINE tTVReal MathEx::log2(tTVReal x) {
		return NativeMathEx::log2(x);
	};
	KTL_INLINE tTVReal MathEx::logN(tTVReal base, tTVReal x) {
		return NativeMathEx::logN(base, x);
	};
	KTL_INLINE tTVReal MathEx::powm1(tTVReal x, tTVReal y) {
		return NativeMathEx::powm1(x, y);
	}
	KTL_INLINE tTVReal MathEx::expm1(tTVReal x) {
		return NativeMathEx::expm1(x);
	}
	KTL_INLINE tTVReal MathEx::log1P(tTVReal x) {
		return NativeMathEx::log1P(x);
	}
	//
	//	SUMMARY: ガンマ関数系メソッド
	//
	KTL_INLINE tTVReal MathEx::tgamma(tTVReal x) {
		return NativeMathEx::tgamma(x);
	}
	KTL_INLINE tTVReal MathEx::tgamma1Pm1(tTVReal x) {
		return NativeMathEx::tgamma1Pm1(x);
	}
	KTL_INLINE tTVReal MathEx::lgamma(tTVReal x) {
		return NativeMathEx::lgamma(x);
	}
	KTL_INLINE tTVReal MathEx::digamma(tTVReal x) {
		return NativeMathEx::digamma(x);
	}
	KTL_INLINE tTVReal MathEx::tgammaRatio(tTVReal x, tTVReal y) {
		return NativeMathEx::tgammaRatio(x, y);
	}
	KTL_INLINE tTVReal MathEx::tgammaDeltaRatio(tTVReal x, tTVReal delta) {
		return NativeMathEx::tgammaDeltaRatio(x, delta);
	}
	KTL_INLINE tTVReal MathEx::gammaP(tTVReal a, tTVReal x) {
		return NativeMathEx::gammaP(a, x);
	}
	KTL_INLINE tTVReal MathEx::gammaQ(tTVReal a, tTVReal x) {
		return NativeMathEx::gammaQ(a, x);
	}
	KTL_INLINE tTVReal MathEx::tgammaLower(tTVReal a, tTVReal x) {
		return NativeMathEx::tgammaLower(a, x);
	}
	KTL_INLINE tTVReal MathEx::tgamma(tTVReal a, tTVReal x) {
		return NativeMathEx::tgamma(a, x);
	}
	KTL_INLINE tTVReal MathEx::gammaPInv(tTVReal a, tTVReal p) {
		return NativeMathEx::gammaPInv(a, p);
	}
	KTL_INLINE tTVReal MathEx::gammaQInv(tTVReal a, tTVReal q) {
		return NativeMathEx::gammaQInv(a, q);
	}
	KTL_INLINE tTVReal MathEx::gammaPInva(tTVReal a, tTVReal p) {
		return NativeMathEx::gammaPInva(a, p);
	}
	KTL_INLINE tTVReal MathEx::gammaQInva(tTVReal a, tTVReal q) {
		return NativeMathEx::gammaQInva(a, q);
	}
	KTL_INLINE tTVReal MathEx::gammaPDerivative(tTVReal a, tTVReal x) {
		return NativeMathEx::gammaPDerivative(a, x);
	}
	//
	//	SUMMARY: ベータ関数系メソッド
	//
	KTL_INLINE tTVReal MathEx::beta(tTVReal a, tTVReal b) {
		return NativeMathEx::beta(a, b);
	}
	KTL_INLINE tTVReal MathEx::ibeta(tTVReal a, tTVReal b, tTVReal x) {
		return NativeMathEx::ibeta(a, b, x);
	}
	KTL_INLINE tTVReal MathEx::ibetac(tTVReal a, tTVReal b, tTVReal x) {
		return NativeMathEx::ibetac(a, b, x);
	}
	KTL_INLINE tTVReal MathEx::beta(tTVReal a, tTVReal b, tTVReal x) {
		return NativeMathEx::beta(a, b, x);
	}
	KTL_INLINE tTVReal MathEx::betac(tTVReal a, tTVReal b, tTVReal x) {
		return NativeMathEx::betac(a, b, x);
	}
	KTL_INLINE tTVReal MathEx::ibetaInv(tTVReal a, tTVReal b, tTVReal p) {
		return NativeMathEx::ibetaInv(a, b, p);
	}
	KTL_INLINE tTVReal MathEx::ibetacInv(tTVReal a, tTVReal b, tTVReal q) {
		return NativeMathEx::ibetacInv(a, b, q);
	}
	KTL_INLINE tTVReal MathEx::ibetaInva(tTVReal a, tTVReal b, tTVReal p) {
		return NativeMathEx::ibetaInva(a, b, p);
	}
	KTL_INLINE tTVReal MathEx::ibetacInva(tTVReal a, tTVReal b, tTVReal q) {
		return NativeMathEx::ibetacInva(a, b, q);
	}
	KTL_INLINE tTVReal MathEx::ibetaInvb(tTVReal a, tTVReal b, tTVReal p) {
		return NativeMathEx::ibetaInvb(a, b, p);
	}
	KTL_INLINE tTVReal MathEx::ibetacInvb(tTVReal a, tTVReal b, tTVReal q) {
		return NativeMathEx::ibetacInvb(a, b, q);
	}
	KTL_INLINE tTVReal MathEx::ibetaDerivative(tTVReal a, tTVReal b, tTVReal x) {
		return NativeMathEx::betac(a, b, x);
	}
	//
	//	SUMMARY:誤差関数系メソッド
	//
	KTL_INLINE tTVReal MathEx::erf(tTVReal x) {
		return NativeMathEx::erf(x);
	}
	KTL_INLINE tTVReal MathEx::erfc(tTVReal x) {
		return NativeMathEx::erfc(x);
	}
	KTL_INLINE tTVReal MathEx::erfInv(tTVReal p) {
		return NativeMathEx::erfInv(p);
	}
	KTL_INLINE tTVReal MathEx::erfcInv(tTVReal p) {
		return NativeMathEx::erfcInv(p);
	}
	//
	//	SUMMARY:ルジャンドル多項式系メソッド
	//
	KTL_INLINE tTVReal MathEx::legendreP(tTVInteger l, tTVReal x) {
		return NativeMathEx::legendreP(boost::numeric_cast<int>(l), x);
	}
	KTL_INLINE tTVReal MathEx::legendreP(tTVInteger l, tTVInteger m, tTVReal x) {
		return NativeMathEx::legendreP(boost::numeric_cast<int>(l), boost::numeric_cast<int>(m), x);
	}
	KTL_INLINE tTVReal MathEx::legendreQ(tTVInteger l, tTVReal x) {
		return NativeMathEx::legendreQ(boost::numeric_cast<unsigned>(l), x);
	}
	KTL_INLINE tTVReal MathEx::legendreNext(tTVInteger l, tTVReal x, tTVReal pl, tTVReal plm1) {
		return NativeMathEx::legendreNext(boost::numeric_cast<unsigned>(l), x, pl, plm1);
	}
	KTL_INLINE tTVReal MathEx::legendreNext(tTVInteger l, tTVInteger m, tTVReal x, tTVReal pl, tTVReal plm1) {
		return NativeMathEx::legendreNext(boost::numeric_cast<unsigned>(l), boost::numeric_cast<unsigned>(m), x, pl, plm1);
	}
	KTL_INLINE tTVReal MathEx::laguerre(tTVInteger n, tTVReal x) {
		return NativeMathEx::laguerre(boost::numeric_cast<int>(n), x);
	}
	KTL_INLINE tTVReal MathEx::laguerre(tTVInteger n, tTVInteger m, tTVReal x) {
		return NativeMathEx::laguerre(boost::numeric_cast<int>(n), boost::numeric_cast<int>(m), x);
	}
	KTL_INLINE tTVReal MathEx::laguerreNext(tTVInteger n, tTVReal x, tTVReal ln, tTVReal lnm1) {
		return NativeMathEx::laguerreNext(boost::numeric_cast<unsigned>(n), x, ln, lnm1);
	}
	KTL_INLINE tTVReal MathEx::laguerreNext(tTVInteger n, tTVInteger l, tTVReal x, tTVReal ln, tTVReal lnm1) {
		return NativeMathEx::laguerreNext(boost::numeric_cast<unsigned>(n), boost::numeric_cast<unsigned>(l), x, ln, lnm1);
	}
	KTL_INLINE tTVReal MathEx::hermite(tTVInteger n, tTVReal x) {
		return NativeMathEx::hermite(boost::numeric_cast<unsigned>(n), x);
	}
	KTL_INLINE tTVReal MathEx::hermiteNext(tTVInteger n, tTVReal x, tTVReal hn, tTVReal hnm1) {
		return NativeMathEx::hermiteNext(boost::numeric_cast<unsigned>(n), x, hn, hnm1);
	}
	KTL_INLINE tTJSVariant MathEx::sphericalHarmonic(tTVInteger n, tTVInteger m, tTVReal theta, tTVReal phi) {
		return NativeMathEx::sphericalHarmonic(boost::numeric_cast<unsigned>(n), boost::numeric_cast<int>(m), theta, phi);
	}
	KTL_INLINE tTVReal MathEx::sphericalHarmonicR(tTVInteger n, tTVInteger m, tTVReal theta, tTVReal phi) {
		return NativeMathEx::sphericalHarmonicR(boost::numeric_cast<unsigned>(n), boost::numeric_cast<int>(m), theta, phi);
	}
	KTL_INLINE tTVReal MathEx::sphericalHarmonicI(tTVInteger n, tTVInteger m, tTVReal theta, tTVReal phi) {
		return NativeMathEx::sphericalHarmonicI(boost::numeric_cast<unsigned>(n), boost::numeric_cast<int>(m), theta, phi);
	}
	//
	//	SUMMARY: ベッセル関数系メソッド
	//
	KTL_INLINE tTVReal MathEx::cylBesselJ(tTVReal v, tTVReal x) {
		return NativeMathEx::cylBesselJ(v, x);
	}
	KTL_INLINE tTVReal MathEx::cylNeumann(tTVReal v, tTVReal x) {
		return NativeMathEx::cylNeumann(v, x);
	}
	KTL_INLINE tTVReal MathEx::cylBesselI(tTVReal v, tTVReal x) {
		return NativeMathEx::cylBesselI(v, x);
	}
	KTL_INLINE tTVReal MathEx::cylBesselK(tTVReal v, tTVReal x) {
		return NativeMathEx::cylBesselK(v, x);
	}
	KTL_INLINE tTVReal MathEx::sphBessel(tTVInteger v, tTVReal x) {
		return NativeMathEx::sphBessel(boost::numeric_cast<unsigned>(v), x);
	}
	KTL_INLINE tTVReal MathEx::sphNeumann(tTVInteger v, tTVReal x) {
		return NativeMathEx::sphNeumann(boost::numeric_cast<unsigned>(v), x);
	}
	//
	//	SUMMARY: ゼータ関数系メソッド
	//
	KTL_INLINE tTVReal MathEx::zeta(tTVReal z) {
		return NativeMathEx::zeta(z);
	}
	//
	//	SUMMARY: シンク関数系メソッド
	//
	KTL_INLINE tTVReal MathEx::sincPi(tTVReal x) {
		return NativeMathEx::sincPi(x);
	}
	KTL_INLINE tTVReal MathEx::sinhcPi(tTVReal x) {
		return NativeMathEx::sinhcPi(x);
	}
	//
	//	SUMMARY: 楕円積分系メソッド
	//
	KTL_INLINE tTVReal MathEx::ellintRf(tTVReal x, tTVReal y, tTVReal z) {
		return NativeMathEx::ellintRf(x, y, z);
	}
	KTL_INLINE tTVReal MathEx::ellintRd(tTVReal x, tTVReal y, tTVReal z) {
		return NativeMathEx::ellintRd(x, y, z);
	}
	KTL_INLINE tTVReal MathEx::ellintRj(tTVReal x, tTVReal y, tTVReal z, tTVReal p) {
		return NativeMathEx::ellintRj(x, y, z, p);
	}
	KTL_INLINE tTVReal MathEx::ellintRc(tTVReal x, tTVReal y) {
		return NativeMathEx::ellintRc(x, y);
	}
	KTL_INLINE tTVReal MathEx::ellint1(tTVReal k, tTVReal phi) {
		return NativeMathEx::ellint1(k, phi);
	}
	KTL_INLINE tTVReal MathEx::ellint1(tTVReal k) {
		return NativeMathEx::ellint1(k);
	}
	KTL_INLINE tTVReal MathEx::ellint2(tTVReal k, tTVReal phi) {
		return NativeMathEx::ellint2(k, phi);
	}
	KTL_INLINE tTVReal MathEx::ellint2(tTVReal k) {
		return NativeMathEx::ellint2(k);
	}
	KTL_INLINE tTVReal MathEx::ellint3(tTVReal k, tTVReal n, tTVReal phi) {
		return NativeMathEx::ellint3(k, n, phi);
	}
	KTL_INLINE tTVReal MathEx::ellint3(tTVReal k, tTVReal n) {
		return NativeMathEx::ellint3(k, n);
	}
	//
	//	SUMMARY: 指数積分系メソッド
	//
	KTL_INLINE tTVReal MathEx::expint(tTVInteger n, tTVReal z) {
		return NativeMathEx::expint(boost::numeric_cast<unsigned>(n), z);
	}
	KTL_INLINE tTVReal MathEx::expint(tTVReal z) {
		return NativeMathEx::expint(z);
	}
	//
	//	SUMMARY: 階乗系メソッド
	//
	KTL_INLINE tTVReal MathEx::factorial(tTVInteger i) {
		return NativeMathEx::factorial(boost::numeric_cast<unsigned>(i));
	}
	KTL_INLINE tTVReal MathEx::doubleFactorial(tTVInteger i) {
		return NativeMathEx::doubleFactorial(boost::numeric_cast<unsigned>(i));
	}
	KTL_INLINE tTVReal MathEx::risingFactorial(tTVReal x, tTVInteger n) {
		return NativeMathEx::risingFactorial(x, boost::numeric_cast<int>(n));
	}
	KTL_INLINE tTVReal MathEx::fallingFactorial(tTVReal x, tTVInteger i) {
		return NativeMathEx::fallingFactorial(x, boost::numeric_cast<unsigned>(i));
	}
	KTL_INLINE tTVReal MathEx::binomialCoefficient(tTVInteger n, tTVInteger k) {
		return NativeMathEx::binomialCoefficient(boost::numeric_cast<unsigned>(n), boost::numeric_cast<unsigned>(k));
	}
	//
	//	SUMMARY: 公倍公約系メソッド
	//
	KTL_INLINE tTVInteger MathEx::gcd(tTVInteger a, tTVInteger b) {
		return NativeMathEx::gcd(a, b);
	}
	KTL_INLINE tTVInteger MathEx::lcm(tTVInteger a, tTVInteger b) {
		return NativeMathEx::lcm(a, b);
	}
	//
	//	SUMMARY: 比較系メソッド
	//
	KTL_INLINE tTVReal MathEx::min(tjs_int numparams, tTJSVariant** param) {
		return NativeMathEx::min(numparams, param);
	}
	KTL_INLINE tTVReal MathEx::max(tjs_int numparams, tTJSVariant** param) {
		return NativeMathEx::max(numparams, param);
	}
	//
	//	SUMMARY: ランダム系メソッド
	//
	KTL_INLINE tTVReal MathEx::random() {
		return NativeMathEx::random();
	};
	KTL_INLINE tTVReal MathEx::randomDevice() {
		return NativeMathEx::randomDevice();
	};
	//
	//	SUMMARY: 定数系プロパティ
	//
	KTL_INLINE tTVReal MathEx::pi() {
		return NativeMathEx::pi();
	}
	KTL_INLINE tTVReal MathEx::e() {
		return NativeMathEx::e();
	}
	KTL_INLINE tTVReal MathEx::ln10() {
		return NativeMathEx::ln10();
	}
	KTL_INLINE tTVReal MathEx::ln2() {
		return NativeMathEx::ln2();
	}
	KTL_INLINE tTVReal MathEx::log10e() {
		return NativeMathEx::log10e();
	}
	KTL_INLINE tTVReal MathEx::log2e() {
		return NativeMathEx::log2e();
	}
	KTL_INLINE tTVReal MathEx::sqrt2() {
		return NativeMathEx::sqrt2();
	}
	KTL_INLINE tTVReal MathEx::sqrt1_2() {
		return NativeMathEx::sqrt1_2();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
