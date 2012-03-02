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

//
// KTL_FAIL_MESSAGE
//
#define KTL_FAIL_MESSAGE(SECTION, MESSAGE, LEVEL) \
	{ \
		SPRIG_KRKR_SECTION((SECTION), LEVEL); \
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), (MESSAGE), LEVEL); \
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("file"), SPRIG_KRKR_TJS_W(__FILE__), LEVEL); \
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("line"), __LINE__, LEVEL); \
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("function"), SPRIG_KRKR_TJS_W(BOOST_CURRENT_FUNCTION), LEVEL); \
	}

//
// KTL_WARNING
//
#define KTL_WARNING(SECTION, MESSAGE) \
	KTL_FAIL_MESSAGE((SECTION), (MESSAGE), SPRIG_KRKR_LOG_LEVEL_WARNING)

//
// KTL_ERROR_NOTHROW
//
#define KTL_ERROR_NOTHROW(SECTION, MESSAGE) \
	KTL_FAIL_MESSAGE((SECTION), (MESSAGE), SPRIG_KRKR_LOG_LEVEL_ERROR)

//
// KTL_ERROR
//
#define KTL_ERROR(SECTION, MESSAGE, TYPE) \
	{ \
		KTL_ERROR_NOTHROW((SECTION), (MESSAGE)); \
		SPRIG_THROW_EXCEPTION(TYPE(sprig::section_line((SECTION), (MESSAGE)).c_str())); \
	}

#endif	// #ifndef KTL_ERROR_HPP
