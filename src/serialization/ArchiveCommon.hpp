#pragma once

#include "Config.hpp"

#include <cstring>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/utility/swap.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/exception.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/transfer.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <ktl/error.hpp>
#include <ktl/variant_types.hpp>
#include <ktl/serialization.hpp>

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Archive:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Archive:error")

//
// KTL_SERIALIZATION_AT
//
#ifndef KTL_SERIALIZATION_CONFIG_USING_SUBSCRIPT_OPERATOR
#	define KTL_SERIALIZATION_AT at
#else	// #ifndef KTL_SERIALIZATION_CONFIG_USING_SUBSCRIPT_OPERATOR
#	define KTL_SERIALIZATION_AT operator[]
#endif	// #ifndef KTL_SERIALIZATION_CONFIG_USING_SUBSCRIPT_OPERATOR

namespace ktl {
	class DataElement;
	class BaseElement;
	class ObjectElement;
	class SerializerElement;
	class ElementMap;

	//
	// DataElement
	//
	class DataElement {
	public:
		typedef sprig::krkr::tjs::intptr_type id_type;
		typedef id_type index_type;
		typedef std::basic_string<tjs_char> string_type;
		typedef std::vector<tjs_uint8> octet_type;
	public:
		static VariantTypes::value_type const tvtVoid = tvtVoid;
		static VariantTypes::value_type const tvtObject = tvtObject;
		static VariantTypes::value_type const tvtString = tvtString;
		static VariantTypes::value_type const tvtOctet = tvtOctet;
		static VariantTypes::value_type const tvtInteger = tvtInteger;
		static VariantTypes::value_type const tvtReal = tvtReal;
		static VariantTypes::value_type const tvtBaseObject = -1;
	private:
		VariantTypes::value_type type_;
		tTJSVariant value_;
	public:
		DataElement()
			: type_(tvtVoid)
		{}
		explicit DataElement(tTJSVariant const& v)
			: type_(v.Type())
			, value_(v)
		{}
		explicit DataElement(index_type index)
			: type_(tvtBaseObject)
			, value_(index)
		{}
		id_type getID() const {
			return type_ == tvtObject
				? static_cast<id_type>(value_)
				: 0
				;
		}
		bool hasIndex() const {
			return type_ == tvtBaseObject;
		}
		index_type getIndex() const {
			return static_cast<index_type>(value_);
		}
		void get(tTJSVariant& v) const {
			switch (type_) {
			case tvtVoid:
				v.Clear();
				break;
			case tvtObject:
				v = static_cast<iTJSDispatch2*>(0);
				break;
			case tvtString:
			case tvtInteger:
			case tvtReal:
			case tvtOctet:
				v = value_;
				break;
			case tvtBaseObject:
				SPRIG_ASSERT_EXCEPTION(0 && "case tvtBaseObject: in function "__FUNCTION__);
				break;
			default:
				SPRIG_ASSERT_EXCEPTION(0 && "default: in function "__FUNCTION__);
			}
		}
	private:
		friend class boost::serialization::access;
		BOOST_SERIALIZATION_SPLIT_MEMBER();
		template<typename Archive>
		void save(Archive& ar, unsigned int const) const {
			ar & boost::serialization::make_nvp("type", type_);
			switch (type_) {
			case tvtVoid:
				break;
			case tvtObject:
				{
					id_type id = reinterpret_cast<id_type>(value_.AsObjectNoAddRef());
					ar & boost::serialization::make_nvp("val", id);
				}
				break;
			case tvtString:
			case tvtInteger:
			case tvtReal:
				{
					string_type string = static_cast<tTJSString>(value_).c_str();
					ar & boost::serialization::make_nvp("val", string);
				}
				break;
			case tvtOctet:
				{
					tTJSVariantOctet* v_octet = value_.AsOctetNoAddRef();
					tjs_uint const length = v_octet->GetLength();
					tjs_uint8 const* const data = v_octet->GetData();
					octet_type octet(length);
					if (length) {
						std::copy(
							data,
							data + length,
							&octet[0]
							);
					}
					ar & boost::serialization::make_nvp("val", octet);
				}
				break;
			case tvtBaseObject:
				{
					index_type index = static_cast<index_type>(value_);
					ar & boost::serialization::make_nvp("val", index);
				}
				break;
			default:
				SPRIG_ASSERT_EXCEPTION(0 && "default: in function "__FUNCTION__);
			}
		}
		template<typename Archive>
		void load(Archive& ar, unsigned int const) {
			ar & boost::serialization::make_nvp("type", type_);
			switch (type_) {
			case tvtVoid:
				break;
			case tvtObject:
				{
					id_type id;
					ar & boost::serialization::make_nvp("val", id);
					value_ = id;
				}
				break;
			case tvtString:
				{
					string_type string;
					ar & boost::serialization::make_nvp("val", string);
					value_ = string.c_str();
				}
				break;
			case tvtInteger:
				{
					string_type string;
					ar & boost::serialization::make_nvp("val", string);
					value_ = string.c_str();
					value_.ToInteger();
				}
				break;
			case tvtReal:
				{
					string_type string;
					ar & boost::serialization::make_nvp("val", string);
					value_ = string.c_str();
					value_.ToReal();
				}
				break;
			case tvtOctet:
				{
					octet_type octet;
					ar & boost::serialization::make_nvp("val", octet);
					value_ = tTJSVariant(octet.empty() ? 0 : &octet[0], octet.size());
				}
				break;
			case tvtBaseObject:
				{
					index_type index;
					ar & boost::serialization::make_nvp("val", index);
					value_ = index;
				}
				break;
			default:
				SPRIG_ASSERT_EXCEPTION(0 && "default: in function "__FUNCTION__);
			}
		}
	private:
		template<typename Key>
		static Key const& getKeyType() {
			static Key const key = SPRIG_KRKR_TJS_W("type");
			return key;
		}
		template<typename Key>
		static Key const& getKeyVal() {
			static Key const key = SPRIG_KRKR_TJS_W("val");
			return key;
		}
	public:
		template<typename Ptree>
		void savePt(Ptree& pt) const {
			typedef typename Ptree::value_type pt_value_type;
			typedef typename Ptree::key_type pt_key_type;
			typedef typename Ptree::data_type pt_data_type;

			Ptree& type = pt.put_child(getKeyType<pt_key_type>(), Ptree());
			Ptree& val = pt.put_child(getKeyVal<pt_key_type>(), Ptree());

			// type
			type.data() = sprig::str_cast<pt_data_type>(type_);
			// val
			switch (type_) {
			case tvtVoid:
				break;
			case tvtObject:
				{
					id_type id = reinterpret_cast<id_type>(value_.AsObjectNoAddRef());
					val.data() = sprig::str_cast<pt_data_type>(id);
				}
				break;
			case tvtString:
			case tvtInteger:
			case tvtReal:
				{
					string_type string = static_cast<tTJSString>(value_).c_str();
					val.data() = string;
				}
				break;
			case tvtOctet:
				{
					tTJSVariantOctet* v_octet = value_.AsOctetNoAddRef();
					boost::intrusive_ptr<tTJSVariantString> oct(
						::TJSOctetToListString(v_octet),
						false
						);
					string_type string = oct
						? sprig::krkr::tjs::as_c_str(oct.get())
						: string_type()
						;
					val.data() = string;
				}
				break;
			case tvtBaseObject:
				{
					index_type index = static_cast<index_type>(value_);
					val.data() = sprig::str_cast<pt_data_type>(index);
				}
				break;
			default:
				SPRIG_ASSERT_EXCEPTION(0 && "default: in function "__FUNCTION__);
			}
		}
		template<typename Ptree>
		void loadPt(Ptree& pt) {
			typedef typename Ptree::value_type pt_value_type;
			typedef typename Ptree::key_type pt_key_type;
			typedef typename Ptree::data_type pt_data_type;

			Ptree& type = pt.get_child(getKeyType<pt_key_type>());
			Ptree& val = pt.get_child(getKeyVal<pt_key_type>());

			// type
			VariantTypes::value_type new_type = boost::lexical_cast<VariantTypes::value_type>(type.data());
			// val
			tTJSVariant value;
			switch (new_type) {
			case tvtVoid:
				break;
			case tvtObject:
				{
					id_type id = boost::lexical_cast<id_type>(val.data());
					value = id;
				}
				break;
			case tvtString:
				{
					string_type string = val.data();
					value = string.c_str();
				}
				break;
			case tvtInteger:
				{
					string_type string = val.data();
					value = string.c_str();
					value.ToInteger();
				}
				break;
			case tvtReal:
				{
					string_type string = val.data();
					value = string.c_str();
					value.ToReal();
				}
				break;
			case tvtOctet:
				{
					std::basic_istringstream<tjs_char> is(val.data());
					is >> std::hex;
					octet_type octet;
					std::copy(
						std::istream_iterator<sprig::transfer<tjs_uint8, tjs_uint16>, tjs_char>(is),
						std::istream_iterator<sprig::transfer<tjs_uint8, tjs_uint16>, tjs_char>(),
						std::back_inserter(octet)
						);
					value = tTJSVariant(octet.empty() ? 0 : &octet[0], octet.size());
				}
				break;
			case tvtBaseObject:
				{
					index_type index = boost::lexical_cast<id_type>(val.data());
					value = index;
				}
				break;
			default:
				SPRIG_ASSERT_EXCEPTION(0 && "default: in function "__FUNCTION__);
			}

			boost::swap(type_, new_type);
			boost::swap(value_, value);
		}
	};

