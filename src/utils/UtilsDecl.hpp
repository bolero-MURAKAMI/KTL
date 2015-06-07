/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_UTILS_UTILS_DECL_HPP
#define SRC_UTILS_UTILS_DECL_HPP

#include <cstddef>
#include <string>
#include <vector>
#include <sstream>
#include <boost/cstdint.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <sprig/external/tp_stub.hpp>

namespace ktl {
	//
	// NativeUtils
	//
	class NativeUtils {
	public:
		typedef boost::int_fast32_t int_type;
		typedef std::size_t size_type;
		typedef tjs_uint8 byte_type;
		typedef boost::uint_fast32_t flag_type;
		typedef std::basic_string<tjs_char> tjs_string_type;
		typedef std::vector<iTJSDispatch2*> object_stack_type;
	public:
		//
		// CallbackEnumMembers
		//
		class CallbackEnumMembers
			: public tTJSDispatch
		{
		private:
			tTJSVariant result_;
			sprig::krkr::tjs::object_type result_obj_;
		public:
			CallbackEnumMembers();
			tjs_error TJS_INTF_METHOD FuncCall(
				tjs_uint32 flag,
				tjs_char const* membername,
				tjs_uint32* hint,
				tTJSVariant* result,
				tjs_int numparams,
				tTJSVariant** param,
				iTJSDispatch2* objthis
				);
			tTJSVariant const& result() const;
		};
		//
		// CallbackEnumMembersName
		//
		class CallbackEnumMembersName
			: public tTJSDispatch
		{
		private:
			tTJSVariant result_;
			sprig::krkr::tjs::object_type result_obj_;
			tjs_int num_;
		public:
			CallbackEnumMembersName();
			tjs_error TJS_INTF_METHOD FuncCall(
				tjs_uint32 flag,
				tjs_char const* membername,
				tjs_uint32* hint,
				tTJSVariant* result,
				tjs_int numparams,
				tTJSVariant** param,
				iTJSDispatch2* objthis
				);
			tTJSVariant const& result() const;
		};
		//
		// CallbackEnumMembersSaveStruct
		//
		template<typename Writer>
		class CallbackEnumMembersSaveStruct
			: public tTJSDispatch
		{
		private:
			Writer* writer_;
			object_stack_type* obj_stack_;
			bool is_not_first_;
		public:
			CallbackEnumMembersSaveStruct(Writer& w, object_stack_type& stack);
			tjs_error TJS_INTF_METHOD FuncCall(
				tjs_uint32 flag,
				tjs_char const* membername,
				tjs_uint32* hint,
				tTJSVariant* result,
				tjs_int numparams,
				tTJSVariant** param,
				iTJSDispatch2* objthis
				);
			Writer& writer() const;
		};
		//
		// SaveStructStringWriter
		//
		class SaveStructStringWriter {
		private:
			flag_type flag_;
			tjs_char indent_char_;
			size_type indent_count_;
			tjs_string_type indent_;
			std::basic_ostringstream<tjs_char> oss_;
		public:
			explicit SaveStructStringWriter(
				flag_type flag = 0,
				tjs_char indent_char = SPRIG_KRKR_TJS_W(' '),
				size_type indent_count = 1
				);
			void write(tjs_string_type const& s);
			void write(tjs_char const* s);
			void write(tjs_char c);
			void newline();
			void write_indent();
			void push_indent();
			void pop_indent();
			void write_const();
			tjs_string_type result() const;
		};
	private:
		template<typename Writer>
		static tjs_error writeSaveStructString(Writer& writer, tTJSVariant const& v, object_stack_type& stack);
		static int_type saveStorage(
			tjs_char const* storage,
			void const* data,
			ULONG length
			);
	public:
		NativeUtils();
	public:
		//
		//	SUMMARY: セーブ系フラグ
		//
		static flag_type const sfNoConstQualifier = 0x00000001;
		//
		//	SUMMARY: 定数系プロパティ
		//
		static size_type const sizeofWChar = sizeof(tjs_char);
		static size_type const sizeofNChar = sizeof(tjs_nchar);
	public:
		//
		//	SUMMARY: オブジェクト系メソッド
		//
		static tTJSVariant getCii(iTJSDispatch2* obj);
		static tTJSVariant getCii(iTJSDispatch2* obj, size_type num);
		static tTJSVariant getCount(tTJSVariantClosure const& closure);
		static tTJSVariant getContext(tTJSVariantClosure const& closure);
		//
		//	SUMMARY: オクテット系メソッド
		//
		static byte_type const* getOctetData(tTJSVariantOctet const* octet);
		static size_type getOctetSize(tTJSVariantOctet const* octet);
		static byte_type const* beginOctet(tTJSVariantOctet const* octet);
		static byte_type const* endOctet(tTJSVariantOctet const* octet);
		//
		//	SUMMARY: 文字列系メソッド
		//
		static byte_type const* getStringData(tjs_char const* string);
		static size_type getStringSize(tjs_char const* string);
		static byte_type const* beginString(tjs_char const* string);
		static byte_type const* endString(tjs_char const* string);
		//
		//	SUMMARY: 変換系メソッド
		//
		static tTJSVariant toString(tTJSVariant const* v);
		static tTJSVariant toReadableString(tTJSVariant const* v);
		static tTJSVariant toReadableString(tTJSVariant const* v, size_type max_size);
		static tTJSVariant toExpressionString(tTJSVariant const* v);
		//
		//	SUMMARY: ストレージ系メソッド
		//
		static tTJSVariant loadStorageToOctet(tjs_char const* storage);
		static size_type saveStorageFromOctet(tjs_char const* storage, tTJSVariantOctet const* octet);
		//
		//	SUMMARY: 列挙系メソッド
		//
		static tTJSVariant enumMembers(tTJSVariantClosure const& closure);
		static tTJSVariant enumMembersName(tTJSVariantClosure const& closure);
		//
		//	SUMMARY: セーブ系メソッド
		//
		static tTJSVariant saveStructString(
			tTJSVariant const* v,
			flag_type flag = 0x00000000,
			boost::optional<tjs_char> indent_char = boost::none,
			boost::optional<size_type> indent_count = boost::none
			);
		static int_type saveStruct(
			tjs_char const* storage,
			tTJSVariant const* v,
			flag_type flag = 0x00000000,
			boost::optional<tjs_char> indent_char = boost::none,
			boost::optional<size_type> indent_count = boost::none
			);
	};

