/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_ENCODING_ENCODING_HPP
#define SRC_ENCODING_ENCODING_HPP

#include <clocale>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <sprig/external/unicode/ucnv.hpp>
#include <sprig/external/unicode/urename.hpp>
#include <sprig/external/unicode/ucsdet.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/get_pointer.hpp>
#include <sprig/locale_saver.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/str_length.hpp>
#include <sprig/url_encode.hpp>
#include <sprig/base64.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "EncodingDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Encoding:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Encoding:error")

namespace ktl {
	//
	// NativeEncoding
	//
	KTL_INLINE bool NativeEncoding::encodeAsBuffer(
		binary_type& buffer,
		tjs_char const* charset,
		tjs_char const* source
		)
	{
		//	COMMENT: コンバータ作成
		UErrorCode error = U_ZERO_ERROR;
		boost::shared_ptr<UConverter> converter(
			::ucnv_openU(charset, &error),
			&::ucnv_close
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_openU: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		std::size_t source_length = sprig::str_length(source);
		if (!source_length) {
			buffer.clear();
			return true;
		}
		//	COMMENT: 変換
		error = U_ZERO_ERROR;
		int32_t length = ::ucnv_fromUChars(
			converter.get(),
			0,
			0,
			source,
			source_length,
			&error
			);
		if (error != U_BUFFER_OVERFLOW_ERROR || length <= 0) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_fromUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		buffer.resize(length);
		error = U_ZERO_ERROR;
		::ucnv_fromUChars(
			converter.get(),
			reinterpret_cast<char*>(&buffer[0]),
			buffer.size(),
			source,
			source_length,
			&error
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_fromUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeEncoding::encodeAsBuffer(
		binary_type& buffer,
		tjs_char const* charset,
		tjs_uint8 const* source_data,
		tjs_uint source_length,
		tjs_char const* source_charset
		)
	{
		if (sameCharsetAlias(source_charset, charset)) {
			buffer.clear();
			std::copy(
				source_data,
				source_data + source_length,
				std::back_inserter(buffer)
				);
			return true;
		}
		//	COMMENT: Unicodeへのコンバータ作成
		UErrorCode error = U_ZERO_ERROR;
		boost::shared_ptr<UConverter> from_converter(
			::ucnv_openU(source_charset, &error),
			&::ucnv_close
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_openU: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		if (!source_length) {
			buffer.clear();
			return true;
		}
		//	COMMENT: Unicodeへの変換
		error = U_ZERO_ERROR;
		int32_t uchars_length = ::ucnv_toUChars(
			from_converter.get(),
			0,
			0,
			reinterpret_cast<char const*>(source_data),
			source_length,
			&error
			);
		if (error != U_BUFFER_OVERFLOW_ERROR || uchars_length <= 0) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_toUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		std::vector<UChar> uchars(uchars_length);
		error = U_ZERO_ERROR;
		::ucnv_toUChars(
			from_converter.get(),
			&uchars[0],
			uchars.size(),
			reinterpret_cast<char const*>(source_data),
			source_length,
			&error
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_toUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		//	COMMENT: Unicodeからのコンバータ作成
		error = U_ZERO_ERROR;
		boost::shared_ptr<UConverter> to_converter(
			::ucnv_openU(charset, &error),
			&::ucnv_close
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_openU: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		//	COMMENT: Unicodeからの変換
		error = U_ZERO_ERROR;
		int32_t length = ::ucnv_fromUChars(
			to_converter.get(),
			0,
			0,
			&uchars[0],
			uchars.size(),
			&error
			);
		if (error != U_BUFFER_OVERFLOW_ERROR || length <= 0) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_fromUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		buffer.resize(length);
		error = U_ZERO_ERROR;
		::ucnv_fromUChars(
			to_converter.get(),
			reinterpret_cast<char*>(&buffer[0]),
			buffer.size(),
			&uchars[0],
			uchars.size(),
			&error
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_fromUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeEncoding::encodeAsString(
		string_type& string,
		tjs_uint8 const* source_data,
		tjs_uint source_length,
		tjs_char const* source_charset
		)
	{
		//	COMMENT: コンバータ作成
		UErrorCode error = U_ZERO_ERROR;
		boost::shared_ptr<UConverter> converter(
			::ucnv_openU(source_charset, &error),
			&::ucnv_close
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_openU: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		if (!source_length) {
			string.clear();
			return true;
		}
		//	COMMENT: 変換
		error = U_ZERO_ERROR;
		int32_t length = ::ucnv_toUChars(
			converter.get(),
			0,
			0,
			reinterpret_cast<char const*>(source_data),
			source_length,
			&error
			);
		if (error != U_BUFFER_OVERFLOW_ERROR || length <= 0) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_toUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		string.resize(length);
		error = U_ZERO_ERROR;
		::ucnv_toUChars(
			converter.get(),
			&string[0],
			string.size(),
			reinterpret_cast<char const*>(source_data),
			source_length,
			&error
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_toUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeEncoding::URLEncodeAsBuffer(
		binary_type& dest,
		binary_type const& src,
		flag_type flag
		)
	{
		dest.clear();
		return sprig::url_encode(
			src,
			std::back_inserter(dest),
			0
				| (flag & uefSpaceToPlus ? sprig::url_encode_options::space_to_plus : 0)
				| (flag & uefEncodeTilde ? sprig::url_encode_options::tilde : 0)
			);
	}
	KTL_INLINE bool NativeEncoding::URLDecodeAsBuffer(
		binary_type& dest,
		binary_type const& src
		)
	{
		dest.clear();
		return sprig::url_decode(
			src,
			std::back_inserter(dest)
			);
	}
	KTL_INLINE bool NativeEncoding::URLDecodeAsBuffer(
		binary_type& dest,
		narrow_string_type const& src
		)
	{
		dest.clear();
		return sprig::url_decode(
			src,
			std::back_inserter(dest)
			);
	}
	KTL_INLINE bool NativeEncoding::base64EncodeAsBuffer(
		binary_type& dest,
		tjs_uint8 const* source_data,
		tjs_uint source_length,
		size_type line_max_length
		)
	{
		dest.clear();
		return sprig::base64_encode(
			boost::iterator_range<tjs_uint8 const*>(
				source_data,
				source_data + source_length
				),
			std::back_inserter(dest),
			line_max_length
			);
	}
	KTL_INLINE bool NativeEncoding::base64DecodeAsBuffer(
		binary_type& dest,
		tjs_char const* source
		)
	{
		dest.clear();
		return sprig::base64_decode(
			sprig::str_cast<std::basic_string<tjs_nchar> >(source),
			std::back_inserter(dest)
			);
	}
	KTL_INLINE bool NativeEncoding::base64DecodeAsBuffer(
		binary_type& dest,
		tjs_uint8 const* source_data,
		tjs_uint source_length
		)
	{
		dest.clear();
		return sprig::base64_decode(
			boost::iterator_range<tjs_uint8 const*>(
				source_data,
				source_data + source_length
				),
			std::back_inserter(dest)
			);
	}
	KTL_INLINE boost::shared_ptr<UCharsetDetector> NativeEncoding::setupDetector(
		tjs_uint8 const* source_data,
		tjs_uint source_length
		)
	{
		UErrorCode error = U_ZERO_ERROR;
		//	COMMENT: ディテクタ作成
		boost::shared_ptr<UCharsetDetector> detector(
			::ucsdet_open(&error),
			&::ucsdet_close
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucsdet_open: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return boost::shared_ptr<UCharsetDetector>();
		}
		//	COMMENT: ソース設定
		::ucsdet_setText(
			detector.get(),
			reinterpret_cast<char const*>(source_data),
			source_length,
			&error
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucsdet_setText: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return boost::shared_ptr<UCharsetDetector>();
		}
		return detector;
	}
	KTL_INLINE tTJSVariant NativeEncoding::makeMatchResult(
		UCharsetMatch const* matcher,
		int_type confidence_threshold
		)
	{
		UErrorCode error = U_ZERO_ERROR;
		//	COMMENT: コンフィデンス
		int32_t confidence = ::ucsdet_getConfidence(matcher, &error);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucsdet_getConfidence: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return tTJSVariant();
		}
		if (confidence < confidence_threshold) {
			return tTJSVariant();
		}
		//	COMMENT: 名前
		char const* name = ::ucsdet_getName(matcher, &error);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucsdet_getName: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return tTJSVariant();
		}
		//	COMMENT: 言語
		char const* language = ::ucsdet_getLanguage(matcher, &error);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucsdet_getLanguage: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return tTJSVariant();
		}
		//	COMMENT: 結果生成
		sprig::krkr::tjs::object_type result(
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
				0, 0, 0
				),
			false
			);
		sprig::krkr::tjs::AddMember(sprig::get_pointer(result), SPRIG_KRKR_TJS_W("name"), tTJSVariant(name));
		sprig::krkr::tjs::AddMember(sprig::get_pointer(result), SPRIG_KRKR_TJS_W("language"), tTJSVariant(language));
		sprig::krkr::tjs::AddMember(sprig::get_pointer(result), SPRIG_KRKR_TJS_W("confidence"), tTJSVariant(confidence));
		return tTJSVariant(result.get(), result.get());
	}
	NativeEncoding::NativeEncoding() {}
	//
	//	SUMMARY: URLエンコード系メソッド
	//
	KTL_INLINE tTJSVariant NativeEncoding::URLEncodeToAsciiOctet(
		tjs_char const* charset,
		tjs_char const* source,
		flag_type flag
		)
	{
		binary_type src;
		if (!encodeAsBuffer(src, charset, source)) {
			return tTJSVariant();
		}
		binary_type dest;
		if (!URLEncodeAsBuffer(dest, src, flag)) {
			return tTJSVariant();
		}
		return tTJSVariant(
			dest.size()
				? &dest[0]
				: 0
				,
			dest.size()
			);
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLEncodeToAsciiOctet(
		tjs_char const* charset,
		tTJSVariantOctet const* source,
		tjs_char const* source_charset,
		flag_type flag
		)
	{
		binary_type src;
		if (!encodeAsBuffer(src, charset, sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_length(source), source_charset)) {
			return tTJSVariant();
		}
		binary_type dest;
		if (!URLEncodeAsBuffer(dest, src, flag)) {
			return tTJSVariant();
		}
		return tTJSVariant(
			dest.size()
				? &dest[0]
				: 0
				,
			dest.size()
			);
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLEncodeToAsciiOctet(
		tTJSVariantOctet const* source,
		flag_type flag
		)
	{
		binary_type src(sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_data(source) + sprig::krkr::tjs::octet_length(source));
		binary_type dest;
		if (!URLEncodeAsBuffer(dest, src, flag)) {
			return tTJSVariant();
		}
		return tTJSVariant(
			dest.size()
				? &dest[0]
				: 0
				,
			dest.size()
			);
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLEncodeToString(
		tjs_char const* charset,
		tjs_char const* source,
		flag_type flag
		)
	{
		binary_type src;
		if (!encodeAsBuffer(src, charset, source)) {
			return tTJSVariant();
		}
		binary_type dest;
		if (!URLEncodeAsBuffer(dest, src, flag)) {
			return tTJSVariant();
		}
		dest.push_back('\0');
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&dest[0]));
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLEncodeToString(
		tjs_char const* charset,
		tTJSVariantOctet const* source,
		tjs_char const* source_charset,
		flag_type flag
		)
	{
		binary_type src;
		if (!encodeAsBuffer(src, charset, sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_length(source), source_charset)) {
			return tTJSVariant();
		}
		binary_type dest;
		if (!URLEncodeAsBuffer(dest, src, flag)) {
			return tTJSVariant();
		}
		dest.push_back('\0');
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&dest[0]));
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLEncodeToString(
		tTJSVariantOctet const* source,
		flag_type flag
		)
	{
		binary_type src(sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_data(source) + sprig::krkr::tjs::octet_length(source));
		binary_type dest;
		if (!URLEncodeAsBuffer(dest, src, flag)) {
			return tTJSVariant();
		}
		dest.push_back('\0');
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&dest[0]));
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLDecodeToOctetData(
		tjs_char const* source
		)
	{
		binary_type dest;
		if (!URLDecodeAsBuffer(dest, sprig::str_cast<narrow_string_type>(source))) {
			return tTJSVariant();
		}
		return tTJSVariant(
			dest.size()
				? &dest[0]
				: 0
				,
			dest.size()
			);
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLDecodeToOctetData(
		tTJSVariantOctet const* source,
		tjs_char const* source_charset
		)
	{
		string_type src;
		if (!encodeAsString(src, sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_length(source), source_charset)) {
			return tTJSVariant();
		}
		binary_type dest;
		if (!URLDecodeAsBuffer(dest, sprig::str_cast<narrow_string_type>(src))) {
			return tTJSVariant();
		}
		return tTJSVariant(
			dest.size()
				? &dest[0]
				: 0
				,
			dest.size()
			);
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLDecodeToOctetData(
		tTJSVariantOctet const* source
		)
	{
		binary_type src(sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_data(source) + sprig::krkr::tjs::octet_length(source));
		binary_type dest;
		if (!URLDecodeAsBuffer(dest, src)) {
			return tTJSVariant();
		}
		return tTJSVariant(
			dest.size()
				? &dest[0]
				: 0
				,
			dest.size()
			);
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLDecodeToString(
		tjs_char const* charset,
		tjs_char const* source
		)
	{
		binary_type dest;
		if (!URLDecodeAsBuffer(dest, sprig::str_cast<narrow_string_type>(source))) {
			return tTJSVariant();
		}
		string_type string;
		if (dest.empty()) {
			return tTJSVariant(string.c_str());
		}
		if (!encodeAsString(string, &dest[0], dest.size(), charset)) {
			return tTJSVariant();
		}
		return tTJSVariant(string.c_str());
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLDecodeToString(
		tjs_char const* charset,
		tTJSVariantOctet const* source,
		tjs_char const* source_charset
		)
	{
		string_type src;
		if (!encodeAsString(src, sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_length(source), source_charset)) {
			return tTJSVariant();
		}
		binary_type dest;
		if (!URLDecodeAsBuffer(dest, sprig::str_cast<narrow_string_type>(src))) {
			return tTJSVariant();
		}
		string_type string;
		if (dest.empty()) {
			return tTJSVariant(string.c_str());
		}
		if (!encodeAsString(string, &dest[0], dest.size(), charset)) {
			return tTJSVariant();
		}
		return tTJSVariant(string.c_str());
	}
	KTL_INLINE tTJSVariant NativeEncoding::URLDecodeToString(
		tjs_char const* charset,
		tTJSVariantOctet const* source
		)
	{
		binary_type src(sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_data(source) + sprig::krkr::tjs::octet_length(source));
		binary_type dest;
		if (!URLDecodeAsBuffer(dest, src)) {
			return tTJSVariant();
		}
		string_type string;
		if (dest.empty()) {
			return tTJSVariant(string.c_str());
		}
		if (!encodeAsString(string, &dest[0], dest.size(), charset)) {
			return tTJSVariant();
		}
		return tTJSVariant(string.c_str());
	}
	//
	//	SUMMARY: Base64エンコード系メソッド
	//
	KTL_INLINE tTJSVariant NativeEncoding::base64EncodeToAsciiOctet(
		tTJSVariantOctet const* source,
		size_type line_max_length
		)
	{
		binary_type dest;
		if (!base64EncodeAsBuffer(
				dest,
				sprig::krkr::tjs::octet_data(source),
				sprig::krkr::tjs::octet_length(source),
				line_max_length
				)
			)
		{
			return tTJSVariant();
		}
		return tTJSVariant(
			dest.size()
				? &dest[0]
				: 0
				,
			dest.size()
			);
	}
	KTL_INLINE tTJSVariant NativeEncoding::base64EncodeToString(
		tTJSVariantOctet const* source,
		size_type line_max_length
		)
	{
		binary_type dest;
		if (!base64EncodeAsBuffer(
				dest,
				sprig::krkr::tjs::octet_data(source),
				sprig::krkr::tjs::octet_length(source),
				line_max_length
				)
			)
		{
			return tTJSVariant();
		}
		dest.push_back('\0');
		return tTJSVariant(
			reinterpret_cast<tjs_nchar const*>(&dest[0])
			);
	}
	KTL_INLINE tTJSVariant NativeEncoding::base64DecodeToOctetData(
		tjs_char const* source
		)
	{
		binary_type dest;
		if (!base64DecodeAsBuffer(
				dest,
				source
				)
			)
		{
			return tTJSVariant();
		}
		return tTJSVariant(
			dest.size()
				? &dest[0]
				: 0
				,
			dest.size()
			);
	}
	KTL_INLINE tTJSVariant NativeEncoding::base64DecodeToOctetData(
		tTJSVariantOctet const* source
		)
	{
		binary_type dest;
		if (!base64DecodeAsBuffer(
				dest,
				sprig::krkr::tjs::octet_data(source),
				sprig::krkr::tjs::octet_length(source)
				)
			)
		{
			return tTJSVariant();
		}
		return tTJSVariant(
			dest.size()
				? &dest[0]
				: 0
				,
			dest.size()
			);
	}
	//
	//	SUMMARY: エンコード系メソッド
	//
	KTL_INLINE tTJSVariant NativeEncoding::encodeToOctet(
		tjs_char const* charset,
		tjs_char const* source
		)
	{
		binary_type buffer;
		return encodeAsBuffer(buffer, charset, source)
			? tTJSVariant(
				buffer.size()
					? &buffer[0]
					: 0
					,
				buffer.size()
				)
			: tTJSVariant()
			;
	}
	KTL_INLINE tTJSVariant NativeEncoding::encodeToOctet(
		tjs_char const* charset,
		tTJSVariantOctet const* source,
		tjs_char const* source_charset
		)
	{
		binary_type buffer;
		return encodeAsBuffer(buffer, charset, sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_length(source), source_charset)
			? tTJSVariant(
				buffer.size()
					? &buffer[0]
					: 0
					,
				buffer.size()
				)
			: tTJSVariant()
			;
	}
	KTL_INLINE tTJSVariant NativeEncoding::encodeToString(
		tTJSVariantOctet const* source,
		tjs_char const* source_charset
		)
	{
		string_type string;
		return encodeAsString(string, sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_length(source), source_charset)
			? tTJSVariant(string.c_str())
			: tTJSVariant()
			;
	}
	//
	//	SUMMARY: 比較系メソッド
	//
	KTL_INLINE bool NativeEncoding::sameCharsetAlias(
		tjs_char const* name1,
		tjs_char const* name2
		)
	{
		sprig::locale_saver saver;
		std::setlocale(LC_ALL, "");
		return ::ucnv_compareNames(
			sprig::str_cast<std::string>(name1).c_str(),
			sprig::str_cast<std::string>(name2).c_str()
			)
			== 0
			;
	}
	//
	//	SUMMARY: 検出系メソッド
	//
	KTL_INLINE tTJSVariant NativeEncoding::detect(
		tTJSVariantOctet const* source,
		size_type length,
		int_type confidence_threshold
		)
	{
		tjs_uint8 const* source_data = sprig::krkr::tjs::octet_data(source);
		tjs_uint source_length = length
			? std::min<tjs_uint>(length, sprig::krkr::tjs::octet_length(source))
			: sprig::krkr::tjs::octet_length(source)
			;
		if (!source_length) {
			return tTJSVariant();
		}
		sprig::locale_saver saver;
		std::setlocale(LC_ALL, "");
		boost::shared_ptr<UCharsetDetector> detector = setupDetector(source_data, source_length);
		if (!detector) {
			return tTJSVariant();
		}
		//	COMMENT: 検出
		UErrorCode error = U_ZERO_ERROR;
		UCharsetMatch const* matcher = ::ucsdet_detect(detector.get(), &error);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucsdet_detect: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return tTJSVariant();
		}
		return makeMatchResult(matcher, confidence_threshold);
	}
	KTL_INLINE tTJSVariant NativeEncoding::detectAll(
		tTJSVariantOctet const* source,
		size_type length,
		int_type confidence_threshold
		)
	{
		tjs_uint8 const* source_data = sprig::krkr::tjs::octet_data(source);
		tjs_uint source_length = length
			? std::min<tjs_uint>(length, sprig::krkr::tjs::octet_length(source))
			: sprig::krkr::tjs::octet_length(source)
			;
		if (!source_length) {
			return tTJSVariant();
		}
		sprig::locale_saver saver;
		std::setlocale(LC_ALL, "");
		boost::shared_ptr<UCharsetDetector> detector = setupDetector(source_data, source_length);
		if (!detector) {
			return tTJSVariant();
		}
		//	COMMENT: 検出
		UErrorCode error = U_ZERO_ERROR;
		int32_t num_found = 0;
		UCharsetMatch const** matchers = ::ucsdet_detectAll(detector.get(), &num_found, &error);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucsdet_detectAll: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return tTJSVariant();
		}
		sprig::krkr::tjs::object_type result(
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				0, 0, 0
				),
			false
			);
		for (int32_t i = 0; i < num_found; ++i) {
			tTJSVariant var = makeMatchResult(matchers[i], confidence_threshold);
			if (var.Type() == tvtVoid) {
				break;
			}
			sprig::krkr::tjs::AddMember(sprig::get_pointer(result), i, var);
		}
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE bool NativeEncoding::checkDetected(
		tjs_char const* charset_name,
		tTJSVariantOctet const* source,
		size_type length,
		int_type confidence_threshold
		)
	{
		tjs_uint8 const* source_data = sprig::krkr::tjs::octet_data(source);
		tjs_uint source_length = length
			? std::min<tjs_uint>(length, sprig::krkr::tjs::octet_length(source))
			: sprig::krkr::tjs::octet_length(source)
			;
		if (!source_length) {
			return false;
		}
		sprig::locale_saver saver;
		std::setlocale(LC_ALL, "");
		boost::shared_ptr<UCharsetDetector> detector = setupDetector(source_data, source_length);
		if (!detector) {
			return false;
		}
		//	COMMENT: 検出
		UErrorCode error = U_ZERO_ERROR;
		int32_t num_found = 0;
		UCharsetMatch const** matchers = ::ucsdet_detectAll(detector.get(), &num_found, &error);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucsdet_detectAll: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		std::string charset = sprig::str_cast<std::string>(charset_name);
		char const* charset_str = charset.c_str();
		for (int32_t i = 0; i < num_found; ++i) {
			if (confidence_threshold) {
				//	COMMENT: コンフィデンス
				int32_t confidence = ::ucsdet_getConfidence(matchers[i], &error);
				if (U_FAILURE(error)) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						tTJSString(SPRIG_KRKR_TJS_W("error in ucsdet_getConfidence: ")) + ::u_errorName(error),
						sprig::krkr::internal_error
						);
					return false;
				}
				if (confidence < confidence_threshold) {
					break;
				}
			}
			//	COMMENT: 名前
			char const* name = ::ucsdet_getName(matchers[i], &error);
			if (U_FAILURE(error)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					tTJSString(SPRIG_KRKR_TJS_W("error in ucsdet_getName: ")) + ::u_errorName(error),
					sprig::krkr::internal_error
					);
				return false;
			}
			if (::ucnv_compareNames(name, charset_str) == 0) {
				return true;
			}
		}
		return false;
	}

	//
	// Encoding
	//
	Encoding::Encoding() {}
	tjs_error TJS_INTF_METHOD Encoding::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Encoding::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeEncoding>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Encoding::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Encoding::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: URLエンコード系メソッド
	//
	KTL_INLINE tTJSVariant Encoding::URLEncodeToAsciiOctet(
		tTJSVariantString const* charset,
		tTJSVariantString const* source,
		tTVInteger flag
		)
	{
		return NativeEncoding::URLEncodeToAsciiOctet(
			sprig::krkr::tjs::as_c_str(charset),
			sprig::krkr::tjs::as_c_str(source),
			sprig::numeric::bit_cast<NativeEncoding::flag_type>(flag)
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLEncodeToAsciiOctet(
		tTJSVariantString const* charset,
		tTJSVariantOctet const* source,
		tTJSVariantString const* source_charset,
		tTVInteger flag
		)
	{
		return NativeEncoding::URLEncodeToAsciiOctet(
			sprig::krkr::tjs::as_c_str(charset),
			source,
			sprig::krkr::tjs::as_c_str(source_charset),
			sprig::numeric::bit_cast<NativeEncoding::flag_type>(flag)
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLEncodeToAsciiOctet(
		tTJSVariantOctet const* source,
		tTVInteger flag
		)
	{
		return NativeEncoding::URLEncodeToAsciiOctet(
			source,
			sprig::numeric::bit_cast<NativeEncoding::flag_type>(flag)
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLEncodeToString(
		tTJSVariantString const* charset,
		tTJSVariantString const* source,
		tTVInteger flag
		)
	{
		return NativeEncoding::URLEncodeToString(
			sprig::krkr::tjs::as_c_str(charset),
			sprig::krkr::tjs::as_c_str(source),
			sprig::numeric::bit_cast<NativeEncoding::flag_type>(flag)
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLEncodeToString(
		tTJSVariantString const* charset,
		tTJSVariantOctet const* source,
		tTJSVariantString const* source_charset,
		tTVInteger flag
		)
	{
		return NativeEncoding::URLEncodeToString(
			sprig::krkr::tjs::as_c_str(charset),
			source,
			sprig::krkr::tjs::as_c_str(source_charset),
			sprig::numeric::bit_cast<NativeEncoding::flag_type>(flag)
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLEncodeToString(
		tTJSVariantOctet const* source,
		tTVInteger flag
		)
	{
		return NativeEncoding::URLEncodeToString(
			source,
			sprig::numeric::bit_cast<NativeEncoding::flag_type>(flag)
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLDecodeToOctetData(
		tTJSVariantString const* source
		)
	{
		return NativeEncoding::URLDecodeToOctetData(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLDecodeToOctetData(
		tTJSVariantOctet const* source,
		tTJSVariantString const* source_charset
		)
	{
		return NativeEncoding::URLDecodeToOctetData(
			source,
			sprig::krkr::tjs::as_c_str(source_charset)
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLDecodeToOctetData(
		tTJSVariantOctet const* source
		)
	{
		return NativeEncoding::URLDecodeToOctetData(
			source
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLDecodeToString(
		tTJSVariantString const* charset,
		tTJSVariantString const* source
		)
	{
		return NativeEncoding::URLDecodeToString(
			sprig::krkr::tjs::as_c_str(charset),
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLDecodeToString(
		tTJSVariantString const* charset,
		tTJSVariantOctet const* source,
		tTJSVariantString const* source_charset
		)
	{
		return NativeEncoding::URLDecodeToString(
			sprig::krkr::tjs::as_c_str(charset),
			source,
			sprig::krkr::tjs::as_c_str(source_charset)
			);
	}
	KTL_INLINE tTJSVariant Encoding::URLDecodeToString(
		tTJSVariantString const* charset,
		tTJSVariantOctet const* source
		)
	{
		return NativeEncoding::URLDecodeToString(
			sprig::krkr::tjs::as_c_str(charset),
			source
			);
	}
	//
	//	SUMMARY: Base64エンコード系メソッド
	//
	KTL_INLINE tTJSVariant Encoding::base64EncodeToAsciiOctet(
		tTJSVariantOctet const* source,
		tTVInteger line_max_length
		)
	{
		return NativeEncoding::base64EncodeToAsciiOctet(
			source,
			boost::numeric_cast<NativeEncoding::size_type>(line_max_length)
			);
	}
	KTL_INLINE tTJSVariant Encoding::base64EncodeToString(
		tTJSVariantOctet const* source,
		tTVInteger line_max_length
		)
	{
		return NativeEncoding::base64EncodeToString(
			source,
			boost::numeric_cast<NativeEncoding::size_type>(line_max_length)
			);
	}
	KTL_INLINE tTJSVariant Encoding::base64DecodeToOctetData(
		tTJSVariantString const* source
		)
	{
		return NativeEncoding::base64DecodeToOctetData(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE tTJSVariant Encoding::base64DecodeToOctetData(
		tTJSVariantOctet const* source
		)
	{
		return NativeEncoding::base64DecodeToOctetData(
			source
			);
	}
	//
	//	SUMMARY: エンコード系メソッド
	//
	KTL_INLINE tTJSVariant Encoding::encodeToOctet(
		tTJSVariantString const* charset,
		tTJSVariantString const* source
		)
	{
		return NativeEncoding::encodeToOctet(
			sprig::krkr::tjs::as_c_str(charset),
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE tTJSVariant Encoding::encodeToOctet(
		tTJSVariantString const* charset,
		tTJSVariantOctet const* source,
		tTJSVariantString const* source_charset
		)
	{
		return NativeEncoding::encodeToOctet(
			sprig::krkr::tjs::as_c_str(charset),
			source,
			sprig::krkr::tjs::as_c_str(source_charset)
			);
	}
	KTL_INLINE tTJSVariant Encoding::encodeToString(
		tTJSVariantOctet const* source,
		tTJSVariantString const* source_charset
		)
	{
		return NativeEncoding::encodeToString(
			source,
			sprig::krkr::tjs::as_c_str(source_charset)
			);
	}
	//
	//	SUMMARY: 比較系メソッド
	//
	KTL_INLINE bool Encoding::sameCharsetAlias(
		tTJSVariantString const* name1,
		tTJSVariantString const* name2
		)
	{
		return NativeEncoding::sameCharsetAlias(
			sprig::krkr::tjs::as_c_str(name1),
			sprig::krkr::tjs::as_c_str(name2)
			);
	}
	//
	//	SUMMARY: 検出系メソッド
	//
	KTL_INLINE tTJSVariant Encoding::detect(
		tTJSVariantOctet const* source,
		tTVInteger length,
		tTVInteger confidence_threshold
		)
	{
		return NativeEncoding::detect(
			source,
			boost::numeric_cast<NativeEncoding::size_type>(length),
			boost::numeric_cast<NativeEncoding::int_type>(confidence_threshold)
			);
	}
	KTL_INLINE tTJSVariant Encoding::detectAll(
		tTJSVariantOctet const* source,
		tTVInteger length,
		tTVInteger confidence_threshold
		)
	{
		return NativeEncoding::detectAll(
			source,
			boost::numeric_cast<NativeEncoding::size_type>(length),
			boost::numeric_cast<NativeEncoding::int_type>(confidence_threshold)
			);
	}
	KTL_INLINE bool Encoding::checkDetected(
		tTJSVariantString const* charset_name,
		tTJSVariantOctet const* source,
		tTVInteger length,
		tTVInteger confidence_threshold
		)
	{
		return NativeEncoding::checkDetected(
			sprig::krkr::tjs::as_c_str(charset_name),
			source,
			boost::numeric_cast<NativeEncoding::size_type>(length),
			boost::numeric_cast<NativeEncoding::int_type>(confidence_threshold)
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_ENCODING_ENCODING_HPP
