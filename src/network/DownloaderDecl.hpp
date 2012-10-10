#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <fstream>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/thread.hpp>
#include <boost/cstdint.hpp>
#include <boost/range/iterator_range.hpp>
#include <sprig/config/lib/openssl.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

#include "NetworkCommon.hpp"

namespace ktl {
	//
	// NativeDownloader
	//
	class NativeDownloader {
	public:
		typedef NetworkService::size_type size_type;
		typedef NetworkUtils::impl_string_type impl_string_type;
		typedef NetworkUtils::flag_type flag_type;
		typedef NetworkUtils::timeout_timer_type timeout_timer_type;
		typedef NetworkUtils::time_point_type time_point_type;
		typedef NetworkUtils::rep_type rep_type;
		typedef NetworkUtils::milliseconds_type milliseconds_type;
		typedef boost::int_fast32_t int_type;
		typedef std::vector<char> buffer_type;
		typedef boost::recursive_mutex mutex_type;
		typedef mutex_type::scoped_lock scoped_lock_type;
	private:
		static size_type getEnableSize(size_type length, size_type spos);
		static size_type getEnableSize(size_type length, size_type spos, size_type ssize);
		static char const* bufferData(boost::asio::streambuf const& buffer);
		static char const* bufferEnd(boost::asio::streambuf const& buffer);
		static boost::iterator_range<char const*> bufferRange(boost::asio::streambuf const& buffer);
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
		Handshakers handshakers_;
		Writers writers_;
		Readers readers_;
		TimeoutTimers timeout_timers_;
		//
		Resolvers resolvers2_;
		Connectors connectors2_;
		Handshakers handshakers2_;
		Writers writers2_;
		Readers readers2_;
		TimeoutTimers timeout_timers2_;
		//
		URLInfo url_info_;
		URLInfo url_info2_;
		HTTPResponse http_response_;
		HTTPResponse http_response2_;
		HTTPHeader http_header_;
		HTTPHeader http_header2_;
		//
		flag_type flag_;
		boost::shared_ptr<std::string> storage_;
		size_type transferred_size_;
		size_type content_length_;
		bool chunked_;
		//
		boost::shared_ptr<std::ofstream> storage_out_;
		boost::shared_ptr<buffer_type> buffer_;
		//
		boost::shared_ptr<buffer_type> upload_buffer_;
		//
		bool is_processing_;
		bool failed_;
		bool cancelled_;
		//
		boost::shared_ptr<tTJSVariant> on_finished_;
		//
		boost::shared_ptr<std::string> custom_header_;
		//
		bool process_status_code_error_;
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
		void handleHandshake(
			boost::system::error_code const& error
			);
		void handleWrite(
			boost::system::error_code const& error,
			size_type bytes_transferred
			);
		void handleReadUntil(
			boost::system::error_code const& error,
			size_type bytes_transferred
			);
		void handleLoadBlock(
			boost::system::error_code const& error,
			size_type bytes_transferred
			);
		void handleReadChunkSize(
			boost::system::error_code const& error,
			size_type bytes_transferred
			);
		void handleReadChunkData(
			boost::system::error_code const& error,
			size_type bytes_transferred,
			size_type remaind_chunk_size
			);
		void handleReadChunkDataAfter(
			boost::system::error_code const& error,
			size_type bytes_transferred
			);
		void handleTimeout(
			boost::system::error_code const& error
			);
		bool resolve(impl_string_type const& host_name, impl_string_type const& service_name);
		bool connect();
		bool connectSSL();
		bool handshake();
		int_type write();
		int_type read();
		int_type read(size_type size);
		int_type readUntil(impl_string_type const& delim);
		bool loadBlock();
		bool loadChunked();
		bool analyURL(impl_string_type const& url);
		bool setStorageName(tjs_char const* storage, bool to_buffer);
		void putHTTPRequest(impl_string_type const& host_name, impl_string_type const& content_path);
		bool analyHTTPResponse(bool async);
		bool openBuffer(bool async);
		bool updateBuffer(size_type bytes_transferred);
		void resetInfo();
		void resetWorkers();
		void resetWorkingBuffer(bool setup = false);
		void resetBuffer(bool setup = false);
		void resetSocket();
		void enableSocket();
		void enableSocketSSL();
		void setupOnProcessStart(bool to_buffer, flag_type flag);
		void cleanupOnProcessFailed();
		void cleanupOnProcessSucceeded();
		bool downloadHTTPImpl(
			tjs_char const* url,
			tjs_char const* storage,
			bool to_buffer,
			flag_type flag = 0x00000000
			);
		bool asyncDownloadHTTPImpl(
			tjs_char const* url,
			tjs_char const* storage,
			bool to_buffer,
			flag_type flag = 0x00000000
			);
		bool cancelImpl();
		bool cancelTimeoutImpl();
		bool cancelTimeoutImplNoErrorHandling();
	public:
		NativeDownloader();
		boost::shared_ptr<boost::asio::io_service> const& ioService() const;
		mutex_type& mutex() const;
		bool unfinished() const;
		bool doCancel();
	public:
		//
		//	SUMMARY: ダウンロード系フラグ
		//
		static flag_type const dhfAnalyHTTPHeader = 0x00000001;
	public:
		//
		//	SUMMARY: 初期化系メソッド
		//
		bool reset();
		//
		//	SUMMARY: ダウンロード系メソッド
		//
		bool downloadHTTP(
			tjs_char const* url,
			tjs_char const* storage,
			flag_type flag = 0x00000000
			);
		bool asyncDownloadHTTP(
			tjs_char const* url,
			tjs_char const* storage,
			flag_type flag = 0x00000000
			);
		bool cancel();
		bool isProcessing() const;
		bool failed() const;
		bool cancelled() const;
		//
		//	SUMMARY: 情報系メソッド
		//
		size_type transferredSize() const;
		size_type contentLength() const;
		bool chunked() const;
		//
		//	SUMMARY: バッファダウンロード系メソッド
		//
		bool downloadHTTPToBuffer(
			tjs_char const* url,
			flag_type flag = 0x00000000
			);
		bool asyncDownloadHTTPToBuffer(
			tjs_char const* url,
			flag_type flag = 0x00000000
			);
		bool clearBuffer();
		bool bufferEnable() const;
		int_type bufferSize() const;
		//
		//	SUMMARY: バッファ取得系メソッド
		//
		tTJSVariant getBufferAsOctet() const;
		tTJSVariant getBufferAsOctet(size_type pos) const;
		tTJSVariant getBufferAsOctet(size_type pos, size_type size) const;
		tTJSVariant getBufferAsString() const;
		tTJSVariant getBufferAsString(size_type pos) const;
		tTJSVariant getBufferAsString(size_type pos, size_type size) const;
		tTJSVariant getBufferAsNarrowString() const;
		tTJSVariant getBufferAsNarrowString(size_type pos) const;
		tTJSVariant getBufferAsNarrowString(size_type pos, size_type size) const;
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
		//	SUMMARY: ポスト系メソッド
		//
		bool addPostDataEncoded(tjs_char const* source);
		bool addPostDataEncoded(tTJSVariantOctet const* source);
		bool clearPostData();
		bool postDataEnable() const;
		int_type postDataSize() const;
		//
		//	SUMMARY: URL情報系メソッド
		//
		impl_string_type const& hostName() const;
		impl_string_type const& serviceName() const;
		impl_string_type const& contentPath() const;
		impl_string_type URL() const;
		std::string storageLocalName() const;
		//
		//	SUMMARY: HTTPレスポンス系メソッド
		//
		impl_string_type const& HTTPVersion() const;
		impl_string_type const& statusCode() const;
		impl_string_type const& reasonPhrase() const;
		tTJSVariant getField(tjs_char const* name) const;
		//
		//	SUMMARY: コールバック系メソッド
		//
		tTJSVariant getOnFinished() const;
		void setOnFinished(tTJSVariant const& func);
		//
		//	SUMMARY: カスタムヘッダ系メソッド
		//
		bool setCustomHeader(tjs_char const* source);
		bool clearCustomHeader();
		//
		//	SUMMARY: 処理設定系メソッド
		//
		void setProcessStatusCodeError(bool value);
		bool getProcessStatusCodeError() const;
	};

