/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_SERIALIZATION_SERIALIZATION_HPP
#define SRC_SERIALIZATION_SERIALIZATION_HPP

#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>
#include <ktl/serialization.hpp>

#include "SerializationCommon.hpp"
#include "SerializationDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Serialization:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Serialization:error")

//
// KTL_SERIALIZATION_TRACE_SAVE_SECTION
//
#ifndef KTL_SERIALIZATION_CONFIG_TRACE_SAVE
#	define KTL_SERIALIZATION_TRACE_SAVE_SECTION(NAME)
#else	// #ifndef KTL_SERIALIZATION_CONFIG_TRACE_SAVE
#	define KTL_SERIALIZATION_TRACE_SAVE_SECTION(NAME) SPRIG_KRKR_SECTION((NAME), SPRIG_KRKR_LOG_LEVEL_NORMAL)
#endif	// #ifndef KTL_SERIALIZATION_CONFIG_TRACE_SAVE
//
// KTL_SERIALIZATION_TRACE_LOAD_SECTION
//
#ifndef KTL_SERIALIZATION_CONFIG_TRACE_LOAD
#	define KTL_SERIALIZATION_TRACE_LOAD_SECTION(NAME)
#else	// #ifndef KTL_SERIALIZATION_CONFIG_TRACE_LOAD
#	define KTL_SERIALIZATION_TRACE_LOAD_SECTION(NAME) SPRIG_KRKR_SECTION((NAME), SPRIG_KRKR_LOG_LEVEL_NORMAL)
#endif	// #ifndef KTL_SERIALIZATION_CONFIG_TRACE_LOAD

