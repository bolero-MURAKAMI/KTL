/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_STOREGE_STREAM_STOREGE_STREAM_HPP
#define SRC_STOREGE_STREAM_STOREGE_STREAM_HPP

#include <string>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/str_length.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "StorageStreamDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:StorageStream:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:StorageStream:error")

namespace ktl {
	//
	// NativeStorageStream
	//
	KTL_INLINE NativeStorageStream::size_type NativeStorageStream::getEnableSize(size_type length, size_type spos) {
		return spos <= length
			? length - spos
			: 0
			;
	}
	KTL_INLINE NativeStorageStream::size_type NativeStorageStream::getEnableSize(size_type length, size_type spos, size_type ssize) {
		return spos + ssize <= length
			? ssize
			: spos <= length
				? length - spos
				: 0
			;
	}
	bool NativeStorageStream::readBuffer(binary_type& buffer) {
		if (!stream_) {
			return false;
		}
		int_type current_pos = current();
		if (current_pos < 0) {
			return false;
		}
		int_type total_size = size();
		if (total_size < 0 || current_pos < total_size) {
			return false;
		}
		size_type result_size = total_size - current_pos;
		if (!result_size) {
			buffer.clear();
			return true;
		}
		buffer.resize(result_size);
		ULONG io_size = 0;
		if (FAILED(stream_->Read(&buffer[0], result_size, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ファイル読込に失敗しました"),
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	bool NativeStorageStream::readBuffer(binary_type& buffer, size_type size) {
		if (!stream_) {
			return false;
		}
		if (!size) {
			buffer.clear();
			return true;
		}
		buffer.resize(size);
		ULONG io_size = 0;
		if (FAILED(stream_->Read(&buffer[0], size, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ファイル読込に失敗しました"),
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	NativeStorageStream::NativeStorageStream() {}
	//
	//	SUMMARY: 開閉系メソッド
	//
	KTL_INLINE bool NativeStorageStream::open(tjs_char const* storage, flag_type flag) {
		sprig::com_ptr<IStream> stream(::TVPCreateIStream(storage, flag));
		if (!stream) {
			return false;
		}
		stream_.swap(stream);
		return true;
	}
	KTL_INLINE bool NativeStorageStream::close() {
		if (!stream_) {
			return false;
		}
		stream_.reset();
		return true;
	}
	KTL_INLINE bool NativeStorageStream::isOpen() const {
		return stream_;
	}
	//
	//	SUMMARY: 読込系メソッド
	//
	KTL_INLINE tTJSVariant NativeStorageStream::readOctet() {
		binary_type buffer;
		if (!readBuffer(buffer)) {
			return tTJSVariant();
		}
		return tTJSVariant(
			buffer.size()
				? &buffer[0]
				: 0
				,
			buffer.size()
			);
	}
	KTL_INLINE tTJSVariant NativeStorageStream::readOctet(size_type size) {
		binary_type buffer;
		if (!readBuffer(buffer, size)) {
			return tTJSVariant();
		}
		return tTJSVariant(
			buffer.size()
				? &buffer[0]
				: 0
				,
			buffer.size()
			);
	}
	KTL_INLINE tTJSVariant NativeStorageStream::readString() {
		binary_type buffer;
		if (!readBuffer(buffer)) {
			return tTJSVariant();
		}
		buffer.insert(buffer.end(), sizeof(tjs_char) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&buffer[0]));
	}
	KTL_INLINE tTJSVariant NativeStorageStream::readString(size_type size) {
		binary_type buffer;
		if (!readBuffer(buffer, size)) {
			return tTJSVariant();
		}
		buffer.insert(buffer.end(), sizeof(tjs_char) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&buffer[0]));
	}
	KTL_INLINE tTJSVariant NativeStorageStream::readNarrowString() {
		binary_type buffer;
		if (!readBuffer(buffer)) {
			return tTJSVariant();
		}
		buffer.insert(buffer.end(), sizeof(tjs_nchar) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&buffer[0]));
	}
	KTL_INLINE tTJSVariant NativeStorageStream::readNarrowString(size_type size) {
		binary_type buffer;
		if (!readBuffer(buffer, size)) {
			return tTJSVariant();
		}
		buffer.insert(buffer.end(), sizeof(tjs_nchar) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&buffer[0]));
	}
	//
	//	SUMMARY: 書込系メソッド
	//
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeOctet(tTJSVariantOctet const* source) {
		if (!stream_) {
			return -1;
		}
		tjs_uint const length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* const data = sprig::krkr::tjs::octet_data(source);
		return writeBytes(data, length);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeOctet(tTJSVariantOctet const* source, size_type spos) {
		if (!stream_) {
			return -1;
		}
		tjs_uint const length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* const data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return writeBytes(data + spos, result_size);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize) {
		if (!stream_) {
			return -1;
		}
		tjs_uint const length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* const data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return writeBytes(data + spos, result_size);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeString(tjs_char const* source) {
		if (!stream_) {
			return -1;
		}
		return writeBytes(
			reinterpret_cast<byte_type const*>(source),
			sprig::str_length(source) * sizeof(tjs_char)
			);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeString(tjs_char const* source, size_type spos) {
		if (!stream_) {
			return -1;
		}
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return writeBytes(data + spos, result_size);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeString(tjs_char const* source, size_type spos, size_type ssize) {
		if (!stream_) {
			return -1;
		}
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return writeBytes(data + spos, result_size);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeNarrowString(tjs_nchar const* source) {
		if (!stream_) {
			return -1;
		}
		return writeBytes(
			reinterpret_cast<byte_type const*>(source),
			sprig::str_length(source) * sizeof(tjs_nchar)
			);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeNarrowString(tjs_nchar const* source, size_type spos) {
		if (!stream_) {
			return -1;
		}
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return writeBytes(data + spos, result_size);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeNarrowString(tjs_nchar const* source, size_type spos, size_type ssize) {
		if (!stream_) {
			return -1;
		}
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return writeBytes(data + spos, result_size);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeBlock(byte_type const* first, byte_type const* last) {
		return writeBytes(first, last - first);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::writeBytes(byte_type const* data, size_type size) {
		if (!stream_) {
			return -1;
		}
		if (!size) {
			return size;
		}
		ULONG io_size = 0;
		if (FAILED(stream_->Write(data, size, &io_size))) {
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
	//	SUMMARY: シーク系メソッド
	//
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::seek(int_type offset, flag_type flag) {
		if (!stream_) {
			return -1;
		}
		LARGE_INTEGER move;
		move.QuadPart = offset;
		ULARGE_INTEGER new_pos;
		if (FAILED(stream_->Seek(move, flag, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ファイルシークに失敗しました"),
				sprig::krkr::internal_error
				);
			return -1;
		}
		return static_cast<int_type>(new_pos.QuadPart);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::current() {
		return seek(0, sfCur);
	}
	KTL_INLINE NativeStorageStream::int_type NativeStorageStream::size() {
		int_type cur = current();
		if (cur < 0) {
			return cur;
		}
		int_type result = seek(0, sfEnd);
		if (result < 0) {
			return result;
		}
		int_type comeback = seek(cur, sfSet);
		if (comeback < 0) {
			return comeback;
		}
		return result;
	}

	//
	// StorageStream
	//
	StorageStream::StorageStream() {}
	tjs_error TJS_INTF_METHOD StorageStream::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("StorageStream::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeStorageStream>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD StorageStream::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("StorageStream::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 開閉系メソッド
	//
	KTL_INLINE bool StorageStream::open(tTJSVariantString const* storage, tTVInteger flag) {
		return instance_->open(
			sprig::krkr::tjs::as_c_str(storage),
			sprig::numeric::bit_cast<NativeStorageStream::flag_type>(flag)
			);
	}
	KTL_INLINE bool StorageStream::close() {
		return instance_->close();
	}
	KTL_INLINE bool StorageStream::isOpen() const {
		return instance_->isOpen();
	}
	//
	//	SUMMARY: 読込系メソッド
	//
	KTL_INLINE tTJSVariant StorageStream::readOctet() {
		return instance_->readOctet();
	}
	KTL_INLINE tTJSVariant StorageStream::readOctet(tTVInteger size) {
		return instance_->readOctet(
			boost::numeric_cast<NativeStorageStream::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant StorageStream::readString() {
		return instance_->readString();
	}
	KTL_INLINE tTJSVariant StorageStream::readString(tTVInteger size) {
		return instance_->readString();
	}
	KTL_INLINE tTJSVariant StorageStream::readNarrowString() {
		return instance_->readNarrowString();
	}
	KTL_INLINE tTJSVariant StorageStream::readNarrowString(tTVInteger size) {
		return instance_->readNarrowString();
	}
	//
	//	SUMMARY: 書込系メソッド
	//
	KTL_INLINE tTVInteger StorageStream::writeOctet(tTJSVariantOctet const* source) {
		return instance_->writeOctet(
			source
			);
	}
	KTL_INLINE tTVInteger StorageStream::writeOctet(tTJSVariantOctet const* source, tTVInteger spos) {
		return instance_->writeOctet(
			source,
			boost::numeric_cast<NativeStorageStream::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger StorageStream::writeOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->writeOctet(
			source,
			boost::numeric_cast<NativeStorageStream::size_type>(spos),
			boost::numeric_cast<NativeStorageStream::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger StorageStream::writeString(tTJSVariantString const* source) {
		return instance_->writeString(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE tTVInteger StorageStream::writeString(tTJSVariantString const* source, tTVInteger spos) {
		return instance_->writeString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeStorageStream::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger StorageStream::writeString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->writeString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeStorageStream::size_type>(spos),
			boost::numeric_cast<NativeStorageStream::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger StorageStream::writeNarrowString(tTJSVariantString const* source) {
		return instance_->writeNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
			);
	}
	KTL_INLINE tTVInteger StorageStream::writeNarrowString(tTJSVariantString const* source, tTVInteger spos) {
		return instance_->writeNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeStorageStream::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger StorageStream::writeNarrowString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->writeNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeStorageStream::size_type>(spos),
			boost::numeric_cast<NativeStorageStream::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger StorageStream::writeBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last) {
		return instance_->writeBlock(
			reinterpret_cast<NativeStorageStream::byte_type const*>(first),
			reinterpret_cast<NativeStorageStream::byte_type const*>(last)
			);
	}
	KTL_INLINE tTVInteger StorageStream::writeBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size) {
		return instance_->writeBytes(
			reinterpret_cast<NativeStorageStream::byte_type const*>(data),
			boost::numeric_cast<NativeStorageStream::size_type>(size)
			);
	}
	//
	//	SUMMARY: シーク系メソッド
	//
	KTL_INLINE tTVInteger StorageStream::seek(tTVInteger offset, tTVInteger flag) {
		return instance_->seek(
			boost::numeric_cast<NativeStorageStream::int_type>(offset),
			sprig::numeric::bit_cast<NativeStorageStream::flag_type>(flag)
			);
	}
	KTL_INLINE tTVInteger StorageStream::current() {
		return instance_->current();
	}
	KTL_INLINE tTVInteger StorageStream::size() {
		return instance_->size();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_STOREGE_STREAM_STOREGE_STREAM_HPP
