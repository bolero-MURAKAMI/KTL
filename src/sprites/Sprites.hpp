#pragma once

#include <cstddef>
#include <deque>
#include <memory>
#include <sstream>
#include <boost/foreach.hpp>
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
#include <sprig/call_traits.hpp>
#include <sprig/get_pointer.hpp>
#include <sprig/dg/foundation.hpp>
#include <sprig/dg/value_info.hpp>
#include <sprig/dg/functions.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "SpritesDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Sprites:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Sprites:error")

//
// KTL_SPRITES_POINTER_CAST
//
#if defined(KTL_SPRITES_CONFIG_USING_STATIC_POINTER_CAST)
#	define KTL_SPRITES_POINTER_CAST boost::static_pointer_cast
#else
#	define KTL_SPRITES_POINTER_CAST boost::dynamic_pointer_cast
#endif
//
// KTL_SPRITES_PERFORMANCE_TIMER_ENABLE_DEFAULT
//
#ifndef KTL_SPRITES_CONFIG_PERFORMANCE_TIMER_ENABLE_DEFAULT
#	define KTL_SPRITES_PERFORMANCE_TIMER_ENABLE_DEFAULT false
#else
#	define KTL_SPRITES_PERFORMANCE_TIMER_ENABLE_DEFAULT true
#endif

namespace ktl {
	//
	// NativeSprites
	//
	KTL_INLINE NativeSprites::optional_value_type NativeSprites::FindValue(
		sprig::call_traits<NativeSprites::key_type>::param_type key
		) const
	{
		priority_map_type::const_iterator i = sprite_buffer_->find(key);
		if (i == sprite_buffer_->end()) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("無効なキーが指定されました:") + key,
				sprig::dg::bad_process
				);
			return boost::none;
		}
		return *i;
	}
	KTL_INLINE NativeSprites::sprite_drawer_concrete_type NativeSprites::FindSpriteDrawer(
		sprig::call_traits<NativeSprites::key_type>::param_type key
		) const
	{
		optional_value_type value = FindValue(key);
		return value ? KTL_SPRITES_POINTER_CAST<sprite_drawer_type>(value->ref_mapped()) : sprite_drawer_concrete_type();
	}
	NativeSprites::NativeSprites()
		: sprite_buffer_(new sprite_buffer_type())
		, clear_color_(D3DCOLOR_ARGB(0x00, 0x00, 0x00, 0x00))
		, layer_obj_(sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Layer")))
		, layer_buffer_for_write_prop_(sprig::krkr::tjs::GetMemberNoAddRef(sprig::get_pointer(layer_obj_), SPRIG_KRKR_TJS_W("mainImageBufferForWrite")))
		, layer_buffer_prop_(sprig::krkr::tjs::GetMemberNoAddRef(sprig::get_pointer(layer_obj_), SPRIG_KRKR_TJS_W("mainImageBuffer")))
		, layer_pitch_prop_(sprig::krkr::tjs::GetMemberNoAddRef(sprig::get_pointer(layer_obj_), SPRIG_KRKR_TJS_W("mainImageBufferPitch")))
		, layer_image_width_prop_(sprig::krkr::tjs::GetMemberNoAddRef(sprig::get_pointer(layer_obj_), SPRIG_KRKR_TJS_W("imageWidth")))
		, layer_image_height_prop_(sprig::krkr::tjs::GetMemberNoAddRef(sprig::get_pointer(layer_obj_), SPRIG_KRKR_TJS_W("imageHeight")))
		, enable_(false)
		, run_(false)
		, requested_update_(false)
		, enable_performance_timer_(KTL_SPRITES_PERFORMANCE_TIMER_ENABLE_DEFAULT)
	{}
	NativeSprites::~NativeSprites() throw() {
		if (run_) {
			TVPRemoveContinuousEventHook(this);
		}
	}
	void TJS_INTF_METHOD NativeSprites::OnContinuousCallback(tjs_uint64 tick) {
		try {
			onContinuous();
		} catch (std::exception const& e) {
			sprig::krkr::tjs::rethrow_exception_to_tjs_exception(e);
		}
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE HRESULT NativeSprites::initialize(sprig::call_traits<NativeSprites::init_params_type>::param_type params) {
		return sprite_buffer_->initialize(params);
	}
	KTL_INLINE HRESULT NativeSprites::reinitialize() {
		requestUpdate();
		return sprite_buffer_->reinitialize();
	}
	KTL_INLINE bool NativeSprites::isInitialized() const {
		return sprite_buffer_->is_initialized();
	}
	KTL_INLINE void NativeSprites::checkInitialized() const {
		if (!isInitialized()) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("NativeSpritesは初期化されていません"),
				sprig::dg::bad_process
				);
		}
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	KTL_INLINE NativeSprites::string_instance_type NativeSprites::saveSpriteBufferByText() const {
		std::basic_ostringstream<tjs_char> os;
		{
			boost::archive::text_woarchive oa(os);
			oa << boost::serialization::make_nvp("sprite_buffer_", sprite_buffer_);
		}
		string_instance_type result(new serialization_string_type(os.str()));
		return result;
	}
	KTL_INLINE void NativeSprites::loadSpriteBufferByText(tjs_char const* data) {
		std::basic_istringstream<tjs_char> is(data ? data : SPRIG_KRKR_TJS_W(""));
		{
			boost::archive::text_wiarchive ia(is);
			ia >> boost::serialization::make_nvp("sprite_buffer_", sprite_buffer_);
		}
	}
	KTL_INLINE NativeSprites::string_instance_type NativeSprites::saveSpriteBufferByXML() const {
		std::basic_ostringstream<tjs_char> os;
		{
			boost::archive::xml_woarchive oa(os);
			oa << boost::serialization::make_nvp("sprite_buffer_", sprite_buffer_);
		}
		string_instance_type result(new serialization_string_type(os.str()));
		return result;
	}
	KTL_INLINE void NativeSprites::loadSpriteBufferByXML(tjs_char const* data) {
		std::basic_istringstream<tjs_char> is(data);
		{
			boost::archive::xml_wiarchive ia(is);
			ia >> boost::serialization::make_nvp("sprite_buffer_", sprite_buffer_);
		}
	}
	KTL_INLINE NativeSprites::binary_instance_type NativeSprites::saveSpriteBufferByBinary() const {
		typedef boost::iostreams::back_insert_device<binary_type> device_type;
		typedef boost::iostreams::stream<device_type> stream_type;

		binary_instance_type result(new binary_type());
		device_type dev(*result);
		stream_type os(dev, std::ios::out | std::ios::binary);
		{
			boost::archive::binary_oarchive oa(os);
			oa << boost::serialization::make_nvp("sprite_buffer_", sprite_buffer_);
		}
		return result;
	}
	KTL_INLINE void NativeSprites::loadSpriteBufferByBinary(tTJSVariantOctet const* data) {
		typedef boost::iostreams::basic_array_source<binary_type::value_type> device_type;
		typedef boost::iostreams::stream<device_type> stream_type;

		device_type dev(
			reinterpret_cast<binary_type::value_type const*>(sprig::krkr::tjs::octet_data(data)),
			sprig::krkr::tjs::octet_length(data)
			);
		stream_type is(dev, std::ios::in | std::ios::binary);
		{
			boost::archive::binary_iarchive ia(is);
			ia >> boost::serialization::make_nvp("sprite_buffer_", sprite_buffer_);
		}
	}
	//
	//	SUMMARY: スプライトドローワ管理系メソッド
	//
	KTL_INLINE bool NativeSprites::generateSpriteDrawer(
		sprig::call_traits<NativeSprites::key_type>::param_type key,
		sprig::call_traits<NativeSprites::priority_type>::param_type priority,
		sprig::call_traits<sprig::dg::texture_manager_init_params>::param_type params
		)
	{
		requestUpdate();
		std::auto_ptr<sprite_drawer_type> sprite_drawer(new sprite_drawer_type());
		sprite_drawer->initialize(sprite_buffer_->get_device(), params);
		return sprite_buffer_->insert(key, priority, instance_type(sprite_drawer)).second;
	}
	KTL_INLINE NativeSprites::priority_map_type::size_type NativeSprites::erase(sprig::call_traits<NativeSprites::key_type>::param_type key) {
		requestUpdate();
		return sprite_buffer_->erase(key);
	}
	KTL_INLINE void NativeSprites::clear() {
		requestUpdate();
		sprite_buffer_->clear();
	}
	KTL_INLINE NativeSprites::priority_map_type::size_type NativeSprites::size() const {
		return sprite_buffer_->size();
	}
	KTL_INLINE NativeSprites::priority_map_type::const_iterator NativeSprites::begin() const {
		return sprite_buffer_->begin();
	}
	KTL_INLINE NativeSprites::priority_map_type::iterator NativeSprites::begin() {
		return sprite_buffer_->begin();
	}
	KTL_INLINE NativeSprites::priority_map_type::const_iterator NativeSprites::end() const {
		return sprite_buffer_->end();
	}
	KTL_INLINE NativeSprites::priority_map_type::iterator NativeSprites::end() {
		return sprite_buffer_->end();
	}
	KTL_INLINE NativeSprites::priority_map_type::const_iterator NativeSprites::find(sprig::call_traits<NativeSprites::key_type>::param_type key) const {
		return sprite_buffer_->find(key);
	}
	KTL_INLINE NativeSprites::priority_map_type::iterator NativeSprites::find(sprig::call_traits<NativeSprites::key_type>::param_type key) {
		return sprite_buffer_->find(key);
	}
	KTL_INLINE bool NativeSprites::exist(sprig::call_traits<key_type>::param_type key) const {
		return sprite_buffer_->find(key) != sprite_buffer_->end();
	}
	KTL_INLINE boost::optional<NativeSprites::priority_type> NativeSprites::getPriority(sprig::call_traits<NativeSprites::key_type>::param_type key) const {
		optional_value_type value = FindValue(key);
		return value ? value->get_priority() : boost::optional<priority_type>();
	}
	KTL_INLINE bool NativeSprites::setPriority(
		sprig::call_traits<NativeSprites::key_type>::param_type key,
		sprig::call_traits<NativeSprites::priority_type>::param_type v
		)
	{
		bool result = true;
		if (!(result = sprite_buffer_->modify_priority(key, v))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("無効なキーが指定されました:") + key,
				sprig::dg::bad_process
				);
			return result;
		}
		requestUpdate();
		return result;
	}
	//
	// KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL
	// KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL
	//
