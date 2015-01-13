/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_NETWORK_SOCKET_DECL_HPP
#define SRC_NETWORK_SOCKET_DECL_HPP

#include <cstddef>
#include <string>
#include <vector>
#include <utility>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/thread.hpp>
#include <boost/cstdint.hpp>
#include <sprig/config/lib/openssl.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

#include "NetworkCommon.hpp"

namespace ktl {
	//
	// NativeSocket
	//
	class NativeSocket {
	public:
		typedef tjs_uint8 byte_type;
		typedef NetworkService::size_type size_type;
		typedef NetworkUtils::impl_string_type impl_string_type;
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
		// MatchCondition
		//
		class MatchCondition {
		public:
			typedef std::vector<char> binary_type;
		private:
			binary_type data_;
		public:
			template<typename Iterator>
			MatchCondition(Iterator first, Iterator last);
			template<typename Iterator>
			std::pair<Iterator, bool> operator()(Iterator first, Iterator last) const;
		};
	private:
		static size_type getEnableSize(size_type length, size_type spos);
		static size_type getEnableSize(size_type length, size_type spos, size_type ssize);
		static boost::optional<boost::asio::ip::address_v4> convertAddressIPv4(tTJSVariant* address);
		static boost::optional<boost::asio::ip::address_v6> convertAddressIPv6(tTJSVariant* address);
		static boost::optional<boost::asio::ip::address> convertAddress(tTJSVariant* address);
	public:
		static sprig::krkr::tjs::intptr_type getInstance(iTJSDispatch2* obj);
		static sprig::krkr::tjs::object_type createNew(tjs_int numparams, tTJSVariant** param);
	private:
		//
		boost::shared_ptr<boost::asio::io_service> io_service_;
		boost::shared_ptr<boost::asio::io_service::strand> strand_;
		boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
		boost::shared_ptr<boost::asio::ip::tcp::socket> socket2_;
		boost::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> > ssl_socket_;
		boost::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> > ssl_socket2_;
		boost::shared_ptr<boost::asio::streambuf> writing_streambuf_;
		boost::shared_ptr<boost::asio::streambuf> reading_streambuf_;
		boost::shared_ptr<timeout_timer_type> timeout_timer_;
		//
		Resolvers resolvers_;
		Connectors connectors_;
		Acceptors acceptors_;
		Handshakers handshakers_;
		Writers writers_;
		Readers readers_;
		TimeoutTimers timeout_timers_;
		//
		Resolvers resolvers2_;
		Connectors connectors2_;
		Acceptors acceptors2_;
		Handshakers handshakers2_;
		Writers writers2_;
		Readers readers2_;
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
		void handleResolove(
			boost::system::error_code const& error,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator
			);
		void handleConnect(
			boost::system::error_code const& error,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator
			);
		void handleConnectSSL(
			boost::system::error_code const& error,
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator
			);
		void handleAccept(
			boost::system::error_code const& error
			);
		void handleAcceptSSL(
			boost::system::error_code const& error
			);
		void handleHandshake(
			boost::system::error_code const& error
			);
		void handleWrite(
			boost::system::error_code const& error,
			size_type bytes_transferred
			);
		void handleRead(
			boost::system::error_code const& error,
			size_type bytes_transferred
			);
		void handleReadUntil(
			boost::system::error_code const& error,
			size_type bytes_transferred
			);
		void handleTimeout(
			boost::system::error_code const& error
			);
		bool setupResolve(impl_string_type const& host_name, impl_string_type const& service_name);
		bool setupConnect();
		bool setupConnect(boost::optional<boost::asio::ip::address> const& addr, unsigned short port);
		bool setupConnectSSL();
		bool setupConnectSSL(boost::optional<boost::asio::ip::address> const& addr, unsigned short port);
		bool setupConnectLoopback(unsigned short port, flag_type protocol_flag);
		bool setupConnectLoopbackSSL(unsigned short port, flag_type protocol_flag);
		bool setupAccept(unsigned short port, flag_type protocol_flag);
		bool setupAcceptSSL(unsigned short port, flag_type protocol_flag);
		bool setupHandshake();
		bool setupWrite();
		bool setupRead();
		bool finishResolve(boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
		bool finishConnect();
		bool finishConnectSSL();
		bool finishAccept();
		bool finishAcceptSSL();
		bool finishHandshake();
		bool finishWrite();
		bool finishRead();
		bool checkResolvers();
		bool checkConnectors();
		bool checkAcceptors();
		bool checkHandshakers();
		bool checkWriters();
		bool checkReaders();
		void enableSocket();
		void enableSocketSSL();
		void setupOnProcessStart();
		void cleanupOnProcessFailed();
		void cleanupOnProcessSucceeded();
		bool setupAcceptor();
		template<typename Char>
		void copyBuffer(
			boost::asio::streambuf const& buf,
			Char* dest,
			size_type pos,
			size_type size
			) const;
		tTJSVariant getBufferAsOctetImpl(
			boost::asio::streambuf const& buf,
			size_type pos,
			size_type size
			) const;
		tTJSVariant getBufferAsStringImpl(
			boost::asio::streambuf const& buf,
			size_type pos,
			size_type size
			) const;
		tTJSVariant getBufferAsNarrowStringImpl(
			boost::asio::streambuf const& buf,
			size_type pos,
			size_type size
			) const;
		bool cancelImpl();
		bool cancelTimeoutImpl();
		bool cancelTimeoutImplNoErrorHandling();
	public:
		NativeSocket();
		boost::shared_ptr<boost::asio::io_service> const& ioService() const;
		mutex_type& mutex() const;
		bool unfinished() const;
		bool doCancel();
		bool resetExternalAcceptors(
			Acceptors const& acceptors,
			boost::shared_ptr<boost::asio::ip::tcp::socket> const& socket
			);
		bool resetExternalAcceptors(
			Acceptors const& acceptors,
			boost::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> > const& ssl_socket
			);
	public:
		//
		//	SUMMARY: プロトコル系フラグ
		//
		static flag_type const pfIPv4 = NetworkUtils::pfIPv4;
		static flag_type const pfIPv6 = NetworkUtils::pfIPv6;
		//
		//	SUMMARY: 完了条件系フラグ
		//
		static flag_type const ccfTransferAtLeast = NetworkUtils::ccfTransferAtLeast;
		static flag_type const ccfTransferExactly = NetworkUtils::ccfTransferExactly;
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
		//	SUMMARY: 名前解決系メソッド
		//
		bool resolve(impl_string_type const& host_name, impl_string_type const& service_name);
		bool asyncResolve(impl_string_type const& host_name, impl_string_type const& service_name);
		bool resolvingFinished() const;
		bool resolvingSucceeded() const;
		tTJSVariant getEndpointList() const;
		//
		//	SUMMARY: 接続系メソッド
		//
		bool connect();
		bool connect(tTJSVariant* address, unsigned short port);
		bool connectSSL();
		bool connectSSL(tTJSVariant* address, unsigned short port);
		bool connectLoopback(unsigned short port, flag_type protocol_flag = NetworkUtils::pfIPv4);
		bool connectLoopbackSSL(unsigned short port, flag_type protocol_flag = NetworkUtils::pfIPv4);
		bool asyncConnect();
		bool asyncConnect(tTJSVariant* address, unsigned short port);
		bool asyncConnectSSL();
		bool asyncConnectSSL(tTJSVariant* address, unsigned short port);
		bool asyncConnectLoopback(unsigned short port, flag_type protocol_flag = NetworkUtils::pfIPv4);
		bool asyncConnectLoopbackSSL(unsigned short port, flag_type protocol_flag = NetworkUtils::pfIPv4);
		bool connectingFinished() const;
		bool connectingSucceeded() const;
		//
		//	SUMMARY: 接続待ち系メソッド
		//
		bool accept(unsigned short port, flag_type protocol_flag = pfIPv4);
		bool acceptSSL(unsigned short port, flag_type protocol_flag = pfIPv4);
		bool asyncAccept(unsigned short port, flag_type protocol_flag = pfIPv4);
		bool asyncAcceptSSL(unsigned short port, flag_type protocol_flag = pfIPv4);
		bool acceptingFinished() const;
		bool acceptingSucceeded() const;
		//
		//	SUMMARY: ハンドシェイク系メソッド
		//
		bool handshake();
		bool asyncHandshake();
		bool handshakingFinished() const;
		bool handshakingSucceeded() const;
		//
		//	SUMMARY: 送信系メソッド
		//
		void pushByte(byte_type source);
		void pushString(tjs_char const* source);
		void pushNarrowString(tjs_nchar const* source);
		void pushOctet(tTJSVariantOctet const* source);
		void pushLine(impl_string_type const& source);
		int_type write();
		int_type asyncWrite();
		bool writingFinished() const;
		bool writingSucceeded() const;
		size_type writingBufferSize() const;
		//
		//	SUMMARY: 送信バッファ取得系メソッド
		//
		tTJSVariant getWritingBufferAsOctet() const;
		tTJSVariant getWritingBufferAsOctet(size_type pos) const;
		tTJSVariant getWritingBufferAsOctet(size_type pos, size_type size) const;
		tTJSVariant getWritingBufferAsString() const;
		tTJSVariant getWritingBufferAsString(size_type pos) const;
		tTJSVariant getWritingBufferAsString(size_type pos, size_type size) const;
		tTJSVariant getWritingBufferAsNarrowString() const;
		tTJSVariant getWritingBufferAsNarrowString(size_type pos) const;
		tTJSVariant getWritingBufferAsNarrowString(size_type pos, size_type size) const;
		int_type consumeWritingBuffer();
		int_type consumeWritingBuffer(size_type size);
		//
		//	SUMMARY: 受信系メソッド
		//
		tTJSVariant popByte();
		tTJSVariant popString();
		tTJSVariant popString(size_type size);
		tTJSVariant popNarrowString();
		tTJSVariant popNarrowString(size_type size);
		tTJSVariant popOctet();
		tTJSVariant popOctet(size_type size);
		tTJSVariant popLine();
		int_type read();
		int_type read(size_type size, flag_type complete_condition_flag = ccfTransferAtLeast);
		int_type readUntilByte(byte_type delim);
		int_type readUntilString(tjs_char const* delim);
		int_type readUntilNarrowString(tjs_nchar const* delim);
		int_type readUntilOctet(tTJSVariantOctet const* delim);
		int_type readUntil(impl_string_type const& delim);
		int_type readUntilRegex(impl_string_type const& expr);
		int_type asyncRead();
		int_type asyncRead(size_type size, flag_type complete_condition_flag = ccfTransferAtLeast);
		int_type asyncReadUntilByte(byte_type delim);
		int_type asyncReadUntilString(tjs_char const* delim);
		int_type asyncReadUntilNarrowString(tjs_nchar const* delim);
		int_type asyncReadUntilOctet(tTJSVariantOctet const* delim);
		int_type asyncReadUntil(impl_string_type const& delim);
		int_type asyncReadUntilRegex(impl_string_type const& expr);
		bool readingFinished() const;
		bool readingSucceeded() const;
		bool eof() const;
		size_type readingBufferSize() const;
		//
		//	SUMMARY: 受信バッファ取得系メソッド
		//
		tTJSVariant getReadingBufferAsOctet() const;
		tTJSVariant getReadingBufferAsOctet(size_type pos) const;
		tTJSVariant getReadingBufferAsOctet(size_type pos, size_type size) const;
		tTJSVariant getReadingBufferAsString() const;
		tTJSVariant getReadingBufferAsString(size_type pos) const;
		tTJSVariant getReadingBufferAsString(size_type pos, size_type size) const;
		tTJSVariant getReadingBufferAsNarrowString() const;
		tTJSVariant getReadingBufferAsNarrowString(size_type pos) const;
		tTJSVariant getReadingBufferAsNarrowString(size_type pos, size_type size) const;
		int_type consumeReadingBuffer();
		int_type consumeReadingBuffer(size_type size);
		//
		//	SUMMARY: タイムアウト系メソッド
		//
		bool waitTimeoutFromNow(rep_type expiry_time_millisec);
		bool waitTimeoutAt(rep_type expiry_time_millisec);
		bool asyncWaitTimeoutFromNow(rep_type expiry_time_millisec);
		bool asyncWaitTimeoutAt(rep_type expiry_time_millisec);
		bool cancelTimeout();
		bool isWaitingTimeout() const;
		rep_type expiresTimeoutFromNow() const;
		rep_type expiresTimeoutAt() const;
		//
		//	SUMMARY: 待機系メソッド
		//
		bool join();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool isProcessing() const;
		bool isOpen() const;
		bool isSSL() const;
		//
		//	SUMMARY: コールバック系メソッド
		//
		tTJSVariant getOnFinished() const;
		void setOnFinished(tTJSVariant const& func);
	};

