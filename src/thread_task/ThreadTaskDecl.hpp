#pragma once

#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

#include "ThreadTaskCommon.hpp"

namespace ktl {
	//
	// NativeThreadTask
	//
	class NativeThreadTask {
	private:
		boost::shared_ptr<ThreadTaskImpl> impl_;
	public:
		NativeThreadTask();
	public:
		//
		//	SUMMARY: 制御系メソッド
		//
		bool start(tTJSVariantClosure const& closure);
		bool cancel();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		tTJSVariant result() const;
		bool isProcessing() const;
		bool cancelled() const;
	};
	//
	// NativeThreadControl
	//
	class NativeThreadControl {
	public:
		typedef ThreadTaskImpl::time_count_type time_count_type;
	private:
		boost::shared_ptr<ThreadTaskImpl> impl_;
	public:
		NativeThreadControl();
		void initImpl(boost::shared_ptr<ThreadTaskImpl> const& impl);
	public:
		//
		//	SUMMARY: 制御系メソッド
		//
		void yield();
		void sleep(time_count_type milliseconds);
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool cancelled() const;
	};

	//
	// ThreadTask
	//
	class ThreadTask
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeThreadTask> instance_;
	public:
		ThreadTask();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: 制御系メソッド
		//
		bool start(tTJSVariantClosure const& closure);
		bool cancel();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		tTJSVariant result() const;
		bool isProcessing() const;
		bool cancelled() const;
	};
	//
	// ThreadControl
	//
	class ThreadControl
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeThreadControl> instance_;
	public:
		ThreadControl();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
		void initImpl(boost::shared_ptr<ThreadTaskImpl> const& impl);
	public:
		//
		//	SUMMARY: 内部系メソッド
		//
		sprig::krkr::tjs::intptr_type getInstance();
		//
		//	SUMMARY: 制御系メソッド
		//
		void yield();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool cancelled() const;
		void sleep(tTVInteger milliseconds);
	};
}	// namespace ktl
