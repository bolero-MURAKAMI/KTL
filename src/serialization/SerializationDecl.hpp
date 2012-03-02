#pragma once

#include <boost/optional/optional.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>

#include "SerializationCommon.hpp"

//
// KTL_SERIALIZATION_MAP
//
#ifndef KTL_SERIALIZATION_CONFIG_USING_UNORDERED_MAP
#	include <map>
#	define KTL_SERIALIZATION_MAP std::map
#else	// #ifndef KTL_SERIALIZATION_CONFIG_USING_UNORDERED_MAP
#	include <boost/unordered/unordered_map.hpp>
#	define KTL_SERIALIZATION_MAP boost::unordered_map
#endif	// #ifndef KTL_SERIALIZATION_CONFIG_USING_UNORDERED_MAP

namespace ktl {
	//
	// NativeSerializationObject
	//
	class NativeSerializationObject {
	public:
		typedef tTJSString string_type;
		typedef boost::shared_ptr<ISerializer> serializer_type;
		typedef KTL_SERIALIZATION_MAP<
			string_type,
			serializer_type
		> serializer_map_type;
		typedef serializer_map_type::value_type value_type;
		typedef serializer_map_type::key_type key_type;
		typedef serializer_map_type::mapped_type mapped_type;
		typedef serializer_map_type::size_type size_type;
		typedef serializer_map_type::iterator iterator;
		typedef serializer_map_type::const_iterator const_iterator;
		typedef boost::optional<value_type const&> optional_value_type;
	private:
		serializer_map_type serializer_map_;
		StackCacheManager stack_cache_manager_;
	public:
		NativeSerializationObject();
	public:
		//
		//	SUMMARY: シリアライザ管理系メソッド
		//
		bool insert(
			key_type const& key,
			tTJSVariant const* v
			);
		size_type erase(key_type const& key);
		void clear();
		size_type size() const;
		const_iterator begin() const;
		iterator begin();
		const_iterator end() const;
		iterator end();
		const_iterator find(key_type const& key) const;
		iterator find(key_type const& key);
		bool exist(key_type const& key) const;
		optional_value_type findValue(key_type const& key) const;
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		void save(
			tTJSVariant const* ar_v,
			key_type const& key,
			tTJSVariant const* v,
			boost::optional<key_type const&> ser_key = boost::none
			);
		void load(
			tTJSVariant const* ar_v,
			key_type const& key,
			tTJSVariant const* v,
			tTJSVariant* result_v
			);
	};

	//
	// SerializationObject
	//
	class SerializationObject
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeSerializationObject> instance_;
	public:
		SerializationObject();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
		//
		//	SUMMARY: シリアライザ管理系メソッド
		//
		bool insert(
			tTJSString const& key,
			tTJSVariant const* v
			);
		tTVInteger erase(tTJSString const& key);
		void clear();
		tTVInteger size() const;
		bool exist(tTJSString const& key) const;
		//
		//	SUMMARY: シリアライズ系メソッド
		//
		void save(
			tTJSVariant const* ar_v,
			tTJSString const& key,
			tTJSVariant const* v,
			boost::optional<tTJSString const&> ser_key = boost::none
			);
		void load(
			tTJSVariant const* ar_v,
			tTJSString const& key,
			tTJSVariant const* v,
			tTJSVariant* result_v
			);
	};
}	// namespace ktl
