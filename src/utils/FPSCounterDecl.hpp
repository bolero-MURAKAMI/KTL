#pragma once

#include <cstddef>
#include <deque>
#include <sprig/external/tp_stub.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/time_counter.hpp>

namespace ktl {
	//
	// NativeFPSCounter
	//
	class NativeFPSCounter
		: public tTVPContinuousEventCallbackIntf
	{
	public:
		typedef sprig::time_counter time_counter_type;
		typedef time_counter_type::value_type value_type;
		typedef std::deque<value_type> container_type;
	private:
		time_counter_type time_counter_;
		container_type container_;
		value_type before_time_;
		bool enable_;
		bool run_;
		value_type unit_time_;
	public:
		NativeFPSCounter();
		~NativeFPSCounter() throw();
		void TJS_INTF_METHOD OnContinuousCallback(tjs_uint64 tick);
	public:
		//
		//	SUMMARY: 出力系メソッド
		//
		value_type getTime() const;
		std::size_t getFPS() const;
		void push(value_type const time);
		void truncate(value_type const time);
		void clear();
		void resetBeforeTime(value_type const time);
		void process();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool getEnable() const;
		void setEnable(bool v);
		bool getRun() const;
		void setRun(bool v);
	};

	//
	// FPSCounter
	//
	class FPSCounter
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeFPSCounter> instance_;
	public:
		FPSCounter();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool getEnable() const;
		void setEnable(bool v);
		bool getRun() const;
		void setRun(bool v);
	};
}	// namespace ktl
