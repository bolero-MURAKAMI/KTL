#pragma once

#include <deque>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <sprig/config/lib/openssl.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

#include "NetworkCommon.hpp"
#include "SocketDecl.hpp"

namespace ktl {
	//
	// NativeAcceptor
	//
	class NativeAcceptor {
	public:
		typedef NetworkService::size_type size_type;
		typedef NetworkUtils::flag_type flag_type;
		typedef NetworkUtils::timeout_timer_type timeout_timer_type;
		typedef NetworkUtils::time_point_type time_point_type;
		typedef NetworkUtils::rep_type rep_type;
		typedef NetworkUtils::milliseconds_type milliseconds_type;
		typedef boost::int_fast32_t int_type;
		typedef boost::recursive_mutex mutex_type;
		typedef mutex_type::scoped_lock scoped_lock_type;
	private:
		//
		// AcceptElement
		//
		struct AcceptElement {
		public:
			boost::shared_ptr<boost::asio::ip::tcp::socket> socket;
			boost::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> > ssl_socket;
			Acceptors acceptors;
		};
	private:
		//
		boost::shared_ptr<boost::asio::io_service> io_service_;
		boost::shared_ptr<boost::asio::io_service::strand> strand_;
		boost::shared_ptr<timeout_timer_type> timeout_timer_;
		//
		AcceptElement element_;
		TimeoutTimers timeout_timers_;
		//
		Acceptors acceptors_;
		std::deque<AcceptElement> accepted_list_;
		TimeoutTimers timeout_timers2_;
		//
		bool is_processing_;
		bool cancelled_;
		//
		boost::shared_ptr<tTJSVariant> on_finished_;
		//
		mutable mutex_type mutex_;
	private:
		void callOnFinished();
		void postOnFinished();
		void handleMonitor(
			boost::system::error_code const& error
			);
		void handleMonitorSSL(
			boost::system::error_code const& error
			);
		bool setupMonitor(unsigned short port, flag_type protocol_flag);
		bool setupMonitorSSL(unsigned short port, flag_type protocol_flag);
		bool restartMonitor();
		bool restartMonitorSSL();
		bool checkAcceptors();
		void setupOnProcessStart();
		void cleanupOnProcessFailed();
		void cleanupOnProcessSucceeded();
		bool setupAcceptor();
		bool cancelImpl();
		bool cancelTimeoutImpl();
		bool cancelTimeoutImplNoErrorHandling();
	public:
		NativeAcceptor();
		boost::shared_ptr<boost::asio::io_service> const& ioService() const;
		mutex_type& mutex() const;
		bool unfinished() const;
		bool doCancel();
	public:
		//
		//	SUMMARY: 初期化系メソッド
		//
		bool reset();
		//
		//	SUMMARY: 制御系メソッド
		//
		bool cancel();
		bool cancelled() const;
		bool close();
		//
		//	SUMMARY: 監視系メソッド
		//
		bool monitor(unsigned short port, flag_type protocol_flag = NetworkUtils::pfIPv4);
		bool monitorSSL(unsigned short port, flag_type protocol_flag = NetworkUtils::pfIPv4);
		//
		//	SUMMARY: 取得系メソッド
		//
		tTJSVariant popAccepted();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool isProcessing() const;
		size_type acceptedCount() const;
		//
		//	SUMMARY: コールバック系メソッド
		//
		tTJSVariant getOnFinished() const;
		void setOnFinished(tTJSVariant const& func);
	};

	//
	// Acceptor
	//
	class Acceptor
		: public tTJSNativeInstance
	{
	private:
		//
		// AliveHandler
		//
		class AliveHandler {
		public:
			typedef NativeAcceptor::scoped_lock_type scoped_lock_type;
		private:
			boost::shared_ptr<NativeAcceptor> instance_;
		public:
			AliveHandler(boost::shared_ptr<NativeAcceptor> const& instance);
			void operator()() const;
		};
	private:
		boost::shared_ptr<NativeAcceptor> instance_;
	public:
		Acceptor();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: 初期化系メソッド
		//
		bool reset();
		//
		//	SUMMARY: 制御系メソッド
		//
		bool cancel();
		bool cancelled() const;
		bool close();
		//
		//	SUMMARY: 監視系メソッド
		//
		bool monitor(tTVInteger port, tTVInteger protocol_flag = NetworkUtils::pfIPv4);
		bool monitorSSL(tTVInteger port, tTVInteger protocol_flag = NetworkUtils::pfIPv4);
		//
		//	SUMMARY: 取得系メソッド
		//
		tTJSVariant popAccepted();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool isProcessing() const;
		tTVInteger acceptedCount() const;
		//
		//	SUMMARY: コールバック系メソッド
		//
		tTJSVariant getOnFinished() const;
		void setOnFinished(tTJSVariant const& func);
	};
}	// namespace ktl