namespace ktl {
	//
	// NativeSerializationObject
	//
	NativeSerializationObject::NativeSerializationObject() {}
	//
	//	SUMMARY: シリアライザ管理系メソッド
	//
	KTL_INLINE bool NativeSerializationObject::insert(
		key_type const& key,
		tTJSVariant const* v
		)
	{
		if (exist(key)) {
			KTL_WARNING(
				KTL_WARNING_SECTION,
				SPRIG_KRKR_TJS_W("既に登録されたシリアライザを上書きします:") + key
				);
		}
		tTJSVariantClosure const& closure = v->AsObjectClosureNoAddRef();
		ISerializer* ser = getSerializerInstance(closure);
		if (ser) {
			// COMMENT: シリアライザオブジェクトを取得できた場合
			serializer_map_[key] = boost::make_shared<NativeSerializer>(
				closure.Object,
				closure.ObjThis,
				ser
				);
			return true;
		} else {
			// COMMENT: シリアライザオブジェクトを取得できない場合
			serializer_map_[key] = boost::make_shared<ScriptSerializer>(
				closure.Object,
				closure.ObjThis
				);
			return true;
		}
		return false;
	}
	KTL_INLINE NativeSerializationObject::serializer_map_type::size_type NativeSerializationObject::erase(key_type const& key) {
		return serializer_map_.erase(key);
	}
	KTL_INLINE void NativeSerializationObject::clear() {
		serializer_map_.clear();
	}
	KTL_INLINE NativeSerializationObject::serializer_map_type::size_type NativeSerializationObject::size() const {
		return serializer_map_.size();
	}
	KTL_INLINE NativeSerializationObject::serializer_map_type::const_iterator NativeSerializationObject::begin() const {
		return serializer_map_.begin();
	}
	KTL_INLINE NativeSerializationObject::serializer_map_type::iterator NativeSerializationObject::begin() {
		return serializer_map_.begin();
	}
	KTL_INLINE NativeSerializationObject::serializer_map_type::const_iterator NativeSerializationObject::end() const {
		return serializer_map_.end();
	}
	KTL_INLINE NativeSerializationObject::serializer_map_type::iterator NativeSerializationObject::end() {
		return serializer_map_.end();
	}
	KTL_INLINE NativeSerializationObject::serializer_map_type::const_iterator NativeSerializationObject::find(key_type const& key) const {
		return serializer_map_.find(key);
	}
	KTL_INLINE NativeSerializationObject::serializer_map_type::iterator NativeSerializationObject::find(key_type const& key) {
		return serializer_map_.find(key);
	}
	KTL_INLINE bool NativeSerializationObject::exist(key_type const& key) const {
		return serializer_map_.find(key) != serializer_map_.end();
	}
	KTL_INLINE NativeSerializationObject::optional_value_type NativeSerializationObject::findValue(
		key_type const& key
		) const
	{
		serializer_map_type::const_iterator i = serializer_map_.find(key);
		if (i == serializer_map_.end()) {
			return boost::none;
		}
		return *i;
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	KTL_INLINE void NativeSerializationObject::save(
		tTJSVariant const* ar_v,
		key_type const& key,
		tTJSVariant const* v,
		boost::optional<key_type> ser_key
		)
	{
		ScopedStackCache cache(stack_cache_manager_, *ar_v);
		ISerializationStack* stack = cache.get();
		if (!stack->set(key, v, ser_key.get_ptr())) {
			tTJSString ser;
			tTJSVariantClosure const& closure = v->AsObjectClosureNoAddRef();
			if (ser_key) {
				ser = *ser_key;
			} else {
				tTJSVariant ser_v;
				if (TJS_FAILED(closure.Object->ClassInstanceInfo(TJS_CII_GET, 0, &ser_v))) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("インスタンス情報を取得できません"),
						sprig::krkr::bad_argument
						);
				}
				ser = ser_v;
			}
			{
				KTL_SERIALIZATION_TRACE_SAVE_SECTION(SPRIG_KRKR_TJS_W("KTL:Serialization:Trace:Save:") + ser);
				optional_value_type value = findValue(ser);
				if (!value) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("対応するシリアライザが登録されていません:") + ser,
						sprig::krkr::bad_argument
						);
				}
				serializer_type const& serializer = value->second;
				stack->pushOnSave(key, v, ser, serializer->version(), serializer->identity());
				{
					tTJSVariant param[] = {
						*ar_v,
						*v
					};
					tTJSVariant* p_param[] = {
						param + 0,
						param + 1
					};
					serializer->save(0, 2, p_param);
				}
				stack->popOnSave();
			}
		}
	}
	KTL_INLINE void NativeSerializationObject::load(
		tTJSVariant const* ar_v,
		key_type const& key,
		tTJSVariant const* v,
		tTJSVariant* result_v
		)
	{
		ScopedStackCache cache(stack_cache_manager_, *ar_v);
		ISerializationStack* stack = cache.get();
		if (stack->isLeaf(key)) {
			stack->get(key, result_v);
		} else {
			ISerializationStack::key_type ser = stack->serializer(key);
			ISerializationStack::version_type ver = stack->version(key);
			ISerializationStack::key_type id = stack->identity(key);
			{
				KTL_SERIALIZATION_TRACE_LOAD_SECTION(SPRIG_KRKR_TJS_W("KTL:Serialization:Trace:Load:") + ser);
				stack->pushOnLoad(key);
				tTJSVariant res_v;
				{
					optional_value_type value = findValue(ser);
					if (!value) {
						KTL_ERROR(
							KTL_ERROR_SECTION,
							SPRIG_KRKR_TJS_W("対応するシリアライザが登録されていません:") + ser,
							sprig::krkr::bad_argument
							);
					}
					serializer_type const& serializer = value->second;
					tTJSVariant param[] = {
						*ar_v,
						v ? *v : tTJSVariant(),
						ver,
						id
					};
					tTJSVariant* p_param[] = {
						param + 0,
						param + 1,
						param + 2,
						param + 3
					};
					serializer->load(&res_v, 4, p_param);
					if (result_v) {
						*result_v = res_v;
					}
				}
				stack->popOnLoad(&res_v);
			}
		}
	}

	//
	// SerializationObject
	//
	SerializationObject::SerializationObject() {}
	tjs_error TJS_INTF_METHOD SerializationObject::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("SerializationObject::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeSerializationObject>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD SerializationObject::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("SerializationObject::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: シリアライザ管理系メソッド
	//
	KTL_INLINE bool SerializationObject::insert(
		tTJSString const& key,
		tTJSVariant const* v
		)
	{
		return instance_->insert(key, v);
	}
	KTL_INLINE tTVInteger SerializationObject::erase(tTJSString const& key) {
		return instance_->erase(key);
	}
	KTL_INLINE void SerializationObject::clear() {
		instance_->clear();
	}
	KTL_INLINE tTVInteger SerializationObject::size() const {
		return instance_->size();
	}
	KTL_INLINE bool SerializationObject::exist(tTJSString const& key) const {
		return instance_->exist(key);
	}
	//
	//	SUMMARY: シリアライズ系メソッド
	//
	KTL_INLINE void SerializationObject::save(
		tTJSVariant const* ar_v,
		tTJSString const& key,
		tTJSVariant const* v,
		boost::optional<tTJSString> ser_key
		)
	{
		if (ser_key) {
			instance_->save(ar_v, key, v, *ser_key);
		} else {
			instance_->save(ar_v, key, v);
		}
	}
	KTL_INLINE void SerializationObject::load(
		tTJSVariant const* ar_v,
		tTJSString const& key,
		tTJSVariant const* v,
		tTJSVariant* result_v
		)
	{
		instance_->load(ar_v, key, v, result_v);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_SERIALIZATION_SERIALIZATION_HPP
