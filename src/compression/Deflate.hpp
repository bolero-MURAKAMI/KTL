/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_COMPRESSION_DEFLATE_HPP
#define SRC_COMPRESSION_DEFLATE_HPP

#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/utility/swap.hpp>
#include <boost/iostreams/write.hpp>
#include <boost/iostreams/put.hpp>
#include <boost/iostreams/flush.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/str_length.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "DeflateDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Deflate:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Deflate:error")

namespace ktl {
	//
	// NativeDeflate
	//
	KTL_INLINE NativeDeflate::size_type NativeDeflate::getEnableSize(size_type length, size_type spos) {
		return spos <= length
			? length - spos
			: 0
			;
	}
	KTL_INLINE NativeDeflate::size_type NativeDeflate::getEnableSize(size_type length, size_type spos, size_type ssize) {
		return spos + ssize <= length
			? ssize
			: spos <= length
				? length - spos
				: 0
			;
	}
	KTL_INLINE bool NativeDeflate::loadStorage(binary_type& binary, tjs_char const* storage) {
		sprig::com_ptr<IStream> in(::TVPCreateIStream(storage, TJS_BS_READ));
		if (!in) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		LARGE_INTEGER move;
		move.QuadPart = 0;
		ULARGE_INTEGER new_pos;
		if (FAILED(in->Seek(move, TJS_BS_SEEK_END, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		size_type file_size = static_cast<size_type>(new_pos.QuadPart);
		if (!file_size) {
			binary.clear();
			return true;
		}
		move.QuadPart = 0;
		if (FAILED(in->Seek(move, TJS_BS_SEEK_SET, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		binary.resize(file_size);
		ULONG io_size = 0;
		if (FAILED(in->Read(&binary[0], file_size, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeDeflate::loadStorage(binary_type& binary, tjs_char const* storage, size_type spos) {
		sprig::com_ptr<IStream> in(::TVPCreateIStream(storage, TJS_BS_READ));
		if (!in) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		LARGE_INTEGER move;
		move.QuadPart = 0;
		ULARGE_INTEGER new_pos;
		if (FAILED(in->Seek(move, TJS_BS_SEEK_END, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		size_type file_size = static_cast<size_type>(new_pos.QuadPart);
		if (!file_size || spos >= file_size) {
			binary.clear();
			return true;
		}
		move.QuadPart = spos;
		if (FAILED(in->Seek(move, TJS_BS_SEEK_SET, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		size_type result_size = file_size - spos;
		binary.resize(result_size);
		ULONG io_size = 0;
		if (FAILED(in->Read(&binary[0], result_size, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeDeflate::loadStorage(binary_type& binary, tjs_char const* storage, size_type spos, size_type ssize) {
		sprig::com_ptr<IStream> in(::TVPCreateIStream(storage, TJS_BS_READ));
		if (!in) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		LARGE_INTEGER move;
		move.QuadPart = 0;
		ULARGE_INTEGER new_pos;
		if (FAILED(in->Seek(move, TJS_BS_SEEK_END, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		size_type file_size = static_cast<size_type>(new_pos.QuadPart);
		if (!file_size || spos >= file_size) {
			binary.clear();
			return true;
		}
		move.QuadPart = spos;
		if (FAILED(in->Seek(move, TJS_BS_SEEK_SET, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		size_type result_size = (std::min)(
			ssize,
			file_size - spos
			);
		binary.resize(result_size);
		ULONG io_size = 0;
		if (FAILED(in->Read(&binary[0], result_size, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	NativeDeflate::NativeDeflate() {}
	//
	//	SUMMARY: 圧縮レベル系フラグ
	//
	int const NativeDeflate::clfNoCompression = boost::iostreams::zlib::no_compression;
	int const NativeDeflate::clfBestSpeed = boost::iostreams::zlib::best_speed;
	int const NativeDeflate::clfBestCompression = boost::iostreams::zlib::best_compression;
	int const NativeDeflate::clfDefaultCompression = boost::iostreams::zlib::default_compression;
	//
	//	SUMMARY: 圧縮メソッド系フラグ
	//
	int const NativeDeflate::cmfDeflated = boost::iostreams::zlib::deflated;
	//
	//	SUMMARY: 圧縮ストラテジー系フラグ
	//
	int const NativeDeflate::csfDefaultStrategy = boost::iostreams::zlib::default_strategy;
	int const NativeDeflate::csfFiltered = boost::iostreams::zlib::filtered;
	int const NativeDeflate::csfHuffmanOnly = boost::iostreams::zlib::huffman_only;
	//
	//	SUMMARY: セットアップ系メソッド
	//
	KTL_INLINE void NativeDeflate::reset() {
		params_type params;
		binary_type binary;
		filter_instance_type filter;
		stream_instance_type stream;
		boost::swap(stream_, stream);
		boost::swap(filter_, filter);
		boost::swap(binary_, binary);
		boost::swap(params_, params);
	}
	KTL_INLINE bool NativeDeflate::setup() {
		binary_type binary;
		boost::swap(binary_, binary);
		filter_instance_type filter(boost::make_shared<filter_type>(params_));
		stream_instance_type stream(boost::make_shared<stream_type>());
		stream->push(boost::ref(*filter));
		stream->push(boost::iostreams::back_inserter(binary_));
		boost::swap(stream_, stream);
		boost::swap(filter_, filter);
		return true;
	}
	KTL_INLINE bool NativeDeflate::setupCompleted() const {
		return stream_;
	}
	KTL_INLINE bool NativeDeflate::finish() {
		if (!stream_) {
			return false;
		}
		boost::iostreams::flush(*stream_);
		stream_instance_type stream;
		boost::swap(stream_, stream);
		return true;
	}
	KTL_INLINE bool NativeDeflate::flush() {
		if (!stream_) {
			return false;
		}
		boost::iostreams::flush(*stream_);
		return true;
	}
	//
	//	SUMMARY: パラメータ系メソッド
	//
	KTL_INLINE int NativeDeflate::getLevel() const {
		return params_.level;
	}
	KTL_INLINE void NativeDeflate::setLevel(int v) {
		params_.level = v;
	}
	KTL_INLINE int NativeDeflate::getMethod() const {
		return params_.method;
	}
	KTL_INLINE void NativeDeflate::setMethod(int v) {
		params_.method = v;
	}
	KTL_INLINE int NativeDeflate::getWindowBits() const {
		return params_.window_bits;
	}
	KTL_INLINE void NativeDeflate::setWindowBits(int v) {
		params_.window_bits = v;
	}
	KTL_INLINE int NativeDeflate::getMemLevel() const {
		return params_.mem_level;
	}
	KTL_INLINE void NativeDeflate::setMemLevel(int v) {
		params_.mem_level = v;
	}
	KTL_INLINE int NativeDeflate::getStrategy() const {
		return params_.strategy;
	}
	KTL_INLINE void NativeDeflate::setStrategy(int v) {
		params_.strategy = v;
	}
	KTL_INLINE bool NativeDeflate::getNoHeader() const {
		return params_.noheader;
	}
	KTL_INLINE void NativeDeflate::setNoHeader(bool v) {
		params_.noheader = v;
	}
	KTL_INLINE bool NativeDeflate::getCalculateCRC() const {
		return params_.calculate_crc;
	}
	KTL_INLINE void NativeDeflate::setCalculateCRC(bool v) {
		params_.calculate_crc = v;
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tTJSVariant NativeDeflate::CRC() {
		if (!filter_) {
			return tTJSVariant();
		}
		if (stream_) {
			boost::iostreams::flush(*stream_);
		}
		return static_cast<tTVInteger>(filter_->crc());
	}
	KTL_INLINE tTJSVariant NativeDeflate::originalSize() {
		if (!filter_) {
			return tTJSVariant();
		}
		if (stream_) {
			boost::iostreams::flush(*stream_);
		}
		return static_cast<tTVInteger>(filter_->total_in());
	}
	//
	//	SUMMARY: 処理系メソッド
	//
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processBlock(byte_type const* first, byte_type const* last) {
		return processBytes(first, last - first);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processBytes(byte_type const* data, size_type size) {
		if (!stream_) {
			return -1;
		}
		if (!size) {
			return size;
		}
		return static_cast<NativeDeflate::int_type>(boost::iostreams::write(*stream_, data, size));
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processByte(byte_type byte) {
		if (!stream_) {
			return -1;
		}
		return boost::iostreams::put(*stream_, byte)
			? 1
			: -1
			;
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processOctet(tTJSVariantOctet const* source) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		if (!length) {
			return length;
		}
		return processBytes(
			reinterpret_cast<byte_type const*>(data),
			length
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processOctet(tTJSVariantOctet const* source, size_type spos) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return processBytes(
			reinterpret_cast<byte_type const*>(data + spos),
			result_size
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return processBytes(
			reinterpret_cast<byte_type const*>(data + spos),
			result_size
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processString(tjs_char const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		if (!length) {
			return length;
		}
		return processBytes(
			data,
			length
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processString(tjs_char const* source, size_type spos) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return processBytes(
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processString(tjs_char const* source, size_type spos, size_type ssize) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return processBytes(
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processNarrowString(tjs_nchar const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		if (!length) {
			return length;
		}
		return processBytes(
			data,
			length
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processNarrowString(tjs_nchar const* source, size_type spos) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return processBytes(
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processNarrowString(tjs_nchar const* source, size_type spos, size_type ssize) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return processBytes(
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processStorage(tjs_char const* storage) {
		if (!stream_) {
			return -1;
		}
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return -1;
		}
		if (!binary.size()) {
			return binary.size();
		}
		return processBytes(
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processStorage(tjs_char const* storage, size_type spos) {
		if (!stream_) {
			return -1;
		}
		binary_type binary;
		if (!loadStorage(binary, storage, spos)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return -1;
		}
		if (!binary.size()) {
			return binary.size();
		}
		return processBytes(
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::processStorage(tjs_char const* storage, size_type spos, size_type ssize) {
		if (!stream_) {
			return -1;
		}
		binary_type binary;
		if (!loadStorage(binary, storage, spos, ssize)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return -1;
		}
		if (!binary.size()) {
			return binary.size();
		}
		return processBytes(
			&binary[0],
			binary.size()
			);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE NativeDeflate::size_type NativeDeflate::size() const {
		if (stream_) {
			return 0;
		}
		return binary_.size();
	}
	KTL_INLINE tTJSVariant NativeDeflate::asOctet() const {
		if (stream_) {
			return tTJSVariant();
		}
		size_type result_size = binary_.size();
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&binary_[0]), result_size);
	}
	KTL_INLINE tTJSVariant NativeDeflate::asOctet(size_type pos) const {
		if (stream_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(binary_.size(), pos);
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&binary_[pos]), result_size);
	}
	KTL_INLINE tTJSVariant NativeDeflate::asOctet(size_type pos, size_type size) const {
		if (stream_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(binary_.size(), pos, size);
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&binary_[pos]), result_size);
	}
	//
	//	SUMMARY: ストレージ系メソッド
	//
	KTL_INLINE NativeDeflate::int_type NativeDeflate::saveStorage(tjs_char const* storage) const {
		if (stream_) {
			return -1;
		}
		sprig::com_ptr<IStream> out(::TVPCreateIStream(storage, TJS_BS_WRITE));
		if (!out) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return -1;
		}
		size_type result_size = binary_.size();
		if (!result_size) {
			return result_size;
		}
		ULONG io_size = 0;
		if (FAILED(out->Write(&binary_[0], binary_.size(), &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ファイル書込に失敗しました"),
				sprig::krkr::internal_error
				);
			return io_size;
		}
		return io_size;
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::saveStorage(tjs_char const* storage, size_type pos) const {
		if (stream_) {
			return -1;
		}
		sprig::com_ptr<IStream> out(::TVPCreateIStream(storage, TJS_BS_WRITE));
		if (!out) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return -1;
		}
		size_type result_size = getEnableSize(binary_.size(), pos);
		if (!result_size) {
			return result_size;
		}
		ULONG io_size = 0;
		if (FAILED(out->Write(&binary_[pos], result_size, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ファイル書込に失敗しました"),
				sprig::krkr::internal_error
				);
			return io_size;
		}
		return io_size;
	}
	KTL_INLINE NativeDeflate::int_type NativeDeflate::saveStorage(tjs_char const* storage, size_type pos, size_type size) const {
		if (stream_) {
			return -1;
		}
		sprig::com_ptr<IStream> out(::TVPCreateIStream(storage, TJS_BS_WRITE));
		if (!out) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return -1;
		}
		size_type result_size = getEnableSize(binary_.size(), pos, size);
		if (!result_size) {
			return result_size;
		}
		ULONG io_size = 0;
		if (FAILED(out->Write(&binary_[pos], result_size, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ファイル書込に失敗しました"),
				sprig::krkr::internal_error
				);
			return io_size;
		}
		return io_size;
	}

	//
	// Deflate
	//
	Deflate::Deflate() {}
	tjs_error TJS_INTF_METHOD Deflate::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Deflate::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeDeflate>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Deflate::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Deflate::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 圧縮レベル系フラグ
	//
	tTVInteger const Deflate::clfNoCompression = NativeDeflate::clfNoCompression;
	tTVInteger const Deflate::clfBestSpeed = NativeDeflate::clfBestSpeed;
	tTVInteger const Deflate::clfBestCompression = NativeDeflate::clfBestCompression;
	tTVInteger const Deflate::clfDefaultCompression = NativeDeflate::clfDefaultCompression;
	//
	//	SUMMARY: 圧縮メソッド系フラグ
	//
	tTVInteger const Deflate::cmfDeflated = NativeDeflate::cmfDeflated;
	//
	//	SUMMARY: 圧縮ストラテジー系フラグ
	//
	tTVInteger const Deflate::csfDefaultStrategy = NativeDeflate::csfDefaultStrategy;
	tTVInteger const Deflate::csfFiltered = NativeDeflate::csfFiltered;
	tTVInteger const Deflate::csfHuffmanOnly = NativeDeflate::csfHuffmanOnly;
	//
	//	SUMMARY: セットアップ系メソッド
	//
	KTL_INLINE void Deflate::reset() {
		instance_->reset();
	}
	KTL_INLINE bool Deflate::setup() {
		return instance_->setup();
	}
	KTL_INLINE bool Deflate::setupCompleted() const {
		return instance_->setupCompleted();
	}
	KTL_INLINE bool Deflate::finish() {
		return instance_->finish();
	}
	KTL_INLINE bool Deflate::flush() {
		return instance_->flush();
	}
	//
	//	SUMMARY: パラメータ系メソッド
	//
	KTL_INLINE tTVInteger Deflate::getLevel() const {
		return instance_->getLevel();
	}
	KTL_INLINE void Deflate::setLevel(tTVInteger v) {
		instance_->setLevel(
			boost::numeric_cast<int>(v)
			);
	}
	KTL_INLINE tTVInteger Deflate::getMethod() const {
		return instance_->getMethod();
	}
	KTL_INLINE void Deflate::setMethod(tTVInteger v) {
		instance_->setMethod(
			boost::numeric_cast<int>(v)
			);
	}
	KTL_INLINE tTVInteger Deflate::getWindowBits() const {
		return instance_->getWindowBits();
	}
	KTL_INLINE void Deflate::setWindowBits(tTVInteger v) {
		instance_->setWindowBits(
			boost::numeric_cast<int>(v)
			);
	}
	KTL_INLINE tTVInteger Deflate::getMemLevel() const {
		return instance_->getMemLevel();
	}
	KTL_INLINE void Deflate::setMemLevel(tTVInteger v) {
		instance_->setMemLevel(
			boost::numeric_cast<int>(v)
			);
	}
	KTL_INLINE tTVInteger Deflate::getStrategy() const {
		return instance_->getStrategy();
	}
	KTL_INLINE void Deflate::setStrategy(tTVInteger v) {
		instance_->setStrategy(
			boost::numeric_cast<int>(v)
			);
	}
	KTL_INLINE bool Deflate::getNoHeader() const {
		return instance_->getNoHeader();
	}
	KTL_INLINE void Deflate::setNoHeader(bool v) {
		instance_->setNoHeader(v);
	}
	KTL_INLINE bool Deflate::getCalculateCRC() const {
		return instance_->getCalculateCRC();
	}
	KTL_INLINE void Deflate::setCalculateCRC(bool v) {
		instance_->setCalculateCRC(v);
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tTJSVariant Deflate::CRC() {
		return instance_->CRC();
	}
	KTL_INLINE tTJSVariant Deflate::originalSize() {
		return instance_->originalSize();
	}
	//
	//	SUMMARY: 処理系メソッド
	//
	KTL_INLINE tTVInteger Deflate::processBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last) {
		return instance_->processBlock(
			reinterpret_cast<NativeDeflate::byte_type const*>(first),
			reinterpret_cast<NativeDeflate::byte_type const*>(last)
			);
	}
	KTL_INLINE tTVInteger Deflate::processBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size) {
		return instance_->processBytes(
			reinterpret_cast<NativeDeflate::byte_type const*>(data),
			boost::numeric_cast<NativeDeflate::size_type>(size)
			);
	}
	KTL_INLINE tTVInteger Deflate::processByte(tTVInteger byte) {
		return instance_->processByte(
			static_cast<NativeDeflate::byte_type>(byte)
			);
	}
	KTL_INLINE tTVInteger Deflate::processOctet(tTJSVariantOctet const* source) {
		return instance_->processOctet(source);
	}
	KTL_INLINE tTVInteger Deflate::processOctet(tTJSVariantOctet const* source, tTVInteger spos) {
		return instance_->processOctet(
			source,
			boost::numeric_cast<NativeDeflate::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger Deflate::processOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->processOctet(
			source,
			boost::numeric_cast<NativeDeflate::size_type>(spos),
			boost::numeric_cast<NativeDeflate::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger Deflate::processString(tTJSVariantString const* source) {
		return instance_->processString(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE tTVInteger Deflate::processString(tTJSVariantString const* source, tTVInteger spos) {
		return instance_->processString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeDeflate::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger Deflate::processString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->processString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeDeflate::size_type>(spos),
			boost::numeric_cast<NativeDeflate::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger Deflate::processNarrowString(tTJSVariantString const* source) {
		return instance_->processNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
			);
	}
	KTL_INLINE tTVInteger Deflate::processNarrowString(tTJSVariantString const* source, tTVInteger spos) {
		return instance_->processNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeDeflate::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger Deflate::processNarrowString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->processNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeDeflate::size_type>(spos),
			boost::numeric_cast<NativeDeflate::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger Deflate::processStorage(tTJSVariantString const* storage) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger Deflate::processStorage(tTJSVariantString const* storage, tTVInteger spos) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeDeflate::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger Deflate::processStorage(tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeDeflate::size_type>(spos),
			boost::numeric_cast<NativeDeflate::size_type>(ssize)
			);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE tTVInteger Deflate::size() const {
		return instance_->size();
	}
	KTL_INLINE tTJSVariant Deflate::asOctet() const {
		return instance_->asOctet();
	}
	KTL_INLINE tTJSVariant Deflate::asOctet(tTVInteger pos) const {
		return instance_->asOctet(
			boost::numeric_cast<NativeDeflate::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Deflate::asOctet(tTVInteger pos, tTVInteger size) const {
		return instance_->asOctet(
			boost::numeric_cast<NativeDeflate::size_type>(pos),
			boost::numeric_cast<NativeDeflate::size_type>(size)
			);
	}
	//
	//	SUMMARY: ストレージ系メソッド
	//
	KTL_INLINE tTVInteger Deflate::saveStorage(tTJSVariantString const* storage) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger Deflate::saveStorage(tTJSVariantString const* storage, tTVInteger pos) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeDeflate::size_type>(pos)
			);
	}
	KTL_INLINE tTVInteger Deflate::saveStorage(tTJSVariantString const* storage, tTVInteger pos, tTVInteger size) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeDeflate::size_type>(pos),
			boost::numeric_cast<NativeDeflate::size_type>(size)
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_COMPRESSION_DEFLATE_HPP
