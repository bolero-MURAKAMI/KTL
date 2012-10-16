#pragma once

#include "Config.hpp"

#include <cstddef>
#include <vector>
#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <ktl/error.hpp>
#include <ktl/serialization.hpp>

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Serialization:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Serialization:error")

namespace ktl {
	//
	// ScriptMethod
	//
	class ScriptMethod {
	private:
		sprig::krkr::tjs::object_type object_;
		sprig::krkr::tjs::object_type obj_this_;
	public:
		ScriptMethod(
			sprig::krkr::tjs::object_type const& object,
			sprig::krkr::tjs::object_type const& obj_this,
			tjs_char const* membername
			)
		{
			tTJSVariant method_v;
			sprig::krkr::tjs::MemberGet(object.get(), membername, &method_v, obj_this.get());
			tTJSVariantClosure const& closure = method_v.AsObjectClosureNoAddRef();
			object_ = closure.Object;
			obj_this_ = closure.ObjThis;
		}
		ScriptMethod(
			sprig::krkr::tjs::object_type const& object,
			sprig::krkr::tjs::object_type const& obj_this
			)
			: object_(object)
			, obj_this_(obj_this)
		{}
		void call(
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param
			) const
		{
			sprig::krkr::tjs::FuncObjectCall(
				object_.get(),
				result,
				numparams,
				param,
				obj_this_.get()
				);
		}
		sprig::krkr::tjs::object_type const& object() const {
			return object_;
		}
		sprig::krkr::tjs::object_type const& obj_this() const {
			return obj_this_;
		}
	};

	//
	// getSerializerInstance
	//
	ISerializer* getSerializerInstance(tTJSVariantClosure const& closure) {
		tTJSVariant ser_v;
		{
			tjs_error const error = closure.Object->PropGet(
				0,
				SPRIG_KRKR_TJS_W("serializer"),
				0,
				&ser_v,
				closure.ObjThis
				);
			if (TJS_FAILED(error) && error != TJS_E_MEMBERNOTFOUND) {
				KTL_ERROR(
					KTL_WARNING_SECTION,
					SPRIG_KRKR_TJS_W("stackの取得でエラーが発生しました"),
					sprig::krkr::tvp_error
					);
			}
		}
		return reinterpret_cast<ISerializer*>(
			static_cast<sprig::krkr::tjs::intptr_type>(ser_v)
			);
	}
	//
	// getSerializationStackInstance
	//
	ISerializationStack* getSerializationStackInstance(tTJSVariantClosure const& closure) {
		tTJSVariant stack_v;
		{
			tjs_error const error = closure.Object->PropGet(
				0,
				SPRIG_KRKR_TJS_W("stack"),
				0,
				&stack_v,
				closure.ObjThis
				);
			if (TJS_FAILED(error) && error != TJS_E_MEMBERNOTFOUND) {
				KTL_ERROR(
					KTL_WARNING_SECTION,
					SPRIG_KRKR_TJS_W("stackの取得でエラーが発生しました"),
					sprig::krkr::tvp_error
					);
			}
		}
		return reinterpret_cast<ISerializationStack*>(
			static_cast<sprig::krkr::tjs::intptr_type>(stack_v)
			);
	}

	//
	// ScriptSerializer
	//
	class ScriptSerializer
		: public ISerializer
	{
	private:
		ScriptMethod save_;
		ScriptMethod load_;
		ISerializer::version_type version_;
		ISerializer::key_type identity_;
	public:
		virtual void save(
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param
			) const
		{
			save_.call(result, numparams, param);
		}
		virtual void load(
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param
			) const
		{
			load_.call(result, numparams, param);
		}
		virtual ISerializer::version_type version() const {
			return version_;
		}
		virtual ISerializer::key_type identity() const {
			return identity_;
		}
	public:
		ScriptSerializer(
			sprig::krkr::tjs::object_type const& object,
			sprig::krkr::tjs::object_type const& obj_this
			)
			: save_(object, obj_this, SPRIG_KRKR_TJS_W("save"))
			, load_(object, obj_this, SPRIG_KRKR_TJS_W("load"))
			, version_()
		{
			{
				tTJSVariant ver_v;
				tjs_error const error = object->PropGet(0, SPRIG_KRKR_TJS_W("version"), 0, &ver_v, obj_this.get());
				if (TJS_FAILED(error) && error != TJS_E_MEMBERNOTFOUND) {
					KTL_ERROR(
						KTL_WARNING_SECTION,
						SPRIG_KRKR_TJS_W("versionの取得でエラーが発生しました"),
						sprig::krkr::tvp_error
						);
				}
				version_ = static_cast<ISerializer::version_type>(ver_v);
			}
			{
				tTJSVariant id_v;
				tjs_error const error = object->PropGet(0, SPRIG_KRKR_TJS_W("identity"), 0, &id_v, obj_this.get());
				if (TJS_FAILED(error) && error != TJS_E_MEMBERNOTFOUND) {
					KTL_ERROR(
						KTL_WARNING_SECTION,
						SPRIG_KRKR_TJS_W("identityの取得でエラーが発生しました"),
						sprig::krkr::tvp_error
						);
				}
				identity_ = static_cast<ISerializer::key_type>(id_v);
			}
		}
	};

