#pragma once

#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

#include "ChecksumCommon.hpp"

namespace ktl {
	//
	// NativeChecksum
	//
	template<typename Impl>
	class NativeChecksum {
	public:
		typedef Impl impl_type;
		typedef typename impl_type::value_type value_type;
		typedef ChecksumUtils::byte_type byte_type;
		typedef ChecksumUtils::size_type size_type;
		typedef typename ChecksumUtils::octet_of<value_type>::type octet_type;
		typedef std::vector<byte_type> binary_type;
	public:
		static size_type getEnableSize(size_type length, size_type spos);
		static size_type getEnableSize(size_type length, size_type spos, size_type ssize);
		static bool loadStorage(binary_type& binary, tjs_char const* storage);
		static bool loadStorage(binary_type& binary, tjs_char const* storage, size_type spos);
		static bool loadStorage(binary_type& binary, tjs_char const* storage, size_type spos, size_type ssize);
	private:
		impl_type impl_;
	public:
		NativeChecksum();
	public:
		//
		//	SUMMARY: 計算系メソッド
		//
		static value_type calculateBlock(byte_type const* first, byte_type const* last);
		static value_type calculateBytes(byte_type const* data, size_type size);
		static value_type calculateOctet(tTJSVariantOctet const* source);
		static value_type calculateString(tjs_char const* source);
		static value_type calculateNarrowString(tjs_nchar const* source);
		static value_type calculateStorage(tjs_char const* storage);
		static octet_type calculateBlockAsOctet(byte_type const* first, byte_type const* last);
		static octet_type calculateBytesAsOctet(byte_type const* data, size_type size);
		static octet_type calculateOctetAsOctet(tTJSVariantOctet const* source);
		static octet_type calculateStringAsOctet(tjs_char const* source);
		static octet_type calculateNarrowStringAsOctet(tjs_nchar const* source);
		static octet_type calculateStorageAsOctet(tjs_char const* storage);
		static tTJSVariant calculateBlockAsHex(byte_type const* first, byte_type const* last, bool upper = false);
		static tTJSVariant calculateBytesAsHex(byte_type const* data, size_type size, bool upper = false);
		static tTJSVariant calculateOctetAsHex(tTJSVariantOctet const* source, bool upper = false);
		static tTJSVariant calculateStringAsHex(tjs_char const* source, bool upper = false);
		static tTJSVariant calculateNarrowStringAsHex(tjs_nchar const* source, bool upper = false);
		static tTJSVariant calculateStorageAsHex(tjs_char const* storage, bool upper = false);
	public:
		//
		//	SUMMARY: 処理系メソッド
		//
		void reset();
		size_type processBlock(byte_type const* first, byte_type const* last);
		size_type processBytes(byte_type const* data, size_type size);
		size_type processByte(byte_type byte);
		size_type processOctet(tTJSVariantOctet const* source);
		size_type processOctet(tTJSVariantOctet const* source, size_type spos);
		size_type processOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize);
		size_type processString(tjs_char const* source);
		size_type processString(tjs_char const* source, size_type spos);
		size_type processString(tjs_char const* source, size_type spos, size_type ssize);
		size_type processNarrowString(tjs_nchar const* source);
		size_type processNarrowString(tjs_nchar const* source, size_type spos);
		size_type processNarrowString(tjs_nchar const* source, size_type spos, size_type ssize);
		size_type processStorage(tjs_char const* storage);
		size_type processStorage(tjs_char const* storage, size_type spos);
		size_type processStorage(tjs_char const* storage, size_type spos, size_type ssize);
		//
		//	SUMMARY: 取得系メソッド
		//
		value_type checksum() const;
		octet_type checksumOctet() const;
		tTJSVariant checksumHex(bool upper = false) const;
	};

	//
	// Checksum
	//
	template<typename Impl>
	class Checksum
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeChecksum<Impl> > instance_;
	public:
		Checksum();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: 計算系メソッド
		//
		static tTJSVariant calculateBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last);
		static tTJSVariant calculateBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size);
		static tTJSVariant calculateOctet(tTJSVariantOctet const* source);
		static tTJSVariant calculateString(tTJSVariantString const* source);
		static tTJSVariant calculateNarrowString(tTJSVariantString const* source);
		static tTJSVariant calculateStorage(tTJSVariantString const* storage);
		static tTJSVariant calculateBlockAsOctet(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last);
		static tTJSVariant calculateBytesAsOctet(sprig::krkr::tjs::intptr_type data, tTVInteger size);
		static tTJSVariant calculateOctetAsOctet(tTJSVariantOctet const* source);
		static tTJSVariant calculateStringAsOctet(tTJSVariantString const* source);
		static tTJSVariant calculateNarrowStringAsOctet(tTJSVariantString const* source);
		static tTJSVariant calculateStorageAsOctet(tTJSVariantString const* storage);
		static tTJSVariant calculateBlockAsHex(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last, bool upper = false);
		static tTJSVariant calculateBytesAsHex(sprig::krkr::tjs::intptr_type data, tTVInteger size, bool upper = false);
		static tTJSVariant calculateOctetAsHex(tTJSVariantOctet const* source, bool upper = false);
		static tTJSVariant calculateStringAsHex(tTJSVariantString const* source, bool upper = false);
		static tTJSVariant calculateNarrowStringAsHex(tTJSVariantString const* source, bool upper = false);
		static tTJSVariant calculateStorageAsHex(tTJSVariantString const* storage, bool upper = false);
	public:
		//
		//	SUMMARY: 処理系メソッド
		//
		void reset();
		tTVInteger processBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last);
		tTVInteger processBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size);
		tTVInteger processByte(tTVInteger byte);
		tTVInteger processOctet(tTJSVariantOctet const* source);
		tTVInteger processOctet(tTJSVariantOctet const* source, tTVInteger spos);
		tTVInteger processOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger processString(tTJSVariantString const* source);
		tTVInteger processString(tTJSVariantString const* source, tTVInteger spos);
		tTVInteger processString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger processNarrowString(tTJSVariantString const* source);
		tTVInteger processNarrowString(tTJSVariantString const* source, tTVInteger spos);
		tTVInteger processNarrowString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger processStorage(tTJSVariantString const* storage);
		tTVInteger processStorage(tTJSVariantString const* storage, tTVInteger spos);
		tTVInteger processStorage(tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize);
		//
		//	SUMMARY: 取得系メソッド
		//
		tTJSVariant checksum() const;
		tTJSVariant checksumOctet() const;
		tTJSVariant checksumHex(bool upper = false) const;
	};
}	// namespace ktl
