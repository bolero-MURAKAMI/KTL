#pragma once

#include <string>
#include <vector>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/crc.hpp>
#include <hamigaki/checksum/md5.hpp>
#include <hamigaki/checksum/sha1.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/checksum_hasher.hpp>
#include <sprig/str_length.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "ChecksumCommon.hpp"
#include "ChecksumDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Checksum:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Checksum:error")

namespace ktl {
	//
	// NativeChecksum
	//
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::getEnableSize(size_type length, size_type spos) {
		return spos <= length
			? length - spos
			: 0
			;
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::getEnableSize(size_type length, size_type spos, size_type ssize) {
		return spos + ssize <= length
			? ssize
			: spos <= length
				? length - spos
				: 0
			;
	}
	template<typename Impl>
	KTL_INLINE bool NativeChecksum<Impl>::loadStorage(binary_type& binary, tjs_char const* storage) {
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
	template<typename Impl>
	KTL_INLINE bool NativeChecksum<Impl>::loadStorage(binary_type& binary, tjs_char const* storage, size_type spos) {
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
	template<typename Impl>
	KTL_INLINE bool NativeChecksum<Impl>::loadStorage(binary_type& binary, tjs_char const* storage, size_type spos, size_type ssize) {
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
	template<typename Impl>
	NativeChecksum<Impl>::NativeChecksum() {}
	//
	//	SUMMARY: 計算系メソッド
	//
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::value_type NativeChecksum<Impl>::calculateBlock(byte_type const* first, byte_type const* last) {
		return sprig::checksum_hasher<impl_type>::calculate_block(first, last);
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::value_type NativeChecksum<Impl>::calculateBytes(byte_type const* data, size_type size) {
		return sprig::checksum_hasher<impl_type>::calculate_bytes(data, size);
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::value_type NativeChecksum<Impl>::calculateOctet(tTJSVariantOctet const* source) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		return calculateBytes(
			reinterpret_cast<byte_type const*>(data),
			length
			);
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::value_type NativeChecksum<Impl>::calculateString(tjs_char const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		return calculateBytes(
			data,
			length
			);
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::value_type NativeChecksum<Impl>::calculateNarrowString(tjs_nchar const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		return calculateBytes(
			data,
			length
			);
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::value_type NativeChecksum<Impl>::calculateStorage(tjs_char const* storage) {
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return NativeChecksum<Impl>::value_type();
		}
		return calculateBytes(
			binary.size()
				? &binary[0]
				: 0
				,
			binary.size()
			);
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::octet_type NativeChecksum<Impl>::calculateBlockAsOctet(byte_type const* first, byte_type const* last) {
		return ChecksumUtils::toOctet(calculateBlock(first, last));
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::octet_type NativeChecksum<Impl>::calculateBytesAsOctet(byte_type const* data, size_type size) {
		return ChecksumUtils::toOctet(calculateBytes(data, size));
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::octet_type NativeChecksum<Impl>::calculateOctetAsOctet(tTJSVariantOctet const* source) {
		return ChecksumUtils::toOctet(calculateOctet(source));
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::octet_type NativeChecksum<Impl>::calculateStringAsOctet(tjs_char const* source) {
		return ChecksumUtils::toOctet(calculateString(source));
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::octet_type NativeChecksum<Impl>::calculateNarrowStringAsOctet(tjs_nchar const* source) {
		return ChecksumUtils::toOctet(calculateNarrowString(source));
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::octet_type NativeChecksum<Impl>::calculateStorageAsOctet(tjs_char const* storage) {
		return ChecksumUtils::toOctet(calculateStorage(storage));
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant NativeChecksum<Impl>::calculateBlockAsHex(byte_type const* first, byte_type const* last, bool upper) {
		return ChecksumUtils::toVariantHexString(calculateBlock(first, last), upper);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant NativeChecksum<Impl>::calculateBytesAsHex(byte_type const* data, size_type size, bool upper) {
		return ChecksumUtils::toVariantHexString(calculateBytes(data, size), upper);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant NativeChecksum<Impl>::calculateOctetAsHex(tTJSVariantOctet const* source, bool upper) {
		return ChecksumUtils::toVariantHexString(calculateOctet(source), upper);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant NativeChecksum<Impl>::calculateStringAsHex(tjs_char const* source, bool upper) {
		return ChecksumUtils::toVariantHexString(calculateString(source), upper);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant NativeChecksum<Impl>::calculateNarrowStringAsHex(tjs_nchar const* source, bool upper) {
		return ChecksumUtils::toVariantHexString(calculateNarrowString(source), upper);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant NativeChecksum<Impl>::calculateStorageAsHex(tjs_char const* storage, bool upper) {
		return ChecksumUtils::toVariantHexString(calculateStorage(storage), upper);
	}
	//
	//	SUMMARY: 処理系メソッド
	//
	template<typename Impl>
	KTL_INLINE void NativeChecksum<Impl>::reset() {
		impl_.reset();
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processBlock(byte_type const* first, byte_type const* last) {
		impl_.process_block(first, last);
		return last - first;
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processBytes(byte_type const* data, size_type size) {
		impl_.process_bytes(data, size);
		return size;
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processByte(byte_type byte) {
		impl_.process_byte(byte);
		return 1;
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processOctet(tTJSVariantOctet const* source) {
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
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processOctet(tTJSVariantOctet const* source, size_type spos) {
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
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize) {
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
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processString(tjs_char const* source) {
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
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processString(tjs_char const* source, size_type spos) {
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
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processString(tjs_char const* source, size_type spos, size_type ssize) {
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
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processNarrowString(tjs_nchar const* source) {
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
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processNarrowString(tjs_nchar const* source, size_type spos) {
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
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processNarrowString(tjs_nchar const* source, size_type spos, size_type ssize) {
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
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processStorage(tjs_char const* storage) {
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return 0;
		}
		if (binary.empty()) {
			return 0;
		}
		return processBytes(
			&binary[0],
			binary.size()
			);
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processStorage(tjs_char const* storage, size_type spos) {
		binary_type binary;
		if (!loadStorage(binary, storage, spos)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return 0;
		}
		if (binary.empty()) {
			return 0;
		}
		return processBytes(
			&binary[0],
			binary.size()
			);
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::size_type NativeChecksum<Impl>::processStorage(tjs_char const* storage, size_type spos, size_type ssize) {
		binary_type binary;
		if (!loadStorage(binary, storage, spos, ssize)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return 0;
		}
		if (binary.empty()) {
			return 0;
		}
		return processBytes(
			&binary[0],
			binary.size()
			);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::value_type NativeChecksum<Impl>::checksum() const {
		return impl_.checksum();
	}
	template<typename Impl>
	KTL_INLINE typename NativeChecksum<Impl>::octet_type NativeChecksum<Impl>::checksumOctet() const {
		return ChecksumUtils::toOctet(impl_.checksum());
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant NativeChecksum<Impl>::checksumHex(bool upper) const {
		return ChecksumUtils::toVariantHexString(impl_.checksum(), upper);
	}

	//
	// Checksum
	//
	template<typename Impl>
	Checksum<Impl>::Checksum() {}
	template<typename Impl>
	tjs_error TJS_INTF_METHOD Checksum<Impl>::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Checksum::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeChecksum<Impl> >();
		return TJS_S_OK;
	}
	template<typename Impl>
	void TJS_INTF_METHOD Checksum<Impl>::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Checksum::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 計算系メソッド
	//
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateBlock(
				reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(first),
				reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(last)
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateBytes(
				reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(data),
				boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(size)
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateOctet(tTJSVariantOctet const* source) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateOctet(
				source
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateString(tTJSVariantString const* source) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateString(
				sprig::krkr::tjs::as_c_str(source)
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateNarrowString(tTJSVariantString const* source) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateNarrowString(
				sprig::str_cast<std::basic_string<tjs_nchar> >(
					sprig::krkr::tjs::as_c_str(source)
					).c_str()
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateStorage(tTJSVariantString const* storage) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateStorage(
				sprig::krkr::tjs::as_c_str(storage)
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateBlockAsOctet(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateBlockAsOctet(
				reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(first),
				reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(last)
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateBytesAsOctet(sprig::krkr::tjs::intptr_type data, tTVInteger size) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateBytesAsOctet(
				reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(data),
				boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(size)
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateOctetAsOctet(tTJSVariantOctet const* source) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateOctetAsOctet(
				source
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateStringAsOctet(tTJSVariantString const* source) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateStringAsOctet(
				sprig::krkr::tjs::as_c_str(source)
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateNarrowStringAsOctet(tTJSVariantString const* source) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateNarrowStringAsOctet(
				sprig::str_cast<std::basic_string<tjs_nchar> >(
					sprig::krkr::tjs::as_c_str(source)
					).c_str()
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateStorageAsOctet(tTJSVariantString const* storage) {
		return ChecksumUtils::toVariant(
			NativeChecksum<Impl>::calculateStorageAsOctet(
				sprig::krkr::tjs::as_c_str(storage)
				)
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateBlockAsHex(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last, bool upper) {
		return NativeChecksum<Impl>::calculateBlockAsHex(
			reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(first),
			reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(last),
			upper
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateBytesAsHex(sprig::krkr::tjs::intptr_type data, tTVInteger size, bool upper) {
		return NativeChecksum<Impl>::calculateBytesAsHex(
			reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(data),
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(size),
			upper
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateOctetAsHex(tTJSVariantOctet const* source, bool upper) {
		return NativeChecksum<Impl>::calculateOctetAsHex(
			source,
			upper
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateStringAsHex(tTJSVariantString const* source, bool upper) {
		return NativeChecksum<Impl>::calculateStringAsHex(
			sprig::krkr::tjs::as_c_str(source),
			upper
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateNarrowStringAsHex(tTJSVariantString const* source, bool upper) {
		return NativeChecksum<Impl>::calculateNarrowStringAsHex(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			upper
			);
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::calculateStorageAsHex(tTJSVariantString const* storage, bool upper) {
		return NativeChecksum<Impl>::calculateStorageAsHex(
			sprig::krkr::tjs::as_c_str(storage),
			upper
			);
	}
	//
	//	SUMMARY: 処理系メソッド
	//
	template<typename Impl>
	KTL_INLINE void Checksum<Impl>::reset() {
		instance_->reset();
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last) {
		return instance_->processBlock(
			reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(first),
			reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(last)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size) {
		return instance_->processBytes(
			reinterpret_cast<typename NativeChecksum<Impl>::byte_type const*>(data),
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(size)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processByte(tTVInteger byte) {
		return instance_->processByte(
			static_cast<typename NativeChecksum<Impl>::byte_type>(byte)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processOctet(tTJSVariantOctet const* source) {
		return instance_->processOctet(
			source
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processOctet(tTJSVariantOctet const* source, tTVInteger spos) {
		return instance_->processOctet(
			source,
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(spos)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->processOctet(
			source,
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(spos),
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(ssize)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processString(tTJSVariantString const* source) {
		return instance_->processString(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processString(tTJSVariantString const* source, tTVInteger spos) {
		return instance_->processString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(spos)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->processString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(spos),
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(ssize)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processNarrowString(tTJSVariantString const* source) {
		return instance_->processNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processNarrowString(tTJSVariantString const* source, tTVInteger spos) {
		return instance_->processNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(spos)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processNarrowString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->processNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(spos),
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(ssize)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processStorage(tTJSVariantString const* storage) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processStorage(tTJSVariantString const* storage, tTVInteger spos) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(spos)
			);
	}
	template<typename Impl>
	KTL_INLINE tTVInteger Checksum<Impl>::processStorage(tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize) {
		return instance_->processStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(spos),
			boost::numeric_cast<typename NativeChecksum<Impl>::size_type>(ssize)
			);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::checksum() const {
		return ChecksumUtils::toVariant(instance_->checksum());
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::checksumOctet() const {
		return ChecksumUtils::toVariant(instance_->checksumOctet());
	}
	template<typename Impl>
	KTL_INLINE tTJSVariant Checksum<Impl>::checksumHex(bool upper) const {
		return instance_->checksumHex(upper);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
