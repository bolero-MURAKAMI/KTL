#pragma once

#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/get_pointer.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "ContinuousHandlerManagerDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:ContinuousHandlerManager:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:ContinuousHandlerManager:error")

namespace ktl {
	//
	// NativeContinuousHandlerManager
	//
	NativeContinuousHandlerManager::NativeContinuousHandlerManager()
		: run_(false)
		, enable_performance_timer_(false)
	{}
	NativeContinuousHandlerManager::~NativeContinuousHandlerManager() throw() {
		if (run_) {
			::TVPRemoveContinuousEventHook(this);
		}
	}
	void TJS_INTF_METHOD NativeContinuousHandlerManager::OnContinuousCallback(tjs_uint64 tick) {
		try {
			beginPerformanceTimer();
			execHandlers(tick);
			endPerformanceTimer();
		} catch (sprig::exception const& e) {
			sprig::krkr::tjs::rethrow_exception_to_tjs_exception(e);
		} catch (boost::exception const& e) {
			sprig::krkr::tjs::rethrow_exception_to_tjs_exception(e);
		} catch (std::exception const& e) {
			sprig::krkr::tjs::rethrow_exception_to_tjs_exception(e);
		}
	}
	//
	//	SUMMARY: ハンドラ管理系メソッド
	//
	KTL_INLINE bool NativeContinuousHandlerManager::insert(
		sprig::call_traits<key_type>::param_type key,
		sprig::call_traits<priority_type>::param_type priority,
		tTJSVariant const* v
		)
	{
		if (exist(key)) {
			KTL_WARNING(
				KTL_WARNING_SECTION,
				SPRIG_KRKR_TJS_W("既に登録されたハンドラを上書きします:") + key
				);
		}
		tTJSVariantClosure const& obj = v->AsObjectClosureNoAddRef();
		return priority_map_.insert(value_type(key, priority, mapped_type(obj.Object, obj.ObjThis))).second;
	}
	KTL_INLINE NativeContinuousHandlerManager::priority_map_type::size_type NativeContinuousHandlerManager::erase(sprig::call_traits<key_type>::param_type key) {
		return priority_map_.erase(key);
	}
	KTL_INLINE void NativeContinuousHandlerManager::clear() {
		priority_map_.clear();
	}
	KTL_INLINE NativeContinuousHandlerManager::priority_map_type::size_type NativeContinuousHandlerManager::size() const {
		return priority_map_.size();
	}
	KTL_INLINE NativeContinuousHandlerManager::priority_map_type::const_iterator NativeContinuousHandlerManager::begin() const {
		return priority_map_.begin();
	}
	KTL_INLINE NativeContinuousHandlerManager::priority_map_type::iterator NativeContinuousHandlerManager::begin() {
		return priority_map_.begin();
	}
	KTL_INLINE NativeContinuousHandlerManager::priority_map_type::const_iterator NativeContinuousHandlerManager::end() const {
		return priority_map_.end();
	}
	KTL_INLINE NativeContinuousHandlerManager::priority_map_type::iterator NativeContinuousHandlerManager::end() {
		return priority_map_.end();
	}
	KTL_INLINE NativeContinuousHandlerManager::priority_map_type::const_iterator NativeContinuousHandlerManager::find(sprig::call_traits<NativeContinuousHandlerManager::key_type>::param_type key) const {
		return priority_map_.find(key);
	}
	KTL_INLINE NativeContinuousHandlerManager::priority_map_type::iterator NativeContinuousHandlerManager::find(sprig::call_traits<NativeContinuousHandlerManager::key_type>::param_type key) {
		return priority_map_.find(key);
	}
	KTL_INLINE bool NativeContinuousHandlerManager::exist(sprig::call_traits<key_type>::param_type key) const {
		return priority_map_.find(key) != priority_map_.end();
	}
	KTL_INLINE boost::optional<NativeContinuousHandlerManager::priority_type> NativeContinuousHandlerManager::getPriority(sprig::call_traits<NativeContinuousHandlerManager::key_type>::param_type key) const {
		optional_value_type value = findValue(key);
		return value ? value->get_priority() : boost::optional<priority_type>();
	}
	KTL_INLINE bool NativeContinuousHandlerManager::setPriority(
		sprig::call_traits<NativeContinuousHandlerManager::key_type>::param_type key,
		sprig::call_traits<NativeContinuousHandlerManager::priority_type>::param_type v
		)
	{
		priority_map_type::iterator position = priority_map_.find(key);
		if (position == priority_map_.end()) {
			return false;
		}
		return priority_map_.modify(position, sprig::priority_modifier<priority_map_defs_type>(v));
	}
	KTL_INLINE NativeContinuousHandlerManager::optional_value_type NativeContinuousHandlerManager::findValue(
		sprig::call_traits<key_type>::param_type key
		) const
	{
		priority_map_type::const_iterator i = priority_map_.find(key);
		if (i == priority_map_.end()) {
			return boost::none;
		}
		return *i;
	}
	//
	//	SUMMARY: 出力系メソッド
	//
	KTL_INLINE void NativeContinuousHandlerManager::beginPerformanceTimer() {
		if (!enable_performance_timer_) {
			return;
		}
		performance_timer_.restart();
	}
	KTL_INLINE void NativeContinuousHandlerManager::endPerformanceTimer() {
		if (!enable_performance_timer_) {
			return;
		}
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
				SPRIG_KRKR_TJS_W("KTL:ContinuousHandlerManager:performance"),
				boost::basic_format<tjs_char>(SPRIG_KRKR_TJS_W("%f[ms]")) % (average * 1000),
				SPRIG_KRKR_LOG_LEVEL_IMPORTANT
				);
		}
	}
	KTL_INLINE void NativeContinuousHandlerManager::execHandlers(tjs_uint64 tick) {
		BOOST_FOREACH(value_type const& e, priority_map_.get<priority_tag_type>()) {
			tTJSVariant param[] = {
				static_cast<tTVInteger>(tick)
			};
			tTJSVariant* p_param[] = {
				param + 0
			};
			sprig::krkr::tjs::FuncObjectCall(
				sprig::get_pointer(e.get_mapped().first),
				0,
				1,
				p_param,
				sprig::get_pointer(e.get_mapped().second)
				);
		}
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool NativeContinuousHandlerManager::getRun() const {
		return run_;
	}
	KTL_INLINE void NativeContinuousHandlerManager::setRun(bool v) {
		if (!run_ && v) {
			TVPAddContinuousEventHook(this);
		} else if (run_ && !v) {
			TVPRemoveContinuousEventHook(this);
		}
		run_ = v;
	}
	//
	//	SUMMARY: デバッグ系メソッド
	//
	KTL_INLINE bool NativeContinuousHandlerManager::getEnablePerformanceTimer() const {
		return enable_performance_timer_;
	}
	KTL_INLINE void NativeContinuousHandlerManager::setEnablePerformanceTimer(bool v) {
		enable_performance_timer_ = v;
	}

	//
	// ContinuousHandlerManager
	//
	ContinuousHandlerManager::ContinuousHandlerManager() {}
	tjs_error TJS_INTF_METHOD ContinuousHandlerManager::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("ContinuousHandlerManager::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeContinuousHandlerManager>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD ContinuousHandlerManager::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("ContinuousHandlerManager::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: ハンドラ管理系メソッド
	//
	KTL_INLINE bool ContinuousHandlerManager::insert(
		tTJSString const& key,
		tTVReal priority,
		tTJSVariant const* v
		)
	{
		return instance_->insert(key.c_str(), priority, v);
	}
	KTL_INLINE tTVInteger ContinuousHandlerManager::erase(tTJSString const& key) {
		return instance_->erase(key.c_str());
	}
	KTL_INLINE void ContinuousHandlerManager::clear() {
		instance_->clear();
	}
	KTL_INLINE tTVInteger ContinuousHandlerManager::size() const {
		return instance_->size();
	}
	KTL_INLINE bool ContinuousHandlerManager::exist(tTJSString const& key) const {
		return instance_->exist(key.c_str());
	}
	KTL_INLINE boost::optional<tTVReal> ContinuousHandlerManager::getPriority(tTJSString const& key) const {
		return instance_->getPriority(key.c_str());
	}
	KTL_INLINE bool ContinuousHandlerManager::setPriority(tTJSString const& key, tTVReal v) {
		return instance_->setPriority(key.c_str(), v);
	}
	KTL_INLINE tTJSVariant ContinuousHandlerManager::find(tTJSString const& key) const {
		tTJSVariant result;
		NativeContinuousHandlerManager::optional_value_type value = instance_->findValue(key.c_str());
		if (value) {
			result = tTJSVariant(sprig::get_pointer(value->get_mapped().first), sprig::get_pointer(value->get_mapped().second));
		}
		return result;
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool ContinuousHandlerManager::getRun() const {
		return instance_->getRun();
	}
	KTL_INLINE void ContinuousHandlerManager::setRun(bool v) {
		instance_->setRun(v);
	}
	//
	//	SUMMARY: デバッグ系メソッド
	//
	KTL_INLINE bool ContinuousHandlerManager::getEnablePerformanceTimer() const {
		return instance_->getEnablePerformanceTimer();
	}
	KTL_INLINE void ContinuousHandlerManager::setEnablePerformanceTimer(bool v) {
		instance_->setEnablePerformanceTimer(v);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
