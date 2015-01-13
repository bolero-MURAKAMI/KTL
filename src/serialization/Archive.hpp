/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_SERIALIZATION_ARCHIVE_HPP
#define SRC_SERIALIZATION_ARCHIVE_HPP

#include <sstream>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/archive/text_woarchive.hpp>
#include <boost/archive/text_wiarchive.hpp>
#include <boost/archive/xml_woarchive.hpp>
#include <boost/archive/xml_wiarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>
#include <ktl/serialization.hpp>

#include "ArchiveCommon.hpp"
#include "ArchiveDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Archive:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Archive:error")

namespace ktl {
	//
	// NativeArchive
	//
	ElementStack& NativeArchive::getElementStack() {
		return element_stack_;
	}
	NativeArchive::NativeArchive() {}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	KTL_INLINE NativeArchive::string_instance_type NativeArchive::saveText() const {
		std::basic_ostringstream<tjs_char> os;
		{
			boost::archive::text_woarchive oa(os);
			element_stack_.saveArchive(oa);
		}
		string_instance_type result(new serialization_string_type(os.str()));
		return result;
	}
	KTL_INLINE void NativeArchive::loadText(tjs_char const* data) {
		std::basic_istringstream<tjs_char> is(data ? data : SPRIG_KRKR_TJS_W(""));
		{
			boost::archive::text_wiarchive ia(is);
			element_stack_.loadArchive(ia);
		}
	}
	KTL_INLINE NativeArchive::string_instance_type NativeArchive::saveXML() const {
		std::basic_ostringstream<tjs_char> os;
		{
			boost::archive::xml_woarchive oa(os);
			element_stack_.saveArchive(oa);
		}
		string_instance_type result(new serialization_string_type(os.str()));
		return result;
	}
	KTL_INLINE void NativeArchive::loadXML(tjs_char const* data) {
		std::basic_istringstream<tjs_char> is(data ? data : SPRIG_KRKR_TJS_W(""));
		{
			boost::archive::xml_wiarchive ia(is);
			element_stack_.loadArchive(ia);
		}
	}
	KTL_INLINE NativeArchive::binary_instance_type NativeArchive::saveBinary() const {
		typedef boost::iostreams::back_insert_device<binary_type> device_type;
		typedef boost::iostreams::stream<device_type> stream_type;

		binary_instance_type result(new binary_type());
		device_type dev(*result);
		stream_type os(dev, std::ios::out | std::ios::binary);
		{
			boost::archive::binary_oarchive oa(os);
			element_stack_.saveArchive(oa);
		}
		return result;
	}
	KTL_INLINE void NativeArchive::loadBinary(tTJSVariantOctet const* data) {
		typedef boost::iostreams::basic_array_source<binary_type::value_type> device_type;
		typedef boost::iostreams::stream<device_type> stream_type;

		device_type dev(
			reinterpret_cast<binary_type::value_type const*>(sprig::krkr::tjs::octet_data(data)),
			sprig::krkr::tjs::octet_length(data)
			);
		stream_type is(dev, std::ios::in | std::ios::binary);
		{
			boost::archive::binary_iarchive ia(is);
			element_stack_.loadArchive(ia);
		}
	}
	//
	//	SUMMARY: Ptシリアライズ系メソッド
	//
	KTL_INLINE NativeArchive::string_instance_type NativeArchive::savePtXML(
		boost::optional<tjs_char> indent_char,
		boost::optional<size_type> indent_count,
		tjs_char const* encoding
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		{
			boost::property_tree::basic_ptree<serialization_string_type, serialization_string_type> pt;
			element_stack_.savePtArchive(pt);
			boost::property_tree::write_xml(
				os,
				pt,
				boost::property_tree::xml_writer_settings<std::basic_string<tjs_char> >(
					indent_char ? *indent_char : SPRIG_KRKR_TJS_W(' '),
					indent_count ? *indent_count : 4,
					encoding ? encoding : SPRIG_KRKR_TJS_W("UTF-16")
					)
				);
		}
		string_instance_type result(new serialization_string_type(os.str()));
		return result;
	}
	KTL_INLINE void NativeArchive::loadPtXML(tjs_char const* data) {
		std::basic_istringstream<tjs_char> is(data ? data : SPRIG_KRKR_TJS_W(""));
		{
			boost::property_tree::basic_ptree<serialization_string_type, serialization_string_type> pt;
			boost::property_tree::read_xml(
				is,
				pt,
				boost::property_tree::xml_parser::no_comments
				);
			element_stack_.loadPtArchive(pt);
		}
	}
	KTL_INLINE NativeArchive::string_instance_type NativeArchive::savePtJSON() const
	{
		std::basic_ostringstream<tjs_char> os;
		{
			boost::property_tree::basic_ptree<serialization_string_type, serialization_string_type> pt;
			element_stack_.savePtArchive(pt);
			boost::property_tree::write_json(
				os,
				pt
				);
		}
		string_instance_type result(new serialization_string_type(os.str()));
		return result;
	}
	KTL_INLINE void NativeArchive::loadPtJSON(tjs_char const* data) {
		std::basic_istringstream<tjs_char> is(data ? data : SPRIG_KRKR_TJS_W(""));
		{
			boost::property_tree::basic_ptree<serialization_string_type, serialization_string_type> pt;
			boost::property_tree::read_json(
				is,
				pt
				);
			element_stack_.loadPtArchive(pt);
		}
	}
	KTL_INLINE NativeArchive::string_instance_type NativeArchive::savePtINFO(
		boost::optional<tjs_char> indent_char,
		boost::optional<size_type> indent_count
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		{
			boost::property_tree::basic_ptree<serialization_string_type, serialization_string_type> pt;
			element_stack_.savePtArchive(pt);
			boost::property_tree::write_info(
				os,
				pt,
				boost::property_tree::info_writer_settings<tjs_char>(
					indent_char ? *indent_char : SPRIG_KRKR_TJS_W(' '),
					indent_count ? *indent_count : 4
					)
				);
		}
		string_instance_type result(new serialization_string_type(os.str()));
		return result;
	}
	KTL_INLINE void NativeArchive::loadPtINFO(tjs_char const* data) {
		std::basic_istringstream<tjs_char> is(data ? data : SPRIG_KRKR_TJS_W(""));
		{
			boost::property_tree::basic_ptree<serialization_string_type, serialization_string_type> pt;
			boost::property_tree::read_info(
				is,
				pt
				);
			element_stack_.loadPtArchive(pt);
		}
	}
	//
	//	SUMMARY: 操作系メソッド
	//
	KTL_INLINE void NativeArchive::clear() {
		element_stack_.clear();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE NativeArchive::size_type NativeArchive::size() const {
		return element_stack_.size();
	}

	//
	// Archive
	//
	NativeArchive& Archive::refNative() {
		return *instance_;
	}
	NativeArchive const& Archive::refNative() const {
		return *instance_;
	}
	Archive::Archive() {}
	tjs_error TJS_INTF_METHOD Archive::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Archive::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeArchive>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Archive::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Archive::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 内部系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type Archive::getStack() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(
			static_cast<ISerializationStack*>(&refNative().getElementStack())
			);
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	KTL_INLINE tTJSVariant Archive::saveText() const {
		return tTJSVariant(instance_->saveText()->c_str());
	}
	KTL_INLINE void Archive::loadText(tTJSVariantString const* data) {
		instance_->loadText(sprig::krkr::tjs::as_c_str(data));
	}
	KTL_INLINE tTJSVariant Archive::saveXML() const {
		return tTJSVariant(instance_->saveXML()->c_str());
	}
	KTL_INLINE void Archive::loadXML(tTJSVariantString const* data) {
		instance_->loadXML(sprig::krkr::tjs::as_c_str(data));
	}
	KTL_INLINE tTJSVariant Archive::saveBinary() const {
		NativeArchive::binary_instance_type data(instance_->saveBinary());
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&(*data)[0]), data->size());
	}
	KTL_INLINE void Archive::loadBinary(tTJSVariantOctet const* data) {
		instance_->loadBinary(data);
	}
	//
	//	SUMMARY: Ptシリアライズ系メソッド
	//
	KTL_INLINE tTJSVariant Archive::savePtXML(
		boost::optional<tTVInteger> indent_char,
		boost::optional<tTVInteger> indent_count,
		tTJSVariantString const* encoding
		) const
	{
		return tTJSVariant(
			instance_->savePtXML(
				indent_char
					? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*indent_char))
					: boost::none
					,
				indent_count
					? boost::optional<NativeArchive::size_type>(boost::numeric_cast<NativeArchive::size_type>(*indent_count))
					: boost::none
					,
				encoding
					? sprig::krkr::tjs::as_c_str(encoding)
					: 0
				)->c_str()
			);
	}
	KTL_INLINE void Archive::loadPtXML(tTJSVariantString const* data) {
		instance_->loadPtXML(
			sprig::krkr::tjs::as_c_str(data)
			);
	}
	KTL_INLINE tTJSVariant Archive::savePtJSON() const
	{
		return tTJSVariant(
			instance_->savePtJSON()->c_str()
			);
	}
	KTL_INLINE void Archive::loadPtJSON(tTJSVariantString const* data) {
		instance_->loadPtJSON(sprig::krkr::tjs::as_c_str(data));
	}
	KTL_INLINE tTJSVariant Archive::savePtINFO(
		boost::optional<tTVInteger> indent_char,
		boost::optional<tTVInteger> indent_count
		) const
	{
		return tTJSVariant(
			instance_->savePtINFO(
				indent_char
					? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*indent_char))
					: boost::none
					,
				indent_count
					? boost::optional<NativeArchive::size_type>(boost::numeric_cast<NativeArchive::size_type>(*indent_count))
					: boost::none
				)->c_str()
			);
	}
	KTL_INLINE void Archive::loadPtINFO(tTJSVariantString const* data) {
		instance_->loadPtINFO(sprig::krkr::tjs::as_c_str(data));
	}
	//
	//	SUMMARY: 操作系メソッド
	//
	KTL_INLINE void Archive::clear() {
		instance_->clear();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tTVInteger Archive::size() const {
		return instance_->size();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_SERIALIZATION_ARCHIVE_HPP
