/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_NETWORK_NETWORK_COMMON_HPP
#define SRC_NETWORK_NETWORK_COMMON_HPP

#include <cstddef>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <boost/cstdint.hpp>
#include <boost/array.hpp>
#include <boost/ref.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/basic_waitable_timer.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/serialization/singleton.hpp>
#include <sprig/config/lib/openssl.hpp>
#include <sprig/external/windows.hpp>
#include <sprig/external/wininet.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/exception.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>

namespace ktl {
	//
	// NetworkServiceImpl
	//
	class NetworkServiceImpl {
	public:
		typedef std::size_t size_type;
		typedef boost::mutex mutex_type;
		typedef mutex_type::scoped_lock scoped_lock_type;
	private:
		boost::shared_ptr<boost::asio::io_service> io_service_;
		boost::shared_ptr<boost::asio::io_service::work> work_;
		boost::shared_ptr<boost::thread_group> thread_group_;
		std::vector<boost::thread*> thread_list_;
		mutable mutex_type mutex_;
	private:
		void run_impl(size_type new_thread_count) {
			SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:NetworkService:run"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("thread_count"), thread_list_.size(), SPRIG_KRKR_LOG_LEVEL_NORMAL);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("new_thread_count"), new_thread_count, SPRIG_KRKR_LOG_LEVEL_NORMAL);
			if (!thread_list_.size()) {
				work_ = boost::make_shared<boost::asio::io_service::work>(boost::ref(*io_service_));
				thread_group_ = boost::make_shared<boost::thread_group>();
				for (size_type i = 0, last = new_thread_count; i != last; ++i) {
					boost::thread* new_thread = thread_group_->create_thread(
						boost::bind(&boost::asio::io_service::run, io_service_.get())
						);
					thread_list_.push_back(new_thread);
				}
			} else if (thread_list_.size() < new_thread_count) {
				for (size_type i = 0, last = new_thread_count - thread_list_.size(); i != last; ++i) {
					boost::thread* new_thread = thread_group_->create_thread(
						boost::bind(&boost::asio::io_service::run, io_service_.get())
						);
					thread_list_.push_back(new_thread);
				}
			} else if (thread_list_.size() > new_thread_count) {
				thread_list_.clear();
				work_.reset();
				thread_group_->join_all();
				thread_group_ = boost::make_shared<boost::thread_group>();
				for (size_type i = 0, last = new_thread_count; i != last; ++i) {
					boost::thread* new_thread = thread_group_->create_thread(
						boost::bind(&boost::asio::io_service::run, io_service_.get())
						);
					thread_list_.push_back(new_thread);
				}
			}
		}
	public:
		NetworkServiceImpl()
			: io_service_(boost::make_shared<boost::asio::io_service>())
		{}
		void run() {
			scoped_lock_type lock(mutex_);
			if (!thread_list_.size()) {
				run_impl(1);
			}
		}
		void run(size_type new_thread_count) {
			scoped_lock_type lock(mutex_);
			run_impl(new_thread_count);
		}
		void run_with_hardware_concurrency() {
			scoped_lock_type lock(mutex_);
			size_type new_thread_count = boost::thread::hardware_concurrency();
			if (!new_thread_count) {
				new_thread_count = 1;
			}
			if (thread_list_.size() < new_thread_count) {
				run_impl(new_thread_count);
			}
		}
		void run_at_least(size_type new_thread_count) {
			scoped_lock_type lock(mutex_);
			if (thread_list_.size() < new_thread_count) {
				run_impl(new_thread_count);
			}
		}
		void run_at_most(size_type new_thread_count) {
			scoped_lock_type lock(mutex_);
			if (thread_list_.size() > new_thread_count) {
				run_impl(new_thread_count);
			}
		}
		void finish() {
			scoped_lock_type lock(mutex_);
			SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:NetworkService:finish"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("thread_count"), thread_list_.size(), SPRIG_KRKR_LOG_LEVEL_NORMAL);
			if (!thread_list_.size()) {
				return;
			}
			thread_list_.clear();
			work_.reset();
			thread_group_->join_all();
			thread_group_.reset();
		}
		void stop() {
			scoped_lock_type lock(mutex_);
			SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:NetworkService:stop"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("thread_count"), thread_list_.size(), SPRIG_KRKR_LOG_LEVEL_NORMAL);
			if (!thread_list_.size()) {
				return;
			}
			thread_list_.clear();
			work_.reset();
			io_service_->stop();
			thread_group_->join_all();
			thread_group_.reset();
			io_service_->reset();
		}
		boost::shared_ptr<boost::asio::io_service> io_service() const {
			scoped_lock_type lock(mutex_);
			return io_service_;
		}
		bool is_running() const {
			scoped_lock_type lock(mutex_);
			return thread_list_.size() != 0;
		}
		size_type thread_count() const {
			scoped_lock_type lock(mutex_);
			return thread_list_.size();
		}
	};
	//
	// NetworkService
	//
	class NetworkService
		: public boost::serialization::singleton<NetworkService>
	{
	public:
		typedef NetworkServiceImpl::size_type size_type;
	public:
		enum on_kill_flag_type {
			stop_on_kill,
			finish_on_kill,
			do_nothing_on_kill
		};
	private:
		boost::scoped_ptr<NetworkServiceImpl> impl_;
		on_kill_flag_type on_kill_flag_;
	public:
		NetworkService()
			: impl_(new NetworkServiceImpl())
			, on_kill_flag_(stop_on_kill)
		{}
		void kill() {
			switch (on_kill_flag_) {
			case stop_on_kill:
				impl_->stop();
				break;
			case finish_on_kill:
				impl_->finish();
				break;
			}
			impl_.reset();
		}
		void run() {
			impl_->run();
		}
		void run(size_type new_thread_count) {
			impl_->run(new_thread_count);
		}
		void run_with_hardware_concurrency() {
			impl_->run_with_hardware_concurrency();
		}
		void run_at_least(size_type new_thread_count = 1) {
			impl_->run_at_least(new_thread_count);
		}
		void run_at_most(size_type new_thread_count = 1) {
			impl_->run_at_most(new_thread_count);
		}
		void finish() {
			impl_->finish();
		}
		void stop() {
			impl_->stop();
		}
		boost::shared_ptr<boost::asio::io_service> io_service() const {
			return impl_->io_service();
		}
		bool is_running() const {
			return impl_->is_running();
		}
		size_type thread_count() const {
			return impl_->thread_count();
		}
		on_kill_flag_type get_on_kill_flag() const {
			return on_kill_flag_;
		}
		void set_on_kill_flag(on_kill_flag_type on_kill_flag) {
			on_kill_flag_ = on_kill_flag;
		}
	};
	//
	// NetworkUtils
	//
	class NetworkUtils {
	public:
		typedef std::string impl_string_type;
		typedef std::size_t size_type;
		typedef boost::uint_fast32_t flag_type;
		typedef boost::asio::basic_waitable_timer<boost::chrono::steady_clock> timeout_timer_type;
		typedef timeout_timer_type::clock_type clock_type;
		typedef timeout_timer_type::time_point time_point_type;
		typedef timeout_timer_type::duration duration_type;
		typedef duration_type::rep rep_type;
		typedef boost::chrono::nanoseconds nanoseconds_type;
		typedef boost::chrono::microseconds microseconds_type;
		typedef boost::chrono::milliseconds milliseconds_type;
		typedef boost::chrono::seconds seconds_type;
	public:
		static flag_type const pfIPv4 = 0x00000000;
		static flag_type const pfIPv6 = 0x00000001;
		static flag_type const ccfTransferAtLeast = 0x00000000;
		static flag_type const ccfTransferExactly = 0x00000001;
	private:
		static bool downloadHTTPImplHTTP(
			impl_string_type const& host_name,
			impl_string_type const& content_path,
			tjs_char const* storage
			)
		{
			boost::asio::io_service& io_service = *NetworkService::get_const_instance().io_service();
			//
			//	COMMENT: 名前解決
			//
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(host_name, "http");
			boost::system::error_code resolve_error;
			boost::asio::ip::tcp::resolver::iterator endpoint_first = resolver.resolve(query, resolve_error);
			if (resolve_error) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("名前解決に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			//
			//	COMMENT: 接続
			//
			boost::asio::ip::tcp::socket socket(io_service);
			boost::system::error_code connect_error;
			boost::asio::connect(
				asSocket(socket),
				endpoint_first,
				connect_error
				);
			if (connect_error) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("接続に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			//
			//	COMMENT: リクエスト送信
			//
			boost::asio::streambuf writing_streambuf;
			{
				std::ostream writing_ostream(&writing_streambuf);
				writing_ostream
					<< "GET " << content_path << " HTTP/1.0\r\n"
					<< "Host: " << host_name << "\r\n"
					<< "Accept: */*\r\n"
					<< "Connection: close\r\n"
					<< "\r\n"
					;
			}
			boost::system::error_code write_error;
			size_type writing_bytes_transferred = boost::asio::write(
				socket,
				writing_streambuf,
				boost::asio::transfer_all(),
				write_error
				);
			if (write_error) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("リクエスト送信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			//
			//	COMMENT: レスポンスヘッダ受信
			//
			boost::asio::streambuf header_reading_streambuf;
			boost::system::error_code header_read_error;
			size_type header_reading_bytes_transferred = boost::asio::read_until(
				socket,
				header_reading_streambuf,
				"\r\n\r\n",
				header_read_error
				);
			if (header_read_error) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンスヘッダ受信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			std::string string(header_reading_bytes_transferred, '\0');
			{
				std::istream reading_istream(&header_reading_streambuf);
				reading_istream.read(&string[0], header_reading_bytes_transferred);
			}
			std::string http_version = string.substr(
				0,
				string.find(' ')
				);
			if (http_version.find("HTTP/") != 0) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("不正なレスポンスが返されました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			std::string status_code = string.substr(
				http_version.size() + 1,
				3
				);
			if (status_code != "200") {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("失敗のステータスコードが返されました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			//
			//	COMMENT: ファイル受信
			//
			sprig::com_ptr<IStream> out(::TVPCreateIStream(storage, TJS_BS_WRITE));
			if (!out) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			boost::array<char, KTL_STORAGE_BUFFER_SIZE> storage_buffer;
			ULONG io_size = 0;
			size_type remained_size = header_reading_streambuf.size();
			if (remained_size) {
				std::istream reading_istream(&header_reading_streambuf);
				size_type size = 0;
				do {
					size = (std::min)(remained_size, sizeof(storage_buffer.elems));
					reading_istream.read(&storage_buffer[0], size);
					if (FAILED(out->Write(&storage_buffer[0], size, &io_size))) {
						SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ファイル書込に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
						return false;
					}
				} while (remained_size -= size);
			}
			boost::system::error_code read_error;
			do {
				size_type reading_bytes_transferred = boost::asio::read(
					socket,
					boost::asio::buffer(storage_buffer),
					boost::asio::transfer_all(),
					read_error
					);
				if (FAILED(out->Write(&storage_buffer[0], reading_bytes_transferred, &io_size))) {
					SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ファイル書込に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					return false;
				}
			} while (!read_error);
			if (read_error != boost::asio::error::eof) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ファイル受信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			return true;
		}
		static bool downloadHTTPImplHTTPS(
			impl_string_type const& host_name,
			impl_string_type const& content_path,
			tjs_char const* storage
			)
		{
			boost::asio::io_service& io_service = *NetworkService::get_const_instance().io_service();
			//
			//	COMMENT: 名前解決
			//
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(host_name, "https");
			boost::system::error_code resolve_error;
			boost::asio::ip::tcp::resolver::iterator endpoint_first = resolver.resolve(query, resolve_error);
			if (resolve_error) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("名前解決に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			//
			//	COMMENT: 接続
			//
			boost::asio::ssl::context context(io_service, boost::asio::ssl::context::sslv23);
			boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket(io_service, context);
			boost::system::error_code connect_error;
			boost::asio::connect(
				asSocket(socket),
				endpoint_first,
				connect_error
				);
			if (connect_error) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("接続に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			//
			//	COMMENT: ハンドシェイク
			//
			boost::system::error_code handshake_error;
			socket.handshake(boost::asio::ssl::stream_base::client, handshake_error);
			if (handshake_error) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ハンドシェイクに失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			//
			//	COMMENT: リクエスト送信
			//
			boost::asio::streambuf writing_streambuf;
			{
				std::ostream writing_ostream(&writing_streambuf);
				writing_ostream
					<< "GET " << content_path << " HTTP/1.0\r\n"
					<< "Host: " << host_name << "\r\n"
					<< "Accept: */*\r\n"
					<< "Connection: close\r\n"
					<< "\r\n"
					;
			}
			boost::system::error_code write_error;
			size_type writing_bytes_transferred = boost::asio::write(
				socket,
				writing_streambuf,
				boost::asio::transfer_all(),
				write_error
				);
			if (write_error) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("リクエスト送信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			//
			//	COMMENT: レスポンス受信
			//
			boost::asio::streambuf header_reading_streambuf;
			boost::system::error_code header_read_error;
			size_type header_reading_bytes_transferred = boost::asio::read_until(
				socket,
				header_reading_streambuf,
				"\r\n\r\n",
				header_read_error
				);
			if (header_read_error) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンス受信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			std::string string(header_reading_bytes_transferred, '\0');
			{
				std::istream reading_istream(&header_reading_streambuf);
				reading_istream.read(&string[0], header_reading_bytes_transferred);
			}
			std::string http_version = string.substr(
				0,
				string.find(' ')
				);
			if (http_version.find("HTTP/") != 0) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("不正なレスポンスが返されました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			std::string status_code = string.substr(
				http_version.size() + 1,
				3
				);
			if (status_code != "200") {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("失敗のステータスコードが返されました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			//
			//	COMMENT: ファイル受信
			//
			sprig::com_ptr<IStream> out(::TVPCreateIStream(storage, TJS_BS_WRITE));
			if (!out) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			boost::array<char, KTL_STORAGE_BUFFER_SIZE> storage_buffer;
			size_type remained_size = header_reading_streambuf.size();
			ULONG io_size = 0;
			if (remained_size) {
				std::istream reading_istream(&header_reading_streambuf);
				size_type size = 0;
				do {
					size = (std::min)(remained_size, sizeof(storage_buffer.elems));
					reading_istream.read(&storage_buffer[0], size);
					if (FAILED(out->Write(&storage_buffer[0], size, &io_size))) {
						SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ファイル書込に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
						return false;
					}
				} while (remained_size -= size);
			}
			boost::system::error_code read_error;
			do {
				size_type reading_bytes_transferred = boost::asio::read(
					socket,
					boost::asio::buffer(storage_buffer),
					boost::asio::transfer_all(),
					read_error
					);
				if (FAILED(out->Write(&storage_buffer[0], reading_bytes_transferred, &io_size))) {
					SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ファイル書込に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					return false;
				}
			} while (!read_error);
			if (read_error != boost::asio::error::eof) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ファイル受信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			return true;
		}
	public:
		static bool downloadHTTP(tjs_char const* url, tjs_char const* storage) {
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("url"), url, SPRIG_KRKR_LOG_LEVEL_NORMAL);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("storage"), storage, SPRIG_KRKR_LOG_LEVEL_NORMAL);
			//
			//	COMMENT: URL解析
			//
			impl_string_type impl_url = sprig::str_cast<impl_string_type>(url);
			impl_string_type::size_type host_offset = 0;
			impl_string_type service_name;
			if (impl_url.find("http://") == 0) {
				service_name = "http";
				host_offset = 7;
			} else if (impl_url.find("https://") == 0) {
				service_name = "https";
				host_offset = 8;
			}
			if (impl_url.size() <= host_offset) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("不正なURLです"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			impl_string_type::size_type host_size = impl_url.find('/', host_offset);
			if (host_size == impl_string_type::npos || host_size - host_offset == 0) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("不正なURLです"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				return false;
			}
			impl_string_type host_name = impl_url.substr(host_offset, host_size - host_offset);
			impl_string_type content_path = impl_url.substr(host_size);
			return service_name == "https"
				? downloadHTTPImplHTTPS(host_name, content_path, storage)
				: downloadHTTPImplHTTP(host_name, content_path, storage)
				;
			}
		static DWORD getInternetConnetionState() {
			DWORD result = 0x00000000;
			::InternetGetConnectedState(&result, 0);
			return result;
		}
		static bool isInternetConnected() {
			DWORD result = 0x00000000;
			return ::InternetGetConnectedState(&result, 0) != 0;
		}
		template<typename Protocol, typename StreamSocketService>
		static boost::asio::basic_stream_socket<Protocol, StreamSocketService>& asSocket(
			boost::asio::basic_stream_socket<Protocol, StreamSocketService>& socket
			)
		{
			return socket;
		}
		template<typename Protocol, typename StreamSocketService>
		static boost::asio::basic_stream_socket<Protocol, StreamSocketService> const& asSocket(
			boost::asio::basic_stream_socket<Protocol, StreamSocketService> const& socket
			)
		{
			return socket;
		}
		template<typename Stream>
		static typename boost::asio::ssl::stream<Stream>::lowest_layer_type& asSocket(
			boost::asio::ssl::stream<Stream>& socket
			)
		{
			return socket.lowest_layer();
		}
		template<typename Stream>
		static typename boost::asio::ssl::stream<Stream>::lowest_layer_type const& asSocket(
			boost::asio::ssl::stream<Stream> const& socket
			)
		{
			return socket.lowest_layer();
		}
		template<typename T>
		static void moveComponent(T& component, T& component2) {
			component = component2;
			component2.reset();
		}
		template<typename T>
		static void resetComponent(T& component, T& component2) {
			component.reset();
			component2.reset();
		}
		template<typename T>
		static void moveErrorCode(T& component, T& component2) {
			component.error_code() = component2.error_code();
			component2.reset();
		}
	};

	//
	// URLInfo
	//
	class URLInfo {
	public:
		typedef NetworkUtils::impl_string_type impl_string_type;
	private:
		impl_string_type host_name_;
		impl_string_type service_name_;
		impl_string_type content_path_;
	public:
		void reset() {
			host_name_.clear();
			service_name_.clear();
			content_path_.clear();
		}
		impl_string_type& host_name() {
			return host_name_;
		}
		impl_string_type& service_name() {
			return service_name_;
		}
		impl_string_type& content_path() {
			return content_path_;
		}
		impl_string_type const& host_name() const {
			return host_name_;
		}
		impl_string_type const& service_name() const {
			return service_name_;
		}
		impl_string_type const& content_path() const {
			return content_path_;
		}
	};

	//
	// HTTPResponse
	//
	class HTTPResponse {
	public:
		typedef NetworkUtils::impl_string_type impl_string_type;
	private:
		impl_string_type http_version_;
		impl_string_type status_code_;
		impl_string_type reason_phrase_;
	public:
		void reset() {
			http_version_.clear();
			status_code_.clear();
			reason_phrase_.clear();
		}
		impl_string_type& http_version() {
			return http_version_;
		}
		impl_string_type& status_code() {
			return status_code_;
		}
		impl_string_type& reason_phrase() {
			return reason_phrase_;
		}
		impl_string_type const& http_version() const {
			return http_version_;
		}
		impl_string_type const& status_code() const {
			return status_code_;
		}
		impl_string_type const& reason_phrase() const {
			return reason_phrase_;
		}
	};

	//
	// HTTPHeader
	//
	class HTTPHeader {
	public:
		typedef NetworkUtils::impl_string_type impl_string_type;
	public:
		template<typename String>
		struct compare
			: public std::binary_function<String, String, bool>
		{
		public:
			typedef String string_type;
		public:
			bool operator()(string_type const& lhs, string_type const& rhs) const {
				return boost::algorithm::ilexicographical_compare(lhs, rhs);
			}
		};
	public:
		typedef std::map<impl_string_type, impl_string_type, compare<impl_string_type> > impl_map_type;
	private:
		impl_map_type field_map_;
	public:
		void reset() {
			field_map_.clear();
		}
		impl_map_type& field_map() {
			return field_map_;
		}
		impl_map_type const& field_map() const {
			return field_map_;
		}
	};

	//
	// Resolvers
	//
	class Resolvers {
	private:
		boost::shared_ptr<boost::asio::ip::tcp::resolver> resolver_;
		boost::shared_ptr<boost::asio::ip::tcp::resolver::query> query_;
		boost::shared_ptr<boost::asio::ip::tcp::resolver::iterator> iterator_;
		boost::optional<boost::system::error_code> error_code_;
	public:
		void reset() {
			resolver_.reset();
			query_.reset();
			iterator_.reset();
			error_code_.reset();
		}
		bool enable() const {
			return error_code_;
		}
		boost::shared_ptr<boost::asio::ip::tcp::resolver>& resolver() {
			return resolver_;
		}
		boost::shared_ptr<boost::asio::ip::tcp::resolver::query>& query() {
			return query_;
		}
		boost::shared_ptr<boost::asio::ip::tcp::resolver::iterator>& iterator() {
			return iterator_;
		}
		boost::optional<boost::system::error_code>& error_code() {
			return error_code_;
		}
		boost::shared_ptr<boost::asio::ip::tcp::resolver> const& resolver() const {
			return resolver_;
		}
		boost::shared_ptr<boost::asio::ip::tcp::resolver::query> const& query() const {
			return query_;
		}
		boost::shared_ptr<boost::asio::ip::tcp::resolver::iterator> const& iterator() const {
			return iterator_;
		}
		boost::optional<boost::system::error_code> const& error_code() const {
			return error_code_;
		}
	};
	//
	// Connectors
	//
	class Connectors {
	private:
		boost::shared_ptr<boost::asio::ssl::context> context_;
		boost::optional<boost::system::error_code> error_code_;
	public:
		void reset() {
			context_.reset();
			error_code_.reset();
		}
		bool enable() const {
			return error_code_;
		}
		boost::shared_ptr<boost::asio::ssl::context>& context() {
			return context_;
		}
		boost::optional<boost::system::error_code>& error_code() {
			return error_code_;
		}
		boost::shared_ptr<boost::asio::ssl::context> const& context() const {
			return context_;
		}
		boost::optional<boost::system::error_code> const& error_code() const {
			return error_code_;
		}
	};
	//
	// Acceptors
	//
	class Acceptors {
	private:
		boost::shared_ptr<boost::asio::ssl::context> context_;
		boost::shared_ptr<boost::asio::ip::tcp::endpoint> endpoint_;
		boost::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
		boost::optional<boost::system::error_code> error_code_;
	public:
		void reset() {
			context_.reset();
			acceptor_.reset();
			error_code_.reset();
		}
		bool enable() const {
			return error_code_;
		}
		boost::shared_ptr<boost::asio::ssl::context>& context() {
			return context_;
		}
		boost::shared_ptr<boost::asio::ip::tcp::endpoint>& endpoint() {
			return endpoint_;
		}
		boost::shared_ptr<boost::asio::ip::tcp::acceptor>& acceptor() {
			return acceptor_;
		}
		boost::optional<boost::system::error_code>& error_code() {
			return error_code_;
		}
		boost::shared_ptr<boost::asio::ssl::context> const& context() const {
			return context_;
		}
		boost::shared_ptr<boost::asio::ip::tcp::endpoint> const& endpoint() const {
			return endpoint_;
		}
		boost::shared_ptr<boost::asio::ip::tcp::acceptor> const& acceptor() const {
			return acceptor_;
		}
		boost::optional<boost::system::error_code> const& error_code() const {
			return error_code_;
		}
	};
	//
	// Handshakers
	//
	class Handshakers {
	private:
		boost::optional<boost::system::error_code> error_code_;
	public:
		void reset() {
			error_code_.reset();
		}
		bool enable() const {
			return error_code_;
		}
		boost::optional<boost::system::error_code>& error_code() {
			return error_code_;
		}
		boost::optional<boost::system::error_code> const& error_code() const {
			return error_code_;
		}
	};
	//
	// Writers
	//
	class Writers {
	private:
		boost::optional<boost::system::error_code> error_code_;
	public:
		void reset() {
			error_code_.reset();
		}
		bool enable() const {
			return error_code_;
		}
		boost::optional<boost::system::error_code>& error_code() {
			return error_code_;
		}
		boost::optional<boost::system::error_code> const& error_code() const {
			return error_code_;
		}
	};
	//
	// Readers
	//
	class Readers {
	private:
		boost::optional<boost::system::error_code> error_code_;
	public:
		void reset() {
			error_code_.reset();
		}
		bool enable() const {
			return error_code_;
		}
		boost::optional<boost::system::error_code>& error_code() {
			return error_code_;
		}
		boost::optional<boost::system::error_code> const& error_code() const {
			return error_code_;
		}
	};
	//
	// TimeoutTimers
	//
	class TimeoutTimers {
	private:
		boost::optional<boost::system::error_code> error_code_;
	public:
		void reset() {
			error_code_.reset();
		}
		bool enable() const {
			return error_code_;
		}
		boost::optional<boost::system::error_code>& error_code() {
			return error_code_;
		}
		boost::optional<boost::system::error_code> const& error_code() const {
			return error_code_;
		}
	};
}	// namespace ktl

#endif	// #ifndef SRC_NETWORK_NETWORK_COMMON_HPP
