#pragma once

#include <boost/ref.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>
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

#include "Bzip2CompressorDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Bzip2Compressor:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Bzip2Compressor:error")

namespace ktl {
	//
	// NativeBzip2Compressor
	//
	KTL_INLINE NativeBzip2Compressor::size_type NativeBzip2Compressor::getEnableSize(size_type length, size_type spos) {
		return spos <= length
			? length - spos
			: 0
			;
	}
	KTL_INLINE NativeBzip2Compressor::size_type NativeBzip2Compressor::getEnableSize(size_type length, size_type spos, size_type ssize) {
		return spos + ssize <= length
			? ssize
			: spos <= length
				? length - spos
				: 0
			;
	}
	KTL_INLINE bool NativeBzip2Compressor::loadStorage(binary_type& binary, tjs_char const* storage) {
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
	KTL_INLINE bool NativeBzip2Compressor::loadStorage(binary_type& binary, tjs_char const* storage, size_type spos) {
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
	KTL_INLINE bool NativeBzip2Compressor::loadStorage(binary_type& binary, tjs_char const* storage, size_type spos, size_type ssize) {
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
	NativeBzip2Compressor::NativeBzip2Compressor() {}
	//
	//	SUMMARY: セットアップ系メソッド
	//
	KTL_INLINE void NativeBzip2Compressor::reset() {
		params_type params;
		binary_type binary;
		filter_instance_type filter;
		stream_instance_type stream;
		boost::swap(stream_, stream);
		boost::swap(filter_, filter);
		boost::swap(binary_, binary);
		boost::swap(params_, params);
	}
	KTL_INLINE bool NativeBzip2Compressor::setup() {
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
	KTL_INLINE bool NativeBzip2Compressor::setupCompleted() const {
		return stream_;
	}
	KTL_INLINE bool NativeBzip2Compressor::finish() {
		if (!stream_) {
			return false;
		}
		boost::iostreams::flush(*stream_);
		stream_instance_type stream;
		boost::swap(stream_, stream);
		return true;
	}
	KTL_INLINE bool NativeBzip2Compressor::flush() {
		if (!stream_) {
			return false;
		}
		boost::iostreams::flush(*stream_);
		return true;
	}
	//
	//	SUMMARY: パラメータ系メソッド
	//
	KTL_INLINE int NativeBzip2Compressor::getBlockSize() const {
		return params_.block_size;
	}
	KTL_INLINE void NativeBzip2Compressor::setBlockSize(int v) {
		params_.block_size = v;
	}
	KTL_INLINE int NativeBzip2Compressor::getWorkFactor() const {
		return params_.work_factor;
	}
	KTL_INLINE void NativeBzip2Compressor::setWorkFactor(int v) {
		params_.work_factor = v;
	}
	//
	//	SUMMARY: 処理系メソッド
	//
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processBlock(byte_type const* first, byte_type const* last) {
		return processBytes(first, last - first);
	}
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processBytes(byte_type const* data, size_type size) {
		if (!stream_) {
			return -1;
		}
		if (!size) {
			return size;
		}
		return static_cast<NativeBzip2Compressor::int_type>(boost::iostreams::write(*stream_, data, size));
	}
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processByte(byte_type byte) {
		if (!stream_) {
			return -1;
		}
		return boost::iostreams::put(*stream_, byte)
			? 1
			: -1
			;
	}
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processOctet(tTJSVariantOctet const* source) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processOctet(tTJSVariantOctet const* source, size_type spos) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processString(tjs_char const* source) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processString(tjs_char const* source, size_type spos) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processString(tjs_char const* source, size_type spos, size_type ssize) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processNarrowString(tjs_nchar const* source) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processNarrowString(tjs_nchar const* source, size_type spos) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processNarrowString(tjs_nchar const* source, size_type spos, size_type ssize) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processStorage(tjs_char const* storage) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processStorage(tjs_char const* storage, size_type spos) {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::processStorage(tjs_char const* storage, size_type spos, size_type ssize) {
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
	KTL_INLINE NativeBzip2Compressor::size_type NativeBzip2Compressor::size() const {
		if (stream_) {
			return 0;
		}
		return binary_.size();
	}
	KTL_INLINE tTJSVariant NativeBzip2Compressor::asOctet() const {
		if (stream_) {
			return tTJSVariant();
		}
		size_type result_size = binary_.size();
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&binary_[0]), result_size);
	}
	KTL_INLINE tTJSVariant NativeBzip2Compressor::asOctet(size_type pos) const {
		if (stream_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(binary_.size(), pos);
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&binary_[pos]), result_size);
	}
	KTL_INLINE tTJSVariant NativeBzip2Compressor::asOctet(size_type pos, size_type size) const {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::saveStorage(tjs_char const* storage) const {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::saveStorage(tjs_char const* storage, size_type pos) const {
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
	KTL_INLINE NativeBzip2Compressor::int_type NativeBzip2Compressor::saveStorage(tjs_char const* storage, size_type pos, size_type size) const {
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
	// Bzip2Compressor
	//
	Bzip2Compressor::Bzip2Compressor() {}
	tjs_error TJS_INTF_METHOD Bzip2Compressor::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Bzip2Compressor::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeBzip2Compressor>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Bzip2Compressor::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Bzip2Compressor::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: セットアップ系メソッド
	//
	KTL_INLINE void Bzip2Compressor::reset() {
		instance_->reset();
	}
	KTL_INLINE bool Bzip2Compressor::setup() {
		return instance_->setup();
	}
	KTL_INLINE bool Bzip2Compressor::setupCompleted() const {
		return instance_->setupCompleted();
	}
	KTL_INLINE bool Bzip2Compressor::finish() {
		return instance_->finish();
	}
	KTL_INLINE bool Bzip2Compressor::flush() {
		return instance_->flush();
	}
	//
	//	SUMMARY: パラメータ系メソッド
	//
	KTL_INLINE tTVInteger Bzip2Compressor::getBlockSize() const {
		return instance_->getBlockSize();
	}
	KTL_INLINE void Bzip2Compressor::setBlockSize(tTVInteger v) {
		instance_->setBlockSize(
			boost::numeric_cast<int>(v)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::getWorkFactor() const {
		return instance_->getWorkFactor();
	}
	KTL_INLINE void Bzip2Compressor::setWorkFactor(tTVInteger v) {
		instance_->setWorkFactor(
			boost::numeric_cast<int>(v)
			);
	}
	//
	//	SUMMARY: 処理系メソッド
	//
	KTL_INLINE tTVInteger Bzip2Compressor::processBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last) {
		return instance_->processBlock(
			reinterpret_cast<NativeBzip2Compressor::byte_type const*>(first),
			reinterpret_cast<NativeBzip2Compressor::byte_type const*>(last)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size) {
		return instance_->processBytes(
			reinterpret_cast<NativeBzip2Compressor::byte_type const*>(data),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(size)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processByte(tTVInteger byte) {
		return instance_->processByte(
			static_cast<NativeBzip2Compressor::byte_type>(byte)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processOctet(tTJSVariantOctet const* source) {
		return instance_->processOctet(source);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processOctet(tTJSVariantOctet const* source, tTVInteger spos) {
		return instance_->processOctet(
			source,
			boost::numeric_cast<NativeBzip2Compressor::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->processOctet(
			source,
			boost::numeric_cast<NativeBzip2Compressor::size_type>(spos),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processString(tTJSVariantString const* source) {
		return instance_->processString(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processString(tTJSVariantString const* source, tTVInteger spos) {
		return instance_->processString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->processString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(spos),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processNarrowString(tTJSVariantString const* source) {
		return instance_->processNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processNarrowString(tTJSVariantString const* source, tTVInteger spos) {
		return instance_->processNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeBzip2Compressor::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processNarrowString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->processNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeBzip2Compressor::size_type>(spos),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processStorage(tTJSVariantString const* storage) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processStorage(tTJSVariantString const* storage, tTVInteger spos) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::processStorage(tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(spos),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(ssize)
			);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE tTVInteger Bzip2Compressor::size() const {
		return instance_->size();
	}
	KTL_INLINE tTJSVariant Bzip2Compressor::asOctet() const {
		return instance_->asOctet();
	}
	KTL_INLINE tTJSVariant Bzip2Compressor::asOctet(tTVInteger pos) const {
		return instance_->asOctet(
			boost::numeric_cast<NativeBzip2Compressor::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Bzip2Compressor::asOctet(tTVInteger pos, tTVInteger size) const {
		return instance_->asOctet(
			boost::numeric_cast<NativeBzip2Compressor::size_type>(pos),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(size)
			);
	}
	//
	//	SUMMARY: ストレージ系メソッド
	//
	KTL_INLINE tTVInteger Bzip2Compressor::saveStorage(tTJSVariantString const* storage) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::saveStorage(tTJSVariantString const* storage, tTVInteger pos) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(pos)
			);
	}
	KTL_INLINE tTVInteger Bzip2Compressor::saveStorage(tTJSVariantString const* storage, tTVInteger pos, tTVInteger size) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(pos),
			boost::numeric_cast<NativeBzip2Compressor::size_type>(size)
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