	//
	// Downloader
	//
	class Downloader
		: public tTJSNativeInstance
	{
	private:
		//
		// AliveHandler
		//
		class AliveHandler {
		public:
			typedef NativeDownloader::scoped_lock_type scoped_lock_type;
		private:
			boost::shared_ptr<NativeDownloader> instance_;
		public:
			AliveHandler(boost::shared_ptr<NativeDownloader> const& instance);
			void operator()() const;
		};
	private:
		boost::shared_ptr<NativeDownloader> instance_;
	public:
		Downloader();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: ダウンロード系フラグ
		//
		static tTVInteger const dhfAnalyHTTPHeader = NativeDownloader::dhfAnalyHTTPHeader;
	public:
		//
		//	SUMMARY: 初期化系メソッド
		//
		bool reset();
		//
		//	SUMMARY: ダウンロード系メソッド
		//
		bool downloadHTTP(
			tTJSVariantString const* url,
			tTJSVariantString const* storage,
			tTVInteger flag = 0x00000000
			);
		bool asyncDownloadHTTP(
			tTJSVariantString const* url,
			tTJSVariantString const* storage,
			tTVInteger flag = 0x00000000
			);
		bool cancel();
		bool isProcessing() const;
		bool failed() const;
		bool cancelled() const;
		//
		//	SUMMARY: 情報系メソッド
		//
		tTVInteger transferredSize() const;
		tTVInteger contentLength() const;
		bool chunked() const;
		//
		//	SUMMARY: バッファダウンロード系メソッド
		//
		bool downloadHTTPToBuffer(
			tTJSVariantString const* url,
			tTVInteger flag = 0x00000000
			);
		bool asyncDownloadHTTPToBuffer(
			tTJSVariantString const* url,
			tTVInteger flag = 0x00000000
			);
		bool clearBuffer();
		bool bufferEnable() const;
		tTVInteger bufferSize() const;
		//
		//	SUMMARY: バッファ取得系メソッド
		//
		tTJSVariant getBufferAsOctet() const;
		tTJSVariant getBufferAsOctet(tTVInteger pos) const;
		tTJSVariant getBufferAsOctet(tTVInteger pos, tTVInteger size) const;
		tTJSVariant getBufferAsString() const;
		tTJSVariant getBufferAsString(tTVInteger pos) const;
		tTJSVariant getBufferAsString(tTVInteger pos, tTVInteger size) const;
		tTJSVariant getBufferAsNarrowString() const;
		tTJSVariant getBufferAsNarrowString(tTVInteger pos) const;
		tTJSVariant getBufferAsNarrowString(tTVInteger pos, tTVInteger size) const;
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
		//	SUMMARY: ポスト系メソッド
		//
		bool addPostDataEncoded(tTJSVariantString const* source);
		bool addPostDataEncoded(tTJSVariantOctet const* source);
		bool clearPostData();
		bool postDataEnable() const;
		tTVInteger postDataSize() const;
		//
		//	SUMMARY: URL情報系メソッド
		//
		tTJSString hostName() const;
		tTJSString serviceName() const;
		tTJSString contentPath() const;
		tTJSString URL() const;
		tTJSString storageLocalName() const;
		//
		//	SUMMARY: HTTPレスポンス系メソッド
		//
		tTJSString HTTPVersion() const;
		tTJSString statusCode() const;
		tTJSString reasonPhrase() const;
		tTJSVariant getField(tTJSVariantString const* name) const;
		//
		//	SUMMARY: コールバック系メソッド
		//
		tTJSVariant getOnFinished() const;
		void setOnFinished(tTJSVariant const& func);
		//
		//	SUMMARY: カスタムヘッダ系メソッド
		//
		bool setCustomHeader(tTJSVariantString const* source);
		bool clearCustomHeader();
		//
		//	SUMMARY: 処理設定系メソッド
		//
		void setProcessStatusCodeError(bool value);
		bool getProcessStatusCodeError() const;
	};
}	// namespace ktl