	//
	// BaseElement
	//
	class BaseElement {
	public:
		typedef ISerializationStack::key_type key_type;
		typedef ISerializationStack::version_type version_type;
		typedef std::map<
			ISerializationStack::key_type,
			DataElement
		> data_map_type;
	private:
		data_map_type data_map_;
		key_type serializer_;
	public:
		BaseElement() {}
		explicit BaseElement(key_type const& ser)
			: serializer_(ser)
		{}
		data_map_type const& getDataMap() const {
			return data_map_;
		}
		data_map_type& getDataMap() {
			return data_map_;
		}
		key_type const& getSerializer() const {
			return serializer_;
		}
		key_type& getSerializer() {
			return serializer_;
		}
	private:
		friend class boost::serialization::access;
		template <typename Archive>
		void serialize(Archive& ar, unsigned int const) {
			ar
				& boost::serialization::make_nvp("dat", data_map_)
				& boost::serialization::make_nvp("ser", serializer_)
				;
		}
	private:
		template<typename Key>
		static Key const& getKeyDat() {
			static Key const key = SPRIG_KRKR_TJS_W("dat");
			return key;
		}
		template<typename Key>
		static Key const& getKeySer() {
			static Key const key = SPRIG_KRKR_TJS_W("ser");
			return key;
		}
	public:
		template<typename Ptree>
		void savePt(Ptree& pt) const {
			typedef typename Ptree::value_type pt_value_type;
			typedef typename Ptree::key_type pt_key_type;
			typedef typename Ptree::data_type pt_data_type;

			Ptree& dat = pt.put_child(getKeyDat<pt_key_type>(), Ptree());
			Ptree& ser = pt.put_child(getKeySer<pt_key_type>(), Ptree());

			// dat
			BOOST_FOREACH(data_map_type::value_type const& e, getDataMap()) {
				Ptree& elem = dat.put_child(sprig::str_cast<pt_key_type>(e.first), Ptree());
				e.second.savePt(elem);
			}
			// ser
			ser.data() = sprig::str_cast<pt_data_type>(getSerializer());
		}
		template<typename Ptree>
		void loadPt(Ptree& pt) {
			typedef typename Ptree::value_type pt_value_type;
			typedef typename Ptree::key_type pt_key_type;
			typedef typename Ptree::data_type pt_data_type;

			Ptree& dat = pt.get_child(getKeyDat<pt_key_type>());
			Ptree& ser = pt.get_child(getKeySer<pt_key_type>());

			// dat
			data_map_type data_map;
			BOOST_FOREACH(pt_value_type const& e, dat) {
				DataElement elem;
				elem.loadPt(e.second);
				data_map[sprig::str_cast<data_map_type::key_type>(e.first)] = elem;
			}
			// ser
			key_type serializer = sprig::str_cast<key_type>(ser.data());

			boost::swap(getDataMap(), data_map);
			boost::swap(getSerializer(), serializer);
		}
	};