	//
	// NativeSerializer
	//
	class NativeSerializer
		: public ISerializer
	{
	private:
		sprig::krkr::tjs::object_type object_;
		sprig::krkr::tjs::object_type obj_this_;
		ISerializer* serializer_;
	public:
		virtual void save(
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param
			) const
		{
			serializer_->save(result, numparams, param);
		}
		virtual void load(
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param
			) const
		{
			serializer_->load(result, numparams, param);
		}
		virtual ISerializer::version_type version() const {
			return serializer_->version();
		}
		virtual ISerializer::key_type identity() const {
			return serializer_->identity();
		}
	public:
		NativeSerializer(
			sprig::krkr::tjs::object_type const& object,
			sprig::krkr::tjs::object_type const& obj_this,
			ISerializer* serializer
			)
			: object_(object)
			, obj_this_(obj_this)
			, serializer_(serializer)
		{}
		sprig::krkr::tjs::object_type const& object() const {
			return object_;
		}
		sprig::krkr::tjs::object_type const& obj_this() const {
			return obj_this_;
		}
		ISerializer* serializer() const {
			return serializer_;
		}
	};

	//
	// ScriptSerializationStack
	//
	class ScriptSerializationStack
		: public ISerializationStack
	{
	private:
		ScriptMethod set_;
		ScriptMethod pushOnSave_;
		ScriptMethod popOnSave_;
		ScriptMethod isLeaf_;
		ScriptMethod get_;
		ScriptMethod pushOnLoad_;
		ScriptMethod popOnLoad_;
		ScriptMethod serializer_;
		ScriptMethod version_;
		ScriptMethod identity_;
	public:
		virtual bool set(
			key_type const& key,
			tTJSVariant const* v,
			key_type const* ser
			)
		{
			tTJSVariant result;
			if (ser) {
				tTJSVariant param[] = {
					key,
					*v,
					*ser
				};
				tTJSVariant* p_param[] = {
					param + 0,
					param + 1,
					param + 2
				};
				set_.call(&result, 3, p_param);
			} else {
				tTJSVariant param[] = {
					key,
					*v
				};
				tTJSVariant* p_param[] = {
					param + 0,
					param + 1
				};
				set_.call(&result, 2, p_param);
			}
			return result.AsInteger() != 0;
		}
		virtual void pushOnSave(
			key_type const& key,
			tTJSVariant const* v,
			key_type const& ser,
			version_type const ver,
			key_type const& identity
			)
		{
			tTJSVariant param[] = {
				key,
				*v,
				ser,
				ver,
				identity
			};
			tTJSVariant* p_param[] = {
				param + 0,
				param + 1,
				param + 2,
				param + 3,
				param + 4
			};
			pushOnSave_.call(0, 5, p_param);
		}
		virtual void popOnSave() {
			popOnSave_.call(0, 0, 0);
		}
		virtual bool isLeaf(key_type const& key) {
			tTJSVariant result;
			{
				tTJSVariant param[] = {
					key
				};
				tTJSVariant* p_param[] = {
					param + 0
				};
				isLeaf_.call(&result, 1, p_param);
			}
			return result.AsInteger() != 0;
		}
		virtual void get(key_type const& key, tTJSVariant* v) {
			tTJSVariant param[] = {
				key
			};
			tTJSVariant* p_param[] = {
				param + 0
			};
			get_.call(v, 1, p_param);
		}
		virtual void pushOnLoad(key_type const& key) {
			tTJSVariant param[] = {
				key
			};
			tTJSVariant* p_param[] = {
				param + 0
			};
			pushOnLoad_.call(0, 1, p_param);
		}
		virtual void popOnLoad(tTJSVariant const* v) {
			tTJSVariant param[] = {
				*v
			};
			tTJSVariant* p_param[] = {
				param + 0
			};
			popOnLoad_.call(0, 1, p_param);
		}
		virtual key_type serializer(key_type const& key) {
			tTJSVariant result;
			{
				tTJSVariant param[] = {
					key
				};
				tTJSVariant* p_param[] = {
					param + 0
				};
				serializer_.call(&result, 1, p_param);
			}
			return static_cast<key_type>(result);
		}
		virtual version_type version(key_type const& key) {
			tTJSVariant result;
			{
				tTJSVariant param[] = {
					key
				};
				tTJSVariant* p_param[] = {
					param + 0
				};
				version_.call(&result, 1, p_param);
			}
			return static_cast<version_type>(result);
		}
		virtual key_type identity(key_type const& key) {
			tTJSVariant result;
			{
				tTJSVariant param[] = {
					key
				};
				tTJSVariant* p_param[] = {
					param + 0
				};
				identity_.call(&result, 1, p_param);
			}
			return static_cast<key_type>(result);
		}
	public:
		ScriptSerializationStack(
			sprig::krkr::tjs::object_type const& object,
			sprig::krkr::tjs::object_type const& obj_this
			)
			: set_(object, obj_this, SPRIG_KRKR_TJS_W("set"))
			, pushOnSave_(object, obj_this, SPRIG_KRKR_TJS_W("pushOnSave"))
			, popOnSave_(object, obj_this, SPRIG_KRKR_TJS_W("popOnSave"))
			, isLeaf_(object, obj_this, SPRIG_KRKR_TJS_W("isLeaf"))
			, get_(object, obj_this, SPRIG_KRKR_TJS_W("get"))
			, pushOnLoad_(object, obj_this, SPRIG_KRKR_TJS_W("pushOnLoad"))
			, popOnLoad_(object, obj_this, SPRIG_KRKR_TJS_W("popOnLoad"))
			, serializer_(object, obj_this, SPRIG_KRKR_TJS_W("serializer"))
			, version_(object, obj_this, SPRIG_KRKR_TJS_W("version"))
			, identity_(object, obj_this, SPRIG_KRKR_TJS_W("identity"))
		{}
	};

