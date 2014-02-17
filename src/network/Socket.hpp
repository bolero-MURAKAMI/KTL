/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_NETWORK_SOCKET_HPP
#define SRC_NETWORK_SOCKET_HPP

#include <cstddef>
#include <cstring>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/ref.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <sprig/config/lib/openssl.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/as_const.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/str_length.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>
#include <ktl/thread_callback.hpp>

#include "NetworkCommon.hpp"
#include "SocketDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Socket:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Socket:error")

namespace ktl {
	//
	// NativeSocket::MatchCondition
	//
	template<typename Iterator>
	KTL_INLINE NativeSocket::MatchCondition::MatchCondition(Iterator first, Iterator last)
		: data_(first, last)
	{}
	template<typename Iterator>
	KTL_INLINE std::pair<Iterator, bool> NativeSocket::MatchCondition::operator()(Iterator first, Iterator last) const {
		Iterator found = std::search(first, last, data_.begin(), data_.end());
		if (found != last) {
			std::advance(found, data_.size());
			return std::pair<Iterator, bool>(found, true);
		}
		return std::pair<Iterator, bool>(first, false);
	}
	//
	// NativeSocket
	//
	KTL_INLINE NativeSocket::size_type NativeSocket::getEnableSize(size_type length, size_type spos) {
		return spos <= length
			? length - spos
			: 0
			;
	}
	KTL_INLINE NativeSocket::size_type NativeSocket::getEnableSize(size_type length, size_type spos, size_type ssize) {
		return spos + ssize <= length
			? ssize
			: spos <= length
				? length - spos
				: 0
			;
	}
	KTL_INLINE boost::optional<boost::asio::ip::address_v4> NativeSocket::convertAddressIPv4(tTJSVariant* address) {
		switch (address->Type()) {
		case tvtString:
			{
				boost::system::error_code error;
				boost::asio::ip::address_v4 v4 = boost::asio::ip::address_v4::from_string(
					sprig::str_cast<impl_string_type>(sprig::krkr::tjs::as_c_str(address->AsStringNoAddRef())),
					error
					);
				if (error) {
					return boost::none;
				}
				return v4;
			}
		case tvtOctet:
			{
				tTJSVariantOctet const* octet = address->AsOctetNoAddRef();
				tjs_uint length = sprig::krkr::tjs::octet_length(octet);
				if (length != 4) {
					return boost::none;
				}
				tjs_uint8 const* data = sprig::krkr::tjs::octet_data(octet);
				boost::asio::ip::address_v4::bytes_type bytes = { {
					data[0],
					data[1],
					data[2],
					data[3]
				} };
				return boost::asio::ip::address_v4(bytes);
			}
		case tvtInteger:
			return boost::asio::ip::address_v4(
				static_cast<unsigned long>(
					static_cast<tTVInteger>(*address)
					)
				);
		default:
			return boost::none;
		}
		return boost::none;
	}
	KTL_INLINE boost::optional<boost::asio::ip::address_v6> NativeSocket::convertAddressIPv6(tTJSVariant* address) {
		switch (address->Type()) {
		case tvtString:
			{
				boost::system::error_code error;
				boost::asio::ip::address_v6 v6 = boost::asio::ip::address_v6::from_string(
					sprig::str_cast<impl_string_type>(sprig::krkr::tjs::as_c_str(address->AsStringNoAddRef())),
					error
					);
				if (error) {
					return boost::none;
				}
				return v6;
			}
		case tvtOctet:
			{
				tTJSVariantOctet const* octet = address->AsOctetNoAddRef();
				tjs_uint length = sprig::krkr::tjs::octet_length(octet);
				if (length != 16) {
					return boost::none;
				}
				tjs_uint8 const* data = sprig::krkr::tjs::octet_data(octet);
				boost::asio::ip::address_v6::bytes_type bytes = { {
					data[0],
					data[1],
					data[2],
					data[3],
					data[4],
					data[5],
					data[6],
					data[7],
					data[8],
					data[9],
					data[10],
					data[11],
					data[12],
					data[13],
					data[14],
					data[15]
				} };
				return boost::asio::ip::address_v6(bytes);
			}
		default:
			return boost::none;
		}
		return boost::none;
	}
	KTL_INLINE boost::optional<boost::asio::ip::address> NativeSocket::convertAddress(tTJSVariant* address) {
		boost::optional<boost::asio::ip::address> addr;
		if (boost::optional<boost::asio::ip::address_v4> v4 = convertAddressIPv4(address)) {
			addr = *v4;
		} else if (boost::optional<boost::asio::ip::address_v6> v6 = convertAddressIPv6(address)) {
			addr = *v6;
		}
		return addr;
	}
	sprig::krkr::tjs::intptr_type NativeSocket::getInstance(iTJSDispatch2* obj) {
		return sprig::krkr::tjs::GetPropValue<sprig::krkr::tjs::intptr_type>(
			sprig::krkr::tjs::GetMemberNoAddRef(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Socket")),
				SPRIG_KRKR_TJS_W("instance")
				),
			obj
			);
	}
	sprig::krkr::tjs::object_type NativeSocket::createNew(tjs_int numparams, tTJSVariant** param) {
		iTJSDispatch2* result_obj = 0;
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Socket")),
			&result_obj, numparams, param, 0
			);
		return sprig::krkr::tjs::object_type(result_obj, false);
	}
	void NativeSocket::callOnFinished() {
		tTJSVariant on_finished;
		{
			scoped_lock_type lock(mutex_);
			if (!(on_finished_ && on_finished_->Type() == tvtObject)) {
				return;
			}
			on_finished = *on_finished_;
			on_finished_.reset();
		}
		{
			tTJSVariantClosure closure(on_finished.AsObjectClosureNoAddRef());
			sprig::krkr::tjs::FuncObjectCall(
				closure.Object,
				0,
				0,
				0,
				closure.ObjThis
				);
		}
	}
	KTL_INLINE void NativeSocket::postOnFinished() {
		if (on_finished_) {
			ktl::thread_callback::post(
				boost::bind(&NativeSocket::callOnFinished, this)
				);
		}
	}
	void NativeSocket::handleResolove(
		boost::system::error_code const& error,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			resolvers2_.error_code() = error;
			finishResolve(endpoint_iterator);
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeSocket::handleConnect(
		boost::system::error_code const& error,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			connectors2_.error_code() = error;
			finishConnect();
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeSocket::handleConnectSSL(
		boost::system::error_code const& error,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			connectors2_.error_code() = error;
			finishConnectSSL();
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeSocket::handleAccept(
		boost::system::error_code const& error
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			acceptors2_.error_code() = error;
			finishAccept();
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeSocket::handleAcceptSSL(
		boost::system::error_code const& error
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			acceptors2_.error_code() = error;
			finishAcceptSSL();
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeSocket::handleHandshake(
		boost::system::error_code const& error
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			handshakers2_.error_code() = error;
			finishHandshake();
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeSocket::handleWrite(
		boost::system::error_code const& error,
		size_type bytes_transferred
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			writers2_.error_code() = error;
			finishWrite();
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeSocket::handleRead(
		boost::system::error_code const& error,
		size_type bytes_transferred
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			readers2_.error_code() = error;
			finishRead();
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeSocket::handleReadUntil(
		boost::system::error_code const& error,
		size_type bytes_transferred
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			readers2_.error_code() = error;
			finishRead();
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeSocket::handleTimeout(
		boost::system::error_code const& error
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			timeout_timers2_.error_code() = error;
			if (*timeout_timers2_.error_code()) {
				NetworkUtils::moveErrorCode(timeout_timers_, timeout_timers2_);
				return;
			}
			cancelImpl();
			NetworkUtils::moveComponent(timeout_timers_, timeout_timers2_);
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	KTL_INLINE bool NativeSocket::setupResolve(impl_string_type const& host_name, impl_string_type const& service_name) {
		if (is_processing_) {
			return false;
		}
		setupOnProcessStart();
		resolvers_.error_code().reset();
		resolvers2_.resolver() = boost::make_shared<boost::asio::ip::tcp::resolver>(boost::ref(*io_service_));
		resolvers2_.query() = boost::make_shared<boost::asio::ip::tcp::resolver::query>(host_name, service_name);
		resolvers2_.error_code() = boost::system::error_code();
		return true;
	}
	KTL_INLINE bool NativeSocket::setupConnect() {
		if (is_processing_) {
			return false;
		}
		if (!resolvers_.error_code() || *resolvers_.error_code()) {
			return false;
		}
		setupOnProcessStart();
		connectors_.error_code().reset();
		connectors2_.error_code() = boost::system::error_code();
		socket2_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(*io_service_));
		return true;
	}
	KTL_INLINE bool NativeSocket::setupConnect(boost::optional<boost::asio::ip::address> const& addr, unsigned short port) {
		if (is_processing_) {
			return false;
		}
		if (!addr) {
			return false;
		}
		setupOnProcessStart();
		connectors_.error_code().reset();
		connectors2_.error_code() = boost::system::error_code();
		socket2_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(*io_service_));
		return true;
	}
	KTL_INLINE bool NativeSocket::setupConnectSSL() {
		if (is_processing_) {
			return false;
		}
		if (!resolvers_.error_code() || *resolvers_.error_code()) {
			return false;
		}
		setupOnProcessStart();
		connectors_.error_code().reset();
		connectors2_.error_code() = boost::system::error_code();
		connectors2_.context() = boost::make_shared<boost::asio::ssl::context>(
			boost::ref(*io_service_),
			boost::asio::ssl::context::sslv23
			);
		ssl_socket2_ = boost::make_shared<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >(
			boost::ref(*io_service_),
			boost::ref(*connectors2_.context())
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::setupConnectSSL(boost::optional<boost::asio::ip::address> const& addr, unsigned short port) {
		if (is_processing_) {
			return false;
		}
		if (!addr) {
			return false;
		}
		setupOnProcessStart();
		connectors_.error_code().reset();
		connectors2_.error_code() = boost::system::error_code();
		connectors2_.context() = boost::make_shared<boost::asio::ssl::context>(
			boost::ref(*io_service_),
			boost::asio::ssl::context::sslv23
			);
		ssl_socket2_ = boost::make_shared<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >(
			boost::ref(*io_service_),
			boost::ref(*connectors2_.context())
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::setupConnectLoopback(unsigned short port, flag_type protocol_flag) {
		if (is_processing_) {
			return false;
		}
		setupOnProcessStart();
		connectors_.error_code().reset();
		connectors2_.error_code() = boost::system::error_code();
		socket2_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(*io_service_));
		return true;
	}
	KTL_INLINE bool NativeSocket::setupConnectLoopbackSSL(unsigned short port, flag_type protocol_flag) {
		if (is_processing_) {
			return false;
		}
		setupOnProcessStart();
		connectors_.error_code().reset();
		connectors2_.error_code() = boost::system::error_code();
		connectors2_.context() = boost::make_shared<boost::asio::ssl::context>(
			boost::ref(*io_service_),
			boost::asio::ssl::context::sslv23
			);
		ssl_socket2_ = boost::make_shared<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >(
			boost::ref(*io_service_),
			boost::ref(*connectors2_.context())
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::setupAccept(unsigned short port, flag_type protocol_flag) {
		if (is_processing_) {
			return false;
		}
		setupOnProcessStart();
		acceptors_.error_code().reset();
		acceptors2_.error_code() = boost::system::error_code();
		acceptors2_.endpoint() = boost::make_shared<boost::asio::ip::tcp::endpoint>(
			protocol_flag == NetworkUtils::pfIPv6
				? boost::asio::ip::tcp::v6()
				: boost::asio::ip::tcp::v4()
				,
				port
			);
		acceptors2_.acceptor() = boost::make_shared<boost::asio::ip::tcp::acceptor>(
			boost::ref(*io_service_)
			);
		if (!setupAcceptor()) {
			return false;
		}
		socket2_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(*io_service_));
		return true;
	}
	KTL_INLINE bool NativeSocket::setupAcceptSSL(unsigned short port, flag_type protocol_flag) {
		if (is_processing_) {
			return false;
		}
		setupOnProcessStart();
		acceptors_.error_code().reset();
		acceptors2_.error_code() = boost::system::error_code();
		acceptors2_.context() = boost::make_shared<boost::asio::ssl::context>(
			boost::ref(*io_service_),
			boost::asio::ssl::context::sslv23
			);
		acceptors2_.endpoint() = boost::make_shared<boost::asio::ip::tcp::endpoint>(
			protocol_flag == NetworkUtils::pfIPv6
				? boost::asio::ip::tcp::v6()
				: boost::asio::ip::tcp::v4()
				,
				port
			);
		acceptors2_.acceptor() = boost::make_shared<boost::asio::ip::tcp::acceptor>(
			boost::ref(*io_service_)
			);
		if (!setupAcceptor()) {
			return false;
		}
		ssl_socket2_ = boost::make_shared<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >(
			boost::ref(*io_service_),
			boost::ref(*acceptors2_.context())
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::setupHandshake() {
		if (is_processing_) {
			return false;
		}
		if (!ssl_socket_) {
			return false;
		}
		setupOnProcessStart();
		handshakers_.error_code().reset();
		handshakers2_.error_code() = boost::system::error_code();
		return true;
	}
	KTL_INLINE bool NativeSocket::setupWrite() {
		if (is_processing_) {
			return false;
		}
		if (!socket_ && !ssl_socket_) {
			return false;
		}
		setupOnProcessStart();
		writers_.error_code().reset();
		writers2_.error_code() = boost::system::error_code();
		return true;
	}
	KTL_INLINE bool NativeSocket::setupRead() {
		if (is_processing_) {
			return false;
		}
		if (!socket_ && !ssl_socket_) {
			return false;
		}
		setupOnProcessStart();
		readers_.error_code().reset();
		readers2_.error_code() = boost::system::error_code();
		return true;
	}
	KTL_INLINE bool NativeSocket::finishResolve(boost::asio::ip::tcp::resolver::iterator endpoint_iterator) {
		if (!checkResolvers()) {
			return false;
		}
		resolvers2_.iterator() = boost::make_shared<boost::asio::ip::tcp::resolver::iterator>(endpoint_iterator);
		NetworkUtils::moveComponent(resolvers_, resolvers2_);
		cleanupOnProcessSucceeded();
		return true;
	}
	KTL_INLINE bool NativeSocket::finishConnect() {
		if (!checkConnectors()) {
			return false;
		}
		NetworkUtils::moveComponent(connectors_, connectors2_);
		enableSocket();
		cleanupOnProcessSucceeded();
		return true;
	}
	KTL_INLINE bool NativeSocket::finishConnectSSL() {
		if (!checkConnectors()) {
			return false;
		}
		NetworkUtils::moveComponent(connectors_, connectors2_);
		enableSocketSSL();
		cleanupOnProcessSucceeded();
		return true;
	}
	KTL_INLINE bool NativeSocket::finishAccept() {
		if (!checkAcceptors()) {
			return false;
		}
		if (acceptors2_.acceptor() && acceptors2_.acceptor()->is_open()) {
			boost::system::error_code error;
			acceptors2_.acceptor()->close(error);
			if (error) {
				NetworkUtils::moveErrorCode(acceptors_, acceptors2_);
				cleanupOnProcessFailed();
				return false;
			}
		}
		NetworkUtils::moveComponent(acceptors_, acceptors2_);
		enableSocket();
		cleanupOnProcessSucceeded();
		return true;
	}
	KTL_INLINE bool NativeSocket::finishAcceptSSL() {
		if (!checkAcceptors()) {
			return false;
		}
		if (acceptors2_.acceptor() && acceptors2_.acceptor()->is_open()) {
			boost::system::error_code error;
			acceptors2_.acceptor()->close(error);
			if (error) {
				NetworkUtils::moveErrorCode(acceptors_, acceptors2_);
				cleanupOnProcessFailed();
				return false;
			}
		}
		NetworkUtils::moveComponent(acceptors_, acceptors2_);
		enableSocketSSL();
		cleanupOnProcessSucceeded();
		return true;
	}
	KTL_INLINE bool NativeSocket::finishHandshake() {
		if (!checkHandshakers()) {
			return false;
		}
		NetworkUtils::moveComponent(handshakers_, handshakers2_);
		cleanupOnProcessSucceeded();
		return true;
	}
	KTL_INLINE bool NativeSocket::finishWrite() {
		if (!checkWriters()) {
			return false;
		}
		NetworkUtils::moveComponent(writers_, writers2_);
		cleanupOnProcessSucceeded();
		return true;
	}
	KTL_INLINE bool NativeSocket::finishRead() {
		if (!checkReaders()) {
			return false;
		}
		NetworkUtils::moveComponent(readers_, readers2_);
		cleanupOnProcessSucceeded();
		return true;
	}
	KTL_INLINE bool NativeSocket::checkResolvers() {
		if (*resolvers2_.error_code()) {
			NetworkUtils::moveErrorCode(resolvers_, resolvers2_);
			cleanupOnProcessFailed();
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeSocket::checkConnectors() {
		if (*connectors2_.error_code()) {
			NetworkUtils::moveErrorCode(connectors_, connectors2_);
			cleanupOnProcessFailed();
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeSocket::checkAcceptors() {
		if (*acceptors2_.error_code()) {
			NetworkUtils::moveErrorCode(acceptors_, acceptors2_);
			cleanupOnProcessFailed();
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeSocket::checkHandshakers() {
		if (*handshakers2_.error_code()) {
			NetworkUtils::moveErrorCode(handshakers_, handshakers2_);
			cleanupOnProcessFailed();
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeSocket::checkWriters() {
		if (*writers2_.error_code()) {
			NetworkUtils::moveErrorCode(writers_, writers2_);
			cleanupOnProcessFailed();
			return false;
		}
		return true;
	}
	KTL_INLINE bool NativeSocket::checkReaders() {
		if (*readers2_.error_code()) {
			NetworkUtils::moveErrorCode(readers_, readers2_);
			cleanupOnProcessFailed();
			return false;
		}
		return true;
	}
	KTL_INLINE void NativeSocket::enableSocket() {
		NetworkUtils::moveComponent(socket_, socket2_);
		NetworkUtils::resetComponent(ssl_socket_, ssl_socket2_);
	}
	KTL_INLINE void NativeSocket::enableSocketSSL() {
		NetworkUtils::moveComponent(ssl_socket_, ssl_socket2_);
		NetworkUtils::resetComponent(socket_, socket2_);
	}
	KTL_INLINE void NativeSocket::setupOnProcessStart() {
		is_processing_ = true;
		cancelled_ = false;
	}
	KTL_INLINE void NativeSocket::cleanupOnProcessFailed() {
		cancelTimeoutImplNoErrorHandling();
		is_processing_ = false;
		postOnFinished();
	}
	KTL_INLINE void NativeSocket::cleanupOnProcessSucceeded() {
		cancelTimeoutImplNoErrorHandling();
		is_processing_ = false;
		postOnFinished();
	}
	KTL_INLINE bool NativeSocket::setupAcceptor() {
		acceptors2_.acceptor()->open(
			acceptors2_.endpoint()->protocol(),
			*acceptors2_.error_code()
			);
		if (!checkAcceptors()) {
			return false;
		}
		acceptors2_.acceptor()->set_option(
			boost::asio::socket_base::reuse_address(true),
			*acceptors2_.error_code()
			);
		if (!checkAcceptors()) {
			return false;
		}
		acceptors2_.acceptor()->bind(
			*acceptors2_.endpoint(),
			*acceptors2_.error_code()
			);
		if (!checkAcceptors()) {
			return false;
		}
		acceptors2_.acceptor()->listen(
			boost::asio::socket_base::max_connections,
			*acceptors2_.error_code()
			);
		if (!checkAcceptors()) {
			return false;
		}
		return true;
	}
	template<typename Char>
	KTL_INLINE void NativeSocket::copyBuffer(
		boost::asio::streambuf const& buf,
		Char* dest,
		size_type pos,
		size_type size
		) const
	{
		typedef boost::asio::streambuf::const_buffers_type const_buffers_type;
		typedef const_buffers_type::value_type const_buffer_type;
		size_type current_size = 0;
		size_type remaind_size = pos;
		const_buffers_type const_buffers = buf.data();
		BOOST_FOREACH(const_buffer_type const_buffer, sprig::as_const(const_buffers)) {
			Char const* buffer_data = boost::asio::buffer_cast<Char const*>(const_buffer);
			size_type buffer_size = boost::asio::buffer_size(const_buffer);
			if (remaind_size) {
				if (remaind_size < buffer_size) {
					buffer_data += remaind_size;
					buffer_size -= remaind_size;
					remaind_size = 0;
				} else {
					remaind_size -= buffer_size;
					continue;
				}
			}
			size_type copy_size = (std::min)(buffer_size, size - current_size);
			std::memcpy(
				dest + current_size,
				buffer_data,
				copy_size
				);
			current_size += copy_size;
			if (current_size >= size) {
				break;
			}
		}
	}
	KTL_INLINE tTJSVariant NativeSocket::getBufferAsOctetImpl(
		boost::asio::streambuf const& buf,
		size_type pos,
		size_type size
		) const
	{
		typedef boost::asio::streambuf::const_buffers_type const_buffers_type;
		typedef const_buffers_type::value_type const_buffer_type;
		if (!size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		std::vector<tjs_uint8> binary(size);
		copyBuffer(buf, &binary[0], pos, size);
		return tTJSVariant(&binary[0], size);
	}
	KTL_INLINE tTJSVariant NativeSocket::getBufferAsStringImpl(
		boost::asio::streambuf const& buf,
		size_type pos,
		size_type size
		) const
	{
		typedef boost::asio::streambuf::const_buffers_type const_buffers_type;
		typedef const_buffers_type::value_type const_buffer_type;
		std::vector<char> binary(size + (sizeof(tjs_char) * 2 - 1));
		if (!size) {
			return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
		}
		copyBuffer(buf, &binary[0], pos, size);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
	}
	KTL_INLINE tTJSVariant NativeSocket::getBufferAsNarrowStringImpl(
		boost::asio::streambuf const& buf,
		size_type pos,
		size_type size
		) const
	{
		typedef boost::asio::streambuf::const_buffers_type const_buffers_type;
		typedef const_buffers_type::value_type const_buffer_type;
		std::vector<char> binary(size + (sizeof(tjs_nchar) * 2 - 1));
		if (!size) {
			return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&binary[0]));
		}
		copyBuffer(buf, &binary[0], pos, size);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&binary[0]));
	}
	KTL_INLINE bool NativeSocket::cancelImpl() {
		if (!is_processing_) {
			return false;
		}
		cancelled_ = true;
		if (resolvers2_.enable()) {
			resolvers2_.resolver()->cancel();
		}
		if (connectors2_.enable()) {
			boost::system::error_code error;
			if (socket2_ && NetworkUtils::asSocket(*socket2_).is_open()) {
				NetworkUtils::asSocket(*socket2_).shutdown(boost::asio::socket_base::shutdown_both, error);
				NetworkUtils::asSocket(*socket2_).close(error);
			} else if (ssl_socket2_ && NetworkUtils::asSocket(*ssl_socket2_).is_open()) {
				NetworkUtils::asSocket(*ssl_socket2_).shutdown(boost::asio::socket_base::shutdown_both, error);
				NetworkUtils::asSocket(*ssl_socket2_).close(error);
			}
			if (error) {
				return false;
			}
		}
		if (acceptors2_.enable()) {
			boost::system::error_code error;
			acceptors2_.acceptor()->close(error);
			if (error) {
				return false;
			}
		}
		if (handshakers2_.enable()) {
			boost::system::error_code error;
			if (ssl_socket_ && NetworkUtils::asSocket(*ssl_socket_).is_open()) {
				NetworkUtils::asSocket(*ssl_socket_).shutdown(boost::asio::socket_base::shutdown_both, error);
				NetworkUtils::asSocket(*ssl_socket_).close(error);
			}
			if (error) {
				return false;
			}
		}
		if (writers2_.enable() || readers2_.enable()) {
			boost::system::error_code error;
			if (socket_ && NetworkUtils::asSocket(*socket_).is_open()) {
				NetworkUtils::asSocket(*socket_).shutdown(boost::asio::socket_base::shutdown_both, error);
				NetworkUtils::asSocket(*socket_).close(error);
			} else if (ssl_socket_ && NetworkUtils::asSocket(*ssl_socket_).is_open()) {
				NetworkUtils::asSocket(*ssl_socket_).shutdown(boost::asio::socket_base::shutdown_both, error);
				NetworkUtils::asSocket(*ssl_socket_).close(error);
			}
			if (error) {
				return false;
			}
		}
		return true;
	}
	KTL_INLINE bool NativeSocket::cancelTimeoutImpl() {
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
	KTL_INLINE bool NativeSocket::cancelTimeoutImplNoErrorHandling() {
		if (!timeout_timers2_.enable()) {
			return false;
		}
		timeout_timer_->cancel();
		return true;
	}
	NativeSocket::NativeSocket()
		: io_service_(NetworkService::get_const_instance().io_service())
		, strand_(boost::make_shared<boost::asio::io_service::strand>(boost::ref(*io_service_)))
		, writing_streambuf_(boost::make_shared<boost::asio::streambuf>())
		, reading_streambuf_(boost::make_shared<boost::asio::streambuf>())
		, timeout_timer_(boost::make_shared<timeout_timer_type>(boost::ref(*io_service_)))
		, is_processing_(false)
		, cancelled_(false)
	{}
	KTL_INLINE boost::shared_ptr<boost::asio::io_service> const& NativeSocket::ioService() const {
		return io_service_;
	}
	KTL_INLINE NativeSocket::mutex_type& NativeSocket::mutex() const {
		return mutex_;
	}
	KTL_INLINE bool NativeSocket::unfinished() const {
		return is_processing_;
	}
	KTL_INLINE bool NativeSocket::doCancel() {
		return cancelImpl();
	}
	KTL_INLINE bool NativeSocket::resetExternalAcceptors(
		Acceptors const& acceptors,
		boost::shared_ptr<boost::asio::ip::tcp::socket> const& socket
		)
	{
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			return false;
		}
		acceptors_ = acceptors;
		socket_ = socket;
		return true;
	}
	KTL_INLINE bool NativeSocket::resetExternalAcceptors(
		Acceptors const& acceptors,
		boost::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> > const& ssl_socket
		)
	{
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			return false;
		}
		acceptors_ = acceptors;
		ssl_socket_ = ssl_socket;
		return true;
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE bool NativeSocket::reset() {
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			return false;
		}
		//
		is_processing_ = false;
		cancelled_ = false;
		//
		NetworkUtils::resetComponent(resolvers_, resolvers2_);
		NetworkUtils::resetComponent(connectors_, connectors2_);
		NetworkUtils::resetComponent(acceptors_, acceptors2_);
		NetworkUtils::resetComponent(handshakers_, handshakers2_);
		NetworkUtils::resetComponent(writers_, writers2_);
		NetworkUtils::resetComponent(readers_, readers2_);
		//
		reading_streambuf_ = boost::make_shared<boost::asio::streambuf>();
		writing_streambuf_ = boost::make_shared<boost::asio::streambuf>();
		//
		NetworkUtils::resetComponent(socket_, socket2_);
		NetworkUtils::resetComponent(ssl_socket_, ssl_socket2_);
		return true;
	}
	//
	//	SUMMARY: 制御系メソッド
	//
	KTL_INLINE bool NativeSocket::cancel() {
		scoped_lock_type lock(mutex_);
		return cancelImpl();
	}
	KTL_INLINE bool NativeSocket::cancelled() const {
		scoped_lock_type lock(mutex_);
		return cancelled_;
	}
	KTL_INLINE bool NativeSocket::close() {
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			if (!cancelImpl()) {
				return false;
			}
		}
		if (socket_ && NetworkUtils::asSocket(*socket_).is_open()) {
			boost::system::error_code error;
			NetworkUtils::asSocket(*socket_).shutdown(boost::asio::socket_base::shutdown_both, error);
			NetworkUtils::asSocket(*socket_).close(error);
			if (error) {
				return false;
			}
		}
		if (ssl_socket_ && NetworkUtils::asSocket(*ssl_socket_).is_open()) {
			boost::system::error_code error;
			NetworkUtils::asSocket(*ssl_socket_).shutdown(boost::asio::socket_base::shutdown_both, error);
			NetworkUtils::asSocket(*ssl_socket_).close(error);
			if (error) {
				return false;
			}
		}
		return true;
	}
	//
	//	SUMMARY: 名前解決系メソッド
	//
	KTL_INLINE bool NativeSocket::resolve(impl_string_type const& host_name, impl_string_type const& service_name) {
		scoped_lock_type lock(mutex_);
		if (!setupResolve(host_name, service_name)) {
			return false;
		}
		return finishResolve(
			resolvers2_.resolver()->resolve(*resolvers2_.query(), *resolvers2_.error_code())
			);
	}
	KTL_INLINE bool NativeSocket::asyncResolve(impl_string_type const& host_name, impl_string_type const& service_name) {
		scoped_lock_type lock(mutex_);
		if (!setupResolve(host_name, service_name)) {
			return false;
		}
		resolvers2_.resolver()->async_resolve(
			*resolvers2_.query(),
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleResolove,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::iterator
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::resolvingFinished() const {
		scoped_lock_type lock(mutex_);
		return resolvers_.enable();
	}
	KTL_INLINE bool NativeSocket::resolvingSucceeded() const {
		scoped_lock_type lock(mutex_);
		return resolvers_.error_code() && !*resolvers_.error_code();
	}
	KTL_INLINE tTJSVariant NativeSocket::getEndpointList() const {
		scoped_lock_type lock(mutex_);
		if (!resolvers_.error_code() || *resolvers_.error_code()) {
			return tTJSVariant();
		}
		sprig::krkr::tjs::object_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = sprig::krkr::tjs::object_type(result_obj, false);
		}
		{
			tjs_int index = 0;
			for (boost::asio::ip::tcp::resolver::iterator i = *resolvers_.iterator(), last = boost::asio::ip::tcp::resolver::iterator(); i != last; ++i, ++index) {
				boost::asio::ip::tcp::endpoint endpoint(*i);
				sprig::krkr::tjs::object_type elem;
				{
					iTJSDispatch2* elem_obj = 0;
					sprig::krkr::tjs::CreateNewObject(
						sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
						&elem_obj, 0, 0, 0
						);
					elem = sprig::krkr::tjs::object_type(elem_obj, false);
				}
				{
					tTJSVariant var(tTVInteger(
						endpoint.protocol() == boost::asio::ip::tcp::v6()
							? pfIPv6
							: pfIPv4
						));
					sprig::krkr::tjs::AddMember(elem.get(), SPRIG_KRKR_TJS_W("protocol"), &var);
				}
				{
					boost::asio::ip::address addr(endpoint.address());
					boost::system::error_code error;
					std::string address(addr.to_string(error));
					if (error) {
						return tTJSVariant();
					}
					tTJSVariant var(address.c_str());
					sprig::krkr::tjs::AddMember(elem.get(), SPRIG_KRKR_TJS_W("address"), &var);
				}
				{
					tTJSVariant var(endpoint.port());
					sprig::krkr::tjs::AddMember(elem.get(), SPRIG_KRKR_TJS_W("port"), &var);
				}
				{
					tTJSVariant var(elem.get(), elem.get());
					sprig::krkr::tjs::AddMemberByNum(result.get(), index, &var);
				}
			}
		}
		return tTJSVariant(result.get(), result.get());
	}
	//
	//	SUMMARY: 接続系メソッド
	//
	KTL_INLINE bool NativeSocket::connect() {
		scoped_lock_type lock(mutex_);
		if (!setupConnect()) {
			return false;
		}
		boost::asio::connect(
			NetworkUtils::asSocket(*socket2_),
			*resolvers_.iterator(),
			*connectors2_.error_code()
			);
		return finishConnect();
	}
	KTL_INLINE bool NativeSocket::connect(tTJSVariant* address, unsigned short port) {
		scoped_lock_type lock(mutex_);
		boost::optional<boost::asio::ip::address> addr = convertAddress(address);
		if (!setupConnect(addr, port)) {
			return false;
		}
		NetworkUtils::asSocket(*socket2_).close(*connectors2_.error_code());
		if (!checkConnectors()) {
			return false;
		}
		NetworkUtils::asSocket(*socket2_).connect(
			boost::asio::ip::tcp::endpoint(
				*addr,
				port
				),
			*connectors2_.error_code()
			);
		return finishConnect();
	}
	KTL_INLINE bool NativeSocket::connectSSL() {
		scoped_lock_type lock(mutex_);
		if (!setupConnectSSL()) {
			return false;
		}
		boost::asio::connect(
			NetworkUtils::asSocket(*ssl_socket2_),
			*resolvers_.iterator(),
			*connectors2_.error_code()
			);
		return finishConnectSSL();
	}
	KTL_INLINE bool NativeSocket::connectSSL(tTJSVariant* address, unsigned short port) {
		scoped_lock_type lock(mutex_);
		boost::optional<boost::asio::ip::address> addr = convertAddress(address);
		if (!setupConnectSSL(addr, port)) {
			return false;
		}
		NetworkUtils::asSocket(*ssl_socket2_).close(*connectors2_.error_code());
		if (!checkConnectors()) {
			return false;
		}
		NetworkUtils::asSocket(*ssl_socket2_).connect(
			boost::asio::ip::tcp::endpoint(
				*addr,
				port
				),
			*connectors2_.error_code()
			);
		return finishConnectSSL();
	}
	KTL_INLINE bool NativeSocket::connectLoopback(unsigned short port, flag_type protocol_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupConnectLoopback(port, protocol_flag)) {
			return false;
		}
		NetworkUtils::asSocket(*socket2_).close(*connectors2_.error_code());
		if (!checkConnectors()) {
			return false;
		}
		NetworkUtils::asSocket(*socket2_).connect(
			protocol_flag == NetworkUtils::pfIPv6
				? boost::asio::ip::tcp::endpoint(
					boost::asio::ip::address_v6::loopback(),
					port
					)
				: boost::asio::ip::tcp::endpoint(
					boost::asio::ip::address_v4::loopback(),
					port
					)
				,
			*connectors2_.error_code()
			);
		return finishConnect();
	}
	KTL_INLINE bool NativeSocket::connectLoopbackSSL(unsigned short port, flag_type protocol_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupConnectLoopbackSSL(port, protocol_flag)) {
			return false;
		}
		NetworkUtils::asSocket(*ssl_socket2_).close(*connectors2_.error_code());
		if (!checkConnectors()) {
			return false;
		}
		NetworkUtils::asSocket(*ssl_socket2_).connect(
			protocol_flag == NetworkUtils::pfIPv6
				? boost::asio::ip::tcp::endpoint(
					boost::asio::ip::address_v6::loopback(),
					port
					)
				: boost::asio::ip::tcp::endpoint(
					boost::asio::ip::address_v4::loopback(),
					port
					)
				,
			*connectors2_.error_code()
			);
		return finishConnectSSL();
	}
	KTL_INLINE bool NativeSocket::asyncConnect() {
		scoped_lock_type lock(mutex_);
		if (!setupConnect()) {
			return false;
		}
		boost::asio::async_connect(
			NetworkUtils::asSocket(*socket2_),
			*resolvers_.iterator(),
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleConnect,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::iterator
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::asyncConnect(tTJSVariant* address, unsigned short port) {
		scoped_lock_type lock(mutex_);
		boost::optional<boost::asio::ip::address> addr = convertAddress(address);
		if (!setupConnect(addr, port)) {
			return false;
		}
		NetworkUtils::asSocket(*socket2_).close(*connectors2_.error_code());
		if (!checkConnectors()) {
			return false;
		}
		NetworkUtils::asSocket(*socket2_).async_connect(
			boost::asio::ip::tcp::endpoint(
				*addr,
				port
				),
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleConnect,
					this,
					boost::asio::placeholders::error,
					boost::asio::ip::tcp::resolver::iterator()
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::asyncConnectSSL() {
		scoped_lock_type lock(mutex_);
		if (!setupConnectSSL()) {
			return false;
		}
		boost::asio::async_connect(
			NetworkUtils::asSocket(*ssl_socket2_),
			*resolvers_.iterator(),
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleConnectSSL,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::iterator
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::asyncConnectSSL(tTJSVariant* address, unsigned short port) {
		scoped_lock_type lock(mutex_);
		boost::optional<boost::asio::ip::address> addr = convertAddress(address);
		if (!setupConnectSSL(addr, port)) {
			return false;
		}
		NetworkUtils::asSocket(*ssl_socket2_).close(*connectors2_.error_code());
		if (!checkConnectors()) {
			return false;
		}
		NetworkUtils::asSocket(*ssl_socket2_).async_connect(
			boost::asio::ip::tcp::endpoint(
				*addr,
				port
				),
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleConnectSSL,
					this,
					boost::asio::placeholders::error,
					boost::asio::ip::tcp::resolver::iterator()
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::asyncConnectLoopback(unsigned short port, flag_type protocol_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupConnectLoopback(port, protocol_flag)) {
			return false;
		}
		NetworkUtils::asSocket(*socket2_).close(*connectors2_.error_code());
		if (!checkConnectors()) {
			return false;
		}
		NetworkUtils::asSocket(*socket2_).async_connect(
			protocol_flag == NetworkUtils::pfIPv6
				? boost::asio::ip::tcp::endpoint(
					boost::asio::ip::address_v6::loopback(),
					port
					)
				: boost::asio::ip::tcp::endpoint(
					boost::asio::ip::address_v4::loopback(),
					port
					)
				,
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleConnect,
					this,
					boost::asio::placeholders::error,
					boost::asio::ip::tcp::resolver::iterator()
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::asyncConnectLoopbackSSL(unsigned short port, flag_type protocol_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupConnectLoopbackSSL(port, protocol_flag)) {
			return false;
		}
		NetworkUtils::asSocket(*ssl_socket2_).close(*connectors2_.error_code());
		if (!checkConnectors()) {
			return false;
		}
		NetworkUtils::asSocket(*ssl_socket2_).async_connect(
			protocol_flag == NetworkUtils::pfIPv6
				? boost::asio::ip::tcp::endpoint(
					boost::asio::ip::address_v6::loopback(),
					port
					)
				: boost::asio::ip::tcp::endpoint(
					boost::asio::ip::address_v4::loopback(),
					port
					)
				,
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleConnectSSL,
					this,
					boost::asio::placeholders::error,
					boost::asio::ip::tcp::resolver::iterator()
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::connectingFinished() const {
		scoped_lock_type lock(mutex_);
		return connectors_.enable();
	}
	KTL_INLINE bool NativeSocket::connectingSucceeded() const {
		scoped_lock_type lock(mutex_);
		return connectors_.error_code() && !*connectors_.error_code();
	}
	//
	//	SUMMARY: 接続待ち系メソッド
	//
	KTL_INLINE bool NativeSocket::accept(unsigned short port, flag_type protocol_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupAccept(port, protocol_flag)) {
			return false;
		}
		NetworkUtils::asSocket(*socket2_).close(*acceptors2_.error_code());
		if (!checkAcceptors()) {
			return false;
		}
		acceptors2_.acceptor()->accept(
			NetworkUtils::asSocket(*socket2_),
			*acceptors2_.error_code()
			);
		return finishAccept();
	}
	KTL_INLINE bool NativeSocket::acceptSSL(unsigned short port, flag_type protocol_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupAcceptSSL(port, protocol_flag)) {
			return false;
		}
		NetworkUtils::asSocket(*ssl_socket2_).close(*acceptors2_.error_code());
		if (!checkAcceptors()) {
			return false;
		}
		acceptors2_.acceptor()->accept(
			NetworkUtils::asSocket(*ssl_socket2_),
			*acceptors2_.error_code()
			);
		return finishAcceptSSL();
	}
	KTL_INLINE bool NativeSocket::asyncAccept(unsigned short port, flag_type protocol_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupAccept(port, protocol_flag)) {
			return false;
		}
		NetworkUtils::asSocket(*socket2_).close(*acceptors2_.error_code());
		if (!checkAcceptors()) {
			return false;
		}
		acceptors2_.acceptor()->async_accept(
			NetworkUtils::asSocket(*socket2_),
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleAccept,
					this,
					boost::asio::placeholders::error
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::asyncAcceptSSL(unsigned short port, flag_type protocol_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupAcceptSSL(port, protocol_flag)) {
			return false;
		}
		NetworkUtils::asSocket(*ssl_socket2_).close(*acceptors2_.error_code());
		if (!checkAcceptors()) {
			return false;
		}
		acceptors2_.acceptor()->async_accept(
			NetworkUtils::asSocket(*ssl_socket2_),
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleAcceptSSL,
					this,
					boost::asio::placeholders::error
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::acceptingFinished() const {
		scoped_lock_type lock(mutex_);
		return acceptors_.enable();
	}
	KTL_INLINE bool NativeSocket::acceptingSucceeded() const {
		scoped_lock_type lock(mutex_);
		return acceptors_.error_code() && !*acceptors_.error_code();
	}
	//
	//	SUMMARY: ハンドシェイク系メソッド
	//
	KTL_INLINE bool NativeSocket::handshake() {
		scoped_lock_type lock(mutex_);
		if (!setupHandshake()) {
			return false;
		}
		ssl_socket_->handshake(
			acceptors_.enable()
				? boost::asio::ssl::stream_base::server
				: boost::asio::ssl::stream_base::client
				,
			*handshakers2_.error_code()
			);
		return finishHandshake();
	}
	KTL_INLINE bool NativeSocket::asyncHandshake() {
		scoped_lock_type lock(mutex_);
		if (!setupHandshake()) {
			return false;
		}
		ssl_socket_->async_handshake(
			acceptors_.enable()
				? boost::asio::ssl::stream_base::server
				: boost::asio::ssl::stream_base::client
				,
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleHandshake,
					this,
					boost::asio::placeholders::error
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::handshakingFinished() const {
		scoped_lock_type lock(mutex_);
		return handshakers_.enable();
	}
	KTL_INLINE bool NativeSocket::handshakingSucceeded() const {
		scoped_lock_type lock(mutex_);
		return handshakers_.error_code() && !*handshakers_.error_code();
	}
	//
	//	SUMMARY: 送信系メソッド
	//
	KTL_INLINE void NativeSocket::pushByte(byte_type source) {
		scoped_lock_type lock(mutex_);
		std::ostream writing_ostream(writing_streambuf_.get());
		writing_ostream.put(source);
	}
	KTL_INLINE void NativeSocket::pushString(tjs_char const* source) {
		scoped_lock_type lock(mutex_);
		std::ostream writing_ostream(writing_streambuf_.get());
		size_type length = sprig::str_length(source) * sizeof(tjs_char);
		writing_ostream.write(reinterpret_cast<char const*>(source), length);
	}
	KTL_INLINE void NativeSocket::pushNarrowString(tjs_nchar const* source) {
		scoped_lock_type lock(mutex_);
		std::ostream writing_ostream(writing_streambuf_.get());
		size_type length = sprig::str_length(source) * sizeof(tjs_nchar);
		writing_ostream.write(source, length);
	}
	KTL_INLINE void NativeSocket::pushOctet(tTJSVariantOctet const* source) {
		scoped_lock_type lock(mutex_);
		std::ostream writing_ostream(writing_streambuf_.get());
		size_type length = sprig::krkr::tjs::octet_length(source);
		writing_ostream.write(reinterpret_cast<char const*>(sprig::krkr::tjs::octet_data(source)), length);
	}
	KTL_INLINE void NativeSocket::pushLine(impl_string_type const& source) {
		scoped_lock_type lock(mutex_);
		std::ostream writing_ostream(writing_streambuf_.get());
		writing_ostream << source << "\r\n";
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::write() {
		scoped_lock_type lock(mutex_);
		if (!setupWrite()) {
			return -1;
		}
		size_type bytes_transferred = 0;
		if (socket_) {
			bytes_transferred = boost::asio::write(
				*socket_,
				*writing_streambuf_,
				boost::asio::transfer_all(),
				*writers2_.error_code()
				);
		} else if (ssl_socket_) {
			bytes_transferred = boost::asio::write(
				*ssl_socket_,
				*writing_streambuf_,
				boost::asio::transfer_all(),
				*writers2_.error_code()
				);
		}
		finishWrite();
		return bytes_transferred;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::asyncWrite() {
		scoped_lock_type lock(mutex_);
		if (!setupWrite()) {
			return -1;
		}
		if (socket_) {
			boost::asio::async_write(
				*socket_,
				*writing_streambuf_,
				boost::asio::transfer_all(),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleWrite,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		} else if (ssl_socket_) {
			boost::asio::async_write(
				*ssl_socket_,
				*writing_streambuf_,
				boost::asio::transfer_all(),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleWrite,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		}
		return writing_streambuf_->size();
	}
	KTL_INLINE bool NativeSocket::writingFinished() const {
		scoped_lock_type lock(mutex_);
		return writers_.enable();
	}
	KTL_INLINE bool NativeSocket::writingSucceeded() const {
		scoped_lock_type lock(mutex_);
		return writers_.error_code() && !*writers_.error_code();
	}
	KTL_INLINE NativeSocket::size_type NativeSocket::writingBufferSize() const {
		scoped_lock_type lock(mutex_);
		return writing_streambuf_->size();
	}
	//
	//	SUMMARY: 送信バッファ取得系メソッド
	//
	KTL_INLINE tTJSVariant NativeSocket::getWritingBufferAsOctet() const {
		scoped_lock_type lock(mutex_);
		return getBufferAsOctetImpl(
			*writing_streambuf_,
			0,
			writing_streambuf_->size()
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getWritingBufferAsOctet(size_type pos) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsOctetImpl(
			*writing_streambuf_,
			pos,
			getEnableSize(writing_streambuf_->size(), pos)
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getWritingBufferAsOctet(size_type pos, size_type size) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsOctetImpl(
			*writing_streambuf_,
			pos,
			getEnableSize(writing_streambuf_->size(), pos, size)
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getWritingBufferAsString() const {
		scoped_lock_type lock(mutex_);
		return getBufferAsStringImpl(
			*writing_streambuf_,
			0,
			writing_streambuf_->size()
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getWritingBufferAsString(size_type pos) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsStringImpl(
			*writing_streambuf_,
			pos,
			getEnableSize(writing_streambuf_->size(), pos)
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getWritingBufferAsString(size_type pos, size_type size) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsStringImpl(
			*writing_streambuf_,
			pos,
			getEnableSize(writing_streambuf_->size(), pos, size)
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getWritingBufferAsNarrowString() const {
		scoped_lock_type lock(mutex_);
		return getBufferAsNarrowStringImpl(
			*writing_streambuf_,
			0,
			writing_streambuf_->size()
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getWritingBufferAsNarrowString(size_type pos) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsNarrowStringImpl(
			*writing_streambuf_,
			pos,
			getEnableSize(writing_streambuf_->size(), pos)
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getWritingBufferAsNarrowString(size_type pos, size_type size) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsNarrowStringImpl(
			*writing_streambuf_,
			pos,
			getEnableSize(writing_streambuf_->size(), pos, size)
			);
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::consumeWritingBuffer() {
		scoped_lock_type lock(mutex_);
		int_type result_size = writing_streambuf_->size();
		writing_streambuf_->consume(result_size);
		return result_size;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::consumeWritingBuffer(size_type size) {
		scoped_lock_type lock(mutex_);
		int_type result_size = (std::min)(size, writing_streambuf_->size());
		writing_streambuf_->consume(result_size);
		return result_size;
	}
	//
	//	SUMMARY: 受信系メソッド
	KTL_INLINE tTJSVariant NativeSocket::popByte() {
		scoped_lock_type lock(mutex_);
		size_type const reading_size = (std::min<size_type>)(1, reading_streambuf_->size());
		char byte = 0;
		if (!reading_size) {
			return tTJSVariant(static_cast<tTVInteger>(0));
		}
		std::istream reading_istream(reading_streambuf_.get());
		reading_istream.read(&byte, reading_size);
		return tTJSVariant(static_cast<tTVInteger>(static_cast<byte_type>(byte)));
	}
	KTL_INLINE tTJSVariant NativeSocket::popString() {
		scoped_lock_type lock(mutex_);
		size_type const reading_size = reading_streambuf_->size();
		std::vector<char> binary(reading_size + (sizeof(tjs_char) * 2 - 1));
		if (!reading_size) {
			return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
		}
		std::istream reading_istream(reading_streambuf_.get());
		reading_istream.read(&binary[0], reading_size);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
	}
	KTL_INLINE tTJSVariant NativeSocket::popString(size_type size) {
		scoped_lock_type lock(mutex_);
		size_type const reading_size = (std::min)(size, reading_streambuf_->size());
		std::vector<char> binary(reading_size + (sizeof(tjs_char) * 2 - 1));
		if (!reading_size) {
			return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
		}
		std::istream reading_istream(reading_streambuf_.get());
		reading_istream.read(&binary[0], reading_size);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&binary[0]));
	}
	KTL_INLINE tTJSVariant NativeSocket::popNarrowString() {
		scoped_lock_type lock(mutex_);
		size_type const reading_size = reading_streambuf_->size();
		std::vector<char> binary(reading_size + (sizeof(tjs_nchar) * 2 - 1));
		if (!reading_size) {
			return tTJSVariant(&binary[0]);
		}
		std::istream reading_istream(reading_streambuf_.get());
		reading_istream.read(&binary[0], reading_size);
		return tTJSVariant(&binary[0]);
	}
	KTL_INLINE tTJSVariant NativeSocket::popNarrowString(size_type size) {
		scoped_lock_type lock(mutex_);
		size_type const reading_size = (std::min)(size, reading_streambuf_->size());
		std::vector<char> binary(reading_size + (sizeof(tjs_nchar) * 2 - 1));
		if (!reading_size) {
			return tTJSVariant(&binary[0]);
		}
		std::istream reading_istream(reading_streambuf_.get());
		reading_istream.read(&binary[0], reading_size);
		return tTJSVariant(&binary[0]);
	}
	KTL_INLINE tTJSVariant NativeSocket::popOctet() {
		scoped_lock_type lock(mutex_);
		size_type const reading_size = reading_streambuf_->size();
		if (!reading_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		std::vector<tjs_uint8> binary(reading_size);
		std::istream reading_istream(reading_streambuf_.get());
		reading_istream.read(reinterpret_cast<char*>(&binary[0]), reading_size);
		return tTJSVariant(&binary[0], reading_size);
	}
	KTL_INLINE tTJSVariant NativeSocket::popOctet(size_type size) {
		scoped_lock_type lock(mutex_);
		size_type const reading_size = (std::min)(size, reading_streambuf_->size());
		if (!reading_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		std::vector<tjs_uint8> binary(reading_size);
		std::istream reading_istream(reading_streambuf_.get());
		reading_istream.read(reinterpret_cast<char*>(&binary[0]), reading_size);
		return tTJSVariant(&binary[0], reading_size);
	}
	KTL_INLINE tTJSVariant NativeSocket::popLine() {
		scoped_lock_type lock(mutex_);
		impl_string_type line;
		if (!reading_streambuf_->size()) {
			return tTJSVariant(line.c_str());
		}
		std::istream reading_istream(reading_streambuf_.get());
		std::getline(reading_istream, line);
		if (!line.empty() && line[line.size() - 1] == '\r') {
			line.erase(line.size() - 1);
		}
		return tTJSVariant(line.c_str());
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::read() {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		size_type bytes_transferred = 0;
		if (socket_) {
			bytes_transferred = boost::asio::read(
				*socket_,
				*reading_streambuf_,
				boost::asio::transfer_all(),
				*readers2_.error_code()
				);
		} else if (ssl_socket_) {
			bytes_transferred = boost::asio::read(
				*ssl_socket_,
				*reading_streambuf_,
				boost::asio::transfer_all(),
				*readers2_.error_code()
				);
		}
		finishRead();
		return bytes_transferred;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::read(size_type size, flag_type complete_condition_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		size_type bytes_transferred = 0;
		if (socket_) {
			bytes_transferred = complete_condition_flag == ccfTransferExactly
				? boost::asio::read(
					*socket_,
					*reading_streambuf_,
					boost::asio::transfer_exactly(size),
					*readers2_.error_code()
					)
				: boost::asio::read(
					*socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(size),
					*readers2_.error_code()
					)
				;
		} else if (ssl_socket_) {
			bytes_transferred = complete_condition_flag == ccfTransferExactly
				? boost::asio::read(
					*ssl_socket_,
					*reading_streambuf_,
					boost::asio::transfer_exactly(size),
					*readers2_.error_code()
					)
				: boost::asio::read(
					*ssl_socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(size),
					*readers2_.error_code()
					)
				;
		}
		finishRead();
		return bytes_transferred;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::readUntilByte(byte_type delim) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		size_type bytes_transferred = 0;
		if (socket_) {
			bytes_transferred = boost::asio::read_until(
				*socket_,
				*reading_streambuf_,
				static_cast<char>(delim),
				*readers2_.error_code()
				);
		} else if (ssl_socket_) {
			bytes_transferred = boost::asio::read_until(
				*ssl_socket_,
				*reading_streambuf_,
				static_cast<char>(delim),
				*readers2_.error_code()
				);
		}
		finishRead();
		return bytes_transferred;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::readUntilString(tjs_char const* delim) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		size_type bytes_transferred = 0;
		char const* delimiter = reinterpret_cast<char const*>(delim);
		size_type length = sprig::str_length(delim) * sizeof(tjs_char);
		if (socket_) {
			bytes_transferred = boost::asio::read_until(
				*socket_,
				*reading_streambuf_,
				MatchCondition(delimiter, delimiter + length),
				*readers2_.error_code()
				);
		} else if (ssl_socket_) {
			bytes_transferred = boost::asio::read_until(
				*ssl_socket_,
				*reading_streambuf_,
				MatchCondition(delimiter, delimiter + length),
				*readers2_.error_code()
				);
		}
		finishRead();
		return bytes_transferred;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::readUntilNarrowString(tjs_nchar const* delim) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		size_type bytes_transferred = 0;
		if (socket_) {
			bytes_transferred = boost::asio::read_until(
				*socket_,
				*reading_streambuf_,
				delim,
				*readers2_.error_code()
				);
		} else if (ssl_socket_) {
			bytes_transferred = boost::asio::read_until(
				*ssl_socket_,
				*reading_streambuf_,
				delim,
				*readers2_.error_code()
				);
		}
		finishRead();
		return bytes_transferred;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::readUntilOctet(tTJSVariantOctet const* delim) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		size_type bytes_transferred = 0;
		char const* delimiter = reinterpret_cast<char const*>(sprig::krkr::tjs::octet_data(delim));
		size_type length = sprig::krkr::tjs::octet_length(delim);
		if (socket_) {
			bytes_transferred = boost::asio::read_until(
				*socket_,
				*reading_streambuf_,
				MatchCondition(delimiter, delimiter + length),
				*readers2_.error_code()
				);
		} else if (ssl_socket_) {
			bytes_transferred = boost::asio::read_until(
				*ssl_socket_,
				*reading_streambuf_,
				MatchCondition(delimiter, delimiter + length),
				*readers2_.error_code()
				);
		}
		finishRead();
		return bytes_transferred;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::readUntil(impl_string_type const& delim) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		size_type bytes_transferred = 0;
		if (socket_) {
			bytes_transferred = boost::asio::read_until(
				*socket_,
				*reading_streambuf_,
				delim,
				*readers2_.error_code()
				);
		} else if (ssl_socket_) {
			bytes_transferred = boost::asio::read_until(
				*ssl_socket_,
				*reading_streambuf_,
				delim,
				*readers2_.error_code()
				);
		}
		finishRead();
		return bytes_transferred;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::readUntilRegex(impl_string_type const& expr) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		size_type bytes_transferred = 0;
		if (socket_) {
			bytes_transferred = boost::asio::read_until(
				*socket_,
				*reading_streambuf_,
				boost::regex(expr),
				*readers2_.error_code()
				);
		} else if (ssl_socket_) {
			bytes_transferred = boost::asio::read_until(
				*ssl_socket_,
				*reading_streambuf_,
				boost::regex(expr),
				*readers2_.error_code()
				);
		}
		finishRead();
		return bytes_transferred;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::asyncRead() {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		if (socket_) {
			boost::asio::async_read(
				*socket_,
				*reading_streambuf_,
				boost::asio::transfer_all(),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleRead,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		} else if (ssl_socket_) {
			boost::asio::async_read(
				*ssl_socket_,
				*reading_streambuf_,
				boost::asio::transfer_all(),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleRead,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		}
		return 0;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::asyncRead(size_type size, flag_type complete_condition_flag) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		if (socket_) {
			complete_condition_flag == ccfTransferExactly
				? boost::asio::async_read(
					*socket_,
					*reading_streambuf_,
					boost::asio::transfer_exactly(size),
					strand_->wrap(
						boost::bind(
							&NativeSocket::handleRead,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred
							)
						)
					)
				: boost::asio::async_read(
					*socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(size),
					strand_->wrap(
						boost::bind(
							&NativeSocket::handleRead,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred
							)
						)
					)
				;
		} else if (ssl_socket_) {
			complete_condition_flag == ccfTransferExactly
				? boost::asio::async_read(
					*ssl_socket_,
					*reading_streambuf_,
					boost::asio::transfer_exactly(size),
					strand_->wrap(
						boost::bind(
							&NativeSocket::handleRead,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred
							)
						)
					)
				: boost::asio::async_read(
					*ssl_socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(size),
					strand_->wrap(
						boost::bind(
							&NativeSocket::handleRead,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred
							)
						)
					)
				;
		}
		return 0;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::asyncReadUntilByte(byte_type delim) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		if (socket_) {
			boost::asio::async_read_until(
				*socket_,
				*reading_streambuf_,
				static_cast<char>(delim),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		} else if (ssl_socket_) {
			boost::asio::async_read_until(
				*ssl_socket_,
				*reading_streambuf_,
				static_cast<char>(delim),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		}
		return 0;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::asyncReadUntilString(tjs_char const* delim) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		char const* delimiter = reinterpret_cast<char const*>(delim);
		size_type length = sprig::str_length(delim) * sizeof(tjs_char);
		if (socket_) {
			boost::asio::async_read_until(
				*socket_,
				*reading_streambuf_,
				MatchCondition(delimiter, delimiter + length),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		} else if (ssl_socket_) {
			boost::asio::async_read_until(
				*ssl_socket_,
				*reading_streambuf_,
				MatchCondition(delimiter, delimiter + length),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		}
		return 0;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::asyncReadUntilNarrowString(tjs_nchar const* delim) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		if (socket_) {
			boost::asio::async_read_until(
				*socket_,
				*reading_streambuf_,
				delim,
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		} else if (ssl_socket_) {
			boost::asio::async_read_until(
				*ssl_socket_,
				*reading_streambuf_,
				delim,
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		}
		return 0;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::asyncReadUntilOctet(tTJSVariantOctet const* delim) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		char const* delimiter = reinterpret_cast<char const*>(sprig::krkr::tjs::octet_data(delim));
		size_type length = sprig::krkr::tjs::octet_length(delim);
		if (socket_) {
			boost::asio::async_read_until(
				*socket_,
				*reading_streambuf_,
				MatchCondition(delimiter, delimiter + length),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		} else if (ssl_socket_) {
			boost::asio::async_read_until(
				*ssl_socket_,
				*reading_streambuf_,
				MatchCondition(delimiter, delimiter + length),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		}
		return 0;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::asyncReadUntil(impl_string_type const& delim) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		if (socket_) {
			boost::asio::async_read_until(
				*socket_,
				*reading_streambuf_,
				delim,
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		} else if (ssl_socket_) {
			boost::asio::async_read_until(
				*ssl_socket_,
				*reading_streambuf_,
				delim,
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		}
		return 0;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::asyncReadUntilRegex(impl_string_type const& expr) {
		scoped_lock_type lock(mutex_);
		if (!setupRead()) {
			return -1;
		}
		if (socket_) {
			boost::asio::async_read_until(
				*socket_,
				*reading_streambuf_,
				boost::regex(expr),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		} else if (ssl_socket_) {
			boost::asio::async_read_until(
				*ssl_socket_,
				*reading_streambuf_,
				boost::regex(expr),
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleReadUntil,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		}
		return 0;
	}
	KTL_INLINE bool NativeSocket::readingFinished() const {
		scoped_lock_type lock(mutex_);
		return readers_.enable();
	}
	KTL_INLINE bool NativeSocket::readingSucceeded() const {
		scoped_lock_type lock(mutex_);
		return readers_.error_code()
			&& (
				!*readers_.error_code()
				|| *readers_.error_code() == boost::asio::error::eof
				)
			;
	}
	KTL_INLINE bool NativeSocket::eof() const {
		scoped_lock_type lock(mutex_);
		return readers_.error_code()
			&& *readers_.error_code() == boost::asio::error::eof
			;
	}
	KTL_INLINE NativeSocket::size_type NativeSocket::readingBufferSize() const {
		scoped_lock_type lock(mutex_);
		return reading_streambuf_->size();
	}
	//
	//	SUMMARY: 受信バッファ取得系メソッド
	//
	KTL_INLINE tTJSVariant NativeSocket::getReadingBufferAsOctet() const {
		scoped_lock_type lock(mutex_);
		return getBufferAsOctetImpl(
			*reading_streambuf_,
			0,
			reading_streambuf_->size()
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getReadingBufferAsOctet(size_type pos) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsOctetImpl(
			*reading_streambuf_,
			pos,
			getEnableSize(reading_streambuf_->size(), pos)
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getReadingBufferAsOctet(size_type pos, size_type size) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsOctetImpl(
			*reading_streambuf_,
			pos,
			getEnableSize(reading_streambuf_->size(), pos, size)
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getReadingBufferAsString() const {
		scoped_lock_type lock(mutex_);
		return getBufferAsStringImpl(
			*reading_streambuf_,
			0,
			reading_streambuf_->size()
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getReadingBufferAsString(size_type pos) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsStringImpl(
			*reading_streambuf_,
			pos,
			getEnableSize(reading_streambuf_->size(), pos)
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getReadingBufferAsString(size_type pos, size_type size) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsStringImpl(
			*reading_streambuf_,
			pos,
			getEnableSize(reading_streambuf_->size(), pos, size)
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getReadingBufferAsNarrowString() const {
		scoped_lock_type lock(mutex_);
		return getBufferAsNarrowStringImpl(
			*reading_streambuf_,
			0,
			reading_streambuf_->size()
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getReadingBufferAsNarrowString(size_type pos) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsNarrowStringImpl(
			*reading_streambuf_,
			pos,
			getEnableSize(reading_streambuf_->size(), pos)
			);
	}
	KTL_INLINE tTJSVariant NativeSocket::getReadingBufferAsNarrowString(size_type pos, size_type size) const {
		scoped_lock_type lock(mutex_);
		return getBufferAsNarrowStringImpl(
			*reading_streambuf_,
			pos,
			getEnableSize(reading_streambuf_->size(), pos, size)
			);
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::consumeReadingBuffer() {
		scoped_lock_type lock(mutex_);
		int_type result_size = reading_streambuf_->size();
		reading_streambuf_->consume(result_size);
		return result_size;
	}
	KTL_INLINE NativeSocket::int_type NativeSocket::consumeReadingBuffer(size_type size) {
		scoped_lock_type lock(mutex_);
		int_type result_size = (std::min)(size, reading_streambuf_->size());
		reading_streambuf_->consume(result_size);
		return result_size;
	}
	//
	//	SUMMARY: タイムアウト系メソッド
	//
	KTL_INLINE bool NativeSocket::waitTimeoutFromNow(rep_type expiry_time_millisec) {
		{
			scoped_lock_type lock(mutex_);
			if (!is_processing_) {
				return false;
			}
			timeout_timers_.error_code().reset();
			timeout_timers2_.error_code() = boost::system::error_code();
			timeout_timer_->expires_from_now(
				milliseconds_type(expiry_time_millisec),
				*timeout_timers2_.error_code()
				);
			if (*timeout_timers2_.error_code()) {
				NetworkUtils::moveErrorCode(timeout_timers_, timeout_timers2_);
				return false;
			}
			timeout_timers2_.error_code() = boost::system::error_code();
			timeout_timer_->async_wait(
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleTimeout,
						this,
						boost::asio::placeholders::error
						)
					)
				);
		}
		for (; ; ) {
			scoped_lock_type lock(mutex_);
			if (!is_processing_) {
				break;
			}
		}
		return true;
	}
	KTL_INLINE bool NativeSocket::waitTimeoutAt(rep_type expiry_time_millisec) {
		{
			scoped_lock_type lock(mutex_);
			if (!is_processing_) {
				return false;
			}
			timeout_timers_.error_code().reset();
			timeout_timers2_.error_code() = boost::system::error_code();
			timeout_timer_->expires_at(
				time_point_type(milliseconds_type(expiry_time_millisec)),
				*timeout_timers2_.error_code()
				);
			if (*timeout_timers2_.error_code()) {
				NetworkUtils::moveErrorCode(timeout_timers_, timeout_timers2_);
				return false;
			}
			timeout_timer_->async_wait(
				strand_->wrap(
					boost::bind(
						&NativeSocket::handleTimeout,
						this,
						boost::asio::placeholders::error
						)
					)
				);
		}
		for (; ; ) {
			scoped_lock_type lock(mutex_);
			if (!is_processing_) {
				break;
			}
		}
		return true;
	}
	KTL_INLINE bool NativeSocket::asyncWaitTimeoutFromNow(rep_type expiry_time_millisec) {
		scoped_lock_type lock(mutex_);
		if (!is_processing_) {
			return false;
		}
		timeout_timers_.error_code().reset();
		timeout_timers2_.error_code() = boost::system::error_code();
		timeout_timer_->expires_from_now(
			milliseconds_type(expiry_time_millisec),
			*timeout_timers2_.error_code()
			);
		if (*timeout_timers2_.error_code()) {
			NetworkUtils::moveErrorCode(timeout_timers_, timeout_timers2_);
			return false;
		}
		timeout_timers2_.error_code() = boost::system::error_code();
		timeout_timer_->async_wait(
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleTimeout,
					this,
					boost::asio::placeholders::error
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::asyncWaitTimeoutAt(rep_type expiry_time_millisec) {
		scoped_lock_type lock(mutex_);
		if (!is_processing_) {
			return false;
		}
		timeout_timers_.error_code().reset();
		timeout_timers2_.error_code() = boost::system::error_code();
		timeout_timer_->expires_at(
			time_point_type(milliseconds_type(expiry_time_millisec)),
			*timeout_timers2_.error_code()
			);
		if (*timeout_timers2_.error_code()) {
			NetworkUtils::moveErrorCode(timeout_timers_, timeout_timers2_);
			return false;
		}
		timeout_timers2_.error_code() = boost::system::error_code();
		timeout_timer_->async_wait(
			strand_->wrap(
				boost::bind(
					&NativeSocket::handleTimeout,
					this,
					boost::asio::placeholders::error
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeSocket::cancelTimeout() {
		scoped_lock_type lock(mutex_);
		return cancelTimeoutImpl();
	}
	KTL_INLINE bool NativeSocket::isWaitingTimeout() const {
		scoped_lock_type lock(mutex_);
		return timeout_timers2_.enable();
	}
	KTL_INLINE NativeSocket::rep_type NativeSocket::expiresTimeoutFromNow() const {
		scoped_lock_type lock(mutex_);
		return boost::chrono::duration_cast<milliseconds_type>(timeout_timer_->expires_from_now()).count();
	}
	KTL_INLINE NativeSocket::rep_type NativeSocket::expiresTimeoutAt() const {
		scoped_lock_type lock(mutex_);
		return boost::chrono::duration_cast<milliseconds_type>(timeout_timer_->expires_at().time_since_epoch()).count();
	}
	//
	//	SUMMARY: 待機系メソッド
	//
	KTL_INLINE bool NativeSocket::join() {
		{
			scoped_lock_type lock(mutex_);
			if (!is_processing_) {
				return false;
			}
		}
		for (; ; ) {
			scoped_lock_type lock(mutex_);
			if (!is_processing_) {
				break;
			}
		}
		return true;
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool NativeSocket::isProcessing() const {
		scoped_lock_type lock(mutex_);
		return is_processing_;
	}
	KTL_INLINE bool NativeSocket::isOpen() const {
		scoped_lock_type lock(mutex_);
		return socket_ && NetworkUtils::asSocket(*socket_).is_open()
			|| ssl_socket_ && NetworkUtils::asSocket(*ssl_socket_).is_open()
			;
	}
	KTL_INLINE bool NativeSocket::isSSL() const {
		scoped_lock_type lock(mutex_);
		return ssl_socket_ && NetworkUtils::asSocket(*ssl_socket_).is_open();
	}
	//
	//	SUMMARY: コールバック系メソッド
	//
	KTL_INLINE tTJSVariant NativeSocket::getOnFinished() const {
		scoped_lock_type lock(mutex_);
		return on_finished_
			? *on_finished_
			: tTJSVariant()
			;
	}
	KTL_INLINE void NativeSocket::setOnFinished(tTJSVariant const& func) {
		scoped_lock_type lock(mutex_);
		if (func.Type() == tvtObject) {
			on_finished_ = boost::make_shared<tTJSVariant>(func);
		} else {
			on_finished_.reset();
		}
	}

	//
	// Socket::AliveHandler
	//
	KTL_INLINE Socket::AliveHandler::AliveHandler(boost::shared_ptr<NativeSocket> const& instance)
		: instance_(instance)
	{}
	KTL_INLINE void Socket::AliveHandler::operator()() const {
		scoped_lock_type lock(instance_->mutex());
		if (instance_->unfinished()) {
			instance_->ioService()->post(*this);
		}
	}
	//
	// Socket
	//
	Socket::Socket() {}
	tjs_error TJS_INTF_METHOD Socket::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Socket::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeSocket>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Socket::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Socket::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		{
			AliveHandler::scoped_lock_type lock(instance_->mutex());
			if (instance_->unfinished()) {
				instance_->doCancel();
				instance_->ioService()->post(AliveHandler(instance_));
			}
		}
		instance_.reset();
	}
	bool Socket::resetExternalAcceptors(
		Acceptors const& acceptors,
		boost::shared_ptr<boost::asio::ip::tcp::socket> const& socket
		)
	{
		return instance_->resetExternalAcceptors(acceptors, socket);
	}
	bool Socket::resetExternalAcceptors(
		Acceptors const& acceptors,
		boost::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> > const& ssl_socket
		)
	{
		return instance_->resetExternalAcceptors(acceptors, ssl_socket);
	}
	//
	//	SUMMARY: 内部系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::intptr_type Socket::getInstance() {
		return reinterpret_cast<sprig::krkr::tjs::intptr_type>(this);
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE bool Socket::reset() {
		return instance_->reset();
	}
	//
	//	SUMMARY: 制御系メソッド
	//
	KTL_INLINE bool Socket::cancel() {
		return instance_->cancel();
	}
	KTL_INLINE bool Socket::cancelled() const {
		return instance_->cancelled();
	}
	KTL_INLINE bool Socket::close() {
		return instance_->close();
	}
	//
	//	SUMMARY: 名前解決系メソッド
	//
	KTL_INLINE bool Socket::resolve(tTJSVariantString const* host_name, tTJSVariantString const* service_name) {
		return instance_->resolve(
			sprig::str_cast<NativeSocket::impl_string_type>(sprig::krkr::tjs::as_c_str(host_name)),
			sprig::str_cast<NativeSocket::impl_string_type>(sprig::krkr::tjs::as_c_str(service_name))
			);
	}
	KTL_INLINE bool Socket::asyncResolve(tTJSVariantString const* host_name, tTJSVariantString const* service_name) {
		return instance_->asyncResolve(
			sprig::str_cast<NativeSocket::impl_string_type>(sprig::krkr::tjs::as_c_str(host_name)),
			sprig::str_cast<NativeSocket::impl_string_type>(sprig::krkr::tjs::as_c_str(service_name))
			);
	}
	KTL_INLINE bool Socket::resolvingFinished() const {
		return instance_->resolvingFinished();
	}
	KTL_INLINE bool Socket::resolvingSucceeded() const {
		return instance_->resolvingSucceeded();
	}
	KTL_INLINE tTJSVariant Socket::getEndpointList() const {
		return instance_->getEndpointList();
	}
	//
	//	SUMMARY: 接続系メソッド
	//
	KTL_INLINE bool Socket::connect() {
		return instance_->connect();
	}
	KTL_INLINE bool Socket::connect(tTJSVariant* address, tTVInteger port) {
		return instance_->connect(
			address,
			boost::numeric_cast<unsigned short>(port)
			);
	}
	KTL_INLINE bool Socket::connectSSL() {
		return instance_->connectSSL();
	}
	KTL_INLINE bool Socket::connectSSL(tTJSVariant* address, tTVInteger port) {
		return instance_->connectSSL(
			address,
			boost::numeric_cast<unsigned short>(port)
			);
	}
	KTL_INLINE bool Socket::connectLoopback(tTVInteger port, tTVInteger protocol_flag) {
		return instance_->connectLoopback(
			boost::numeric_cast<unsigned short>(port),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(protocol_flag)
			);
	}
	KTL_INLINE bool Socket::connectLoopbackSSL(tTVInteger port, tTVInteger protocol_flag) {
		return instance_->connectLoopbackSSL(
			boost::numeric_cast<unsigned short>(port),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(protocol_flag)
			);
	}
	KTL_INLINE bool Socket::asyncConnect() {
		return instance_->asyncConnect();
	}
	KTL_INLINE bool Socket::asyncConnect(tTJSVariant* address, tTVInteger port) {
		return instance_->asyncConnect(
			address,
			boost::numeric_cast<unsigned short>(port)
			);
	}
	KTL_INLINE bool Socket::asyncConnectSSL() {
		return instance_->asyncConnectSSL();
	}
	KTL_INLINE bool Socket::asyncConnectSSL(tTJSVariant* address, tTVInteger port) {
		return instance_->asyncConnectSSL(
			address,
			boost::numeric_cast<unsigned short>(port)
			);
	}
	KTL_INLINE bool Socket::asyncConnectLoopback(tTVInteger port, tTVInteger protocol_flag) {
		return instance_->asyncConnectLoopback(
			boost::numeric_cast<unsigned short>(port),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(protocol_flag)
			);
	}
	KTL_INLINE bool Socket::asyncConnectLoopbackSSL(tTVInteger port, tTVInteger protocol_flag) {
		return instance_->asyncConnectLoopbackSSL(
			boost::numeric_cast<unsigned short>(port),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(protocol_flag)
			);
	}
	KTL_INLINE bool Socket::connectingFinished() const {
		return instance_->connectingFinished();
	}
	KTL_INLINE bool Socket::connectingSucceeded() const {
		return instance_->connectingSucceeded();
	}
	//
	//	SUMMARY: 接続待ち系メソッド
	//
	KTL_INLINE bool Socket::accept(tTVInteger port, tTVInteger protocol_flag) {
		return instance_->accept(
			boost::numeric_cast<unsigned short>(port),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(protocol_flag)
			);
	}
	KTL_INLINE bool Socket::acceptSSL(tTVInteger port, tTVInteger protocol_flag) {
		return instance_->acceptSSL(
			boost::numeric_cast<unsigned short>(port),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(protocol_flag)
			);
	}
	KTL_INLINE bool Socket::asyncAccept(tTVInteger port, tTVInteger protocol_flag) {
		return instance_->asyncAccept(
			boost::numeric_cast<unsigned short>(port),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(protocol_flag)
			);
	}
	KTL_INLINE bool Socket::asyncAcceptSSL(tTVInteger port, tTVInteger protocol_flag) {
		return instance_->asyncAcceptSSL(
			boost::numeric_cast<unsigned short>(port),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(protocol_flag)
			);
	}
	KTL_INLINE bool Socket::acceptingFinished() const {
		return instance_->acceptingFinished();
	}
	KTL_INLINE bool Socket::acceptingSucceeded() const {
		return instance_->acceptingSucceeded();
	}
	//
	//	SUMMARY: ハンドシェイク系メソッド
	//
	KTL_INLINE bool Socket::handshake() {
		return instance_->handshake();
	}
	KTL_INLINE bool Socket::asyncHandshake() {
		return instance_->asyncHandshake();
	}
	KTL_INLINE bool Socket::handshakingFinished() const {
		return instance_->handshakingFinished();
	}
	KTL_INLINE bool Socket::handshakingSucceeded() const {
		return instance_->handshakingSucceeded();
	}
	//
	//	SUMMARY: 送信系メソッド
	//
	KTL_INLINE void Socket::pushByte(tTVInteger source) {
		instance_->pushByte(
			static_cast<NativeSocket::byte_type>(source)
			);
	}
	KTL_INLINE void Socket::pushString(tTJSVariantString const* source) {
		instance_->pushString(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE void Socket::pushNarrowString(tTJSVariantString const* source) {
		instance_->pushNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(sprig::krkr::tjs::as_c_str(source)).c_str()
			);
	}
	KTL_INLINE void Socket::pushOctet(tTJSVariantOctet const* source) {
		instance_->pushOctet(source);
	}
	KTL_INLINE void Socket::pushLine(tTJSVariantString const* source) {
		instance_->pushLine(
			sprig::str_cast<NativeSocket::impl_string_type>(sprig::krkr::tjs::as_c_str(source))
			);
	}
	KTL_INLINE tTVInteger Socket::write() {
		return instance_->write();
	}
	KTL_INLINE tTVInteger Socket::asyncWrite() {
		return instance_->asyncWrite();
	}
	KTL_INLINE bool Socket::writingFinished() const {
		return instance_->writingFinished();
	}
	KTL_INLINE bool Socket::writingSucceeded() const {
		return instance_->writingSucceeded();
	}
	KTL_INLINE tTVInteger Socket::writingBufferSize() const {
		return instance_->writingBufferSize();
	}
	//
	//	SUMMARY: 送信バッファ取得系メソッド
	//
	KTL_INLINE tTJSVariant Socket::getWritingBufferAsOctet() const {
		return instance_->getWritingBufferAsOctet();
	}
	KTL_INLINE tTJSVariant Socket::getWritingBufferAsOctet(tTVInteger pos) const {
		return instance_->getWritingBufferAsOctet(
			boost::numeric_cast<NativeSocket::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Socket::getWritingBufferAsOctet(tTVInteger pos, tTVInteger size) const {
		return instance_->getWritingBufferAsOctet(
			boost::numeric_cast<NativeSocket::size_type>(pos),
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Socket::getWritingBufferAsString() const {
		return instance_->getWritingBufferAsString();
	}
	KTL_INLINE tTJSVariant Socket::getWritingBufferAsString(tTVInteger pos) const {
		return instance_->getWritingBufferAsString(
			boost::numeric_cast<NativeSocket::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Socket::getWritingBufferAsString(tTVInteger pos, tTVInteger size) const {
		return instance_->getWritingBufferAsString(
			boost::numeric_cast<NativeSocket::size_type>(pos),
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Socket::getWritingBufferAsNarrowString() const {
		return instance_->getWritingBufferAsNarrowString();
	}
	KTL_INLINE tTJSVariant Socket::getWritingBufferAsNarrowString(tTVInteger pos) const {
		return instance_->getWritingBufferAsNarrowString(
			boost::numeric_cast<NativeSocket::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Socket::getWritingBufferAsNarrowString(tTVInteger pos, tTVInteger size) const {
		return instance_->getWritingBufferAsNarrowString(
			boost::numeric_cast<NativeSocket::size_type>(pos),
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	KTL_INLINE tTVInteger Socket::consumeWritingBuffer() {
		return instance_->consumeWritingBuffer();
	}
	KTL_INLINE tTVInteger Socket::consumeWritingBuffer(tTVInteger size) {
		return instance_->consumeWritingBuffer(
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	//
	//	SUMMARY: 受信系メソッド
	//
	KTL_INLINE tTJSVariant Socket::popByte() {
		return instance_->popByte();
	}
	KTL_INLINE tTJSVariant Socket::popString() {
		return instance_->popString();
	}
	KTL_INLINE tTJSVariant Socket::popString(tTVInteger size) {
		return instance_->popString(
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Socket::popNarrowString() {
		return instance_->popNarrowString();
	}
	KTL_INLINE tTJSVariant Socket::popNarrowString(tTVInteger size) {
		return instance_->popNarrowString(
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Socket::popOctet() {
		return instance_->popOctet();
	}
	KTL_INLINE tTJSVariant Socket::popOctet(tTVInteger size) {
		return instance_->popOctet(
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Socket::popLine() {
		return instance_->popLine();
	}
	KTL_INLINE tTVInteger Socket::read() {
		return instance_->read();
	}
	KTL_INLINE tTVInteger Socket::read(tTVInteger size, tTVInteger complete_condition_flag) {
		return instance_->read(
			boost::numeric_cast<NativeSocket::size_type>(size),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(complete_condition_flag)
			);
	}
	KTL_INLINE tTVInteger Socket::readUntilByte(tTVInteger delim) {
		return instance_->readUntilByte(
			static_cast<NativeSocket::byte_type>(delim)
			);
	}
	KTL_INLINE tTVInteger Socket::readUntilString(tTJSVariantString const* delim) {
		return instance_->readUntilString(
			sprig::krkr::tjs::as_c_str(delim)
			);
	}
	KTL_INLINE tTVInteger Socket::readUntilNarrowString(tTJSVariantString const* delim) {
		return instance_->readUntilNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(sprig::krkr::tjs::as_c_str(delim)).c_str()
			);
	}
	KTL_INLINE tTVInteger Socket::readUntilOctet(tTJSVariantOctet const* delim) {
		return instance_->readUntilOctet(
			delim
			);
	}
	KTL_INLINE tTVInteger Socket::readUntil(tTJSVariantString const* delim) {
		return instance_->readUntil(
			sprig::str_cast<NativeSocket::impl_string_type>(sprig::krkr::tjs::as_c_str(delim))
			);
	}
	KTL_INLINE tTVInteger Socket::readUntilRegex(tTJSVariantString const* expr) {
		return instance_->readUntilRegex(
			sprig::str_cast<NativeSocket::impl_string_type>(sprig::krkr::tjs::as_c_str(expr))
			);
	}
	KTL_INLINE tTVInteger Socket::asyncRead() {
		return instance_->asyncRead();
	}
	KTL_INLINE tTVInteger Socket::asyncRead(tTVInteger size, tTVInteger complete_condition_flag) {
		return instance_->asyncRead(
			boost::numeric_cast<NativeSocket::size_type>(size),
			sprig::numeric::bit_cast<NativeSocket::flag_type>(complete_condition_flag)
			);
	}
	KTL_INLINE tTVInteger Socket::asyncReadUntilByte(tTVInteger delim) {
		return instance_->asyncReadUntilByte(
			static_cast<NativeSocket::byte_type>(delim)
			);
	}
	KTL_INLINE tTVInteger Socket::asyncReadUntilString(tTJSVariantString const* delim) {
		return instance_->asyncReadUntilString(
			sprig::krkr::tjs::as_c_str(delim)
			);
	}
	KTL_INLINE tTVInteger Socket::asyncReadUntilNarrowString(tTJSVariantString const* delim) {
		return instance_->asyncReadUntilNarrowString(
			sprig::str_cast<std::basic_string<tjs_nchar> >(sprig::krkr::tjs::as_c_str(delim)).c_str()
			);
	}
	KTL_INLINE tTVInteger Socket::asyncReadUntilOctet(tTJSVariantOctet const* delim) {
		return instance_->asyncReadUntilOctet(
			delim
			);
	}
	KTL_INLINE tTVInteger Socket::asyncReadUntil(tTJSVariantString const* delim) {
		return instance_->asyncReadUntil(
			sprig::str_cast<NativeSocket::impl_string_type>(sprig::krkr::tjs::as_c_str(delim))
			);
	}
	KTL_INLINE tTVInteger Socket::asyncReadUntilRegex(tTJSVariantString const* expr) {
		return instance_->asyncReadUntilRegex(
			sprig::str_cast<NativeSocket::impl_string_type>(sprig::krkr::tjs::as_c_str(expr))
			);
	}
	KTL_INLINE bool Socket::readingFinished() const {
		return instance_->readingFinished();
	}
	KTL_INLINE bool Socket::readingSucceeded() const {
		return instance_->readingSucceeded();
	}
	KTL_INLINE bool Socket::eof() const {
		return instance_->eof();
	}
	KTL_INLINE tTVInteger Socket::readingBufferSize() const {
		return instance_->readingBufferSize();
	}
	//
	//	SUMMARY: 受信バッファ取得系メソッド
	//
	KTL_INLINE tTJSVariant Socket::getReadingBufferAsOctet() const {
		return instance_->getReadingBufferAsOctet();
	}
	KTL_INLINE tTJSVariant Socket::getReadingBufferAsOctet(tTVInteger pos) const {
		return instance_->getReadingBufferAsOctet(
			boost::numeric_cast<NativeSocket::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Socket::getReadingBufferAsOctet(tTVInteger pos, tTVInteger size) const {
		return instance_->getReadingBufferAsOctet(
			boost::numeric_cast<NativeSocket::size_type>(pos),
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Socket::getReadingBufferAsString() const {
		return instance_->getReadingBufferAsString();
	}
	KTL_INLINE tTJSVariant Socket::getReadingBufferAsString(tTVInteger pos) const {
		return instance_->getReadingBufferAsString(
			boost::numeric_cast<NativeSocket::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Socket::getReadingBufferAsString(tTVInteger pos, tTVInteger size) const {
		return instance_->getReadingBufferAsString(
			boost::numeric_cast<NativeSocket::size_type>(pos),
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Socket::getReadingBufferAsNarrowString() const {
		return instance_->getReadingBufferAsNarrowString();
	}
	KTL_INLINE tTJSVariant Socket::getReadingBufferAsNarrowString(tTVInteger pos) const {
		return instance_->getReadingBufferAsNarrowString(
			boost::numeric_cast<NativeSocket::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Socket::getReadingBufferAsNarrowString(tTVInteger pos, tTVInteger size) const {
		return instance_->getReadingBufferAsNarrowString(
			boost::numeric_cast<NativeSocket::size_type>(pos),
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	KTL_INLINE tTVInteger Socket::consumeReadingBuffer() {
		return instance_->consumeReadingBuffer();
	}
	KTL_INLINE tTVInteger Socket::consumeReadingBuffer(tTVInteger size) {
		return instance_->consumeReadingBuffer(
			boost::numeric_cast<NativeSocket::size_type>(size)
			);
	}
	//
	//	SUMMARY: タイムアウト系メソッド
	//
	KTL_INLINE bool Socket::waitTimeoutFromNow(tTVInteger expiry_time_millisec) {
		return instance_->waitTimeoutFromNow(
			boost::numeric_cast<NativeSocket::rep_type>(expiry_time_millisec)
			);
	}
	KTL_INLINE bool Socket::waitTimeoutAt(tTVInteger expiry_time_millisec) {
		return instance_->waitTimeoutAt(
			boost::numeric_cast<NativeSocket::rep_type>(expiry_time_millisec)
			);
	}
	KTL_INLINE bool Socket::asyncWaitTimeoutFromNow(tTVInteger expiry_time_millisec) {
		return instance_->asyncWaitTimeoutFromNow(
			boost::numeric_cast<NativeSocket::rep_type>(expiry_time_millisec)
			);
	}
	KTL_INLINE bool Socket::asyncWaitTimeoutAt(tTVInteger expiry_time_millisec) {
		return instance_->asyncWaitTimeoutAt(
			boost::numeric_cast<NativeSocket::rep_type>(expiry_time_millisec)
			);
	}
	KTL_INLINE bool Socket::cancelTimeout() {
		return instance_->cancelTimeout();
	}
	KTL_INLINE bool Socket::isWaitingTimeout() const {
		return instance_->isWaitingTimeout();
	}
	KTL_INLINE tTVInteger Socket::expiresTimeoutFromNow() const {
		return instance_->expiresTimeoutFromNow();
	}
	KTL_INLINE tTVInteger Socket::expiresTimeoutAt() const {
		return instance_->expiresTimeoutAt();
	}
	//
	//	SUMMARY: 待機系メソッド
	//
	KTL_INLINE bool Socket::join() {
		return instance_->join();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE bool Socket::isProcessing() const {
		return instance_->isProcessing();
	}
	KTL_INLINE bool Socket::isOpen() const {
		return instance_->isOpen();
	}
	KTL_INLINE bool Socket::isSSL() const {
		return instance_->isSSL();
	}
	//
	//	SUMMARY: コールバック系メソッド
	//
	KTL_INLINE tTJSVariant Socket::getOnFinished() const {
		return instance_->getOnFinished();
	}
	KTL_INLINE void Socket::setOnFinished(tTJSVariant const& func) {
		instance_->setOnFinished(func);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_NETWORK_SOCKET_HPP
