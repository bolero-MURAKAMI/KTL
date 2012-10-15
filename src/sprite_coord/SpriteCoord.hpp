#pragma once

#include <boost/numeric/conversion/cast.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/scoped_ptr.hpp>
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
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "SpriteCoordCommon.hpp"
#include "SpriteCoordDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:SpriteCoord:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:SpriteCoord:error")

namespace ktl {
	//
	// NativeSpriteCoord
	//
	NativeSpriteCoord::NativeSpriteCoord()
		: coord_trans_(new coord_trans_type())
	{}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	KTL_INLINE NativeSpriteCoord::string_instance_type NativeSpriteCoord::saveCoordTransByText() const {
		std::basic_ostringstream<tjs_char> os;
		{
			boost::archive::text_woarchive oa(os);
			oa << boost::serialization::make_nvp("coord_trans_", coord_trans_);
		}
		string_instance_type result(new serialization_string_type(os.str()));
		return result;
	}
	KTL_INLINE void NativeSpriteCoord::loadCoordTransByText(tjs_char const* data) {
		std::basic_istringstream<tjs_char> is(data ? data : SPRIG_KRKR_TJS_W(""));
		{
			boost::archive::text_wiarchive ia(is);
			ia >> boost::serialization::make_nvp("coord_trans_", coord_trans_);
		}
	}
	KTL_INLINE NativeSpriteCoord::string_instance_type NativeSpriteCoord::saveCoordTransByXML() const {
		std::basic_ostringstream<tjs_char> os;
		{
			boost::archive::xml_woarchive oa(os);
			oa << boost::serialization::make_nvp("coord_trans_", coord_trans_);
		}
		string_instance_type result(new serialization_string_type(os.str()));
		return result;
	}
	KTL_INLINE void NativeSpriteCoord::loadCoordTransByXML(tjs_char const* data) {
		std::basic_istringstream<tjs_char> is(data ? data : SPRIG_KRKR_TJS_W(""));
		{
			boost::archive::xml_wiarchive ia(is);
			ia >> boost::serialization::make_nvp("coord_trans_", coord_trans_);
		}
	}
	KTL_INLINE NativeSpriteCoord::binary_instance_type NativeSpriteCoord::saveCoordTransByBinary() const {
		typedef boost::iostreams::back_insert_device<binary_type> device_type;
		typedef boost::iostreams::stream<device_type> stream_type;

		binary_instance_type result(new binary_type());
		device_type dev(*result);
		stream_type os(dev, std::ios::out | std::ios::binary);
		{
			boost::archive::binary_oarchive oa(os);
			oa << boost::serialization::make_nvp("coord_trans_", coord_trans_);
		}
		return result;
	}
	KTL_INLINE void NativeSpriteCoord::loadCoordTransByBinary(tTJSVariantOctet const* data) {
		typedef boost::iostreams::basic_array_source<binary_type::value_type> device_type;
		typedef boost::iostreams::stream<device_type> stream_type;

		device_type dev(
			reinterpret_cast<binary_type::value_type const*>(sprig::krkr::tjs::octet_data(data)),
			sprig::krkr::tjs::octet_length(data)
			);
		stream_type is(dev, std::ios::in | std::ios::binary);
		{
			boost::archive::binary_iarchive ia(is);
			ia >> boost::serialization::make_nvp("coord_trans_", coord_trans_);
		}
	}
	//
	//	SUMMARY: 更新系メソッド
	//
	KTL_INLINE void NativeSpriteCoord::update() {
		coord_trans_->update();
	}
	//
	//	SUMMARY: パラメータ系メソッド
	//
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::scaling_type>::getter_type NativeSpriteCoord::getScaling() const {
		return coord_trans_->refScaling();
	}
	KTL_INLINE void NativeSpriteCoord::setScaling(sprig::call_traits<scaling_type>::param_type v) {
		coord_trans_->refScaling() = v;
	}
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::rotation_type>::getter_type NativeSpriteCoord::getRotation() const {
		return coord_trans_->refRotation();
	}
	KTL_INLINE void NativeSpriteCoord::setRotation(sprig::call_traits<rotation_type>::param_type v) {
		coord_trans_->refRotation() = v;
	}
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::translation_type>::getter_type NativeSpriteCoord::getTranslation() const {
		return coord_trans_->refTranslation();
	}
	KTL_INLINE void NativeSpriteCoord::setTranslation(sprig::call_traits<translation_type>::param_type v) {
		coord_trans_->refTranslation() = v;
	}
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::rect_type>::getter_type NativeSpriteCoord::getRect() const {
		return coord_trans_->refRect();
	}
	KTL_INLINE void NativeSpriteCoord::setRect(sprig::call_traits<rect_type>::param_type v) {
		coord_trans_->refRect() = v;
	}
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::coord_type>::getter_type NativeSpriteCoord::getCenter() const {
		return coord_trans_->refCenter();
	}
	KTL_INLINE void NativeSpriteCoord::setCenter(sprig::call_traits<coord_type>::param_type v) {
		coord_trans_->refCenter() = v;
	}
	//
	//	SUMMARY: パラメータプロパテイ系メソッド
	//
	KTL_INLINE NativeSpriteCoord::trans_element_type NativeSpriteCoord::getScalingX() const {
		return coord_trans_->refScaling().get<0>();
	}
	KTL_INLINE NativeSpriteCoord::trans_element_type NativeSpriteCoord::getScalingY() const {
		return coord_trans_->refScaling().get<1>();
	}
	KTL_INLINE void NativeSpriteCoord::setScalingX(sprig::call_traits<trans_element_type>::param_type v) {
		coord_trans_->refScaling().get<0>() = v;
	}
	KTL_INLINE void NativeSpriteCoord::setScalingY(sprig::call_traits<trans_element_type>::param_type v) {
		coord_trans_->refScaling().get<1>() = v;
	}
	KTL_INLINE NativeSpriteCoord::trans_element_type NativeSpriteCoord::getTranslationX() const {
		return coord_trans_->refTranslation().get<0>();
	}
	KTL_INLINE NativeSpriteCoord::trans_element_type NativeSpriteCoord::getTranslationY() const {
		return coord_trans_->refTranslation().get<1>();
	}
	KTL_INLINE void NativeSpriteCoord::setTranslationX(sprig::call_traits<trans_element_type>::param_type v) {
		coord_trans_->refTranslation().get<0>() = v;
	}
	KTL_INLINE void NativeSpriteCoord::setTranslationY(sprig::call_traits<trans_element_type>::param_type v) {
		coord_trans_->refTranslation().get<1>() = v;
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getRectLeft() const {
		return coord_trans_->refRect()[0].get<0>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getRectRight() const {
		return coord_trans_->refRect()[1].get<0>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getRectTop() const {
		return coord_trans_->refRect()[0].get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getRectBottom() const {
		return coord_trans_->refRect()[1].get<1>();
	}
	KTL_INLINE void NativeSpriteCoord::setRectLeft(sprig::call_traits<coord_element_type>::param_type v) {
		coord_trans_->refRect()[0].get<0>() = v;
	}
	KTL_INLINE void NativeSpriteCoord::setRectRight(sprig::call_traits<coord_element_type>::param_type v) {
		coord_trans_->refRect()[1].get<0>() = v;
	}
	KTL_INLINE void NativeSpriteCoord::setRectTop(sprig::call_traits<coord_element_type>::param_type v) {
		coord_trans_->refRect()[0].get<1>() = v;
	}
	KTL_INLINE void NativeSpriteCoord::setRectBottom(sprig::call_traits<coord_element_type>::param_type v) {
		coord_trans_->refRect()[1].get<1>() = v;
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getCenterX() const {
		return coord_trans_->refCenter().get<0>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getCenterY() const {
		return coord_trans_->refCenter().get<1>();
	}
	KTL_INLINE void NativeSpriteCoord::setCenterX(sprig::call_traits<coord_element_type>::param_type v) {
		coord_trans_->refCenter().get<0>() = v;
	}
	KTL_INLINE void NativeSpriteCoord::setCenterY(sprig::call_traits<coord_element_type>::param_type v) {
		coord_trans_->refCenter().get<1>() = v;
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::size_type>::getter_type NativeSpriteCoord::getSize() const {
		return coord_trans_->getSize();
	}
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::coords_type>::getter_type NativeSpriteCoord::getPoints() const {
		return coord_trans_->getPoints();
	}
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::coords_type>::getter_type NativeSpriteCoord::getTransformedPoints() const {
		return coord_trans_->getTransformedPoints();
	}
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::coords_type>::getter_type NativeSpriteCoord::getEnclosedPoints() const {
		return coord_trans_->getEnclosedPoints();
	}
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::rect_type>::getter_type NativeSpriteCoord::getEnclosedRect() const {
		return coord_trans_->getEnclosedRect();
	}
	KTL_INLINE sprig::call_traits<NativeSpriteCoord::size_type>::getter_type NativeSpriteCoord::getEnclosedSize() const {
		return coord_trans_->getEnclosedSize();
	}
	//
	//	SUMMARY: 取得プロパテイ系メソッド
	//
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getSizeX() const {
		return coord_trans_->getSize().get<0>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getSizeY() const {
		return coord_trans_->getSize().get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getPoints0X() const {
		return coord_trans_->getPoints()[0].get<0>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getPoints0Y() const {
		return coord_trans_->getPoints()[0].get<1>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getPoints1X() const {
		return coord_trans_->getPoints()[1].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getPoints1Y() const {
		return coord_trans_->getPoints()[1].get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getPoints2X() const {
		return coord_trans_->getPoints()[2].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getPoints2Y() const {
		return coord_trans_->getPoints()[2].get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getPoints3X() const {
		return coord_trans_->getPoints()[3].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getPoints3Y() const {
		return coord_trans_->getPoints()[3].get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getTransformedPoints0X() const {
		return coord_trans_->getTransformedPoints()[0].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getTransformedPoints0Y() const {
		return coord_trans_->getTransformedPoints()[0].get<1>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getTransformedPoints1X() const {
		return coord_trans_->getTransformedPoints()[1].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getTransformedPoints1Y() const {
		return coord_trans_->getTransformedPoints()[1].get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getTransformedPoints2X() const {
		return coord_trans_->getTransformedPoints()[2].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getTransformedPoints2Y() const {
		return coord_trans_->getTransformedPoints()[2].get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getTransformedPoints3X() const {
		return coord_trans_->getTransformedPoints()[3].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getTransformedPoints3Y() const {
		return coord_trans_->getTransformedPoints()[3].get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedPoints0X() const {
		return coord_trans_->getEnclosedPoints()[0].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedPoints0Y() const {
		return coord_trans_->getEnclosedPoints()[0].get<1>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedPoints1X() const {
		return coord_trans_->getEnclosedPoints()[1].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedPoints1Y() const {
		return coord_trans_->getEnclosedPoints()[1].get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedPoints2X() const {
		return coord_trans_->getEnclosedPoints()[2].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedPoints2Y() const {
		return coord_trans_->getEnclosedPoints()[2].get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedPoints3X() const {
		return coord_trans_->getEnclosedPoints()[3].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedPoints3Y() const {
		return coord_trans_->getEnclosedPoints()[3].get<1>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedRectLeft() const {
		return coord_trans_->getEnclosedRect()[0].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedRectRight() const {
		return coord_trans_->getEnclosedRect()[1].get<0>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedRectTop() const {
		return coord_trans_->getEnclosedRect()[0].get<1>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedRectBottom() const {
		return coord_trans_->getEnclosedRect()[1].get<1>();
	};
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedSizeX() const {
		return coord_trans_->getEnclosedSize().get<0>();
	}
	KTL_INLINE NativeSpriteCoord::coord_element_type NativeSpriteCoord::getEnclosedSizeY() const {
		return coord_trans_->getEnclosedSize().get<1>();
	}

	//
	// SpriteCoord
	//
	SpriteCoord::SpriteCoord() {}
	tjs_error TJS_INTF_METHOD SpriteCoord::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("SpriteCoord::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeSpriteCoord>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD SpriteCoord::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("SpriteCoord::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	KTL_INLINE tTJSVariant SpriteCoord::saveCoordTransByText() const {
		return tTJSVariant(instance_->saveCoordTransByText()->c_str());
	}
	KTL_INLINE void SpriteCoord::loadCoordTransByText(tTJSVariantString const* data) {
		instance_->loadCoordTransByText(sprig::krkr::tjs::as_c_str(data));
	}
	KTL_INLINE tTJSVariant SpriteCoord::saveCoordTransByXML() const {
		return tTJSVariant(instance_->saveCoordTransByXML()->c_str());
	}
	KTL_INLINE void SpriteCoord::loadCoordTransByXML(tTJSVariantString const* data) {
		instance_->loadCoordTransByXML(sprig::krkr::tjs::as_c_str(data));
	}
	KTL_INLINE tTJSVariant SpriteCoord::saveCoordTransByBinary() const {
		NativeSpriteCoord::binary_instance_type data(instance_->saveCoordTransByBinary());
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&(*data)[0]), data->size());
	}
	KTL_INLINE void SpriteCoord::loadCoordTransByBinary(tTJSVariantOctet const* data) {
		instance_->loadCoordTransByBinary(data);
	}
	//
	//	SUMMARY: 更新系メソッド
	//
	KTL_INLINE void SpriteCoord::update() {
		instance_->update();
	}
	//
	//	SUMMARY: パラメータ系メソッド
	//
	KTL_INLINE void SpriteCoord::setScaling(tTVReal v1, tTVReal v2) {
		instance_->setScaling(scaling_type(v1, v2));
	}
	KTL_INLINE void SpriteCoord::setRotation(tTVReal v) {
		instance_->setRotation(v);
	}
	KTL_INLINE tTVReal SpriteCoord::getRotation() const {
		return instance_->getRotation().get<0>();
	}
	KTL_INLINE void SpriteCoord::setTranslation(tTVReal v1, tTVReal v2) {
		instance_->setTranslation(translation_type(v1, v2));
	}
	KTL_INLINE void SpriteCoord::setRect(tTVInteger v1, tTVInteger v2, tTVInteger v3, tTVInteger v4) {
		instance_->setRectLeft(boost::numeric_cast<NativeSpriteCoord::coord_element_type>(v1));
		instance_->setRectTop(boost::numeric_cast<NativeSpriteCoord::coord_element_type>(v2));
		instance_->setRectRight(boost::numeric_cast<NativeSpriteCoord::coord_element_type>(v3));
		instance_->setRectBottom(boost::numeric_cast<NativeSpriteCoord::coord_element_type>(v4));
	}
	KTL_INLINE void SpriteCoord::setCenter(tTVInteger v1, tTVInteger v2) {
		instance_->setCenter(
			coord_type(
				boost::numeric_cast<NativeSpriteCoord::coord_element_type>(v1),
				boost::numeric_cast<NativeSpriteCoord::coord_element_type>(v2)
				)
			);
	}
	//
	// KTL_SPRITE_COORD_INTERIOR_GETTER_TRANS_IMPL
	// KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL
	//
#define KTL_SPRITE_COORD_INTERIOR_GETTER_TRANS_IMPL(getter_name) \
	KTL_INLINE tTVReal SpriteCoord::getter_name() const { \
		return instance_->getter_name(); \
	}
#define KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getter_name) \
	KTL_INLINE tTVInteger SpriteCoord::getter_name() const { \
		return instance_->getter_name(); \
	}
	//
	// KTL_SPRITE_COORD_INTERIOR_SETTER_TRANS_IMPL
	// KTL_SPRITE_COORD_INTERIOR_SETTER_COORD_IMPL
	//
#define KTL_SPRITE_COORD_INTERIOR_SETTER_TRANS_IMPL(setter_name) \
	KTL_INLINE void SpriteCoord::setter_name(tTVReal v) { \
		instance_->setter_name(v); \
	}
#define KTL_SPRITE_COORD_INTERIOR_SETTER_COORD_IMPL(setter_name) \
	KTL_INLINE void SpriteCoord::setter_name(tTVInteger v) { \
		instance_->setter_name(boost::numeric_cast<NativeSpriteCoord::coord_element_type>(v)); \
	}
	//
	// KTL_SPRITE_COORD_INTERIOR_TRANS_IMPL
	// KTL_SPRITE_COORD_INTERIOR_COORD_IMPL
	//
#define KTL_SPRITE_COORD_INTERIOR_TRANS_IMPL(getter_name, setter_name) \
	KTL_SPRITE_COORD_INTERIOR_GETTER_TRANS_IMPL(getter_name); \
	KTL_SPRITE_COORD_INTERIOR_SETTER_TRANS_IMPL(setter_name);
#define KTL_SPRITE_COORD_INTERIOR_COORD_IMPL(getter_name, setter_name) \
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getter_name); \
	KTL_SPRITE_COORD_INTERIOR_SETTER_COORD_IMPL(setter_name);
	//
	//	SUMMARY: パラメータプロパテイ系メソッド
	//
	KTL_SPRITE_COORD_INTERIOR_TRANS_IMPL(getScalingX, setScalingX);
	KTL_SPRITE_COORD_INTERIOR_TRANS_IMPL(getScalingY, setScalingY);
	KTL_SPRITE_COORD_INTERIOR_TRANS_IMPL(getTranslationX, setTranslationX);
	KTL_SPRITE_COORD_INTERIOR_TRANS_IMPL(getTranslationY, setTranslationY);
	KTL_SPRITE_COORD_INTERIOR_COORD_IMPL(getRectLeft, setRectLeft);
	KTL_SPRITE_COORD_INTERIOR_COORD_IMPL(getRectRight, setRectRight);
	KTL_SPRITE_COORD_INTERIOR_COORD_IMPL(getRectTop, setRectTop);
	KTL_SPRITE_COORD_INTERIOR_COORD_IMPL(getRectBottom, setRectBottom);
	KTL_SPRITE_COORD_INTERIOR_COORD_IMPL(getCenterX, setCenterX);
	KTL_SPRITE_COORD_INTERIOR_COORD_IMPL(getCenterY, setCenterY);
	//
	//	SUMMARY: 取得プロパテイ系メソッド
	//
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getSizeX);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getSizeY);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getPoints0X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getPoints0Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getPoints1X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getPoints1Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getPoints2X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getPoints2Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getPoints3X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getPoints3Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getTransformedPoints0X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getTransformedPoints0Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getTransformedPoints1X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getTransformedPoints1Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getTransformedPoints2X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getTransformedPoints2Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getTransformedPoints3X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getTransformedPoints3Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedPoints0X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedPoints0Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedPoints1X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedPoints1Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedPoints2X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedPoints2Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedPoints3X);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedPoints3Y);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedRectLeft);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedRectRight);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedRectTop);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedRectBottom);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedSizeX);
	KTL_SPRITE_COORD_INTERIOR_GETTER_COORD_IMPL(getEnclosedSizeY);
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
