#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <sprig/external/unicode/ucsdet.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeEncoding
	//
	class NativeEncoding {
	public:
		typedef boost::uint_fast32_t flag_type;
		typedef boost::int_fast32_t int_type;
		typedef std::vector<tjs_uint8> binary_type;
		typedef std::basic_string<tjs_char> string_type;
		typedef std::basic_string<tjs_nchar> narrow_string_type;
		typedef std::size_t size_type;
	private:
		static bool encodeAsBuffer(
			binary_type& buffer,
			tjs_char const* charset,
			tjs_char const* source
			);
		static bool encodeAsBuffer(
			binary_type& buffer,
			tjs_char const* charset,
			tjs_uint8 const* source_data,
			tjs_uint source_length,
			tjs_char const* source_charset
			);
		static bool encodeAsString(
			string_type& string,
			tjs_uint8 const* source_data,
			tjs_uint source_length,
			tjs_char const* source_charset
			);
		static bool URLEncodeAsBuffer(
			binary_type& dest,
			binary_type const& src,
			flag_type flag = 0x00000000
			);
		static bool URLDecodeAsBuffer(
			binary_type& dest,
			binary_type const& src
			);
		static bool URLDecodeAsBuffer(
			binary_type& dest,
			narrow_string_type const& src
			);
		static bool base64EncodeAsBuffer(
			binary_type& dest,
			tjs_uint8 const* source_data,
			tjs_uint source_length,
			size_type line_max_length = 76
			);
		static bool base64DecodeAsBuffer(
			binary_type& dest,
			tjs_char const* source
			);
		static bool base64DecodeAsBuffer(
			binary_type& dest,
			tjs_uint8 const* source_data,
			tjs_uint source_length
			);
		static boost::shared_ptr<UCharsetDetector> setupDetector(
			tjs_uint8 const* source_data,
			tjs_uint source_length
			);
		static tTJSVariant makeMatchResult(
			UCharsetMatch const* matcher,
			int_type confidence_threshold
			);
	public:
		NativeEncoding();
	public:
		//
		//	SUMMARY: URLエンコード系フラグ
		//
		static flag_type const uefSpaceToPlus = 0x00000001;
		static flag_type const uefEncodeTilde = 0x00000100;
	public:
		//
		//	SUMMARY: URLエンコード系メソッド
		//
		static tTJSVariant URLEncodeToAsciiOctet(
			tjs_char const* charset,
			tjs_char const* source,
			flag_type flag = 0x00000000
			);
		static tTJSVariant URLEncodeToAsciiOctet(
			tjs_char const* charset,
			tTJSVariantOctet const* source,
			tjs_char const* source_charset,
			flag_type flag = 0x00000000
			);
		static tTJSVariant URLEncodeToAsciiOctet(
			tTJSVariantOctet const* source,
			flag_type flag = 0x00000000
			);
		static tTJSVariant URLEncodeToString(
			tjs_char const* charset,
			tjs_char const* source,
			flag_type flag = 0x00000000
			);
		static tTJSVariant URLEncodeToString(
			tjs_char const* charset,
			tTJSVariantOctet const* source,
			tjs_char const* source_charset,
			flag_type flag = 0x00000000
			);
		static tTJSVariant URLEncodeToString(
			tTJSVariantOctet const* source,
			flag_type flag = 0x00000000
			);
		static tTJSVariant URLDecodeToOctetData(
			tjs_char const* source
			);
		static tTJSVariant URLDecodeToOctetData(
			tTJSVariantOctet const* source,
			tjs_char const* source_charset
			);
		static tTJSVariant URLDecodeToOctetData(
			tTJSVariantOctet const* source
			);
		static tTJSVariant URLDecodeToString(
			tjs_char const* charset,
			tjs_char const* source
			);
		static tTJSVariant URLDecodeToString(
			tjs_char const* charset,
			tTJSVariantOctet const* source,
			tjs_char const* source_charset
			);
		static tTJSVariant URLDecodeToString(
			tjs_char const* charset,
			tTJSVariantOctet const* source
			);
		//
		//	SUMMARY: Base64エンコード系メソッド
		//
		static tTJSVariant base64EncodeToAsciiOctet(
			tTJSVariantOctet const* source,
			size_type line_max_length = 76
			);
		static tTJSVariant base64EncodeToString(
			tTJSVariantOctet const* source,
			size_type line_max_length = 76
			);
		static tTJSVariant base64DecodeToOctetData(
			tjs_char const* source
			);
		static tTJSVariant base64DecodeToOctetData(
			tTJSVariantOctet const* source
			);
		//
		//	SUMMARY: エンコード系メソッド
		//
		static tTJSVariant encodeToOctet(
			tjs_char const* charset,
			tjs_char const* source
			);
		static tTJSVariant encodeToOctet(
			tjs_char const* charset,
			tTJSVariantOctet const* source,
			tjs_char const* source_charset
			);
		static tTJSVariant encodeToString(
			tTJSVariantOctet const* source,
			tjs_char const* source_charset
			);
		//
		//	SUMMARY: 比較系メソッド
		//
		static bool sameCharsetAlias(
			tjs_char const* name1,
			tjs_char const* name2
			);
		//
		//	SUMMARY: 検出系メソッド
		//
		static tTJSVariant detect(
			tTJSVariantOctet const* source,
			size_type length = 0,
			int_type confidence_threshold = 0
			);
		static tTJSVariant detectAll(
			tTJSVariantOctet const* source,
			size_type length = 0,
			int_type confidence_threshold = 0
			);
		static bool checkDetected(
			tjs_char const* charset_name,
			tTJSVariantOctet const* source,
			size_type length = 0,
			int_type confidence_threshold = 0
			);
	};

	//
	// Encoding
	//
	class Encoding
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeEncoding> instance_;
	public:
		Encoding();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: URLエンコード系フラグ
		//
		static tTVInteger const uefSpaceToPlus = NativeEncoding::uefSpaceToPlus;
		static tTVInteger const uefEncodeTilde = NativeEncoding::uefEncodeTilde;
	public:
		//
		//	SUMMARY: URLエンコード系メソッド
		//
		static tTJSVariant URLEncodeToAsciiOctet(
			tTJSVariantString const* charset,
			tTJSVariantString const* source,
			tTVInteger flag = 0x00000000
			);
		static tTJSVariant URLEncodeToAsciiOctet(
			tTJSVariantString const* charset,
			tTJSVariantOctet const* source,
			tTJSVariantString const* source_charset,
			tTVInteger flag = 0x00000000
			);
		static tTJSVariant URLEncodeToAsciiOctet(
			tTJSVariantOctet const* source,
			tTVInteger flag = 0x00000000
			);
		static tTJSVariant URLEncodeToString(
			tTJSVariantString const* charset,
			tTJSVariantString const* source,
			tTVInteger flag = 0x00000000
			);
		static tTJSVariant URLEncodeToString(
			tTJSVariantString const* charset,
			tTJSVariantOctet const* source,
			tTJSVariantString const* source_charset,
			tTVInteger flag = 0x00000000
			);
		static tTJSVariant URLEncodeToString(
			tTJSVariantOctet const* source,
			tTVInteger flag = 0x00000000
			);
		static tTJSVariant URLDecodeToOctetData(
			tTJSVariantString const* source
			);
		static tTJSVariant URLDecodeToOctetData(
			tTJSVariantOctet const* source,
			tTJSVariantString const* source_charset
			);
		static tTJSVariant URLDecodeToOctetData(
			tTJSVariantOctet const* source
			);
		static tTJSVariant URLDecodeToString(
			tTJSVariantString const* charset,
			tTJSVariantString const* source
			);
		static tTJSVariant URLDecodeToString(
			tTJSVariantString const* charset,
			tTJSVariantOctet const* source,
			tTJSVariantString const* source_charset
			);
		static tTJSVariant URLDecodeToString(
			tTJSVariantString const* charset,
			tTJSVariantOctet const* source
			);
		//
		//	SUMMARY: Base64エンコード系メソッド
		//
		static tTJSVariant base64EncodeToAsciiOctet(
			tTJSVariantOctet const* source,
			tTVInteger line_max_length = 76
			);
		static tTJSVariant base64EncodeToString(
			tTJSVariantOctet const* source,
			tTVInteger line_max_length = 76
			);
		static tTJSVariant base64DecodeToOctetData(
			tTJSVariantString const* source
			);
		static tTJSVariant base64DecodeToOctetData(
			tTJSVariantOctet const* source
			);
		//
		//	SUMMARY: エンコード系メソッド
		//
		static tTJSVariant encodeToOctet(
			tTJSVariantString const* charset,
			tTJSVariantString const* source
			);
		static tTJSVariant encodeToOctet(
			tTJSVariantString const* charset,
			tTJSVariantOctet const* source,
			tTJSVariantString const* source_charset
			);
		static tTJSVariant encodeToString(
			tTJSVariantOctet const* source,
			tTJSVariantString const* source_charset
			);
		//
		//	SUMMARY: 比較系メソッド
		//
		static bool sameCharsetAlias(
			tTJSVariantString const* name1,
			tTJSVariantString const* name2
			);
		//
		//	SUMMARY: 検出系メソッド
		//
		static tTJSVariant detect(
			tTJSVariantOctet const* source,
			tTVInteger length = 0,
			tTVInteger confidence_threshold = 0
			);
		static tTJSVariant detectAll(
			tTJSVariantOctet const* source,
			tTVInteger length = 0,
			tTVInteger confidence_threshold = 0
			);
		static bool checkDetected(
			tTJSVariantString const* charset_name,
			tTJSVariantOctet const* source,
			tTVInteger length = 0,
			tTVInteger confidence_threshold = 0
			);
	};
}	// namespace ktl
