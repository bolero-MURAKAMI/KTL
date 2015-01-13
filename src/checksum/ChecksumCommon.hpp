/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_CHECKSUM_CHECKSUM_COMMON_HPP
#define SRC_CHECKSUM_CHECKSUM_COMMON_HPP

#include <cstddef>
#include <string>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include <sprig/octet.hpp>
#include <sprig/encode_hex.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>

namespace ktl {
	//
	// ChecksumUtils
	//
	class ChecksumUtils {
	public:
		typedef tjs_uint8 byte_type;
		typedef std::size_t size_type;
		template<typename Value>
		struct octet_of
			: public sprig::array_octet_of<Value, byte_type>
		{};
	public:
		//
		// toOctet
		//
		template<typename Value>
		static typename boost::enable_if<
			boost::is_integral<Value>,
			typename octet_of<Value>::type
		>::type toOctet(Value value) {
			return sprig::to_octet<typename octet_of<Value>::type>(value);
		}
		template<typename Value>
		static typename boost::disable_if<
			boost::is_integral<Value>,
			typename octet_of<Value>::type
		>::type toOctet(Value const& value) {
			return sprig::to_octet<typename octet_of<Value>::type>(value);
		}
		//
		// toVariant
		//
		template<typename Value>
		static typename boost::enable_if<
			boost::is_integral<Value>,
			tTJSVariant
		>::type toVariant(Value value) {
			return static_cast<tTVInteger>(value);
		}
		template<typename Value>
		static typename boost::disable_if<
			boost::is_integral<Value>,
			tTJSVariant
		>::type toVariant(Value const& value) {
			return tTJSVariant(value.elems, Value::static_size);
		}
		//
		// toVariantHexString
		//
		template<typename Value>
		static typename boost::enable_if<
			boost::is_integral<Value>,
			tTJSVariant
		>::type toVariantHexString(Value value, bool upper = false) {
			typedef typename octet_of<Value>::type octet_type;
			std::basic_string<tjs_char> str(octet_type::static_size * 2, SPRIG_KRKR_TJS_W('\0'));
			sprig::encode_hex<tjs_char>(toOctet(value), &str[0], !upper);
			return tTJSVariant(str.c_str());
		}
		template<typename Value>
		static typename boost::disable_if<
			boost::is_integral<Value>,
			tTJSVariant
		>::type toVariantHexString(Value const& value, bool upper = false) {
			std::basic_string<tjs_char> str(Value::static_size * 2, SPRIG_KRKR_TJS_W('\0'));
			sprig::encode_hex<tjs_char>(value, &str[0], !upper);
			return tTJSVariant(str.c_str());
		}
	};
}	// namespace ktl

#endif	// #ifndef SRC_CHECKSUM_CHECKSUM_COMMON_HPP
