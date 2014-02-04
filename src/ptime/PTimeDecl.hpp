/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_PTIME_PTIME_DECL_HPP
#define SRC_PTIME_PTIME_DECL_HPP

#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativePTime
	//
	class NativePTime {
	public:
		typedef boost::posix_time::ptime ptime_type;
		typedef ptime_type::time_type time_type;
		typedef ptime_type::time_duration_type time_duration_type;
		typedef ptime_type::date_type date_type;
		typedef ptime_type::date_duration_type date_duration_type;
	private:
		static sprig::krkr::tjs::intptr_type getInstance(iTJSDispatch2* obj);
		static sprig::krkr::tjs::object_type createNew(tjs_int numparams, tTJSVariant** param);
	private:
		ptime_type ptime_;
	public:
		NativePTime();
	public:
		//
		//	SUMMARY: 割り当て系メソッド
		//
		bool assignLocalTimeSecondClock();
		bool assignUniversalTimeSecondClock();
		bool assignLocalTimeMicrosecClock();
		bool assignUniversalTimeMicrosecClock();
		bool assignLocalTimeDayClock();
		bool assignUniversalTimeDayClock();
		bool assignDateTime(
			date_type::year_type y,
			date_type::month_type m = 1,
			date_type::day_type d = 1,
			time_duration_type::hour_type hour = 0,
			time_duration_type::min_type min = 0,
			time_duration_type::sec_type sec = 0,
			time_duration_type::tick_type fs = 0
			);
		bool assignDate(
			date_type::year_type y,
			date_type::month_type m = 1,
			date_type::day_type d = 1
			);
		bool assignTime(
			time_duration_type::hour_type hour = 0,
			time_duration_type::min_type min = 0,
			time_duration_type::sec_type sec = 0,
			time_duration_type::tick_type fs = 0
			);
		bool assignFromString(std::string const& source);
		bool assignFromISOString(std::string const& source);
		bool assignDateFromString(std::string const& source);
		bool assignDateFromISOString(std::string const& source);
		bool assignTimeFromString(std::string const& source);
		bool assignTimeFromISOString(std::string const& source);
		//
		//	SUMMARY: 文字列化系メソッド
		//
		tTJSVariant toString() const;
		tTJSVariant toISOString() const;
		tTJSVariant toISOExtendedString() const;
		tTJSVariant toDataString() const;
		tTJSVariant toDataISOString() const;
		tTJSVariant toDataISOExtendedString() const;
		tTJSVariant toTimeString() const;
		tTJSVariant toTimeISOString() const;
		//
		//	SUMMARY: 変更系メソッド
		//
		void moveYears(boost::gregorian::years t);
		void moveMonths(boost::gregorian::months t);
		void moveDays(boost::gregorian::days t);
		void moveHours(boost::posix_time::hours t);
		void moveMinutes(boost::posix_time::minutes t);
		void moveSeconds(boost::posix_time::seconds t);
		void moveMilliseconds(boost::posix_time::milliseconds t);
		void moveMicroseconds(boost::posix_time::microseconds t);
		//
		//	SUMMARY: 日付プロパティ系メソッド
		//
		date_type::year_type year() const;
		date_type::month_type month() const;
		date_type::day_type day() const;
		date_type::day_of_year_type dayOfYear() const;
		date_type::day_of_week_type dayOfWeek() const;
		int weekNumber() const;
		date_type::date_int_type julianDay() const;
		date_type::date_int_type modjulianDay() const;
		//
		//	SUMMARY: 時刻プロパティ系メソッド
		//
		time_duration_type::hour_type hours() const;
		time_duration_type::min_type minutes() const;
		time_duration_type::sec_type seconds() const;
		time_duration_type::tick_type totalSeconds() const;
		time_duration_type::tick_type totalMilliseconds() const;
		time_duration_type::tick_type totalMicroseconds() const;
		time_duration_type::tick_type totalNanoseconds() const;
		time_duration_type::fractional_seconds_type fractionalSeconds() const;
		unsigned short numFractionalDigits() const;
	};

	//
	// PTime
	//
	class PTime
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativePTime> instance_;
	public:
		PTime();
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
		sprig::krkr::tjs::intptr_type getInstance();
		//
		//	SUMMARY: 割り当て系メソッド
		//
		bool assignLocalTimeSecondClock();
		bool assignUniversalTimeSecondClock();
		bool assignLocalTimeMicrosecClock();
		bool assignUniversalTimeMicrosecClock();
		bool assignLocalTimeDayClock();
		bool assignUniversalTimeDayClock();
		bool assignDateTime(
			tTVInteger y,
			tTVInteger m = 1,
			tTVInteger d = 1,
			tTVInteger hour = 0,
			tTVInteger min = 0,
			tTVInteger sec = 0,
			tTVInteger fs = 0
			);
		bool assignDate(
			tTVInteger y,
			tTVInteger m = 1,
			tTVInteger d = 1
			);
		bool assignTime(
			tTVInteger hour = 0,
			tTVInteger min = 0,
			tTVInteger sec = 0,
			tTVInteger fs = 0
			);
		bool assignFromString(tTJSVariantString const* source);
		bool assignFromISOString(tTJSVariantString const* source);
		bool assignDateFromString(tTJSVariantString const* source);
		bool assignDateFromISOString(tTJSVariantString const* source);
		bool assignTimeFromString(tTJSVariantString const* source);
		bool assignTimeFromISOString(tTJSVariantString const* source);
		//
		//	SUMMARY: 変更系メソッド
		//
		void moveYears(tTVInteger t);
		void moveMonths(tTVInteger t);
		void moveDays(tTVInteger t);
		void moveHours(tTVInteger t);
		void moveMinutes(tTVInteger t);
		void moveSeconds(tTVInteger t);
		void moveMilliseconds(tTVInteger t);
		void moveMicroseconds(tTVInteger t);
		//
		//	SUMMARY: 文字列化系メソッド
		//
		tTJSVariant toString() const;
		tTJSVariant toISOString() const;
		tTJSVariant toISOExtendedString() const;
		tTJSVariant toDataString() const;
		tTJSVariant toDataISOString() const;
		tTJSVariant toDataISOExtendedString() const;
		tTJSVariant toTimeString() const;
		tTJSVariant toTimeISOString() const;
		//
		//	SUMMARY: 日付プロパティ系メソッド
		//
		tTVInteger year() const;
		tTVInteger month() const;
		tTVInteger day() const;
		tTVInteger dayOfYear() const;
		tTVInteger dayOfWeek() const;
		tTVInteger weekNumber() const;
		tTVInteger julianDay() const;
		tTVInteger modjulianDay() const;
		//
		//	SUMMARY: 時刻プロパティ系メソッド
		//
		tTVInteger hours() const;
		tTVInteger minutes() const;
		tTVInteger seconds() const;
		tTVInteger totalSeconds() const;
		tTVInteger totalMilliseconds() const;
		tTVInteger totalMicroseconds() const;
		tTVInteger totalNanoseconds() const;
		tTVInteger fractionalSeconds() const;
		tTVInteger numFractionalDigits() const;
	};
}	// namespace ktl

#endif	// #ifndef SRC_PTIME_PTIME_DECL_HPP
