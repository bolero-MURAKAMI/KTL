#pragma once

#include <cstddef>
#include <vector>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/cstdint.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeStorageStream
	//
	class NativeStorageStream {
	public:
		typedef boost::int_fast32_t int_type;
		typedef boost::uint_fast32_t flag_type;
		typedef tjs_uint8 byte_type;
		typedef std::vector<byte_type> binary_type;
		typedef binary_type::size_type size_type;
	public:
		static size_type getEnableSize(size_type length, size_type spos);
		static size_type getEnableSize(size_type length, size_type spos, size_type ssize);
	private:
		sprig::com_ptr<IStream> stream_;
	private:
		bool readBuffer(binary_type& buffer);
		bool readBuffer(binary_type& buffer, size_type size);
	public:
		NativeStorageStream();
	public:
		//
		//	SUMMARY: アクセス系フラグ
		//
		static flag_type const afRead = TJS_BS_READ;
		static flag_type const afWrite = TJS_BS_WRITE;
		static flag_type const afAppend = TJS_BS_APPEND;
		static flag_type const afUpdate = TJS_BS_UPDATE;
		//
		//	SUMMARY: シーク系フラグ
		//
		static flag_type const sfSet = TJS_BS_SEEK_SET;
		static flag_type const sfCur = TJS_BS_SEEK_CUR;
		static flag_type const sfEnd = TJS_BS_SEEK_END;
	public:
		//
		//	SUMMARY: 開閉系メソッド
		//
		bool open(tjs_char const* storage, flag_type flag);
		bool close();
		bool isOpen() const;
		//
		//	SUMMARY: 読込系メソッド
		//
		tTJSVariant readOctet();
		tTJSVariant readOctet(size_type size);
		tTJSVariant readString();
		tTJSVariant readString(size_type size);
		tTJSVariant readNarrowString();
		tTJSVariant readNarrowString(size_type size);
		//
		//	SUMMARY: 書込系メソッド
		//
		int_type writeOctet(tTJSVariantOctet const* source);
		int_type writeOctet(tTJSVariantOctet const* source, size_type spos);
		int_type writeOctet(tTJSVariantOctet const* source, size_type spos, size_type ssize);
		int_type writeString(tjs_char const* source);
		int_type writeString(tjs_char const* source, size_type spos);
		int_type writeString(tjs_char const* source, size_type spos, size_type ssize);
		int_type writeNarrowString(tjs_nchar const* source);
		int_type writeNarrowString(tjs_nchar const* source, size_type spos);
		int_type writeNarrowString(tjs_nchar const* source, size_type spos, size_type ssize);
		int_type writeBlock(byte_type const* first, byte_type const* last);
		int_type writeBytes(byte_type const* data, size_type size);
		//
		//	SUMMARY: シーク系メソッド
		//
		int_type seek(int_type offset, flag_type flag);
		int_type current();
		int_type size();
	};

	//
	// StorageStream
	//
	class StorageStream
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeStorageStream> instance_;
	public:
		//
		//	SUMMARY: アクセス系フラグ
		//
		static tTVInteger const afRead = NativeStorageStream::afRead;
		static tTVInteger const afWrite = NativeStorageStream::afWrite;
		static tTVInteger const afAppend = NativeStorageStream::afAppend;
		static tTVInteger const afUpdate = NativeStorageStream::afUpdate;
		//
		//	SUMMARY: シーク系フラグ
		//
		static tTVInteger const sfSet = NativeStorageStream::sfSet;
		static tTVInteger const sfCur = NativeStorageStream::sfCur;
		static tTVInteger const sfEnd = NativeStorageStream::sfEnd;
	public:
		StorageStream();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
		//
		//	SUMMARY: 開閉系メソッド
		//
		bool open(tTJSVariantString const* storage, tTVInteger flag);
		bool close();
		bool isOpen() const;
		//
		//	SUMMARY: 読込系メソッド
		//
		tTJSVariant readOctet();
		tTJSVariant readOctet(tTVInteger size);
		tTJSVariant readString();
		tTJSVariant readString(tTVInteger size);
		tTJSVariant readNarrowString();
		tTJSVariant readNarrowString(tTVInteger size);
		//
		//	SUMMARY: 書込系メソッド
		//
		tTVInteger writeOctet(tTJSVariantOctet const* source);
		tTVInteger writeOctet(tTJSVariantOctet const* source, tTVInteger spos);
		tTVInteger writeOctet(tTJSVariantOctet const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger writeString(tTJSVariantString const* source);
		tTVInteger writeString(tTJSVariantString const* source, tTVInteger spos);
		tTVInteger writeString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger writeNarrowString(tTJSVariantString const* source);
		tTVInteger writeNarrowString(tTJSVariantString const* source, tTVInteger spos);
		tTVInteger writeNarrowString(tTJSVariantString const* source, tTVInteger spos, tTVInteger ssize);
		tTVInteger writeBlock(sprig::krkr::tjs::intptr_type first, sprig::krkr::tjs::intptr_type last);
		tTVInteger writeBytes(sprig::krkr::tjs::intptr_type data, tTVInteger size);
		//
		//	SUMMARY: シーク系メソッド
		//
		tTVInteger seek(tTVInteger offset, tTVInteger flag);
		tTVInteger current();
		tTVInteger size();
	};
}	// namespace ktl
