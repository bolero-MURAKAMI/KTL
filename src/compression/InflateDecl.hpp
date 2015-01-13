/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_COMPRESSION_INFLATE_DECL_HPP
#define SRC_COMPRESSION_INFLATE_DECL_HPP

#include <memory>
#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/cstdint.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeInflate
	//
	class NativeInflate {
	public:
		typedef char byte_type;
		typedef std::streamsize streamsize_type;
		typedef boost::int_fast32_t int_type;
		typedef std::vector<byte_type> binary_type;
		typedef binary_type::size_type size_type;
		typedef boost::iostreams::filtering_stream<boost::iostreams::output, byte_type> stream_type;
		typedef boost::shared_ptr<stream_type> stream_instance_type;
		typedef boost::iostreams::basic_zlib_decompressor<std::allocator<byte_type> > filter_type;
		typedef boost::shared_ptr<filter_type> filter_instance_type;
		typedef boost::iostreams::zlib_params params_type;
	public:
		static size_type getEnableSize(size_type length, size_type spos);
		static size_type getEnableSize(size_type length, size_type spos, size_type ssize);
		static bool loadStorage(binary_type& binary, tjs_char const* storage);
		static bool loadStorage(binary_type& binary, tjs_char const* storage, size_type spos);
		static bool loadStorage(binary_type& binary, tjs_char const* storage, size_type spos, size_type ssize);
	private:
		params_type params_;
		binary_type binary_;
		filter_instance_type filter_;
		stream_instance_type stream_;
	public:
		NativeInflate();
	public:
		//
		//	SUMMARY: 圧縮レベル系フラグ
		//
		static int const clfNoCompression;
		static int const clfBestSpeed;
		static int const clfBestCompression;
		static int const clfDefaultCompression;
		//
		//	SUMMARY: 圧縮メソッド系フラグ
		//
		static int const cmfDeflated;
		//
		//	SUMMARY: 圧縮ストラテジー系フラグ
		//
		static int const csfDefaultStrategy;
		static int const csfFiltered;
		static int const csfHuffmanOnly;
	public:
		//
		//	SUMMARY: セットアップ系メソッド
		//
		void reset();
		bool setup();
		bool setupCompleted() const;
		bool finish();
		bool flush();
		//
		//	SUMMARY: パラメータ系メソッド
		//
		int getLevel() const;
		void setLevel(int v);
		int getMethod() const;
		void setMethod(int v);
		int getWindowBits() const;
		void setWindowBits(int v);
		int getMemLevel() const;
		void setMemLevel(int v);
		int getStrategy() const;
		void setStrategy(int v);
		bool getNoHeader() const;
		void setNoHeader(bool v);
		bool getCalculateCRC() const;
		void setCalculateCRC(bool v);
		//
		//	SUMMARY: プロパティ系メソッド
		//
		tTJSVariant CRC();
		tTJSVariant originalSize();
		//
		//	SUMMARY: 処理系メソッド
		//
		int_type processBlock(byte_type const* first, byte_type const* last);
		int_type processBytes(byte_type const* data, size_type size);
		int_type processByte(byte_type byte);
		int_type processOctet(tTJSVariantOctet const* source);
		int_type processOctet(tTJSVariantOctet const* source, size_type spos);
		int_type processOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize);
		int_type processStorage(tjs_char const* storage);
		int_type processStorage(tjs_char const* storage, size_type spos);
		int_type processStorage(tjs_char const* storage, size_type spos, size_type ssize);
		//
		//	SUMMARY: 取得系メソッド
		//
		size_type size() const;
		tTJSVariant asOctet() const;
		tTJSVariant asOctet(size_type pos) const;
		tTJSVariant asOctet(size_type pos, size_type size) const;
		tTJSVariant asString() const;
		tTJSVariant asString(size_type pos) const;
		tTJSVariant asString(size_type pos, size_type size) const;
		tTJSVariant asNarrowString() const;
		tTJSVariant asNarrowString(size_type pos) const;
		tTJSVariant asNarrowString(size_type pos, size_type size) const;
		//
		//	SUMMARY: ストレージ系メソッド
		//
		int_type saveStorage(tjs_char const* storage) const;
		int_type saveStorage(tjs_char const* storage, size_type pos) const;
		int_type saveStorage(tjs_char const* storage, size_type pos, size_type size) const;
	};

	//
	// Inflate
	//
	class Inflate
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeInflate> instance_;
	public:
		//
		//	SUMMARY: 圧縮レベル系フラグ
		//
		static tTVInteger const clfNoCompression;
		static tTVInteger const clfBestSpeed;
		static tTVInteger const clfBestCompression;
		static tTVInteger const clfDefaultCompression;
		//
		//	SUMMARY: 圧縮メソッド系フラグ
		//
		static tTVInteger const cmfDeflated;
		//
		//	SUMMARY: 圧縮ストラテジー系フラグ
		//
		static tTVInteger const csfDefaultStrategy;
		static tTVInteger const csfFiltered;
		static tTVInteger const csfHuffmanOnly;
	public:
		Inflate();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: セットアップ系メソッド
		//
		void reset();
		bool setup();
		bool setupCompleted() const;
		bool finish();
		bool flush();
		//
		//	SUMMARY: パラメータ系メソッド
		//
		tTVInteger getLevel() const;
		void setLevel(tTVInteger v);
		tTVInteger getMethod() const;
		void setMethod(tTVInteger v);
		tTVInteger getWindowBits() const;
		void setWindowBits(tTVInteger v);
		tTVInteger getMemLevel() const;
		void setMemLevel(tTVInteger v);
		tTVInteger getStrategy() const;
		void setStrategy(tTVInteger v);
		bool getNoHeader() const;
		void setNoHeader(bool v);
		bool getCalculateCRC() const;
		void setCalculateCRC(bool v);
		//
		//	SUMMARY: プロパティ系メソッド
		//
		tTJSVariant CRC();
		tTJSVariant originalSize();
		//
		//	SUMMARY: 処理系メソッド
		//
		tTVInteger processBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last);
		tTVInteger processBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size);
		tTVInteger processByte(tTVInteger byte);
		tTVInteger processOctet(tTJSVariantOctet const* source);
		tTVInteger processOctet(tTJSVariantOctet const* source, tTVInteger spos);
		tTVInteger processOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger processStorage(tTJSVariantString const* storage);
		tTVInteger processStorage(tTJSVariantString const* storage, tTVInteger spos);
		tTVInteger processStorage(tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize);
		//
		//	SUMMARY: 取得系メソッド
		//
		tTVInteger size() const;
		tTJSVariant asOctet() const;
		tTJSVariant asOctet(tTVInteger pos) const;
		tTJSVariant asOctet(tTVInteger pos, tTVInteger size) const;
		tTJSVariant asString() const;
		tTJSVariant asString(tTVInteger pos) const;
		tTJSVariant asString(tTVInteger pos, tTVInteger size) const;
		tTJSVariant asNarrowString() const;
		tTJSVariant asNarrowString(tTVInteger pos) const;
		tTJSVariant asNarrowString(tTVInteger pos, tTVInteger size) const;
		//
		//	SUMMARY: ストレージ系メソッド
		//
		tTVInteger saveStorage(tTJSVariantString const* storage) const;
		tTVInteger saveStorage(tTJSVariantString const* storage, tTVInteger pos) const;
		tTVInteger saveStorage(tTJSVariantString const* storage, tTVInteger pos, tTVInteger size) const;
	};
}	// namespace ktl

#endif	// #ifndef SRC_COMPRESSION_INFLATE_DECL_HPP
