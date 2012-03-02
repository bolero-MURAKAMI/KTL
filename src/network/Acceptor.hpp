#pragma once

#include <boost/ref.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <sprig/config/lib/openssl.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "NetworkCommon.hpp"
#include "SocketDecl.hpp"
#include "AcceptorDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Acceptor:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Acceptor:error")

namespace ktl {
	//
	// NativeAcceptor
	//
	void NativeAcceptor::handleMonitor(
		boost::system::error_code const& error
		)
	{
		scoped_lock_type lock(mutex_);
		element_.acceptors.error_code() = error;
		if (!restartMonitor()) {
			return;
		}
		NetworkUtils::asSocket(*element_.socket).close(*element_.acceptors.error_code());
		if (!checkAcceptors()) {
			return;
		}
		element_.acceptors.acceptor()->async_accept(
			NetworkUtils::asSocket(*element_.socket),
			strand_->wrap(
				boost::bind(
					&NativeAcceptor::handleMonitor,
					this,
					boost::asio::placeholders::error
					)
				)
			);
	}
	void NativeAcceptor::handleMonitorSSL(
		boost::system::error_code const& error
		)
	{
		scoped_lock_type lock(mutex_);
		element_.acceptors.error_code() = error;
		if (!restartMonitorSSL()) {
			return;
		}
		NetworkUtils::asSocket(*element_.ssl_socket).close(*element_.acceptors.error_code());
		if (!checkAcceptors()) {
			return;
		}
		element_.acceptors.acceptor()->async_accept(
			NetworkUtils::asSocket(*element_.ssl_socket),
			strand_->wrap(
				boost::bind(
					&NativeAcceptor::handleMonitorSSL,
					this,
					boost::asio::placeholders::error
					)
				)
			);
	}
	KTL_INLINE bool NativeAcceptor::setupMonitor(unsigned short port, flag_type protocol_flag) {
		if (is_processing_) {
			return false;
		}
		setupOnProcessStart();
		acceptors_.error_code().reset();
		element_.acceptors.error_code() = boost::system::error_code();
		element_.acceptors.endpoint() = boost::make_shared<boost::asio::ip::tcp::endpoint>(
			protocol_flag == NetworkUtils::pfIPv6
				? boost::asio::ip::tcp::v6()
				: boost::asio::ip::tcp::v4()
				,
				port
			);
		element_.acceptors.acceptor() = boost::make_shared<boost::asio::ip::tcp::acceptor>(
			boost::ref(*io_service_)
			);
		if (!setupAcceptor()) {
			return false;
		}
		element_.socket = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(*io_service_));
		return true;
	}
	KTL_INLINE bool NativeAcceptor::setupMonitorSSL(unsigned short port, flag_type protocol_flag) {
		if (is_processing_) {
			return false;
		}
		setupOnProcessStart();
		acceptors_.error_code().reset();
		element_.acceptors.error_code() = boost::system::error_code();
		element_.acceptors.context() = boost::make_shared<boost::asio::ssl::context>(
			boost::ref(*io_service_),
			boost::asio::ssl::context::sslv23
			);
		element_.acceptors.endpoint() = boost::make_shared<boost::asio::ip::tcp::endpoint>(
			protocol_flag == NetworkUtils::pfIPv6
				? boost::asio::ip::tcp::v6()
				: boost::asio::ip::tcp::v4()
				,
				port
			);
		element_.acceptors.acceptor() = boost::make_shared<boost::asio::ip::tcp::acceptor>(
			boost::ref(*io_service_)
			);
		if (!setupAcceptor()) {
			return false;
		}
		element_.ssl_socket = boost::make_shared<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >(
			boost::ref(*io_service_),
			boost::ref(*element_.acceptors.context())
			);
		return true;
	}
	KTL_INLINE bool NativeAcceptor::restartMonitor() {
		if (!checkAcceptors()) {
			return false;
		}
		accepted_list_.push_back(element_);
		acceptors_ = element_.acceptors;

		element_.acceptors.error_code() = boost::system::error_code();
		element_.socket = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(*io_service_));
		return true;
	}
	KTL_INLINE bool NativeAcceptor::restartMonitorSSL() {
		if (!checkAcceptors()) {
			return false;
		}
		accepted_list_.push_back(element_);
		acceptors_ = element_.acceptors;

		element_.acceptors.error_code() = boost::system::error_code();
		element_.ssl_socket = boost::make_shared<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >(
			boost::ref(*io_service_),
			boost::ref(*element_.acceptors.context())
			);
		return true;
	}
	KTL_INLINE bool NativeAcceptor::checkAcceptors() {
		if (*element_.acceptors.error_code()) {
			NetworkUtils::moveErrorCode(acceptors_, element_.acceptors);
			cleanupOnProcessFailed();
			return false;
		}
		return true;
	}
	KTL_INLINE void NativeAcceptor::setupOnProcessStart() {
		is_processing_ = true;
		cancelled_ = false;
	}
	KTL_INLINE void NativeAcceptor::cleanupOnProcessFailed() {
		cancelTimeoutImplNoErrorHandling();
		is_processing_ = false;
	}
	KTL_INLINE void NativeAcceptor::cleanupOnProcessSucceeded() {
		cancelTimeoutImplNoErrorHandling();
		is_processing_ = false;
	}
	KTL_INLINE bool NativeAcceptor::setupAcceptor() {
		element_.acceptors.acceptor()->open(
			element_.acceptors.endpoint()->protocol(),
			*element_.acceptors.error_code()
			);
		if (!checkAcceptors()) {
			return false;
		}
		element_.acceptors.acceptor()->set_option(
			boost::asio::socket_base::reuse_address(true),
			*element_.acceptors.error_code()
			);
		if (!checkAcceptors()) {
			return false;
		}
		element_.acceptors.acceptor()->bind(
			*element_.acceptors.endpoint(),
			*element_.acceptors.error_code()
			);
		if (!checkAcceptors()) {
			return false;
		}
		element_.acceptors.acceptor()->listen(
			boost::asio::socket_base::max_connections,
			*element_.acceptors.error_code()
			);
		if (!checkAcceptors()) {
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeAcceptor::cancelImpl() {
		if (!is_processing_) {
			return false;
		}
		cancelled_ = true;
		if (element_.acceptors.enable()) {
			boost::system::error_code error;
			element_.acceptors.acceptor()->close(error);
			if (error) {
				return false;
			}
		}
		return true;
	}
	KTL_INLINE bool NativeAcceptor::cancelTimeoutImpl() {
		if (!timeout_timers2_.enable()) {
			return false;
		}
		boost::system::error_code error;
		timeout_timer_->cancel(error);
		if (error) {
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeAcceptor::cancelTimeoutImplNoErrorHandling() {
		if (!timeout_timers2_.enable()) {
			return false;
		}
		timeout_timer_->cancel();
		return true;
	}
	NativeAcceptor::NativeAcceptor()
		: io_service_(NetworkService::get_const_instance().io_service())
		, strand_(boost::make_shared<boost::asio::io_service::strand>(boost::ref(*io_service_)))
		, timeout_timer_(boost::make_shared<timeout_timer_type>(boost::ref(*io_service_)))
		, is_processing_(false)
		, cancelled_(false)
	{}
	KTL_INLINE boost::shared_ptr<boost::asio::io_service> const& NativeAcceptor::ioService() const {
		return io_service_;
	}
	KTL_INLINE NativeSocket::mutex_type& NativeAcceptor::mutex() const {
		return mutex_;
	}
	KTL_INLINE bool NativeAcceptor::unfinished() const {
		return is_processing_;
	}
	KTL_INLINE bool NativeAcceptor::doCancel() {
		return cancelImpl();
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE bool NativeAcceptor::reset() {
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			return false;
		}
		//
		is_processing_ = false;
		cancelled_ = false;
		//
		element_ = AcceptElement();
		acceptors_.reset();
		accepted_list_.clear();
		return true;
	}
	//
	//	SUMMARY: 制御系メソッド
	//
	KTL_INLINE bool NativeAcceptor::cancel() {
		scoped_lock_type lock(mutex_);
		return cancelImpl();
	}
	KTL_INLINE bool NativeAcceptor::cancelled() const {
		scoped_lock_type lock(mutex_);
		return cancelled_;
	}
	KTL_INLINE bool NativeAcceptor::close() {
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			if (!cancelImpl()) {
				return false;
			}
		}
		accepted_list_.clear();
		return true;
	}
	//
	//	SUMMARY: 監視系メソッド
	//
	KTL_INLINE bool NativeAcceptor::monitor(unsigned short port, flag_type protocol_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupMonitor(port, protocol_flag)) {
			return false;
		}
		NetworkUtils::asSocket(*element_.socket).close(*element_.acceptors.error_code());
		if (!checkAcceptors()) {
			return false;
		}
		element_.acceptors.acceptor()->async_accept(
			NetworkUtils::asSocket(*element_.socket),
			strand_->wrap(
				boost::bind(
					&NativeAcceptor::handleMonitor,
					this,
					boost::asio::placeholders::error
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeAcceptor::monitorSSL(unsigned short port, flag_type protocol_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupMonitorSSL(port, protocol_flag)) {
			return false;
		}
		NetworkUtils::asSocket(*element_.ssl_socket).close(*element_.acceptors.error_code());
		if (!checkAcceptors()) {
			return false;
		}
		element_.acceptors.acceptor()->async_accept(
			NetworkUtils::asSocket(*element_.ssl_socket),
			strand_->wrap(
				boost::bind(
					&NativeAcceptor::handleMonitorSSL,
					this,
					boost::asio::placeholders::error
					)
				)
			);
		return true;
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE tTJSVariant NativeAcceptor::popAccepted() {
		scoped_lock_type lock(mutex_);
		if (accepted_list_.empty()) {
			return tTJSVariant();
		}
		sprig::krkr::tjs::object_type result(NativeSocket::createNew(0, 0));
		{
			Socket* instance = reinterpret_cast<Socket*>(NativeSocket::getInstance(result.get()));
			{
				AcceptElement const& accepted = accepted_list_.front();
				if (accepted.ssl_socket) {
					if (!instance->resetExternalAcceptors(accepted.acceptors, accepted.ssl_socket)) {
						return tTJSVariant();
					}
				} else if (accepted.socket) {
					if (!instance->resetExternalAcceptors(accepted.acceptors, accepted.socket)) {
						return tTJSVariant();
					}
				}
			}
			accepted_list_.pop_front();
		}
		return tTJSVariant(result.get(), result.get());
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool NativeAcceptor::isProcessing() const {
		scoped_lock_type lock(mutex_);
		return is_processing_;
	}
	KTL_INLINE NativeAcceptor::size_type NativeAcceptor::acceptedCount() const {
		scoped_lock_type lock(mutex_);
		return accepted_list_.size();
	}

	//
	// Acceptor::AliveHandler
	//
	Acceptor::AliveHandler::AliveHandler(boost::shared_ptr<NativeAcceptor> const& instance)
		: instance_(instance)
	{}
	KTL_INLINE void Acceptor::AliveHandler::operator()() const {
		scoped_lock_type lock(instance_->mutex());
		if (instance_->unfinished()) {
			instance_->ioService()->post(*this);
		}
	}
	//
	// Acceptor
	//
	Acceptor::Acceptor() {}
	tjs_error TJS_INTF_METHOD Acceptor::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Acceptor::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeAcceptor>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Acceptor::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Acceptor::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE bool Acceptor::reset() {
		return instance_->reset();
	}
	//
	//	SUMMARY: 制御系メソッド
	//
	KTL_INLINE bool Acceptor::cancel() {
		return instance_->cancel();
	}
	KTL_INLINE bool Acceptor::cancelled() const {
		return instance_->cancelled();
	}
	KTL_INLINE bool Acceptor::close() {
		return instance_->close();
	}
	//
	//	SUMMARY: 監視系メソッド
	//
	KTL_INLINE bool Acceptor::monitor(tTVInteger port, tTVInteger protocol_flag) {
		return instance_->monitor(
			boost::numeric_cast<unsigned short>(port),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(protocol_flag)
			);
	}
	KTL_INLINE bool Acceptor::monitorSSL(tTVInteger port, tTVInteger protocol_flag) {
		return instance_->monitorSSL(
			boost::numeric_cast<unsigned short>(port),
			sprig::numeric::bit_cast<NativeAcceptor::flag_type>(protocol_flag)
			);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE tTJSVariant Acceptor::popAccepted() {
		return instance_->popAccepted();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool Acceptor::isProcessing() const {
		return instance_->isProcessing();
	}
	KTL_INLINE tTVInteger Acceptor::acceptedCount() const {
		return instance_->acceptedCount();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
