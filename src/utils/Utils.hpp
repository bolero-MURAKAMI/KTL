/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_UTILS_UTILS_HPP
#define SRC_UTILS_UTILS_HPP

#include <iosfwd>
#include <iterator>
#include <algorithm>
#include <vector>
#include <sstream>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/optional/optional.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/str_length.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "UtilsDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Utils:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Utils:error")

namespace ktl {
	//
	// NativeUtils
	//
	//
	// NativeUtils::CallbackEnumMembers
	//
	KTL_INLINE NativeUtils::CallbackEnumMembers::CallbackEnumMembers()
		: result_obj_(
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
				0, 0, 0
				)
			)
		, result_(sprig::krkr::tjs::as_object_closure_variant(result_obj_))
	{}
	tjs_error TJS_INTF_METHOD NativeUtils::CallbackEnumMembers::FuncCall(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis)
	{
		if (numparams < 3) {
			return TJS_E_BADPARAMCOUNT;
		}
		if (param[1]->AsInteger() == 0) {
			sprig::krkr::tjs::AddMember(result_obj_, tTJSString(*param[0]).c_str(), *param[2]);
		}
		if (result) {
			*result = true;
		}
		return TJS_S_OK;
	}
	KTL_INLINE tTJSVariant const& NativeUtils::CallbackEnumMembers::result() const {
		return result_;
	}
	//
	// NativeUtils::CallbackEnumMembersName
	//
	KTL_INLINE NativeUtils::CallbackEnumMembersName::CallbackEnumMembersName()
		: result_obj_(
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				0, 0, 0
				)
			)
		, result_(sprig::krkr::tjs::as_object_closure_variant(result_obj_))
		, num_(0)
	{}
	tjs_error TJS_INTF_METHOD NativeUtils::CallbackEnumMembersName::FuncCall(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis)
	{
		if (numparams < 2) {
			return TJS_E_BADPARAMCOUNT;
		}
		if (param[1]->AsInteger() == 0) {
			sprig::krkr::tjs::AddMember(result_obj_, num_++, *param[0]);
		}
		if (result) {
			*result = true;
		}
		return TJS_S_OK;
	}
	KTL_INLINE tTJSVariant const& NativeUtils::CallbackEnumMembersName::result() const {
		return result_;
	}
	//
	// NativeUtils::CallbackEnumMembersSaveStruct
	//
	template<typename Writer>
	KTL_INLINE NativeUtils::CallbackEnumMembersSaveStruct<Writer>::CallbackEnumMembersSaveStruct(Writer& w, object_stack_type& stack)
		: writer_(&w)
		, obj_stack_(&stack)
		, is_not_first_(false)
	{}
	template<typename Writer>
	tjs_error TJS_INTF_METHOD NativeUtils::CallbackEnumMembersSaveStruct<Writer>::FuncCall(
		tjs_uint32 flag,
		tjs_char const* membername,
		tjs_uint32* hint,
		tTJSVariant* result,
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* objthis)
	{
		if (numparams < 3) {
			return TJS_E_BADPARAMCOUNT;
		}
		if (param[1]->AsInteger() != 0) {
			return TJS_S_OK;
		}
		if (is_not_first_) {
			writer_->write(SPRIG_KRKR_TJS_W(","));
		} else {
			is_not_first_ = true;
		}
		writer_->newline();
		writer_->write(::TJSVariantToExpressionString(*param[0]).c_str());
		writer_->write(SPRIG_KRKR_TJS_W(" => "));
		return writeSaveStructString(*writer_, *param[2], *obj_stack_);
	}
	template<typename Writer>
	KTL_INLINE Writer& NativeUtils::CallbackEnumMembersSaveStruct<Writer>::writer() const {
		return *writer_;
	}
	//
	// SaveStructStringWriter
	//
	KTL_INLINE NativeUtils::SaveStructStringWriter::SaveStructStringWriter(
		flag_type flag,
		tjs_char indent_char,
		size_type indent_count
		)
		: flag_(flag)
		, indent_char_(indent_char)
		, indent_count_(indent_count)
		, indent_()
		, oss_()
	{}
	KTL_INLINE void NativeUtils::SaveStructStringWriter::write(tjs_string_type const& s) {
		oss_ << s;
	}
	KTL_INLINE void NativeUtils::SaveStructStringWriter::write(tjs_char const* s) {
		oss_ << s;
	}
	KTL_INLINE void NativeUtils::SaveStructStringWriter::write(tjs_char c) {
		oss_ << c;
	}
	KTL_INLINE void NativeUtils::SaveStructStringWriter::newline() {
		oss_ << SPRIG_KRKR_TJS_W("\r\n") << indent_;
	}
	KTL_INLINE void NativeUtils::SaveStructStringWriter::push_indent() {
		indent_.append(indent_count_, indent_char_);
	}
	KTL_INLINE void NativeUtils::SaveStructStringWriter::pop_indent() {
		indent_.erase(indent_.size() - indent_count_);
	}
	KTL_INLINE void NativeUtils::SaveStructStringWriter::write_const() {
		if (!(flag_ & sfNoConstQualifier)) {
			oss_ << SPRIG_KRKR_TJS_W("(const) ");
		}
	}
	KTL_INLINE NativeUtils::tjs_string_type NativeUtils::SaveStructStringWriter::result() const {
		return oss_.str();
	}

	template<typename Writer>
	KTL_INLINE tjs_error NativeUtils::writeSaveStructString(Writer& writer, tTJSVariant const& v, object_stack_type& stack) {
		switch (v.Type()) {
		case tvtVoid:
		case tvtString:
		case tvtInteger:
		case tvtReal:
		case tvtOctet:
			writer.write(::TJSVariantToExpressionString(v).c_str());
			break;
		case tvtObject:
			{
				tTJSVariantClosure const& closure = v.AsObjectClosureNoAddRef();
				if (!closure.Object) {
					writer.write(SPRIG_KRKR_TJS_W("null"));
					break;
				}
				if (closure.ObjThis && sprig::krkr::tjs::IsInstanceObjectOf(closure.Object, SPRIG_KRKR_TJS_W("Array"))) {
					if (std::find(stack.begin(), stack.end(), closure.Object) != stack.end()) {
						writer.write(SPRIG_KRKR_TJS_W("null /* object recursion detected */"));
						break;
					}
					stack.push_back(closure.Object);
					writer.write_const();
					writer.write(SPRIG_KRKR_TJS_W("["));
					writer.push_indent();
					tjs_error error = TJS_S_OK;
					{
						tjs_int count = sprig::krkr::tjs::GetPropValue<tjs_int>(closure.Object, SPRIG_KRKR_TJS_W("count"));
						if (count) {
							writer.newline();
							error = writeSaveStructString(writer, sprig::krkr::tjs::GetPropValue<tTJSVariant>(closure.Object, 0), stack);
							if (!TJS_FAILED(error)) {
								for (tjs_int i = 1; i != count; ++i) {
									writer.write(SPRIG_KRKR_TJS_W(","));
									writer.newline();
									error = writeSaveStructString(writer, sprig::krkr::tjs::GetPropValue<tTJSVariant>(closure.Object, i), stack);
									if (TJS_FAILED(error)) {
										break;
									}
								}
							}
						}
					}
					writer.pop_indent();
					writer.newline();
					writer.write(SPRIG_KRKR_TJS_W("]"));
					stack.pop_back();
					if (TJS_FAILED(error)) {
						return error;
					}
				} else if (closure.ObjThis && sprig::krkr::tjs::IsInstanceObjectOf(closure.Object, SPRIG_KRKR_TJS_W("Dictionary"))) {
					if (std::find(stack.begin(), stack.end(), closure.Object) != stack.end()) {
						writer.write(SPRIG_KRKR_TJS_W("null /* object recursion detected */"));
						break;
					}
					stack.push_back(closure.Object);
					writer.write_const();
					writer.write(SPRIG_KRKR_TJS_W("%["));
					writer.push_indent();
					tjs_error error = TJS_S_OK;
					{
						CallbackEnumMembersSaveStruct<Writer> callback(writer, stack);
						tTJSVariantClosure caller(&callback);
						error = closure.Object->EnumMembers(0, &caller, closure.Object);
					}
					writer.pop_indent();
					writer.newline();
					writer.write(SPRIG_KRKR_TJS_W("]"));
					stack.pop_back();
					if (TJS_FAILED(error)) {
						return error;
					}
				} else {
					writer.write(SPRIG_KRKR_TJS_W("null /* (object) \""));
					writer.write(sprig::krkr::tjs::escape_c(tTJSString(v)).c_str());
					writer.write(SPRIG_KRKR_TJS_W("\" */"));
				}
			}
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	KTL_INLINE NativeUtils::int_type NativeUtils::saveStorage(
		tjs_char const* storage,
		void const* data,
		ULONG length
		)
	{
		sprig::com_ptr<IStream> out(::TVPCreateIStream(storage, TJS_BS_WRITE));
		if (!out) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return -1;
		}
		if (!length) {
			return length;
		}
		ULONG io_size = 0;
		if (FAILED(out->Write(data, length, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ファイル書込に失敗しました"),
				sprig::krkr::internal_error
				);
			return io_size;
		}
		return io_size;
	}
	NativeUtils::NativeUtils() {}
	//
	//	SUMMARY: オブジェクト系メソッド
	//
	KTL_INLINE tTJSVariant NativeUtils::getCii(iTJSDispatch2* obj) {
		return getCii(obj, 0);
	}
	KTL_INLINE tTJSVariant NativeUtils::getCii(iTJSDispatch2* obj, size_type num) {
		if (!obj) {
			return tTJSVariant();
		}
		tTJSVariant result;
		if (TJS_FAILED(obj->ClassInstanceInfo(TJS_CII_GET, num, &result))) {
			return tTJSVariant();
		}
		return result;
	}
	KTL_INLINE tTJSVariant NativeUtils::getCount(tTJSVariantClosure const& closure) {
		if (!closure.Object) {
			return tTJSVariant();
		}
		tjs_int result = 0;
		if (TJS_FAILED(closure.GetCount(&result, 0, 0, 0))) {
			return tTJSVariant();
		}
		return result;
	}
	KTL_INLINE tTJSVariant NativeUtils::getContext(tTJSVariantClosure const& closure) {
		return tTJSVariant(closure.ObjThis);
	}
	//
	//	SUMMARY: オクテット系メソッド
	//
	KTL_INLINE NativeUtils::byte_type const* NativeUtils::getOctetData(tTJSVariantOctet const* octet) {
		return sprig::krkr::tjs::octet_data(octet);
	}
	KTL_INLINE NativeUtils::size_type NativeUtils::getOctetSize(tTJSVariantOctet const* octet) {
		return sprig::krkr::tjs::octet_length(octet);
	}
	KTL_INLINE NativeUtils::byte_type const* NativeUtils::beginOctet(tTJSVariantOctet const* octet) {
		return sprig::krkr::tjs::octet_data(octet);
	}
	KTL_INLINE NativeUtils::byte_type const* NativeUtils::endOctet(tTJSVariantOctet const* octet) {
		return sprig::krkr::tjs::octet_data(octet) + sprig::krkr::tjs::octet_length(octet);
	}
	//
	//	SUMMARY: 文字列系メソッド
	//
	KTL_INLINE NativeUtils::byte_type const* NativeUtils::getStringData(tjs_char const* string) {
		return reinterpret_cast<byte_type const*>(string);
	}
	KTL_INLINE NativeUtils::size_type NativeUtils::getStringSize(tjs_char const* string) {
		return string
			? sprig::str_length(string) * sizeof(tjs_char)
			: 0
			;
	}
	KTL_INLINE NativeUtils::byte_type const* NativeUtils::beginString(tjs_char const* string) {
		return reinterpret_cast<byte_type const*>(string);
	}
	KTL_INLINE NativeUtils::byte_type const* NativeUtils::endString(tjs_char const* string) {
		return reinterpret_cast<byte_type const*>(string + getStringSize(string));
	}
	//
	//	SUMMARY: 変換系メソッド
	//
	KTL_INLINE tTJSVariant NativeUtils::toString(tTJSVariant const* v) {
		tTJSVariant result(*v);
		switch (v->Type()) {
		case tvtOctet:
			{
				tTJSVariant result;
				{
					tTJSVariantString* str = ::TJSOctetToListString(v->AsOctetNoAddRef());
					result = str;
					str->Release();
				}
				return result;
			}
		default:
			{
				tTJSVariant result(*v);
				result.ToString();
				return result;
			}
		}
	}
	KTL_INLINE tTJSVariant NativeUtils::toReadableString(tTJSVariant const* v) {
		return ::TJSVariantToReadableString(*v);
	}
	KTL_INLINE tTJSVariant NativeUtils::toReadableString(tTJSVariant const* v, size_type max_size) {
		return ::TJSVariantToReadableString(*v, max_size);
	}
	KTL_INLINE tTJSVariant NativeUtils::toExpressionString(tTJSVariant const* v) {
		return ::TJSVariantToExpressionString(*v);
	}
	//
	//	SUMMARY: ストレージ系メソッド
	//
	KTL_INLINE tTJSVariant NativeUtils::loadStorageToOctet(tjs_char const* storage) {
		sprig::com_ptr<IStream> in(::TVPCreateIStream(storage, TJS_BS_READ));
		if (!in) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました")) + storage,
				sprig::krkr::internal_error
				);
			return tTJSVariant();
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
			return tTJSVariant();
		}
		size_type result_size = static_cast<size_type>(new_pos.QuadPart);
		if (!result_size) {
			return tTJSVariant(0, result_size);
		}
		if (FAILED(in->Seek(move, TJS_BS_SEEK_SET, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return tTJSVariant();
		}
		std::vector<tjs_uint8> buffer(result_size);
		ULONG io_size = 0;
		if (FAILED(in->Read(&buffer[0], result_size, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return tTJSVariant();
		}
		return tTJSVariant(
			io_size
				? &buffer[0]
				: 0
				,
			io_size
			);
	}
	KTL_INLINE NativeUtils::size_type NativeUtils::saveStorageFromOctet(tjs_char const* storage, tTJSVariantOctet const* octet) {
		return saveStorage(storage, sprig::krkr::tjs::octet_data(octet), sprig::krkr::tjs::octet_length(octet));
	}
	//
	//	SUMMARY: 列挙系メソッド
	//
	KTL_INLINE tTJSVariant NativeUtils::enumMembers(tTJSVariantClosure const& closure) {
		if (!closure.Object) {
			return tTJSVariant();
		}
		CallbackEnumMembers callback;
		tTJSVariantClosure caller(&callback);
		if (TJS_FAILED(closure.EnumMembers(0, &caller, closure.ObjThis))) {
			return tTJSVariant();
		}
		return callback.result();
	}
	KTL_INLINE tTJSVariant NativeUtils::enumMembersName(tTJSVariantClosure const& closure) {
		if (!closure.Object) {
			return tTJSVariant();
		}
		CallbackEnumMembersName callback;
		tTJSVariantClosure caller(&callback);
		if (TJS_FAILED(closure.EnumMembers(TJS_ENUM_NO_VALUE, &caller, closure.ObjThis))) {
			return tTJSVariant();
		}
		return callback.result();
	}
	//
	//	SUMMARY: セーブ系メソッド
	//
	KTL_INLINE tTJSVariant NativeUtils::saveStructString(
		tTJSVariant const* v,
		flag_type flag,
		boost::optional<tjs_char> indent_char,
		boost::optional<size_type> indent_count
		)
	{
		SaveStructStringWriter writer(
			flag,
			indent_char ? *indent_char : SPRIG_KRKR_TJS_W(' '),
			indent_count ? *indent_count : 1
			);
		object_stack_type stack;
		if (TJS_FAILED(writeSaveStructString(writer, *v, stack))) {
			return tTJSVariant();
		}
		return writer.result().c_str();
	}
	KTL_INLINE NativeUtils::int_type NativeUtils::saveStruct(
		tjs_char const* storage,
		tTJSVariant const* v,
		flag_type flag,
		boost::optional<tjs_char> indent_char,
		boost::optional<size_type> indent_count
		)
	{
		SaveStructStringWriter writer(
			flag,
			indent_char ? *indent_char : SPRIG_KRKR_TJS_W(' '),
			indent_count ? *indent_count : 1
			);
		object_stack_type stack;
		writer.write(SPRIG_KRKR_TJS_W('\xFEFF'));
		if (TJS_FAILED(writeSaveStructString(writer, *v, stack))) {
			return tTJSVariant();
		}
		tjs_string_type s = writer.result();
		return saveStorage(storage, s.c_str(), s.size() * sizeof(tjs_char));
	}

	//
	// Utils
	//
	Utils::Utils() {}
	tjs_error TJS_INTF_METHOD Utils::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Utils::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeUtils>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Utils::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Utils::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: オブジェクト系メソッド
	//
	KTL_INLINE tTJSVariant Utils::getCii(iTJSDispatch2* obj) {
		return NativeUtils::getCii(obj);
	}
	KTL_INLINE tTJSVariant Utils::getCii(iTJSDispatch2* obj, tTVInteger num) {
		return NativeUtils::getCii(
			obj,
			boost::numeric_cast<NativeUtils::size_type>(num)
			);
	}
	KTL_INLINE tTJSVariant Utils::getCount(tTJSVariantClosure const& closure) {
		return NativeUtils::getCount(closure);
	}
	KTL_INLINE tTJSVariant Utils::getContext(tTJSVariantClosure const& closure) {
		return NativeUtils::getContext(closure);
	}
	//
	//	SUMMARY: オクテット系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type Utils::getOctetData(tTJSVariantOctet const* octet) {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(
			NativeUtils::getOctetData(octet)
			);
	}
	KTL_INLINE tTVInteger Utils::getOctetSize(tTJSVariantOctet const* octet) {
		return NativeUtils::getOctetSize(octet);
	}
	KTL_INLINE sprig::krkr::tjs::intptr_type Utils::beginOctet(tTJSVariantOctet const* octet) {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(
			NativeUtils::beginOctet(octet)
			);
	}
	KTL_INLINE sprig::krkr::tjs::intptr_type Utils::endOctet(tTJSVariantOctet const* octet) {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(
			NativeUtils::endOctet(octet)
			);
	}
	//
	//	SUMMARY: 文字列系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type Utils::getStringData(tTJSVariantString const* string) {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(
			NativeUtils::getStringData(
				sprig::krkr::tjs::as_c_str(string)
				)
			);
	}
	KTL_INLINE tTVInteger Utils::getStringSize(tTJSVariantString const* string) {
		return NativeUtils::getStringSize(
			sprig::krkr::tjs::as_c_str(string)
			);
	}
	KTL_INLINE sprig::krkr::tjs::intptr_type Utils::beginString(tTJSVariantString const* string) {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(
			NativeUtils::beginString(
				sprig::krkr::tjs::as_c_str(string)
				)
			);
	}
	KTL_INLINE sprig::krkr::tjs::intptr_type Utils::endString(tTJSVariantString const* string) {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(
			NativeUtils::endString(
				sprig::krkr::tjs::as_c_str(string)
				)
			);
	}
	//
	//	SUMMARY: 変換系メソッド
	//
	KTL_INLINE tTJSVariant Utils::toString(tTJSVariant const* v) {
		return NativeUtils::toString(v);
	}
	KTL_INLINE tTJSVariant Utils::toReadableString(tTJSVariant const* v) {
		return NativeUtils::toReadableString(v);
	}
	KTL_INLINE tTJSVariant Utils::toReadableString(tTJSVariant const* v, tTVInteger max_size) {
		return NativeUtils::toReadableString(
			v,
			boost::numeric_cast<NativeUtils::size_type>(max_size)
			);
	}
	KTL_INLINE tTJSVariant Utils::toExpressionString(tTJSVariant const* v) {
		return NativeUtils::toExpressionString(v);
	}
	//
	//	SUMMARY: ストレージ系メソッド
	//
	KTL_INLINE tTJSVariant Utils::loadStorageToOctet(tTJSVariantString const* storage) {
		return NativeUtils::loadStorageToOctet(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE tTVInteger Utils::saveStorageFromOctet(tTJSVariantString const* storage, tTJSVariantOctet const* octet) {
		return NativeUtils::saveStorageFromOctet(
			sprig::krkr::tjs::as_c_str(storage),
			octet
			);
	}
	//
	//	SUMMARY: 列挙系メソッド
	//
	KTL_INLINE tTJSVariant Utils::enumMembers(tTJSVariantClosure const& closure) {
		return NativeUtils::enumMembers(closure);
	}
	KTL_INLINE tTJSVariant Utils::enumMembersName(tTJSVariantClosure const& closure) {
		return NativeUtils::enumMembersName(closure);
	}
	//
	//	SUMMARY: セーブ系メソッド
	//
	KTL_INLINE tTJSVariant Utils::saveStructString(
		tTJSVariant const* v,
		tTVInteger flag,
		boost::optional<tTVInteger> indent_char,
		boost::optional<tTVInteger> indent_count
		)
	{
		return NativeUtils::saveStructString(
			v,
			sprig::numeric::bit_cast<NativeUtils::flag_type>(flag),
			indent_char
				? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*indent_char))
				: boost::none
				,
			indent_count
				? boost::optional<NativeUtils::size_type>(boost::numeric_cast<NativeUtils::size_type>(*indent_count))
				: boost::none
			);
	}
	KTL_INLINE tTVInteger Utils::saveStruct(
		tTJSVariantString const* storage,
		tTJSVariant const* v,
		tTVInteger flag,
		boost::optional<tTVInteger> indent_char,
		boost::optional<tTVInteger> indent_count
		)
	{
		return NativeUtils::saveStruct(
			sprig::krkr::tjs::as_c_str(storage),
			v,
			sprig::numeric::bit_cast<NativeUtils::flag_type>(flag),
			indent_char
				? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*indent_char))
				: boost::none
				,
			indent_count
				? boost::optional<NativeUtils::size_type>(boost::numeric_cast<NativeUtils::size_type>(*indent_count))
				: boost::none
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_UTILS_UTILS_HPP
