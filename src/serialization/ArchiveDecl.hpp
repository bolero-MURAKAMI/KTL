#pragma once

#include <vector>
#include <string>
#include <memory>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

#include "ArchiveCommon.hpp"

namespace ktl {
	//
	// NativeArchive
	//
	class NativeArchive {
	public:
		typedef std::basic_string<tjs_char> serialization_string_type;
		typedef std::auto_ptr<serialization_string_type> string_instance_type;
		typedef std::vector<char> binary_type;
		typedef std::auto_ptr<binary_type> binary_instance_type;
		typedef ElementStack::size_type size_type;
	private:
		ElementStack element_stack_;
	public:
		ElementStack& getElementStack();
	public:
		NativeArchive();
	public:
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		string_instance_type saveText() const;
		void loadText(tjs_char const* data);
		string_instance_type saveXML() const;
		void loadXML(tjs_char const* data);
		binary_instance_type saveBinary() const;
		void loadBinary(tTJSVariantOctet const* data);
		//
		//	SUMMARY: Ptシリアライズ系メソッド
		//
		string_instance_type savePtXML(
			boost::optional<tjs_char> indent_char = boost::none,
			boost::optional<size_type> indent_count = boost::none,
			tjs_char const* encoding = 0
			) const;
		void loadPtXML(tjs_char const* data);
		string_instance_type savePtJSON() const;
		void loadPtJSON(tjs_char const* data);
		string_instance_type savePtINFO(
			boost::optional<tjs_char> indent_char = boost::none,
			boost::optional<size_type> indent_count = boost::none
			) const;
		void loadPtINFO(tjs_char const* data);
		//
		//	SUMMARY: 操作系メソッド
		//
		void clear();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		size_type size() const;
	};

	//
	// Archive
	//
	class Archive
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeArchive> instance_;
	public:
		NativeArchive& refNative();
		NativeArchive const& refNative() const;
	public:
		Archive();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
		//
		//	SUMMARY: 内部系メソッド
		//
		sprig::krkr::tjs::intptr_type getStack();
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		tTJSVariant saveText() const;
		void loadText(tTJSVariantString const* data);
		tTJSVariant saveXML() const;
		void loadXML(tTJSVariantString const* data);
		tTJSVariant saveBinary() const;
		void loadBinary(tTJSVariantOctet const* data);
		//
		//	SUMMARY: Ptシリアライズ系メソッド
		//
		tTJSVariant savePtXML(
			boost::optional<tTVInteger> indent_char = boost::none,
			boost::optional<tTVInteger> indent_count = boost::none,
			tTJSVariantString const* encoding = 0
			) const;
		void loadPtXML(tTJSVariantString const* data);
		tTJSVariant savePtJSON() const;
		void loadPtJSON(tTJSVariantString const* data);
		tTJSVariant savePtINFO(
			boost::optional<tTVInteger> indent_char = boost::none,
			boost::optional<tTVInteger> = boost::none
			) const;
		void loadPtINFO(tTJSVariantString const* data);
		//
		//	SUMMARY: 操作系メソッド
		//
		void clear();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		tTVInteger size() const;
	};
}	// namespace ktl
