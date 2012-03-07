#ifndef KTL_THREAD_CALLBACK_HPP
#define KTL_THREAD_CALLBACK_HPP

#include <sprig/config/config.hpp>

#ifdef SPRIG_USING_PRAGMA_ONCE
#	pragma once
#endif	// #ifdef SPRIG_USING_PRAGMA_ONCE

#include <ktl/config.hpp>

#include <string>
#include <deque>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/serialization/singleton.hpp>
#include <sprig/external/windows.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/error.hpp>

#define KTL_WM_THREAD_CALLBACK (WM_APP + 1)

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:thread_callback:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:thread_callback:error")

namespace ktl {
	//
	// thread_callback
	//
	class thread_callback
		: public boost::serialization::singleton<thread_callback>
	{
	public:
		typedef boost::recursive_mutex mutex_type;
		typedef mutex_type::scoped_lock scoped_lock_type;
		typedef std::basic_string<TCHAR> string_type;
		typedef boost::function<void ()> callback_type;
	private:
		typedef std::deque<callback_type> callback_list_type;
	private:
		HWND window_handle_;
		WNDCLASSEX window_class_;
		ATOM atom_;
		string_type identity_;
		callback_list_type callback_list_;
		mutable mutex_type mutex_;
	private:
		static mutex_type& mutex() {
			return get_const_instance().mutex_;
		}
		static bool call_one() {
			if (is_destroyed()) {
				return false;
			}
			scoped_lock_type lock(mutex());
			thread_callback& self = get_mutable_instance();
			if (self.callback_list_.empty()) {
				return false;
			}
			self.callback_list_.front()();
			self.callback_list_.pop_front();
			return true;
		}
		static LRESULT CALLBACK windowProcedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param) {
			switch (message) {
			case KTL_WM_THREAD_CALLBACK:
				while (call_one()) ;
				return 0;
			}
			return ::DefWindowProc(window_handle, message, w_param, l_param);
		}
	public:
		static void setup() {
			if (is_destroyed()) {
				return;
			}
			scoped_lock_type lock(mutex());
			thread_callback& self = get_mutable_instance();
			self.initialize();
		}
		static void teardown() throw() {
			if (is_destroyed()) {
				return;
			}
			scoped_lock_type lock(mutex());
			thread_callback& self = get_mutable_instance();
			self.dispose();
		}
		static void post(callback_type const& callback) {
			if (is_destroyed()) {
				return;
			}
			scoped_lock_type lock(mutex());
			thread_callback& self = get_mutable_instance();
			self.initialize();
			self.callback_list_.push_back(callback);
			::PostMessage(self.window_handle_, KTL_WM_THREAD_CALLBACK, 0, 0);
		}
	private:
		void initialize() {
			if (window_handle_) {
				return;
			}
			if (!(atom_ = ::RegisterClassEx(&window_class_))) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("ウインドウクラスの登録に失敗しました"),
					sprig::krkr::win32_error
					);
				return;
			}
			window_handle_ = ::CreateWindowEx(
				0,												// 拡張ウィンドウスタイル
				reinterpret_cast<LPCTSTR>(MAKELONG(atom_, 0)),	// クラス名
				window_class_.lpszClassName,					// ウィンドウタイトル
				0,												// ウィンドウスタイル
				CW_USEDEFAULT,									// ウィンドウ位置
				0,												// ウィンドウ位置
				CW_USEDEFAULT,									// ウィンドウサイズ
				0,												// ウィンドウサイズ
				HWND_MESSAGE,									// 親ウィンドウ
				0,												// メニュー
				window_class_.hInstance,						// インスタンス
				0												// ウィンドウ作成データのアドレス
				);
			if (!window_handle_) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("ウインドウの作成に失敗しました"),
					sprig::krkr::win32_error
					);
				return;
			}
		}
		void dispose() {
			if (window_handle_) {
				if (!::DestroyWindow(window_handle_)) {
					KTL_ERROR_NOTHROW(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("ウインドウの破棄に失敗しました")
						);
				}
				window_handle_ = 0;
			}
			if (atom_) {
				if (!::UnregisterClass(reinterpret_cast<LPCTSTR>(MAKELONG(atom_, 0)), 0)) {
					KTL_ERROR_NOTHROW(
						KTL_ERROR_SECTION,
						SPRIG_KRKR_TJS_W("ウインドウクラスの登録解除に失敗しました")
						);
				}
				atom_ = 0;
			}
		}
	public:
		thread_callback()
			: window_handle_(0)
			, atom_(0)
			, identity_(sprig::str_cast<string_type>(boost::uuids::random_generator()()))
		{
			window_class_.cbSize = sizeof(WNDCLASSEX);							// 構造体のサイズ
			window_class_.style = 0;											// クラススタイル
			window_class_.lpfnWndProc = static_cast<WNDPROC>(&windowProcedure);	// ウィンドウプロシージャ
			window_class_.cbClsExtra = 0;										// 補足メモリブロックのサイズ
			window_class_.cbWndExtra = 0;										// 補足メモリブロックのサイズ
			window_class_.hInstance = ::GetModuleHandle(0);						// インスタンス
			window_class_.hIcon = 0;											// アイコン
			window_class_.hCursor = 0;											// カーソル
			window_class_.hbrBackground = 0;									// 背景色
			window_class_.lpszMenuName = 0;										// メニュー名
			window_class_.lpszClassName = identity_.c_str();					// クラス名
			window_class_.hIconSm = 0;											// 小さいアイコン
		}
		~thread_callback() throw() {
			dispose();
		}
	};
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

