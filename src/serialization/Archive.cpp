
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "Archive.hpp"

namespace ktl {
	//
	// CreateNativeClassArchive
	//
#define TJS_NATIVE_CLASSID_NAME ArchiveClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassArchive, Archive, Archive)
	//
	//	SUMMARY: 内部系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Archive, stack, getStack);
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveText)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_RESULT_SET(
			this_->saveText()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveText);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadText)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->loadText(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadText);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_RESULT_SET(
			this_->saveXML()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveXML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->loadXML(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadXML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveBinary)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_RESULT_SET(
			this_->saveBinary()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveBinary);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadBinary)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		this_->loadBinary(SPRIG_KRKR_ARG_OCTET(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadBinary);
	//
	//	SUMMARY: Ptシリアライズ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(savePtXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		if (numparams >= 3) {
			SPRIG_KRKR_TYPE_OR_VOID_CHECK(2, tvtString);
		}
		SPRIG_KRKR_RESULT_SET(
			this_->savePtXML(
				numparams >= 1 && SPRIG_KRKR_ARG_TYPE(0) != tvtVoid
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(0))
					: boost::none
					,
				numparams >= 2 && SPRIG_KRKR_ARG_TYPE(1) != tvtVoid
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(1))
					: boost::none
					,
				numparams >= 3 && SPRIG_KRKR_ARG_TYPE(2) != tvtVoid
					? SPRIG_KRKR_ARG_STRING(2)
					: 0
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(savePtXML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadPtXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->loadPtXML(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadPtXML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(savePtJSON)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_RESULT_SET(
			this_->savePtJSON()
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(savePtJSON);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadPtJSON)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->loadPtJSON(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadPtJSON);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(savePtINFO)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_RESULT_SET(
			this_->savePtINFO(
				numparams >= 1 && SPRIG_KRKR_ARG_TYPE(0) != tvtVoid
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(0))
					: boost::none
					,
				numparams >= 2 && SPRIG_KRKR_ARG_TYPE(1) != tvtVoid
					? boost::optional<tTVInteger>(SPRIG_KRKR_ARG_VARIANT(1))
					: boost::none
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(savePtINFO);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadPtINFO)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Archive);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->loadPtINFO(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadPtINFO);
	//
	//	SUMMARY: 操作系メソッド
	//
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(Archive, clear);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Archive, size, size);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_archive
// unregist_ktl_archive
//
static void regist_ktl_archive() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Archive:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Archive"), ktl::CreateNativeClassArchive());
	}
}
static void unregist_ktl_archive() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Archive:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Archive"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_archive);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_archive);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Archive:information"));