	//
	// StackCacheManager
	//
	class StackCacheManager {
	private:
		class Cache {
		private:
			tTJSVariant archive_;
			ISerializationStack* stack_;
			std::size_t count_;
			boost::shared_ptr<ISerializationStack> stack_instance_;
		public:
			Cache()
				: stack_(0)
				, count_(0)
			{}
			explicit Cache(tTJSVariant const& archive)
				: archive_(archive)
				, stack_(0)
				, count_(1)
			{
				tTJSVariantClosure const& closure = archive_.AsObjectClosureNoAddRef();
				stack_ = getSerializationStackInstance(closure);
				if (!stack_) {
					// COMMENT: スタックオブジェクトを取得できない場合
					stack_instance_ = boost::make_shared<ScriptSerializationStack>(
						closure.Object,
						closure.ObjThis
						);
					stack_ = stack_instance_.get();
				}
			}
			bool same(tTJSVariant const& archive) const {
				return archive_.DiscernCompare(archive);
			}
			ISerializationStack* addRef() {
				++count_;
				return stack_;
			}
			bool release() {
				if (--count_ <= 0) {
					archive_.Clear();
					stack_ = 0;
					stack_instance_.reset();
					return true;
				}
				return false;
			}
			ISerializationStack* get() const {
				return stack_;
			}
		};
	private:
		std::vector<Cache> caches_;
	public:
		ISerializationStack* push(tTJSVariant const& archive) {
			if (caches_.empty() || !caches_.back().same(archive)) {
				caches_.push_back(Cache(archive));
			} else {
				return caches_.back().addRef();
			}
			return caches_.back().get();
		}
		void pop() {
			if (caches_.back().release()) {
				caches_.pop_back();
			}
		}
	};

	//
	// ScopedStackCache
	//
	class ScopedStackCache {
	private:
		StackCacheManager& manager_;
		ISerializationStack* stack_;
	public:
		ScopedStackCache(StackCacheManager& manager, tTJSVariant const& archive)
			: manager_(manager)
			, stack_(manager_.push(archive))
		{}
		~ScopedStackCache() {
			manager_.pop();
		}
		ISerializationStack* get() const {
			return stack_;
		}
	};
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