	//
	// Socket
	//
	class Socket
		: public tTJSNativeInstance
	{
	private:
		//
		// AliveHandler
		//
		class AliveHandler {
		public:
			typedef NativeSocket::scoped_lock_type scoped_lock_type;
		private:
			boost::shared_ptr<NativeSocket> instance_;
		public:
			AliveHandler(boost::shared_ptr<NativeSocket> const& instance);
			void operator()() const;
		};
	private:
		boost::shared_ptr<NativeSocket> instance_;
	public:
		Socket();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
		bool resetExternalAcceptors(
			Acceptors const& acceptors,
			boost::shared_ptr<boost::asio::ip::tcp::socket> const& socket
			);
		bool resetExternalAcceptors(
			Acceptors const& acceptors,
			boost::shared_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> > const& ssl_socket
			);
	public:
		//
		//	SUMMARY: プロトコル系フラグ
		//
		static tTVInteger const pfIPv4 = NativeSocket::pfIPv4;
		static tTVInteger const pfIPv6 = NativeSocket::pfIPv6;
		//
		//	SUMMARY: 完了条件系フラグ
		//
		static tTVInteger const ccfTransferAtLeast = NativeSocket::ccfTransferAtLeast;
		static tTVInteger const ccfTransferExactly = NativeSocket::ccfTransferExactly;
	public:
		//
		//	SUMMARY: 内部系メソッド
		//
		sprig::krkr::tjs::intptr_type getInstance();
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
		//	SUMMARY: 名前解決系メソッド
		//
		bool resolve(tTJSVariantString const* host_name, tTJSVariantString const* service_name);
		bool asyncResolve(tTJSVariantString const* host_name, tTJSVariantString const* service_name);
		bool resolvingFinished() const;
		bool resolvingSucceeded() const;
		tTJSVariant getEndpointList() const;
		//
		//	SUMMARY: 接続系メソッド
		//
		bool connect();
		bool connect(tTJSVariant* address, tTVInteger port);
		bool connectSSL();
		bool connectSSL(tTJSVariant* address, tTVInteger port);
		bool connectLoopback(tTVInteger port, tTVInteger protocol_flag = NetworkUtils::pfIPv4);
		bool connectLoopbackSSL(tTVInteger port, tTVInteger protocol_flag = NetworkUtils::pfIPv4);
		bool asyncConnect();
		bool asyncConnect(tTJSVariant* address, tTVInteger port);
		bool asyncConnectSSL();
		bool asyncConnectSSL(tTJSVariant* address, tTVInteger port);
		bool asyncConnectLoopback(tTVInteger port, tTVInteger protocol_flag = NetworkUtils::pfIPv4);
		bool asyncConnectLoopbackSSL(tTVInteger port, tTVInteger protocol_flag = NetworkUtils::pfIPv4);
		bool connectingFinished() const;
		bool connectingSucceeded() const;
		//
		//	SUMMARY: 接続待ち系メソッド
		//
		bool accept(tTVInteger port, tTVInteger protocol_flag = pfIPv4);
		bool acceptSSL(tTVInteger port, tTVInteger protocol_flag = pfIPv4);
		bool asyncAccept(tTVInteger port, tTVInteger protocol_flag = pfIPv4);
		bool asyncAcceptSSL(tTVInteger port, tTVInteger protocol_flag = pfIPv4);
		bool acceptingFinished() const;
		bool acceptingSucceeded() const;
		//
		//	SUMMARY: ハンドシェイク系メソッド
		//
		bool handshake();
		bool asyncHandshake();
		bool handshakingFinished() const;
		bool handshakingSucceeded() const;
		//
		//	SUMMARY: 送信系メソッド
		//
		void pushByte(tTVInteger source);
		void pushString(tTJSVariantString const* source);
		void pushNarrowString(tTJSVariantString const* source);
		void pushOctet(tTJSVariantOctet const* source);
		void pushLine(tTJSVariantString const* source);
		tTVInteger write();
		tTVInteger asyncWrite();
		bool writingFinished() const;
		bool writingSucceeded() const;
		tTVInteger writingBufferSize() const;
		//
		//	SUMMARY: 送信バッファ取得系メソッド
		//
		tTJSVariant getWritingBufferAsOctet() const;
		tTJSVariant getWritingBufferAsOctet(tTVInteger pos) const;
		tTJSVariant getWritingBufferAsOctet(tTVInteger pos, tTVInteger size) const;
		tTJSVariant getWritingBufferAsString() const;
		tTJSVariant getWritingBufferAsString(tTVInteger pos) const;
		tTJSVariant getWritingBufferAsString(tTVInteger pos, tTVInteger size) const;
		tTJSVariant getWritingBufferAsNarrowString() const;
		tTJSVariant getWritingBufferAsNarrowString(tTVInteger pos) const;
		tTJSVariant getWritingBufferAsNarrowString(tTVInteger pos, tTVInteger size) const;
		tTVInteger consumeWritingBuffer();
		tTVInteger consumeWritingBuffer(tTVInteger size);
		//
		//	SUMMARY: 受信系メソッド
		//
		tTJSVariant popByte();
		tTJSVariant popString();
		tTJSVariant popString(tTVInteger size);
		tTJSVariant popNarrowString();
		tTJSVariant popNarrowString(tTVInteger size);
		tTJSVariant popOctet();
		tTJSVariant popOctet(tTVInteger size);
		tTJSVariant popLine();
		tTVInteger read();
		tTVInteger read(tTVInteger size, tTVInteger complete_condition_flag = ccfTransferAtLeast);
		tTVInteger readUntilByte(tTVInteger delim);
		tTVInteger readUntilString(tTJSVariantString const* delim);
		tTVInteger readUntilNarrowString(tTJSVariantString const* delim);
		tTVInteger readUntilOctet(tTJSVariantOctet const* delim);
		tTVInteger readUntil(tTJSVariantString const* delim);
		tTVInteger readUntilRegex(tTJSVariantString const* expr);
		tTVInteger asyncRead();
		tTVInteger asyncRead(tTVInteger size, tTVInteger complete_condition_flag = ccfTransferAtLeast);
		tTVInteger asyncReadUntilByte(tTVInteger delim);
		tTVInteger asyncReadUntilString(tTJSVariantString const* delim);
		tTVInteger asyncReadUntilNarrowString(tTJSVariantString const* delim);
		tTVInteger asyncReadUntilOctet(tTJSVariantOctet const* delim);
		tTVInteger asyncReadUntil(tTJSVariantString const* delim);
		tTVInteger asyncReadUntilRegex(tTJSVariantString const* expr);
		bool readingFinished() const;
		bool readingSucceeded() const;
		bool eof() const;
		tTVInteger readingBufferSize() const;
		//
		//	SUMMARY: 受信バッファ取得系メソッド
		//
		tTJSVariant getReadingBufferAsOctet() const;
		tTJSVariant getReadingBufferAsOctet(tTVInteger pos) const;
		tTJSVariant getReadingBufferAsOctet(tTVInteger pos, tTVInteger size) const;
		tTJSVariant getReadingBufferAsString() const;
		tTJSVariant getReadingBufferAsString(tTVInteger pos) const;
		tTJSVariant getReadingBufferAsString(tTVInteger pos, tTVInteger size) const;
		tTJSVariant getReadingBufferAsNarrowString() const;
		tTJSVariant getReadingBufferAsNarrowString(tTVInteger pos) const;
		tTJSVariant getReadingBufferAsNarrowString(tTVInteger pos, tTVInteger size) const;
		tTVInteger consumeReadingBuffer();
		tTVInteger consumeReadingBuffer(tTVInteger size);
		//
		//	SUMMARY: タイムアウト系メソッド
		//
		bool waitTimeoutFromNow(tTVInteger expiry_time_millisec);
		bool waitTimeoutAt(tTVInteger expiry_time_millisec);
		bool asyncWaitTimeoutFromNow(tTVInteger expiry_time_millisec);
		bool asyncWaitTimeoutAt(tTVInteger expiry_time_millisec);
		bool cancelTimeout();
		bool isWaitingTimeout() const;
		tTVInteger expiresTimeoutFromNow() const;
		tTVInteger expiresTimeoutAt() const;
		//
		//	SUMMARY: 待機系メソッド
		//
		bool join();
		//
		//	SUMMARY: プロパティ系メソッド
		//
		bool isProcessing() const;
		bool isOpen() const;
		bool isSSL() const;
		//
		//	SUMMARY: コールバック系メソッド
		//
		tTJSVariant getOnFinished() const;
		void setOnFinished(tTJSVariant const& func);
	};
}	// namespace ktl

namespace boost {
	namespace asio {
		//
		// is_match_condition
		//
		template<>
		struct is_match_condition<ktl::NativeSocket::MatchCondition>
			: public boost::true_type
		{};
	}	// namespace asio
}	// namespace boost

#endif	// #ifndef SRC_NETWORK_SOCKET_DECL_HPP
