#ifndef KTL_ERROR_HPP
#define KTL_ERROR_HPP

#include <sprig/config/config.hpp>

#ifdef SPRIG_USING_PRAGMA_ONCE
#	pragma once
#endif	// #ifdef SPRIG_USING_PRAGMA_ONCE

#include <ktl/config.hpp>

#include <boost/current_function.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/exception.hpp>
#include <sprig/section.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>

namespace ktl {
	namespace {
		//
		// fail_message
		//
		KTL_INLINE void fail_message(
			sprig::krkr::tjs::string_argument_type const& section,
			sprig::krkr::tjs::string_argument_type const& message,
			sprig::krkr::tjs::string_argument_type const& file,
			sprig::krkr::tjs::string_argument_type const& line,
			sprig::krkr::tjs::string_argument_type const& function
			)
		{
			sprig::krkr::tjs::section s(section);
			sprig::krkr::tjs::output_value(SPRIG_KRKR_TJS_W("message"), message);
			sprig::krkr::tjs::output_value(SPRIG_KRKR_TJS_W("file"), file);
			sprig::krkr::tjs::output_value(SPRIG_KRKR_TJS_W("line"), line);
			sprig::krkr::tjs::output_value(SPRIG_KRKR_TJS_W("function"), function);
		}
	}	// anonymous-namespace
}	// namespace ktl

//
// KTL_FAIL_MESSAGE
//
#define KTL_FAIL_MESSAGE(SECTION, MESSAGE, LEVEL) \
	SPRIG_KRKR_LOG_LEVEL_SWITCH( \
		ktl::fail_message((SECTION), (MESSAGE), SPRIG_KRKR_TJS_W(__FILE__), __LINE__, SPRIG_KRKR_TJS_W(BOOST_CURRENT_FUNCTION)), \
		LEVEL \
		)

//
// KTL_WARNING
// KTL_ERROR_NOTHROW
// KTL_ERROR
//
#define KTL_WARNING(SECTION, MESSAGE) \
	KTL_FAIL_MESSAGE((SECTION), (MESSAGE), SPRIG_KRKR_LOG_LEVEL_WARNING)
#define KTL_ERROR_NOTHROW(SECTION, MESSAGE) \
	KTL_FAIL_MESSAGE((SECTION), (MESSAGE), SPRIG_KRKR_LOG_LEVEL_ERROR)
#define KTL_ERROR(SECTION, MESSAGE, TYPE) \
	{ \
		KTL_ERROR_NOTHROW((SECTION), (MESSAGE)); \
		SPRIG_THROW_EXCEPTION(TYPE(sprig::section_line((SECTION), (MESSAGE)).c_str())); \
	}

#endif	// #ifndef KTL_ERROR_HPP
