/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_OCTET_BUILDER_OCTET_BUILDER_HPP
#define SRC_OCTET_BUILDER_OCTET_BUILDER_HPP

#include <cstddef>
#include <cstring>
#include <string>
#include <algorithm>
#include <iterator>
#include <boost/foreach.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/next_prior.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/split_join.hpp>
#include <sprig/str_length.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/get_pointer.hpp>
#include <sprig/exception.hpp>
#include <sprig/section.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "OctetBuilderDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:OctetBuilder:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:OctetBuilder:error")

namespace ktl {
	//
	// NativeOctetBuilder
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type NativeOctetBuilder::getInstance(iTJSDispatch2* obj) {
		return sprig::krkr::tjs::GetPropValue<sprig::krkr::tjs::intptr_type>(
			sprig::krkr::tjs::GetClassMember(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("OctetBuilder")),
				SPRIG_KRKR_TJS_W("instance")
				).AsObjectNoAddRef(),
			obj
			);
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeOctetBuilder::createNew(tjs_int numparams, tTJSVariant** param) {
		return sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("OctetBuilder")),
			numparams, param, 0
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::getEnableSize(size_type length, size_type spos) {
		return spos <= length
			? length - spos
			: 0
			;
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::getEnableSize(size_type length, size_type spos, size_type ssize) {
		return spos + ssize <= length
			? ssize
			: spos <= length
				? length - spos
				: 0
			;
	}
	KTL_INLINE bool NativeOctetBuilder::loadStorage(binary_type& binary, tjs_char const* storage) {
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
	KTL_INLINE bool NativeOctetBuilder::loadStorage(binary_type& binary, tjs_char const* storage, size_type spos) {
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
	KTL_INLINE bool NativeOctetBuilder::loadStorage(binary_type& binary, tjs_char const* storage, size_type spos, size_type ssize) {
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
	template<typename IntType>
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setInteger(size_type pos, tTVInteger t, flag_type flag) {
		if (isOutOfRange(pos + sizeof(IntType) - 1)) {
			binary_.resize(pos + sizeof(IntType));
		}
		flag_type vef = flag & vefMask;
		switch (vef) {
		case vefBigEndian:
		default:
			sprig::integer_split(
				static_cast<IntType>(t),
				&binary_[pos]
			);
			break;
		case vefLittleEndian:
			sprig::integer_split(
				static_cast<IntType>(t),
				std::reverse_iterator<byte_type*>(&binary_[pos] + sizeof(IntType))
				);
			break;
		}
		return sizeof(IntType);
	}
	template<typename FloatType>
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setReal(size_type pos, tTVReal t, flag_type flag) {
		if (isOutOfRange(pos + sizeof(FloatType) - 1)) {
			binary_.resize(pos + sizeof(FloatType));
		}
		flag_type vef = flag & vefMask;
			switch (vef) {
			case vefBigEndian:
			default:
				sprig::float_split(
					static_cast<FloatType>(t),
					&binary_[pos]
				);
				break;
			case vefLittleEndian:
				sprig::float_split(
					static_cast<FloatType>(t),
					std::reverse_iterator<byte_type*>(&binary_[pos] + sizeof(FloatType))
					);
				break;
			}
		return sizeof(FloatType);
	}
	template<typename IntType>
	KTL_INLINE void NativeOctetBuilder::assignInteger(tTVInteger t, flag_type flag) {
		binary_.resize(sizeof(IntType));
		flag_type vef = flag & vefMask;
		switch (vef) {
		case vefBigEndian:
		default:
			sprig::integer_split(
				static_cast<IntType>(t),
				&binary_[0]
			);
			break;
		case vefLittleEndian:
			sprig::integer_split(
				static_cast<IntType>(t),
				std::reverse_iterator<byte_type*>(&binary_[0] + sizeof(IntType))
				);
			break;
		}
	}
	template<typename FloatType>
	KTL_INLINE void NativeOctetBuilder::assignReal(tTVReal t, flag_type flag) {
		binary_.resize(sizeof(FloatType));
		flag_type vef = flag & vefMask;
		switch (vef) {
		case vefBigEndian:
		default:
			sprig::float_split(
				static_cast<FloatType>(t),
				&binary_[0]
			);
			break;
		case vefLittleEndian:
			sprig::float_split(
				static_cast<FloatType>(t),
				std::reverse_iterator<byte_type*>(&binary_[0] + sizeof(FloatType))
				);
			break;
		}
	}
	template<typename IntType>
	KTL_INLINE void NativeOctetBuilder::insertInteger(size_type pos, tTVInteger t, flag_type flag) {
		binary_.insert(getPos(pos), sizeof(IntType), 0);
		flag_type vef = flag & vefMask;
		switch (vef) {
		case vefBigEndian:
		default:
			sprig::integer_split(
				static_cast<IntType>(t),
				&binary_[0]
			);
			break;
		case vefLittleEndian:
			sprig::integer_split(
				static_cast<IntType>(t),
				std::reverse_iterator<byte_type*>(&binary_[0] + sizeof(IntType))
				);
			break;
		}
	}
	template<typename FloatType>
	KTL_INLINE void NativeOctetBuilder::insertReal(size_type pos, tTVReal t, flag_type flag) {
		binary_.insert(getPos(pos), sizeof(FloatType), 0);
		flag_type vef = flag & vefMask;
		switch (vef) {
		case vefBigEndian:
		default:
			sprig::float_split(
				static_cast<FloatType>(t),
				&binary_[0]
			);
			break;
		case vefLittleEndian:
			sprig::float_split(
				static_cast<FloatType>(t),
				std::reverse_iterator<byte_type*>(&binary_[0] + sizeof(FloatType))
				);
			break;
		}
	}
	template<typename IntType>
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addInteger(tTVInteger t, flag_type flag) {
		size_type result = binary_.size();
		binary_.resize(result + sizeof(IntType));
		flag_type vef = flag & vefMask;
		switch (vef) {
		case vefBigEndian:
		default:
			sprig::integer_split(
				static_cast<IntType>(t),
				&binary_[result]
			);
			break;
		case vefLittleEndian:
			sprig::integer_split(
				static_cast<IntType>(t),
				std::reverse_iterator<byte_type*>(&binary_[result] + sizeof(IntType))
				);
			break;
		}
		return result;
	}
	template<typename FloatType>
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addReal(tTVReal t, flag_type flag) {
		size_type result = binary_.size();
		binary_.resize(result + sizeof(FloatType));
		flag_type vef = flag & vefMask;
		switch (vef) {
		case vefBigEndian:
		default:
			sprig::float_split(
				static_cast<FloatType>(t),
				&binary_[result]
			);
			break;
		case vefLittleEndian:
			sprig::float_split(
				static_cast<FloatType>(t),
				std::reverse_iterator<byte_type*>(&binary_[result] + sizeof(FloatType))
				);
			break;
		}
		return result;
	}
	template<typename IntType>
	KTL_INLINE tTJSVariant NativeOctetBuilder::asInteger(size_type pos, flag_type flag) const {
		if (isOutOfRange(pos + sizeof(IntType) - 1)) {
			return tTJSVariant();
		}
		flag_type vef = flag & vefMask;
		switch (vef) {
		case vefBigEndian:
		default:
			return tTJSVariant(
				static_cast<tTVInteger>(
					sprig::integer_join<IntType>(
						&binary_[pos]
						)
					)
				);
			break;
		case vefLittleEndian:
			return tTJSVariant(
				static_cast<tTVInteger>(
					sprig::integer_join<IntType>(
						std::reverse_iterator<byte_type const*>(&binary_[pos] + sizeof(IntType))
						)
					)
				);
		}
	}
	template<typename FloatType>
	KTL_INLINE tTJSVariant NativeOctetBuilder::asReal(size_type pos, flag_type flag) const {
		if (isOutOfRange(pos + sizeof(FloatType) - 1)) {
			return tTJSVariant();
		}
		flag_type vef = flag & vefMask;
		switch (vef) {
		case vefBigEndian:
		default:
			return tTJSVariant(
				static_cast<tTVReal>(
					sprig::float_join<FloatType>(
						&binary_[pos]
						)
					)
				);
			break;
		case vefLittleEndian:
			return tTJSVariant(
				static_cast<tTVReal>(
					sprig::float_join<FloatType>(
						std::reverse_iterator<byte_type const*>(&binary_[pos] + sizeof(FloatType))
						)
					)
				);
		}
	}
	NativeOctetBuilder::NativeOctetBuilder() {}
	KTL_INLINE void NativeOctetBuilder::emptyCheck() const {
		if (binary_.empty()) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("要素が空です"),
				sprig::krkr::bad_state
				);
		}
	}
	KTL_INLINE bool NativeOctetBuilder::isOutOfRange(size_type pos) const {
		return pos >= binary_.size();
	}
	KTL_INLINE bool NativeOctetBuilder::isOutOfRangeOff(size_type pos) const {
		return pos > binary_.size();
	}
	KTL_INLINE NativeOctetBuilder::iterator NativeOctetBuilder::getPos(size_type pos) {
		if (isOutOfRange(pos)) {
			binary_.resize(pos);
		}
		return boost::next(binary_.begin(), pos);
	}
	KTL_INLINE NativeOctetBuilder::iterator NativeOctetBuilder::getBegin() {
		return binary_.begin();
	}
	KTL_INLINE NativeOctetBuilder::iterator NativeOctetBuilder::getEnd() {
		return binary_.end();
	}
	//
	//	SUMMARY: 生成系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type NativeOctetBuilder::fromHex(tTJSVariantString const* data) {
		//	COMMENT: 変換テーブル
		static tjs_char const* upper_table = SPRIG_KRKR_TJS_W("0123456789ABCDEF");
		static tjs_char const* upper_end = upper_table + 16;
		static tjs_char const* lower_table = SPRIG_KRKR_TJS_W("0123456789abcdef");
		static tjs_char const* lower_end = lower_table + 16;
		struct func {
			static inline int_type to_harf_byte(tjs_char c) {
				tjs_char const* found = std::find(upper_table, upper_end, c);
				if (found != upper_end) {
					return std::distance(upper_table, found);
				} else {
					tjs_char const* found = std::find(lower_table, lower_end, c);
					if (found != lower_end) {
						return std::distance(lower_table, found);
					} else {
						return -1;
					}
				}
			}
		};
		tjs_char const* str = sprig::krkr::tjs::as_c_str(data);
		std::size_t const len = data->GetLength();
		binary_type binary;
		for (std::size_t i = 0, last = len % 2 ? len - 1 : len; i != last; i += 2) {
			//	COMMENT: 1文字目
			int_type byte = func::to_harf_byte(str[i]);
			if (byte < 0) {
				return sprig::krkr::tjs::object_type();
			}
			//	COMMENT: 2文字目
			int_type byte2 = func::to_harf_byte(str[i + 1]);
			if (byte2 < 0) {
				return sprig::krkr::tjs::object_type();
			}
			//	COMMENT: 代入
			byte <<= 4;
			byte |= byte2;
			binary.push_back(static_cast<byte_type>(byte));
		}
		//	COMMENT: 文字数が奇数なら最後の文字だけ別に処理
		if (len % 2) {
			int_type byte = func::to_harf_byte(str[len - 1]);
			if (byte < 0) {
				return sprig::krkr::tjs::object_type();
			}
			byte <<= 4;
			binary.push_back(static_cast<byte_type>(byte));
		}
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		OctetBuilder* ptr = reinterpret_cast<OctetBuilder*>(getInstance(sprig::get_pointer(result)));
		ptr->init(binary.begin(), binary.end());
		return result;
	}
	//
	//	SUMMARY: 書込系メソッド
	//
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::set(size_type pos, tTJSVariant* v, flag_type flag) {
		flag_type vtf = flag & vtfMask;
		switch (vtf) {
		case vtfByte:
			if (isOutOfRange(pos)) {
				binary_.resize(pos + 1);
			}
			binary_[pos] = static_cast<byte_type>(v->AsInteger());
			return 1;
		case vtfInt8:
			return setInteger<boost::int8_t>(pos, v->AsInteger(), flag);
		case vtfInt16:
			return setInteger<boost::int16_t>(pos, v->AsInteger(), flag);
		case vtfInt32:
			return setInteger<boost::int32_t>(pos, v->AsInteger(), flag);
		case vtfInt64:
			return setInteger<boost::int64_t>(pos, v->AsInteger(), flag);
		case vtfUInt8:
			return setInteger<boost::uint8_t>(pos, v->AsInteger(), flag);
		case vtfUInt16:
			return setInteger<boost::uint16_t>(pos, v->AsInteger(), flag);
		case vtfUInt32:
			return setInteger<boost::uint32_t>(pos, v->AsInteger(), flag);
		case vtfUInt64:
			return setInteger<boost::uint64_t>(pos, v->AsInteger(), flag);
		case vtfFloat:
			return setReal<float>(pos, v->AsReal(), flag);
		case vtfDouble:
			return setReal<double>(pos, v->AsReal(), flag);
		case vtfOctet:
			if (v->Type() != tvtOctet) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return 0;
			}
			return setOctet(
				pos,
				v->AsOctetNoAddRef()
				);
		case vtfString:
			if (v->Type() != tvtString) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return 0;
			}
			return setString(
				pos,
				sprig::krkr::tjs::as_c_str(v->AsStringNoAddRef())
				);
		case vtfNarrowString:
			if (v->Type() != tvtString) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return 0;
			}
			return setNarrowString(
				pos,
				sprig::str_cast<std::basic_string<tjs_nchar> >(
					sprig::krkr::tjs::as_c_str(v->AsStringNoAddRef())
					).c_str()
				);
		case vtfBuilder:
			if (v->Type() != tvtObject) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return 0;
			}
			{
				iTJSDispatch2* obj = v->AsObjectNoAddRef();
				if (!obj || !sprig::krkr::tjs::IsInstanceObjectOf(obj, SPRIG_KRKR_TJS_W("OctetBuilder"))) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("型が一致しません"),
						sprig::krkr::bad_argument
						);
					return 0;
				}
				return setBuilder(pos, obj);
			}
		default:
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("無効なフラグです"),
				sprig::krkr::bad_argument
				);
			return 0;
		}
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setByte(size_type pos, size_type size, byte_type b) {
		if (isOutOfRange(pos + size)) {
			binary_.resize(pos + size);
		}
		if (!binary_.empty()) {
			std::memset(&binary_[pos], b, size);
		}
		return size;
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setBuilder(size_type pos, iTJSDispatch2* source) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		if (!length) {
			return length;
		}
		return setBytes(
			pos,
			reinterpret_cast<byte_type const*>(instance->begin()),
			length
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setBuilder(size_type pos, iTJSDispatch2* source, size_type spos) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return setBytes(
			pos,
			reinterpret_cast<byte_type const*>(instance->begin()) + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setBuilder(size_type pos, iTJSDispatch2* source, size_type spos, size_type ssize) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return setBytes(
			pos,
			reinterpret_cast<byte_type const*>(instance->begin()) + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setOctet(size_type pos, tTJSVariantOctet const* source) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		if (!length) {
			return length;
		}
		return setBytes(
			pos,
			data,
			length
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setOctet(size_type pos, tTJSVariantOctet const* source, size_type spos) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return setBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setOctet(size_type pos, tTJSVariantOctet const* source, size_type spos, size_type ssize) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return setBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setString(size_type pos, tjs_char const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		return setBytes(
			pos,
			data,
			length
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setString(size_type pos, tjs_char const* source, size_type spos) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return setBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setString(size_type pos, tjs_char const* source, size_type spos, size_type ssize) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return setBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setNarrowString(size_type pos, tjs_nchar const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		return setBytes(
			pos,
			data,
			length
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setNarrowString(size_type pos, tjs_nchar const* source, size_type spos) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return setBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setNarrowString(size_type pos, tjs_nchar const* source, size_type spos, size_type ssize) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return setBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setStorage(size_type pos, tjs_char const* storage) {
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
		return setBytes(
			pos,
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setStorage(size_type pos, tjs_char const* storage, size_type spos) {
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
		return setBytes(
			pos,
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setStorage(size_type pos, tjs_char const* storage, size_type spos, size_type ssize) {
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
		return setBytes(
			pos,
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setBlock(size_type pos, byte_type const* first, byte_type const* last) {
		return setBytes(pos, first, last - first);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::setBytes(size_type pos, byte_type const* data, size_type size) {
		if (isOutOfRange(pos + size)) {
			binary_.resize(pos + size);
		}
		if (!binary_.empty()) {
			std::memmove(&binary_[pos], data, size);
		}
		return size;
	}
	//
	//	SUMMARY: 割当系メソッド
	//
	KTL_INLINE void NativeOctetBuilder::assign(tTJSVariant* v, flag_type flag) {
		flag_type vtf = flag & vtfMask;
		switch (vtf) {
		case vtfByte:
			binary_.assign(1, static_cast<byte_type>(v->AsInteger()));
			return;
		case vtfInt8:
			assignInteger<boost::int8_t>(v->AsInteger(), flag);
			return;
		case vtfInt16:
			assignInteger<boost::int16_t>(v->AsInteger(), flag);
			return;
		case vtfInt32:
			assignInteger<boost::int32_t>(v->AsInteger(), flag);
			return;
		case vtfInt64:
			assignInteger<boost::int64_t>(v->AsInteger(), flag);
			return;
		case vtfUInt8:
			assignInteger<boost::uint8_t>(v->AsInteger(), flag);
			return;
		case vtfUInt16:
			assignInteger<boost::uint16_t>(v->AsInteger(), flag);
			return;
		case vtfUInt32:
			assignInteger<boost::uint32_t>(v->AsInteger(), flag);
			return;
		case vtfUInt64:
			assignInteger<boost::uint64_t>(v->AsInteger(), flag);
			return;
		case vtfFloat:
			assignReal<float>(v->AsReal(), flag);
			return;
		case vtfDouble:
			assignReal<double>(v->AsReal(), flag);
			return;
		case vtfOctet:
			if (v->Type() != tvtOctet) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return;
			}
			return assignOctet(
				v->AsOctetNoAddRef()
				);
		case vtfString:
			if (v->Type() != tvtString) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return;
			}
			assignString(
				sprig::krkr::tjs::as_c_str(v->AsStringNoAddRef())
				);
			return;
		case vtfNarrowString:
			if (v->Type() != tvtString) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return;
			}
			assignNarrowString(
				sprig::str_cast<std::basic_string<tjs_nchar> >(
					sprig::krkr::tjs::as_c_str(v->AsStringNoAddRef())
					).c_str()
				);
			return;
		case vtfBuilder:
			if (v->Type() != tvtObject) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return;
			}
			{
				iTJSDispatch2* obj = v->AsObjectNoAddRef();
				if (!obj || !sprig::krkr::tjs::IsInstanceObjectOf(obj, SPRIG_KRKR_TJS_W("OctetBuilder"))) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("型が一致しません"),
						sprig::krkr::bad_argument
						);
					return;
				}
				assignBuilder(obj);
				return;
			}
		default:
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("無効なフラグです"),
				sprig::krkr::bad_argument
				);
			return;
		}
	}
	KTL_INLINE void NativeOctetBuilder::assignByte(size_type size, byte_type b) {
		binary_.assign(size, b);
	}
	KTL_INLINE void NativeOctetBuilder::assignBuilder(iTJSDispatch2* source) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		assignBytes(
			length
				? reinterpret_cast<byte_type const*>(instance->begin())
				: 0
				,
			length
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignBuilder(iTJSDispatch2* source, size_type spos) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		size_type result_size = getEnableSize(length, spos);
		assignBytes(
			result_size
				? reinterpret_cast<byte_type const*>(instance->begin()) + spos
				: 0
				,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignBuilder(iTJSDispatch2* source, size_type spos, size_type ssize) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		size_type result_size = getEnableSize(length, spos, ssize);
		assignBytes(
			result_size
				? reinterpret_cast<byte_type const*>(instance->begin()) + spos
				: 0
				,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignOctet(tTJSVariantOctet const* source) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		assignBytes(
			length
				? data
				: 0
				,
			length
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignOctet(tTJSVariantOctet const* source, size_type spos) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos);
		assignBytes(
			result_size
				? data + spos
				: 0
				,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		assignBytes(
			result_size
				? data + spos
				: 0
				,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignString(tjs_char const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		assignBytes(
			length
				? data
				: 0
				,
			length
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignString(tjs_char const* source, size_type spos) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		assignBytes(
			result_size
				? data + spos
				: 0
				,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignString(tjs_char const* source, size_type spos, size_type ssize) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		assignBytes(
			result_size
				? data + spos
				: 0
				,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignNarrowString(tjs_nchar const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		assignBytes(
			length
				? data
				: 0
				,
			length
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignNarrowString(tjs_nchar const* source, size_type spos) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		assignBytes(
			result_size
				? data + spos
				: 0
				,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignNarrowString(tjs_nchar const* source, size_type spos, size_type ssize) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		assignBytes(
			result_size
				? data + spos
				: 0
				,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignStorage(tjs_char const* storage) {
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return;
		}
		assignBytes(
			binary.size()
				? &binary[0]
				: 0
				,
			binary.size()
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignStorage(tjs_char const* storage, size_type spos) {
		binary_type binary;
		if (!loadStorage(binary, storage, spos)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return;
		}
		assignBytes(
			binary.size()
				? &binary[0]
				: 0
				,
			binary.size()
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignStorage(tjs_char const* storage, size_type spos, size_type ssize) {
		binary_type binary;
		if (!loadStorage(binary, storage, spos, ssize)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return;
		}
		assignBytes(
			binary.size()
				? &binary[0]
				: 0
				,
			binary.size()
			);
	}
	KTL_INLINE void NativeOctetBuilder::assignBlock(byte_type const* first, byte_type const* last) {
		assignBytes(first, last - first);
	}
	KTL_INLINE void NativeOctetBuilder::assignBytes(byte_type const* data, size_type size) {
		binary_.resize(size);
		if (!binary_.empty()) {
			std::memmove(&binary_[0], data, size);
		}
	}
	//
	//	SUMMARY: 挿入系メソッド
	//
	KTL_INLINE void NativeOctetBuilder::insert(size_type pos, tTJSVariant* v, flag_type flag) {
		flag_type vtf = flag & vtfMask;
		switch (vtf) {
		case vtfByte:
			binary_.insert(getPos(pos), static_cast<byte_type>(v->AsInteger()));
			return;
		case vtfInt8:
			insertInteger<boost::int8_t>(pos, v->AsInteger(), flag);
			return;
		case vtfInt16:
			insertInteger<boost::int16_t>(pos, v->AsInteger(), flag);
			return;
		case vtfInt32:
			insertInteger<boost::int32_t>(pos, v->AsInteger(), flag);
			return;
		case vtfInt64:
			insertInteger<boost::int64_t>(pos, v->AsInteger(), flag);
			return;
		case vtfUInt8:
			insertInteger<boost::uint8_t>(pos, v->AsInteger(), flag);
			return;
		case vtfUInt16:
			insertInteger<boost::uint16_t>(pos, v->AsInteger(), flag);
			return;
		case vtfUInt32:
			insertInteger<boost::uint32_t>(pos, v->AsInteger(), flag);
			return;
		case vtfUInt64:
			insertInteger<boost::uint64_t>(pos, v->AsInteger(), flag);
			return;
		case vtfFloat:
			insertReal<float>(pos, v->AsReal(), flag);
			return;
		case vtfDouble:
			insertReal<double>(pos, v->AsReal(), flag);
			return;
		case vtfOctet:
			if (v->Type() != tvtOctet) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return;
			}
			return insertOctet(
				pos,
				v->AsOctetNoAddRef()
				);
		case vtfString:
			if (v->Type() != tvtString) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return;
			}
			insertString(
				pos,
				sprig::krkr::tjs::as_c_str(v->AsStringNoAddRef())
				);
			return;
		case vtfNarrowString:
			if (v->Type() != tvtString) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return;
			}
			insertNarrowString(
				pos,
				sprig::str_cast<std::basic_string<tjs_nchar> >(
					sprig::krkr::tjs::as_c_str(v->AsStringNoAddRef())
					).c_str()
				);
			return;
		case vtfBuilder:
			if (v->Type() != tvtObject) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return;
			}
			{
				iTJSDispatch2* obj = v->AsObjectNoAddRef();
				if (!obj || !sprig::krkr::tjs::IsInstanceObjectOf(obj, SPRIG_KRKR_TJS_W("OctetBuilder"))) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("型が一致しません"),
						sprig::krkr::bad_argument
						);
					return;
				}
				insertBuilder(pos, obj);
				return;
			}
		default:
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("無効なフラグです"),
				sprig::krkr::bad_argument
				);
			return;
		}
	}
	KTL_INLINE void NativeOctetBuilder::insertByte(size_type pos, size_type size, byte_type b) {
		binary_.insert(getPos(pos), size, b);
	}
	KTL_INLINE void NativeOctetBuilder::insertBuilder(size_type pos, iTJSDispatch2* source) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		if (!length) {
			return;
		}
		insertBytes(
			pos,
			reinterpret_cast<byte_type const*>(instance->begin()),
			length
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertBuilder(size_type pos, iTJSDispatch2* source, size_type spos) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return;
		}
		insertBytes(
			pos,
			reinterpret_cast<byte_type const*>(instance->begin()),
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertBuilder(size_type pos, iTJSDispatch2* source, size_type spos, size_type ssize) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return;
		}
		insertBytes(
			pos,
			reinterpret_cast<byte_type const*>(instance->begin()),
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertOctet(size_type pos, tTJSVariantOctet const* source) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		if (!length) {
			return;
		}
		insertBytes(
			pos,
			data,
			length
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertOctet(size_type pos, tTJSVariantOctet const* source, size_type spos) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return;
		}
		insertBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertOctet(size_type pos, tTJSVariantOctet const* source, size_type spos, size_type ssize) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return;
		}
		insertBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertString(size_type pos, tjs_char const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		if (!length) {
			return;
		}
		insertBytes(
			pos,
			data,
			length
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertString(size_type pos, tjs_char const* source, size_type spos) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return;
		}
		insertBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertString(size_type pos, tjs_char const* source, size_type spos, size_type ssize) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return;
		}
		insertBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertNarrowString(size_type pos, tjs_nchar const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		if (!length) {
			return;
		}
		insertBytes(
			pos,
			data,
			length
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertNarrowString(size_type pos, tjs_nchar const* source, size_type spos) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return;
		}
		insertBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertNarrowString(size_type pos, tjs_nchar const* source, size_type spos, size_type ssize) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return;
		}
		insertBytes(
			pos,
			data + spos,
			result_size
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertStorage(size_type pos, tjs_char const* storage) {
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return;
		}
		if (binary.empty()) {
			return;
		}
		return insertBytes(
			pos,
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertStorage(size_type pos, tjs_char const* storage, size_type spos) {
		binary_type binary;
		if (!loadStorage(binary, storage, spos)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return;
		}
		if (binary.empty()) {
			return;
		}
		return insertBytes(
			pos,
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertStorage(size_type pos, tjs_char const* storage, size_type spos, size_type ssize) {
		binary_type binary;
		if (!loadStorage(binary, storage, spos, ssize)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return;
		}
		if (binary.empty()) {
			return;
		}
		return insertBytes(
			pos,
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE void NativeOctetBuilder::insertBlock(size_type pos, byte_type const* first, byte_type const* last) {
		binary_.insert(getPos(pos), first, last);
	}
	KTL_INLINE void NativeOctetBuilder::insertBytes(size_type pos, byte_type const* data, size_type size) {
		insertBlock(pos, data, data + size);
	}
	//
	//	SUMMARY: 追加系メソッド
	//
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::add(tTJSVariant* v, flag_type flag) {
		flag_type vtf = flag & vtfMask;
		size_type result = binary_.size();
		switch (vtf) {
		case vtfByte:
			binary_.push_back(static_cast<byte_type>(v->AsInteger()));
			return result;
		case vtfInt8:
			return addInteger<boost::int8_t>(v->AsInteger(), flag);
		case vtfInt16:
			return addInteger<boost::int16_t>(v->AsInteger(), flag);
		case vtfInt32:
			return addInteger<boost::int32_t>(v->AsInteger(), flag);
		case vtfInt64:
			return addInteger<boost::int64_t>(v->AsInteger(), flag);
		case vtfUInt8:
			return addInteger<boost::uint8_t>(v->AsInteger(), flag);
		case vtfUInt16:
			return addInteger<boost::uint16_t>(v->AsInteger(), flag);
		case vtfUInt32:
			return addInteger<boost::uint32_t>(v->AsInteger(), flag);
		case vtfUInt64:
			return addInteger<boost::uint64_t>(v->AsInteger(), flag);
		case vtfFloat:
			return addReal<float>(v->AsReal(), flag);
		case vtfDouble:
			return addReal<double>(v->AsReal(), flag);
		case vtfOctet:
			if (v->Type() != tvtOctet) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return result;
			}
			return addOctet(
				v->AsOctetNoAddRef()
				);
		case vtfString:
			if (v->Type() != tvtString) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return result;
			}
			return addString(
				sprig::krkr::tjs::as_c_str(v->AsStringNoAddRef())
				);
		case vtfNarrowString:
			if (v->Type() != tvtString) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return result;
			}
			return addNarrowString(
				sprig::str_cast<std::basic_string<tjs_nchar> >(
					sprig::krkr::tjs::as_c_str(v->AsStringNoAddRef())
					).c_str()
				);
		case vtfBuilder:
			if (v->Type() != tvtObject) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("型が一致しません"),
					sprig::krkr::bad_argument
					);
				return result;
			}
			{
				iTJSDispatch2* obj = v->AsObjectNoAddRef();
				if (!obj || !sprig::krkr::tjs::IsInstanceObjectOf(obj, SPRIG_KRKR_TJS_W("OctetBuilder"))) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("型が一致しません"),
						sprig::krkr::bad_argument
						);
					return result;
				}
				return addBuilder(obj);
			}
		default:
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("無効なフラグです"),
				sprig::krkr::bad_argument
				);
			return result;
		}
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addByte(size_type size, byte_type b) {
		size_type result = binary_.size();
		binary_.insert(binary_.end(), size, b);
		return result;
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addBuilder(iTJSDispatch2* source) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		if (!length) {
			return length;
		}
		return addBytes(
			reinterpret_cast<byte_type const*>(instance->begin()),
			length
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addBuilder(iTJSDispatch2* source, size_type spos) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return addBytes(
			reinterpret_cast<byte_type const*>(instance->begin()),
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addBuilder(iTJSDispatch2* source, size_type spos, size_type ssize) {
		OctetBuilder* instance = reinterpret_cast<OctetBuilder*>(getInstance(source));
		size_type length = static_cast<size_type>(instance->size());
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return addBytes(
			reinterpret_cast<byte_type const*>(instance->begin()),
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addOctet(tTJSVariantOctet const* source) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		if (!length) {
			return length;
		}
		return addBytes(
			data,
			length
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addOctet(tTJSVariantOctet const* source, size_type spos) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return addBytes(
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize) {
		tjs_uint length = sprig::krkr::tjs::octet_length(source);
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return addBytes(
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addString(tjs_char const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		if (!length) {
			return length;
		}
		return addBytes(
			data,
			length
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addString(tjs_char const* source, size_type spos) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return addBytes(
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addString(tjs_char const* source, size_type spos, size_type ssize) {
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return addBytes(
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addNarrowString(tjs_nchar const* source) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		if (!length) {
			return length;
		}
		return addBytes(
			data,
			length
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addNarrowString(tjs_nchar const* source, size_type spos) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos);
		if (!result_size) {
			return result_size;
		}
		return addBytes(
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addNarrowString(tjs_nchar const* source, size_type spos, size_type ssize) {
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		byte_type const* data = reinterpret_cast<byte_type const*>(source);
		size_type result_size = getEnableSize(length, spos, ssize);
		if (!result_size) {
			return result_size;
		}
		return addBytes(
			data + spos,
			result_size
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addStorage(tjs_char const* storage) {
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return 0;
		}
		if (!binary.size()) {
			return binary.size();
		}
		return addBytes(
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addStorage(tjs_char const* storage, size_type spos) {
		binary_type binary;
		if (!loadStorage(binary, storage, spos)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return 0;
		}
		if (!binary.size()) {
			return binary.size();
		}
		return addBytes(
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addStorage(tjs_char const* storage, size_type spos, size_type ssize) {
		binary_type binary;
		if (!loadStorage(binary, storage, spos, ssize)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return 0;
		}
		if (!binary.size()) {
			return binary.size();
		}
		return addBytes(
			&binary[0],
			binary.size()
			);
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addBlock(byte_type const* first, byte_type const* last) {
		size_type result = binary_.size();
		binary_.insert(binary_.end(), first, last);
		return result;
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::addBytes(byte_type const* data, size_type size) {
		return addBlock(data, data + size);
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE void NativeOctetBuilder::clear() {
		binary_.clear();
	}
	//
	//	SUMMARY: 削除系メソッド
	//
	KTL_INLINE void NativeOctetBuilder::erase(size_type pos) {
		if (isOutOfRange(pos)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("インデックスが範囲外です"),
				sprig::krkr::bad_argument
				);
		}
		binary_.erase(getPos(pos));
	}
	KTL_INLINE void NativeOctetBuilder::erase(size_type pos, size_type size) {
		if (isOutOfRange(pos) || isOutOfRange(pos + size)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("インデックスが範囲外です"),
				sprig::krkr::bad_argument
				);
		}
		binary_.erase(getPos(pos), getPos(pos + size));
	}
	//
	//	SUMMARY: 前後削除系メソッド
	//
	KTL_INLINE NativeOctetBuilder::int_type NativeOctetBuilder::pop() {
		emptyCheck();
		int_type result = binary_.back();
		binary_.pop_back();
		return result;
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::pop(size_type size) {
		size_type result = size >= binary_.size()
			? size
			: binary_.size()
			;
		binary_.erase(
			getPos(binary_.size() - result),
			binary_.end()
			);
		return result;
	}
	KTL_INLINE NativeOctetBuilder::int_type NativeOctetBuilder::shift() {
		emptyCheck();
		int_type result = binary_.front();
		binary_.erase(binary_.begin());
		return result;
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::shift(size_type size) {
		size_type result = size >= binary_.size()
			? size
			: binary_.size()
			;
		binary_.erase(
			binary_.begin(),
			getPos(result)
			);
		return result;
	}
	//
	//	SUMMARY: 変換系メソッド
	//
	KTL_INLINE tTJSVariant NativeOctetBuilder::as(size_type pos, flag_type flag) const {
		flag_type vtf = flag & vtfMask;
		switch (vtf) {
		case vtfByte:
			if (isOutOfRange(pos)) {
				return tTJSVariant();
			}
			return binary_[pos];
		case vtfInt8:
			return asInteger<boost::int8_t>(pos, flag);
		case vtfInt16:
			return asInteger<boost::int16_t>(pos, flag);
		case vtfInt32:
			return asInteger<boost::int32_t>(pos, flag);
		case vtfInt64:
			return asInteger<boost::int64_t>(pos, flag);
		case vtfUInt8:
			return asInteger<boost::uint8_t>(pos, flag);
		case vtfUInt16:
			return asInteger<boost::uint16_t>(pos, flag);
		case vtfUInt32:
			return asInteger<boost::uint32_t>(pos, flag);
		case vtfUInt64:
			return asInteger<boost::uint64_t>(pos, flag);
		case vtfFloat:
			return asReal<float>(pos, flag);
		case vtfDouble:
			return asReal<double>(pos, flag);
		case vtfOctet:
			return asOctet(pos);
		case vtfString:
			return asString(pos);
		case vtfNarrowString:
			return asNarrowString(pos);
		case vtfBuilder:
			return asBuilder(pos);
		default:
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("無効なフラグです"),
				sprig::krkr::bad_argument
				);
			return tTJSVariant();
		}
	}
	KTL_INLINE tTJSVariant NativeOctetBuilder::asBuilder() const {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		size_type result_size = binary_.size();
		if (!result_size) {
			return sprig::krkr::tjs::as_object_closure_variant(result);
		}
		OctetBuilder* ptr = reinterpret_cast<OctetBuilder*>(getInstance(sprig::get_pointer(result)));
		ptr->init(&binary_[0], &binary_[0] + result_size);
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeOctetBuilder::asBuilder(size_type pos) const {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		size_type result_size = getEnableSize(binary_.size(), pos);
		if (!result_size) {
			return sprig::krkr::tjs::as_object_closure_variant(result);
		}
		OctetBuilder* ptr = reinterpret_cast<OctetBuilder*>(getInstance(sprig::get_pointer(result)));
		ptr->init(&binary_[pos], &binary_[pos] + result_size);
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeOctetBuilder::asBuilder(size_type pos, size_type size) const {
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		size_type result_size = getEnableSize(binary_.size(), pos, size);
		if (!result_size) {
			return sprig::krkr::tjs::as_object_closure_variant(result);
		}
		OctetBuilder* ptr = reinterpret_cast<OctetBuilder*>(getInstance(sprig::get_pointer(result)));
		ptr->init(&binary_[pos], &binary_[pos] + result_size);
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeOctetBuilder::asOctet() const {
		size_type result_size = binary_.size();
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(&binary_[0], result_size);
	}
	KTL_INLINE tTJSVariant NativeOctetBuilder::asOctet(size_type pos) const {
		size_type result_size = getEnableSize(binary_.size(), pos);
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(&binary_[pos], result_size);
	}
	KTL_INLINE tTJSVariant NativeOctetBuilder::asOctet(size_type pos, size_type size) const {
		size_type result_size = getEnableSize(binary_.size(), pos, size);
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(&binary_[pos], result_size);
	}
	KTL_INLINE tTJSVariant NativeOctetBuilder::asString() const {
		binary_type binary(binary_);
		binary.insert(binary.end(), sizeof(tjs_char), 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
	}
	tTJSVariant NativeOctetBuilder::asString(size_type pos) const {
		size_type result_size = getEnableSize(binary_.size(), pos);
		binary_type binary(
			result_size
				? boost::next(binary_.begin(), pos)
				: binary_.end()
				,
			binary_.end()
			);
		binary.insert(binary.end(), sizeof(tjs_char) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
	}
	tTJSVariant NativeOctetBuilder::asString(size_type pos, size_type size) const {
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
		binary.insert(binary.end(), sizeof(tjs_char) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
	}
	KTL_INLINE tTJSVariant NativeOctetBuilder::asNarrowString() const {
		binary_type binary(binary_);
		binary.insert(binary.end(), sizeof(tjs_nchar) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&binary[0]));
	}
	tTJSVariant NativeOctetBuilder::asNarrowString(size_type pos) const {
		size_type result_size = getEnableSize(binary_.size(), pos);
		binary_type binary(
			result_size
				? boost::next(binary_.begin(), pos)
				: binary_.end()
				,
			binary_.end()
			);
		binary.insert(binary.end(), sizeof(tjs_nchar) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&binary[0]));
	}
	tTJSVariant NativeOctetBuilder::asNarrowString(size_type pos, size_type size) const {
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
		binary.insert(binary.end(), sizeof(tjs_nchar) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&binary[0]));
	}
	//
	//	SUMMARY: ストレージ系メソッド
	//
	KTL_INLINE NativeOctetBuilder::int_type NativeOctetBuilder::saveStorage(tjs_char const* storage) const {
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
	KTL_INLINE NativeOctetBuilder::int_type NativeOctetBuilder::saveStorage(tjs_char const* storage, size_type pos) const {
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
	KTL_INLINE NativeOctetBuilder::int_type NativeOctetBuilder::saveStorage(tjs_char const* storage, size_type pos, size_type size) const {
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
	//	SUMMARY: 演算系メソッド
	//
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::transformAndByte(byte_type b) {
		BOOST_FOREACH(byte_type& e, binary_) {
			e &= b;
		}
		return binary_.size();
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::transformOrByte(byte_type b) {
		BOOST_FOREACH(byte_type& e, binary_) {
			e |= b;
		}
		return binary_.size();
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::transformXorByte(byte_type b) {
		BOOST_FOREACH(byte_type& e, binary_) {
			e ^= b;
		}
		return binary_.size();
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::transformNotByte() {
		BOOST_FOREACH(byte_type& e, binary_) {
			e = ~e;
		}
		return binary_.size();
	}
	//
	//	SUMMARY: 検索系メソッド
	//
	KTL_INLINE NativeOctetBuilder::int_type NativeOctetBuilder::findByte(byte_type b) const {
		const_iterator first = binary_.begin();
		const_iterator last = binary_.end();
		const_iterator where = std::find(first, last, b);
		return where != last
			? std::distance(first, where)
			: -1
			;
	}
	KTL_INLINE NativeOctetBuilder::int_type NativeOctetBuilder::findByte(byte_type b, size_type pos) const {
		if (isOutOfRange(pos)) {
			return -1;
		}
		const_iterator first = boost::next(binary_.begin(), pos);
		const_iterator last = binary_.end();
		const_iterator where = std::find(first, last, b);
		return where != last
			? std::distance(first, where)
			: -1
			;
	}
	KTL_INLINE NativeOctetBuilder::int_type NativeOctetBuilder::findByte(byte_type b, size_type pos, size_type size) const {
		if (isOutOfRange(pos)) {
			return -1;
		}
		size_type process_size = getEnableSize(binary_.size(), pos, size);
		if (!process_size) {
			return -1;
		}
		const_iterator first = boost::next(binary_.begin(), pos);
		const_iterator last = boost::next(binary_.begin(), pos + process_size);
		const_iterator where = std::find(first, last, b);
		return where != last
			? std::distance(first, where)
			: -1
			;
	}
	//
	//	SUMMARY: メモリ系メソッド
	//
	KTL_INLINE void NativeOctetBuilder::copy(sprig::krkr::tjs::intptr_type dest, size_type pos, size_type size) const {
		if (isOutOfRangeOff(pos) || isOutOfRangeOff(pos + size)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("オフセットが範囲外です"),
				sprig::krkr::bad_argument
				);
		}
		std::memmove(reinterpret_cast<void*>(dest), &binary_[pos], size);
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	KTL_INLINE NativeOctetBuilder::int_type NativeOctetBuilder::back() const {
		emptyCheck();
		return binary_.back();
	}
	KTL_INLINE NativeOctetBuilder::int_type NativeOctetBuilder::front() const {
		emptyCheck();
		return binary_.front();
	}
	//
	//	SUMMARY: ポインタ取得プロパティ系メソッド
	//
	KTL_INLINE NativeOctetBuilder::byte_type* NativeOctetBuilder::begin() {
		return binary_.empty()
			? 0
			: &binary_[0]
			;
	}
	KTL_INLINE NativeOctetBuilder::byte_type* NativeOctetBuilder::end() {
		return binary_.empty()
			? 0
			: &binary_[0] + binary_.size()
			;
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool NativeOctetBuilder::empty() const {
		return binary_.empty();
	}
	KTL_INLINE NativeOctetBuilder::size_type NativeOctetBuilder::size() const {
		return binary_.size();
	}
	KTL_INLINE void NativeOctetBuilder::resize(size_type size) {
		binary_.resize(size);
	}

	//
	// OctetBuilder
	//
	OctetBuilder::OctetBuilder() {}
	tjs_error TJS_INTF_METHOD OctetBuilder::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("OctetBuilder::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeOctetBuilder>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD OctetBuilder::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("OctetBuilder::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	KTL_INLINE NativeOctetBuilder::iterator OctetBuilder::getBegin() {
		return instance_->getBegin();
	}
	KTL_INLINE NativeOctetBuilder::iterator OctetBuilder::getEnd() {
		return instance_->getEnd();
	}
	//
	//	SUMMARY: 生成系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type OctetBuilder::fromHex(tTJSVariantString const* data) {
		return NativeOctetBuilder::fromHex(data);
	}
	//
	//	SUMMARY: 内部系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type OctetBuilder::getInstance() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(this);
	}
	//
	//	SUMMARY: 書込系メソッド
	//
	KTL_INLINE tTVInteger OctetBuilder::set(tTVInteger pos, tTJSVariant* v, tTVInteger flag) {
		return instance_->set(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			v,
			sprig::numeric::bit_cast<NativeOctetBuilder::flag_type>(flag)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setByte(tTVInteger pos, tTVInteger size, tTVInteger b) {
		return instance_->setByte(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size),
			static_cast<NativeOctetBuilder::byte_type>(b)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setBuilder(tTVInteger pos, iTJSDispatch2* source) {
		return instance_->setBuilder(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setBuilder(tTVInteger pos, iTJSDispatch2* source, tTVInteger spos) {
		return instance_->setBuilder(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setBuilder(tTVInteger pos, iTJSDispatch2* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->setBuilder(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setOctet(tTVInteger pos, tTJSVariantOctet const* source) {
		return instance_->setOctet(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setOctet(tTVInteger pos, tTJSVariantOctet const* source, tTVInteger spos) {
		return instance_->setOctet(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setOctet(tTVInteger pos, tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->setOctet(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setString(tTVInteger pos, tTJSVariantString const* source) {
		return instance_->setString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos) {
		return instance_->setString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->setString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setNarrowString(tTVInteger pos, tTJSVariantString const* source) {
		return instance_->setNarrowString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setNarrowString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos) {
		return instance_->setNarrowString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setNarrowString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->setNarrowString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setStorage(tTVInteger pos, tTJSVariantString const* storage) {
		return instance_->setStorage(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setStorage(tTVInteger pos, tTJSVariantString const* storage, tTVInteger spos) {
		return instance_->setStorage(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setStorage(tTVInteger pos, tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize) {
		return instance_->setStorage(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setBlock(
		tTVInteger pos,
		sprig::krkr::tjs::intptr_type first,
		sprig::krkr::tjs::intptr_type last
		)
	{
		return instance_->setBlock(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(first),
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(last)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::setBytes(
		tTVInteger pos,
		sprig::krkr::tjs::intptr_type data,
		tTVInteger size
		)
	{
		return instance_->setBytes(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(data),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	//
	//	SUMMARY: 割当系メソッド
	//
	KTL_INLINE void OctetBuilder::assign(tTJSVariant* v, tTVInteger flag) {
		instance_->assign(
			v,
			sprig::numeric::bit_cast<NativeOctetBuilder::flag_type>(flag)
			);
	}
	KTL_INLINE void OctetBuilder::assignByte(tTVInteger size, tTVInteger b) {
		instance_->assignByte(
			boost::numeric_cast<NativeOctetBuilder::size_type>(size),
			static_cast<NativeOctetBuilder::byte_type>(b)
			);
	}
	KTL_INLINE void OctetBuilder::assignBuilder(iTJSDispatch2* source) {
		instance_->assignBuilder(source);
	}
	KTL_INLINE void OctetBuilder::assignBuilder(iTJSDispatch2* source, tTVInteger spos) {
		instance_->assignBuilder(
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE void OctetBuilder::assignBuilder(iTJSDispatch2* source, tTVInteger spos, tTVInteger ssize) {
		instance_->assignBuilder(
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE void OctetBuilder::assignOctet(tTJSVariantOctet const* source) {
		instance_->assignOctet(source);
	}
	KTL_INLINE void OctetBuilder::assignOctet(tTJSVariantOctet const* source, tTVInteger spos) {
		instance_->assignOctet(
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE void OctetBuilder::assignOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize) {
		instance_->assignOctet(
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE void OctetBuilder::assignString(tTJSVariantString const* source) {
		instance_->assignString(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE void OctetBuilder::assignString(tTJSVariantString const* source, tTVInteger spos) {
		instance_->assignString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE void OctetBuilder::assignString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		instance_->assignString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE void OctetBuilder::assignNarrowString(tTJSVariantString const* source) {
		instance_->assignNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
			);
	}
	KTL_INLINE void OctetBuilder::assignNarrowString(tTJSVariantString const* source, tTVInteger spos) {
		instance_->assignNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE void OctetBuilder::assignNarrowString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		instance_->assignNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE void OctetBuilder::assignStorage(tTJSVariantString const* storage) {
		instance_->assignStorage(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE void OctetBuilder::assignStorage(tTJSVariantString const* storage, tTVInteger spos) {
		instance_->assignStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE void OctetBuilder::assignStorage(tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize) {
		instance_->assignStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE void OctetBuilder::assignBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last) {
		instance_->assignBlock(
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(first),
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(last)
			);
	}
	KTL_INLINE void OctetBuilder::assignBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size) {
		instance_->assignBytes(
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(data),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	//
	//	SUMMARY: 挿入系メソッド
	//
	KTL_INLINE void OctetBuilder::insert(tTVInteger pos, tTJSVariant* v, tTVInteger flag) {
		instance_->insert(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			v,
			sprig::numeric::bit_cast<NativeOctetBuilder::flag_type>(flag)
			);
	}
	KTL_INLINE void OctetBuilder::insertByte(tTVInteger pos, tTVInteger size, tTVInteger b) {
		instance_->insertByte(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size),
			static_cast<NativeOctetBuilder::byte_type>(b)
			);
	}
	KTL_INLINE void OctetBuilder::insertBuilder(tTVInteger pos, iTJSDispatch2* source) {
		instance_->insertBuilder(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source
			);
	}
	KTL_INLINE void OctetBuilder::insertBuilder(tTVInteger pos, iTJSDispatch2* source, tTVInteger spos) {
		instance_->insertBuilder(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE void OctetBuilder::insertBuilder(tTVInteger pos, iTJSDispatch2* source, tTVInteger spos, tTVInteger ssize) {
		instance_->insertBuilder(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE void OctetBuilder::insertOctet(tTVInteger pos, tTJSVariantOctet const* source) {
		instance_->insertOctet(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source
			);
	}
	KTL_INLINE void OctetBuilder::insertOctet(tTVInteger pos, tTJSVariantOctet const* source, tTVInteger spos) {
		instance_->insertOctet(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE void OctetBuilder::insertOctet(tTVInteger pos, tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize) {
		instance_->insertOctet(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE void OctetBuilder::insertString(tTVInteger pos, tTJSVariantString const* source) {
		instance_->insertString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE void OctetBuilder::insertString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos) {
		instance_->insertString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE void OctetBuilder::insertString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		instance_->insertString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE void OctetBuilder::insertNarrowString(tTVInteger pos, tTJSVariantString const* source) {
		instance_->insertNarrowString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
			);
	}
	KTL_INLINE void OctetBuilder::insertNarrowString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos) {
		instance_->insertNarrowString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE void OctetBuilder::insertNarrowString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		instance_->insertNarrowString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE void OctetBuilder::insertStorage(tTVInteger pos, tTJSVariantString const* storage) {
		instance_->insertStorage(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE void OctetBuilder::insertStorage(tTVInteger pos, tTJSVariantString const* storage, tTVInteger spos) {
		instance_->insertStorage(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE void OctetBuilder::insertStorage(tTVInteger pos, tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize) {
		instance_->insertStorage(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE void OctetBuilder::insertBlock(
		tTVInteger pos,
		sprig::krkr::tjs::intptr_type first,
		sprig::krkr::tjs::intptr_type last
		)
	{
		instance_->insertBlock(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(first),
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(last)
			);
	}
	KTL_INLINE void OctetBuilder::insertBytes(
		tTVInteger pos,
		sprig::krkr::tjs::intptr_type data,
		tTVInteger size
		)
	{
		instance_->insertBytes(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(data),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	//
	//	SUMMARY: 追加系メソッド
	//
	KTL_INLINE tTVInteger OctetBuilder::add(tTJSVariant* v, tTVInteger flag) {
		return instance_->add(
			v,
			sprig::numeric::bit_cast<NativeOctetBuilder::flag_type>(flag)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addByte(tTVInteger size, tTVInteger b) {
		return instance_->addByte(
			boost::numeric_cast<NativeOctetBuilder::size_type>(size),
			static_cast<NativeOctetBuilder::byte_type>(b)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addBuilder(iTJSDispatch2* source) {
		return instance_->addBuilder(source);
	}
	KTL_INLINE tTVInteger OctetBuilder::addBuilder(iTJSDispatch2* source, tTVInteger spos) {
		return instance_->addBuilder(
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addBuilder(iTJSDispatch2* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->addBuilder(
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addOctet(tTJSVariantOctet const* source) {
		return instance_->addOctet(source);
	}
	KTL_INLINE tTVInteger OctetBuilder::addOctet(tTJSVariantOctet const* source, tTVInteger spos) {
		return instance_->addOctet(
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->addOctet(
			source,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addString(tTJSVariantString const* source) {
		return instance_->addString(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addString(tTJSVariantString const* source, tTVInteger spos) {
		return instance_->addString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->addString(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addNarrowString(tTJSVariantString const* source) {
		return instance_->addNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addNarrowString(tTJSVariantString const* source, tTVInteger spos) {
		return instance_->addNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addNarrowString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize) {
		return instance_->addNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(
				sprig::krkr::tjs::as_c_str(source)
				).c_str()
				,
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addStorage(tTJSVariantString const* storage) {
		return instance_->addStorage(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addStorage(tTJSVariantString const* storage, tTVInteger spos) {
		return instance_->addStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addStorage(tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize) {
		return instance_->addStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeOctetBuilder::size_type>(spos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(ssize)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last) {
		return instance_->addBlock(
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(first),
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(last)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::addBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size) {
		return instance_->addBytes(
			reinterpret_cast<NativeOctetBuilder::byte_type const*>(data),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE void OctetBuilder::clear() {
		instance_->clear();
	}
	//
	//	SUMMARY: 削除系メソッド
	//
	KTL_INLINE void OctetBuilder::erase(tTVInteger pos) {
		instance_->erase(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos)
			);
	}
	KTL_INLINE void OctetBuilder::erase(tTVInteger pos, tTVInteger size) {
		instance_->erase(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	//
	//	SUMMARY: 前後削除系メソッド
	//
	KTL_INLINE tTVInteger OctetBuilder::pop() {
		return instance_->pop();
	}
	KTL_INLINE tTVInteger OctetBuilder::pop(tTVInteger size) {
		return instance_->pop(
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::shift() {
		return instance_->shift();
	}
	KTL_INLINE tTVInteger OctetBuilder::shift(tTVInteger size) {
		return instance_->shift(
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	//
	//	SUMMARY: 変換系メソッド
	//
	KTL_INLINE tTJSVariant OctetBuilder::as(tTVInteger pos, tTVInteger flag) const {
		return instance_->as(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			sprig::numeric::bit_cast<NativeOctetBuilder::flag_type>(flag)
			);
	}
	KTL_INLINE tTJSVariant OctetBuilder::asBuilder() const {
		return instance_->asBuilder();
	}
	KTL_INLINE tTJSVariant OctetBuilder::asBuilder(tTVInteger pos) const {
		return instance_->asBuilder(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant OctetBuilder::asBuilder(tTVInteger pos, tTVInteger size) const {
		return instance_->asBuilder(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant OctetBuilder::asOctet() const {
		return instance_->asOctet();
	}
	KTL_INLINE tTJSVariant OctetBuilder::asOctet(tTVInteger pos) const {
		return instance_->asOctet(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant OctetBuilder::asOctet(tTVInteger pos, tTVInteger size) const {
		return instance_->asOctet(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant OctetBuilder::asString() const {
		return instance_->asString();
	}
	KTL_INLINE tTJSVariant OctetBuilder::asString(tTVInteger pos) const {
		return instance_->asString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant OctetBuilder::asString(tTVInteger pos, tTVInteger size) const {
		return instance_->asString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant OctetBuilder::asNarrowString() const {
		return instance_->asNarrowString();
	}
	KTL_INLINE tTJSVariant OctetBuilder::asNarrowString(tTVInteger pos) const {
		return instance_->asNarrowString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant OctetBuilder::asNarrowString(tTVInteger pos, tTVInteger size) const {
		return instance_->asNarrowString(
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	//
	//	SUMMARY: ストレージ系メソッド
	//
	KTL_INLINE tTVInteger OctetBuilder::saveStorage(tTJSVariantString const* storage) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::saveStorage(tTJSVariantString const* storage, tTVInteger pos) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::saveStorage(tTJSVariantString const* storage, tTVInteger pos, tTVInteger size) const {
		return instance_->saveStorage(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	//
	//	SUMMARY: 演算系メソッド
	//
	KTL_INLINE tTVInteger OctetBuilder::transformAndByte(tTVInteger b) {
		return instance_->transformAndByte(
			static_cast<NativeOctetBuilder::byte_type>(b)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::transformOrByte(tTVInteger b) {
		return instance_->transformOrByte(
			static_cast<NativeOctetBuilder::byte_type>(b)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::transformXorByte(tTVInteger b) {
		return instance_->transformXorByte(
			static_cast<NativeOctetBuilder::byte_type>(b)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::transformNotByte() {
		return instance_->transformNotByte();
	}
	//
	//	SUMMARY: 検索系メソッド
	//
	KTL_INLINE tTVInteger OctetBuilder::findByte(tTVInteger b) const {
		return instance_->findByte(
			static_cast<NativeOctetBuilder::byte_type>(b)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::findByte(tTVInteger b, tTVInteger pos) const {
		return instance_->findByte(
			static_cast<NativeOctetBuilder::byte_type>(b),
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos)
			);
	}
	KTL_INLINE tTVInteger OctetBuilder::findByte(tTVInteger b, tTVInteger pos, tTVInteger size) const {
		return instance_->findByte(
			static_cast<NativeOctetBuilder::byte_type>(b),
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	//
	//	SUMMARY: メモリ系メソッド
	//
	KTL_INLINE void OctetBuilder::copy(sprig::krkr::tjs::intptr_type dest, tTVInteger pos, tTVInteger size) const {
		instance_->copy(
			dest,
			boost::numeric_cast<NativeOctetBuilder::size_type>(pos),
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
	//
	//	SUMMARY: 取得プロパティ系メソッド
	//
	KTL_INLINE tTVInteger OctetBuilder::back() const {
		return instance_->back();
	}
	KTL_INLINE tTVInteger OctetBuilder::front() const {
		return instance_->front();
	}
	//
	//	SUMMARY: ポインタ取得プロパティ系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type OctetBuilder::begin() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(instance_->begin());
	}
	KTL_INLINE sprig::krkr::tjs::intptr_type OctetBuilder::end() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(instance_->end());
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool OctetBuilder::empty() const {
		return instance_->empty();
	}
	KTL_INLINE tTVInteger OctetBuilder::size() const {
		return instance_->size();
	}
	KTL_INLINE void OctetBuilder::resize(tTVInteger size) {
		instance_->resize(
			boost::numeric_cast<NativeOctetBuilder::size_type>(size)
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_OCTET_BUILDER_OCTET_BUILDER_HPP
