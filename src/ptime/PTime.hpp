/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_PTIME_PTIME_HPP
#define SRC_PTIME_PTIME_HPP

#include <string>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/utility/swap.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "PTimeDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:PTime:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:PTime:error")

namespace ktl {
	//
	// NativePTime
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type NativePTime::getInstance(iTJSDispatch2* obj) {
		return sprig::krkr::tjs::GetPropValue<sprig::krkr::tjs::intptr_type>(
			sprig::krkr::tjs::GetMemberNoAddRef(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("PTime")),
				SPRIG_KRKR_TJS_W("instance")
				),
			obj
			);
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativePTime::createNew(tjs_int numparams, tTJSVariant** param) {
		iTJSDispatch2* result_obj = 0;
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("PTime")),
			&result_obj,
			numparams,
			param,
			0
			);
		return sprig::krkr::tjs::object_type(result_obj, false);
	}
	NativePTime::NativePTime()
		: ptime_(date_type(1970, 1, 1))
	{}
	//
	//	SUMMARY: 割り当て系メソッド
	//
	KTL_INLINE bool NativePTime::assignLocalTimeSecondClock() {
		ptime_type ptime = boost::posix_time::second_clock::local_time();
		if (ptime.is_special()) {
			return false;
		}
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignUniversalTimeSecondClock() {
		ptime_type ptime = boost::posix_time::second_clock::universal_time();
		if (ptime.is_special()) {
			return false;
		}
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignLocalTimeMicrosecClock() {
		ptime_type ptime = boost::posix_time::microsec_clock::local_time();
		if (ptime.is_special()) {
			return false;
		}
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignUniversalTimeMicrosecClock() {
		ptime_type ptime = boost::posix_time::microsec_clock::universal_time();
		if (ptime.is_special()) {
			return false;
		}
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignLocalTimeDayClock() {
		date_type date = boost::gregorian::day_clock::local_day();
		if (date.is_special()) {
			return false;
		}
		ptime_type ptime(
			date,
			time_duration_type()
			);
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignUniversalTimeDayClock() {
		date_type date = boost::gregorian::day_clock::universal_day();
		if (date.is_special()) {
			return false;
		}
		ptime_type ptime(
			date,
			time_duration_type()
			);
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignDateTime(
		date_type::year_type y,
		date_type::month_type m,
		date_type::day_type d,
		time_duration_type::hour_type hour,
		time_duration_type::min_type min,
		time_duration_type::sec_type sec,
		time_duration_type::tick_type fs
		)
	{
		ptime_type ptime(
			date_type(y, m, d),
			time_duration_type(hour, min, sec, fs)
			);
		if (ptime.is_special()) {
			return false;
		}
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignDate(
		date_type::year_type y,
		date_type::month_type m,
		date_type::day_type d
		)
	{
		ptime_type ptime(
			date_type(y, m, d),
			ptime_.time_of_day()
			);
		if (ptime.is_special()) {
			return false;
		}
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignTime(
		time_duration_type::hour_type hour,
		time_duration_type::min_type min,
		time_duration_type::sec_type sec,
		time_duration_type::tick_type fs
		)
	{
		ptime_type ptime(
			ptime_.date(),
			time_duration_type(hour, min, sec, fs)
			);
		if (ptime.is_special()) {
			return false;
		}
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignFromString(std::string const& source) {
		ptime_type ptime = boost::posix_time::time_from_string(source);
		if (ptime.is_special()) {
			return false;
		}
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignFromISOString(std::string const& source) {
		ptime_type ptime = boost::posix_time::from_iso_string(source);
		if (ptime.is_special()) {
			return false;
		}
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignDateFromString(std::string const& source) {
		date_type date = boost::gregorian::from_string(source);
		if (date.is_special()) {
			return false;
		}
		ptime_type ptime(
			date,
			ptime_.time_of_day()
			);
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignDateFromISOString(std::string const& source) {
		date_type date = boost::gregorian::from_undelimited_string(source);
		if (date.is_special()) {
			return false;
		}
		ptime_type ptime(
			date,
			ptime_.time_of_day()
			);
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignTimeFromString(std::string const& source) {
		time_duration_type dur = boost::posix_time::duration_from_string(source);
		if (dur.is_special()) {
			return false;
		}
		ptime_type ptime(
			ptime_.date(),
			dur
			);
		boost::swap(ptime_, ptime);
		return true;
	}
	KTL_INLINE bool NativePTime::assignTimeFromISOString(std::string const& source) {
		time_duration_type dur = boost::date_time::parse_undelimited_time_duration<time_duration_type>(source);
		if (dur.is_special()) {
			return false;
		}
		ptime_type ptime(
			ptime_.date(),
			dur
			);
		boost::swap(ptime_, ptime);
		return true;
	}
	//
	//	SUMMARY: 変更系メソッド
	//
	KTL_INLINE void NativePTime::moveYears(boost::gregorian::years t) {
		ptime_ += t;
	}
	KTL_INLINE void NativePTime::moveMonths(boost::gregorian::months t) {
		ptime_ += t;
	}
	KTL_INLINE void NativePTime::moveDays(boost::gregorian::days t) {
		ptime_ += t;
	}
	KTL_INLINE void NativePTime::moveHours(boost::posix_time::hours t) {
		ptime_ += t;
	}
	KTL_INLINE void NativePTime::moveMinutes(boost::posix_time::minutes t) {
		ptime_ += t;
	}
	KTL_INLINE void NativePTime::moveSeconds(boost::posix_time::seconds t) {
		ptime_ += t;
	}
	KTL_INLINE void NativePTime::moveMilliseconds(boost::posix_time::milliseconds t) {
		ptime_ += t;
	}
	KTL_INLINE void NativePTime::moveMicroseconds(boost::posix_time::microseconds t) {
		ptime_ += t;
	}
	//
	//	SUMMARY: 文字列化系メソッド
	//
	KTL_INLINE tTJSVariant NativePTime::toString() const {
		return tTJSVariant(boost::posix_time::to_simple_wstring(ptime_).c_str());
	}
	KTL_INLINE tTJSVariant NativePTime::toISOString() const {
		return tTJSVariant(boost::posix_time::to_iso_wstring(ptime_).c_str());
	}
	KTL_INLINE tTJSVariant NativePTime::toISOExtendedString() const {
		return tTJSVariant(boost::posix_time::to_iso_extended_wstring(ptime_).c_str());
	}
	KTL_INLINE tTJSVariant NativePTime::toDataString() const {
		return tTJSVariant(boost::gregorian::to_simple_wstring(ptime_.date()).c_str());
	}
	KTL_INLINE tTJSVariant NativePTime::toDataISOString() const {
		return tTJSVariant(boost::gregorian::to_iso_wstring(ptime_.date()).c_str());
	}
	KTL_INLINE tTJSVariant NativePTime::toDataISOExtendedString() const {
		return tTJSVariant(boost::gregorian::to_iso_extended_wstring(ptime_.date()).c_str());
	}
	KTL_INLINE tTJSVariant NativePTime::toTimeString() const {
		return tTJSVariant(boost::posix_time::to_simple_wstring(ptime_.time_of_day()).c_str());
	}
	KTL_INLINE tTJSVariant NativePTime::toTimeISOString() const {
		return tTJSVariant(boost::posix_time::to_iso_wstring(ptime_.time_of_day()).c_str());
	}
	//
	//	SUMMARY: 日付プロパティ系メソッド
	//
	KTL_INLINE NativePTime::date_type::year_type NativePTime::year() const {
		return ptime_.date().year();
	}
	KTL_INLINE NativePTime::date_type::month_type NativePTime::month() const {
		return ptime_.date().month();
	}
	KTL_INLINE NativePTime::date_type::day_type NativePTime::day() const {
		return ptime_.date().day();
	}
	KTL_INLINE NativePTime::date_type::day_of_year_type NativePTime::dayOfYear() const {
		return ptime_.date().day_of_year();
	}
	KTL_INLINE NativePTime::date_type::day_of_week_type NativePTime::dayOfWeek() const {
		return ptime_.date().day_of_week();
	}
	KTL_INLINE int NativePTime::weekNumber() const {
		return ptime_.date().week_number();
	}
	KTL_INLINE NativePTime::date_type::date_int_type NativePTime::julianDay() const {
		return ptime_.date().julian_day();
	}
	KTL_INLINE NativePTime::date_type::date_int_type NativePTime::modjulianDay() const {
		return ptime_.date().modjulian_day();
	}
	//
	//	SUMMARY: 時刻プロパティ系メソッド
	//
	KTL_INLINE NativePTime::time_duration_type::hour_type NativePTime::hours() const {
		return ptime_.time_of_day().hours();
	}
	KTL_INLINE NativePTime::time_duration_type::min_type NativePTime::minutes() const {
		return ptime_.time_of_day().minutes();
	}
	KTL_INLINE NativePTime::time_duration_type::sec_type NativePTime::seconds() const {
		return ptime_.time_of_day().seconds();
	}
	KTL_INLINE NativePTime::time_duration_type::tick_type NativePTime::totalSeconds() const {
		return ptime_.time_of_day().total_seconds();
	};
	KTL_INLINE NativePTime::time_duration_type::tick_type NativePTime::totalMilliseconds() const {
		return ptime_.time_of_day().total_milliseconds();
	}
	KTL_INLINE NativePTime::time_duration_type::tick_type NativePTime::totalMicroseconds() const {
		return ptime_.time_of_day().total_milliseconds();
	}
	KTL_INLINE NativePTime::time_duration_type::tick_type NativePTime::totalNanoseconds() const {
		return ptime_.time_of_day().total_nanoseconds();
	}
	KTL_INLINE NativePTime::time_duration_type::fractional_seconds_type NativePTime::fractionalSeconds() const {
		return ptime_.time_of_day().fractional_seconds();
	}
	KTL_INLINE unsigned short NativePTime::numFractionalDigits() const {
		return ptime_.time_of_day().num_fractional_digits();
	}

	//
	// PTime
	//
	PTime::PTime() {}
	tjs_error TJS_INTF_METHOD PTime::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("PTime::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativePTime>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD PTime::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("PTime::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 内部系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type PTime::getInstance() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(this);
	}
	//
	//	SUMMARY: 割り当て系メソッド
	//
	KTL_INLINE bool PTime::assignLocalTimeSecondClock() {
		return instance_->assignLocalTimeSecondClock();
	}
	KTL_INLINE bool PTime::assignUniversalTimeSecondClock() {
		return instance_->assignUniversalTimeSecondClock();
	}
	KTL_INLINE bool PTime::assignLocalTimeMicrosecClock() {
		return instance_->assignLocalTimeMicrosecClock();
	}
	KTL_INLINE bool PTime::assignUniversalTimeMicrosecClock() {
		return instance_->assignUniversalTimeMicrosecClock();
	}
	KTL_INLINE bool PTime::assignLocalTimeDayClock() {
		return instance_->assignLocalTimeDayClock();
	}
	KTL_INLINE bool PTime::assignUniversalTimeDayClock() {
		return instance_->assignUniversalTimeDayClock();
	}
	KTL_INLINE bool PTime::assignDateTime(
		tTVInteger y,
		tTVInteger m,
		tTVInteger d,
		tTVInteger hour,
		tTVInteger min,
		tTVInteger sec,
		tTVInteger fs
		)
	{
		typedef NativePTime::date_type date_type;
		typedef NativePTime::time_duration_type time_duration_type;
		return instance_->assignDateTime(
			boost::numeric_cast<date_type::year_type::value_type>(y),
			boost::numeric_cast<date_type::month_type::value_type>(m),
			boost::numeric_cast<date_type::day_type::value_type>(d),
			boost::numeric_cast<time_duration_type::hour_type>(hour),
			boost::numeric_cast<time_duration_type::min_type>(min),
			boost::numeric_cast<time_duration_type::sec_type>(sec),
			boost::numeric_cast<time_duration_type::tick_type>(fs)
			);
	}
	KTL_INLINE bool PTime::assignDate(
		tTVInteger y,
		tTVInteger m,
		tTVInteger d
		)
	{
		typedef NativePTime::date_type date_type;
		return instance_->assignDate(
			boost::numeric_cast<date_type::year_type::value_type>(y),
			boost::numeric_cast<date_type::month_type::value_type>(m),
			boost::numeric_cast<date_type::day_type::value_type>(d)
			);
	}
	KTL_INLINE bool PTime::assignTime(
		tTVInteger hour,
		tTVInteger min,
		tTVInteger sec,
		tTVInteger fs
		)
	{
		typedef NativePTime::time_duration_type time_duration_type;
		return instance_->assignTime(
			boost::numeric_cast<time_duration_type::hour_type>(hour),
			boost::numeric_cast<time_duration_type::min_type>(min),
			boost::numeric_cast<time_duration_type::sec_type>(sec),
			boost::numeric_cast<time_duration_type::tick_type>(fs)
			);
	}
	KTL_INLINE bool PTime::assignFromString(tTJSVariantString const* source) {
		return instance_->assignFromString(
			sprig::str_cast<std::string>(sprig::krkr::tjs::as_c_str(source))
			);
	}
	KTL_INLINE bool PTime::assignFromISOString(tTJSVariantString const* source) {
		return instance_->assignFromISOString(
			sprig::str_cast<std::string>(sprig::krkr::tjs::as_c_str(source))
			);
	}
	KTL_INLINE bool PTime::assignDateFromString(tTJSVariantString const* source) {
		return instance_->assignDateFromString(
			sprig::str_cast<std::string>(sprig::krkr::tjs::as_c_str(source))
			);
	}
	KTL_INLINE bool PTime::assignDateFromISOString(tTJSVariantString const* source) {
		return instance_->assignDateFromISOString(
			sprig::str_cast<std::string>(sprig::krkr::tjs::as_c_str(source))
			);
	}
	KTL_INLINE bool PTime::assignTimeFromString(tTJSVariantString const* source) {
		return instance_->assignTimeFromString(
			sprig::str_cast<std::string>(sprig::krkr::tjs::as_c_str(source))
			);
	}
	KTL_INLINE bool PTime::assignTimeFromISOString(tTJSVariantString const* source) {
		return instance_->assignTimeFromISOString(
			sprig::str_cast<std::string>(sprig::krkr::tjs::as_c_str(source))
			);
	}
	//
	//	SUMMARY: 変更系メソッド
	//
	KTL_INLINE void PTime::moveYears(tTVInteger t) {
		instance_->moveYears(
			boost::gregorian::years(boost::numeric_cast<NativePTime::date_duration_type::duration_rep_type>(t))
			);
	}
	KTL_INLINE void PTime::moveMonths(tTVInteger t) {
		instance_->moveMonths(
			boost::gregorian::months(boost::numeric_cast<NativePTime::date_duration_type::duration_rep_type>(t))
			);
	}
	KTL_INLINE void PTime::moveDays(tTVInteger t) {
		instance_->moveDays(
			boost::gregorian::days(boost::numeric_cast<NativePTime::date_duration_type::duration_rep_type>(t))
			);
	}
	KTL_INLINE void PTime::moveHours(tTVInteger t) {
		instance_->moveHours(
			boost::posix_time::hours(boost::numeric_cast<NativePTime::time_duration_type::hour_type>(t))
			);
	}
	KTL_INLINE void PTime::moveMinutes(tTVInteger t) {
		instance_->moveMinutes(
			boost::posix_time::minutes(boost::numeric_cast<NativePTime::time_duration_type::min_type>(t))
			);
	}
	KTL_INLINE void PTime::moveSeconds(tTVInteger t) {
		instance_->moveSeconds(
			boost::posix_time::seconds(boost::numeric_cast<NativePTime::time_duration_type::sec_type>(t))
			);
	}
	KTL_INLINE void PTime::moveMilliseconds(tTVInteger t) {
		instance_->moveMilliseconds(
			boost::posix_time::milliseconds(boost::numeric_cast<NativePTime::time_duration_type::tick_type>(t))
			);
	}
	KTL_INLINE void PTime::moveMicroseconds(tTVInteger t) {
		instance_->moveMicroseconds(
			boost::posix_time::microseconds(boost::numeric_cast<NativePTime::time_duration_type::tick_type>(t))
			);
	}
	//
	//	SUMMARY: 文字列化系メソッド
	//
	KTL_INLINE tTJSVariant PTime::toString() const {
		return instance_->toString();
	}
	KTL_INLINE tTJSVariant PTime::toISOString() const {
		return instance_->toISOString();
	}
	KTL_INLINE tTJSVariant PTime::toISOExtendedString() const {
		return instance_->toISOExtendedString();
	}
	KTL_INLINE tTJSVariant PTime::toDataString() const {
		return instance_->toDataString();
	}
	KTL_INLINE tTJSVariant PTime::toDataISOString() const {
		return instance_->toDataISOString();
	}
	KTL_INLINE tTJSVariant PTime::toDataISOExtendedString() const {
		return instance_->toDataISOExtendedString();
	}
	KTL_INLINE tTJSVariant PTime::toTimeString() const {
		return instance_->toTimeString();
	}
	KTL_INLINE tTJSVariant PTime::toTimeISOString() const {
		return instance_->toTimeISOString();
	}
	//
	//	SUMMARY: 日付プロパティ系メソッド
	//
	KTL_INLINE tTVInteger PTime::year() const {
		return instance_->year();
	}
	KTL_INLINE tTVInteger PTime::month() const {
		return instance_->month();
	}
	KTL_INLINE tTVInteger PTime::day() const {
		return instance_->day();
	}
	KTL_INLINE tTVInteger PTime::dayOfYear() const {
		return instance_->dayOfYear();
	}
	KTL_INLINE tTVInteger PTime::dayOfWeek() const {
		return instance_->dayOfWeek();
	}
	KTL_INLINE tTVInteger PTime::weekNumber() const {
		return instance_->weekNumber();
	}
	KTL_INLINE tTVInteger PTime::julianDay() const {
		return instance_->julianDay();
	}
	KTL_INLINE tTVInteger PTime::modjulianDay() const {
		return instance_->modjulianDay();
	}
	//
	//	SUMMARY: 時刻プロパティ系メソッド
	//
	KTL_INLINE tTVInteger PTime::hours() const {
		return instance_->hours();
	}
	KTL_INLINE tTVInteger PTime::minutes() const {
		return instance_->minutes();
	}
	KTL_INLINE tTVInteger PTime::seconds() const {
		return instance_->seconds();
	}
	KTL_INLINE tTVInteger PTime::totalSeconds() const {
		return instance_->totalSeconds();
	}
	KTL_INLINE tTVInteger PTime::totalMilliseconds() const {
		return instance_->totalMilliseconds();
	}
	KTL_INLINE tTVInteger PTime::totalMicroseconds() const {
		return instance_->totalMicroseconds();
	}
	KTL_INLINE tTVInteger PTime::totalNanoseconds() const {
		return instance_->totalNanoseconds();
	}
	KTL_INLINE tTVInteger PTime::fractionalSeconds() const {
		return instance_->fractionalSeconds();
	}
	KTL_INLINE tTVInteger PTime::numFractionalDigits() const {
		return instance_->numFractionalDigits();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_PTIME_PTIME_HPP