	//
	// ObjectElement
	//
	class ObjectElement {
	public:
		typedef ISerializationStack::key_type key_type;
		typedef ISerializationStack::version_type version_type;
		typedef BaseElement::data_map_type data_map_type;
		typedef std::vector<
			BaseElement
		> base_list_type;
	private:
		BaseElement object_;
		base_list_type base_list_;
	public:
		ObjectElement() {}
		explicit ObjectElement(key_type const& ser)
			: object_(ser)
		{}
		data_map_type const& getDataMap() const {
			return object_.getDataMap();
		}
		data_map_type& getDataMap() {
			return object_.getDataMap();
		}
		key_type const& getSerializer() const {
			return object_.getSerializer();
		}
		key_type& getSerializer() {
			return object_.getSerializer();
		}
		base_list_type const& getBaseList() const {
			return base_list_;
		}
		base_list_type& getBaseList() {
			return base_list_;
		}
	private:
		friend class boost::serialization::access;
		template <typename Archive>
		void serialize(Archive& ar, unsigned int const) {
			ar
				& boost::serialization::make_nvp("dat", getDataMap())
				& boost::serialization::make_nvp("ser", getSerializer())
				& boost::serialization::make_nvp("base", base_list_)
				;
		}
	private:
		template<typename Key>
		static Key const& getKeyDat() {
			static Key const key = SPRIG_KRKR_TJS_W("dat");
			return key;
		}
		template<typename Key>
		static Key const& getKeySer() {
			static Key const key = SPRIG_KRKR_TJS_W("ser");
			return key;
		}
		template<typename Key>
		static Key const& getKeyBase() {
			static Key const key = SPRIG_KRKR_TJS_W("base");
			return key;
		}
	public:
		template<typename Ptree>
		void savePt(Ptree& pt) const {
			typedef typename Ptree::value_type pt_value_type;
			typedef typename Ptree::key_type pt_key_type;
			typedef typename Ptree::data_type pt_data_type;

			Ptree& dat = pt.put_child(getKeyDat<pt_key_type>(), Ptree());
			Ptree& ser = pt.put_child(getKeySer<pt_key_type>(), Ptree());
			Ptree& base = pt.put_child(getKeyBase<pt_key_type>(), Ptree());

			// dat
			BOOST_FOREACH(data_map_type::value_type const& e, getDataMap()) {
				Ptree& elem = dat.put_child(sprig::str_cast<pt_key_type>(e.first), Ptree());
				e.second.savePt(elem);
			}
			// ser
			ser.data() = sprig::str_cast<pt_data_type>(getSerializer());
			// base
			{
				pt_key_type prefix = SPRIG_KRKR_TJS_W("_");
				for (base_list_type::size_type i = 0, last = base_list_.size(); i != last; ++i) {
					Ptree& elem = base.put_child(
						prefix + sprig::str_cast<pt_key_type>(i),
						Ptree()
						);
					base_list_[i].savePt(elem);
				}
			}
		}
		template<typename Ptree>
		void loadPt(Ptree& pt) {
			typedef typename Ptree::value_type pt_value_type;
			typedef typename Ptree::key_type pt_key_type;
			typedef typename Ptree::data_type pt_data_type;

			Ptree& dat = pt.get_child(getKeyDat<pt_key_type>());
			Ptree& ser = pt.get_child(getKeySer<pt_key_type>());
			Ptree& base = pt.get_child(getKeyBase<pt_key_type>());

			// dat
			data_map_type data_map;
			BOOST_FOREACH(pt_value_type const& e, dat) {
				DataElement elem;
				elem.loadPt(e.second);
				data_map[sprig::str_cast<data_map_type::key_type>(e.first)] = elem;
			}
			// ser
			key_type serializer = sprig::str_cast<key_type>(ser.data());
			// base
			base_list_type base_list(base.size());
			BOOST_FOREACH(pt_value_type const& e, base) {
				if (e.first.size() < 2) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("オブジェクトマップのキーが無効な形式です:") + e.first,
						sprig::krkr::bad_argument
						);
				}
				BaseElement elem;
				elem.loadPt(e.second);
				base_list.at(
					boost::lexical_cast<base_list_type::size_type>(
						e.first.c_str() + 1
						)
					) = elem;
			}

