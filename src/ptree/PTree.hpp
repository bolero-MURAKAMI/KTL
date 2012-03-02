#pragma once

#include <iterator>
#include <sstream>
#include <functional>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/range.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/utility/swap.hpp>
#include <sprig/external/unicode/ucnv.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/get_pointer.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/str_length.hpp>
#include <sprig/str_compare.hpp>
#include <sprig/predicate.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/property_tree/yaml_parser.hpp>
#include <sprig/property_tree/html_parser.hpp>
#include <sprig/property_tree/str_converter.hpp>
#include <sprig/utility/property_tree.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "PTreeDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:PTree:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:PTree:error")

namespace ktl {
	//
	// NativePTree::SortCompare
	//
	template<typename KeyCompare>
	KTL_INLINE NativePTree::SortCompare<KeyCompare>::SortCompare(key_compare key_comp)
		: key_compare(key_comp)
	{}
	template<typename KeyCompare>
	KTL_INLINE bool NativePTree::SortCompare<KeyCompare>::operator()(
		pt_ptree_type::value_type const& lhs,
		pt_ptree_type::value_type const& rhs
		) const
	{
		return key_compare::operator()(lhs.first, rhs.first);
	}
	//
	// NativePTree::SpecAnalyzer
	//
	KTL_INLINE NativePTree::SpecAnalyzer::SpecAnalyzer(pt_string_type& spec)
		: op_nop_(false)
		, op_ins_(false)
		, op_seq_(false)
	{
		pt_string_type::reverse_iterator i = spec.rbegin();
		pt_string_type::reverse_iterator last = spec.rend();
		for (; i != last; ++i) {
			switch (*i) {
			case SPRIG_KRKR_TJS_W('-'):
				op_nop_ = true;
				break;
			case SPRIG_KRKR_TJS_W('|'):
				op_ins_ = true;
				break;
			case SPRIG_KRKR_TJS_W('+'):
				op_seq_ = true;
				break;
			default:
				goto loop_breaked;
			}
		}
	loop_breaked:
		spec.erase(i.base(), spec.end());
	}
	KTL_INLINE bool NativePTree::SpecAnalyzer::opNop() const {
		return op_nop_;
	}
	KTL_INLINE bool NativePTree::SpecAnalyzer::opIns() const {
		return op_ins_;
	}
	KTL_INLINE bool NativePTree::SpecAnalyzer::opSeq() const {
		return op_seq_;
	}
	//
	// NativePTree::CodeStrConverter
	//
	KTL_INLINE NativePTree::CodeStrConverter::CodeStrConverter(UConverter* converter)
		: converter_(converter)
	{}
	KTL_INLINE std::string NativePTree::CodeStrConverter::to_str(pt_string_type const& from) const {
		std::string buffer;
		tjs_char const* source = from.c_str();
		std::size_t source_length = from.size();
		if (!source_length) {
			return buffer;
		}
		//	COMMENT: 変換
		UErrorCode error = U_ZERO_ERROR;
		int32_t length = ::ucnv_fromUChars(
			converter_,
			0,
			0,
			source,
			source_length,
			&error
			);
		if (error != U_BUFFER_OVERFLOW_ERROR || length <= 0) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_fromUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return buffer;
		}
		buffer.resize(length);
		error = U_ZERO_ERROR;
		::ucnv_fromUChars(
			converter_,
			reinterpret_cast<char*>(&buffer[0]),
			buffer.size(),
			source,
			source_length,
			&error
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_fromUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return buffer;
		}
		return buffer;
	}
	KTL_INLINE NativePTree::pt_string_type NativePTree::CodeStrConverter::from_str(std::string const& from) const {
		NativePTree::pt_string_type string;
		char const* source = from.c_str();
		std::size_t source_length = from.size();
		if (!source_length) {
			return string;
		}
		//	COMMENT: 変換
		UErrorCode error = U_ZERO_ERROR;
		int32_t length = ::ucnv_toUChars(
			converter_,
			0,
			0,
			source,
			source_length,
			&error
			);
		if (error != U_BUFFER_OVERFLOW_ERROR || length <= 0) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_toUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return string;
		}
		string.resize(length);
		error = U_ZERO_ERROR;
		::ucnv_toUChars(
			converter_,
			&string[0],
			string.size(),
			source,
			source_length,
			&error
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_toUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return string;
		}
		return string;
	}
	//
	// NativePTree
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type NativePTree::getInstance(iTJSDispatch2* obj) {
		return sprig::krkr::tjs::GetPropValue<sprig::krkr::tjs::intptr_type>(
			sprig::krkr::tjs::GetMemberNoAddRef(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("PTree")),
				SPRIG_KRKR_TJS_W("instance")
				),
			obj
			);
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativePTree::createNew(tjs_int numparams, tTJSVariant** param) {
		iTJSDispatch2* result_obj = 0;
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("PTree")),
			&result_obj,
			numparams,
			param,
			0
			);
		return sprig::krkr::tjs::object_type(result_obj, false);
	}
	KTL_INLINE bool NativePTree::encodeAsBuffer(
		binary_type& buffer,
		tjs_char const* to,
		tjs_char const* source
		)
	{
		//	COMMENT: コンバータ作成
		UErrorCode error = U_ZERO_ERROR;
		boost::shared_ptr<UConverter> converter(
			::ucnv_openU(to, &error),
			&::ucnv_close
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_openU: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		std::size_t source_length = sprig::str_length(source);
		if (!source_length) {
			buffer.clear();
			return true;
		}
		//	COMMENT: 変換
		error = U_ZERO_ERROR;
		int32_t length = ::ucnv_fromUChars(
			converter.get(),
			0,
			0,
			source,
			source_length,
			&error
			);
		if (error != U_BUFFER_OVERFLOW_ERROR || length <= 0) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_fromUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		buffer.resize(length);
		error = U_ZERO_ERROR;
		::ucnv_fromUChars(
			converter.get(),
			reinterpret_cast<char*>(&buffer[0]),
			buffer.size(),
			source,
			source_length,
			&error
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_fromUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativePTree::encodeAsString(
		pt_string_type& string,
		tjs_uint8 const* source_data,
		tjs_uint source_length,
		tjs_char const* from
		)
	{
		//	COMMENT: コンバータ作成
		UErrorCode error = U_ZERO_ERROR;
		boost::shared_ptr<UConverter> converter(
			::ucnv_openU(from, &error),
			&::ucnv_close
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_openU: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		if (!source_length) {
			string.clear();
			return true;
		}
		//	COMMENT: 変換
		error = U_ZERO_ERROR;
		int32_t length = ::ucnv_toUChars(
			converter.get(),
			0,
			0,
			reinterpret_cast<char const*>(source_data),
			source_length,
			&error
			);
		if (error != U_BUFFER_OVERFLOW_ERROR || length <= 0) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_toUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		string.resize(length);
		error = U_ZERO_ERROR;
		::ucnv_toUChars(
			converter.get(),
			&string[0],
			string.size(),
			reinterpret_cast<char const*>(source_data),
			source_length,
			&error
			);
		if (U_FAILURE(error)) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_toUChars: ")) + ::u_errorName(error),
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativePTree::loadStorage(
		binary_type& binary,
		tjs_char const* storage
		)
	{
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
	KTL_INLINE NativePTree::int_type NativePTree::saveStorage(
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
	template<typename Ptree>
	KTL_INLINE boost::optional<Ptree&> NativePTree::getNodeInsertable(
		Ptree& ptree,
		tjs_char const* path_insertable,
		tjs_char separator,
		tjs_char sharp,
		boost::optional<boost::optional<Ptree&>&> parent,
		boost::optional<typename boost::range_iterator<Ptree>::type&> where
		)
	{
		typedef typename boost::range_iterator<Ptree>::type iterator;
		typedef boost::escaped_list_separator<tjs_char> separator_type;
		typedef boost::tokenizer<separator_type, tjs_char const*, pt_string_type> tokenizer_type;
		separator_type sep(
			SPRIG_KRKR_TJS_W('\\'),
			separator,
			SPRIG_KRKR_TJS_W('\"')
			);
		tokenizer_type tokenizer(
			boost::iterator_range<tjs_char const*>(
				path_insertable,
				path_insertable + sprig::str_length(path_insertable)
				),
			sep
			);
		bool req_parent = parent;
		bool req_where = where;
		if (req_parent) {
			*parent = boost::none;
		}
		if (req_where) {
			*where = iterator();
		}
		boost::optional<pt_ptree_type&> node = ptree_;
		for (tokenizer_type::iterator token_i = tokenizer.begin(), token_last = tokenizer.end();
			token_i != token_last;
			++token_i
			)
		{
			tokenizer_type::value_type const& token = *token_i;
			pt_string_type::size_type sharp_pos = token.rfind(sharp);
			if (sharp_pos == pt_string_type::npos) {
				//	COMMENT: '#' が無い場合
				iterator found = node->to_iterator(node->find(token));
				if (found == node->end()) {
					found = node->push_back(
						pt_ptree_type::value_type(token, pt_ptree_type())
						);
				}
				if (req_parent) {
					*parent = node;
				}
				if (req_where) {
					*where = found;
				}
				node = found->second;
			} else if (sharp_pos == token.size() - 1) {
				//	COMMENT: '#' が末尾の場合
				pt_string_type key;
				pt_string_type spec = token.substr(0, sharp_pos);
				SpecAnalyzer spec_analyzer(spec);
				iterator found = node->end();
				if (!spec.empty()) {
					pt_ptree_type::size_type num = boost::lexical_cast<pt_ptree_type::size_type>(spec);
					if (num >= node->size()) {
						for (pt_ptree_type::size_type i = 0, last = num - node->size() + 1; i != last; ++i) {
							node->push_back(
								pt_ptree_type::value_type(key, pt_ptree_type())
								);
						}
					}
					found = node->begin();
					std::advance(found, num);
				}
				if (spec_analyzer.opIns() || spec_analyzer.opSeq()) {
					found = node->insert(
						found,
						pt_ptree_type::value_type(key, pt_ptree_type())
						);
				}
				if (found == node->end()) {
					return boost::none;
				}
				if (req_parent) {
					*parent = node;
				}
				if (req_where) {
					*where = found;
				}
				node = found->second;
			} else {
				//	COMMENT: '#' が中間の場合
				pt_string_type key = token.substr(0, sharp_pos);
				pt_string_type spec = token.substr(sharp_pos + 1);
				SpecAnalyzer spec_analyzer(spec);
				iterator found = node->end();
				if (spec_analyzer.opNop()) {
					found = node->to_iterator(node->find(key));
					if (found == node->end()) {
						found = node->push_back(
							pt_ptree_type::value_type(key, pt_ptree_type())
							);
					}
				} else if (!spec.empty()) {
					pt_ptree_type::size_type num = boost::lexical_cast<pt_ptree_type::size_type>(spec);
					pt_ptree_type::size_type count = 0;
					if (spec_analyzer.opSeq()) {
						if (num >= node->size()) {
							for (pt_ptree_type::size_type i = 0, last = num - node->size() + 1; i != last; ++i) {
								node->push_back(
									pt_ptree_type::value_type(key, pt_ptree_type())
									);
							}
						}
						found = node->begin();
						std::advance(found, num);
					} else {
						for (iterator i = node->begin(), last = node->end(); i != last; ++i) {
							if (i->first == key) {
								if (count == num) {
									found = i;
									break;
								}
								++count;
							}
						}
					}
					if (found == node->end()) {
						for (pt_ptree_type::size_type i = 0, last = num - count + 1; i != last; ++i) {
							node->push_back(
								pt_ptree_type::value_type(key, pt_ptree_type())
								);
						}
						found = node->end();
						--found;
					}
				}
				if (spec_analyzer.opIns() || spec_analyzer.opSeq()) {
					found = node->insert(
						found,
						pt_ptree_type::value_type(key, pt_ptree_type())
						);
				}
				if (found == node->end()) {
					return boost::none;
				}
				if (req_parent) {
					*parent = node;
				}
				if (req_where) {
					*where = found;
				}
				node = found->second;
			}
		}
		return node;
	}
	template<typename Ptree>
	KTL_INLINE boost::optional<Ptree&> NativePTree::getNodeExists(
		Ptree& ptree,
		tjs_char const* path_exists,
		tjs_char separator,
		tjs_char sharp,
		boost::optional<boost::optional<Ptree&>&> parent,
		boost::optional<typename boost::range_iterator<Ptree>::type&> where
		)
	{
		typedef typename boost::range_iterator<Ptree>::type iterator;
		typedef boost::escaped_list_separator<tjs_char> separator_type;
		typedef boost::tokenizer<separator_type, tjs_char const*, pt_string_type> tokenizer_type;
		separator_type sep(
			SPRIG_KRKR_TJS_W('\\'),
			separator,
			SPRIG_KRKR_TJS_W('\"')
			);
		tokenizer_type tokenizer(
			boost::iterator_range<tjs_char const*>(
				path_exists,
				path_exists + sprig::str_length(path_exists)
				),
			sep
			);
		bool req_parent = parent;
		bool req_where = where;
		if (req_parent) {
			*parent = boost::none;
		}
		if (req_where) {
			*where = iterator();
		}
		boost::optional<Ptree&> node = ptree;
		for (tokenizer_type::iterator token_i = tokenizer.begin(), token_last = tokenizer.end();
			token_i != token_last;
			++token_i
			)
		{
			tokenizer_type::value_type const& token = *token_i;
			pt_string_type::size_type sharp_pos = token.rfind(sharp);
			if (sharp_pos == pt_string_type::npos) {
				//	COMMENT: '#' が無い場合
				iterator found = node->to_iterator(node->find(token));
				if (found == node->end()) {
					return boost::none;
				}
				if (req_parent) {
					*parent = node;
				}
				if (req_where) {
					*where = found;
				}
				node = found->second;
			} else if (sharp_pos == token.size() - 1) {
				//	COMMENT: '#' が末尾の場合
				pt_string_type spec = token.substr(0, sharp_pos);
				SpecAnalyzer spec_analyzer(spec);
				iterator found = node->end();
				if (!spec.empty()) {
					pt_ptree_type::size_type num = boost::lexical_cast<pt_ptree_type::size_type>(spec);
					if (num < node->size()) {
						found = node->begin();
						std::advance(found, num);
					}
				}
				if (found == node->end()) {
					return boost::none;
				}
				if (req_parent) {
					*parent = node;
				}
				if (req_where) {
					*where = found;
				}
				node = found->second;
			} else {
				//	COMMENT: '#' が中間の場合
				pt_string_type key = token.substr(0, sharp_pos);
				pt_string_type spec = token.substr(sharp_pos + 1);
				SpecAnalyzer spec_analyzer(spec);
				iterator found = node->end();
				if (spec_analyzer.opNop()) {
					found = node->to_iterator(node->find(key));
				} else if (!spec.empty()) {
					pt_ptree_type::size_type num = boost::lexical_cast<pt_ptree_type::size_type>(spec);
					pt_ptree_type::size_type count = 0;
					for (iterator i = node->begin(), last = node->end(); i != last; ++i) {
						if (i->first == key) {
							if (count == num) {
								found = i;
								break;
							}
							++count;
						}
					}
				}
				if (found == node->end()) {
					return boost::none;
				}
				if (req_parent) {
					*parent = node;
				}
				if (req_where) {
					*where = found;
				}
				node = found->second;
			}
		}
		return node;
	}
	NativePTree::NativePTree()
		: path_separator_(SPRIG_KRKR_TJS_W('.'))
		, path_sharp_(SPRIG_KRKR_TJS_W('#'))
	{}
	KTL_INLINE void NativePTree::init(pt_ptree_type const& ptree) {
		ptree_ = ptree;
	}
	KTL_INLINE NativePTree::pt_ptree_type const& NativePTree::getValue() const {
		return ptree_;
	}
	//
	//	SUMMARY: アクセス系メソッド
	//
	KTL_INLINE tTJSVariant NativePTree::get(tjs_char const* path_exists) const {
		boost::optional<pt_ptree_type const&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_
			);
		if (!node) {
			return tTJSVariant();
		}
		return tTJSVariant(node->data().c_str());
	}
	KTL_INLINE tTJSVariant NativePTree::getKey(tjs_char const* path_exists) const {
		typedef boost::optional<pt_ptree_type const&> parent_type;
		typedef boost::range_iterator<pt_ptree_type const>::type where_type;
		parent_type parent;
		where_type where;
		boost::optional<pt_ptree_type const&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_,
			boost::optional<parent_type&>(parent),
			boost::optional<where_type&>(where)
			);
		if (!node || !parent) {
			return tTJSVariant();
		}
		return tTJSVariant(where->first.c_str());
	}
	KTL_INLINE bool NativePTree::put(tjs_char const* path_insertable, tjs_char const* data) {
		boost::optional<pt_ptree_type&> node = getNodeInsertable(
			ptree_,
			path_insertable,
			path_separator_,
			path_sharp_
			);
		if (!node) {
			return false;
		}
		node->data() = data;
		return true;
	}
	KTL_INLINE bool NativePTree::erase(tjs_char const* path_exists) {
		typedef boost::optional<pt_ptree_type&> parent_type;
		typedef boost::range_iterator<pt_ptree_type>::type where_type;
		parent_type parent;
		where_type where;
		boost::optional<pt_ptree_type&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_,
			boost::optional<parent_type&>(parent),
			boost::optional<where_type&>(where)
			);
		if (!node || !parent) {
			return false;
		}
		parent->erase(where);
		return true;
	}
	KTL_INLINE NativePTree::int_type NativePTree::erase(tjs_char const* path_exists, tjs_char const* key) {
		boost::optional<pt_ptree_type&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_
			);
		if (!node) {
			return -1;
		}
		return node->erase(key);
	}
	KTL_INLINE bool NativePTree::exist(tjs_char const* path_exists) const {
		boost::optional<pt_ptree_type const&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_
			);
		return node;
	}
	KTL_INLINE NativePTree::int_type NativePTree::size(tjs_char const* path_exists) const {
		boost::optional<pt_ptree_type const&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_
			);
		if (!node) {
			return -1;
		}
		return node->size();
	}
	KTL_INLINE NativePTree::int_type NativePTree::count(tjs_char const* path_exists, tjs_char const* key) const {
		boost::optional<pt_ptree_type const&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_
			);
		if (!node) {
			return -1;
		}
		return node->count(key);
	}
	KTL_INLINE bool NativePTree::clear(tjs_char const* path_exists) {
		boost::optional<pt_ptree_type&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_
			);
		if (!node) {
			return false;
		}
		node->clear();
		return true;
	}
	KTL_INLINE bool NativePTree::sort(tjs_char const* path_exists, tTJSVariant const* comp, bool stable) {
		boost::optional<pt_ptree_type&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_
			);
		if (!node) {
			return false;
		}
		if (comp && comp->Type() != tvtVoid) {
			switch (comp->Type()) {
			case tvtString:
				{
					tjs_char const* comp_op = sprig::krkr::tjs::as_c_str(comp->AsStringNoAddRef());
					if (sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("+")) == 0
						|| sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("a")) == 0
						)
					{
						if (stable) {
							sprig::stable_sort_ptree(*node);
						} else {
							node->sort(SortCompare<>());
						}
					} else if (sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("-")) == 0
						|| sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("z")) == 0
						)
					{
						if (stable) {
							sprig::stable_sort_ptree(*node, std::greater<pt_string_type>());
						} else {
							node->sort(SortCompare<std::greater<pt_string_type> >());
						}
					} else if (sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("ia")) == 0) {
						if (stable) {
							sprig::stable_sort_ptree(*node, sprig::predicate::lexicographical_compare());
						} else {
							node->sort(SortCompare<sprig::predicate::lexicographical_compare_t<> >());
						}
					} else if (sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("iz")) == 0) {
						if (stable) {
							sprig::stable_sort_ptree(*node, sprig::predicate::lexicographical_compare<sprig::predicate::is_igreater>());
						} else {
							node->sort(SortCompare<sprig::predicate::lexicographical_compare_t<sprig::predicate::is_igreater> >());
						}
					} else {
						return false;
					}
				}
				break;
			default:
				return false;
			}
		} else {
			if (stable) {
				sprig::stable_sort_ptree(*node);
			} else {
				node->sort(SortCompare<>());
			}
		}
		return true;
	}
	KTL_INLINE bool NativePTree::reverse(tjs_char const* path_exists) {
		boost::optional<pt_ptree_type&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_
			);
		if (!node) {
			return false;
		}
		node->reverse();
		return true;
	}
	//
	//	SUMMARY: 複製系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type NativePTree::clone(tjs_char const* path_exists) const {
		boost::optional<pt_ptree_type const&> node = getNodeExists(
			ptree_,
			path_exists,
			path_separator_,
			path_sharp_
			);
		if (!node) {
			return sprig::krkr::tjs::object_type();
		}
		sprig::krkr::tjs::object_type result = createNew(0, 0);
		PTree* ptr = reinterpret_cast<PTree*>(getInstance(sprig::get_pointer(result)));
		ptr->init(*node);
		return result;
	}
	KTL_INLINE bool NativePTree::assign(tjs_char const* path_insertable, iTJSDispatch2* obj) {
		boost::optional<pt_ptree_type&> node = getNodeInsertable(
			ptree_,
			path_insertable,
			path_separator_,
			path_sharp_
			);
		if (!node) {
			return false;
		}
		if (obj) {
			PTree* ptr = reinterpret_cast<PTree*>(getInstance(obj));
			*node = ptr->getValue();
		} else {
			boost::swap(*node, pt_ptree_type());
		}
		return true;
	}
	//
	//	SUMMARY: XML読み込み系メソッド
	//
	KTL_INLINE bool NativePTree::readXML(
		tjs_char const* source,
		flag_type flag
		)
	{
		std::basic_istringstream<tjs_char> is(source ? source : SPRIG_KRKR_TJS_W(""));
		pt_ptree_type ptree;
		boost::property_tree::read_xml(is, ptree, flag);
		boost::swap(ptree_, ptree);
		return true;
	}
	KTL_INLINE bool NativePTree::readXML(
		tTJSVariantOctet const* source,
		tjs_char const* code,
		flag_type flag
		)
	{
		pt_string_type string;
		if (!encodeAsString(string, source->GetData(), source->GetLength(), code)) {
			return false;
		}
		return readXML(string.c_str(), flag);
	}
	KTL_INLINE bool NativePTree::readStorageXML(
		tjs_char const* storage,
		flag_type flag
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		if (binary.empty()
			|| binary.size() % sizeof(tjs_char)
			|| reinterpret_cast<tjs_char const*>(&binary[0])[0] != L'\xFEFF'
			)
		{
			binary.insert(binary.end(), sizeof(tjs_nchar), 0);
			return readXML(
				sprig::str_cast<pt_string_type>(
					reinterpret_cast<tjs_nchar const*>(&binary[0])
					).c_str(),
				flag
				);
		} else {
			binary.insert(binary.end(), sizeof(tjs_char), 0);
			return readXML(reinterpret_cast<tjs_char const*>(&binary[0]), flag);
		}
		return true;
	}
	KTL_INLINE bool NativePTree::readStorageXML(
		tjs_char const* storage,
		tjs_char const* code,
		flag_type flag
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		pt_string_type string;
		if (!encodeAsString(string, binary.size() ? &binary[0] : 0, binary.size(), code)) {
			return false;
		}
		return readXML(string.c_str(), flag);
	}
	//
	//	SUMMARY: JSON読み込み系メソッド
	//
	KTL_INLINE bool NativePTree::readJSON(
		tjs_char const* source
		)
	{
		std::basic_istringstream<tjs_char> is(source ? source : SPRIG_KRKR_TJS_W(""));
		pt_ptree_type ptree;
		boost::property_tree::read_json(is, ptree);
		boost::swap(ptree_, ptree);
		return true;
	}
	KTL_INLINE bool NativePTree::readJSON(
		tTJSVariantOctet const* source,
		tjs_char const* code
		)
	{
		pt_string_type string;
		if (!encodeAsString(string, source->GetData(), source->GetLength(), code)) {
			return false;
		}
		return readJSON(string.c_str());
	}
	KTL_INLINE bool NativePTree::readStorageJSON(
		tjs_char const* storage
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		if (binary.empty()
			|| binary.size() % sizeof(tjs_char)
			|| reinterpret_cast<tjs_char const*>(&binary[0])[0] != L'\xFEFF'
			)
		{
			binary.insert(binary.end(), sizeof(tjs_nchar), 0);
			return readJSON(
				sprig::str_cast<pt_string_type>(
					reinterpret_cast<tjs_nchar const*>(&binary[0])
					).c_str()
				);
		} else {
			binary.insert(binary.end(), sizeof(tjs_char), 0);
			return readJSON(reinterpret_cast<tjs_char const*>(&binary[0]));
		}
		return true;
	}
	KTL_INLINE bool NativePTree::readStorageJSON(
		tjs_char const* storage,
		tjs_char const* code
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		pt_string_type string;
		if (!encodeAsString(string, binary.size() ? &binary[0] : 0, binary.size(), code)) {
			return false;
		}
		return readJSON(string.c_str());
	}
	//
	//	SUMMARY: INI読み込み系メソッド
	//
	KTL_INLINE bool NativePTree::readINI(
		tjs_char const* source
		)
	{
		std::basic_istringstream<tjs_char> is(source ? source : SPRIG_KRKR_TJS_W(""));
		pt_ptree_type ptree;
		boost::property_tree::read_ini(is, ptree);
		boost::swap(ptree_, ptree);
		return true;
	}
	KTL_INLINE bool NativePTree::readINI(
		tTJSVariantOctet const* source,
		tjs_char const* code
		)
	{
		pt_string_type string;
		if (!encodeAsString(string, source->GetData(), source->GetLength(), code)) {
			return false;
		}
		return readINI(string.c_str());
	}
	KTL_INLINE bool NativePTree::readStorageINI(
		tjs_char const* storage
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		if (binary.empty()
			|| binary.size() % sizeof(tjs_char)
			|| reinterpret_cast<tjs_char const*>(&binary[0])[0] != L'\xFEFF'
			)
		{
			binary.insert(binary.end(), sizeof(tjs_nchar), 0);
			return readINI(
				sprig::str_cast<pt_string_type>(
					reinterpret_cast<tjs_nchar const*>(&binary[0])
					).c_str()
				);
		} else {
			binary.insert(binary.end(), sizeof(tjs_char), 0);
			return readINI(reinterpret_cast<tjs_char const*>(&binary[0]));
		}
		return true;
	}
	KTL_INLINE bool NativePTree::readStorageINI(
		tjs_char const* storage,
		tjs_char const* code
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		pt_string_type string;
		if (!encodeAsString(string, binary.size() ? &binary[0] : 0, binary.size(), code)) {
			return false;
		}
		return readINI(string.c_str());
	}
	//
	//	SUMMARY: INFO読み込み系メソッド
	//
	KTL_INLINE bool NativePTree::readINFO(
		tjs_char const* source
		)
	{
		std::basic_istringstream<tjs_char> is(source ? source : SPRIG_KRKR_TJS_W(""));
		pt_ptree_type ptree;
		boost::property_tree::read_info(is, ptree);
		boost::swap(ptree_, ptree);
		return true;
	}
	KTL_INLINE bool NativePTree::readINFO(
		tTJSVariantOctet const* source,
		tjs_char const* code
		)
	{
		pt_string_type string;
		if (!encodeAsString(string, source->GetData(), source->GetLength(), code)) {
			return false;
		}
		return readINFO(string.c_str());
	}
	KTL_INLINE bool NativePTree::readStorageINFO(
		tjs_char const* storage
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		if (binary.empty()
			|| binary.size() % sizeof(tjs_char)
			|| reinterpret_cast<tjs_char const*>(&binary[0])[0] != L'\xFEFF'
			)
		{
			binary.insert(binary.end(), sizeof(tjs_nchar), 0);
			return readINFO(
				sprig::str_cast<pt_string_type>(
					reinterpret_cast<tjs_nchar const*>(&binary[0])
					).c_str()
				);
		} else {
			binary.insert(binary.end(), sizeof(tjs_char), 0);
			return readINFO(reinterpret_cast<tjs_char const*>(&binary[0]));
		}
		return true;
	}
	KTL_INLINE bool NativePTree::readStorageINFO(
		tjs_char const* storage,
		tjs_char const* code
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		pt_string_type string;
		if (!encodeAsString(string, binary.size() ? &binary[0] : 0, binary.size(), code)) {
			return false;
		}
		return readINFO(string.c_str());
	}
	//
	//	SUMMARY: YAML読み込み系メソッド
	//
	KTL_INLINE bool NativePTree::readYAML(
		tjs_char const* source,
		flag_type flag
		)
	{
		std::basic_istringstream<tjs_char> is(source ? source : SPRIG_KRKR_TJS_W(""));
		pt_ptree_type ptree;
		{
			//	COMMENT: コンバータ作成
			UErrorCode error = U_ZERO_ERROR;
			boost::shared_ptr<UConverter> converter(
				::ucnv_openU(L"UTF-8", &error),
				&::ucnv_close
				);
			if (U_FAILURE(error)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_openU: ")) + ::u_errorName(error),
					sprig::krkr::internal_error
					);
				return false;
			}
			sprig::property_tree::read_yaml(is, ptree, CodeStrConverter(converter.get()), flag);
		}
		boost::swap(ptree_, ptree);
		return true;
	}
	KTL_INLINE bool NativePTree::readYAML(
		tTJSVariantOctet const* source,
		tjs_char const* code,
		flag_type flag
		)
	{
		pt_string_type string;
		if (!encodeAsString(string, source->GetData(), source->GetLength(), code)) {
			return false;
		}
		return readYAML(string.c_str(), flag);
	}
	KTL_INLINE bool NativePTree::readStorageYAML(
		tjs_char const* storage,
		flag_type flag
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		if (binary.empty()
			|| binary.size() % sizeof(tjs_char)
			|| reinterpret_cast<tjs_char const*>(&binary[0])[0] != L'\xFEFF'
			)
		{
			binary.insert(binary.end(), sizeof(tjs_nchar), 0);
			return readYAML(
				sprig::str_cast<pt_string_type>(
					reinterpret_cast<tjs_nchar const*>(&binary[0])
					).c_str(),
				flag
				);
		} else {
			binary.insert(binary.end(), sizeof(tjs_char), 0);
			return readYAML(reinterpret_cast<tjs_char const*>(&binary[0]), flag);
		}
		return true;
	}
	KTL_INLINE bool NativePTree::readStorageYAML(
		tjs_char const* storage,
		tjs_char const* code,
		flag_type flag
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		pt_string_type string;
		if (!encodeAsString(string, binary.size() ? &binary[0] : 0, binary.size(), code)) {
			return false;
		}
		return readYAML(string.c_str(), flag);
	}
	//
	//	SUMMARY: HTML読み込み系メソッド
	//
	KTL_INLINE bool NativePTree::readHTML(
		tjs_char const* source,
		flag_type flag
		)
	{
		std::basic_istringstream<tjs_char> is(source ? source : SPRIG_KRKR_TJS_W(""));
		pt_ptree_type ptree;
		sprig::property_tree::read_html(is, ptree, sprig::property_tree::default_str_converter<pt_string_type>(), flag);
		boost::swap(ptree_, ptree);
		return true;
	}
	KTL_INLINE bool NativePTree::readHTML(
		tTJSVariantOctet const* source,
		tjs_char const* code,
		flag_type flag
		)
	{
		pt_string_type string;
		if (!encodeAsString(string, source->GetData(), source->GetLength(), code)) {
			return false;
		}
		return readHTML(string.c_str(), flag);
	}
	KTL_INLINE bool NativePTree::readStorageHTML(
		tjs_char const* storage,
		flag_type flag
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		if (binary.empty()
			|| binary.size() % sizeof(tjs_char)
			|| reinterpret_cast<tjs_char const*>(&binary[0])[0] != L'\xFEFF'
			)
		{
			binary.insert(binary.end(), sizeof(tjs_nchar), 0);
			return readHTML(
				sprig::str_cast<pt_string_type>(
					reinterpret_cast<tjs_nchar const*>(&binary[0])
					).c_str(),
				flag
				);
		} else {
			binary.insert(binary.end(), sizeof(tjs_char), 0);
			return readHTML(reinterpret_cast<tjs_char const*>(&binary[0]), flag);
		}
		return true;
	}
	KTL_INLINE bool NativePTree::readStorageHTML(
		tjs_char const* storage,
		tjs_char const* code,
		flag_type flag
		)
	{
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		pt_string_type string;
		if (!encodeAsString(string, binary.size() ? &binary[0] : 0, binary.size(), code)) {
			return false;
		}
		return readHTML(string.c_str(), flag);
	}
	//
	//	SUMMARY: XML書き出し系メソッド
	//
	KTL_INLINE tTJSVariant NativePTree::writeXMLToString(
		boost::optional<tjs_char> indent_char,
		boost::optional<size_type> indent_count
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		boost::property_tree::write_xml(
			os,
			ptree_,
			boost::property_tree::xml_writer_settings<tjs_char>(
				indent_char ? *indent_char : SPRIG_KRKR_TJS_W(' '),
				indent_count ? *indent_count : 4,
				SPRIG_KRKR_TJS_W("UTF-16")
				)
			);
		return tTJSVariant(os.str().c_str());
	}
	KTL_INLINE tTJSVariant NativePTree::writeXMLToOctet(
		tjs_char const* code,
		boost::optional<tjs_char> indent_char,
		boost::optional<size_type> indent_count
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		boost::property_tree::write_xml(
			os,
			ptree_,
			boost::property_tree::xml_writer_settings<tjs_char>(
				indent_char ? *indent_char : SPRIG_KRKR_TJS_W(' '),
				indent_count ? *indent_count : 4,
				code
				)
			);
		binary_type binary;
		if (!encodeAsBuffer(binary, code, os.str().c_str())) {
			return tTJSVariant();
		}
		return tTJSVariant(
			binary.size()
				? &binary[0]
				: 0
				,
			binary.size()
			);
	}
	KTL_INLINE NativePTree::int_type NativePTree::writeStorageXML(
		tjs_char const* storage,
		tjs_char const* code,
		boost::optional<tjs_char> indent_char,
		boost::optional<size_type> indent_count
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		if (!code) {
			os << SPRIG_KRKR_TJS_W('\xFEFF');
		}
		boost::property_tree::write_xml(
			os,
			ptree_,
			boost::property_tree::xml_writer_settings<tjs_char>(
				indent_char ? *indent_char : SPRIG_KRKR_TJS_W(' '),
				indent_count ? *indent_count : 4,
				code ? code : SPRIG_KRKR_TJS_W("UTF-16")
				)
			);
		if (code) {
			binary_type binary;
			if (!encodeAsBuffer(binary, code, os.str().c_str())) {
				return -1;
			}
			return saveStorage(
				storage,
				binary.size()
					? &binary[0]
					: 0
					,
				binary.size()
				);
		} else {
			pt_string_type string = os.str();
			return saveStorage(
				storage,
				string.size()
					? &string[0]
					: 0
					,
				string.size() * sizeof(tjs_char)
				);
		}
	}
	//
	//	SUMMARY: JSON書き出し系メソッド
	//
	KTL_INLINE tTJSVariant NativePTree::writeJSONToString(
		bool pretty
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		boost::property_tree::write_json(
			os,
			ptree_,
			pretty
			);
		return tTJSVariant(os.str().c_str());
	}
	KTL_INLINE tTJSVariant NativePTree::writeJSONToOctet(
		tjs_char const* code,
		bool pretty
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		boost::property_tree::write_json(
			os,
			ptree_,
			pretty
			);
		binary_type binary;
		if (!encodeAsBuffer(binary, code, os.str().c_str())) {
			return tTJSVariant();
		}
		return tTJSVariant(
			binary.size()
				? &binary[0]
				: 0
				,
			binary.size()
			);
	}
	KTL_INLINE NativePTree::int_type NativePTree::writeStorageJSON(
		tjs_char const* storage,
		tjs_char const* code,
		bool pretty
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		if (!code) {
			os << SPRIG_KRKR_TJS_W('\xFEFF');
		}
		boost::property_tree::write_json(
			os,
			ptree_,
			pretty
			);
		if (code) {
			binary_type binary;
			if (!encodeAsBuffer(binary, code, os.str().c_str())) {
				return -1;
			}
			return saveStorage(
				storage,
				binary.size()
					? &binary[0]
					: 0
					,
				binary.size()
				);
		} else {
			pt_string_type string = os.str();
			return saveStorage(
				storage,
				string.size()
					? &string[0]
					: 0
					,
				string.size() * sizeof(tjs_char)
				);
		}
	}
	//
	//	SUMMARY: INI書き出し系メソッド
	//
	KTL_INLINE tTJSVariant NativePTree::writeINIToString() const {
		std::basic_ostringstream<tjs_char> os;
		boost::property_tree::write_ini(
			os,
			ptree_
			);
		return tTJSVariant(os.str().c_str());
	}
	KTL_INLINE tTJSVariant NativePTree::writeINIToOctet(
		tjs_char const* code
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		boost::property_tree::write_ini(
			os,
			ptree_
			);
		binary_type binary;
		if (!encodeAsBuffer(binary, code, os.str().c_str())) {
			return tTJSVariant();
		}
		return tTJSVariant(
			binary.size()
				? &binary[0]
				: 0
				,
			binary.size()
			);
	}
	KTL_INLINE NativePTree::int_type NativePTree::writeStorageINI(
		tjs_char const* storage,
		tjs_char const* code
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		if (!code) {
			os << SPRIG_KRKR_TJS_W('\xFEFF');
		}
		boost::property_tree::write_ini(
			os,
			ptree_
			);
		if (code) {
			binary_type binary;
			if (!encodeAsBuffer(binary, code, os.str().c_str())) {
				return -1;
			}
			return saveStorage(
				storage,
				binary.size()
					? &binary[0]
					: 0
					,
				binary.size()
				);
		} else {
			pt_string_type string = os.str();
			return saveStorage(
				storage,
				string.size()
					? &string[0]
					: 0
					,
				string.size() * sizeof(tjs_char)
				);
		}
	}
	//
	//	SUMMARY: INFO書き出し系メソッド
	//
	KTL_INLINE tTJSVariant NativePTree::writeINFOToString(
		boost::optional<tjs_char> indent_char,
		boost::optional<size_type> indent_count
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		boost::property_tree::write_info(
			os,
			ptree_,
			boost::property_tree::info_writer_settings<tjs_char>(
				indent_char ? *indent_char : SPRIG_KRKR_TJS_W(' '),
				indent_count ? *indent_count : 4
				)
			);
		return tTJSVariant(os.str().c_str());
	}
	KTL_INLINE tTJSVariant NativePTree::writeINFOToOctet(
		tjs_char const* code,
		boost::optional<tjs_char> indent_char,
		boost::optional<size_type> indent_count
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		boost::property_tree::write_info(
			os,
			ptree_,
			boost::property_tree::info_writer_settings<tjs_char>(
				indent_char ? *indent_char : SPRIG_KRKR_TJS_W(' '),
				indent_count ? *indent_count : 4
				)
			);
		binary_type binary;
		if (!encodeAsBuffer(binary, code, os.str().c_str())) {
			return tTJSVariant();
		}
		return tTJSVariant(
			binary.size()
				? &binary[0]
				: 0
				,
			binary.size()
			);
	}
	KTL_INLINE NativePTree::int_type NativePTree::writeStorageINFO(
		tjs_char const* storage,
		tjs_char const* code,
		boost::optional<tjs_char> indent_char,
		boost::optional<size_type> indent_count
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		if (!code) {
			os << SPRIG_KRKR_TJS_W('\xFEFF');
		}
		boost::property_tree::write_info(
			os,
			ptree_,
			boost::property_tree::info_writer_settings<tjs_char>(
				indent_char ? *indent_char : SPRIG_KRKR_TJS_W(' '),
				indent_count ? *indent_count : 4
				)
			);
		if (code) {
			binary_type binary;
			if (!encodeAsBuffer(binary, code, os.str().c_str())) {
				return -1;
			}
			return saveStorage(
				storage,
				binary.size()
					? &binary[0]
					: 0
					,
				binary.size()
				);
		} else {
			pt_string_type string = os.str();
			return saveStorage(
				storage,
				string.size()
					? &string[0]
					: 0
					,
				string.size() * sizeof(tjs_char)
				);
		}
	}
	//
	//	SUMMARY: YAML書き出し系メソッド
	//
	KTL_INLINE tTJSVariant NativePTree::writeYAMLToString(
		flag_type flag
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		{
			//	COMMENT: コンバータ作成
			UErrorCode error = U_ZERO_ERROR;
			boost::shared_ptr<UConverter> converter(
				::ucnv_openU(L"UTF-8", &error),
				&::ucnv_close
				);
			if (U_FAILURE(error)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_openU: ")) + ::u_errorName(error),
					sprig::krkr::internal_error
					);
				return false;
			}
			sprig::property_tree::write_yaml(os, ptree_, CodeStrConverter(converter.get()), flag);
		}
		return tTJSVariant(os.str().c_str());
	}
	KTL_INLINE tTJSVariant NativePTree::writeYAMLToOctet(
		tjs_char const* code,
		flag_type flag
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		{
			//	COMMENT: コンバータ作成
			UErrorCode error = U_ZERO_ERROR;
			boost::shared_ptr<UConverter> converter(
				::ucnv_openU(L"UTF-8", &error),
				&::ucnv_close
				);
			if (U_FAILURE(error)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_openU: ")) + ::u_errorName(error),
					sprig::krkr::internal_error
					);
				return false;
			}
			sprig::property_tree::write_yaml(os, ptree_, CodeStrConverter(converter.get()), flag);
		}
		binary_type binary;
		if (!encodeAsBuffer(binary, code, os.str().c_str())) {
			return tTJSVariant();
		}
		return tTJSVariant(
			binary.size()
				? &binary[0]
				: 0
				,
			binary.size()
			);
	}
	KTL_INLINE NativePTree::int_type NativePTree::writeStorageYAML(
		tjs_char const* storage,
		tjs_char const* code,
		flag_type flag
		) const
	{
		std::basic_ostringstream<tjs_char> os;
		if (!code) {
			os << SPRIG_KRKR_TJS_W('\xFEFF');
		}
		{
			//	COMMENT: コンバータ作成
			UErrorCode error = U_ZERO_ERROR;
			boost::shared_ptr<UConverter> converter(
				::ucnv_openU(L"UTF-8", &error),
				&::ucnv_close
				);
			if (U_FAILURE(error)) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					tTJSString(SPRIG_KRKR_TJS_W("error in ucnv_openU: ")) + ::u_errorName(error),
					sprig::krkr::internal_error
					);
				return false;
			}
			sprig::property_tree::write_yaml(os, ptree_, CodeStrConverter(converter.get()), flag);
		}
		if (code) {
			binary_type binary;
			if (!encodeAsBuffer(binary, code, os.str().c_str())) {
				return -1;
			}
			return saveStorage(
				storage,
				binary.size()
					? &binary[0]
					: 0
					,
				binary.size()
				);
		} else {
			pt_string_type string = os.str();
			return saveStorage(
				storage,
				string.size()
					? &string[0]
					: 0
					,
				string.size() * sizeof(tjs_char)
				);
		}
	}
	//
	//	SUMMARY: フォーマット系メソッド
	//
	KTL_INLINE bool NativePTree::trimXML() {
		sprig::trim_xml(ptree_);
		return true;
	}
	KTL_INLINE bool NativePTree::stratifyINI(boost::optional<tjs_char> path_separator) {
		pt_ptree_type ptree(
			path_separator
				? sprig::format_from_ini(ptree_, *path_separator)
				: sprig::format_from_ini(ptree_, path_separator_)
			);
		boost::swap(ptree_, ptree_);
		return true;
	}
	KTL_INLINE bool NativePTree::unstratifyINI(boost::optional<tjs_char> path_separator) {
		pt_ptree_type ptree(
			path_separator
				? sprig::format_to_ini(ptree_, *path_separator)
				: sprig::format_to_ini(ptree_, path_separator_)
			);
		boost::swap(ptree_, ptree_);
		return true;
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tjs_char NativePTree::getPathSeparator() const {
		return path_separator_;
	}
	KTL_INLINE void NativePTree::setPathSeparator(tjs_char path_separator) {
		path_separator_ = path_separator;
	}
	KTL_INLINE tjs_char NativePTree::getPathSharp() const {
		return path_sharp_;
	}
	KTL_INLINE void NativePTree::setPathSharp(tjs_char path_sharp) {
		path_sharp_ = path_sharp;
	}

	//
	// PTree
	//
	PTree::PTree() {}
	tjs_error TJS_INTF_METHOD PTree::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("PTree::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativePTree>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD PTree::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("PTree::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	KTL_INLINE void PTree::init(NativePTree::pt_ptree_type const& ptree) {
		instance_->init(ptree);
	}
	KTL_INLINE NativePTree::pt_ptree_type const& PTree::getValue() const {
		return instance_->getValue();
	}
	//
	//	SUMMARY: 内部系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type PTree::getInstance() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(this);
	}
	//
	//	SUMMARY: アクセス系メソッド
	//
	KTL_INLINE tTJSVariant PTree::get(tTJSVariantString const* path_exists) const {
		return instance_->get(
			sprig::krkr::tjs::as_c_str(path_exists)
			);
	}
	KTL_INLINE tTJSVariant PTree::getKey(tTJSVariantString const* path_exists) const {
		return instance_->getKey(
			sprig::krkr::tjs::as_c_str(path_exists)
			);
	}
	KTL_INLINE bool PTree::put(tTJSVariantString const* path_insertable, tTJSVariantString const* data) {
		return instance_->put(
			sprig::krkr::tjs::as_c_str(path_insertable),
			sprig::krkr::tjs::as_c_str(data)
			);
	}
	KTL_INLINE bool PTree::erase(tTJSVariantString const* path_exists) {
		return instance_->erase(
			sprig::krkr::tjs::as_c_str(path_exists)
			);
	}
	KTL_INLINE tTVInteger PTree::erase(tTJSVariantString const* path_exists, tTJSVariantString const* key) {
		return instance_->erase(
			sprig::krkr::tjs::as_c_str(path_exists),
			sprig::krkr::tjs::as_c_str(key)
			);
	}
	KTL_INLINE bool PTree::exist(tTJSVariantString const* path_exists) const {
		return instance_->exist(
			sprig::krkr::tjs::as_c_str(path_exists)
			);
	}
	tTVInteger PTree::size(tTJSVariantString const* path_exists) const {
		return instance_->size(
			sprig::krkr::tjs::as_c_str(path_exists)
			);
	}
	KTL_INLINE tTVInteger PTree::count(tTJSVariantString const* path_exists, tTJSVariantString const* key) const {
		return instance_->count(
			sprig::krkr::tjs::as_c_str(path_exists),
			sprig::krkr::tjs::as_c_str(key)
			);
	}
	KTL_INLINE bool PTree::clear(tTJSVariantString const* path_exists) {
		return instance_->clear(
			sprig::krkr::tjs::as_c_str(path_exists)
			);
	}
	KTL_INLINE bool PTree::sort(tTJSVariantString const* path_exists, tTJSVariant const* comp, bool stable) {
		return instance_->sort(
			sprig::krkr::tjs::as_c_str(path_exists),
			comp,
			stable
			);
	}
	KTL_INLINE bool PTree::reverse(tTJSVariantString const* path_exists) {
		return instance_->reverse(
			sprig::krkr::tjs::as_c_str(path_exists)
			);
	}
	//
	//	SUMMARY: 複製系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type PTree::clone(tTJSVariantString const* path_exists) const {
		return instance_->clone(
			sprig::krkr::tjs::as_c_str(path_exists)
			);
	}
	KTL_INLINE bool PTree::assign(tTJSVariantString const* path_insertable, iTJSDispatch2* obj) {
		return instance_->assign(
			sprig::krkr::tjs::as_c_str(path_insertable),
			obj
			);
	}
	//
	//	SUMMARY: XML読み込み系メソッド
	//
	KTL_INLINE bool PTree::readXML(
		tTJSVariantString const* source,
		tTVInteger flag
		)
	{
		return instance_->readXML(
			sprig::krkr::tjs::as_c_str(source),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE bool PTree::readXML(
		tTJSVariantOctet const* source,
		tTJSVariantString const* code,
		tTVInteger flag
		)
	{
		return instance_->readXML(
			source,
			sprig::krkr::tjs::as_c_str(code),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE bool PTree::readStorageXML(
		tTJSVariantString const* storage,
		tTVInteger flag
		)
	{
		return instance_->readStorageXML(
			sprig::krkr::tjs::as_c_str(storage),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE bool PTree::readStorageXML(
		tTJSVariantString const* storage,
		tTJSVariantString const* code,
		tTVInteger flag
		)
	{
		return instance_->readStorageXML(
			sprig::krkr::tjs::as_c_str(storage),
			sprig::krkr::tjs::as_c_str(code),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	//
	//	SUMMARY: JSON読み込み系メソッド
	//
	KTL_INLINE bool PTree::readJSON(
		tTJSVariantString const* source
		)
	{
		return instance_->readJSON(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE bool PTree::readJSON(
		tTJSVariantOctet const* source,
		tTJSVariantString const* code
		)
	{
		return instance_->readJSON(
			source,
			sprig::krkr::tjs::as_c_str(code)
			);
	}
	KTL_INLINE bool PTree::readStorageJSON(
		tTJSVariantString const* storage
		)
	{
		return instance_->readStorageJSON(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE bool PTree::readStorageJSON(
		tTJSVariantString const* storage,
		tTJSVariantString const* code
		)
	{
		return instance_->readStorageJSON(
			sprig::krkr::tjs::as_c_str(storage),
			sprig::krkr::tjs::as_c_str(code)
			);
	}
	//
	//	SUMMARY: INI読み込み系メソッド
	//
	KTL_INLINE bool PTree::readINI(
		tTJSVariantString const* source
		)
	{
		return instance_->readINI(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE bool PTree::readINI(
		tTJSVariantOctet const* source,
		tTJSVariantString const* code
		)
	{
		return instance_->readINI(
			source,
			sprig::krkr::tjs::as_c_str(code)
			);
	}
	KTL_INLINE bool PTree::readStorageINI(
		tTJSVariantString const* storage
		)
	{
		return instance_->readStorageINI(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE bool PTree::readStorageINI(
		tTJSVariantString const* storage,
		tTJSVariantString const* code
		)
	{
		return instance_->readStorageINI(
			sprig::krkr::tjs::as_c_str(storage),
			sprig::krkr::tjs::as_c_str(code)
			);
	}
	//
	//	SUMMARY: INFO読み込み系メソッド
	//
	KTL_INLINE bool PTree::readINFO(
		tTJSVariantString const* source
		)
	{
		return instance_->readINFO(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE bool PTree::readINFO(
		tTJSVariantOctet const* source,
		tTJSVariantString const* code
		)
	{
		return instance_->readINFO(
			source,
			sprig::krkr::tjs::as_c_str(code)
			);
	}
	KTL_INLINE bool PTree::readStorageINFO(
		tTJSVariantString const* storage
		)
	{
		return instance_->readStorageINFO(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE bool PTree::readStorageINFO(
		tTJSVariantString const* storage,
		tTJSVariantString const* code
		)
	{
		return instance_->readStorageINFO(
			sprig::krkr::tjs::as_c_str(storage),
			sprig::krkr::tjs::as_c_str(code)
			);
	}
	//
	//	SUMMARY: YAML読み込み系メソッド
	//
	KTL_INLINE bool PTree::readYAML(
		tTJSVariantString const* source,
		tTVInteger flag
		)
	{
		return instance_->readYAML(
			sprig::krkr::tjs::as_c_str(source),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE bool PTree::readYAML(
		tTJSVariantOctet const* source,
		tTJSVariantString const* code,
		tTVInteger flag
		)
	{
		return instance_->readYAML(
			source,
			sprig::krkr::tjs::as_c_str(code),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE bool PTree::readStorageYAML(
		tTJSVariantString const* storage,
		tTVInteger flag
		)
	{
		return instance_->readStorageYAML(
			sprig::krkr::tjs::as_c_str(storage),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE bool PTree::readStorageYAML(
		tTJSVariantString const* storage,
		tTJSVariantString const* code,
		tTVInteger flag
		)
	{
		return instance_->readStorageYAML(
			sprig::krkr::tjs::as_c_str(storage),
			sprig::krkr::tjs::as_c_str(code),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	//
	//	SUMMARY: HTML読み込み系メソッド
	//
	KTL_INLINE bool PTree::readHTML(
		tTJSVariantString const* source,
		tTVInteger flag
		)
	{
		return instance_->readHTML(
			sprig::krkr::tjs::as_c_str(source),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE bool PTree::readHTML(
		tTJSVariantOctet const* source,
		tTJSVariantString const* code,
		tTVInteger flag
		)
	{
		return instance_->readHTML(
			source,
			sprig::krkr::tjs::as_c_str(code),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE bool PTree::readStorageHTML(
		tTJSVariantString const* storage,
		tTVInteger flag
		)
	{
		return instance_->readStorageHTML(
			sprig::krkr::tjs::as_c_str(storage),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE bool PTree::readStorageHTML(
		tTJSVariantString const* storage,
		tTJSVariantString const* code,
		tTVInteger flag
		)
	{
		return instance_->readStorageHTML(
			sprig::krkr::tjs::as_c_str(storage),
			sprig::krkr::tjs::as_c_str(code),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	//
	//	SUMMARY: XML書き出し系メソッド
	//
	KTL_INLINE tTJSVariant PTree::writeXMLToString(
		boost::optional<tTVInteger> indent_char,
		boost::optional<tTVInteger> indent_count
		) const
	{
		return instance_->writeXMLToString(
			indent_char
				? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*indent_char))
				: boost::none
				,
			indent_count
				? boost::optional<NativePTree::size_type>(boost::numeric_cast<NativePTree::size_type>(*indent_count))
				: boost::none
			);
	}
	KTL_INLINE tTJSVariant PTree::writeXMLToOctet(
		tTJSVariantString const* code,
		boost::optional<tTVInteger> indent_char,
		boost::optional<tTVInteger> indent_count
		) const
	{
		return instance_->writeXMLToOctet(
			sprig::krkr::tjs::as_c_str(code),
			indent_char
				? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*indent_char))
				: boost::none
				,
			indent_count
				? boost::optional<NativePTree::size_type>(boost::numeric_cast<NativePTree::size_type>(*indent_count))
				: boost::none
			);
	}
	KTL_INLINE tTVInteger PTree::writeStorageXML(
		tTJSVariantString const* storage,
		tTJSVariantString const* code,
		boost::optional<tTVInteger> indent_char,
		boost::optional<tTVInteger> indent_count
		) const
	{
		return instance_->writeStorageXML(
			sprig::krkr::tjs::as_c_str(storage),
			code
				? sprig::krkr::tjs::as_c_str(code)
				: 0
				,
			indent_char
				? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*indent_char))
				: boost::none
				,
			indent_count
				? boost::optional<NativePTree::size_type>(boost::numeric_cast<NativePTree::size_type>(*indent_count))
				: boost::none
			);
	}
	//
	//	SUMMARY: JSON書き出し系メソッド
	//
	KTL_INLINE tTJSVariant PTree::writeJSONToString(
		bool pretty
		) const
	{
		return instance_->writeJSONToString(pretty);
	}
	KTL_INLINE tTJSVariant PTree::writeJSONToOctet(
		tTJSVariantString const* code,
		bool pretty
		) const
	{
		return instance_->writeJSONToOctet(
			sprig::krkr::tjs::as_c_str(code),
			pretty
			);
	}
	KTL_INLINE tTVInteger PTree::writeStorageJSON(
		tTJSVariantString const* storage,
		tTJSVariantString const* code,
		bool pretty
		) const
	{
		return instance_->writeStorageJSON(
			sprig::krkr::tjs::as_c_str(storage),
			code
				? sprig::krkr::tjs::as_c_str(code)
				: 0
				,
			pretty
			);
	}
	//
	//	SUMMARY: INI書き出し系メソッド
	//
	KTL_INLINE tTJSVariant PTree::writeINIToString() const {
		return instance_->writeINIToString();
	}
	KTL_INLINE tTJSVariant PTree::writeINIToOctet(
		tTJSVariantString const* code
		) const
	{
		return instance_->writeINIToOctet(
			sprig::krkr::tjs::as_c_str(code)
			);
	}
	KTL_INLINE tTVInteger PTree::writeStorageINI(
		tTJSVariantString const* storage,
		tTJSVariantString const* code
		) const
	{
		return instance_->writeStorageINI(
			sprig::krkr::tjs::as_c_str(storage),
			code
				? sprig::krkr::tjs::as_c_str(code)
				: 0
			);
	}
	//
	//	SUMMARY: INFO書き出し系メソッド
	//
	KTL_INLINE tTJSVariant PTree::writeINFOToString(
		boost::optional<tTVInteger> indent_char,
		boost::optional<tTVInteger> indent_count
		) const
	{
		return instance_->writeINFOToString(
			indent_char
				? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*indent_char))
				: boost::none
				,
			indent_count
				? boost::optional<NativePTree::size_type>(boost::numeric_cast<NativePTree::size_type>(*indent_count))
				: boost::none
			);
	}
	KTL_INLINE tTJSVariant PTree::writeINFOToOctet(
		tTJSVariantString const* code,
		boost::optional<tTVInteger> indent_char,
		boost::optional<tTVInteger> indent_count
		) const
	{
		return instance_->writeINFOToOctet(
			sprig::krkr::tjs::as_c_str(code),
			indent_char
				? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*indent_char))
				: boost::none
				,
			indent_count
				? boost::optional<NativePTree::size_type>(boost::numeric_cast<NativePTree::size_type>(*indent_count))
				: boost::none
			);
	}
	KTL_INLINE tTVInteger PTree::writeStorageINFO(
		tTJSVariantString const* storage,
		tTJSVariantString const* code,
		boost::optional<tTVInteger> indent_char,
		boost::optional<tTVInteger> indent_count
		) const
	{
		return instance_->writeStorageINFO(
			sprig::krkr::tjs::as_c_str(storage),
			code
				? sprig::krkr::tjs::as_c_str(code)
				: 0
				,
			indent_char
				? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*indent_char))
				: boost::none
				,
			indent_count
				? boost::optional<NativePTree::size_type>(boost::numeric_cast<NativePTree::size_type>(*indent_count))
				: boost::none
			);
	}
	//
	//	SUMMARY: YAML書き出し系メソッド
	//
	KTL_INLINE tTJSVariant PTree::writeYAMLToString(
		tTVInteger flag
		) const
	{
		return instance_->writeYAMLToString(
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE tTJSVariant PTree::writeYAMLToOctet(
		tTJSVariantString const* code,
		tTVInteger flag
		) const
	{
		return instance_->writeYAMLToOctet(
			sprig::krkr::tjs::as_c_str(code),
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	KTL_INLINE tTVInteger PTree::writeStorageYAML(
		tTJSVariantString const* storage,
		tTJSVariantString const* code,
		tTVInteger flag
		) const
	{
		return instance_->writeStorageYAML(
			sprig::krkr::tjs::as_c_str(storage),
			code
				? sprig::krkr::tjs::as_c_str(code)
				: 0
				,
			sprig::numeric::bit_cast<NativePTree::flag_type>(flag)
			);
	}
	//
	//	SUMMARY: フォーマット系メソッド
	//
	KTL_INLINE bool PTree::trimXML() {
		return instance_->trimXML();
	}
	KTL_INLINE bool PTree::stratifyINI(boost::optional<tTVInteger> path_separator) {
		return instance_->stratifyINI(
			path_separator
				? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*path_separator))
				: boost::none
			);
	}
	KTL_INLINE bool PTree::unstratifyINI(boost::optional<tTVInteger> path_separator) {
		return instance_->unstratifyINI(
			path_separator
				? boost::optional<tjs_char>(boost::numeric_cast<tjs_char>(*path_separator))
				: boost::none
			);
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tTVInteger PTree::getPathSeparator() const {
		return instance_->getPathSeparator();
	}
	KTL_INLINE void PTree::setPathSeparator(tTVInteger path_separator) {
		instance_->setPathSeparator(
			static_cast<tjs_char>(path_separator)
			);
	}
	KTL_INLINE tTVInteger PTree::getPathSharp() const {
		return instance_->getPathSeparator();
	}
	KTL_INLINE void PTree::setPathSharp(tTVInteger path_sharp) {
		instance_->setPathSharp(
			static_cast<tjs_char>(path_sharp)
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