//
// ktl_thread_callback_setup
// ktl_thread_callback_teardown
//
static void ktl_thread_callback_setup() {
	ktl::thread_callback::setup();
}
static void ktl_thread_callback_teardown() {
	ktl::thread_callback::setup();
}

//
// KTL_THREAD_CALLBACK_POST_OUTPUT
// KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE
// KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT
//
#define KTL_THREAD_CALLBACK_POST_OUTPUT(MESSAGE, LEVEL) \
	SPRIG_KRKR_LOG_LEVEL_SWITCH( \
		ktl::thread_callback::post( \
			boost::bind(sprig::krkr::tjs::output, MESSAGE) \
			), \
		LEVEL \
		)
#define KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(NAME, CONTENTS, LEVEL) \
	SPRIG_KRKR_LOG_LEVEL_SWITCH( \
		ktl::thread_callback::post( \
			boost::bind(sprig::krkr::tjs::output_value, NAME, CONTENTS) \
			), \
		LEVEL \
		)
#define KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(COMMENT, LEVEL) \
	SPRIG_KRKR_LOG_LEVEL_SWITCH( \
		ktl::thread_callback::post( \
			boost::bind(sprig::krkr::tjs::output_comment, COMMENT) \
			), \
		LEVEL \
		)

//
// KTL_THREAD_CALLBACK_POST_FAIL_MESSAGE
//
#define KTL_THREAD_CALLBACK_POST_FAIL_MESSAGE(SECTION, MESSAGE, LEVEL) \
	SPRIG_KRKR_LOG_LEVEL_SWITCH( \
		ktl::thread_callback::post( \
			boost::bind(ktl::fail_message, (SECTION), (MESSAGE), SPRIG_KRKR_TJS_W(__FILE__), __LINE__, SPRIG_KRKR_TJS_W(BOOST_CURRENT_FUNCTION)) \
			), \
		LEVEL \
		)

//
// KTL_THREAD_CALLBACK_POST_WARNING
// KTL_THREAD_CALLBACK_POST_ERROR
//
#define KTL_THREAD_CALLBACK_POST_WARNING(SECTION, MESSAGE) \
	KTL_THREAD_CALLBACK_POST_FAIL_MESSAGE((SECTION), (MESSAGE), SPRIG_KRKR_LOG_LEVEL_WARNING)
#define KTL_THREAD_CALLBACK_POST_ERROR(SECTION, MESSAGE) \
	KTL_THREAD_CALLBACK_POST_FAIL_MESSAGE((SECTION), (MESSAGE), SPRIG_KRKR_LOG_LEVEL_ERROR)

#endif	// #ifndef KTL_THREAD_CALLBACK_HPP
