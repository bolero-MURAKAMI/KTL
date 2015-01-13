/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_NETWORK_DOWNLOADER_HPP
#define SRC_NETWORK_DOWNLOADER_HPP

#include <cstddef>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <fstream>
#include <boost/ref.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/fusion/include/vector_tie.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/cxx11/copy_n.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/utility/swap.hpp>
#include <sprig/config/lib/openssl.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/str_length.hpp>
#include <sprig/parser/http_header.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>
#include <ktl/thread_callback.hpp>

#include "NetworkCommon.hpp"
#include "DownloaderDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Downloader:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Downloader:error")

namespace ktl {
	//
	// NativeDownloader
	//
	KTL_INLINE NativeDownloader::size_type NativeDownloader::getEnableSize(size_type length, size_type spos) {
		return spos <= length
			? length - spos
			: 0
			;
	}
	KTL_INLINE NativeDownloader::size_type NativeDownloader::getEnableSize(size_type length, size_type spos, size_type ssize) {
		return spos + ssize <= length
			? ssize
			: spos <= length
				? length - spos
				: 0
			;
	}
	KTL_INLINE char const* NativeDownloader::bufferData(boost::asio::streambuf const& buffer) {
		return boost::asio::buffer_cast<char const*>(buffer.data());
	}
	KTL_INLINE char const* NativeDownloader::bufferEnd(boost::asio::streambuf const& buffer) {
		return bufferData(buffer) + buffer.size();
	}
	KTL_INLINE boost::iterator_range<char const*> NativeDownloader::bufferRange(boost::asio::streambuf const& buffer) {
		return boost::iterator_range<char const*>(bufferData(buffer), bufferEnd(buffer));
	}
	void NativeDownloader::callOnFinished() {
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
	KTL_INLINE void NativeDownloader::postOnFinished() {
		if (on_finished_) {
			ktl::thread_callback::post(
				boost::bind(&NativeDownloader::callOnFinished, this)
				);
		}
	}
	void NativeDownloader::handleResolove(
		boost::system::error_code const& error,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			resolvers2_.error_code() = error;
			if (*resolvers2_.error_code()) {
				NetworkUtils::moveErrorCode(resolvers_, resolvers2_);
				if (*resolvers_.error_code() != boost::asio::error::operation_aborted) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("名前解決に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *resolvers_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), resolvers_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("名前解決がキャンセルされました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				cleanupOnProcessFailed();
				return;
			}
			resolvers2_.iterator() = boost::make_shared<boost::asio::ip::tcp::resolver::iterator>(endpoint_iterator);
			NetworkUtils::moveComponent(resolvers_, resolvers2_);
			//
			// 接続開始
			//
			connectors_.error_code().reset();
			connectors2_.error_code() = boost::system::error_code();
			if (url_info_.service_name() == "https") {
				connectors2_.context() = boost::make_shared<boost::asio::ssl::context>(
					boost::ref(*io_service_),
					boost::asio::ssl::context::sslv23
					);
				ssl_socket2_ = boost::make_shared<boost::asio::ssl::stream<boost::asio::ip::tcp::socket> >(
					boost::ref(*io_service_),
					boost::ref(*connectors2_.context())
					);
				boost::asio::async_connect(
					NetworkUtils::asSocket(*ssl_socket2_),
					*resolvers_.iterator(),
					strand_->wrap(
						boost::bind(
							&NativeDownloader::handleConnectSSL,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::iterator
							)
						)
					);
			} else {
				socket2_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(*io_service_));
				boost::asio::async_connect(
					NetworkUtils::asSocket(*socket2_),
					*resolvers_.iterator(),
					strand_->wrap(
						boost::bind(
							&NativeDownloader::handleConnect,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::iterator
							)
						)
					);
			}
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeDownloader::handleConnect(
		boost::system::error_code const& error,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			connectors2_.error_code() = error;
			if (*connectors2_.error_code()) {
				NetworkUtils::moveErrorCode(connectors_, connectors2_);
				if (*connectors_.error_code() != boost::asio::error::operation_aborted) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("接続に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *connectors_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), connectors_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("接続がキャンセルされました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				cleanupOnProcessFailed();
				return;
			}
			NetworkUtils::moveComponent(connectors_, connectors2_);
			enableSocket();
			//
			// 送信開始
			//
			putHTTPRequest(url_info_.host_name(), url_info_.content_path());
			writers_.error_code().reset();
			writers2_.error_code() = boost::system::error_code();
			boost::asio::async_write(
				*socket_,
				*writing_streambuf_,
				boost::asio::transfer_all(),
				strand_->wrap(
					boost::bind(
						&NativeDownloader::handleWrite,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeDownloader::handleConnectSSL(
		boost::system::error_code const& error,
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			connectors2_.error_code() = error;
			if (*connectors2_.error_code()) {
				NetworkUtils::moveErrorCode(connectors_, connectors2_);
				if (*connectors_.error_code() != boost::asio::error::operation_aborted) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("接続に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *connectors_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), connectors_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("接続がキャンセルされました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				cleanupOnProcessFailed();
				return;
			}
			NetworkUtils::moveComponent(connectors_, connectors2_);
			enableSocketSSL();
			//
			// ハンドシェイク開始
			//
			handshakers_.error_code().reset();
			handshakers2_.error_code() = boost::system::error_code();
			ssl_socket_->async_handshake(
				boost::asio::ssl::stream_base::client,
				strand_->wrap(
					boost::bind(
						&NativeDownloader::handleHandshake,
						this,
						boost::asio::placeholders::error
						)
					)
				);
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeDownloader::handleHandshake(
		boost::system::error_code const& error
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			handshakers2_.error_code() = error;
			if (*handshakers2_.error_code()) {
				NetworkUtils::moveErrorCode(handshakers_, handshakers2_);
				if (*handshakers_.error_code() != boost::asio::error::operation_aborted) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ハンドシェイクに失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *handshakers_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), handshakers_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ハンドシェイクがキャンセルされました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				cleanupOnProcessFailed();
				return;
			}
			NetworkUtils::moveComponent(handshakers_, handshakers2_);
			//
			// 送信開始
			//
			putHTTPRequest(url_info_.host_name(), url_info_.content_path());
			writers_.error_code().reset();
			writers2_.error_code() = boost::system::error_code();
			boost::asio::async_write(
				*ssl_socket_,
				*writing_streambuf_,
				boost::asio::transfer_all(),
				strand_->wrap(
					boost::bind(
						&NativeDownloader::handleWrite,
						this,
						boost::asio::placeholders::error,
						boost::asio::placeholders::bytes_transferred
						)
					)
				);
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeDownloader::handleWrite(
		boost::system::error_code const& error,
		size_type bytes_transferred
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			writers2_.error_code() = error;
			if (*writers2_.error_code()) {
				NetworkUtils::moveErrorCode(writers_, writers2_);
				if (*writers_.error_code() != boost::asio::error::operation_aborted) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("リクエスト送信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *writers_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), writers_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("リクエスト送信がキャンセルされました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				cleanupOnProcessFailed();
				return;
			}
			NetworkUtils::moveComponent(writers_, writers2_);
			//
			// 受信開始
			//
			readers_.error_code().reset();
			readers2_.error_code() = boost::system::error_code();
			if (socket_) {
				boost::asio::async_read_until(
					*socket_,
					*reading_streambuf_,
					"\r\n\r\n",
					strand_->wrap(
						boost::bind(
							&NativeDownloader::handleReadUntil,
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
					"\r\n\r\n",
					strand_->wrap(
						boost::bind(
							&NativeDownloader::handleReadUntil,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred
							)
						)
					);
			}
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeDownloader::handleReadUntil(
		boost::system::error_code const& error,
		size_type bytes_transferred
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			readers2_.error_code() = error;
			if (*readers2_.error_code()) {
				NetworkUtils::moveErrorCode(readers_, readers2_);
				if (*readers_.error_code() != boost::asio::error::operation_aborted) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンス受信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *readers_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), readers_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンス受信がキャンセルされました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				cleanupOnProcessFailed();
				return;
			}
			NetworkUtils::moveComponent(readers_, readers2_);
			if (!analyHTTPResponse(true)) {
				KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンス解析でエラーが発生しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				cleanupOnProcessFailed();
				return;
			}
			if (http_response_.status_code() == "204") {
				KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("処理が完了しました/サーバから送信されたコンテンツはありません"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
				cleanupOnProcessSucceeded();
				return;
			}
			if (storage_ && storage_->empty()) {
				KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("処理が完了しました/サーバから送信されたコンテンツを保存しません"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
				cleanupOnProcessSucceeded();
				return;
			}
			if (!openBuffer(true)) {
				cleanupOnProcessFailed();
				return;
			}
			if (chunked_) {
				// 先読みされた分を処理
				boost::tuple<bool, bool, size_type, impl_string_type> result(processChunked());
				if (!boost::get<0>(result)) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("チャンクデータの処理でエラーが発生しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					cleanupOnProcessFailed();
					return;
				}
				if (boost::get<1>(result)) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ダウンロード完了しました"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
					cleanupOnProcessSucceeded();
					return;
				}
				//
				// ロード開始
				//
				readers_.error_code().reset();
				readers2_.error_code() = boost::system::error_code();
				if (socket_) {
					boost::asio::async_read(
						*socket_,
						*reading_streambuf_,
						boost::asio::transfer_at_least(1),
						strand_->wrap(
							boost::bind(
								&NativeDownloader::handleLoadChunked,
								this,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred,
								boost::get<2>(result),
								boost::get<3>(result)
								)
							)
						);
				} else if (ssl_socket_) {
					boost::asio::async_read(
						*ssl_socket_,
						*reading_streambuf_,
						boost::asio::transfer_at_least(1),
						strand_->wrap(
							boost::bind(
								&NativeDownloader::handleLoadChunked,
								this,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred,
								boost::get<2>(result),
								boost::get<3>(result)
								)
							)
						);
				}
			} else {
				// 先読みされた分を処理
				{
					transferred_size_ = (content_length_ && reading_streambuf_->size() > content_length_)
						? content_length_
						: reading_streambuf_->size()
						;
					if (!updateBuffer(transferred_size_)) {
						cleanupOnProcessFailed();
						return;
					}
					if (content_length_ && transferred_size_ >= content_length_) {
						KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ダウンロード完了しました"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
						cleanupOnProcessSucceeded();
						return;
					}
				}
				//
				// ロード開始
				//
				readers_.error_code().reset();
				readers2_.error_code() = boost::system::error_code();
				if (socket_) {
					boost::asio::async_read(
						*socket_,
						*reading_streambuf_,
						boost::asio::transfer_at_least(1),
						strand_->wrap(
							boost::bind(
								&NativeDownloader::handleLoadBlock,
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
						boost::asio::transfer_at_least(1),
						strand_->wrap(
							boost::bind(
								&NativeDownloader::handleLoadBlock,
								this,
								boost::asio::placeholders::error,
								boost::asio::placeholders::bytes_transferred
								)
							)
						);
				}
			}
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeDownloader::handleLoadBlock(
		boost::system::error_code const& error,
		size_type bytes_transferred
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			readers2_.error_code() = error;
			if (*readers2_.error_code() && *readers2_.error_code() != boost::asio::error::eof) {
				NetworkUtils::moveErrorCode(readers_, readers2_);
				if (*readers_.error_code() != boost::asio::error::operation_aborted) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("データ受信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *readers_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), readers_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("データ受信がキャンセルされました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				cleanupOnProcessFailed();
				return;
			}
			NetworkUtils::moveComponent(readers_, readers2_);
			size_type real_bytes_transferred = (content_length_ && transferred_size_ + bytes_transferred > content_length_)
				? content_length_ - transferred_size_
				: bytes_transferred
				;
			transferred_size_ += real_bytes_transferred;
			if (!updateBuffer(real_bytes_transferred)) {
				cleanupOnProcessFailed();
				return;
			}
			if (content_length_ && transferred_size_ >= content_length_) {
				KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ダウンロード完了しました"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
				cleanupOnProcessSucceeded();
				return;
			} else if (error == boost::asio::error::eof) {
				if (content_length_) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("受信データ長が指定データ長に足りません"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					cleanupOnProcessFailed();
					return;
				}
				KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ダウンロード完了しました"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
				cleanupOnProcessSucceeded();
				return;
			}
			readers_.error_code().reset();
			readers2_.error_code() = boost::system::error_code();
			if (socket_) {
				boost::asio::async_read(
					*socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(1),
					strand_->wrap(
						boost::bind(
							&NativeDownloader::handleLoadBlock,
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
					boost::asio::transfer_at_least(1),
					strand_->wrap(
						boost::bind(
							&NativeDownloader::handleLoadBlock,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred
							)
						)
					);
			}
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	KTL_INLINE void NativeDownloader::handleLoadChunked(
		boost::system::error_code const& error,
		size_type bytes_transferred,
		size_type chunk_size,
		impl_string_type const& chunk_size_block
		)
	{
		scoped_lock_type lock(mutex_);
		SPRIG_KRKR_TRY() {
			readers2_.error_code() = error;
			if (*readers2_.error_code() && *readers2_.error_code() != boost::asio::error::eof) {
				NetworkUtils::moveErrorCode(readers_, readers2_);
				if (*readers_.error_code() != boost::asio::error::operation_aborted) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("データ受信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *readers_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), readers_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("データ受信がキャンセルされました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				cleanupOnProcessFailed();
				return;
			}
			NetworkUtils::moveComponent(readers_, readers2_);
			boost::tuple<bool, bool, size_type, impl_string_type> result(processChunked(chunk_size, chunk_size_block));
			if (!boost::get<0>(result)) {
				KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("チャンクデータの処理でエラーが発生しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				cleanupOnProcessFailed();
				return;
			}
			if (boost::get<1>(result) || error == boost::asio::error::eof) {
				KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ダウンロード完了しました"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
				cleanupOnProcessSucceeded();
				return;
			}
			readers_.error_code().reset();
			readers2_.error_code() = boost::system::error_code();
			if (socket_) {
				boost::asio::async_read(
					*socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(1),
					strand_->wrap(
						boost::bind(
							&NativeDownloader::handleLoadChunked,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred,
							boost::get<2>(result),
							boost::get<3>(result)
							)
						)
					);
			} else if (ssl_socket_) {
				boost::asio::async_read(
					*ssl_socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(1),
					strand_->wrap(
						boost::bind(
							&NativeDownloader::handleLoadChunked,
							this,
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred,
							boost::get<2>(result),
							boost::get<3>(result)
							)
						)
					);
			}
		} SPRIG_KRKR_CATCH(e) {
			KTL_THREAD_CALLBACK_POST_ERROR(KTL_ERROR_SECTION, SPRIG_KRKR_TJS_W("非同期処理内で例外が投げられました"));
			KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("exception_information"), sprig::krkr::tjs::exception_information(e), SPRIG_KRKR_LOG_LEVEL_ERROR);
			cleanupOnProcessFailed();
			return;
		}
	}
	void NativeDownloader::handleTimeout(
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
	KTL_INLINE bool NativeDownloader::resolve(impl_string_type const& host_name, impl_string_type const& service_name) {
		if (resolvers2_.enable()) {
			return false;
		}
		resolvers_.error_code().reset();
		resolvers2_.resolver() = boost::make_shared<boost::asio::ip::tcp::resolver>(boost::ref(*io_service_));
		resolvers2_.query() = boost::make_shared<boost::asio::ip::tcp::resolver::query>(host_name, service_name);
		resolvers2_.error_code() = boost::system::error_code();
		resolvers2_.iterator() = boost::make_shared<boost::asio::ip::tcp::resolver::iterator>(
			resolvers2_.resolver()->resolve(*resolvers2_.query(), *resolvers2_.error_code())
			);
		if (*resolvers2_.error_code()) {
			NetworkUtils::moveErrorCode(resolvers_, resolvers2_);
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("名前解決に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *resolvers_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), resolvers_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			return false;
		}
		NetworkUtils::moveComponent(resolvers_, resolvers2_);
		return true;
	}
	KTL_INLINE bool NativeDownloader::connect() {
		if (connectors2_.enable()) {
			return false;
		}
		if (!resolvers_.error_code() || *resolvers_.error_code()) {
			return false;
		}
		connectors_.error_code().reset();
		connectors2_.error_code() = boost::system::error_code();
		socket2_ = boost::make_shared<boost::asio::ip::tcp::socket>(boost::ref(*io_service_));
		boost::asio::connect(
			NetworkUtils::asSocket(*socket2_),
			*resolvers_.iterator(),
			*connectors2_.error_code()
			);
		if (*connectors2_.error_code()) {
			NetworkUtils::moveErrorCode(connectors_, connectors2_);
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("接続に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *connectors_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), connectors_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			return false;
		}
		NetworkUtils::moveComponent(connectors_, connectors2_);
		enableSocket();
		return true;
	}
	KTL_INLINE bool NativeDownloader::connectSSL() {
		if (connectors2_.enable()) {
			return false;
		}
		if (!resolvers_.error_code() || *resolvers_.error_code()) {
			return false;
		}
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
		boost::asio::connect(
			NetworkUtils::asSocket(*ssl_socket2_),
			*resolvers_.iterator(),
			*connectors2_.error_code()
			);
		if (*connectors2_.error_code()) {
			NetworkUtils::moveErrorCode(connectors_, connectors2_);
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("接続に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *connectors_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), connectors_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			return false;
		}
		NetworkUtils::moveComponent(connectors_, connectors2_);
		enableSocketSSL();
		return true;
	}
	KTL_INLINE bool NativeDownloader::handshake() {
		if (handshakers2_.enable()) {
			return false;
		}
		if (!ssl_socket_) {
			return false;
		}
		handshakers_.error_code().reset();
		handshakers2_.error_code() = boost::system::error_code();
		ssl_socket_->handshake(
			boost::asio::ssl::stream_base::client,
			*handshakers2_.error_code()
			);
		if (*handshakers2_.error_code()) {
			NetworkUtils::moveErrorCode(handshakers_, handshakers2_);
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ハンドシェイクに失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *handshakers_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), handshakers_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			return false;
		}
		NetworkUtils::moveComponent(handshakers_, handshakers2_);
		return true;
	}
	KTL_INLINE NativeDownloader::int_type NativeDownloader::write() {
		if (writers2_.enable()) {
			return -1;
		}
		if (!socket_ && !ssl_socket_) {
			return -1;
		}
		writers_.error_code().reset();
		writers2_.error_code() = boost::system::error_code();
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
		if (*writers2_.error_code()) {
			NetworkUtils::moveErrorCode(writers_, writers2_);
			return bytes_transferred;
		}
		NetworkUtils::moveComponent(writers_, writers2_);
		return bytes_transferred;
	}
	KTL_INLINE NativeDownloader::int_type NativeDownloader::read() {
		if (readers2_.enable()) {
			return -1;
		}
		if (!socket_ && !ssl_socket_) {
			return -1;
		}
		readers_.error_code().reset();
		readers2_.error_code() = boost::system::error_code();
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
		if (*readers2_.error_code()) {
			NetworkUtils::moveErrorCode(readers_, readers2_);
			return bytes_transferred;
		}
		NetworkUtils::moveComponent(readers_, readers2_);
		return bytes_transferred;
	}
	KTL_INLINE NativeDownloader::int_type NativeDownloader::read(size_type size) {
		if (readers2_.enable()) {
			return -1;
		}
		if (!socket_ && !ssl_socket_) {
			return -1;
		}
		readers_.error_code().reset();
		readers2_.error_code() = boost::system::error_code();
		size_type bytes_transferred = 0;
		if (socket_) {
			bytes_transferred = boost::asio::read(
				*socket_,
				*reading_streambuf_,
				boost::asio::transfer_at_least(size),
				*readers2_.error_code()
				);
		} else if (ssl_socket_) {
			bytes_transferred = boost::asio::read(
				*ssl_socket_,
				*reading_streambuf_,
				boost::asio::transfer_at_least(size),
				*readers2_.error_code()
				);
		}
		if (*readers2_.error_code()) {
			NetworkUtils::moveErrorCode(readers_, readers2_);
			return bytes_transferred;
		}
		NetworkUtils::moveComponent(readers_, readers2_);
		return bytes_transferred;
	}
	KTL_INLINE NativeDownloader::int_type NativeDownloader::readUntil(impl_string_type const& delim) {
		if (readers2_.enable()) {
			return -1;
		}
		if (!socket_ && !ssl_socket_) {
			return -1;
		}
		readers_.error_code().reset();
		readers2_.error_code() = boost::system::error_code();
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
		if (*readers2_.error_code()) {
			NetworkUtils::moveErrorCode(readers_, readers2_);
			return bytes_transferred;
		}
		NetworkUtils::moveComponent(readers_, readers2_);
		return bytes_transferred;
	}
	KTL_INLINE bool NativeDownloader::loadBlock() {
		boost::system::error_code error;
		size_type bytes_transferred = 0;
		for (; ; ) {
			if (socket_) {
				bytes_transferred = boost::asio::read(
					*socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(1),
					error
					);
			} else if (ssl_socket_) {
				bytes_transferred = boost::asio::read(
					*ssl_socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(1),
					error
					);
			}
			if (error && error != boost::asio::error::eof) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("データ受信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), error, SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), error.message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				cleanupOnProcessFailed();
				return false;
			}
			size_type real_bytes_transferred = (content_length_ && transferred_size_ + bytes_transferred > content_length_)
				? content_length_ - transferred_size_
				: bytes_transferred
				;
			transferred_size_ += real_bytes_transferred;
			if (!updateBuffer(real_bytes_transferred)) {
				cleanupOnProcessFailed();
				return false;
			}
			if (content_length_ && transferred_size_ >= content_length_) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ダウンロード完了しました"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
				cleanupOnProcessSucceeded();
				return true;
			} else if (error == boost::asio::error::eof) {
				if (content_length_) {
					SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("受信データ長が指定データ長に足りません"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					cleanupOnProcessFailed();
					return false;
				}
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ダウンロード完了しました"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
				cleanupOnProcessSucceeded();
				return true;
			}
		}
	}
	KTL_INLINE bool NativeDownloader::loadChunked(size_type chunk_size, impl_string_type chunk_size_block) {
		boost::system::error_code error;
		size_type bytes_transferred = 0;
		for (; ; ) {
			if (socket_) {
				bytes_transferred = boost::asio::read(
					*socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(1),
					error
					);
			} else if (ssl_socket_) {
				bytes_transferred = boost::asio::read(
					*ssl_socket_,
					*reading_streambuf_,
					boost::asio::transfer_at_least(1),
					error
					);
			}
			if (error && error != boost::asio::error::eof) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("データ受信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), error, SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), error.message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				cleanupOnProcessFailed();
				return false;
			}
			boost::tuple<bool, bool, size_type, impl_string_type> result(processChunked(chunk_size, chunk_size_block));
			if (!boost::get<0>(result)) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("チャンクデータの処理でエラーが発生しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				cleanupOnProcessFailed();
				return false;
			}
			if (boost::get<1>(result) || error == boost::asio::error::eof) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ダウンロード完了しました"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
				cleanupOnProcessSucceeded();
				return true;
			}
			chunk_size = boost::get<2>(result);
			chunk_size_block = boost::get<3>(result);
		}
	}
	KTL_INLINE bool NativeDownloader::analyURL(impl_string_type const& url) {
		impl_string_type::size_type host_offset = 0;
		if (url.find("http://") == 0) {
			url_info2_.service_name() = "http";
			host_offset = 7;
		} else if (url.find("https://") == 0) {
			url_info2_.service_name() = "https";
			host_offset = 8;
		}
		if (url.size() <= host_offset) {
			url_info2_.reset();
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("不正なURLです"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			return false;
		}
		impl_string_type::size_type host_size = url.find('/', host_offset);
		if (host_size == impl_string_type::npos || host_size - host_offset == 0) {
			url_info2_.reset();
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("不正なURLです"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			return false;
		}
		url_info2_.host_name() = url.substr(host_offset, host_size - host_offset);
		url_info2_.content_path() = url.substr(host_size);
		NetworkUtils::moveComponent(url_info_, url_info2_);
		return true;
	}
	KTL_INLINE bool NativeDownloader::setStorageName(tjs_char const* storage, bool to_buffer) {
		if (!to_buffer) {
			if (sprig::str_length(storage)) {
				tTJSString localized = ::TVPNormalizeStorageName(storage);
				::TVPGetLocalName(localized);
				if (localized.IsEmpty()) {
					SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ローカルファイル名の取得に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					return false;
				}
				storage_ = boost::make_shared<std::string>(sprig::str_cast<std::string>(localized));
			} else {
				storage_ = boost::make_shared<std::string>();
			}
		}
		return true;
	}
	KTL_INLINE void NativeDownloader::putHTTPRequest(impl_string_type const& host_name, impl_string_type const& content_path) {
		if (upload_buffer_) {
			{
				std::ostream writing_ostream(writing_streambuf_.get());
				writing_ostream
					<< "POST " << content_path << " HTTP/1.0\r\n"
					<< "Host: " << host_name << "\r\n"
					<< "Accept: */*\r\n"
					<< "Connection: close\r\n"
					<< "Content-Length: " << upload_buffer_->size() << "\r\n"
					;
				//  HACK: カスタムヘッダを送信
				if (custom_header_) {
					writing_ostream << *custom_header_;
					//SPRIG_KRKR_OUTPUT_COMMENT(*custom_header_, SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				}
				writing_ostream
					<< "\r\n"
					;
			}
			std::copy(
				upload_buffer_->begin(),
				upload_buffer_->end(),
				std::ostreambuf_iterator<char>(writing_streambuf_.get())
				);
		} else {
			std::ostream writing_ostream(writing_streambuf_.get());
			writing_ostream
				<< "GET " << content_path << " HTTP/1.0\r\n"
				<< "Host: " << host_name << "\r\n"
				<< "Accept: */*\r\n"
				<< "Connection: close\r\n"
				;
			//	HACK: カスタムヘッダを送信
			if (custom_header_) {
				writing_ostream << *custom_header_;
			}
			writing_ostream
				<< "\r\n"
				;
		}
	}
	KTL_INLINE bool NativeDownloader::analyHTTPResponse(bool async) {
		//	取得
		impl_string_type response_and_header;
		{
			boost::iterator_range<char const*> buffer_range = bufferRange(*reading_streambuf_);
			boost::iterator_range<char const*> delim_found = boost::algorithm::find_first(
				buffer_range,
				"\r\n\r\n"
				);
			if (!delim_found) {
				if (async) {
					SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンスを取得できません"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンスを取得できません"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				return false;
			}
			response_and_header.resize(boost::end(delim_found) - boost::begin(buffer_range));
			{
				std::istream reading_istream(reading_streambuf_.get());
				reading_istream.read(&response_and_header[0], response_and_header.size());
			}
		}
		impl_string_type::const_iterator first = response_and_header.begin();
		impl_string_type::const_iterator last = response_and_header.end();
		//	解析
		{
			//	レスポンス行解析
			sprig::parser::http_response_line<
				impl_string_type::const_iterator,
				boost::fusion::result_of::vector_tie<
					impl_string_type,
					impl_string_type,
					impl_string_type
				>::type
			> http_response_line_parser;
			if (!boost::spirit::qi::parse(
				first,
				last,
				http_response_line_parser,
				boost::fusion::vector_tie(
					http_response2_.http_version(),
					http_response2_.status_code(),
					http_response2_.reason_phrase()
					)
				))
			{
				http_response2_.reset();
				if (async) {
					SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンス解析に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンス解析に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				return false;
			}
			NetworkUtils::moveComponent(http_response_, http_response2_);
		}
		if (process_status_code_error_ && http_response_.status_code() != "200") {
			if (!upload_buffer_ || http_response_.status_code() != "204") {
				if (async) {
					SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("失敗のステータスコードが返されました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("status_code"), http_response_.status_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("失敗のステータスコードが返されました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					KTL_THREAD_CALLBACK_POST_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("status_code"), http_response_.status_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				}
				return false;
			}
		}
		//	ヘッダ解析
		{
			sprig::parser::http_header<
				impl_string_type::const_iterator,
				HTTPHeader::impl_map_type
			> http_header_parser;
			if (!boost::spirit::qi::parse(
				first,
				last,
				http_header_parser,
				http_header2_.field_map()
				))
			{
				http_header2_.reset();
				if (async) {
					SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ヘッダ解析に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				} else {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ヘッダ解析に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				return false;
			}
			NetworkUtils::moveComponent(http_header_, http_header2_);
		}
		//	ヘッダ情報の反映
		{
			HTTPHeader::impl_map_type::const_iterator not_found = http_header_.field_map().end();
			HTTPHeader::impl_map_type::const_iterator found = not_found;
			if ((found = http_header_.field_map().find("Transfer-Encoding")) != not_found) {
				if (boost::algorithm::iequals(found->second, "chunked")) {
					chunked_ = true;
				}
			} else if ((found = http_header_.field_map().find("Content-Length")) != not_found) {
				content_length_ = std::strtol(found->second.c_str(), 0, 10);
				if (!content_length_) {
					if (async) {
						SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンスのContent-Lengthが不正です"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					} else {
						KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンスのContent-Lengthが不正です"), SPRIG_KRKR_LOG_LEVEL_WARNING);
					}
					return false;
				}
			}
		}
		return true;
	}
	KTL_INLINE bool NativeDownloader::openBuffer(bool async) {
		if (storage_) {
			storage_out_ = boost::make_shared<std::ofstream>(
				storage_->c_str(),
				std::ios_base::out | std::ios_base::binary
				);
			if (!*storage_out_) {
				if (async) {
					KTL_THREAD_CALLBACK_POST_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				} else {
					SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				}
				cleanupOnProcessFailed();
				return false;
			}
		} else {
			buffer_ = boost::make_shared<buffer_type>();
		}
		return true;
	}
	KTL_INLINE bool NativeDownloader::updateBuffer(size_type bytes_transferred) {
		if (storage_) {
			boost::algorithm::copy_n(
				std::istreambuf_iterator<char>(reading_streambuf_.get()),
				bytes_transferred,
				std::ostreambuf_iterator<char>(*storage_out_)
				);
		} else {
			boost::algorithm::copy_n(
				std::istreambuf_iterator<char>(reading_streambuf_.get()),
				bytes_transferred,
				std::back_inserter(*buffer_)
				);
		}
		return true;
	}
	KTL_INLINE boost::tuple<bool, bool, NativeDownloader::size_type, NativeDownloader::impl_string_type> NativeDownloader::processChunked(
		size_type chunk_size,
		impl_string_type chunk_size_block
		)
	{
		while (reading_streambuf_->size()) {
			if (chunk_size > 2) {
				size_type readable_size = std::min(chunk_size - 2, reading_streambuf_->size());
				transferred_size_ += readable_size;
				if (!updateBuffer(readable_size)) {
					return boost::tuple<bool, bool, size_type, impl_string_type>(false, false, 0);
				}
				chunk_size -= readable_size;
			} else if (chunk_size) {
				size_type readable_size = std::min(chunk_size, reading_streambuf_->size());
				impl_string_type cr_lf(readable_size, '\0');
				{
					std::istream reading_istream(reading_streambuf_.get());
					reading_istream.read(&cr_lf[0], readable_size);
				}
			} else {
				{
					impl_string_type chunk_size_piece;
					std::istream reading_istream(reading_streambuf_.get());
					std::getline(reading_istream, chunk_size_piece);
					if (chunk_size_block.empty()) {
						chunk_size_block.swap(chunk_size_piece);
					} else {
						chunk_size_block += chunk_size_piece;
					}
				}
				if (chunk_size_block.empty() || chunk_size_block[chunk_size_block.size() - 1] != '\r') {
					return boost::tuple<bool, bool, size_type, impl_string_type>(true, false, 0, chunk_size_block);
				}
				{
					impl_string_type::size_type ext_pos = chunk_size_block.find(';');
					if (ext_pos != impl_string_type::npos) {
						chunk_size_block.erase(ext_pos);
					} else {
						chunk_size_block.erase(chunk_size_block.size() - 1);
					}
				}
				chunk_size = std::strtol(chunk_size_block.c_str(), 0, 16);
				chunk_size_block.clear();
				if (!chunk_size) {
					return boost::tuple<bool, bool, size_type, impl_string_type>(true, true, 0);
				}
				chunk_size += 2;
			}
		}
		return boost::tuple<bool, bool, size_type, impl_string_type>(true, false, chunk_size, chunk_size_block);
	}
	KTL_INLINE void NativeDownloader::resetInfo() {
		NetworkUtils::resetComponent(url_info_, url_info2_);
		NetworkUtils::resetComponent(http_response_, http_response2_);
		NetworkUtils::resetComponent(http_header_, http_header2_);
		flag_ = 0x00000000;
		storage_.reset();
		transferred_size_ = 0;
		content_length_ = 0;
		chunked_ = false;
	}
	KTL_INLINE void NativeDownloader::resetWorkers() {
		NetworkUtils::resetComponent(resolvers_, resolvers2_);
		NetworkUtils::resetComponent(connectors_, connectors2_);
		NetworkUtils::resetComponent(handshakers_, handshakers2_);
		NetworkUtils::resetComponent(writers_, writers2_);
		NetworkUtils::resetComponent(readers_, readers2_);
	}
	KTL_INLINE void NativeDownloader::resetWorkingBuffer(bool setup) {
		writing_streambuf_ = boost::make_shared<boost::asio::streambuf>();
		reading_streambuf_ = boost::make_shared<boost::asio::streambuf>();
		storage_out_.reset();
		if (!setup) {
			upload_buffer_.reset();
			//	HACK: カスタムヘッダをリセットする
			custom_header_.reset();
		}
	}
	KTL_INLINE void NativeDownloader::resetBuffer(bool setup) {
		resetWorkingBuffer(setup);
		buffer_.reset();
	}
	KTL_INLINE void NativeDownloader::resetSocket() {
		NetworkUtils::resetComponent(socket_, socket2_);
		NetworkUtils::resetComponent(ssl_socket_, ssl_socket2_);
	}
	KTL_INLINE void NativeDownloader::enableSocket() {
		NetworkUtils::moveComponent(socket_, socket2_);
		NetworkUtils::resetComponent(ssl_socket_, ssl_socket2_);
	}
	KTL_INLINE void NativeDownloader::enableSocketSSL() {
		NetworkUtils::moveComponent(ssl_socket_, ssl_socket2_);
		NetworkUtils::resetComponent(socket_, socket2_);
	}
	KTL_INLINE void NativeDownloader::setupOnProcessStart(bool to_buffer, flag_type flag) {
		is_processing_ = true;
		failed_ = false;
		cancelled_ = false;
		resetInfo();
		resetBuffer(true);
		flag_ = flag;
	}
	KTL_INLINE void NativeDownloader::cleanupOnProcessFailed() {
		resetWorkingBuffer();
		resetSocket();
		cancelTimeoutImplNoErrorHandling();
		is_processing_ = false;
		failed_ = true;
		//	HACK: 処理が完了するたびに、processStatusCodeError を true にリセットする
		process_status_code_error_ = true;
		postOnFinished();
	}
	KTL_INLINE void NativeDownloader::cleanupOnProcessSucceeded() {
		resetWorkingBuffer();
		resetSocket();
		cancelTimeoutImplNoErrorHandling();
		is_processing_ = false;
		failed_ = false;
		//	HACK: 処理が完了するたびに、processStatusCodeError を true にリセットする
		process_status_code_error_ = true;
		postOnFinished();
	}
	KTL_INLINE bool NativeDownloader::downloadHTTPImpl(
		tjs_char const* url,
		tjs_char const* storage,
		bool to_buffer,
		flag_type flag
		)
	{
		if (is_processing_) {
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("非同期処理が既に実行中です"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			return false;
		}
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("url"), url, SPRIG_KRKR_LOG_LEVEL_NORMAL);
		if (!to_buffer) {
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("storage"), storage, SPRIG_KRKR_LOG_LEVEL_NORMAL);
		}
		setupOnProcessStart(to_buffer, flag);
		if (!analyURL(sprig::str_cast<impl_string_type>(url))) {
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("URL解析でエラーが発生しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			cleanupOnProcessFailed();
			return false;
		}
		if (!setStorageName(storage, to_buffer)) {
			cleanupOnProcessFailed();
			return false;
		}
		if (!resolve(url_info_.host_name(), url_info_.service_name())) {
			cleanupOnProcessFailed();
			return false;
		}
		if (url_info_.service_name() == "https"
			? !connectSSL()
			: !connect()
			)
		{
			cleanupOnProcessFailed();
			return false;
		}
		if (url_info_.service_name() == "https" && !handshake()) {
			cleanupOnProcessFailed();
			return false;
		}
		putHTTPRequest(url_info_.host_name(), url_info_.content_path());
		if (write() < 0 || *writers_.error_code()) {
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("リクエスト送信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *writers_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), writers_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			cleanupOnProcessFailed();
			return false;
		}
		if (readUntil("\r\n\r\n") < 0 || *readers_.error_code()) {
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンス受信に失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), *readers_.error_code(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), readers_.error_code()->message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
			cleanupOnProcessFailed();
			return false;
		}
		if (!analyHTTPResponse(false)) {
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("レスポンス解析でエラーが発生しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			cleanupOnProcessFailed();
			return false;
		}
		if (http_response_.status_code() == "204") {
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("処理が完了しました/サーバから送信されたコンテンツはありません"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
			cleanupOnProcessSucceeded();
			return false;
		}
		if (storage_ && storage_->empty()) {
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("処理が完了しました/サーバから送信されたコンテンツを保存しません"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
			cleanupOnProcessSucceeded();
			return false;
		}
		if (!openBuffer(false)) {
			cleanupOnProcessFailed();
			return false;
		}
		if (chunked_) {
			// 先読みされた分を処理
			boost::tuple<bool, bool, size_type, impl_string_type> result(processChunked());
			if (!boost::get<0>(result)) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("チャンクデータの処理でエラーが発生しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				cleanupOnProcessFailed();
				return false;
			}
			if (boost::get<1>(result)) {
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ダウンロード完了しました"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
				cleanupOnProcessSucceeded();
				return true;
			}
			if (!loadChunked(boost::get<2>(result), boost::get<3>(result))) {
				cleanupOnProcessFailed();
				return false;
			}
		} else {
			// 先読みされた分を処理
			{
				transferred_size_ = (content_length_ && reading_streambuf_->size() > content_length_)
					? content_length_
					: reading_streambuf_->size()
					;
				if (!updateBuffer(transferred_size_)) {
					cleanupOnProcessFailed();
					return false;
				}
				if (content_length_ && transferred_size_ >= content_length_) {
					SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ダウンロード完了しました"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
					cleanupOnProcessSucceeded();
					return true;
				}
			}
			if (!loadBlock()) {
				cleanupOnProcessFailed();
				return false;
			}
		}
		cleanupOnProcessSucceeded();
		return true;
	}
	KTL_INLINE bool NativeDownloader::asyncDownloadHTTPImpl(
		tjs_char const* url,
		tjs_char const* storage,
		bool to_buffer,
		flag_type flag
		)
	{
		if (is_processing_) {
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("非同期処理が既に実行中です"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			return false;
		}
		SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("url"), url, SPRIG_KRKR_LOG_LEVEL_NORMAL);
		if (!to_buffer) {
			SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("storage"), storage, SPRIG_KRKR_LOG_LEVEL_NORMAL);
		}
		setupOnProcessStart(to_buffer, flag);
		if (!analyURL(sprig::str_cast<impl_string_type>(url))) {
			SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("URL解析でエラーが発生しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
			cleanupOnProcessFailed();
			return false;
		}
		if (!setStorageName(storage, to_buffer)) {
			cleanupOnProcessFailed();
			return false;
		}
		//
		// 名前解決開始
		//
		resolvers_.error_code().reset();
		resolvers2_.resolver() = boost::make_shared<boost::asio::ip::tcp::resolver>(
			boost::ref(*io_service_)
			);
		resolvers2_.query() = boost::make_shared<boost::asio::ip::tcp::resolver::query>(
			url_info_.host_name(),
			url_info_.service_name()
			);
		resolvers2_.error_code() = boost::system::error_code();
		resolvers2_.resolver()->async_resolve(
			*resolvers2_.query(),
			strand_->wrap(
				boost::bind(
					&NativeDownloader::handleResolove,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::iterator
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeDownloader::cancelImpl() {
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
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("接続のキャンセルに失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), error, SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), error.message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
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
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("ハンドシェイクのキャンセルに失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), error, SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), error.message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
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
				SPRIG_KRKR_OUTPUT_COMMENT(SPRIG_KRKR_TJS_W("送受信のキャンセルに失敗しました"), SPRIG_KRKR_LOG_LEVEL_WARNING);
				SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("error_code"), error, SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				SPRIG_KRKR_OUTPUT_VALUE(SPRIG_KRKR_TJS_W("message"), error.message(), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
				return false;
			}
		}
		return true;
	}
	KTL_INLINE bool NativeDownloader::cancelTimeoutImpl() {
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
	KTL_INLINE bool NativeDownloader::cancelTimeoutImplNoErrorHandling() {
		if (!timeout_timers2_.enable()) {
			return false;
		}
		timeout_timer_->cancel();
		return true;
	}
	NativeDownloader::NativeDownloader()
		: io_service_(NetworkService::get_const_instance().io_service())
		, strand_(boost::make_shared<boost::asio::io_service::strand>(boost::ref(*io_service_)))
		, writing_streambuf_(boost::make_shared<boost::asio::streambuf>())
		, reading_streambuf_(boost::make_shared<boost::asio::streambuf>())
		, timeout_timer_(boost::make_shared<timeout_timer_type>(boost::ref(*io_service_)))
		, flag_(0x00000000)
		, transferred_size_(0)
		, content_length_(0)
		, chunked_(false)
		, is_processing_(false)
		, failed_(false)
		, cancelled_(false)
		//	HACK: processStatusCodeError の初期値は true
		, process_status_code_error_(true)
	{}
	KTL_INLINE boost::shared_ptr<boost::asio::io_service> const& NativeDownloader::ioService() const {
		return io_service_;
	}
	KTL_INLINE NativeDownloader::mutex_type& NativeDownloader::mutex() const {
		return mutex_;
	}
	KTL_INLINE bool NativeDownloader::unfinished() const {
		return is_processing_;
	}
	KTL_INLINE bool NativeDownloader::doCancel() {
		return cancelImpl();
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE bool NativeDownloader::reset() {
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			return false;
		}
		resetInfo();
		is_processing_ = false;
		failed_ = false;
		cancelled_ = false;
		resetWorkers();
		resetBuffer();
		resetSocket();
		return true;
	}
	//
	//	SUMMARY: ダウンロード系メソッド
	//
	KTL_INLINE bool NativeDownloader::downloadHTTP(
		tjs_char const* url,
		tjs_char const* storage,
		flag_type flag
		)
	{
		scoped_lock_type lock(mutex_);
		return downloadHTTPImpl(url, storage, false, flag);
	}
	KTL_INLINE bool NativeDownloader::asyncDownloadHTTP(
		tjs_char const* url,
		tjs_char const* storage,
		flag_type flag
		)
	{
		scoped_lock_type lock(mutex_);
		return asyncDownloadHTTPImpl(url, storage, false, flag);
	}
	KTL_INLINE bool NativeDownloader::cancel() {
		scoped_lock_type lock(mutex_);
		return cancelImpl();
	}
	KTL_INLINE bool NativeDownloader::isProcessing() const {
		scoped_lock_type lock(mutex_);
		return is_processing_;
	}
	KTL_INLINE bool NativeDownloader::failed() const {
		scoped_lock_type lock(mutex_);
		return failed_;
	}
	KTL_INLINE bool NativeDownloader::cancelled() const {
		scoped_lock_type lock(mutex_);
		return cancelled_;
	}
	//
	//	SUMMARY: 情報系メソッド
	//
	KTL_INLINE NativeDownloader::size_type NativeDownloader::transferredSize() const {
		scoped_lock_type lock(mutex_);
		return transferred_size_;
	}
	KTL_INLINE NativeDownloader::size_type NativeDownloader::contentLength() const {
		scoped_lock_type lock(mutex_);
		return content_length_;
	}
	KTL_INLINE bool NativeDownloader::chunked() const {
		scoped_lock_type lock(mutex_);
		return chunked_;
	}
	//
	//	SUMMARY: バッファダウンロード系メソッド
	//
	KTL_INLINE bool NativeDownloader::downloadHTTPToBuffer(
		tjs_char const* url,
		flag_type flag
		)
	{
		scoped_lock_type lock(mutex_);
		return downloadHTTPImpl(url, 0, true, flag);
	}
	KTL_INLINE bool NativeDownloader::asyncDownloadHTTPToBuffer(
		tjs_char const* url,
		flag_type flag
		)
	{
		scoped_lock_type lock(mutex_);
		return asyncDownloadHTTPImpl(url, 0, true, flag);
	}
	KTL_INLINE bool NativeDownloader::clearBuffer() {
		scoped_lock_type lock(mutex_);
		if (!is_processing_) {
			return false;
		}
		buffer_.reset();
		return true;
	}
	KTL_INLINE bool NativeDownloader::bufferEnable() const {
		scoped_lock_type lock(mutex_);
		return static_cast<bool>(buffer_);
	}
	KTL_INLINE NativeDownloader::int_type NativeDownloader::bufferSize() const {
		scoped_lock_type lock(mutex_);
		return buffer_
			? buffer_->size()
			: -1
			;
	}
	//
	//	SUMMARY: バッファ取得系メソッド
	//
	tTJSVariant NativeDownloader::getBufferAsOctet() const {
		scoped_lock_type lock(mutex_);
		if (!buffer_) {
			return tTJSVariant();
		}
		size_type result_size = buffer_->size();
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&(*buffer_)[0]), result_size);
	}
	tTJSVariant NativeDownloader::getBufferAsOctet(size_type pos) const {
		scoped_lock_type lock(mutex_);
		if (!buffer_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(buffer_->size(), pos);
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&(*buffer_)[pos]), result_size);
	}
	tTJSVariant NativeDownloader::getBufferAsOctet(size_type pos, size_type size) const {
		scoped_lock_type lock(mutex_);
		if (!buffer_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(buffer_->size(), pos, size);
		if (!result_size) {
			return tTJSVariant(static_cast<tjs_uint8 const*>(0), 0);
		}
		return tTJSVariant(reinterpret_cast<tjs_uint8 const*>(&(*buffer_)[pos]), result_size);
	}
	tTJSVariant NativeDownloader::getBufferAsString() const {
		scoped_lock_type lock(mutex_);
		if (!buffer_) {
			return tTJSVariant();
		}
		buffer_type buffer(*buffer_);
		buffer.insert(buffer.end(), sizeof(tjs_char) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&buffer[0]));
	}
	tTJSVariant NativeDownloader::getBufferAsString(size_type pos) const {
		scoped_lock_type lock(mutex_);
		if (!buffer_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(buffer_->size(), pos);
		buffer_type buffer(
			result_size
				? boost::next(buffer_->begin(), pos)
				: buffer_->end()
				,
			buffer_->end()
			);
		buffer.insert(buffer.end(), sizeof(tjs_char) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&buffer[0]));
	}
	tTJSVariant NativeDownloader::getBufferAsString(size_type pos, size_type size) const {
		scoped_lock_type lock(mutex_);
		if (!buffer_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(buffer_->size(), pos, size);
		buffer_type buffer(
			result_size
				? boost::next(buffer_->begin(), pos)
				: buffer_->end()
				,
			result_size
				? boost::next(buffer_->begin(), pos + result_size)
				: buffer_->end()
			);
		buffer.insert(buffer.end(), sizeof(tjs_char) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_char const*>(&buffer[0]));
	}
	tTJSVariant NativeDownloader::getBufferAsNarrowString() const {
		scoped_lock_type lock(mutex_);
		if (!buffer_) {
			return tTJSVariant();
		}
		buffer_type buffer(*buffer_);
		buffer.insert(buffer.end(), sizeof(tjs_nchar) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&buffer[0]));
	}
	tTJSVariant NativeDownloader::getBufferAsNarrowString(size_type pos) const {
		scoped_lock_type lock(mutex_);
		if (!buffer_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(buffer_->size(), pos);
		buffer_type buffer(
			result_size
				? boost::next(buffer_->begin(), pos)
				: buffer_->end()
				,
			buffer_->end()
			);
		buffer.insert(buffer.end(), sizeof(tjs_nchar) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&buffer[0]));
	}
	tTJSVariant NativeDownloader::getBufferAsNarrowString(size_type pos, size_type size) const {
		scoped_lock_type lock(mutex_);
		if (!buffer_) {
			return tTJSVariant();
		}
		size_type result_size = getEnableSize(buffer_->size(), pos, size);
		buffer_type buffer(
			result_size
				? boost::next(buffer_->begin(), pos)
				: buffer_->end()
				,
			result_size
				? boost::next(buffer_->begin(), pos + result_size)
				: buffer_->end()
			);
		buffer.insert(buffer.end(), sizeof(tjs_nchar) * 2 - 1, 0);
		return tTJSVariant(reinterpret_cast<tjs_nchar const*>(&buffer[0]));
	}
	//
	//	SUMMARY: タイムアウト系メソッド
	//
	KTL_INLINE bool NativeDownloader::waitTimeoutFromNow(rep_type expiry_time_millisec) {
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
						&NativeDownloader::handleTimeout,
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
	KTL_INLINE bool NativeDownloader::waitTimeoutAt(rep_type expiry_time_millisec) {
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
			timeout_timers2_.error_code() = boost::system::error_code();
			timeout_timer_->async_wait(
				strand_->wrap(
					boost::bind(
						&NativeDownloader::handleTimeout,
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
	KTL_INLINE bool NativeDownloader::asyncWaitTimeoutFromNow(rep_type expiry_time_millisec) {
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
					&NativeDownloader::handleTimeout,
					this,
					boost::asio::placeholders::error
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeDownloader::asyncWaitTimeoutAt(rep_type expiry_time_millisec) {
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
					&NativeDownloader::handleTimeout,
					this,
					boost::asio::placeholders::error
					)
				)
			);
		return true;
	}
	KTL_INLINE bool NativeDownloader::cancelTimeout() {
		scoped_lock_type lock(mutex_);
		return cancelTimeoutImpl();
	}
	KTL_INLINE bool NativeDownloader::isWaitingTimeout() const {
		scoped_lock_type lock(mutex_);
		return static_cast<bool>(timeout_timers2_.error_code());
	}
	KTL_INLINE NativeDownloader::rep_type NativeDownloader::expiresTimeoutFromNow() const {
		scoped_lock_type lock(mutex_);
		return boost::chrono::duration_cast<milliseconds_type>(timeout_timer_->expires_from_now()).count();
	}
	KTL_INLINE NativeDownloader::rep_type NativeDownloader::expiresTimeoutAt() const {
		scoped_lock_type lock(mutex_);
		return boost::chrono::duration_cast<milliseconds_type>(timeout_timer_->expires_at().time_since_epoch()).count();
	}
	//
	//	SUMMARY: 待機系メソッド
	//
	KTL_INLINE bool NativeDownloader::join() {
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
	//	SUMMARY: ポスト系メソッド
	//
	KTL_INLINE bool NativeDownloader::addPostDataEncoded(tjs_char const* source) {
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			return false;
		}
		std::string str(sprig::str_cast<std::string>(source));
		if (!upload_buffer_) {
			upload_buffer_ = boost::make_shared<buffer_type>();
		}
		std::copy(
			str.begin(),
			str.end(),
			std::back_inserter(*upload_buffer_)
			);
		return true;
	}
	KTL_INLINE bool NativeDownloader::addPostDataEncoded(tTJSVariantOctet const* source) {
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			return false;
		}
		char const* data = reinterpret_cast<char const*>(sprig::krkr::tjs::octet_data(source));
		size_type length = sprig::krkr::tjs::octet_length(source);
		if (!upload_buffer_) {
			upload_buffer_ = boost::make_shared<buffer_type>();
		}
		std::copy(
			data,
			data + length,
			std::back_inserter(*upload_buffer_)
			);
		return true;
	}
	KTL_INLINE bool NativeDownloader::clearPostData() {
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			return false;
		}
		upload_buffer_.reset();
		return true;
	}
	KTL_INLINE bool NativeDownloader::postDataEnable() const {
		return static_cast<bool>(upload_buffer_);
	}
	KTL_INLINE NativeDownloader::int_type NativeDownloader::postDataSize() const {
		return upload_buffer_
			? upload_buffer_->size()
			: -1
			;
	}
	//
	//	SUMMARY: URL情報系メソッド
	//
	KTL_INLINE NativeDownloader::impl_string_type const& NativeDownloader::hostName() const {
		scoped_lock_type lock(mutex_);
		return url_info_.host_name();
	}
	KTL_INLINE NativeDownloader::impl_string_type const& NativeDownloader::serviceName() const {
		scoped_lock_type lock(mutex_);
		return url_info_.service_name();
	}
	KTL_INLINE NativeDownloader::impl_string_type const& NativeDownloader::contentPath() const {
		scoped_lock_type lock(mutex_);
		return url_info_.content_path();
	}
	KTL_INLINE NativeDownloader::impl_string_type NativeDownloader::URL() const {
		scoped_lock_type lock(mutex_);
		if (url_info_.host_name().empty()) {
			return impl_string_type();
		}
		std::basic_ostringstream<impl_string_type::value_type, impl_string_type::traits_type> os;
		os << url_info_.service_name() << "://" << url_info_.host_name() << url_info_.content_path();
		return os.str();
	}
	KTL_INLINE std::string NativeDownloader::storageLocalName() const {
		scoped_lock_type lock(mutex_);
		return storage_
			? *storage_
			: std::string()
			;
	}
	//
	//	SUMMARY: HTTPレスポンス系メソッド
	//
	KTL_INLINE NativeDownloader::impl_string_type const& NativeDownloader::HTTPVersion() const {
		scoped_lock_type lock(mutex_);
		return http_response_.http_version();
	}
	KTL_INLINE NativeDownloader::impl_string_type const& NativeDownloader::statusCode() const {
		scoped_lock_type lock(mutex_);
		return http_response_.status_code();
	}
	KTL_INLINE NativeDownloader::impl_string_type const& NativeDownloader::reasonPhrase() const {
		scoped_lock_type lock(mutex_);
		return http_response_.reason_phrase();
	}
	KTL_INLINE tTJSVariant NativeDownloader::getField(tjs_char const* name) const {
		scoped_lock_type lock(mutex_);
		typedef HTTPHeader::impl_map_type::const_iterator const_iterator;
		const_iterator found = http_header_.field_map().find(
			sprig::str_cast<impl_string_type>(name)
			);
		if (found == http_header_.field_map().end()) {
			return tTJSVariant();
		}
		return tTJSVariant(found->second.c_str());
	}
	//
	//	SUMMARY: コールバック系メソッド
	//
	KTL_INLINE tTJSVariant NativeDownloader::getOnFinished() const {
		scoped_lock_type lock(mutex_);
		return on_finished_
			? *on_finished_
			: tTJSVariant()
			;
	}
	KTL_INLINE void NativeDownloader::setOnFinished(tTJSVariant const& func) {
		scoped_lock_type lock(mutex_);
		if (func.Type() == tvtObject) {
			on_finished_ = boost::make_shared<tTJSVariant>(func);
		} else {
			on_finished_.reset();
		}
	}
	//
	//	SUMMARY: カスタムヘッダ系メソッド
	//
	//		HACK: 行末の"\r\n"設定はスクリプト側に任せる
	//
	KTL_INLINE bool NativeDownloader::setCustomHeader(tjs_char const* source) {
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			return false;
		}
		std::string str(sprig::str_cast<std::string>(source));
		if (!custom_header_) {
			custom_header_ = boost::make_shared<std::string>();
		}
		std::copy(
			str.begin(),
			str.end(),
			std::back_inserter(*custom_header_)
			);
		return true;
	}
	KTL_INLINE bool NativeDownloader::clearCustomHeader() {
		scoped_lock_type lock(mutex_);
		if (is_processing_) {
			return false;
		}
		custom_header_.reset();
		return true;
	}
	//
	//	SUMMARY: 処理設定系メソッド
	//
	//		HACK: false に設定すると、失敗のステータスコードが返されても解析を続ける
	//
	KTL_INLINE void NativeDownloader::setProcessStatusCodeError(bool value) {
		scoped_lock_type lock(mutex_);
		process_status_code_error_ = value;
	}
	KTL_INLINE bool NativeDownloader::getProcessStatusCodeError() const {
		scoped_lock_type lock(mutex_);
		return process_status_code_error_;
	}

	//
	// Downloader::AliveHandler
	//
	KTL_INLINE Downloader::AliveHandler::AliveHandler(boost::shared_ptr<NativeDownloader> const& instance)
		: instance_(instance)
	{}
	KTL_INLINE void Downloader::AliveHandler::operator()() const {
		scoped_lock_type lock(instance_->mutex());
		if (instance_->unfinished()) {
			instance_->ioService()->post(*this);
		}
	}
	//
	// Downloader
	//
	Downloader::Downloader() {}
	tjs_error TJS_INTF_METHOD Downloader::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Downloader::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeDownloader>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Downloader::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Downloader::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		{
			AliveHandler::scoped_lock_type lock(instance_->mutex());
			if (instance_->unfinished()) {
				instance_->doCancel();
				instance_->ioService()->post(AliveHandler(instance_));
			}
		}
		instance_.reset();
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE bool Downloader::reset() {
		return instance_->reset();
	}
	//
	//	SUMMARY: ダウンロード系メソッド
	//
	KTL_INLINE bool Downloader::downloadHTTP(
		tTJSVariantString const* url,
		tTJSVariantString const* storage,
		tTVInteger flag
		) {
		return instance_->downloadHTTP(
			sprig::krkr::tjs::as_c_str(url),
			sprig::krkr::tjs::as_c_str(storage),
			sprig::numeric::bit_cast<NativeDownloader::flag_type>(flag)
			);
	}
	KTL_INLINE bool Downloader::asyncDownloadHTTP(
		tTJSVariantString const* url,
		tTJSVariantString const* storage,
		tTVInteger flag
		)
	{
		return instance_->asyncDownloadHTTP(
			sprig::krkr::tjs::as_c_str(url),
			sprig::krkr::tjs::as_c_str(storage),
			sprig::numeric::bit_cast<NativeDownloader::flag_type>(flag)
			);
	}
	KTL_INLINE bool Downloader::cancel() {
		return instance_->cancel();
	}
	KTL_INLINE bool Downloader::isProcessing() const {
		return instance_->isProcessing();
	}
	KTL_INLINE bool Downloader::failed() const {
		return instance_->failed();
	}
	KTL_INLINE bool Downloader::cancelled() const {
		return instance_->cancelled();
	}
	//
	//	SUMMARY: 情報系メソッド
	//
	KTL_INLINE tTVInteger Downloader::transferredSize() const {
		return instance_->transferredSize();
	}
	KTL_INLINE tTVInteger Downloader::contentLength() const {
		return instance_->contentLength();
	}
	KTL_INLINE bool Downloader::chunked() const {
		return instance_->chunked();
	}
	//
	//	SUMMARY: バッファダウンロード系メソッド
	//
	KTL_INLINE bool Downloader::downloadHTTPToBuffer(
		tTJSVariantString const* url,
		tTVInteger flag
		)
	{
		return instance_->downloadHTTPToBuffer(
			sprig::krkr::tjs::as_c_str(url),
			sprig::numeric::bit_cast<NativeDownloader::flag_type>(flag)
			);
	}
	KTL_INLINE bool Downloader::asyncDownloadHTTPToBuffer(
		tTJSVariantString const* url,
		tTVInteger flag
		)
	{
		return instance_->asyncDownloadHTTPToBuffer(
			sprig::krkr::tjs::as_c_str(url),
			sprig::numeric::bit_cast<NativeDownloader::flag_type>(flag)
			);
	}
	KTL_INLINE bool Downloader::clearBuffer() {
		return instance_->clearBuffer();
	}
	KTL_INLINE bool Downloader::bufferEnable() const {
		return instance_->bufferEnable();
	}
	KTL_INLINE tTVInteger Downloader::bufferSize() const {
		return instance_->bufferSize();
	}
	//
	//	SUMMARY: バッファ取得系メソッド
	//
	KTL_INLINE tTJSVariant Downloader::getBufferAsOctet() const {
		return instance_->getBufferAsOctet();
	}
	KTL_INLINE tTJSVariant Downloader::getBufferAsOctet(tTVInteger pos) const {
		return instance_->getBufferAsOctet(
			boost::numeric_cast<NativeDownloader::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Downloader::getBufferAsOctet(tTVInteger pos, tTVInteger size) const {
		return instance_->getBufferAsOctet(
			boost::numeric_cast<NativeDownloader::size_type>(pos),
			boost::numeric_cast<NativeDownloader::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Downloader::getBufferAsString() const {
		return instance_->getBufferAsString();
	}
	KTL_INLINE tTJSVariant Downloader::getBufferAsString(tTVInteger pos) const {
		return instance_->getBufferAsString(
			boost::numeric_cast<NativeDownloader::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Downloader::getBufferAsString(tTVInteger pos, tTVInteger size) const {
		return instance_->getBufferAsString(
			boost::numeric_cast<NativeDownloader::size_type>(pos),
			boost::numeric_cast<NativeDownloader::size_type>(size)
			);
	}
	KTL_INLINE tTJSVariant Downloader::getBufferAsNarrowString() const {
		return instance_->getBufferAsNarrowString();
	}
	KTL_INLINE tTJSVariant Downloader::getBufferAsNarrowString(tTVInteger pos) const {
		return instance_->getBufferAsNarrowString(
			boost::numeric_cast<NativeDownloader::size_type>(pos)
			);
	}
	KTL_INLINE tTJSVariant Downloader::getBufferAsNarrowString(tTVInteger pos, tTVInteger size) const {
		return instance_->getBufferAsNarrowString(
			boost::numeric_cast<NativeDownloader::size_type>(pos),
			boost::numeric_cast<NativeDownloader::size_type>(size)
			);
	}
	//
	//	SUMMARY: タイムアウト系メソッド
	//
	KTL_INLINE bool Downloader::waitTimeoutFromNow(tTVInteger expiry_time_millisec) {
		return instance_->waitTimeoutFromNow(
			boost::numeric_cast<NativeDownloader::rep_type>(expiry_time_millisec)
			);
	}
	KTL_INLINE bool Downloader::waitTimeoutAt(tTVInteger expiry_time_millisec) {
		return instance_->waitTimeoutAt(
			boost::numeric_cast<NativeDownloader::rep_type>(expiry_time_millisec)
			);
	}
	KTL_INLINE bool Downloader::asyncWaitTimeoutFromNow(tTVInteger expiry_time_millisec) {
		return instance_->asyncWaitTimeoutFromNow(
			boost::numeric_cast<NativeDownloader::rep_type>(expiry_time_millisec)
			);
	}
	KTL_INLINE bool Downloader::asyncWaitTimeoutAt(tTVInteger expiry_time_millisec) {
		return instance_->asyncWaitTimeoutAt(
			boost::numeric_cast<NativeDownloader::rep_type>(expiry_time_millisec)
			);
	}
	KTL_INLINE bool Downloader::cancelTimeout() {
		return instance_->cancelTimeout();
	}
	KTL_INLINE bool Downloader::isWaitingTimeout() const {
		return instance_->isWaitingTimeout();
	}
	KTL_INLINE tTVInteger Downloader::expiresTimeoutFromNow() const {
		return instance_->expiresTimeoutFromNow();
	}
	KTL_INLINE tTVInteger Downloader::expiresTimeoutAt() const {
		return instance_->expiresTimeoutAt();
	}
	//
	//	SUMMARY: 待機系メソッド
	//
	KTL_INLINE bool Downloader::join() {
		return instance_->join();
	}
	//
	//	SUMMARY: ポスト系メソッド
	//
	KTL_INLINE bool Downloader::addPostDataEncoded(tTJSVariantString const* source) {
		return instance_->addPostDataEncoded(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE bool Downloader::addPostDataEncoded(tTJSVariantOctet const* source) {
		return instance_->addPostDataEncoded(
			source
			);
	}
	KTL_INLINE bool Downloader::clearPostData() {
		return instance_->clearPostData();
	}
	KTL_INLINE bool Downloader::postDataEnable() const {
		return instance_->postDataEnable();
	}
	KTL_INLINE tTVInteger Downloader::postDataSize() const {
		return instance_->postDataSize();
	}
	//
	//	SUMMARY: URL情報系メソッド
	//
	KTL_INLINE tTJSString Downloader::hostName() const {
		return instance_->hostName().c_str();
	}
	KTL_INLINE tTJSString Downloader::serviceName() const {
		return instance_->serviceName().c_str();
	}
	KTL_INLINE tTJSString Downloader::contentPath() const {
		return instance_->contentPath().c_str();
	}
	KTL_INLINE tTJSString Downloader::URL() const {
		return instance_->URL().c_str();
	}
	KTL_INLINE tTJSString Downloader::storageLocalName() const {
		return instance_->storageLocalName().c_str();
	}
	//
	//	SUMMARY: HTTPレスポンス系メソッド
	//
	KTL_INLINE tTJSString Downloader::HTTPVersion() const {
		return instance_->HTTPVersion().c_str();
	}
	KTL_INLINE tTJSString Downloader::statusCode() const {
		return instance_->statusCode().c_str();
	}
	KTL_INLINE tTJSString Downloader::reasonPhrase() const {
		return instance_->reasonPhrase().c_str();
	}
	KTL_INLINE tTJSVariant Downloader::getField(tTJSVariantString const* name) const {
		return instance_->getField(
			sprig::krkr::tjs::as_c_str(name)
			);
	}
	//
	//	SUMMARY: コールバック系メソッド
	//
	KTL_INLINE tTJSVariant Downloader::getOnFinished() const {
		return instance_->getOnFinished();
	}
	KTL_INLINE void Downloader::setOnFinished(tTJSVariant const& func) {
		instance_->setOnFinished(func);
	}
	//
	//	SUMMARY: カスタムヘッダ系メソッド
	//
	KTL_INLINE bool Downloader::setCustomHeader(tTJSVariantString const* source) {
		return instance_->setCustomHeader(
			sprig::krkr::tjs::as_c_str(source)
			);
	}
	KTL_INLINE bool Downloader::clearCustomHeader() {
		return instance_->clearCustomHeader();
	}
	//
	//	SUMMARY: 処理設定系メソッド
	//
	KTL_INLINE void Downloader::setProcessStatusCodeError(bool value) {
		instance_->setProcessStatusCodeError(value);
	}
	KTL_INLINE bool Downloader::getProcessStatusCodeError() const {
		return instance_->getProcessStatusCodeError();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_NETWORK_DOWNLOADER_HPP
