/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_SPRITES_SPRITES_DECL_HPP
#define SRC_SPRITES_SPRITES_DECL_HPP

#include <vector>
#include <string>
#include <memory>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/call_traits.hpp>
#include <sprig/performance_timer.hpp>
#include <sprig/dg/sprite_buffer.hpp>
#include <sprig/dg/sprite_drawer.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeSprites
	//
	class NativeSprites
		: public tTVPContinuousEventCallbackIntf
	{
	public:
		typedef sprig::dg::sprite_buffer<tTJSString> sprite_buffer_type;
		typedef sprite_buffer_type::init_params_type init_params_type;
		typedef sprite_buffer_type::key_type key_type;
		typedef sprite_buffer_type::priority_type priority_type;
		typedef sprite_buffer_type::priority_map_type priority_map_type;
		typedef sprite_buffer_type::instance_type instance_type;
		typedef sprite_buffer_type::coord_type coord_type;
		typedef sprig::dg::sprite_drawer<key_type, priority_type> sprite_drawer_type;
		typedef boost::scoped_ptr<sprite_buffer_type> sprite_buffer_instance_type;
		typedef D3DCOLOR color_type;
		typedef std::basic_string<tjs_char> serialization_string_type;
		typedef std::auto_ptr<serialization_string_type> string_instance_type;
		typedef std::vector<char> binary_type;
		typedef std::auto_ptr<binary_type> binary_instance_type;
	private:
		typedef sprite_buffer_type::value_type value_type;
		typedef boost::optional<value_type const&> optional_value_type;
		typedef boost::shared_ptr<sprite_drawer_type> sprite_drawer_concrete_type;
	public:
		template<typename T>
		static typename sprite_drawer_type::BLEND_MODE toBlendMode(T value) {
			return sprite_drawer_type::to_blend_mode(value);
		}
		template<typename T>
		static typename sprite_drawer_type::HIT_TEST_MODE toHitTestMode(T value) {
			return sprite_drawer_type::to_hit_test_mode(value);
		}
	private:
		sprite_buffer_instance_type sprite_buffer_;
		color_type clear_color_;
		sprig::krkr::tjs::object_type layer_obj_;
		sprig::krkr::tjs::object_type layer_buffer_for_write_prop_;
		sprig::krkr::tjs::object_type layer_buffer_prop_;
		sprig::krkr::tjs::object_type layer_pitch_prop_;
		sprig::krkr::tjs::object_type layer_image_width_prop_;
		sprig::krkr::tjs::object_type layer_image_height_prop_;
		sprig::krkr::tjs::object_type layer_;
		bool enable_;
		bool run_;
		bool requested_update_;
		// DEBUG:
		std::vector<sprig::performance_timer::value_type> performance_times_;
		sprig::performance_timer performance_timer_;
		sprig::performance_timer unit_performance_timer_;
		bool enable_performance_timer_;
	private:
		optional_value_type FindValue(sprig::call_traits<key_type>::param_type key) const;
		sprite_drawer_concrete_type FindSpriteDrawer(sprig::call_traits<key_type>::param_type key) const;
	public:
		NativeSprites();
		~NativeSprites() throw();
		void TJS_INTF_METHOD OnContinuousCallback(tjs_uint64 tick);
	public:
		//
		//	SUMMARY: ブレンド系プロパティ
		//
		enum BlendMode {
			bmDefault = sprite_drawer_type::BLEND_MODE_DEFAULT,
			bmAlpha = sprite_drawer_type::BLEND_MODE_ALPHA,
			bmAddAlpha = sprite_drawer_type::BLEND_MODE_ADD_ALPHA,
			bmAdditive = sprite_drawer_type::BLEND_MODE_ADDITIVE,
			bmSubtractive = sprite_drawer_type::BLEND_MODE_SUBTRACTIVE,
			bmMultiplicative = sprite_drawer_type::BLEND_MODE_MULTIPLICATIVE,
			bmLighten = sprite_drawer_type::BLEND_MODE_LIGHTEN,
			bmDarken = sprite_drawer_type::BLEND_MODE_DARKEN,
			bmScreen = sprite_drawer_type::BLEND_MODE_SCREEN
		};
		//
		//	SUMMARY: ヒットテスト系プロパティ
		//
		enum HitTestMode {
			htmNone = sprite_drawer_type::HIT_TEST_MODE_NONE,
			htmRect = sprite_drawer_type::HIT_TEST_MODE_RECT,
			htmColor = sprite_drawer_type::HIT_TEST_MODE_COLOR
		};
	public:
		//
		//	SUMMARY: 初期化系メソッド
		//
		HRESULT initialize(sprig::call_traits<init_params_type>::param_type params);
		HRESULT reinitialize();
		bool isInitialized() const;
		void checkInitialized() const;
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		string_instance_type saveSpriteBufferByText() const;
		void loadSpriteBufferByText(tjs_char const* data);
		string_instance_type saveSpriteBufferByXML() const;
		void loadSpriteBufferByXML(tjs_char const* data);
		binary_instance_type saveSpriteBufferByBinary() const;
		void loadSpriteBufferByBinary(tTJSVariantOctet const* data);
		//
		//	SUMMARY: スプライトドローワ管理系メソッド
		//
		bool generateSpriteDrawer(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<priority_type>::param_type priority,
			sprig::call_traits<sprig::dg::texture_manager_init_params>::param_type params
			);
		priority_map_type::size_type erase(sprig::call_traits<key_type>::param_type key);
		void clear();
		priority_map_type::size_type size() const;
		priority_map_type::const_iterator begin() const;
		priority_map_type::iterator begin();
		priority_map_type::const_iterator end() const;
		priority_map_type::iterator end();
		priority_map_type::const_iterator find(sprig::call_traits<key_type>::param_type key) const;
		priority_map_type::iterator find(sprig::call_traits<key_type>::param_type key);
		bool exist(sprig::call_traits<key_type>::param_type key) const;
		boost::optional<priority_type> getPriority(sprig::call_traits<key_type>::param_type key) const;
		bool setPriority(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<priority_type>::param_type v
			);
		//
		//	SUMMARY: スプライトドローワ操作系メソッド
		//
		boost::optional<bool> getVisible(sprig::call_traits<key_type>::param_type key) const;
		bool setVisible(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<bool>::param_type v
			);
		sprite_drawer_type::scaling_type getScaling(sprig::call_traits<key_type>::param_type key) const;
		bool setScaling(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::scaling_type>::param_type v
			);
		sprite_drawer_type::scaling_element_type getScalingX(sprig::call_traits<key_type>::param_type key) const;
		sprite_drawer_type::scaling_element_type getScalingY(sprig::call_traits<key_type>::param_type key) const;
		bool setScalingX(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::scaling_element_value_type>::param_type v
			);
		bool setScalingY(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::scaling_element_value_type>::param_type v
			);
		sprite_drawer_type::rotation_type getRotation(sprig::call_traits<key_type>::param_type key) const;
		bool setRotation(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::rotation_type>::param_type v
			);
		sprite_drawer_type::translation_type getTranslation(sprig::call_traits<key_type>::param_type key) const;
		bool setTranslation(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::translation_type>::param_type v
			);
		sprite_drawer_type::translation_element_type getTranslationX(sprig::call_traits<key_type>::param_type key) const;
		sprite_drawer_type::translation_element_type getTranslationY(sprig::call_traits<key_type>::param_type key) const;
		bool setTranslationX(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::translation_element_value_type>::param_type v
			);
		bool setTranslationY(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::translation_element_value_type>::param_type v
			);
		sprite_drawer_type::rect_type getRect(sprig::call_traits<key_type>::param_type key) const;
		bool setRect(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::rect_type>::param_type v
			);
		sprite_drawer_type::rect_element_type getRectLeft(sprig::call_traits<key_type>::param_type key) const;
		sprite_drawer_type::rect_element_type getRectTop(sprig::call_traits<key_type>::param_type key) const;
		sprite_drawer_type::rect_element_type getRectRight(sprig::call_traits<key_type>::param_type key) const;
		sprite_drawer_type::rect_element_type getRectBottom(sprig::call_traits<key_type>::param_type key) const;
		bool setRectLeft(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::rect_element_value_type>::param_type v
			);
		bool setRectTop(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::rect_element_value_type>::param_type v
			);
		bool setRectRight(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::rect_element_value_type>::param_type v
			);
		bool setRectBottom(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::rect_element_value_type>::param_type v
			);
		sprite_drawer_type::center_type getCenter(sprig::call_traits<key_type>::param_type key) const;
		bool setCenter(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::center_type>::param_type v
			);
		sprite_drawer_type::center_element_type getCenterX(sprig::call_traits<key_type>::param_type key) const;
		sprite_drawer_type::center_element_type getCenterY(sprig::call_traits<key_type>::param_type key) const;
		sprite_drawer_type::center_element_type getCenterZ(sprig::call_traits<key_type>::param_type key) const;
		bool setCenterX(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::center_element_value_type>::param_type v
			);
		bool setCenterY(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::center_element_value_type>::param_type v
			);
		bool setCenterZ(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::center_element_value_type>::param_type v
			);
		sprite_drawer_type::position_type getPosition(sprig::call_traits<key_type>::param_type key) const;
		bool setPosition(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::position_type>::param_type v
			);
		sprite_drawer_type::position_element_type getPositionX(sprig::call_traits<key_type>::param_type key) const;
		sprite_drawer_type::position_element_type getPositionY(sprig::call_traits<key_type>::param_type key) const;
		sprite_drawer_type::position_element_type getPositionZ(sprig::call_traits<key_type>::param_type key) const;
		bool setPositionX(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::position_element_value_type>::param_type v
			);
		bool setPositionY(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::position_element_value_type>::param_type v
			);
		bool setPositionZ(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::position_element_value_type>::param_type v
			);
		sprite_drawer_type::color_type getColor(sprig::call_traits<key_type>::param_type key) const;
		bool setColor(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::color_type>::param_type v
			);
		boost::optional<sprite_drawer_type::BLEND_MODE> getBlendMode(sprig::call_traits<key_type>::param_type key) const;
		bool setBlendMode(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::BLEND_MODE>::param_type v
			);
		boost::optional<sprite_drawer_type::HIT_TEST_MODE> getHitTestMode(sprig::call_traits<key_type>::param_type key) const;
		bool setHitTestMode(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::HIT_TEST_MODE>::param_type v
			);
		boost::optional<sprite_drawer_type::hit_threshold_type> getHitThreshold(sprig::call_traits<key_type>::param_type key) const;
		bool setHitThreshold(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<sprite_drawer_type::hit_threshold_type>::param_type v
			);
		//
		//	SUMMARY: 更新系メソッド
		//
		HRESULT onContinuous();
		HRESULT update();
		bool requestUpdate();
		bool isRequestedUpdate();
		//
		//	SUMMARY: 描画系メソッド
		//
		HRESULT draw();
		HRESULT loadImageFromLayer(
			sprig::call_traits<key_type>::param_type key,
			tTJSVariant const* v
			);
		HRESULT clearImage(sprig::call_traits<key_type>::param_type key);
		//
		//	SUMMARY: ヒットテスト系メソッド
		//
		boost::shared_ptr<key_type> hitTest(sprig::call_traits<coord_type>::param_type point);
		sprig::krkr::tjs::object_type multiTargetHitTest(sprig::call_traits<coord_type>::param_type point);
		sprig::krkr::tjs::object_type multiPointHitTest(tjs_int numparams, tTJSVariant** param);
		sprig::krkr::tjs::object_type multiTPHitTest(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: 座標系メソッド
		//
		sprig::krkr::tjs::object_type toLocalCoord(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<coord_type>::param_type v
			) const;
		sprig::krkr::tjs::object_type toGlobalCoord(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<coord_type>::param_type v
			) const;
		//
		//	SUMMARY: プロパティ系メソッド
		//
		color_type getClearColor() const;
		void setClearColor(sprig::call_traits<color_type>::param_type v);
		sprig::krkr::tjs::object_type getLayer() const;
		void setLayer(tTJSVariant const* v);
		bool getEnable() const;
		void setEnable(bool v);
		bool getRun() const;
		void setRun(bool v);
		//
		//	SUMMARY: デバッグ系メソッド
		//
		void outputValueInfo() const;
		bool getEnablePerformanceTimer() const;
		void setEnablePerformanceTimer(bool v);
	};

	//
	//	Sprites
	//
	class Sprites
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeSprites> instance_;
	public:
		Sprites();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: ブレンド系プロパティ
		//
		static tTVInteger const bmDefault = NativeSprites::bmDefault;
		static tTVInteger const bmAlpha = NativeSprites::bmAlpha;
		static tTVInteger const bmAddAlpha = NativeSprites::bmAddAlpha;
		static tTVInteger const bmAdditive = NativeSprites::bmAdditive;
		static tTVInteger const bmSubtractive = NativeSprites::bmSubtractive;
		static tTVInteger const bmMultiplicative = NativeSprites::bmMultiplicative;
		static tTVInteger const bmLighten = NativeSprites::bmLighten;
		static tTVInteger const bmDarken = NativeSprites::bmDarken;
		static tTVInteger const bmScreen = NativeSprites::bmScreen;
		//
		//	SUMMARY: ヒットテスト系プロパティ
		//
		static tTVInteger const htmNone = NativeSprites::htmNone;
		static tTVInteger const htmRect = NativeSprites::htmRect;
		static tTVInteger const htmColor = NativeSprites::htmColor;
		//
		//	SUMMARY: 初期化系メソッド
		//
		bool initialize(tTJSVariant const* params);
		bool reinitialize();
		bool isInitialized() const;
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		tTJSVariant saveSpriteBufferByText() const;
		void loadSpriteBufferByText(tTJSVariantString const* data);
		tTJSVariant saveSpriteBufferByXML() const;
		void loadSpriteBufferByXML(tTJSVariantString const* data);
		tTJSVariant saveSpriteBufferByBinary() const;
		void loadSpriteBufferByBinary(tTJSVariantOctet const* data);
		//
		//	SUMMARY: スプライトドローワ管理系メソッド
		//
		bool generateSpriteDrawer(
			tTJSString const& key,
			tTVReal priority,
			tTJSVariant const* params
			);
		tTVInteger erase(tTJSString const& key);
		void clear();
		tTVInteger size() const;
		bool exist(tTJSString const& key) const;
		boost::optional<tTVReal> getPriority(tTJSString const& key) const;
		bool setPriority(tTJSString const& key, tTVReal v);
		//
		//	SUMMARY: スプライトドローワ操作系メソッド
		//
		boost::optional<tTVInteger> getVisible(tTJSString const& key) const;
		bool setVisible(tTJSString const& key, tTVInteger v);
		bool setScaling(tTJSString const& key, tTVReal v1, tTVReal v2);
		boost::optional<tTVReal> getScalingY(tTJSString const& key) const;
		boost::optional<tTVReal> getScalingX(tTJSString const& key) const;
		bool setScalingX(tTJSString const& key, tTVReal v);
		bool setScalingY(tTJSString const& key, tTVReal v);
		boost::optional<tTVReal> getRotation(tTJSString const& key) const;
		bool setRotation(tTJSString const& key, tTVReal v);
		bool setTranslation(tTJSString const& key, tTVReal v1, tTVReal v2);
		boost::optional<tTVReal> getTranslationX(tTJSString const& key) const;
		boost::optional<tTVReal> getTranslationY(tTJSString const& key) const;
		bool setTranslationX(tTJSString const& key, tTVReal v);
		bool setTranslationY(tTJSString const& key, tTVReal v);
		bool setRect(tTJSString const& key, tTVInteger v1, tTVInteger v2, tTVInteger v3, tTVInteger v4);
		boost::optional<tTVInteger> getRectLeft(tTJSString const& key) const;
		boost::optional<tTVInteger> getRectTop(tTJSString const& key) const;
		boost::optional<tTVInteger> getRectRight(tTJSString const& key) const;
		boost::optional<tTVInteger> getRectBottom(tTJSString const& key) const;
		bool setRectLeft(tTJSString const& key, tTVInteger v);
		bool setRectTop(tTJSString const& key, tTVInteger v);
		bool setRectRight(tTJSString const& key, tTVInteger v);
		bool setRectBottom(tTJSString const& key, tTVInteger v);
		bool setCenter(tTJSString const& key, tTVReal v1, tTVReal v2, tTVReal v3);
		boost::optional<tTVReal> getCenterX(tTJSString const& key) const;
		boost::optional<tTVReal> getCenterY(tTJSString const& key) const;
		boost::optional<tTVReal> getCenterZ(tTJSString const& key) const;
		bool setCenterX(tTJSString const& key, tTVReal v);
		bool setCenterY(tTJSString const& key, tTVReal v);
		bool setCenterZ(tTJSString const& key, tTVReal v);
		bool Sprites::setPosition(tTJSString const& key, tTVReal v1, tTVReal v2, tTVReal v3);
		boost::optional<tTVReal> getPositionX(tTJSString const& key) const;
		boost::optional<tTVReal> getPositionY(tTJSString const& key) const;
		boost::optional<tTVReal> getPositionZ(tTJSString const& key) const;
		bool setPositionX(tTJSString const& key, tTVReal v);
		bool setPositionY(tTJSString const& key, tTVReal v);
		bool setPositionZ(tTJSString const& key, tTVReal v);
		boost::optional<tTVInteger> getColor(tTJSString const& key) const;
		bool setColor(tTJSString const& key, tTVInteger v);
		boost::optional<tTVInteger> getBlendMode(tTJSString const& key) const;
		bool setBlendMode(tTJSString const& key, tTVInteger v);
		boost::optional<tTVInteger> getHitTestMode(tTJSString const& key) const;
		bool setHitTestMode(tTJSString const& key, tTVInteger v);
		boost::optional<tTVInteger> getHitThreshold(tTJSString const& key) const;
		bool setHitThreshold(tTJSString const& key, tTVInteger v);
		//
		//	SUMMARY: 拡張スプライトドローワ操作系メソッド
		//
		bool setScalingXY(tTJSString const& key, tTVReal v);
		bool setScalingSize(tTJSString const& key, tTVInteger v1, tTVInteger v2);
		bool setScalingSizeX(tTJSString const& key, tTVInteger v);
		bool setScalingSizeY(tTJSString const& key, tTVInteger v);
		bool setRectSquare(tTJSString const& key, tTVInteger v1, tTVInteger v2, tTVInteger v3, tTVInteger v4);
		bool setRectPoint(tTJSString const& key, tTVInteger v1, tTVInteger v2);
		boost::optional<tTVInteger> getRectPointX(tTJSString const& key) const;
		boost::optional<tTVInteger> getRectPointY(tTJSString const& key) const;
		bool setRectPointX(tTJSString const& key, tTVInteger v);
		bool setRectPointY(tTJSString const& key, tTVInteger v);
		bool setRectSize(tTJSString const& key, tTVInteger v1, tTVInteger v2);
		boost::optional<tTVInteger> getRectSizeX(tTJSString const& key) const;
		boost::optional<tTVInteger> getRectSizeY(tTJSString const& key) const;
		bool setRectSizeX(tTJSString const& key, tTVInteger v);
		bool setRectSizeY(tTJSString const& key, tTVInteger v);
		bool setRectSizeFromCenter(tTJSString const& key, tTVInteger v1, tTVInteger v2);
		bool setRectSizeXFromCenter(tTJSString const& key, tTVInteger v);
		bool setRectSizeYFromCenter(tTJSString const& key, tTVInteger v);
		bool setRectRegionSize(tTJSString const& key, tTVInteger v1, tTVInteger v2);
		bool setRectRegionSizeX(tTJSString const& key, tTVInteger v);
		bool setRectRegionSizeY(tTJSString const& key, tTVInteger v);
		bool setCenterByRate(tTJSString const& key, tTVReal v1, tTVReal v2, tTVReal v3);
		boost::optional<tTVReal> getCenterXByRate(tTJSString const& key) const;
		boost::optional<tTVReal> getCenterYByRate(tTJSString const& key) const;
		boost::optional<tTVReal> getCenterZByRate(tTJSString const& key) const;
		bool setCenterXByRate(tTJSString const& key, tTVReal v);
		bool setCenterYByRate(tTJSString const& key, tTVReal v);
		bool setCenterZByRate(tTJSString const& key, tTVReal v);
		bool setCenterXYZByRate(tTJSString const& key, tTVReal v);
		boost::optional<tTVReal> getRotationByDegree(tTJSString const& key) const;
		bool setRotationByDegree(tTJSString const& key, tTVReal v);
		//
		//	SUMMARY: 更新系メソッド
		//
		bool onContinuous();
		bool update();
		bool requestUpdate();
		bool isRequestedUpdate();
		//
		//	SUMMARY: 描画系メソッド
		//
		bool draw();
		bool loadImageFromLayer(tTJSString const& key, tTJSVariant const* v);
		bool clearImage(tTJSString const& key);
		//
		//	SUMMARY: ヒットテスト系メソッド
		//
		boost::shared_ptr<tTJSString> hitTest(tTVInteger v1, tTVInteger v2);
		sprig::krkr::tjs::object_type multiTargetHitTest(tTVInteger v1, tTVInteger v2);
		sprig::krkr::tjs::object_type multiPointHitTest(tjs_int numparams, tTJSVariant** param);
		sprig::krkr::tjs::object_type multiTPHitTest(tjs_int numparams, tTJSVariant** param);
		//
		//	SUMMARY: 座標系メソッド
		//
		sprig::krkr::tjs::object_type toLocalCoord(tTJSString const& key, tTVInteger v1, tTVInteger v2) const;
		sprig::krkr::tjs::object_type toGlobalCoord(tTJSString const& key, tTVInteger v1, tTVInteger v2) const;
		//
		//	SUMMARY: プロパティ系メソッド
		//
		tTVInteger getClearColor() const;
		void setClearColor(tTVInteger v);
		iTJSDispatch2* getLayer() const;
		void setLayer(tTJSVariant const* v);
		bool getEnable() const;
		void setEnable(bool v);
		bool getRun() const;
		void setRun(bool v);
		//
		//	SUMMARY: デバッグ系メソッド
		//
		void outputValueInfo() const;
		bool getEnablePerformanceTimer() const;
		void setEnablePerformanceTimer(bool v);
	};
}	// namespace ktl

