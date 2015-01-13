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

#include "Sprites.hpp"

//
// KTL_SPRITES_NATIVE_GETTER_METHOD_DECL
// KTL_SPRITES_NATIVE_SETTER_METHOD_DECL
//
#define KTL_SPRITES_NATIVE_GETTER_METHOD_DECL(GETTER_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(GETTER_NAME) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, Sprites); \
		SPRIG_KRKR_NUMPARAMS_CHECK(1); \
		SPRIG_KRKR_OPTIONAL_RESULT_SET(this_->GETTER_NAME(SPRIG_KRKR_ARG_VARIANT(0))); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(GETTER_NAME);
#define KTL_SPRITES_NATIVE_SETTER_METHOD_DECL(SETTER_NAME) \
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(SETTER_NAME) \
	{ \
		TJS_GET_NATIVE_INSTANCE(this_, Sprites); \
		SPRIG_KRKR_NUMPARAMS_CHECK(2); \
		this_->SETTER_NAME(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1)); \
		return TJS_S_OK; \
	} \
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(SETTER_NAME);
//
// KTL_SPRITES_NATIVE_PROP_METHOD_DECL
//
#define KTL_SPRITES_NATIVE_PROP_METHOD_DECL(GETTER_NAME, SETTER_NAME) \
	KTL_SPRITES_NATIVE_GETTER_METHOD_DECL(GETTER_NAME) \
	KTL_SPRITES_NATIVE_SETTER_METHOD_DECL(SETTER_NAME)

