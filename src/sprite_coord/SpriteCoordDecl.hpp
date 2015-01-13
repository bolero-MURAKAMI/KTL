/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_SPRITE_COORD_SPRITE_COORD_DECL_HPP
#define SRC_SPRITE_COORD_SPRITE_COORD_DECL_HPP

#include <vector>
#include <string>
#include <memory>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/call_traits.hpp>

#include "SpriteCoordCommon.hpp"

namespace ktl {
	//
	// NativeSpriteCoord
	//
	class NativeSpriteCoord {
	public:
		typedef CoordTrans coord_trans_type;
		typedef boost::scoped_ptr<coord_trans_type> coord_trans_instance_type;
		typedef coord_trans_type::coord_element_type coord_element_type;
		typedef coord_trans_type::coord_type coord_type;
		typedef coord_trans_type::trans_element_type trans_element_type;
		typedef coord_trans_type::scaling_type scaling_type;
		typedef coord_trans_type::rotation_type rotation_type;
		typedef coord_trans_type::translation_type translation_type;
		typedef coord_trans_type::size_type size_type;
		typedef coord_trans_type::rect_type rect_type;
		typedef coord_trans_type::coords_type coords_type;
		typedef std::basic_string<tjs_char> serialization_string_type;
		typedef std::auto_ptr<serialization_string_type> string_instance_type;
		typedef std::vector<char> binary_type;
		typedef std::auto_ptr<binary_type> binary_instance_type;
	private:
		coord_trans_instance_type coord_trans_;
	public:
		NativeSpriteCoord();
	public:
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		string_instance_type saveCoordTransByText() const;
		void loadCoordTransByText(tjs_char const* data);
		string_instance_type saveCoordTransByXML() const;
		void loadCoordTransByXML(tjs_char const* data);
		binary_instance_type saveCoordTransByBinary() const;
		void loadCoordTransByBinary(tTJSVariantOctet const* data);
		//
		//	SUMMARY: 更新系メソッド
		//
		void update();
		//
		//	SUMMARY: パラメータ系メソッド
		//
		sprig::call_traits<scaling_type>::getter_type getScaling() const;
		void setScaling(sprig::call_traits<scaling_type>::param_type v);
		sprig::call_traits<rotation_type>::getter_type getRotation() const;
		void setRotation(sprig::call_traits<rotation_type>::param_type v);
		sprig::call_traits<translation_type>::getter_type getTranslation() const;
		void setTranslation(sprig::call_traits<translation_type>::param_type v);
		sprig::call_traits<rect_type>::getter_type getRect() const;
		void setRect(sprig::call_traits<rect_type>::param_type v);
		sprig::call_traits<coord_type>::getter_type getCenter() const;
		void setCenter(sprig::call_traits<coord_type>::param_type v);
		//
		//	SUMMARY: パラメータプロパテイ系メソッド
		//
		trans_element_type getScalingX() const;
		trans_element_type getScalingY() const;
		void setScalingX(sprig::call_traits<trans_element_type>::param_type v);
		void setScalingY(sprig::call_traits<trans_element_type>::param_type v);
		trans_element_type getTranslationX() const;
		trans_element_type getTranslationY() const;
		void setTranslationX(sprig::call_traits<trans_element_type>::param_type v);
		void setTranslationY(sprig::call_traits<trans_element_type>::param_type v);
		coord_element_type getRectLeft() const;
		coord_element_type getRectRight() const;
		coord_element_type getRectTop() const;
		coord_element_type getRectBottom() const;
		void setRectLeft(sprig::call_traits<coord_element_type>::param_type v);
		void setRectRight(sprig::call_traits<coord_element_type>::param_type v);
		void setRectTop(sprig::call_traits<coord_element_type>::param_type v);
		void setRectBottom(sprig::call_traits<coord_element_type>::param_type v);
		coord_element_type getCenterX() const;
		coord_element_type getCenterY() const;
		void setCenterX(sprig::call_traits<coord_element_type>::param_type v);
		void setCenterY(sprig::call_traits<coord_element_type>::param_type v);
		//
		//	SUMMARY: 取得系メソッド
		//
		sprig::call_traits<size_type>::getter_type getSize() const;
		sprig::call_traits<coords_type>::getter_type getPoints() const;
		sprig::call_traits<coords_type>::getter_type getTransformedPoints() const;
		sprig::call_traits<coords_type>::getter_type getEnclosedPoints() const;
		sprig::call_traits<rect_type>::getter_type getEnclosedRect() const;
		sprig::call_traits<size_type>::getter_type getEnclosedSize() const;
		//
		//	SUMMARY: 取得プロパテイ系メソッド
		//
		coord_element_type getSizeX() const;
		coord_element_type getSizeY() const;
		coord_element_type getPoints0X() const;
		coord_element_type getPoints0Y() const;
		coord_element_type getPoints1X() const;
		coord_element_type getPoints1Y() const;
		coord_element_type getPoints2X() const;
		coord_element_type getPoints2Y() const;
		coord_element_type getPoints3X() const;
		coord_element_type getPoints3Y() const;
		coord_element_type getTransformedPoints0X() const;
		coord_element_type getTransformedPoints0Y() const;
		coord_element_type getTransformedPoints1X() const;
		coord_element_type getTransformedPoints1Y() const;
		coord_element_type getTransformedPoints2X() const;
		coord_element_type getTransformedPoints2Y() const;
		coord_element_type getTransformedPoints3X() const;
		coord_element_type getTransformedPoints3Y() const;
		coord_element_type getEnclosedPoints0X() const;
		coord_element_type getEnclosedPoints0Y() const;
		coord_element_type getEnclosedPoints1X() const;
		coord_element_type getEnclosedPoints1Y() const;
		coord_element_type getEnclosedPoints2X() const;
		coord_element_type getEnclosedPoints2Y() const;
		coord_element_type getEnclosedPoints3X() const;
		coord_element_type getEnclosedPoints3Y() const;
		coord_element_type getEnclosedRectLeft() const;
		coord_element_type getEnclosedRectRight() const;
		coord_element_type getEnclosedRectTop() const;
		coord_element_type getEnclosedRectBottom() const;
		coord_element_type getEnclosedSizeX() const;
		coord_element_type getEnclosedSizeY() const;
	};

