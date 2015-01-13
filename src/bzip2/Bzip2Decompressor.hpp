/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_BZIP2_BZIP2_DECOMPRESSOR_HPP
#define SRC_BZIP2_BZIP2_DECOMPRESSOR_HPP

#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/utility/swap.hpp>
#include <boost/iostreams/write.hpp>
#include <boost/iostreams/put.hpp>
#include <boost/iostreams/flush.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <sprig/external/bzlib.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/str_length.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "Bzip2DecompressorDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Bzip2Decompressor:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Bzip2Decompressor:error")

namespace ktl {
	//
	// NativeBzip2Decompressor
	//
	KTL_INLINE NativeBzip2Decompressor::size_type NativeBzip2Decompressor::getEnableSize(size_type length, size_type spos) {
		return spos <= length
			? length - spos
			: 0
			;
	}
	KTL_INLINE NativeBzip2Decompressor::size_type NativeBzip2Decompressor::getEnableSize(size_type length, size_type spos, size_type ssize) {
		return spos + ssize <= length
			? ssize
			: spos <= length
				? length - spos
				: 0
			;
	}
	KTL_INLINE bool NativeBzip2Decompressor::loadStorage(binary_type& binary, tjs_char const* storage) {
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
	KTL_INLINE bool NativeBzip2Decompressor::loadStorage(binary_type& binary, tjs_char const* storage, size_type spos) {
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
	KTL_INLINE bool NativeBzip2Decompressor::loadStorage(binary_type& binary, tjs_char const* storage, size_type spos, size_type ssize) {
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
	NativeBzip2Decompressor::NativeBzip2Decompressor()
		: params_(boost::iostreams::bzip2::default_small)
	{}
	//
	//	SUMMARY: セットアップ系メソッド
	//
	KTL_INLINE void NativeBzip2Decompressor::reset() {
		params_type params(boost::iostreams::bzip2::default_small);
		binary_type binary;
		filter_instance_type filter;
		stream_instance_type stream;
		boost::swap(stream_, stream);
		boost::swap(filter_, filter);
		boost::swap(binary_, binary);
		boost::swap(params_, params);
	}
	KTL_INLINE bool NativeBzip2Decompressor::setup() {
		binary_type binary;
		boost::swap(binary_, binary);
		filter_instance_type filter(boost::make_shared<filter_type>(params_.small));
		stream_instance_type stream(boost::make_shared<stream_type>());
		stream->push(boost::ref(*filter));
		stream->push(boost::iostreams::back_inserter(binary_));
		boost::swap(stream_, stream);
		boost::swap(filter_, filter);
		return true;
	}
	KTL_INLINE bool NativeBzip2Decompressor::setupCompleted() const {
		return static_cast<bool>(stream_);
	}
	KTL_INLINE bool NativeBzip2Decompressor::finish() {
		if (!stream_) {
			return false;
		}
		boost::iostreams::flush(*stream_);
		stream_instance_type stream;
		boost::swap(stream_, stream);
		return true;
	}
	KTL_INLINE bool NativeBzip2Decompressor::flush() {
		if (!stream_) {
			return false;
		}
		boost::iostreams::flush(*stream_);
		return true;
	}
	//
	//	SUMMARY: パラメータ系メソッド
	//
	KTL_INLINE bool NativeBzip2Decompressor::getSmall() const {
		return params_.small;
	}
	KTL_INLINE void NativeBzip2Decompressor::setSmall(bool v) {
		params_.small = v;
	}
	//
	//	SUMMARY: 処理系メソッド
	//
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::processBlock(byte_type const* first, byte_type const* last) {
		return processBytes(first, last - first);
	}
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::processBytes(byte_type const* data, size_type size) {
		if (!stream_) {
			return -1;
		}
		if (!size) {
			return size;
		}
		return static_cast<NativeBzip2Decompressor::int_type>(boost::iostreams::write(*stream_, data, size));
	}
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::processByte(byte_type byte) {
		if (!stream_) {
			return -1;
		}
		return boost::iostreams::put(*stream_, byte)
			? 1
			: -1
			;
	}
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::processOctet(tTJSVariantOctet const* source) {
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
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::processOctet(tTJSVariantOctet const* source, size_type spos) {
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
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::processOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize) {
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
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::processStorage(tjs_char const* storage) {
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
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::processStorage(tjs_char const* storage, size_type spos) {
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
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::processStorage(tjs_char const* storage, size_type spos, size_type ssize) {
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
	KTL_INLINE NativeBzip2Decompressor::size_type NativeBzip2Decompressor::size() const {
		if (stream_) {
			return 0;
		}
		return binary_.size();
	}
	KTL_INLINE tTJSVariant NativeBzip2Decompressor::asOctet() const {
		if (stream_) {
			return tTJSVariant();
		}
		size_type result_size = binary_.size();
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&binary_[0]), result_size);
	}
	KTL_INLINE tTJSVariant NativeBzip2Decompressor::asOctet(size_type pos) const {
		if (stream_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(binary_.size(), pos);
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&binary_[pos]), result_size);
	}
	KTL_INLINE tTJSVariant NativeBzip2Decompressor::asOctet(size_type pos, size_type size) const {
		if (stream_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(binary_.size(), pos, size);
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&binary_[pos]), result_size);
	}
	KTL_INLINE tTJSVariant NativeBzip2Decompressor::asString() const {
		binary_type binary(binary_);
		binary.insert(binary.end(), sizeof(tjs_char) * 2 + 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
	}
	tTJSVariant NativeBzip2Decompressor::asString(size_type pos) const {
		size_type result_size = getEnableSize(binary_.size(), pos);
		binary_type binary(
			result_size
				? boost::next(binary_.begin(), pos)
				: binary_.end()
				,
			binary_.end()
			);
		binary.insert(binary.end(), sizeof(tjs_char) * 2 + 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
	}
	tTJSVariant NativeBzip2Decompressor::asString(size_type pos, size_type size) const {
		size_type result_size = getEnableSize(binary_.size(), pos, size);
		binary_type binary(
			result_size
				? boost::next(binary_.begin(), pos)
				: binary_.end()
				,
			result_size
				? boost::next(binary_.begin(), pos + result_size)
				: binary_.end()
			);
		binary.insert(binary.end(), sizeof(tjs_char) * 2 + 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
	}
	KTL_INLINE tTJSVariant NativeBzip2Decompressor::asNarrowString() const {
		binary_type binary(binary_);
		binary.insert(binary.end(), sizeof(tjs_nchar) * 2 + 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&binary[0]));
	}
	tTJSVariant NativeBzip2Decompressor::asNarrowString(size_type pos) const {
		size_type result_size = getEnableSize(binary_.size(), pos);
		binary_type binary(
			result_size
				? boost::next(binary_.begin(), pos)
				: binary_.end()
				,
			binary_.end()
			);
		binary.insert(binary.end(), sizeof(tjs_nchar) * 2 + 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&binary[0]));
	}
	tTJSVariant NativeBzip2Decompressor::asNarrowString(size_type pos, size_type size) const {
		size_type result_size = getEnableSize(binary_.size(), pos, size);
		binary_type binary(
			result_size
				? boost::next(binary_.begin(), pos)
				: binary_.end()
				,
			result_size
				? boost::next(binary_.begin(), pos + result_size)
				: binary_.end()
			);
		binary.insert(binary.end(), sizeof(tjs_nchar) * 2 + 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&binary[0]));
	}
	//
	//	SUMMARY: ストレージ系メソッド
	//
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::saveStorage(tjs_char const* storage) const {
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
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::saveStorage(tjs_char const* storage, size_type pos) const {
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
	KTL_INLINE NativeBzip2Decompressor::int_type NativeBzip2Decompressor::saveStorage(tjs_char const* storage, size_type pos, size_type size) const {
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
	// Bzip2Decompressor
	//
	Bzip2Decompressor::Bzip2Decompressor() {}
	tjs_error TJS_INTF_METHOD Bzip2Decompressor::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Bzip2Decompressor::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeBzip2Decompressor>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Bzip2Decompressor::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Bzip2Decompressor::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: セットアップ系メソッド
	//
	KTL_INLINE void Bzip2Decompressor::reset() {
		instance_->reset();
	}
	KTL_INLINE bool Bzip2Decompressor::setup() {
		return instance_->setup();
	}
	KTL_INLINE bool Bzip2Decompressor::setupCompleted() const {
		return instance_->setupCompleted();
	}
	KTL_INLINE bool Bzip2Decompressor::finish() {
		return instance_->finish();
	}
	KTL_INLINE bool Bzip2Decompressor::flush() {
		return instance_->flush();
	}
	//
	//	SUMMARY: パラメータ系メソッド
	//
	KTL_INLINE bool Bzip2Decompressor::getSmall() const {
		return instance_->getSmall();
	}
	KTL_INLINE void Bzip2Decompressor::setSmall(bool v) {
		instance_->setSmall(
			v
			);
	}
	//
	//	SUMMARY: 処理系メソッド
	//
	KTL_INLINE tTVInteger Bzip2Decompressor::processBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last) {
		return instance_->processBlock(
			reinterpret_cast<NativeBzip2Decompressor::byte_type const*>(first),
			reinterpret_cast<NativeBzip2Decompressor::byte_type const*>(last)
			);
	}
	KTL_INLINE tTVInteger Bzip2Decompressor::processBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size) {
		return instance_->processBytes(
			reinterpret_cast<NativeBzip2Decompressor::byte_type const*>(data),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(size)
			);
	}
	KTL_INLINE tTVInteger Bzip2Decompressor::processByte(tTVInteger byte) {
		return instance_->processByte(
			static_cast<NativeBzip2Decompressor::byte_type>(byte)
			);
	}
	KTL_INLINE tTVInteger Bzip2Decompressor::processOctet(tTJSVariantOctet const* source) {
		return instance_->processOctet(source);
	}
	KTL_INLINE tTVInteger Bzip2Decompressor::processOctet(tTJSVariantOctet const* source, tTVInteger spos) {
		return instance_->processOctet(
			source,
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger Bzip2Decompressor::processOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->processOctet(
			source,
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(spos),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger Bzip2Decompressor::processStorage(tTJSVariantString const* storage) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger Bzip2Decompressor::processStorage(tTJSVariantString const* storage, tTVInteger spos) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger Bzip2Decompressor::processStorage(tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(spos),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(ssize)
			);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE tTVInteger Bzip2Decompressor::size() const {
		return instance_->size();
	}
	KTL_INLINE tTJSVariant Bzip2Decompressor::asOctet() const {
		return instance_->asOctet();
	}
	KTL_INLINE tTJSVariant Bzip2Decompressor::asOctet(tTVInteger pos) const {
		return instance_->asOctet(
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Bzip2Decompressor::asOctet(tTVInteger pos, tTVInteger size) const {
		return instance_->asOctet(
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(pos),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Bzip2Decompressor::asString() const {
		return instance_->asString();
	}
	KTL_INLINE tTJSVariant Bzip2Decompressor::asString(tTVInteger pos) const {
		return instance_->asString(
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Bzip2Decompressor::asString(tTVInteger pos, tTVInteger size) const {
		return instance_->asString(
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(pos),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Bzip2Decompressor::asNarrowString() const {
		return instance_->asNarrowString();
	}
	KTL_INLINE tTJSVariant Bzip2Decompressor::asNarrowString(tTVInteger pos) const {
		return instance_->asNarrowString(
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Bzip2Decompressor::asNarrowString(tTVInteger pos, tTVInteger size) const {
		return instance_->asNarrowString(
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(pos),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(size)
			);
	}
	//
	//	SUMMARY: ストレージ系メソッド
	//
	KTL_INLINE tTVInteger Bzip2Decompressor::saveStorage(tTJSVariantString const* storage) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger Bzip2Decompressor::saveStorage(tTJSVariantString const* storage, tTVInteger pos) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(pos)
			);
	}
	KTL_INLINE tTVInteger Bzip2Decompressor::saveStorage(tTJSVariantString const* storage, tTVInteger pos, tTVInteger size) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(pos),
			boost::numeric_cast<NativeBzip2Decompressor::size_type>(size)
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_BZIP2_BZIP2_DECOMPRESSOR_HPP