namespace ktl {
	//
	// CreateNativeClassSprites
	//
#define TJS_NATIVE_CLASSID_NAME SpritesClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassSprites, Sprites, Sprites)
	//
	//	SUMMARY: ブレンド系プロパティ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(bmDefault, Sprites::bmDefault);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(bmAlpha, Sprites::bmAlpha);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(bmAddAlpha, Sprites::bmAddAlpha);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(bmAdditive, Sprites::bmAdditive);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(bmSubtractive, Sprites::bmSubtractive);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(bmMultiplicative, Sprites::bmMultiplicative);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(bmLighten, Sprites::bmLighten);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(bmDarken, Sprites::bmDarken);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(bmScreen, Sprites::bmScreen);
	//
	//	SUMMARY: ヒットテスト系プロパティ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(htmNone, Sprites::htmNone);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(htmRect, Sprites::htmRect);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(htmColor, Sprites::htmColor);
	//
	//	SUMMARY: 初期化系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(initialize)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(this_->initialize(SPRIG_KRKR_ARG(0)));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(initialize);
	SPRIG_KRKR_NATIVE_METHOD_DECL_INVOKE_PARAM_VOID_RES_VARIANT(Sprites, reinitialize);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT_GETONLY(Sprites, isInitialized, isInitialized);
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveSpriteBufferByText)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_RESULT_SET(this_->saveSpriteBufferByText());
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveSpriteBufferByText);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadSpriteBufferByText)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->loadSpriteBufferByText(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadSpriteBufferByText);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveSpriteBufferByXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_RESULT_SET(this_->saveSpriteBufferByXML());
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveSpriteBufferByXML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadSpriteBufferByXML)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		this_->loadSpriteBufferByXML(SPRIG_KRKR_ARG_STRING(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadSpriteBufferByXML);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(saveSpriteBufferByBinary)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_RESULT_SET(this_->saveSpriteBufferByBinary());
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(saveSpriteBufferByBinary);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadSpriteBufferByBinary)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtOctet);
		this_->loadSpriteBufferByBinary(SPRIG_KRKR_ARG_OCTET(0));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadSpriteBufferByBinary);
	//
	//	SUMMARY: スプライトドローワ管理系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(generateSpriteDrawer)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		SPRIG_KRKR_INVOKE_RESULT_SET(this_->generateSpriteDrawer(
			SPRIG_KRKR_ARG_VARIANT(0),
			SPRIG_KRKR_ARG_VARIANT(1),
			SPRIG_KRKR_ARG(2)
			));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(generateSpriteDrawer);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VARIANT_RES_VOID(Sprites, erase);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(Sprites, clear);
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VARIANT(Sprites, size);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(exist)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_RESULT_SET(this_->exist(SPRIG_KRKR_ARG_VARIANT(0)));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(exist);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getPriority, setPriority);
	//
	//	SUMMARY: スプライトドローワ操作系メソッド
	//
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getVisible, setVisible);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setScaling)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		this_->setScaling(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setScaling);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getScalingX, setScalingX);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getScalingY, setScalingY);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getRotation, setRotation);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setTranslation)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		this_->setTranslation(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setTranslation);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getTranslationX, setTranslationX);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getTranslationY, setTranslationY);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setRect)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(5);
		this_->setRect(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3), SPRIG_KRKR_ARG_VARIANT(4));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setRect);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getRectLeft, setRectLeft);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getRectTop, setRectTop);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getRectRight, setRectRight);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getRectBottom, setRectBottom);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setCenter)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		this_->setCenter(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setCenter);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getCenterX, setCenterX);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getCenterY, setCenterY);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getCenterZ, setCenterZ);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setPosition)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		this_->setPosition(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setPosition);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getPositionX, setPositionX);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getPositionY, setPositionY);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getPositionZ, setPositionZ);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getColor, setColor);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getBlendMode, setBlendMode);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getHitThreshold, setHitThreshold);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getHitTestMode, setHitTestMode);
	//
	//	SUMMARY: 拡張スプライトドローワ操作系メソッド
	//
	KTL_SPRITES_NATIVE_SETTER_METHOD_DECL(setScalingXY);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setScalingSize)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		this_->setScalingSize(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setScalingSize);
	KTL_SPRITES_NATIVE_SETTER_METHOD_DECL(setScalingSizeX);
	KTL_SPRITES_NATIVE_SETTER_METHOD_DECL(setScalingSizeY);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setRectSquare)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(5);
		this_->setRectSquare(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3), SPRIG_KRKR_ARG_VARIANT(4));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setRectSquare);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setRectPoint)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		this_->setRectPoint(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setRectPoint);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getRectPointX, setRectPointX);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getRectPointY, setRectPointY);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setRectSize)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		this_->setRectSize(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setRectSize);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getRectSizeX, setRectSizeX);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getRectSizeY, setRectSizeY);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setRectSizeFromCenter)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		this_->setRectSizeFromCenter(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setRectSizeFromCenter);
	KTL_SPRITES_NATIVE_SETTER_METHOD_DECL(setRectSizeXFromCenter);
	KTL_SPRITES_NATIVE_SETTER_METHOD_DECL(setRectSizeYFromCenter);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setRectRegionSize)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		this_->setRectRegionSize(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setRectRegionSize);
	KTL_SPRITES_NATIVE_SETTER_METHOD_DECL(setRectRegionSizeX);
	KTL_SPRITES_NATIVE_SETTER_METHOD_DECL(setRectRegionSizeY);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setCenterByRate)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		this_->setCenterByRate(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setCenterByRate);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getCenterXByRate, setCenterXByRate);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getCenterYByRate, setCenterYByRate);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getCenterZByRate, setCenterZByRate);
	KTL_SPRITES_NATIVE_SETTER_METHOD_DECL(setCenterXYZByRate);
	KTL_SPRITES_NATIVE_PROP_METHOD_DECL(getRotationByDegree, setRotationByDegree)
	//
	//	SUMMARY: 更新系メソッド
	//
	SPRIG_KRKR_NATIVE_METHOD_DECL_INVOKE_PARAM_VOID_RES_VARIANT(Sprites, onContinuous)
	SPRIG_KRKR_NATIVE_METHOD_DECL_INVOKE_PARAM_VOID_RES_VARIANT(Sprites, update)
	SPRIG_KRKR_NATIVE_METHOD_DECL_INVOKE_PARAM_VOID_RES_VARIANT(Sprites, requestUpdate)
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VARIANT(Sprites, isRequestedUpdate)
	//
	//	SUMMARY: 描画系メソッド
	//
	SPRIG_KRKR_NATIVE_METHOD_DECL_INVOKE_PARAM_VOID_RES_VARIANT(Sprites, draw)
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(loadImageFromLayer)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(this_->loadImageFromLayer(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG(1)));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(loadImageFromLayer);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(clearImage)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_INVOKE_RESULT_SET(this_->clearImage(SPRIG_KRKR_ARG_VARIANT(0)));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(clearImage);
	//
	//	SUMMARY: ヒットテスト系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(hitTest)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		if (result) {
			boost::shared_ptr<tTJSString> value = this_->hitTest(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
			if (value) {
				*result = *value;
			} else {
				*result = tTJSVariant();
			}
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(hitTest);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(multiTargetHitTest)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		if (result) {
			sprig::krkr::tjs::object_type obj = this_->multiTargetHitTest(SPRIG_KRKR_ARG_VARIANT(0), SPRIG_KRKR_ARG_VARIANT(1));
			*result = tTJSVariant(obj.get(), obj.get());
			obj->AddRef();
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(multiTargetHitTest);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(multiPointHitTest)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		if (result) {
			sprig::krkr::tjs::object_type obj = this_->multiPointHitTest(numparams, param);
			*result = tTJSVariant(obj.get(), obj.get());
			obj->AddRef();
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(multiPointHitTest);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(multiTPHitTest)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		if (result) {
			sprig::krkr::tjs::object_type obj = this_->multiTPHitTest(numparams, param);
			*result = tTJSVariant(obj.get(), obj.get());
			obj->AddRef();
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(multiTPHitTest);
	//
	//	SUMMARY: 座標系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(toLocalCoord)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		if (result) {
			sprig::krkr::tjs::object_type obj = this_->toLocalCoord(
				SPRIG_KRKR_ARG_VARIANT(0),
				SPRIG_KRKR_ARG_VARIANT(1),
				SPRIG_KRKR_ARG_VARIANT(2)
				);
			*result = tTJSVariant(obj.get(), obj.get());
			obj->AddRef();
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(toLocalCoord);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(toGlobalCoord)
	{
		TJS_GET_NATIVE_INSTANCE(this_, Sprites);
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		if (result) {
			sprig::krkr::tjs::object_type obj = this_->toGlobalCoord(
				SPRIG_KRKR_ARG_VARIANT(0),
				SPRIG_KRKR_ARG_VARIANT(1),
				SPRIG_KRKR_ARG_VARIANT(2)
				);
			*result = tTJSVariant(obj.get(), obj.get());
			obj->AddRef();
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(toGlobalCoord);
	//
	//	SUMMARY: プロパティ系メソッド
	//
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(Sprites, clearColor, getClearColor, setClearColor);
	SPRIG_KRKR_NATIVE_PROP_DECL_TJSCLASS(Sprites, layer, getLayer, setLayer);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(Sprites, enable, getEnable, setEnable);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(Sprites, run, getRun, setRun);
	//
	//	SUMMARY: デバッグ系メソッド
	//
	SPRIG_KRKR_NATIVE_METHOD_DECL_PARAM_VOID_RES_VOID(Sprites, outputValueInfo);
	SPRIG_KRKR_NATIVE_PROP_DECL_VARIANT(Sprites, enablePerformanceTimer, getEnablePerformanceTimer, setEnablePerformanceTimer);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// push_ktl_outputter_to_dg_logger
//
static void push_ktl_outputter_to_dg_logger() {
	//
	//	COMMENT: sprig::dgのロガーにsprig::krkr::tjsへの出力を追加
	//
	struct pusher {
		int operator()() const {
			sprig::dg::indent_logger().push(sprig::krkr::tjs::outputter());
			return 0;
		}
	};
	static int const pushed = pusher()();
}
NCB_PRE_REGIST_CALLBACK(push_ktl_outputter_to_dg_logger);

//
// regist_ktl_sprites
// unregist_ktl_sprites
//
static void regist_ktl_sprites() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Sprites:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("Sprites"), ktl::CreateNativeClassSprites());
	}
}
static void unregist_ktl_sprites() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Sprites:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("Sprites"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_sprites);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_sprites);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Sprites:information"));