	//
	// Utils
	//
	class Utils
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeUtils> instance_;
	public:
		Utils();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: セーブ系フラグ
		//
		static tTVInteger const sfNoConstQualifier = NativeUtils::sfNoConstQualifier;
		//
		//	SUMMARY: 定数系プロパティ
		//
		static tTVInteger const sizeofWChar = NativeUtils::sizeofWChar;
		static tTVInteger const sizeofNChar = NativeUtils::sizeofNChar;
	public:
		//
		//	SUMMARY: オブジェクト系メソッド
		//
		static tTJSVariant getCii(iTJSDispatch2* obj);
		static tTJSVariant getCii(iTJSDispatch2* obj, tTVInteger num);
		static tTJSVariant getCount(tTJSVariantClosure const& closure);
		static tTJSVariant getContext(tTJSVariantClosure const& closure);
		//
		//	SUMMARY: オクテット系メソッド
		//
		static sprig::krkr::tjs::intptr_type getOctetData(tTJSVariantOctet const* octet);
		static tTVInteger getOctetSize(tTJSVariantOctet const* octet);
		static sprig::krkr::tjs::intptr_type beginOctet(tTJSVariantOctet const* octet);
		static sprig::krkr::tjs::intptr_type endOctet(tTJSVariantOctet const* octet);
		//
		//	SUMMARY: 文字列系メソッド
		//
		static sprig::krkr::tjs::intptr_type getStringData(tTJSVariantString const* string);
		static tTVInteger getStringSize(tTJSVariantString const* string);
		static sprig::krkr::tjs::intptr_type beginString(tTJSVariantString const* string);
		static sprig::krkr::tjs::intptr_type endString(tTJSVariantString const* string);
		//
		//	SUMMARY: 変換系メソッド
		//
		static tTJSVariant toString(tTJSVariant const* v);
		static tTJSVariant toReadableString(tTJSVariant const* v);
		static tTJSVariant toReadableString(tTJSVariant const* v, tTVInteger max_size);
		static tTJSVariant toExpressionString(tTJSVariant const* v);
		//
		//	SUMMARY: ストレージ系メソッド
		//
		static tTJSVariant loadStorageToOctet(tTJSVariantString const* storage);
		static tTVInteger saveStorageFromOctet(tTJSVariantString const* storage, tTJSVariantOctet const* octet);
		//
		//	SUMMARY: 列挙系メソッド
		//
		static tTJSVariant enumMembers(tTJSVariantClosure const& closure);
		static tTJSVariant enumMembersName(tTJSVariantClosure const& closure);
		//
		//	SUMMARY: セーブ系メソッド
		//
		static tTJSVariant saveStructString(
			tTJSVariant const* v,
			tTVInteger flag = 0x00000000,
			boost::optional<tTVInteger> indent_char = boost::none,
			boost::optional<tTVInteger> indent_count = boost::none
			);
		static tTVInteger saveStruct(
			tTJSVariantString const* storage,
			tTJSVariant const* v,
			tTVInteger flag = 0x00000000,
			boost::optional<tTVInteger> indent_char = boost::none,
			boost::optional<tTVInteger> indent_count = boost::none
			);
	};
}	// namespace ktl

#endif	// #ifndef SRC_UTILS_UTILS_DECL_HPP
