#pragma once

#include <cstddef>
#include <string>
#include <functional>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/cstdint.hpp>
#include <sprig/external/unicode/ucnv.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/property_tree/yaml_parser_flags.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativePTree
	//
	class NativePTree {
	public:
		typedef boost::int_fast32_t int_type;
		typedef boost::uint_fast32_t flag_type;
		typedef std::vector<tjs_uint8> binary_type;
		typedef std::basic_string<tjs_char> pt_string_type;
		typedef boost::property_tree::basic_ptree<pt_string_type, pt_string_type> pt_ptree_type;
		typedef std::size_t size_type;
	private:
		//
		// SortCompare
		//
		template<typename KeyCompare = pt_ptree_type::key_compare>
		class SortCompare
			: public std::binary_function<pt_ptree_type::value_type, pt_ptree_type::value_type, bool>
			, private KeyCompare
		{
		public:
			typedef KeyCompare key_compare;
		public:
			explicit SortCompare(key_compare key_comp = KeyCompare());
			bool operator()(
				pt_ptree_type::value_type const& lhs,
				pt_ptree_type::value_type const& rhs
				) const;
		};
		//
		// SpecAnalyzer
		//
		class SpecAnalyzer {
		private:
			bool op_nop_;
			bool op_ins_;
			bool op_seq_;
		public:
			explicit SpecAnalyzer(pt_string_type& spec);
			bool opNop() const;
			bool opIns() const;
			bool opSeq() const;
		};
		//
		// CodeStrConverter
		//
		class CodeStrConverter {
		private:
			UConverter* converter_;
		public:
			explicit CodeStrConverter(UConverter* converter);
			std::string to_str(pt_string_type const& from) const;
			pt_string_type from_str(std::string const& from) const;
		};
	private:
		static sprig::krkr::tjs::intptr_type getInstance(iTJSDispatch2* obj);
		static sprig::krkr::tjs::object_type createNew(tjs_int numparams, tTJSVariant** param);
		static bool encodeAsBuffer(
			binary_type& buffer,
			tjs_char const* to,
			tjs_char const* source
			);
		static bool encodeAsString(
			pt_string_type& string,
			tjs_uint8 const* source_data,
			tjs_uint source_length,
			tjs_char const* from
			);
		static bool loadStorage(
			binary_type& binary,
			tjs_char const* storage
			);
		static int_type saveStorage(
			tjs_char const* storage,
			void const* data,
			ULONG length
			);
		template<typename Ptree>
		boost::optional<Ptree&> getNodeInsertable(
			Ptree& ptree,
			tjs_char const* path_insertable,
			tjs_char separator,
			tjs_char sharp,
			boost::optional<boost::optional<Ptree&>&> parent = boost::none,
			boost::optional<typename boost::range_iterator<Ptree>::type&> where = boost::none
			);
		template<typename Ptree>
		static boost::optional<Ptree&> getNodeExists(
			Ptree& ptree,
			tjs_char const* path_exists,
			tjs_char separator,
			tjs_char sharp,
			boost::optional<boost::optional<Ptree&>&> parent = boost::none,
			boost::optional<typename boost::range_iterator<Ptree>::type&> where = boost::none
			);
	private:
		pt_ptree_type ptree_;
		tjs_char path_separator_;
		tjs_char path_sharp_;
	public:
		NativePTree();
		void init(pt_ptree_type const& ptree);
		pt_ptree_type const& getValue() const;
	public:
		//
		//	SUMMARY: XMLパース系フラグ
		//
		static flag_type const pfXMLNoConcatText = boost::property_tree::xml_parser::no_concat_text;
		static flag_type const pfXMLNoComments = boost::property_tree::xml_parser::no_comments;
		static flag_type const pfXMLTrimWhitespace = boost::property_tree::xml_parser::trim_whitespace;
		//
		//	SUMMARY: YAMLパース系フラグ
		//
		static flag_type const pfYAMLMultiDocument = sprig::property_tree::yaml_parser::reader_flags::multi_document;
		//
		//	SUMMARY: YAMLエミット系フラグ
		//
		static flag_type const efYAMLMultiDocument = sprig::property_tree::yaml_parser::writer_flags::multi_document;
		static flag_type const efYAMLInsertEndOfDocument = sprig::property_tree::yaml_parser::writer_flags::insert_end_of_document;
		//
		//	SUMMARY: HTMLパース系フラグ
		//
		static flag_type const pfHTMLNoConcatText = sprig::property_tree::html_parser::reader_flags::no_concat_text;
		static flag_type const pfHTMLNoComments = sprig::property_tree::html_parser::reader_flags::no_comments;
		static flag_type const pfHTMLTrimWhitespace = sprig::property_tree::html_parser::reader_flags::trim_whitespace;
	public:
		//
		//	SUMMARY: アクセス系メソッド
		//
		tTJSVariant get(tjs_char const* path_exists) const;
		tTJSVariant getKey(tjs_char const* path_exists) const;
		bool put(tjs_char const* path_insertable, tjs_char const* data);
		bool erase(tjs_char const* path_exists);
		int_type erase(tjs_char const* path_exists, tjs_char const* key);
		bool exist(tjs_char const* path_exists) const;
		int_type size(tjs_char const* path_exists) const;
		int_type count(tjs_char const* path_exists, tjs_char const* key) const;
		bool clear(tjs_char const* path_exists);
		bool sort(tjs_char const* path_exists, tTJSVariant const* comp = 0, bool stable = false);
		bool reverse(tjs_char const* path_exists);
		//
		//	SUMMARY: 複製系メソッド
		//
		sprig::krkr::tjs::object_type clone(tjs_char const* path_exists) const;
		bool assign(tjs_char const* path_insertable, iTJSDispatch2* obj);
		//
		//	SUMMARY: XML読み込み系メソッド
		//
		bool readXML(
			tjs_char const* source,
			flag_type flag = 0x00000000
			);
		bool readXML(
			tTJSVariantOctet const* source,
			tjs_char const* code,
			flag_type flag = 0x00000000
			);
		bool readStorageXML(
			tjs_char const* storage,
			flag_type flag = 0x00000000
			);
		bool readStorageXML(
			tjs_char const* storage,
			tjs_char const* code,
			flag_type flag = 0x00000000
			);
		//
		//	SUMMARY: JSON読み込み系メソッド
		//
		bool readJSON(
			tjs_char const* source
			);
		bool readJSON(
			tTJSVariantOctet const* source,
			tjs_char const* code
			);
		bool readStorageJSON(
			tjs_char const* storage
			);
		bool readStorageJSON(
			tjs_char const* storage,
			tjs_char const* code
			);
		//
		//	SUMMARY: INI読み込み系メソッド
		//
		bool readINI(
			tjs_char const* source
			);
		bool readINI(
			tTJSVariantOctet const* source,
			tjs_char const* code
			);
		bool readStorageINI(
			tjs_char const* storage
			);
		bool readStorageINI(
			tjs_char const* storage,
			tjs_char const* code
			);
		//
		//	SUMMARY: INFO読み込み系メソッド
		//
		bool readINFO(
			tjs_char const* source
			);
		bool readINFO(
			tTJSVariantOctet const* source,
			tjs_char const* code
			);
		bool readStorageINFO(
			tjs_char const* storage
			);
		bool readStorageINFO(
			tjs_char const* storage,
			tjs_char const* code
			);
		//
		//	SUMMARY: YAML読み込み系メソッド
		//
		bool readYAML(
			tjs_char const* source,
			flag_type flag = 0x00000000
			);
		bool readYAML(
			tTJSVariantOctet const* source,
			tjs_char const* code,
			flag_type flag = 0x00000000
			);
		bool readStorageYAML(
			tjs_char const* storage,
			flag_type flag = 0x00000000
			);
		bool readStorageYAML(
			tjs_char const* storage,
			tjs_char const* code,
			flag_type flag = 0x00000000
			);
		//
		//	SUMMARY: HTML読み込み系メソッド
		//
		bool readHTML(
			tjs_char const* source,
			flag_type flag = 0x00000000
			);
		bool readHTML(
			tTJSVariantOctet const* source,
			tjs_char const* code,
			flag_type flag = 0x00000000
			);
		bool readStorageHTML(
			tjs_char const* storage,
			flag_type flag = 0x00000000
			);
		bool readStorageHTML(
			tjs_char const* storage,
			tjs_char const* code,
			flag_type flag = 0x00000000
			);
		//
		//	SUMMARY: XML書き出し系メソッド
		//
		tTJSVariant writeXMLToString(
			boost::optional<tjs_char> indent_char = boost::none,
			boost::optional<size_type> indent_count = boost::none
			) const;
		tTJSVariant writeXMLToOctet(
			tjs_char const* code,
			boost::optional<tjs_char> indent_char = boost::none,
			boost::optional<size_type> indent_count = boost::none
			) const;
		int_type writeStorageXML(
			tjs_char const* storage,
			tjs_char const* code = 0,
			boost::optional<tjs_char> indent_char = boost::none,
			boost::optional<size_type> indent_count = boost::none
			) const;
		//
		//	SUMMARY: JSON書き出し系メソッド
		//
		tTJSVariant writeJSONToString(
			bool pretty = true
			) const;
		tTJSVariant writeJSONToOctet(
			tjs_char const* code,
			bool pretty = true
			) const;
		int_type writeStorageJSON(
			tjs_char const* storage,
			tjs_char const* code = 0,
			bool pretty = true
			) const;
		//
		//	SUMMARY: INI書き出し系メソッド
		//
		tTJSVariant writeINIToString() const;
		tTJSVariant writeINIToOctet(
			tjs_char const* code
			) const;
		int_type writeStorageINI(
			tjs_char const* storage,
			tjs_char const* code = 0
			) const;
		//
		//	SUMMARY: INFO書き出し系メソッド
		//
		tTJSVariant writeINFOToString(
			boost::optional<tjs_char> indent_char = boost::none,
			boost::optional<size_type> indent_count = boost::none
			) const;
		tTJSVariant writeINFOToOctet(
			tjs_char const* code,
			boost::optional<tjs_char> indent_char = boost::none,
			boost::optional<size_type> indent_count = boost::none
			) const;
		int_type writeStorageINFO(
			tjs_char const* storage,
			tjs_char const* code = 0,
			boost::optional<tjs_char> indent_char = boost::none,
			boost::optional<size_type> indent_count = boost::none
			) const;
		//
		//	SUMMARY: YAML書き出し系メソッド
		//
		tTJSVariant writeYAMLToString(
			flag_type flag = 0x00000000
			) const;
		tTJSVariant writeYAMLToOctet(
			tjs_char const* code,
			flag_type flag = 0x00000000
			) const;
		int_type writeStorageYAML(
			tjs_char const* storage,
			tjs_char const* code = 0,
			flag_type flag = 0x00000000
			) const;
		//
		//	SUMMARY: フォーマット系メソッド
		//
		bool trimXML();
		bool stratifyINI(boost::optional<tjs_char> path_separator = boost::none);
		bool unstratifyINI(boost::optional<tjs_char> path_separator = boost::none);
		//
		//	SUMMARY: プロパティ系メソッド
		//
		tjs_char getPathSeparator() const;
		void setPathSeparator(tjs_char path_separator);
		tjs_char getPathSharp() const;
		void setPathSharp(tjs_char path_sharp);
	};

	//
	// PTree
	//
	class PTree
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativePTree> instance_;
	public:
		PTree();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		void init(NativePTree::pt_ptree_type const& ptree);
		NativePTree::pt_ptree_type const& getValue() const;
	public:
		//
		//	SUMMARY: XMLパース系フラグ
		//
		static tTVInteger const pfXMLNoConcatText = NativePTree::pfXMLNoConcatText;
		static tTVInteger const pfXMLNoComments = NativePTree::pfXMLNoComments;
		static tTVInteger const pfXMLTrimWhitespace = NativePTree::pfXMLTrimWhitespace;
		//
		//	SUMMARY: YAMLパース系フラグ
		//
		static tTVInteger const pfYAMLMultiDocument = NativePTree::pfYAMLMultiDocument;
		//
		//	SUMMARY: YAMLエミット系フラグ
		//
		static tTVInteger const efYAMLMultiDocument = NativePTree::efYAMLMultiDocument;
		static tTVInteger const efYAMLInsertEndOfDocument = NativePTree::efYAMLInsertEndOfDocument;
		//
		//	SUMMARY: HTMLパース系フラグ
		//
		static tTVInteger const pfHTMLNoConcatText = NativePTree::pfHTMLNoConcatText;
		static tTVInteger const pfHTMLNoComments = NativePTree::pfHTMLNoComments;
		static tTVInteger const pfHTMLTrimWhitespace = NativePTree::pfHTMLTrimWhitespace;
	public:
		//
		//	SUMMARY: 内部系メソッド
		//
		sprig::krkr::tjs::intptr_type getInstance();
		//
		//	SUMMARY: アクセス系メソッド
		//
		tTJSVariant get(tTJSVariantString const* path_exists) const;
		tTJSVariant getKey(tTJSVariantString const* path_exists) const;
		bool put(tTJSVariantString const* path_insertable, tTJSVariantString const* data);
		bool erase(tTJSVariantString const* path_exists);
		tTVInteger erase(tTJSVariantString const* path_exists, tTJSVariantString const* key);
		bool exist(tTJSVariantString const* path_exists) const;
		tTVInteger size(tTJSVariantString const* path_exists) const;
		tTVInteger count(tTJSVariantString const* path_exists, tTJSVariantString const* key) const;
		bool clear(tTJSVariantString const* path_exists);
		bool sort(tTJSVariantString const* path_exists, tTJSVariant const* comp = 0, bool stable = false);
		bool reverse(tTJSVariantString const* path_exists);
		//
		//	SUMMARY: 複製系メソッド
		//
		sprig::krkr::tjs::object_type clone(tTJSVariantString const* path_exists) const;
		bool assign(tTJSVariantString const* path_insertable, iTJSDispatch2* obj);
		//
		//	SUMMARY: XML読み込み系メソッド
		//
		bool readXML(
			tTJSVariantString const* source,
			tTVInteger flag = 0x00000000
			);
		bool readXML(
			tTJSVariantOctet const* source,
			tTJSVariantString const* code,
			tTVInteger flag = 0x00000000
			);
		bool readStorageXML(
			tTJSVariantString const* storage,
			tTVInteger flag = 0x00000000
			);
		bool readStorageXML(
			tTJSVariantString const* storage,
			tTJSVariantString const* code,
			tTVInteger flag = 0x00000000
			);
		//
		//	SUMMARY: JSON読み込み系メソッド
		//
		bool readJSON(
			tTJSVariantString const* source
			);
		bool readJSON(
			tTJSVariantOctet const* source,
			tTJSVariantString const* code
			);
		bool readStorageJSON(
			tTJSVariantString const* storage
			);
		bool readStorageJSON(
			tTJSVariantString const* storage,
			tTJSVariantString const* code
			);
		//
		//	SUMMARY: INI読み込み系メソッド
		//
		bool readINI(
			tTJSVariantString const* source
			);
		bool readINI(
			tTJSVariantOctet const* source,
			tTJSVariantString const* code
			);
		bool readStorageINI(
			tTJSVariantString const* storage
			);
		bool readStorageINI(
			tTJSVariantString const* storage,
			tTJSVariantString const* code
			);
		//
		//	SUMMARY: INFO読み込み系メソッド
		//
		bool readINFO(
			tTJSVariantString const* source
			);
		bool readINFO(
			tTJSVariantOctet const* source,
			tTJSVariantString const* code
			);
		bool readStorageINFO(
			tTJSVariantString const* storage
			);
		bool readStorageINFO(
			tTJSVariantString const* storage,
			tTJSVariantString const* code
			);
		//
		//	SUMMARY: YAML読み込み系メソッド
		//
		bool readYAML(
			tTJSVariantString const* source,
			tTVInteger flag = 0x00000000
			);
		bool readYAML(
			tTJSVariantOctet const* source,
			tTJSVariantString const* code,
			tTVInteger flag = 0x00000000
			);
		bool readStorageYAML(
			tTJSVariantString const* storage,
			tTVInteger flag = 0x00000000
			);
		bool readStorageYAML(
			tTJSVariantString const* storage,
			tTJSVariantString const* code,
			tTVInteger flag = 0x00000000
			);
		//
		//	SUMMARY: HTML読み込み系メソッド
		//
		bool readHTML(
			tTJSVariantString const* source,
			tTVInteger flag = 0x00000000
			);
		bool readHTML(
			tTJSVariantOctet const* source,
			tTJSVariantString const* code,
			tTVInteger flag = 0x00000000
			);
		bool readStorageHTML(
			tTJSVariantString const* storage,
			tTVInteger flag = 0x00000000
			);
		bool readStorageHTML(
			tTJSVariantString const* storage,
			tTJSVariantString const* code,
			tTVInteger flag = 0x00000000
			);
		//
		//	SUMMARY: XML書き出し系メソッド
		//
		tTJSVariant writeXMLToString(
			boost::optional<tTVInteger> indent_char = boost::none,
			boost::optional<tTVInteger> indent_count = boost::none
			) const;
		tTJSVariant writeXMLToOctet(
			tTJSVariantString const* code,
			boost::optional<tTVInteger> indent_char = boost::none,
			boost::optional<tTVInteger> indent_count = boost::none
			) const;
		tTVInteger writeStorageXML(
			tTJSVariantString const* storage,
			tTJSVariantString const* code = 0,
			boost::optional<tTVInteger> indent_char = boost::none,
			boost::optional<tTVInteger> indent_count = boost::none
			) const;
		//
		//	SUMMARY: JSON書き出し系メソッド
		//
		tTJSVariant writeJSONToString(
			bool pretty = true
			) const;
		tTJSVariant writeJSONToOctet(
			tTJSVariantString const* code,
			bool pretty = true
			) const;
		tTVInteger writeStorageJSON(
			tTJSVariantString const* storage,
			tTJSVariantString const* code = 0,
			bool pretty = true
			) const;
		//
		//	SUMMARY: INI書き出し系メソッド
		//
		tTJSVariant writeINIToString() const;
		tTJSVariant writeINIToOctet(
			tTJSVariantString const* code
			) const;
		tTVInteger writeStorageINI(
			tTJSVariantString const* storage,
			tTJSVariantString const* code = 0
			) const;
		//
		//	SUMMARY: INFO書き出し系メソッド
		//
		tTJSVariant writeINFOToString(
			boost::optional<tTVInteger> indent_char = boost::none,
			boost::optional<tTVInteger> indent_count = boost::none
			) const;
		tTJSVariant writeINFOToOctet(
			tTJSVariantString const* code,
			boost::optional<tTVInteger> indent_char = boost::none,
			boost::optional<tTVInteger> indent_count = boost::none
			) const;
		tTVInteger writeStorageINFO(
			tTJSVariantString const* storage,
			tTJSVariantString const* code = 0,
			boost::optional<tTVInteger> indent_char = boost::none,
			boost::optional<tTVInteger> indent_count = boost::none
			) const;
		//
		//	SUMMARY: YAML書き出し系メソッド
		//
		tTJSVariant writeYAMLToString(
			tTVInteger flag = 0x00000000
			) const;
		tTJSVariant writeYAMLToOctet(
			tTJSVariantString const* code,
			tTVInteger flag = 0x00000000
			) const;
		tTVInteger writeStorageYAML(
			tTJSVariantString const* storage,
			tTJSVariantString const* code = 0,
			tTVInteger flag = 0x00000000
			) const;
		//
		//	SUMMARY: フォーマット系メソッド
		//
		bool trimXML();
		bool stratifyINI(boost::optional<tTVInteger> path_separator = boost::none);
		bool unstratifyINI(boost::optional<tTVInteger> path_separator = boost::none);
		//
		//	SUMMARY: プロパティ系メソッド
		//
		tTVInteger getPathSeparator() const;
		void setPathSeparator(tTVInteger path_separator);
		tTVInteger getPathSharp() const;
		void setPathSharp(tTVInteger path_sharp);
	};
}	// namespace ktl
