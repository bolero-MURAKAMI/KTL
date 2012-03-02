#pragma once

#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "ThreadTaskCommon.hpp"
#include "ThreadTaskDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:ThreadTask:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:ThreadTask:error")

namespace ktl {
	//
	// NativeThreadTask
	//
	NativeThreadTask::NativeThreadTask()
		: impl_(boost::make_shared<ThreadTaskImpl>())
	{
		sprig::krkr::tjs::object_type control_obj;
		{
			iTJSDispatch2* obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("ThreadControl")),
				&obj,
				0,
				0,
				0
				);
			control_obj = sprig::krkr::tjs::object_type(obj, false);
		}
		ThreadControl* control = reinterpret_cast<ThreadControl*>(
			sprig::krkr::tjs::GetPropValue<sprig::krkr::tjs::intptr_type>(
				sprig::krkr::tjs::GetMemberNoAddRef(
					sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("ThreadControl")),
					SPRIG_KRKR_TJS_W("instance")
					),
					control_obj.get()
				)
			);
		control->initImpl(impl_);
		impl_->init_control(control_obj);
	}
	//
	//	SUMMARY: 制御系メソッド
	//
	KTL_INLINE bool NativeThreadTask::start(tTJSVariantClosure const& closure) {
		return impl_->start(closure);
	}
	KTL_INLINE bool NativeThreadTask::cancel() {
		return impl_->cancel();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tTJSVariant NativeThreadTask::result() const {
		return impl_->result();
	}
	KTL_INLINE bool NativeThreadTask::isProcessing() const {
		return impl_->is_processing();
	}
	KTL_INLINE bool NativeThreadTask::cancelled() const {
		return impl_->cancelled();
	}

	//
	// NativeThreadControl
	//
	NativeThreadControl::NativeThreadControl() {}
	void NativeThreadControl::initImpl(boost::shared_ptr<ThreadTaskImpl> const& impl) {
		impl_ = impl;
	}
	//
	//	SUMMARY: 制御系メソッド
	//
	KTL_INLINE void NativeThreadControl::yield() {
		impl_->yield();
	}
	KTL_INLINE void NativeThreadControl::sleep(time_count_type milliseconds) {
		impl_->sleep(milliseconds);
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool NativeThreadControl::cancelled() const {
		return impl_->cancelled();
	}

	//
	// ThreadTask
	//
	ThreadTask::ThreadTask() {}
	tjs_error TJS_INTF_METHOD ThreadTask::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("ThreadTask::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeThreadTask>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD ThreadTask::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("ThreadTask::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 制御系メソッド
	//
	KTL_INLINE bool ThreadTask::start(tTJSVariantClosure const& closure) {
		return instance_->start(closure);
	}
	KTL_INLINE bool ThreadTask::cancel() {
		return instance_->cancel();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE tTJSVariant ThreadTask::result() const {
		return instance_->result();
	}
	KTL_INLINE bool ThreadTask::isProcessing() const {
		return instance_->isProcessing();
	}
	KTL_INLINE bool ThreadTask::cancelled() const {
		return instance_->cancelled();
	}

	//
	// ThreadControl
	//
	ThreadControl::ThreadControl() {}
	tjs_error TJS_INTF_METHOD ThreadControl::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("ThreadControl::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeThreadControl>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD ThreadControl::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("ThreadControl::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	KTL_INLINE void ThreadControl::initImpl(boost::shared_ptr<ThreadTaskImpl> const& impl) {
		instance_->initImpl(impl);
	}
	//
	//	SUMMARY: 内部系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type ThreadControl::getInstance() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(this);
	}
	//
	//	SUMMARY: 制御系メソッド
	//
	KTL_INLINE void ThreadControl::yield() {
		instance_->yield();
	}
	KTL_INLINE void ThreadControl::sleep(tTVInteger milliseconds) {
		instance_->sleep(milliseconds);
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool ThreadControl::cancelled() const {
		return instance_->cancelled();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