	//
	// SpriteCoord
	//
	class SpriteCoord
		: public tTJSNativeInstance
	{
	public:
		typedef NativeSpriteCoord::coord_element_type coord_element_type;
		typedef NativeSpriteCoord::coord_type coord_type;
		typedef NativeSpriteCoord::trans_element_type trans_element_type;
		typedef NativeSpriteCoord::scaling_type scaling_type;
		typedef NativeSpriteCoord::rotation_type rotation_type;
		typedef NativeSpriteCoord::translation_type translation_type;
		typedef NativeSpriteCoord::size_type size_type;
		typedef NativeSpriteCoord::rect_type rect_type;
		typedef NativeSpriteCoord::coords_type coords_type;
	private:
		boost::shared_ptr<NativeSpriteCoord> instance_;
	public:
		SpriteCoord();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		tTJSVariant saveCoordTransByText() const;
		void loadCoordTransByText(tTJSVariantString const* data);
		tTJSVariant saveCoordTransByXML() const;
		void loadCoordTransByXML(tTJSVariantString const* data);
		tTJSVariant saveCoordTransByBinary() const;
		void loadCoordTransByBinary(tTJSVariantOctet const* data);
		//
		//	SUMMARY: 更新系メソッド
		//
		void update();
		//
		//	SUMMARY: パラメータ系メソッド
		//
		void setScaling(tTVReal v1, tTVReal v2);
		void setRotation(tTVReal v);
		tTVReal getRotation() const;
		void setTranslation(tTVReal v1, tTVReal v2);
		void setRect(tTVInteger v1, tTVInteger v2, tTVInteger v3, tTVInteger v4);
		void setCenter(tTVInteger v1, tTVInteger v2);
		//
		//	SUMMARY: パラメータプロパテイ系メソッド
		//
		tTVReal getScalingX() const;
		tTVReal getScalingY() const;
		void setScalingX(tTVReal v);
		void setScalingY(tTVReal v);
		tTVReal getTranslationX() const;
		tTVReal getTranslationY() const;
		void setTranslationX(tTVReal v);
		void setTranslationY(tTVReal v);
		tTVInteger getRectLeft() const;
		tTVInteger getRectRight() const;
		tTVInteger getRectTop() const;
		tTVInteger getRectBottom() const;
		void setRectLeft(tTVInteger v);
		void setRectRight(tTVInteger v);
		void setRectTop(tTVInteger v);
		void setRectBottom(tTVInteger v);
		tTVInteger getCenterX() const;
		tTVInteger getCenterY() const;
		void setCenterX(tTVInteger v);
		void setCenterY(tTVInteger v);
		//
		//	SUMMARY: 取得プロパテイ系メソッド
		//
		tTVInteger getSizeX() const;
		tTVInteger getSizeY() const;
		tTVInteger getPoints0X() const;
		tTVInteger getPoints0Y() const;
		tTVInteger getPoints1X() const;
		tTVInteger getPoints1Y() const;
		tTVInteger getPoints2X() const;
		tTVInteger getPoints2Y() const;
		tTVInteger getPoints3X() const;
		tTVInteger getPoints3Y() const;
		tTVInteger getTransformedPoints0X() const;
		tTVInteger getTransformedPoints0Y() const;
		tTVInteger getTransformedPoints1X() const;
		tTVInteger getTransformedPoints1Y() const;
		tTVInteger getTransformedPoints2X() const;
		tTVInteger getTransformedPoints2Y() const;
		tTVInteger getTransformedPoints3X() const;
		tTVInteger getTransformedPoints3Y() const;
		tTVInteger getEnclosedPoints0X() const;
		tTVInteger getEnclosedPoints0Y() const;
		tTVInteger getEnclosedPoints1X() const;
		tTVInteger getEnclosedPoints1Y() const;
		tTVInteger getEnclosedPoints2X() const;
		tTVInteger getEnclosedPoints2Y() const;
		tTVInteger getEnclosedPoints3X() const;
		tTVInteger getEnclosedPoints3Y() const;
		tTVInteger getEnclosedRectLeft() const;
		tTVInteger getEnclosedRectRight() const;
		tTVInteger getEnclosedRectTop() const;
		tTVInteger getEnclosedRectBottom() const;
		tTVInteger getEnclosedSizeX() const;
		tTVInteger getEnclosedSizeY() const;
	};
}	// namespace ktl

#endif	// #ifndef SRC_SPRITE_COORD_SPRITE_COORD_DECL_HPP
