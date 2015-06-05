/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "Config.hpp"

#include <boost/optional/optional.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "PTree.hpp"

namespace ktl {
	//
	// CreateNativeClassPTree
	//
#define TJS_NATIVE_CLASSID_NAME PTreeClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassPTree, PTree, PTree)
	//
	//	SUMMARY: XMLパース系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfXMLNoConcatText, PTree::pfXMLNoConcatText);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfXMLNoComments, PTree::pfXMLNoComments);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfXMLTrimWhitespace, PTree::pfXMLTrimWhitespace);
	//
	//	SUMMARY: YAMLパース系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfYAMLMultiDocument, PTree::pfYAMLMultiDocument);
	//
	//	SUMMARY: YAMLエミット系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(efYAMLMultiDocument, PTree::efYAMLMultiDocument);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(efYAMLInsertEndOfDocument, PTree::efYAMLInsertEndOfDocument);
	//
	//	SUMMARY: HTMLパース系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfHTMLNoConcatText, PTree::pfHTMLNoConcatText);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfHTMLNoComments, PTree::pfHTMLNoComments);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfHTMLTrimWhitespace, PTree::pfHTMLTrimWhitespace);
	//
	//
	//	SUMMARY: 内部系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(PTree, instance, getInstance);
	//
	//	SUMMARY: アクセス系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(get)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			this_->get(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(get);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getKey)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			this_->getKey(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getKey);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(put)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->put(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(put);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(erase)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->erase(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->erase(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(erase);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(exist)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			this_->exist(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(exist);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(size)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			this_->size(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(size);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(count)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		SPRIG_KRKR_RESULT_SET(
			this_->count(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(count);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(clear)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->clear(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(clear);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(sort)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->sort(
				SPRIG_KRKR_ARG_STRING(0),
				SPRIG_KRKR_ARG_DEFAULT(1, 0),
				SPRIG_KRKR_ARG_BOOL_DEFAULT(2, false)
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(sort);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(reverse)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->reverse(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(reverse);
	//
	//	SUMMARY: 複製系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(clone)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (result) {
			sprig::krkr::tjs::object_type obj = this_->clone(
				SPRIG_KRKR_ARG_STRING(0)
				);
			*result = sprig::krkr::tjs::as_object_closure_variant(obj);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(clone);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(assign)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INSTANCEOF_OR_NULL_CHECK(1, PTree);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->assign(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_OBJECT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(assign);
	//
	//	SUMMARY: XML読み込み系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(1);
		switch (SPRIG_KRKR_ARG_TYPE(0)) {
		case tvtString:
			if (numparams >= 2) {
				SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readXML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
					);
			} else {
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readXML(SPRIG_KRKR_ARG_STRING(0))
					);
			}
			break;
		case tvtOctet:
			SPRIG_KRKR_NUMPARAMS_EASY_CHECK(2);
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			if (numparams >= 3) {
				SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readXML(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
					);
			} else {
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readXML(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_STRING(1))
					);
			}
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readXML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readStorageXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageXML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			switch (SPRIG_KRKR_ARG_TYPE(1)) {
			case tvtString:
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readStorageXML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
					);
				break;
			case tvtInteger:
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readStorageXML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
					);
				break;
			default:
				return TJS_E_INVALIDPARAM;
			}
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageXML(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readStorageXML);
	//
	//	SUMMARY: JSON読み込み系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readJSON)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		switch (SPRIG_KRKR_ARG_TYPE(0)) {
		case tvtString:
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readJSON(SPRIG_KRKR_ARG_STRING(0))
				);
			break;
		case tvtOctet:
			SPRIG_KRKR_NUMPARAMS_CHECK(2);
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readJSON(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_STRING(1))
				);
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readJSON);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readStorageJSON)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageJSON(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageJSON(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readStorageJSON);
	//
	//	SUMMARY: INI読み込み系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readINI)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		switch (SPRIG_KRKR_ARG_TYPE(0)) {
		case tvtString:
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readINI(SPRIG_KRKR_ARG_STRING(0))
				);
			break;
		case tvtOctet:
			SPRIG_KRKR_NUMPARAMS_CHECK(2);
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readINI(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_STRING(1))
				);
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readINI);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readStorageINI)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageINI(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageINI(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readStorageINI);
	//
	//	SUMMARY: INFO読み込み系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readINFO)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		switch (SPRIG_KRKR_ARG_TYPE(0)) {
		case tvtString:
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readINFO(SPRIG_KRKR_ARG_STRING(0))
				);
			break;
		case tvtOctet:
			SPRIG_KRKR_NUMPARAMS_CHECK(2);
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readINFO(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_STRING(1))
				);
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readINFO);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readStorageINFO)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageINFO(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageINFO(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readStorageINFO);
	//
	//	SUMMARY: YAML読み込み系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readYAML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(1);
		switch (SPRIG_KRKR_ARG_TYPE(0)) {
		case tvtString:
			if (numparams >= 2) {
				SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readYAML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
					);
			} else {
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readYAML(SPRIG_KRKR_ARG_STRING(0))
					);
			}
			break;
		case tvtOctet:
			SPRIG_KRKR_NUMPARAMS_EASY_CHECK(2);
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			if (numparams >= 3) {
				SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readYAML(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
					);
			} else {
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readYAML(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_STRING(1))
					);
			}
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readYAML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readStorageYAML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageYAML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			switch (SPRIG_KRKR_ARG_TYPE(1)) {
			case tvtString:
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readStorageYAML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
					);
				break;
			case tvtInteger:
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readStorageYAML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
					);
				break;
			default:
				return TJS_E_INVALIDPARAM;
			}
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageYAML(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readStorageYAML);
	//
	//	SUMMARY: HTML読み込み系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readHTML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(1);
		switch (SPRIG_KRKR_ARG_TYPE(0)) {
		case tvtString:
			if (numparams >= 2) {
				SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readHTML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
					);
			} else {
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readHTML(SPRIG_KRKR_ARG_STRING(0))
					);
			}
			break;
		case tvtOctet:
			SPRIG_KRKR_NUMPARAMS_EASY_CHECK(2);
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			if (numparams >= 3) {
				SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readHTML(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
					);
			} else {
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readHTML(SPRIG_KRKR_ARG_OCTET(0), SPRIG_KRKR_ARG_STRING(1))
					);
			}
			break;
		default:
			return TJS_E_INVALIDPARAM;
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readHTML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readStorageHTML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
			SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageHTML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		} else if (numparams >= 2) {
			switch (SPRIG_KRKR_ARG_TYPE(1)) {
			case tvtString:
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readStorageHTML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
					);
				break;
			case tvtInteger:
				SPRIG_KRKR_INVOKE_RESULT_SET(
					this_->readStorageHTML(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
					);
				break;
			default:
				return TJS_E_INVALIDPARAM;
			}
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->readStorageHTML(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readStorageHTML);
	//
	//	SUMMARY: XML書き出し系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeXMLToString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_RESULT_SET(
			this_->writeXMLToString(
				SPRIG_KRKR_ENABLE_ARG(0)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(0))
					: boost::none
					,
				SPRIG_KRKR_ENABLE_ARG(1)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(1))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeXMLToString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeXMLToOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			this_->writeXMLToOctet(
				SPRIG_KRKR_ARG_STRING(0),
				SPRIG_KRKR_ENABLE_ARG(1)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(1))
					: boost::none
					,
				SPRIG_KRKR_ENABLE_ARG(2)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(2))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeXMLToOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeStorageXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		}
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->writeStorageXML(
				SPRIG_KRKR_ARG_STRING(0),
				SPRIG_KRKR_ENABLE_ARG(1)
					? SPRIG_KRKR_ARG_STRING(1)
					: 0
					,
				SPRIG_KRKR_ENABLE_ARG(2)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(2))
					: boost::none
					,
				SPRIG_KRKR_ENABLE_ARG(3)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(3))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeStorageXML);
	//
	//	SUMMARY: JSON書き出し系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeJSONToString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_RESULT_SET(
			this_->writeJSONToString(
				SPRIG_KRKR_ARG_BOOL_DEFAULT(0, true)
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeJSONToString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeJSONToOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			this_->writeJSONToOctet(
				SPRIG_KRKR_ARG_STRING(0),
				SPRIG_KRKR_ARG_BOOL_DEFAULT(1, true)
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeJSONToOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeStorageJSON)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_OR_VOID_CHECK(1, tvtString);
		}
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->writeStorageJSON(
				SPRIG_KRKR_ARG_STRING(0),
				SPRIG_KRKR_ENABLE_ARG(1)
					? SPRIG_KRKR_ARG_STRING(1)
					: 0
					,
				SPRIG_KRKR_ARG_BOOL_DEFAULT(2, true)
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeStorageJSON);
	//
	//	SUMMARY: INI書き出し系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeINIToString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_RESULT_SET(
			this_->writeINIToString()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeINIToString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeINIToOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			this_->writeINIToOctet(
				SPRIG_KRKR_ARG_STRING(0)
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeINIToOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeStorageINI)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_OR_VOID_CHECK(1, tvtString);
		}
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->writeStorageINI(
				SPRIG_KRKR_ARG_STRING(0),
				SPRIG_KRKR_ENABLE_ARG(1)
					? SPRIG_KRKR_ARG_STRING(1)
					: 0
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeStorageINI);
	//
	//	SUMMARY: INFO書き出し系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeINFOToString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_RESULT_SET(
			this_->writeINFOToString(
				SPRIG_KRKR_ENABLE_ARG(0)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(0))
					: boost::none
					,
				SPRIG_KRKR_ENABLE_ARG(1)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(1))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeINFOToString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeINFOToOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			this_->writeINFOToOctet(
				SPRIG_KRKR_ARG_STRING(0),
				SPRIG_KRKR_ENABLE_ARG(1)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(1))
					: boost::none
					,
				SPRIG_KRKR_ENABLE_ARG(2)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(2))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeINFOToOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeStorageINFO)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		}
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->writeStorageINFO(
				SPRIG_KRKR_ARG_STRING(0),
				SPRIG_KRKR_ENABLE_ARG(1)
					? SPRIG_KRKR_ARG_STRING(1)
					: 0
					,
				SPRIG_KRKR_ENABLE_ARG(2)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(2))
					: boost::none
					,
				SPRIG_KRKR_ENABLE_ARG(3)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(3))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeStorageINFO);
	//
	//	SUMMARY: YAML書き出し系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeYAMLToString)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		if (numparams >= 1) {
			SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
			SPRIG_KRKR_RESULT_SET(
				this_->writeYAMLToString(SPRIG_KRKR_ARG_VARIANT(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->writeYAMLToString()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeYAMLToString);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeYAMLToOctet)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
			SPRIG_KRKR_RESULT_SET(
				this_->writeYAMLToOctet(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				this_->writeYAMLToOctet(SPRIG_KRKR_ARG_STRING(0))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeYAMLToOctet);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(writeStorageYAML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		if (numparams >= 2) {
			SPRIG_KRKR_TYPE_OR_VOID_CHECK(1, tvtString);
		}
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_CHECK(2, tvtInteger);
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeStorageYAML(
					SPRIG_KRKR_ARG_STRING(0),
					SPRIG_KRKR_ARG_TYPE(1) != tvtVoid
						? SPRIG_KRKR_ARG_STRING(1)
						: 0
						,
					SPRIG_KRKR_ARG_VARIANT(2)
					)
				);
		} else if (numparams >= 2) {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeStorageYAML(
					SPRIG_KRKR_ARG_STRING(0),
					SPRIG_KRKR_ARG_TYPE(1) != tvtVoid
						? SPRIG_KRKR_ARG_STRING(1)
						: 0
					)
				);
		} else {
			SPRIG_KRKR_INVOKE_RESULT_SET(
				this_->writeStorageYAML(
					SPRIG_KRKR_ARG_STRING(0)
					)
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(writeStorageYAML);
	//
	//	SUMMARY: フォーマット系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(trimXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->trimXML()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(trimXML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(stratifyINI)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->stratifyINI(
				SPRIG_KRKR_ENABLE_ARG(0)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(0))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(stratifyINI);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(unstratifyINI)
	{
		TJS_GET_NATIVE_INSTANCE(this_, PTree);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			this_->unstratifyINI(
				SPRIG_KRKR_ENABLE_ARG(0)
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(0))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(unstratifyINI);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(PTree, pathSeparator, getPathSeparator, setPathSeparator);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(PTree, pathSharp, getPathSharp, setPathSharp);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_ptree
// unregist_ktl_ptree
//
static void regist_ktl_ptree() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:PTree:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddNewMember(global, SPRIG_KRKR_TJS_W("PTree"), ktl::CreateNativeClassPTree());
	}
}
static void unregist_ktl_ptree() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:PTree:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global, SPRIG_KRKR_TJS_W("PTree"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_ptree);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_ptree);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:PTree:information"));