#define KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(parameter_type, GETTER_NAME, drawer_getter_name) \
	KTL_INLINE NativeSprites::sprite_drawer_type::parameter_type NativeSprites::GETTER_NAME( \
		sprig::call_traits<NativeSprites::key_type>::param_type key \
		) const \
	{ \
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key); \
		return sprite_drawer ? sprite_drawer->drawer_getter_name() : boost::none; \
	}
#define KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(parameter_type, SETTER_NAME, drawer_setter_name) \
	KTL_INLINE bool NativeSprites::SETTER_NAME( \
		sprig::call_traits<NativeSprites::key_type>::param_type key, \
		sprig::call_traits<NativeSprites::sprite_drawer_type::parameter_type>::param_type v \
		) \
	{ \
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key); \
		requestUpdate(); \
		return sprite_drawer ? (sprite_drawer->drawer_setter_name(v), true) : false; \
	}
	//
	//	SUMMARY: スプライトドローワ操作系メソッド
	//
	KTL_INLINE boost::optional<bool> NativeSprites::getVisible(sprig::call_traits<NativeSprites::key_type>::param_type key) const {
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key);
		return sprite_drawer ? boost::optional<bool>(sprite_drawer->get_visible()) : boost::none;
	}
	KTL_INLINE bool NativeSprites::setVisible(
		sprig::call_traits<NativeSprites::key_type>::param_type key,
		sprig::call_traits<bool>::param_type v
		)
	{
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key);
		requestUpdate();
		return sprite_drawer ? (sprite_drawer->set_visible(v), true) : false;
	}
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(scaling_type, getScaling, get_scaling);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(scaling_type, setScaling, set_scaling);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(scaling_element_type, getScalingX, get_scaling_x);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(scaling_element_type, getScalingY, get_scaling_y);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(scaling_element_value_type, setScalingX, set_scaling_x);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(scaling_element_value_type, setScalingY, set_scaling_y);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(rotation_type, getRotation, get_rotation);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(rotation_type, setRotation, set_rotation);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(translation_type, getTranslation, get_translation);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(translation_type, setTranslation, set_translation);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(translation_element_type, getTranslationX, get_translation_x);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(translation_element_type, getTranslationY, get_translation_y);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(translation_element_value_type, setTranslationX, set_translation_x);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(translation_element_value_type, setTranslationY, set_translation_y);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(rect_type, getRect, get_rect);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(rect_type, setRect, set_rect);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(rect_element_type, getRectLeft, get_rect_left);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(rect_element_type, getRectTop, get_rect_top);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(rect_element_type, getRectRight, get_rect_right);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(rect_element_type, getRectBottom, get_rect_bottom);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(rect_element_value_type, setRectLeft, set_rect_left);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(rect_element_value_type, setRectTop, set_rect_top);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(rect_element_value_type, setRectRight, set_rect_right);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(rect_element_value_type, setRectBottom, set_rect_bottom);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(center_type, getCenter, get_center);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(center_type, setCenter, set_center);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(center_element_type, getCenterX, get_center_x);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(center_element_type, getCenterY, get_center_y);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(center_element_type, getCenterZ, get_center_z);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(center_element_value_type, setCenterX, set_center_x);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(center_element_value_type, setCenterY, set_center_y);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(center_element_value_type, setCenterZ, set_center_z);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(position_type, getPosition, get_position);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(position_type, setPosition, set_position);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(position_element_type, getPositionX, get_position_x);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(position_element_type, getPositionY, get_position_y);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(position_element_type, getPositionZ, get_position_z);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(position_element_value_type, setPositionX, set_position_x);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(position_element_value_type, setPositionY, set_position_y);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(position_element_value_type, setPositionZ, set_position_z);
	KTL_SPRITE_INTERIOR_DRAWER_GETTER_IMPL(color_type, getColor, get_color);
	KTL_SPRITE_INTERIOR_DRAWER_SETTER_IMPL(color_type, setColor, set_color);
	KTL_INLINE boost::optional<NativeSprites::sprite_drawer_type::BLEND_MODE> NativeSprites::getBlendMode(
		sprig::call_traits<NativeSprites::key_type>::param_type key
		) const
	{
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key);
		return sprite_drawer ? boost::optional<sprite_drawer_type::BLEND_MODE>(sprite_drawer->get_blend_mode()) : boost::none;
	}
	KTL_INLINE bool NativeSprites::setBlendMode(
		sprig::call_traits<NativeSprites::key_type>::param_type key,
		sprig::call_traits<NativeSprites::sprite_drawer_type::BLEND_MODE>::param_type v
		)
	{
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key);
		return sprite_drawer ? (sprite_drawer->set_blend_mode(v), true) : false;
	}
	KTL_INLINE boost::optional<NativeSprites::sprite_drawer_type::HIT_TEST_MODE> NativeSprites::getHitTestMode(
		sprig::call_traits<NativeSprites::key_type>::param_type key
		) const
	{
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key);
		return sprite_drawer ? boost::optional<sprite_drawer_type::HIT_TEST_MODE>(sprite_drawer->get_hit_test_mode()) : boost::none;
	}
	KTL_INLINE bool NativeSprites::setHitTestMode(
		sprig::call_traits<NativeSprites::key_type>::param_type key,
		sprig::call_traits<NativeSprites::sprite_drawer_type::HIT_TEST_MODE>::param_type v
		)
	{
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key);
		return sprite_drawer ? (sprite_drawer->set_hit_test_mode(v), true) : false;
	}
	KTL_INLINE boost::optional<NativeSprites::sprite_drawer_type::hit_threshold_type> NativeSprites::getHitThreshold(
		sprig::call_traits<NativeSprites::key_type>::param_type key
		) const
	{
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key);
		return sprite_drawer ? boost::optional<sprite_drawer_type::hit_threshold_type>(sprite_drawer->get_hit_threshold()) : boost::none;
	}
	KTL_INLINE bool NativeSprites::setHitThreshold(
		sprig::call_traits<NativeSprites::key_type>::param_type key,
		sprig::call_traits<NativeSprites::sprite_drawer_type::hit_threshold_type>::param_type v
		)
	{
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key);
		return sprite_drawer ? (sprite_drawer->set_hit_threshold(v), true) : false;
	}
	//
	//	SUMMARY: 更新系メソッド
	//
	KTL_INLINE HRESULT NativeSprites::onContinuous() {
		HRESULT result = D3D_OK;
		// DEBUG
		if (enable_performance_timer_) {
			performance_timer_.restart();
		}
		checkInitialized();
		if (enable_) {
			if (FAILED(result = update())) {
				return result;
			}
		}
		// DEBUG
		if (enable_performance_timer_) {
			performance_times_.push_back(performance_timer_.elapsed());
			if (unit_performance_timer_.elapsed() > 1.0) {
				unit_performance_timer_.restart();
				sprig::performance_timer::value_type average = 0;
				BOOST_FOREACH(sprig::performance_timer::value_type const& e, performance_times_) {
					average += e;
				}
				average /= performance_times_.size();
				performance_times_.clear();
				SPRIG_KRKR_OUTPUT_VALUE(
					SPRIG_KRKR_TJS_W("KTL:Sprites:onContinuous:elapsed-average"),
					average,
					SPRIG_KRKR_LOG_LEVEL_IMPORTANT
					);
			}
		}
		return result;
	}
	KTL_INLINE HRESULT NativeSprites::update() {
		HRESULT result = D3D_OK;
		if (requested_update_) {
			if (FAILED(result = draw())) {
				return result;
			}
			requested_update_ = false;
		}
		return result;
	}
	KTL_INLINE bool NativeSprites::requestUpdate() {
		bool before = requested_update_;
		requested_update_ = true;
		return before;
	}
	KTL_INLINE bool NativeSprites::isRequestedUpdate() {
		return requested_update_;
	}
	//
	//	SUMMARY: 描画系メソッド
	//
	KTL_INLINE HRESULT NativeSprites::draw() {
		HRESULT result = D3D_OK;
		if (!enable_ || !layer_) {
			result = S_FALSE;
			return result;
		}
		if (FAILED(result = sprite_buffer_->draw_render_target(true, clear_color_))) {
			return result;
		}
		void* const bits = reinterpret_cast<void*>(
			sprig::krkr::tjs::GetPropValue<tjs_int>(sprig::get_pointer(layer_buffer_for_write_prop_), sprig::get_pointer(layer_))
			);
		SIZE const size = {
			sprig::krkr::tjs::GetPropValue<tjs_int>(sprig::get_pointer(layer_image_width_prop_), sprig::get_pointer(layer_)),
			sprig::krkr::tjs::GetPropValue<tjs_int>(sprig::get_pointer(layer_image_height_prop_), sprig::get_pointer(layer_)),
		};
		std::size_t const pitch = sprig::krkr::tjs::GetPropValue<tjs_int>(sprig::get_pointer(layer_pitch_prop_), sprig::get_pointer(layer_));
		std::size_t const pixel_size = 4;
		POINT const offset = {0, 0};
		RECT const rect = {0, 0, size.cx, size.cy};
		if (FAILED(result = sprite_buffer_->transfer(bits, size, pitch, pixel_size, offset, rect))) {
			return result;
		}
		sprig::krkr::tjs::MethodCall(sprig::get_pointer(layer_), SPRIG_KRKR_TJS_W("update"), 0, 0, 0);
		return result;
	}
	KTL_INLINE HRESULT NativeSprites::loadImageFromLayer(
		sprig::call_traits<NativeSprites::key_type>::param_type key,
		tTJSVariant const* v
		)
	{
		HRESULT result = D3D_OK;
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key);
		if (!sprite_drawer) {
			result = S_FALSE;
			return result;
		}
		if (FAILED(result = sprig::dg::fill_surface(
			sprite_drawer->get_surface(),
			4,
			boost::none,
			sprite_drawer->get_surface_desc()
			)))
		{
			return result;
		}
		if (!v || v->Type() == tvtVoid) {
		} else if (v->Type() == tvtObject
			&& sprig::krkr::tjs::IsInstanceObjectOf(v->AsObjectNoAddRef(), SPRIG_KRKR_TJS_W("Layer"))
			)
		{
			iTJSDispatch2* layer = v->AsObjectNoAddRef();
			void* const bits = reinterpret_cast<void*>(
				sprig::krkr::tjs::GetPropValue<tjs_int>(sprig::get_pointer(layer_buffer_prop_), sprig::get_pointer(layer))
				);
			SIZE const size = {
				sprig::krkr::tjs::GetPropValue<tjs_int>(sprig::get_pointer(layer_image_width_prop_), sprig::get_pointer(layer)),
				sprig::krkr::tjs::GetPropValue<tjs_int>(sprig::get_pointer(layer_image_height_prop_), sprig::get_pointer(layer))
			};
			std::size_t const pitch = sprig::krkr::tjs::GetPropValue<tjs_int>(sprig::get_pointer(layer_pitch_prop_), sprig::get_pointer(layer));
			std::size_t const pixel_size = 4;
			POINT const offset = {0, 0};
			RECT const rect = {0, 0, size.cx, size.cy};
			if (FAILED(result = sprig::dg::transfer_from_memory(
				sprite_drawer->get_surface(),
				bits,
				size,
				pitch,
				pixel_size,
				offset,
				rect,
				sprite_drawer->get_surface_desc()
				)))
			{
				return result;
			}
		} else {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("Layerオブジェクトでもvoidでもない値が渡されました"),
				sprig::dg::bad_process
				);
			result = E_FAIL;
			return result;
		}
		return result;
	}
	KTL_INLINE HRESULT NativeSprites::clearImage(sprig::call_traits<key_type>::param_type key) {
		HRESULT result = D3D_OK;
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key);
		if (!sprite_drawer) {
			result = S_FALSE;
			return result;
		}
		if (FAILED(result = sprig::dg::fill_surface(
			sprite_drawer->get_surface(),
			4,
			boost::none,
			sprite_drawer->get_surface_desc()
			)))
		{
			return result;
		}
		requestUpdate();
		return result;
	}
	//
	//	SUMMARY: ヒットテスト系メソッド
	//
	KTL_INLINE boost::shared_ptr<NativeSprites::key_type> NativeSprites::hitTest(sprig::call_traits<coord_type>::param_type point) {
		typedef boost::shared_ptr<NativeSprites::key_type> result_type;
		if (!enable_) {
			return result_type();
		}
		bool hit = false;
		key_type key;
		if (FAILED(sprite_buffer_->hit_test_period(hit, key, point))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ヒットテストでエラーが発生しました"),
				sprig::dg::bad_process
				);
			return result_type();
		}
		return hit
			? boost::make_shared<key_type>(key)
			: result_type()
			;
	}
	sprig::krkr::tjs::object_type NativeSprites::multiTargetHitTest(sprig::call_traits<coord_type>::param_type point) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		if (!enable_) {
			return result;
		}
		std::size_t hit = 0;
		std::deque<key_type> key(size());
		if (FAILED(sprite_buffer_->multi_hit_test_period(hit, key.begin(), point))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ヒットテストでエラーが発生しました"),
				sprig::dg::bad_process
				);
			return result;
		}
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj,
				0,
				0,
				0
				);
			result = result_type(result_obj, false);
		}
		{
			std::deque<key_type>::const_iterator k = key.begin();
			for (std::size_t i = 0; i != hit; ++i, ++k) {
				sprig::krkr::tjs::SetPropValue(tTJSVariant(*k), i, sprig::get_pointer(result));
			}
		}
		return result;
	}
	sprig::krkr::tjs::object_type NativeSprites::multiPointHitTest(tjs_int numparams, tTJSVariant** param) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		if (!enable_) {
			return result;
		}
		std::deque<bool> hit(numparams, false);
		std::deque<key_type> key(numparams);
		std::deque<coord_type> point;
		for (std::size_t i = 0; i != numparams; ++i) {
			if (param[i]->Type() != tvtObject) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("オブジェクトでない引数です"),
					sprig::dg::bad_process
					);
				return result;
			}
			iTJSDispatch2* param_obj = param[i]->AsObjectNoAddRef();
			point.push_back(coord_type(
				static_cast<FLOAT>(sprig::krkr::tjs::GetPropValue<tjs_int>(0, param_obj)),
				static_cast<FLOAT>(sprig::krkr::tjs::GetPropValue<tjs_int>(1, param_obj)),
				0.0f
				));
		}
		if (FAILED(sprite_buffer_->hit_test_period(hit.begin(), key.begin(), point.begin(), point.end()))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ヒットテストでエラーが発生しました"),
				sprig::dg::bad_process
				);
			return result;
		}
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj,
				0,
				0,
				0
				);
			result = result_type(result_obj, false);
		}
		{
			std::deque<bool>::const_iterator h = hit.begin();
			std::deque<key_type>::const_iterator k = key.begin();
			for (std::size_t i = 0; i != numparams; ++i, ++h, ++k) {
				sprig::krkr::tjs::SetPropValue(*h ? tTJSVariant(*k) : tTJSVariant(), i, sprig::get_pointer(result));
			}
		}
		return result;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeSprites::multiTPHitTest(tjs_int numparams, tTJSVariant** param) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		if (!enable_) {
			return result;
		}
		std::deque<std::size_t> hit(numparams, 0);
		std::deque<std::deque<key_type> > keys(numparams, std::deque<key_type>(size()));
		std::deque<coord_type> point;
		for (std::size_t i = 0; i != numparams; ++i) {
			if (param[i]->Type() != tvtObject) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("オブジェクトでない引数です"),
					sprig::dg::bad_process
					);
				return result;
			}
			iTJSDispatch2* param_obj = param[i]->AsObjectNoAddRef();
			point.push_back(coord_type(
				static_cast<FLOAT>(sprig::krkr::tjs::GetPropValue<tjs_int>(0, param_obj)),
				static_cast<FLOAT>(sprig::krkr::tjs::GetPropValue<tjs_int>(1, param_obj)),
				0.0f
				));
		}
		{
			std::deque<std::deque<key_type>::iterator> key(numparams);
			for (std::size_t i = 0; i != numparams; ++i) {
				key[i] = keys[i].begin();
			}
			if (FAILED(sprite_buffer_->multi_hit_test_period(hit.begin(), key.begin(), point.begin(), point.end()))) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("ヒットテストでエラーが発生しました"),
					sprig::dg::bad_process
					);
				return result;
			}
		}
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj,
				0,
				0,
				0
				);
			result = result_type(result_obj, false);
		}
		{
			std::deque<std::size_t>::const_iterator h = hit.begin();
			std::deque<std::deque<key_type> >::const_iterator key = keys.begin();
			for (std::size_t i = 0; i != numparams; ++i, ++h, ++key) {
				result_type result_elem;
				{
					iTJSDispatch2* result_elem_obj = 0;
					sprig::krkr::tjs::CreateNewObject(
						sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
						&result_elem_obj,
						0,
						0,
						0
						);
					result_elem = result_type(result_elem_obj, false);
					tTJSVariant var(result_elem_obj, result_elem_obj);
					result_elem->AddRef();
					sprig::krkr::tjs::SetPropValue(var, i, sprig::get_pointer(result));
				}
				std::deque<key_type>::const_iterator k = key->begin();
				std::size_t count = *h;
				for (std::size_t j = 0; j != count; ++j, ++k) {
					sprig::krkr::tjs::SetPropValue(tTJSVariant(*k), j, sprig::get_pointer(result_elem));
				}
			}
		}
		return result;
	}
	//
	//	SUMMARY: 座標系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type NativeSprites::toLocalCoord(
		sprig::call_traits<key_type>::param_type key,
		sprig::call_traits<coord_type>::param_type v
		) const
	{
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		if (!enable_) {
			return result;
		}
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key); \
		coord_type coord = sprite_drawer->to_local_coord(v);
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj,
				0,
				0,
				0
				);
			result = result_type(result_obj, false);
		}
		sprig::krkr::tjs::SetPropValue(coord.x, 0, sprig::get_pointer(result));
		sprig::krkr::tjs::SetPropValue(coord.y, 1, sprig::get_pointer(result));
		return result;
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeSprites::toGlobalCoord(
		sprig::call_traits<key_type>::param_type key,
		sprig::call_traits<coord_type>::param_type v
		) const
	{
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		if (!enable_) {
			return result;
		}
		sprite_drawer_concrete_type sprite_drawer = FindSpriteDrawer(key); \
		coord_type coord = sprite_drawer->to_global_coord(v);
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj,
				0,
				0,
				0
				);
			result = result_type(result_obj, false);
		}
		sprig::krkr::tjs::SetPropValue(coord.x, 0, sprig::get_pointer(result));
		sprig::krkr::tjs::SetPropValue(coord.y, 1, sprig::get_pointer(result));
		return result;
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE NativeSprites::color_type NativeSprites::getClearColor() const {
		return clear_color_;
	}
	KTL_INLINE void NativeSprites::setClearColor(sprig::call_traits<NativeSprites::color_type>::param_type v) {
		clear_color_ = v;
		requestUpdate();
	}
	KTL_INLINE sprig::krkr::tjs::object_type NativeSprites::getLayer() const {
		return layer_;
	}
	KTL_INLINE void NativeSprites::setLayer(tTJSVariant const* v) {
		if (!v || v->Type() == tvtVoid) {
			layer_.reset();
		} else if (v->Type() == tvtObject
			&& sprig::krkr::tjs::IsInstanceObjectOf(v->AsObjectNoAddRef(), SPRIG_KRKR_TJS_W("Layer"))
			)
		{
			layer_ = v->AsObjectNoAddRef();
		} else {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("Layerオブジェクトでもvoidでもない値が渡されました"),
				sprig::dg::bad_process
				);
		}
		requestUpdate();
	}
	KTL_INLINE bool NativeSprites::getEnable() const {
		return enable_;
	}
	KTL_INLINE void NativeSprites::setEnable(bool v) {
		enable_ = v;
		requestUpdate();
	}
	KTL_INLINE bool NativeSprites::getRun() const {
		return run_;
	}
	KTL_INLINE void NativeSprites::setRun(bool v) {
		if (!run_ && v) {
			TVPAddContinuousEventHook(this);
		} else if (run_ && !v) {
			TVPRemoveContinuousEventHook(this);
		}
		run_ = v;
		requestUpdate();
	}
	//
	//	SUMMARY: デバッグ系メソッド
	//
	KTL_INLINE void NativeSprites::outputValueInfo() const {
		SPRIG_DG_OUTPUT_VALUE_INFO(TEXT("NativeSprites::outputValueInfo"), sprite_buffer_);
	}
	KTL_INLINE bool NativeSprites::getEnablePerformanceTimer() const {
		return enable_performance_timer_;
	}
	KTL_INLINE void NativeSprites::setEnablePerformanceTimer(bool v) {
		enable_performance_timer_ = v;
	}

	//
	// Sprites
	//
	Sprites::Sprites() {}
	tjs_error TJS_INTF_METHOD Sprites::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Sprites::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeSprites>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Sprites::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Sprites::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE bool Sprites::initialize(tTJSVariant const* params) {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Sprites::initialize"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		bool result = true;
		if (params && params->Type() == tvtObject) {
			sprig::krkr::tjs::object_type obj = params->AsObjectNoAddRef();
			NativeSprites::init_params_type sbip;
			tTJSVariant v;
			if (sprig::krkr::tjs::MemberGet(sprig::get_pointer(obj), SPRIG_KRKR_TJS_W("name"), &v)->Type() != tvtVoid) {
				sbip.name = static_cast<tTJSString>(v).c_str();
			}
			if (sprig::krkr::tjs::MemberGet(sprig::get_pointer(obj), SPRIG_KRKR_TJS_W("width"), &v)->Type() != tvtVoid) {
				sbip.tmip.width = static_cast<tjs_int>(v);
			}
			if (sprig::krkr::tjs::MemberGet(sprig::get_pointer(obj), SPRIG_KRKR_TJS_W("height"), &v)->Type() != tvtVoid) {
				sbip.tmip.height = static_cast<tjs_int>(v);
			}
			sbip.tmip.format = D3DFMT_A8R8G8B8;
			result = SUCCEEDED(instance_->initialize(sbip));
		} else {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("有効なオブジェクトでない値が渡されました"),
				sprig::dg::bad_process
				);
			result = false;
			return result;
		}
		return result;
	}
	KTL_INLINE bool Sprites::reinitialize() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Sprites::reinitialize"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		return SUCCEEDED(instance_->reinitialize());
	}
	KTL_INLINE bool Sprites::isInitialized() const {
		return instance_->isInitialized();
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	KTL_INLINE tTJSVariant Sprites::saveSpriteBufferByText() const {
		return tTJSVariant(instance_->saveSpriteBufferByText()->c_str());
	}
	KTL_INLINE void Sprites::loadSpriteBufferByText(tTJSVariantString const* data) {
		instance_->loadSpriteBufferByText(sprig::krkr::tjs::as_c_str(data));
	}
	KTL_INLINE tTJSVariant Sprites::saveSpriteBufferByXML() const {
		return tTJSVariant(instance_->saveSpriteBufferByXML()->c_str());
	}
	KTL_INLINE void Sprites::loadSpriteBufferByXML(tTJSVariantString const* data) {
		instance_->loadSpriteBufferByXML(sprig::krkr::tjs::as_c_str(data));
	}
	KTL_INLINE tTJSVariant Sprites::saveSpriteBufferByBinary() const {
		NativeSprites::binary_instance_type data(instance_->saveSpriteBufferByBinary());
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&(*data)[0]), data->size());
	}
	KTL_INLINE void Sprites::loadSpriteBufferByBinary(tTJSVariantOctet const* data) {
		instance_->loadSpriteBufferByBinary(data);
	}
	//
	//	SUMMARY: スプライトドローワ管理系メソッド
	//
	KTL_INLINE bool Sprites::generateSpriteDrawer(
		tTJSString const& key,
		tTVReal priority,
		tTJSVariant const* params
		)
	{
		bool result = true;
		if (params && params->Type() == tvtObject) {
			sprig::krkr::tjs::object_type obj = params->AsObjectNoAddRef();
			sprig::dg::texture_manager_init_params tmip;
			tTJSVariant v;
			if (sprig::krkr::tjs::MemberGet(sprig::get_pointer(obj), SPRIG_KRKR_TJS_W("width"), &v)->Type() != tvtVoid) {
				tmip.width = static_cast<tjs_int>(v);
			}
			if (sprig::krkr::tjs::MemberGet(sprig::get_pointer(obj), SPRIG_KRKR_TJS_W("height"), &v)->Type() != tvtVoid) {
				tmip.height = static_cast<tjs_int>(v);
			}
			tmip.format = D3DFMT_A8R8G8B8;
			return result = instance_->generateSpriteDrawer(key, priority, tmip);
		} else {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("有効なオブジェクトでない値が渡されました"),
				sprig::dg::bad_process
				);
			result = false;
			return result;
		}
		return result;
	}
	KTL_INLINE tTVInteger Sprites::erase(tTJSString const& key) {
		return instance_->erase(key);
	}
	KTL_INLINE void Sprites::clear() {
		instance_->clear();
	}
	KTL_INLINE tTVInteger Sprites::size() const {
		return instance_->size();
	}
	KTL_INLINE bool Sprites::exist(tTJSString const& key) const {
		return instance_->exist(key);
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getPriority(tTJSString const& key) const {
		return instance_->getPriority(key);
	}
	KTL_INLINE bool Sprites::setPriority(tTJSString const& key, tTVReal v) {
		return instance_->setPriority(key, v);
	}
	//
	//	SUMMARY: スプライトドローワ操作系メソッド
	//
	KTL_INLINE boost::optional<tTVInteger> Sprites::getVisible(tTJSString const& key) const {
		return boost::optional<tTVInteger>(instance_->getVisible(key));
	}
	KTL_INLINE bool Sprites::setVisible(tTJSString const& key, tTVInteger v) {
		return instance_->setVisible(key, v != 0);
	}
	KTL_INLINE bool Sprites::setScaling(tTJSString const& key, tTVReal v1, tTVReal v2) {
		return instance_->setScaling(key, D3DXVECTOR2(static_cast<FLOAT>(v1), static_cast<FLOAT>(v2)));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getScalingX(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getScalingX(key));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getScalingY(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getScalingX(key));
	}
	KTL_INLINE bool Sprites::setScalingX(tTJSString const& key, tTVReal v) {
		return instance_->setScalingX(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE bool Sprites::setScalingY(tTJSString const& key, tTVReal v) {
		return instance_->setScalingY(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getRotation(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getRotation(key));
	}
	KTL_INLINE bool Sprites::setRotation(tTJSString const& key, tTVReal v) {
		return instance_->setRotation(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE bool Sprites::setTranslation(tTJSString const& key, tTVReal v1, tTVReal v2) {
		return instance_->setTranslation(key, D3DXVECTOR2(static_cast<FLOAT>(v1), static_cast<FLOAT>(v2)));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getTranslationX(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getTranslationX(key));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getTranslationY(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getTranslationY(key));
	}
	KTL_INLINE bool Sprites::setTranslationX(tTJSString const& key, tTVReal v) {
		return instance_->setTranslationX(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE bool Sprites::setTranslationY(tTJSString const& key, tTVReal v) {
		return instance_->setTranslationY(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE bool Sprites::setRect(tTJSString const& key, tTVInteger v1, tTVInteger v2, tTVInteger v3, tTVInteger v4) {
		return instance_->setRect(
			key,
			sprig::dg::make_rect(
				boost::numeric_cast<NativeSprites::sprite_drawer_type::rect_element_value_type>(v1),
				boost::numeric_cast<NativeSprites::sprite_drawer_type::rect_element_value_type>(v2),
				boost::numeric_cast<NativeSprites::sprite_drawer_type::rect_element_value_type>(v3),
				boost::numeric_cast<NativeSprites::sprite_drawer_type::rect_element_value_type>(v4)
				)
			);
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getRectLeft(tTJSString const& key) const {
		return boost::optional<tTVInteger>(instance_->getRectLeft(key));
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getRectTop(tTJSString const& key) const {
		return boost::optional<tTVInteger>(instance_->getRectTop(key));
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getRectRight(tTJSString const& key) const {
		return boost::optional<tTVInteger>(instance_->getRectRight(key));
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getRectBottom(tTJSString const& key) const {
		return boost::optional<tTVInteger>(instance_->getRectBottom(key));
	}
	KTL_INLINE bool Sprites::setRectLeft(tTJSString const& key, tTVInteger v) {
		return instance_->setRectLeft(key, boost::numeric_cast<NativeSprites::sprite_drawer_type::rect_element_value_type>(v));
	}
	KTL_INLINE bool Sprites::setRectTop(tTJSString const& key, tTVInteger v) {
		return instance_->setRectTop(key, boost::numeric_cast<NativeSprites::sprite_drawer_type::rect_element_value_type>(v));
	}
	KTL_INLINE bool Sprites::setRectRight(tTJSString const& key, tTVInteger v) {
		return instance_->setRectRight(key, boost::numeric_cast<NativeSprites::sprite_drawer_type::rect_element_value_type>(v));
	}
	KTL_INLINE bool Sprites::setRectBottom(tTJSString const& key, tTVInteger v) {
		return instance_->setRectBottom(key, boost::numeric_cast<NativeSprites::sprite_drawer_type::rect_element_value_type>(v));
	}
	KTL_INLINE bool Sprites::setCenter(tTJSString const& key, tTVReal v1, tTVReal v2, tTVReal v3) {
		return instance_->setCenter(key, D3DXVECTOR3(static_cast<FLOAT>(v1), static_cast<FLOAT>(v2), static_cast<FLOAT>(v3)));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getCenterX(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getCenterX(key));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getCenterY(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getCenterY(key));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getCenterZ(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getCenterZ(key));
	}
	KTL_INLINE bool Sprites::setCenterX(tTJSString const& key, tTVReal v) {
		return instance_->setCenterX(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE bool Sprites::setCenterY(tTJSString const& key, tTVReal v) {
		return instance_->setCenterY(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE bool Sprites::setCenterZ(tTJSString const& key, tTVReal v) {
		return instance_->setCenterZ(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE bool Sprites::setPosition(tTJSString const& key, tTVReal v1, tTVReal v2, tTVReal v3) {
		return instance_->setCenter(key, D3DXVECTOR3(static_cast<FLOAT>(v1), static_cast<FLOAT>(v2), static_cast<FLOAT>(v3)));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getPositionX(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getPositionX(key));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getPositionY(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getPositionY(key));
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getPositionZ(tTJSString const& key) const {
		return boost::optional<tTVReal>(instance_->getPositionZ(key));
	}
	KTL_INLINE bool Sprites::setPositionX(tTJSString const& key, tTVReal v) {
		return instance_->setPositionX(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE bool Sprites::setPositionY(tTJSString const& key, tTVReal v) {
		return instance_->setPositionY(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE bool Sprites::setPositionZ(tTJSString const& key, tTVReal v) {
		return instance_->setPositionZ(key, static_cast<FLOAT>(v));
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getColor(tTJSString const& key) const {
		return boost::optional<tTVInteger>(instance_->getColor(key));
	}
	KTL_INLINE bool Sprites::setColor(tTJSString const& key, tTVInteger v) {
		return instance_->setColor(key, boost::numeric_cast<NativeSprites::sprite_drawer_type::color_type::value_type>(v));
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getBlendMode(tTJSString const& key) const {
		return boost::optional<tTVInteger>(instance_->getBlendMode(key));
	}
	KTL_INLINE bool Sprites::setBlendMode(tTJSString const& key, tTVInteger v) {
		return instance_->setBlendMode(key, NativeSprites::toBlendMode(v));
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getHitTestMode(tTJSString const& key) const {
		return boost::optional<tTVInteger>(instance_->getHitTestMode(key));
	}
	KTL_INLINE bool Sprites::setHitTestMode(tTJSString const& key, tTVInteger v) {
		return instance_->setHitTestMode(key, NativeSprites::toHitTestMode(v));
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getHitThreshold(tTJSString const& key) const {
		return boost::optional<tTVInteger>(instance_->getHitThreshold(key));
	}
	KTL_INLINE bool Sprites::setHitThreshold(tTJSString const& key, tTVInteger v) {
		return instance_->setHitThreshold(key, boost::numeric_cast<NativeSprites::sprite_drawer_type::hit_threshold_type>(v));
	}
	//
	//	SUMMARY: 拡張スプライトドローワ操作系メソッド
	//
	KTL_INLINE bool Sprites::setScalingXY(tTJSString const& key, tTVReal v) {
		return setScaling(key, v, v);
	}
	KTL_INLINE bool Sprites::setScalingSize(tTJSString const& key, tTVInteger v1, tTVInteger v2) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setScaling(key, static_cast<tTVReal>(v1) / (rect.right - rect.left), static_cast<tTVReal>(v2) / (rect.bottom - rect.top));
	}
	KTL_INLINE bool Sprites::setScalingSizeX(tTJSString const& key, tTVInteger v) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setScalingX(key, static_cast<tTVReal>(v) / (rect.right - rect.left));
	}
	KTL_INLINE bool Sprites::setScalingSizeY(tTJSString const& key, tTVInteger v) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setScalingY(key, static_cast<tTVReal>(v) / (rect.bottom - rect.top));
	}
	KTL_INLINE bool Sprites::setRectSquare(tTJSString const& key, tTVInteger v1, tTVInteger v2, tTVInteger v3, tTVInteger v4) {
		return setRect(key, v1, v2, v1 + v3, v2 + v4);
	}
	KTL_INLINE bool Sprites::setRectPoint(tTJSString const& key, tTVInteger v1, tTVInteger v2) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setRect(key, v1, v2, v1 + (rect.right - rect.left), v2 + (rect.bottom - rect.top));
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getRectPointX(tTJSString const& key) const {
		return getRectLeft(key);
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getRectPointY(tTJSString const& key) const {
		return getRectTop(key);
	}
	KTL_INLINE bool Sprites::setRectPointX(tTJSString const& key, tTVInteger v) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setRect(key, v, rect.top, v + (rect.right - rect.left), rect.bottom);
	}
	KTL_INLINE bool Sprites::setRectPointY(tTJSString const& key, tTVInteger v) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setRect(key, rect.left, v, rect.right, v + (rect.bottom - rect.top));
	}
	KTL_INLINE bool Sprites::setRectSize(tTJSString const& key, tTVInteger v1, tTVInteger v2) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setRect(key, rect.left, rect.top, rect.left + v1, rect.top + v2);
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getRectSizeX(tTJSString const& key) const {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return rect.right - rect.left;
	}
	KTL_INLINE boost::optional<tTVInteger> Sprites::getRectSizeY(tTJSString const& key) const {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return rect.bottom - rect.top;
	}
	KTL_INLINE bool Sprites::setRectSizeX(tTJSString const& key, tTVInteger v) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setRect(key, rect.left, rect.top, rect.left + v, rect.bottom);
	}
	KTL_INLINE bool Sprites::setRectSizeY(tTJSString const& key, tTVInteger v) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setRect(key, rect.left, rect.top, rect.right, rect.top + v);
	}
	KTL_INLINE bool Sprites::setRectSizeFromCenter(tTJSString const& key, tTVInteger v1, tTVInteger v2) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		NativeSprites::sprite_drawer_type::center_value_type center = instance_->getCenter(key).get_value_or(NativeSprites::sprite_drawer_type::center_value_type());
		NativeSprites::sprite_drawer_type::center_value_type g_center(rect.left + center.x, rect.top + center.y, FLOAT());
		FLOAT width = static_cast<FLOAT>(rect.right - rect.left);
		FLOAT height = static_cast<FLOAT>(rect.bottom - rect.top);
		FLOAT cx_rate = center.x / width;
		FLOAT cy_rate = center.y / height;
		NativeSprites::sprite_drawer_type::rect_value_type new_rect = sprig::dg::make_rect(
			static_cast<UINT>(g_center.x - v1 * cx_rate),
			static_cast<UINT>(g_center.y - v2 * cy_rate),
			static_cast<UINT>(g_center.x + v1 * (1.0 - cx_rate)),
			static_cast<UINT>(g_center.y + v2 * (1.0 - cy_rate))
			);
		NativeSprites::sprite_drawer_type::center_value_type new_center(
			(new_rect.right - new_rect.left) * cx_rate,
			(new_rect.bottom - new_rect.top) * cy_rate,
			FLOAT()
			);
		return instance_->setRect(key, new_rect) && instance_->setCenter(key, new_center);
	}
	KTL_INLINE bool Sprites::setRectSizeXFromCenter(tTJSString const& key, tTVInteger v) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		NativeSprites::sprite_drawer_type::center_value_type center = instance_->getCenter(key).get_value_or(NativeSprites::sprite_drawer_type::center_value_type());
		NativeSprites::sprite_drawer_type::center_value_type g_center(rect.left + center.x, rect.top + center.y, FLOAT());
		FLOAT width = static_cast<FLOAT>(rect.right - rect.left);
		FLOAT cx_rate = center.x / width;
		NativeSprites::sprite_drawer_type::rect_value_type new_rect = sprig::dg::make_rect(
			static_cast<UINT>(g_center.x - v * cx_rate),
			static_cast<UINT>(rect.top),
			static_cast<UINT>(g_center.x + v * (1.0 - cx_rate)),
			static_cast<UINT>(rect.bottom)
			);
		NativeSprites::sprite_drawer_type::center_value_type new_center(
			(new_rect.right - new_rect.left) * cx_rate,
			center.y,
			FLOAT()
			);
		return instance_->setRect(key, new_rect) && instance_->setCenter(key, new_center);
	}
	KTL_INLINE bool Sprites::setRectSizeYFromCenter(tTJSString const& key, tTVInteger v) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		NativeSprites::sprite_drawer_type::center_value_type center = instance_->getCenter(key).get_value_or(NativeSprites::sprite_drawer_type::center_value_type());
		NativeSprites::sprite_drawer_type::center_value_type g_center(rect.left + center.x, rect.top + center.y, FLOAT());
		FLOAT height = static_cast<FLOAT>(rect.bottom - rect.top);
		FLOAT cy_rate = center.y / height;
		NativeSprites::sprite_drawer_type::rect_value_type new_rect = sprig::dg::make_rect(
			static_cast<UINT>(rect.left),
			static_cast<UINT>(g_center.y - v * cy_rate),
			static_cast<UINT>(rect.right),
			static_cast<UINT>(g_center.y + v * (1.0 - cy_rate))
			);
		NativeSprites::sprite_drawer_type::center_value_type new_center(
			center.x,
			(new_rect.bottom - new_rect.top) * cy_rate,
			FLOAT()
			);
		return instance_->setRect(key, new_rect) && instance_->setCenter(key, new_center);
	}
	KTL_INLINE bool Sprites::setRectRegionSize(tTJSString const& key, tTVInteger v1, tTVInteger v2) {
		NativeSprites::sprite_drawer_type::scaling_value_type scaling = instance_->getScaling(key).get_value_or(NativeSprites::sprite_drawer_type::scaling_value_type());
		return setRectSizeFromCenter(key, static_cast<tTVInteger>(v1 / scaling.x), static_cast<tTVInteger>(v2 / scaling.y));
	}
	KTL_INLINE bool Sprites::setRectRegionSizeX(tTJSString const& key, tTVInteger v) {
		NativeSprites::sprite_drawer_type::scaling_value_type scaling = instance_->getScaling(key).get_value_or(NativeSprites::sprite_drawer_type::scaling_value_type());
		return setRectSizeXFromCenter(key, static_cast<tTVInteger>(v / scaling.x));
	}
	KTL_INLINE bool Sprites::setRectRegionSizeY(tTJSString const& key, tTVInteger v) {
		NativeSprites::sprite_drawer_type::scaling_value_type scaling = instance_->getScaling(key).get_value_or(NativeSprites::sprite_drawer_type::scaling_value_type());
		return setRectSizeYFromCenter(key, static_cast<tTVInteger>(v / scaling.y));
	}
	KTL_INLINE bool Sprites::setCenterByRate(tTJSString const& key, tTVReal v1, tTVReal v2, tTVReal v3) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setCenter(key, v1 * (rect.right - rect.left), v2 * (rect.bottom - rect.top), tTVReal());
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getCenterXByRate(tTJSString const& key) const {
		return getCenterX(key).get_value_or(tTVReal()) / getRectSizeX(key).get_value_or(tTVInteger());
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getCenterYByRate(tTJSString const& key) const {
		return getCenterY(key).get_value_or(tTVReal()) / getRectSizeY(key).get_value_or(tTVInteger());
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getCenterZByRate(tTJSString const& key) const {
		return tTVReal();
	}
	KTL_INLINE bool Sprites::setCenterXByRate(tTJSString const& key, tTVReal v) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setCenterX(key, v * (rect.right - rect.left));
	}
	KTL_INLINE bool Sprites::setCenterYByRate(tTJSString const& key, tTVReal v) {
		NativeSprites::sprite_drawer_type::rect_value_type rect = instance_->getRect(key).get_value_or(sprig::dg::make_rect());
		return setCenterY(key, v * (rect.bottom - rect.top));
	}
	KTL_INLINE bool Sprites::setCenterZByRate(tTJSString const& key, tTVReal v) {
		return setCenterZ(key, tTVReal());
	}
	KTL_INLINE bool Sprites::setCenterXYZByRate(tTJSString const& key, tTVReal v) {
		return setCenterByRate(key, v, v, v);
	}
	KTL_INLINE boost::optional<tTVReal> Sprites::getRotationByDegree(tTJSString const& key) const {
		boost::optional<tTVReal> result(instance_->getRotation(key));
		if (result) {
			*result = D3DXToDegree(*result);
		}
		return result;
	}
	KTL_INLINE bool Sprites::setRotationByDegree(tTJSString const& key, tTVReal v) {
		return instance_->setRotation(key, D3DXToRadian(static_cast<FLOAT>(v)));
	}
	//
	//	SUMMARY: 更新系メソッド
	//
	KTL_INLINE bool Sprites::onContinuous() {
		return SUCCEEDED(instance_->onContinuous());
	}
	KTL_INLINE bool Sprites::update() {
		return SUCCEEDED(instance_->update());
	}
	KTL_INLINE bool Sprites::requestUpdate() {
		return instance_->requestUpdate();
	}
	KTL_INLINE bool Sprites::isRequestedUpdate() {
		return instance_->isRequestedUpdate();
	}
	//
	//	SUMMARY: 描画系メソッド
	//
	KTL_INLINE bool Sprites::draw() {
		return SUCCEEDED(instance_->draw());
	}
	KTL_INLINE bool Sprites::loadImageFromLayer(tTJSString const& key, tTJSVariant const* v) {
		return SUCCEEDED(instance_->loadImageFromLayer(key, v));
	}
	KTL_INLINE bool Sprites::clearImage(tTJSString const& key) {
		return SUCCEEDED(instance_->clearImage(key));
	}
	//
	//	SUMMARY: ヒットテスト系メソッド
	//
	KTL_INLINE boost::shared_ptr<tTJSString> Sprites::hitTest(tTVInteger v1, tTVInteger v2) {
		return instance_->hitTest(NativeSprites::coord_type(static_cast<FLOAT>(v1), static_cast<FLOAT>(v2), 0.0f));
	}
	KTL_INLINE sprig::krkr::tjs::object_type Sprites::multiTargetHitTest(tTVInteger v1, tTVInteger v2) {
		return instance_->multiTargetHitTest(NativeSprites::coord_type(static_cast<FLOAT>(v1), static_cast<FLOAT>(v2), 0.0f));
	}
	KTL_INLINE sprig::krkr::tjs::object_type Sprites::multiPointHitTest(tjs_int numparams, tTJSVariant** param) {
		return instance_->multiPointHitTest(numparams, param);
	}
	KTL_INLINE sprig::krkr::tjs::object_type Sprites::multiTPHitTest(tjs_int numparams, tTJSVariant** param) {
		return instance_->multiTPHitTest(numparams, param);
	}
	//
	//	SUMMARY: 座標系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type Sprites::toLocalCoord(tTJSString const& key, tTVInteger v1, tTVInteger v2) const {
		return instance_->toLocalCoord(key, NativeSprites::coord_type(static_cast<FLOAT>(v1), static_cast<FLOAT>(v2), 0.0f));
	}
	KTL_INLINE sprig::krkr::tjs::object_type Sprites::toGlobalCoord(tTJSString const& key, tTVInteger v1, tTVInteger v2) const {
		return instance_->toGlobalCoord(key, NativeSprites::coord_type(static_cast<FLOAT>(v1), static_cast<FLOAT>(v2), 0.0f));
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tTVInteger Sprites::getClearColor() const {
		return instance_->getClearColor();
	}
	KTL_INLINE void Sprites::setClearColor(tTVInteger v) {
		instance_->setClearColor(boost::numeric_cast<NativeSprites::color_type>(v));
	}
	KTL_INLINE iTJSDispatch2* Sprites::getLayer() const {
		return sprig::get_pointer(instance_->getLayer());
	}
	KTL_INLINE void Sprites::setLayer(tTJSVariant const* v) {
		instance_->setLayer(v);
	}
	KTL_INLINE bool Sprites::getEnable() const {
		return instance_->getEnable();
	}
	KTL_INLINE void Sprites::setEnable(bool v) {
		instance_->setEnable(v);
	}
	KTL_INLINE bool Sprites::getRun() const {
		return instance_->getRun();
	}
	KTL_INLINE void Sprites::setRun(bool v) {
		instance_->setRun(v);
	}
	//
	//	SUMMARY: デバッグ系メソッド
	//
	KTL_INLINE void Sprites::outputValueInfo() const {
		instance_->outputValueInfo();
	}
	KTL_INLINE bool Sprites::getEnablePerformanceTimer() const {
		return instance_->getEnablePerformanceTimer();
	}
	KTL_INLINE void Sprites::setEnablePerformanceTimer(bool v) {
		instance_->setEnablePerformanceTimer(v);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
