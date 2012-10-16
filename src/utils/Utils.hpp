#pragma once

#include <iosfwd>
#include <iterator>
#include <algorithm>
#include <vector>
#include <sstream>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
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
	NativeUtils::CallbackEnumMembers::CallbackEnumMembers()
		: result_obj_(0)
	{
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
			&result_obj_,
			0,
			0,
			0
			);
		sprig::krkr::tjs::object_type result(result_obj_, false);
		result_ = tTJSVariant(result_obj_, result_obj_);
	}
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
			sprig::krkr::tjs::AddMember(result_obj_, tTJSString(*param[0]).c_str(), param[2]);
		}
		if (result) {
			*result = true;
		}
		return TJS_S_OK;
	}
	tTJSVariant const& NativeUtils::CallbackEnumMembers::result() const {
		return result_;
	}
	//
	// NativeUtils::CallbackEnumMembersName
	//
	NativeUtils::CallbackEnumMembersName::CallbackEnumMembersName()
		: result_obj_(0)
		, num_(0)
	{
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			&result_obj_,
			0,
			0,
			0
			);
		sprig::krkr::tjs::object_type result(result_obj_, false);
		result_ = tTJSVariant(result_obj_, result_obj_);
	}
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
			sprig::krkr::tjs::AddMemberByNum(result_obj_, num_++, param[0]);
		}
		if (result) {
			*result = true;
		}
		return TJS_S_OK;
	}
	tTJSVariant const& NativeUtils::CallbackEnumMembersName::result() const {
		return result_;
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
		sprig::com_ptr<IStream> out(::TVPCreateIStream(storage, TJS_BS_WRITE));
		if (!out) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました")) + storage,
				sprig::krkr::internal_error
				);
			return -1;
		}
		tjs_uint8 const* data = sprig::krkr::tjs::octet_data(octet);
		tjs_uint const length = sprig::krkr::tjs::octet_length(octet);
		ULONG io_size = 0;
		if (FAILED(out->Write(data, length, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル書込に失敗しました")) + storage,
				sprig::krkr::internal_error
				);
			return -1;
		}
		return io_size;
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
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
