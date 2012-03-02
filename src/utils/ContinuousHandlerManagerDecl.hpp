#pragma once

#include <string>
#include <vector>
#include <utility>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/priority_map.hpp>
#include <sprig/call_traits.hpp>
#include <sprig/performance_timer.hpp>
#include <sprig/timer.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeContinuousHandlerManager
	//
	class NativeContinuousHandlerManager
		: public tTVPContinuousEventCallbackIntf
	{
	public:
		typedef sprig::priority_map_defs<
			std::basic_string<tjs_char>,
			double,
			std::pair<sprig::krkr::tjs::object_type, sprig::krkr::tjs::object_type>
		> priority_map_defs_type;
		typedef priority_map_defs_type::key_type key_type;
		typedef priority_map_defs_type::priority_type priority_type;
		typedef priority_map_defs_type::mapped_type mapped_type;
		typedef priority_map_defs_type::value_type value_type;
		typedef priority_map_defs_type::key_comparator_type key_comparator_type;
		typedef priority_map_defs_type::comparator_type comparator_type;
		typedef priority_map_defs_type::key_tag_type key_tag_type;
		typedef priority_map_defs_type::priority_tag_type priority_tag_type;
		typedef priority_map_defs_type::type priority_map_type;
		typedef priority_map_type::index<priority_map_defs_type::key_tag_type>::type key_index_type;
		typedef priority_map_type::index<priority_map_defs_type::priority_tag_type>::type priority_index_type;
		typedef boost::optional<value_type const&> optional_value_type;
	private:
		priority_map_type priority_map_;
		bool run_;
		std::vector<sprig::performance_timer::value_type> performance_times_;
		sprig::performance_timer performance_timer_;
		sprig::performance_timer unit_performance_timer_;
		bool enable_performance_timer_;
	public:
		NativeContinuousHandlerManager();
		~NativeContinuousHandlerManager() throw();
		void TJS_INTF_METHOD OnContinuousCallback(tjs_uint64 tick);
	public:
		//
		//	SUMMARY: ハンドラ管理系メソッド
		//
		bool insert(
			sprig::call_traits<key_type>::param_type key,
			sprig::call_traits<priority_type>::param_type priority,
			tTJSVariant const* v
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
		optional_value_type findValue(sprig::call_traits<key_type>::param_type key) const;
		//
		//	SUMMARY: 出力系メソッド
		//
		void beginPerformanceTimer();
		void endPerformanceTimer();
		void execHandlers(tjs_uint64 tick);
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool getRun() const;
		void setRun(bool v);
		//
		//	SUMMARY: デバッグ系メソッド
		//
		bool getEnablePerformanceTimer() const;
		void setEnablePerformanceTimer(bool v);
	};

	//
	// ContinuousHandlerManager
	//
	class ContinuousHandlerManager
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeContinuousHandlerManager> instance_;
	public:
		ContinuousHandlerManager();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
		//
		//	SUMMARY: ハンドラ管理系メソッド
		//
		bool insert(
			tTJSString const& key,
			tTVReal priority,
			tTJSVariant const* v
			);
		tTVInteger erase(tTJSString const& key);
		void clear();
		tTVInteger size() const;
		bool exist(tTJSString const& key) const;
		boost::optional<tTVReal> getPriority(tTJSString const& key) const;
		bool setPriority(tTJSString const& key, tTVReal v);
		tTJSVariant find(tTJSString const& key) const;
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool getRun() const;
		void setRun(bool v);
		//
		//	SUMMARY: デバッグ系メソッド
		//
		bool getEnablePerformanceTimer() const;
		void setEnablePerformanceTimer(bool v);
	};
}	// namespace ktl
