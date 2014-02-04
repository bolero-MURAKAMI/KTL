/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_BZIP2_BZIP2_COMPRESSOR_DECL_HPP
#define SRC_BZIP2_BZIP2_COMPRESSOR_DECL_HPP

#include <memory>
#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/cstdint.hpp>
#include <sprig/external/bzlib.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeBzip2Compressor
	//
	class NativeBzip2Compressor {
	public:
		typedef char byte_type;
		typedef std::streamsize streamsize_type;
		typedef boost::int_fast32_t int_type;
		typedef std::vector<byte_type> binary_type;
		typedef binary_type::size_type size_type;
		typedef boost::iostreams::filtering_stream<boost::iostreams::output, byte_type> stream_type;
		typedef boost::shared_ptr<stream_type> stream_instance_type;
		typedef boost::iostreams::basic_bzip2_compressor<std::allocator<byte_type> > filter_type;
		typedef boost::shared_ptr<filter_type> filter_instance_type;
		typedef boost::iostreams::bzip2_params params_type;
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
		NativeBzip2Compressor();
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
		int getBlockSize() const;
		void setBlockSize(int v);
		int getWorkFactor() const;
		void setWorkFactor(int v);
		//
		//	SUMMARY: 処理系メソッド
		//
		int_type processBlock(byte_type const* first, byte_type const* last);
		int_type processBytes(byte_type const* data, size_type size);
		int_type processByte(byte_type byte);
		int_type processOctet(tTJSVariantOctet const* source);
		int_type processOctet(tTJSVariantOctet const* source, size_type spos);
		int_type processOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize);
		int_type processString(tjs_char const* source);
		int_type processString(tjs_char const* source, size_type spos);
		int_type processString(tjs_char const* source, size_type spos, size_type ssize);
		int_type processNarrowString(tjs_nchar const* source);
		int_type processNarrowString(tjs_nchar const* source, size_type spos);
		int_type processNarrowString(tjs_nchar const* source, size_type spos, size_type ssize);
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
		//
		//	SUMMARY: ストレージ系メソッド
		//
		int_type saveStorage(tjs_char const* storage) const;
		int_type saveStorage(tjs_char const* storage, size_type pos) const;
		int_type saveStorage(tjs_char const* storage, size_type pos, size_type size) const;
	};

	//
	// Bzip2Compressor
	//
	class Bzip2Compressor
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeBzip2Compressor> instance_;
	public:
		Bzip2Compressor();
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
		tTVInteger getBlockSize() const;
		void setBlockSize(tTVInteger v);
		tTVInteger getWorkFactor() const;
		void setWorkFactor(tTVInteger v);
		//
		//	SUMMARY: 処理系メソッド
		//
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
		tTVInteger size() const;
		tTJSVariant asOctet() const;
		tTJSVariant asOctet(tTVInteger pos) const;
		tTJSVariant asOctet(tTVInteger pos, tTVInteger size) const;
		//
		//	SUMMARY: ストレージ系メソッド
		//
		tTVInteger saveStorage(tTJSVariantString const* storage) const;
		tTVInteger saveStorage(tTJSVariantString const* storage, tTVInteger pos) const;
		tTVInteger saveStorage(tTJSVariantString const* storage, tTVInteger pos, tTVInteger size) const;
	};
}	// namespace ktl

#endif	// #ifndef SRC_BZIP2_BZIP2_COMPRESSOR_DECL_HPP
