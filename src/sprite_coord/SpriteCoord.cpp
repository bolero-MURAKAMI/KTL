/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "SpriteCoord.hpp"

namespace ktl {
	//
	// CreateNativeClassSpriteCoord
	//
#define TJS_NATIVE_CLASSID_NAME SpriteCoordClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassSpriteCoord, SpriteCoord, SpriteCoord)
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveCoordTransByText)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_RESULT_SET(this_->saveCoordTransByText());
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveCoordTransByText);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadCoordTransByText)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->loadCoordTransByText(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadCoordTransByText);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveCoordTransByXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_RESULT_SET(this_->saveCoordTransByXML());
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveCoordTransByXML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadCoordTransByXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->loadCoordTransByXML(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadCoordTransByXML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveCoordTransByBinary)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_RESULT_SET(this_->saveCoordTransByBinary());
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveCoordTransByBinary);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadCoordTransByBinary)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		this_->loadCoordTransByBinary(SPRIG_KRKR_ARG_OCTET(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadCoordTransByBinary);
	//
	//	SUMMARY: 更新系メソッド
	//
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(SpriteCoord, update);
	//
	//	SUMMARY: パラメータ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setScaling)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		this_->setScaling(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setScaling);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setRotation)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		this_->setRotation(SPRIG_KRKR_ARG_VARIANT(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setRotation);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setTranslation)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		this_->setTranslation(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setTranslation);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setRect)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		this_->setRect(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setRect);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setCenter)
	{
		TJS_GET_NATIVE_INSTANCE(this_, SpriteCoord);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		this_->setCenter(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setCenter);
	//
	//	SUMMARY: パラメータプロパテイ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, scalingX, getScalingX, setScalingX);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, scalingY, getScalingY, setScalingY);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, rotation, getRotation, setRotation);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, translationX, getTranslationX, setTranslationX);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, translationY, getTranslationY, setTranslationY);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, rectLeft, getRectLeft, setRectLeft);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, rectRight, getRectRight, setRectRight);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, rectTop, getRectTop, setRectTop);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, rectBottom, getRectBottom, setRectBottom);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, centerX, getCenterX, setCenterX);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(SpriteCoord, centerY, getCenterY, setCenterY);
	//
	//	SUMMARY: 取得プロパテイ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, sizeX, getSizeX);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, sizeY, getSizeY);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, points0X, getPoints0X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, points0Y, getPoints0Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, points1X, getPoints1X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, points1Y, getPoints1Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, points2X, getPoints2X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, points2Y, getPoints2Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, points3X, getPoints3X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, points3Y, getPoints3Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, transformedPoints0X, getTransformedPoints0X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, transformedPoints0Y, getTransformedPoints0Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, transformedPoints1X, getTransformedPoints1X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, transformedPoints1Y, getTransformedPoints1Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, transformedPoints2X, getTransformedPoints2X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, transformedPoints2Y, getTransformedPoints2Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, transformedPoints3X, getTransformedPoints3X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, transformedPoints3Y, getTransformedPoints3Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedPoints0X, getEnclosedPoints0X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedPoints0Y, getEnclosedPoints0Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedPoints1X, getEnclosedPoints1X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedPoints1Y, getEnclosedPoints1Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedPoints2X, getEnclosedPoints2X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedPoints2Y, getEnclosedPoints2Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedPoints3X, getEnclosedPoints3X);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedPoints3Y, getEnclosedPoints3Y);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedRectLeft, getEnclosedRectLeft);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedRectRight, getEnclosedRectRight);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedRectTop, getEnclosedRectTop);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedRectBottom, getEnclosedRectBottom);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedSizeX, getEnclosedSizeX);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(SpriteCoord, enclosedSizeY, getEnclosedSizeY);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_sprite_coord
// unregist_ktl_sprite_coord
//
static void regist_ktl_sprite_coord() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:SpriteCoord:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddNewMember(global.get(), SPRIG_KRKR_TJS_W("SpriteCoord"), ktl::CreateNativeClassSpriteCoord());
	}
}
static void unregist_ktl_sprite_coord() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:SpriteCoord:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("SpriteCoord"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_sprite_coord);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_sprite_coord);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:SpriteCoord:information"));