			boost::swap(getDataMap(), data_map);
			boost::swap(getSerializer(), serializer);
			boost::swap(base_list_, base_list);
		}
	};

	//
	// SerializerElement
	//
	class SerializerElement {
	public:
		typedef ISerializationStack::key_type key_type;
		typedef ISerializationStack::version_type version_type;
	private:
		version_type version_;
		key_type identity_;
	public:
		SerializerElement()
			: version_()
		{}
		SerializerElement(version_type ver, key_type const& id)
			: version_(ver)
			, identity_(id)
		{}
		version_type const& getVersion() const {
			return version_;
		}
		version_type& getVersion() {
			return version_;
		}
		key_type const& getIdentity() const {
			return identity_;
		}
		key_type& getIdentity() {
			return identity_;
		}
	private:
		friend class boost::serialization::access;
		template <typename Archive>
		void serialize(Archive& ar, unsigned int const) {
			ar
				& boost::serialization::make_nvp("ver", version_)
				& boost::serialization::make_nvp("id", identity_)
				;
		}
	private:
		template<typename Key>
		static Key const& getKeyVer() {
			static Key const key = SPRIG_KRKR_TJS_W("ver");
			return key;
		}
		template<typename Key>
		static Key const& getKeyID() {
			static Key const key = SPRIG_KRKR_TJS_W("id");
			return key;
		}
	public:
		template<typename Ptree>
		void savePt(Ptree& pt) const {
			typedef typename Ptree::value_type pt_value_type;
			typedef typename Ptree::key_type pt_key_type;
			typedef typename Ptree::data_type pt_data_type;

			Ptree& ver = pt.put_child(getKeyVer<pt_key_type>(), Ptree());
			Ptree& id = pt.put_child(getKeyID<pt_key_type>(), Ptree());

			// ver
			ver.data() = sprig::str_cast<pt_data_type>(version_);
			// id
			id.data() = sprig::str_cast<pt_data_type>(identity_);
		}
		template<typename Ptree>
		void loadPt(Ptree& pt) {
			typedef typename Ptree::value_type pt_value_type;
			typedef typename Ptree::key_type pt_key_type;
			typedef typename Ptree::data_type pt_data_type;

			Ptree& ver = pt.get_child(getKeyVer<pt_key_type>());
			Ptree& id = pt.get_child(getKeyID<pt_key_type>());

			// ver
			version_type version = boost::lexical_cast<version_type>(ver.data());
			// id
			key_type identity = sprig::str_cast<key_type>(id.data());

			boost::swap(version_, version);
			boost::swap(identity_, identity);
		}
	};

	//
	// ElementMap
	//
	class ElementMap {
	public:
		typedef ISerializationStack::key_type key_type;
		typedef ISerializationStack::version_type version_type;
		typedef BaseElement::data_map_type data_map_type;
		typedef ObjectElement::base_list_type base_list_type;
		typedef std::map<
			DataElement::id_type,
			ObjectElement
		> object_map_type;
		typedef std::map<
			key_type,
			SerializerElement
		> serializer_map_type;
	public:
		static ISerializationStack::version_type const static_version = 0;
	private:
		version_type version_;
		data_map_type data_map_;
		object_map_type object_map_;
		serializer_map_type serializer_map_;
	public:
		ElementMap()
			: version_(static_version)
		{}
		version_type const getVersion() const {
			return version_;
		}
		data_map_type const& getDataMap() const {
			return data_map_;
		}
		data_map_type& getDataMap() {
			return data_map_;
		}
		object_map_type const& getObjectMap() const {
			return object_map_;
		}
		object_map_type& getObjectMap() {
			return object_map_;
		}
		serializer_map_type const& getSerializerMap() const {
			return serializer_map_;
		}
		serializer_map_type& getSerializerMap() {
			return serializer_map_;
		}
	private:
		friend class boost::serialization::access;
		template <typename Archive>
		void serialize(Archive& ar, unsigned int const) {
			ar
				& boost::serialization::make_nvp("ver", version_)
				& boost::serialization::make_nvp("dat", data_map_)
				& boost::serialization::make_nvp("obj", object_map_)
				& boost::serialization::make_nvp("ser", serializer_map_)
				;
		}
	private:
		template<typename Key>
		static Key const& getKeyVer() {
			static Key const key = SPRIG_KRKR_TJS_W("ver");
			return key;
		}
		template<typename Key>
		static Key const& getKeyDat() {
			static Key const key = SPRIG_KRKR_TJS_W("dat");
			return key;
		}
		template<typename Key>
		static Key const& getKeyObj() {
			static Key const key = SPRIG_KRKR_TJS_W("obj");
			return key;
		}
		template<typename Key>
		static Key const& getKeySer() {
			static Key const key = SPRIG_KRKR_TJS_W("ser");
			return key;
		}
	public:
		template<typename Ptree>
		void savePt(Ptree& pt) const {
			typedef typename Ptree::value_type pt_value_type;
			typedef typename Ptree::key_type pt_key_type;
			typedef typename Ptree::data_type pt_data_type;

			Ptree& ver = pt.put_child(getKeyVer<pt_key_type>(), Ptree());
			Ptree& dat = pt.put_child(getKeyDat<pt_key_type>(), Ptree());
			Ptree& obj = pt.put_child(getKeyObj<pt_key_type>(), Ptree());
			Ptree& ser = pt.put_child(getKeySer<pt_key_type>(), Ptree());

			// ver
			ver.data() = sprig::str_cast<pt_data_type>(version_);
			// dat
			BOOST_FOREACH(data_map_type::value_type const& e, data_map_) {
				Ptree& elem = dat.put_child(sprig::str_cast<pt_key_type>(e.first), Ptree());
				e.second.savePt(elem);
			}
			// obj
			{
				pt_key_type prefix = SPRIG_KRKR_TJS_W("_");
				BOOST_FOREACH(object_map_type::value_type const& e, object_map_) {
					Ptree& elem = obj.put_child(
						prefix + sprig::str_cast<pt_key_type>(e.first),
						Ptree()
						);
					e.second.savePt(elem);
				}
			}
			// ser
			BOOST_FOREACH(serializer_map_type::value_type const& e, serializer_map_) {
				Ptree& elem = ser.put_child(sprig::str_cast<pt_key_type>(e.first), Ptree());
				e.second.savePt(elem);
			}
		}
		template<typename Ptree>
		void loadPt(Ptree& pt) {
			typedef typename Ptree::value_type pt_value_type;
			typedef typename Ptree::key_type pt_key_type;
			typedef typename Ptree::data_type pt_data_type;

			Ptree& ver = pt.get_child(getKeyVer<pt_key_type>());
			Ptree& dat = pt.get_child(getKeyDat<pt_key_type>());
			Ptree& obj = pt.get_child(getKeyObj<pt_key_type>());
			Ptree& ser = pt.get_child(getKeySer<pt_key_type>());

			// ver
			version_type version = boost::lexical_cast<version_type>(ver.data());
			// dat
			data_map_type data_map;
			BOOST_FOREACH(pt_value_type const& e, dat) {
				DataElement elem;
				elem.loadPt(e.second);
				data_map[sprig::str_cast<data_map_type::key_type>(e.first)] = elem;
			}
			// obj
			object_map_type object_map;
			BOOST_FOREACH(pt_value_type const& e, obj) {
				if (e.first.size() < 2) {
					KTL_ERROR(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("オブジェクトマップのキーが無効な形式です:") + e.first,
						sprig::krkr::bad_argument
						);
				}
				ObjectElement elem;
				elem.loadPt(e.second);
				object_map[
					boost::lexical_cast<object_map_type::key_type>(
						e.first.c_str() + 1
						)
					] = elem;
			}
			// ser
			serializer_map_type serializer_map;
			BOOST_FOREACH(pt_value_type const& e, ser) {
				SerializerElement elem;
				elem.loadPt(e.second);
				serializer_map[sprig::str_cast<serializer_map_type::key_type>(e.first)] = elem;
			}

			boost::swap(version_, version);
			boost::swap(data_map_, data_map);
			boost::swap(object_map_, object_map);
			boost::swap(serializer_map_, serializer_map);
		}
	};

	//
	// ElementStack
	//
	class ElementStack
		: public ISerializationStack
	{
	public:
		typedef ISerializationStack::key_type key_type;
		typedef ISerializationStack::version_type version_type;
		typedef BaseElement::data_map_type data_map_type;
		typedef ObjectElement::base_list_type base_list_type;
		typedef ElementMap::object_map_type object_map_type;
		typedef std::vector<
			DataElement::index_type
		> index_stack_type;
		typedef std::pair<
			DataElement::id_type,
			index_stack_type
		> instance_stack_element_type;
		typedef std::vector<
			instance_stack_element_type
		> instance_stack_type;
		typedef std::map<
			DataElement::id_type,
			tTJSVariant
		> instance_map_type;
		typedef data_map_type::size_type size_type;
	private:
		ElementMap element_map_;
		instance_stack_type instance_stack_;
		instance_map_type instance_map_;
	private:
		//	COMMENT: オブジェクトIDを返す
		//		: リーフ値に対しては常に0を返す
		DataElement::id_type getID(tTJSVariant const* v) {
			return v->Type() == tvtObject
				? reinterpret_cast<DataElement::id_type>(v->AsObjectNoAddRef())
				: 0;
		}
		//	COMMENT: 指定されたIDが既に登録されているか
		bool existObject(DataElement::id_type id) {
			return element_map_.getObjectMap().count(id) != 0;
		}
		//	COMMENT: 指定されたIDが現在シリアライズ中のクラスに一致するか
		bool isCurrentInstance(DataElement::id_type id) {
			return !instance_stack_.empty() && id == currentInstanceID();
		}
		//	COMMENT: 指定されたIDの、基底クラスリストの次のインデックスを返す
		//		: IDが既に登録されていることが前提
		DataElement::index_type nextBaseListIndex(DataElement::id_type id) {
			return element_map_.getObjectMap()[id].getBaseList().size();
		}
		//	COMMENT: 現在シリアライズ中のクラスのIDを返す
		//		: インスタンススタックが空でないことが前提
		DataElement::id_type& currentInstanceID() {
			return instance_stack_.back().first;
		}
		//	COMMENT: 現在シリアライズ中のクラスのインデックススタックを返す
		//		: インスタンススタックが空でないことが前提
		index_stack_type& currentInstanceIndexStack() {
			return instance_stack_.back().second;
		}
		//	COMMENT: 現在シリアライズ中の基底クラスリストを返す
		//		: インスタンススタックが空でないことが前提
		base_list_type& currentBaseList() {
			DataElement::id_type cur_id = currentInstanceID();
			return element_map_.getObjectMap()[cur_id].getBaseList();
		}
		//	COMMENT: 現在シリアライズ中のデータマップを返す
		data_map_type& currentDataMap() {
			//	COMMENT: インスタンススタックが空か？
			if (instance_stack_.empty()) {
				//	COMMENT: ルートのデータマップを返す
				return element_map_.getDataMap();
			} else {
				index_stack_type& cur_index = currentInstanceIndexStack();
				//	COMMENT: 基底クラスをシリアライズ中であるか？
				if (!cur_index.empty()) {
					//	COMMENT: 基底クラスのデータマップを返す
					return currentBaseList()
						.KTL_SERIALIZATION_AT(static_cast<base_list_type::size_type>(cur_index.back()))
						.getDataMap();
				} else {
					//	COMMENT: ルートクラスのデータマップを返す
					return element_map_.getObjectMap()[currentInstanceID()].getDataMap();
				}
			}
		}
	public:
		virtual bool set(
			key_type const& key,
			tTJSVariant const* v,
			key_type const* ser
			)
		{
			data_map_type& data_map = currentDataMap();
			if (data_map.find(key) != data_map.end()) {
				KTL_WARNING(
					KTL_WARNING_SECTION,
					SPRIG_KRKR_TJS_W("キーに対応する要素を上書きします:") + key
					);
			}
			DataElement::id_type id = getID(v);
			//	COMMENT: シリアライザキーが明示的指定されているか？
			if (ser) {
				//	COMMENT: この場合、対象は有効なオブジェクトでなければならない
				//	COMMENT: 現在シリアライズ中のクラスに一致するか？
				if (isCurrentInstance(id)) {
					//	COMMENT: 基底クラスのシリアライズとみなす
					data_map[key] = DataElement(nextBaseListIndex(id));
				} else {
					//	COMMENT: ダックタイプのシリアライズとみなす
					data_map[key] = DataElement(*v);
				}
				//	COMMENT: いずれの場合も、シリアライザの呼出が必要
				return false;
			} else {
				//	COMMENT: 要素の値を設定する
				data_map[key] = DataElement(*v);
			}
			//	COMMENT: シリアライザの呼出が必要ないのは、
			//		: リーフ値である、
			//		: またはIDが既に登録されている場合
			return !id || existObject(id);
		}
		virtual void pushOnSave(
			key_type const& key,
			tTJSVariant const* v,
			key_type const& ser,
			version_type const ver,
			key_type const& identity
			)
		{
			data_map_type& data_map = currentDataMap();
			DataElement& data = data_map[key];
			DataElement::id_type id = getID(v);
			//	COMMENT: 現在シリアライズ中のクラスに一致するか？
			if (isCurrentInstance(id)) {
				//	COMMENT: 基底クラスのシリアライズとみなす
				DataElement::index_type next_index = nextBaseListIndex(id);
				currentBaseList().push_back(BaseElement(ser));
				index_stack_type& cur_index = currentInstanceIndexStack();
				cur_index.push_back(next_index);
			} else {
				//	COMMENT: ルートクラスのシリアライズとみなす
				element_map_.getObjectMap()[id] = ObjectElement(ser);
				instance_stack_.push_back(instance_stack_element_type(id, index_stack_type()));
				//	COMMENT: シリアライザマップに未登録ならば登録する
				if (!element_map_.getSerializerMap().count(ser)) {
					element_map_.getSerializerMap()[ser] = SerializerElement(ver, identity);
				}
			}
		}
		virtual void popOnSave() {
			index_stack_type& cur_index = currentInstanceIndexStack();
			if (!cur_index.empty()) {
				cur_index.pop_back();
			} else {
				instance_stack_.pop_back();
			}
		}
		virtual bool isLeaf(key_type const& key) {
			data_map_type& data_map = currentDataMap();
			data_map_type::iterator i = data_map.find(key);
			if (i == data_map.end()) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("キーに対応する要素が存在しません:") + key,
					sprig::krkr::bad_argument
					);
			}
			DataElement& data = i->second;
			DataElement::id_type id = data.getID();
			//	COMMENT: シリアライザの呼出が必要ないのは、
			//		: 対象が基底クラスでない、かつ
			//		: リーフ値である、
			//		: またはインスタンスが既に登録されている場合
			return !data.hasIndex()
				&& (
					!id
					|| instance_map_.count(id)
				);
		}
		virtual void get(key_type const& key, tTJSVariant* v) {
			if (v) {
				data_map_type& data_map = currentDataMap();
				DataElement& data = data_map[key];
				DataElement::id_type id = data.getID();
				//	COMMENT: リーフ値でないか
				if (id) {
					//	COMMENT: 登録済インスタンスのデシリアライズとみなす
					*v = instance_map_[id];
				} else {
					//	COMMENT: リーフ値のデシリアライズとみなす
					data.get(*v);
				}
			}
		}
		virtual void pushOnLoad(key_type const& key) {
			data_map_type& data_map = currentDataMap();
			DataElement& data = data_map[key];
			if (data.hasIndex()) {
				//	COMMENT: 基底クラスのデシリアライズとみなす
				index_stack_type& cur_index = currentInstanceIndexStack();
				cur_index.push_back(data.getIndex());
			} else {
				//	COMMENT: ルートクラスのデシリアライズとみなす
				DataElement::id_type id = data.getID();
				instance_stack_.push_back(instance_stack_element_type(id, index_stack_type()));
			}
		}
		virtual void popOnLoad(tTJSVariant const* v) {
			index_stack_type& cur_index = currentInstanceIndexStack();
			//	COMMENT: カレントインデックスが設定されているか？
			if (!cur_index.empty()) {
				//	COMMENT: 基底クラスのデシリアライズとみなす
				cur_index.pop_back();
			} else {
				//	COMMENT: ルートクラスのデシリアライズとみなす
				DataElement::id_type cur_id = currentInstanceID();
				instance_map_[cur_id] = *v;
				instance_stack_.pop_back();
			}
		}
		virtual key_type serializer(key_type const& key) {
			data_map_type& data_map = currentDataMap();
			DataElement& data = data_map[key];
			if (data.hasIndex()) {
				//	COMMENT: 基底クラスのデシリアライズとみなす
				return element_map_.getObjectMap()[currentInstanceID()]
					.getBaseList()
					.KTL_SERIALIZATION_AT(static_cast<base_list_type::size_type>(data.getIndex()))
					.getSerializer();
			} else {
				//	COMMENT: ルートクラスのデシリアライズとみなす
				DataElement::id_type id = data.getID();
				return element_map_.getObjectMap()[id]
					.getSerializer();
			}
		}
		virtual version_type version(key_type const& key) {
			return element_map_.getSerializerMap()[key].getVersion();
		}
		virtual key_type identity(key_type const& key) {
			return element_map_.getSerializerMap()[key].getIdentity();
		}
	private:
		template<typename Key>
		static Key const& getKeyPtArchive() {
			static Key const key = SPRIG_KRKR_TJS_W("pt_archive");
			return key;
		}
	private:
		void reset(ElementMap const& root) {
			ElementMap new_element_map(root);
			instance_stack_type new_instance_stack;
			instance_map_type new_instance_map;
			boost::swap(element_map_, new_element_map);
			boost::swap(instance_stack_, new_instance_stack);
			boost::swap(instance_map_, new_instance_map);
		}
	public:
		ElementStack() {}
		virtual ~ElementStack() {}
		ElementMap const& getElementMap() const {
			return element_map_;
		}
		ElementMap& getElementMap() {
			return element_map_;
		}
		void clear() {
			reset(ElementMap());
		}
		size_type size() const {
			return const_cast<ElementStack*>(this)->currentDataMap().size();
		}
		template<typename Archive>
		void saveArchive(Archive& ar) const {
			ar << boost::serialization::make_nvp("archive", element_map_);
		}
		template<typename Archive>
		void loadArchive(Archive& ar) {
			ElementMap element_map;
			ar >> boost::serialization::make_nvp("archive", element_map);
			reset(element_map);
		}
		template<typename Ptree>
		void savePtArchive(Ptree& pt) const {
			Ptree& pt_archive = pt.add_child(
				getKeyPtArchive<typename Ptree::key_type>(),
				Ptree()
				);
			element_map_.savePt(pt_archive);
		}
		template<typename Ptree>
		void loadPtArchive(Ptree& pt) {
			ElementMap element_map;
			Ptree& pt_archive = pt.get_child(getKeyPtArchive<typename Ptree::key_type>());
			element_map.loadPt(pt_archive);
			reset(element_map);
		}
	};
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