namespace sprig {
	namespace dg {
		//
		// value_info tTJSString
		//
		SPRIG_DG_SPECIALIZED_VALUE_INFO(tTJSString);
		template<typename loggerT>
		void value_info(tTJSString const& value, loggerT& logger) {
			logger.section_line(TEXT("type"), typeid(value).name());
			logger.section_line(TEXT("str"), value);
		}
		//
		// value_info boost::shared_ptr<sprite_buffer>
		//
		SPRIG_DG_SPECIALIZED_VALUE_INFO(ktl::NativeSprites::sprite_buffer_type::instance_type);
		template<typename loggerT>
		void value_info(ktl::NativeSprites::sprite_buffer_type::instance_type const& value, loggerT& logger) {
			logger.section_line(TEXT("type"), typeid(value).name());
			logger.section_line(TEXT("get"), value.get());
			logger.section_line(TEXT("use_count"), value.use_count());
			{
				ktl::NativeSprites::sprite_drawer_type* sprite_drawer
					= dynamic_cast<ktl::NativeSprites::sprite_drawer_type*>(value.get());
				if (sprite_drawer) {
					value_info(*sprite_drawer, logger);
				}
			}
		}
	}	// namespace dg
}	// namespace sprig

//
//	COMMENT: 具象クラスの登録
//
BOOST_CLASS_EXPORT(ktl::NativeSprites::sprite_drawer_type);
BOOST_CLASS_EXPORT(ktl::NativeSprites::sprite_drawer_type::sprite_drawer_filter_type);
BOOST_CLASS_EXPORT(ktl::NativeSprites::sprite_buffer_type::sprite_transferer_type::render_transferer_type::render_filter_type);

#endif	// #ifndef SRC_SPRITES_SPRITES_DECL_HPP
