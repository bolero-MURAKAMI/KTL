/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_OCTET_BUILDER_OCTET_BUILDER_DECL_HPP
#define SRC_OCTET_BUILDER_OCTET_BUILDER_DECL_HPP

#include <vector>
#include <memory>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeOctetBuilder
	//
	class NativeOctetBuilder {
	public:
		typedef boost::uint_fast32_t flag_type;
		typedef boost::int_fast32_t int_type;
		typedef tjs_uint8 byte_type;
		typedef std::vector<byte_type> binary_type;
		typedef binary_type::iterator iterator;
		typedef binary_type::const_iterator const_iterator;
		typedef binary_type::size_type size_type;
	public:
		static sprig::krkr::tjs::intptr_type getInstance(iTJSDispatch2* obj);
		static sprig::krkr::tjs::object_type createNew(tjs_int numparams, tTJSVariant** param);
		static size_type getEnableSize(size_type length, size_type spos);
		static size_type getEnableSize(size_type length, size_type spos, size_type ssize);
		static bool loadStorage(binary_type& binary, tjs_char const* storage);
		static bool loadStorage(binary_type& binary, tjs_char const* storage, size_type spos);
		static bool loadStorage(binary_type& binary, tjs_char const* storage, size_type spos, size_type ssize);
	private:
		binary_type binary_;
	private:
		template<typename IntType>
		size_type setInteger(size_type pos, tTVInteger t, flag_type flag);
		template<typename FloatType>
		size_type setReal(size_type pos, tTVReal t, flag_type flag);
		template<typename IntType>
		void assignInteger(tTVInteger t, flag_type flag);
		template<typename FloatType>
		void assignReal(tTVReal t, flag_type flag);
		template<typename IntType>
		void insertInteger(size_type pos, tTVInteger t, flag_type flag);
		template<typename FloatType>
		void insertReal(size_type pos, tTVReal t, flag_type flag);
		template<typename IntType>
		size_type addInteger(tTVInteger t, flag_type flag);
		template<typename FloatType>
		size_type addReal(tTVReal t, flag_type flag);
		template<typename IntType>
		tTJSVariant asInteger(size_type pos, flag_type flag) const;
		template<typename FloatType>
		tTJSVariant asReal(size_type pos, flag_type flag) const;
	public:
		NativeOctetBuilder();
		void emptyCheck() const;
		bool isOutOfRange(size_type pos) const;
		bool isOutOfRangeOff(size_type pos) const;
		iterator getPos(size_type pos);
		iterator getBegin();
		iterator getEnd();
	public:
		template<typename Iterator>
		void init(Iterator first, Iterator last) {
			binary_.assign(first, last);
		}
	public:
		//
		//	SUMMARY: タイプ系フラグ
		//
		static flag_type const vtfMask = 0x0000FFFF;
		static flag_type const vtfByte = 0x00000001;
		static flag_type const vtfInt8 = 0x00000010;
		static flag_type const vtfInt16 = 0x00000020;
		static flag_type const vtfInt32 = 0x00000030;
		static flag_type const vtfInt64 = 0x00000040;
		static flag_type const vtfUInt8 = 0x00000080;
		static flag_type const vtfUInt16 = 0x00000090;
		static flag_type const vtfUInt32 = 0x000000A0;
		static flag_type const vtfUInt64 = 0x000000B0;
		static flag_type const vtfFloat = 0x00000100;
		static flag_type const vtfDouble = 0x00000200;
		static flag_type const vtfOctet = 0x00001000;
		static flag_type const vtfString = 0x00002000;
		static flag_type const vtfNarrowString = 0x00003000;
		static flag_type const vtfBuilder = 0x00004000;
		//
		//	SUMMARY: エンディアン系フラグ
		//
		static flag_type const vefMask = 0x000F0000;
		static flag_type const vefBigEndian = 0x00010000;
		static flag_type const vefLittleEndian = 0x00020000;
	public:
		//
		//	SUMMARY: 生成系メソッド
		//
		static sprig::krkr::tjs::object_type fromHex(tTJSVariantString const* data);
	public:
		//
		//	SUMMARY: 書込系メソッド
		//
		size_type set(size_type pos, tTJSVariant* v, flag_type flag = vtfByte);
		size_type setByte(size_type pos, size_type size, byte_type b);
		size_type setBuilder(size_type pos, iTJSDispatch2* source);
		size_type setBuilder(size_type pos, iTJSDispatch2* source, size_type spos);
		size_type setBuilder(size_type pos, iTJSDispatch2* source, size_type spos, size_type ssize);
		size_type setOctet(size_type pos, tTJSVariantOctet const* source);
		size_type setOctet(size_type pos, tTJSVariantOctet const* source, size_type spos);
		size_type setOctet(size_type pos, tTJSVariantOctet const* source, size_type spos, size_type ssize);
		size_type setString(size_type pos, tjs_char const* source);
		size_type setString(size_type pos, tjs_char const* source, size_type spos);
		size_type setString(size_type pos, tjs_char const* source, size_type spos, size_type ssize);
		size_type setNarrowString(size_type pos, tjs_nchar const* source);
		size_type setNarrowString(size_type pos, tjs_nchar const* source, size_type spos);
		size_type setNarrowString(size_type pos, tjs_nchar const* source, size_type spos, size_type ssize);
		size_type setStorage(size_type pos, tjs_char const* storage);
		size_type setStorage(size_type pos, tjs_char const* storage, size_type spos);
		size_type setStorage(size_type pos, tjs_char const* storage, size_type spos, size_type ssize);
		size_type setBlock(size_type pos, byte_type const* first, byte_type const* last);
		size_type setBytes(size_type pos, byte_type const* data, size_type size);
		//
		//	SUMMARY: 割当系メソッド
		//
		void assign(tTJSVariant* v, flag_type flag = vtfByte);
		void assignByte(size_type size, byte_type b);
		void assignBuilder(iTJSDispatch2* source);
		void assignBuilder(iTJSDispatch2* source, size_type spos);
		void assignBuilder(iTJSDispatch2* source, size_type spos, size_type ssize);
		void assignOctet(tTJSVariantOctet const* source);
		void assignOctet(tTJSVariantOctet const* source, size_type spos);
		void assignOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize);
		void assignString(tjs_char const* source);
		void assignString(tjs_char const* source, size_type spos);
		void assignString(tjs_char const* source, size_type spos, size_type ssize);
		void assignNarrowString(tjs_nchar const* source);
		void assignNarrowString(tjs_nchar const* source, size_type spos);
		void assignNarrowString(tjs_nchar const* source, size_type spos, size_type ssize);
		void assignStorage(tjs_char const* storage);
		void assignStorage(tjs_char const* storage, size_type spos);
		void assignStorage(tjs_char const* storage, size_type spos, size_type ssize);
		void assignBlock(byte_type const* first, byte_type const* last);
		void assignBytes(byte_type const* data, size_type size);
		//
		//	SUMMARY: 挿入系メソッド
		//
		void insert(size_type pos, tTJSVariant* v, flag_type flag = vtfByte);
		void insertByte(size_type pos, size_type size, byte_type b);
		void insertBuilder(size_type pos, iTJSDispatch2* source);
		void insertBuilder(size_type pos, iTJSDispatch2* source, size_type spos);
		void insertBuilder(size_type pos, iTJSDispatch2* source, size_type spos, size_type ssize);
		void insertOctet(size_type pos, tTJSVariantOctet const* source);
		void insertOctet(size_type pos, tTJSVariantOctet const* source, size_type spos);
		void insertOctet(size_type pos, tTJSVariantOctet const* source, size_type spos, size_type ssize);
		void insertString(size_type pos, tjs_char const* source);
		void insertString(size_type pos, tjs_char const* source, size_type spos);
		void insertString(size_type pos, tjs_char const* source, size_type spos, size_type ssize);
		void insertNarrowString(size_type pos, tjs_nchar const* source);
		void insertNarrowString(size_type pos, tjs_nchar const* source, size_type spos);
		void insertNarrowString(size_type pos, tjs_nchar const* source, size_type spos, size_type ssize);
		void insertStorage(size_type pos, tjs_char const* storage);
		void insertStorage(size_type pos, tjs_char const* storage, size_type spos);
		void insertStorage(size_type pos, tjs_char const* storage, size_type spos, size_type ssize);
		void insertBlock(size_type pos, byte_type const* first, byte_type const* last);
		void insertBytes(size_type pos, byte_type const* data, size_type size);
		//
		//	SUMMARY: 追加系メソッド
		//
		size_type add(tTJSVariant* v, flag_type flag = vtfByte);
		size_type addByte(size_type size, byte_type b);
		size_type addBuilder(iTJSDispatch2* source);
		size_type addBuilder(iTJSDispatch2* source, size_type spos);
		size_type addBuilder(iTJSDispatch2* source, size_type spos, size_type ssize);
		size_type addOctet(tTJSVariantOctet const* source);
		size_type addOctet(tTJSVariantOctet const* source, size_type spos);
		size_type addOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize);
		size_type addString(tjs_char const* source);
		size_type addString(tjs_char const* source, size_type spos);
		size_type addString(tjs_char const* source, size_type spos, size_type ssize);
		size_type addNarrowString(tjs_nchar const* source);
		size_type addNarrowString(tjs_nchar const* source, size_type spos);
		size_type addNarrowString(tjs_nchar const* source, size_type spos, size_type ssize);
		size_type addStorage(tjs_char const* storage);
		size_type addStorage(tjs_char const* storage, size_type spos);
		size_type addStorage(tjs_char const* storage, size_type spos, size_type ssize);
		size_type addBlock(byte_type const* first, byte_type const* last);
		size_type addBytes(byte_type const* data, size_type size);
		//
		//	SUMMARY: 初期化系メソッド
		//
		void clear();
		//
		//	SUMMARY: 削除系メソッド
		//
		void erase(size_type pos);
		void erase(size_type pos, size_type size);
		//
		//	SUMMARY: 前後削除系メソッド
		//
		int_type pop();
		size_type pop(size_type size);
		int_type shift();
		size_type shift(size_type size);
		//
		//	SUMMARY: 変換系メソッド
		//
		tTJSVariant as(size_type pos, flag_type flag = vtfByte) const;
		tTJSVariant asBuilder() const;
		tTJSVariant asBuilder(size_type pos) const;
		tTJSVariant asBuilder(size_type pos, size_type size) const;
		tTJSVariant asOctet() const;
		tTJSVariant asOctet(size_type pos) const;
		tTJSVariant asOctet(size_type pos, size_type size) const;
		tTJSVariant asString() const;
		tTJSVariant asString(size_type pos) const;
		tTJSVariant asString(size_type pos, size_type size) const;
		tTJSVariant asNarrowString() const;
		tTJSVariant asNarrowString(size_type pos) const;
		tTJSVariant asNarrowString(size_type pos, size_type size) const;
		//
		//	SUMMARY: ストレージ系メソッド
		//
		int_type saveStorage(tjs_char const* storage) const;
		int_type saveStorage(tjs_char const* storage, size_type pos) const;
		int_type saveStorage(tjs_char const* storage, size_type pos, size_type size) const;
		//
		//	SUMMARY: 演算系メソッド
		//
		size_type transformAndByte(byte_type b);
		size_type transformOrByte(byte_type b);
		size_type transformXorByte(byte_type b);
		size_type transformNotByte();
		//
		//	SUMMARY: 検索系メソッド
		//
		int_type findByte(byte_type b) const;
		int_type findByte(byte_type b, size_type pos) const;
		int_type findByte(byte_type b, size_type pos, size_type size) const;
		//
		//	SUMMARY: メモリ系メソッド
		//
		void copy(sprig::krkr::tjs::intptr_type dest, size_type pos, size_type size) const;
		//
		//	SUMMARY: 取得プロパティ系メソッド
		//
		int_type back() const;
		int_type front() const;
		//
		//	SUMMARY: ポインタ取得プロパティ系メソッド
		//
		byte_type* begin();
		byte_type* end();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool empty() const;
		size_type size() const;
		void resize(size_type size);
	};

	//
	// OctetBuilder
	//
	class OctetBuilder
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeOctetBuilder> instance_;
	public:
		OctetBuilder();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		NativeOctetBuilder::iterator getBegin();
		NativeOctetBuilder::iterator getEnd();
	public:
		template<typename Iterator>
		void init(Iterator first, Iterator last) {
			instance_->init(first, last);
		}
	public:
		//
		//	SUMMARY: タイプ系フラグ
		//
		static tTVInteger const vtfMask = NativeOctetBuilder::vtfMask;
		static tTVInteger const vtfByte = NativeOctetBuilder::vtfByte;
		static tTVInteger const vtfInt8 = NativeOctetBuilder::vtfInt8;
		static tTVInteger const vtfInt16 = NativeOctetBuilder::vtfInt16;
		static tTVInteger const vtfInt32 = NativeOctetBuilder::vtfInt32;
		static tTVInteger const vtfInt64 = NativeOctetBuilder::vtfInt64;
		static tTVInteger const vtfUInt8 = NativeOctetBuilder::vtfUInt8;
		static tTVInteger const vtfUInt16 = NativeOctetBuilder::vtfUInt16;
		static tTVInteger const vtfUInt32 = NativeOctetBuilder::vtfUInt32;
		static tTVInteger const vtfUInt64 = NativeOctetBuilder::vtfUInt64;
		static tTVInteger const vtfFloat = NativeOctetBuilder::vtfFloat;
		static tTVInteger const vtfDouble = NativeOctetBuilder::vtfDouble;
		static tTVInteger const vtfOctet = NativeOctetBuilder::vtfOctet;
		static tTVInteger const vtfString = NativeOctetBuilder::vtfString;
		static tTVInteger const vtfNarrowString = NativeOctetBuilder::vtfNarrowString;
		static tTVInteger const vtfBuilder = NativeOctetBuilder::vtfBuilder;
		//
		//	SUMMARY: エンディアン系フラグ
		//
		static tTVInteger const vefMask = NativeOctetBuilder::vefMask;
		static tTVInteger const vefBigEndian = NativeOctetBuilder::vefBigEndian;
		static tTVInteger const vefLittleEndian = NativeOctetBuilder::vefLittleEndian;
	public:
		//
		//	SUMMARY: 生成系メソッド
		//
		static sprig::krkr::tjs::object_type fromHex(tTJSVariantString const* data);
	public:
		//
		//	SUMMARY: 内部系メソッド
		//
		sprig::krkr::tjs::intptr_type getInstance();
		//
		//	SUMMARY: 書込系メソッド
		//
		tTVInteger set(tTVInteger pos, tTJSVariant* v, tTVInteger flag = vtfByte);
		tTVInteger setByte(tTVInteger pos, tTVInteger size, tTVInteger b);
		tTVInteger setBuilder(tTVInteger pos, iTJSDispatch2* source);
		tTVInteger setBuilder(tTVInteger pos, iTJSDispatch2* source, tTVInteger spos);
		tTVInteger setBuilder(tTVInteger pos, iTJSDispatch2* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger setOctet(tTVInteger pos, tTJSVariantOctet const* source);
		tTVInteger setOctet(tTVInteger pos, tTJSVariantOctet const* source, tTVInteger spos);
		tTVInteger setOctet(tTVInteger pos, tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger setString(tTVInteger pos, tTJSVariantString const* source);
		tTVInteger setString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos);
		tTVInteger setString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger setNarrowString(tTVInteger pos, tTJSVariantString const* source);
		tTVInteger setNarrowString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos);
		tTVInteger setNarrowString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger setStorage(tTVInteger pos, tTJSVariantString const* storage);
		tTVInteger setStorage(tTVInteger pos, tTJSVariantString const* storage, tTVInteger spos);
		tTVInteger setStorage(tTVInteger pos, tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize);
		tTVInteger setBlock(tTVInteger pos, sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last);
		tTVInteger setBytes(tTVInteger pos, sprig::krkr::tjs::intptr_type data, tTVInteger size);
		//
		//	SUMMARY: 割当系メソッド
		//
		void assign(tTJSVariant* v, tTVInteger flag = vtfByte);
		void assignByte(tTVInteger size, tTVInteger b);
		void assignBuilder(iTJSDispatch2* source);
		void assignBuilder(iTJSDispatch2* source, tTVInteger spos);
		void assignBuilder(iTJSDispatch2* source, tTVInteger spos, tTVInteger ssize);
		void assignOctet(tTJSVariantOctet const* source);
		void assignOctet(tTJSVariantOctet const* source, tTVInteger spos);
		void assignOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize);
		void assignString(tTJSVariantString const* source);
		void assignString(tTJSVariantString const* source, tTVInteger spos);
		void assignString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		void assignNarrowString(tTJSVariantString const* source);
		void assignNarrowString(tTJSVariantString const* source, tTVInteger spos);
		void assignNarrowString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		void assignStorage(tTJSVariantString const* storage);
		void assignStorage(tTJSVariantString const* storage, tTVInteger spos);
		void assignStorage(tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize);
		void assignBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last);
		void assignBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size);
		//
		//	SUMMARY: 挿入系メソッド
		//
		void insert(tTVInteger pos, tTJSVariant* v, tTVInteger flag = vtfByte);
		void insertByte(tTVInteger pos, tTVInteger size, tTVInteger b);
		void insertBuilder(tTVInteger pos, iTJSDispatch2* source);
		void insertBuilder(tTVInteger pos, iTJSDispatch2* source, tTVInteger spos);
		void insertBuilder(tTVInteger pos, iTJSDispatch2* source, tTVInteger spos, tTVInteger ssize);
		void insertOctet(tTVInteger pos, tTJSVariantOctet const* source);
		void insertOctet(tTVInteger pos, tTJSVariantOctet const* source, tTVInteger spos);
		void insertOctet(tTVInteger pos, tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize);
		void insertString(tTVInteger pos, tTJSVariantString const* source);
		void insertString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos);
		void insertString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		void insertNarrowString(tTVInteger pos, tTJSVariantString const* source);
		void insertNarrowString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos);
		void insertNarrowString(tTVInteger pos, tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		void insertStorage(tTVInteger pos, tTJSVariantString const* storage);
		void insertStorage(tTVInteger pos, tTJSVariantString const* storage, tTVInteger spos);
		void insertStorage(tTVInteger pos, tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize);
		void insertBlock(tTVInteger pos, sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last);
		void insertBytes(tTVInteger pos, sprig::krkr::tjs::intptr_type data, tTVInteger size);
		//
		//	SUMMARY: 追加系メソッド
		//
		tTVInteger add(tTJSVariant* v, tTVInteger flag = vtfByte);
		tTVInteger addByte(tTVInteger size, tTVInteger b);
		tTVInteger addBuilder(iTJSDispatch2* source);
		tTVInteger addBuilder(iTJSDispatch2* source, tTVInteger spos);
		tTVInteger addBuilder(iTJSDispatch2* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger addOctet(tTJSVariantOctet const* source);
		tTVInteger addOctet(tTJSVariantOctet const* source, tTVInteger spos);
		tTVInteger addOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger addString(tTJSVariantString const* source);
		tTVInteger addString(tTJSVariantString const* source, tTVInteger spos);
		tTVInteger addString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger addNarrowString(tTJSVariantString const* source);
		tTVInteger addNarrowString(tTJSVariantString const* source, tTVInteger spos);
		tTVInteger addNarrowString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger addStorage(tTJSVariantString const* storage);
		tTVInteger addStorage(tTJSVariantString const* storage, tTVInteger spos);
		tTVInteger addStorage(tTJSVariantString const* storage, tTVInteger spos, tTVInteger ssize);
		tTVInteger addBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last);
		tTVInteger addBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size);
		//
		//	SUMMARY: 初期化系メソッド
		//
		void clear();
		//
		//	SUMMARY: 削除系メソッド
		//
		void erase(tTVInteger pos);
		void erase(tTVInteger pos, tTVInteger size);
		//
		//	SUMMARY: 前後削除系メソッド
		//
		tTVInteger pop();
		tTVInteger pop(tTVInteger size);
		tTVInteger shift();
		tTVInteger shift(tTVInteger size);
		//
		//	SUMMARY: 変換系メソッド
		//
		tTJSVariant as(tTVInteger pos, tTVInteger flag = vtfByte) const;
		tTJSVariant asBuilder() const;
		tTJSVariant asBuilder(tTVInteger pos) const;
		tTJSVariant asBuilder(tTVInteger pos, tTVInteger size) const;
		tTJSVariant asOctet() const;
		tTJSVariant asOctet(tTVInteger pos) const;
		tTJSVariant asOctet(tTVInteger pos, tTVInteger size) const;
		tTJSVariant asString() const;
		tTJSVariant asString(tTVInteger pos) const;
		tTJSVariant asString(tTVInteger pos, tTVInteger size) const;
		tTJSVariant asNarrowString() const;
		tTJSVariant asNarrowString(tTVInteger pos) const;
		tTJSVariant asNarrowString(tTVInteger pos, tTVInteger size) const;
		//
		//	SUMMARY: ストレージ系メソッド
		//
		tTVInteger saveStorage(tTJSVariantString const* storage) const;
		tTVInteger saveStorage(tTJSVariantString const* storage, tTVInteger pos) const;
		tTVInteger saveStorage(tTJSVariantString const* storage, tTVInteger pos, tTVInteger size) const;
		//
		//	SUMMARY: 演算系メソッド
		//
		tTVInteger transformAndByte(tTVInteger b);
		tTVInteger transformOrByte(tTVInteger b);
		tTVInteger transformXorByte(tTVInteger b);
		tTVInteger transformNotByte();
		//
		//	SUMMARY: 検索系メソッド
		//
		tTVInteger findByte(tTVInteger b) const;
		tTVInteger findByte(tTVInteger b, tTVInteger pos) const;
		tTVInteger findByte(tTVInteger b, tTVInteger pos, tTVInteger size) const;
		//
		//	SUMMARY: メモリ系メソッド
		//
		void copy(sprig::krkr::tjs::intptr_type dest, tTVInteger pos, tTVInteger size) const;
		//
		//	SUMMARY: 取得プロパティ系メソッド
		//
		tTVInteger back() const;
		tTVInteger front() const;
		//
		//	SUMMARY: ポインタ取得プロパティ系メソッド
		//
		sprig::krkr::tjs::intptr_type begin();
		sprig::krkr::tjs::intptr_type end();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool empty() const;
		tTVInteger size() const;
		void resize(tTVInteger size);
	};
}	// namespace ktl

#endif	// #ifndef SRC_OCTET_BUILDER_OCTET_BUILDER_DECL_HPP
