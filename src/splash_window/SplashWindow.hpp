/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_SPLASH_WINDOW_SPLASH_WINDOW_HPP
#define SRC_SPLASH_WINDOW_SPLASH_WINDOW_HPP

#include <vector>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/windows.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "WindowCommon.hpp"
#include "SplashWindowDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:SplashWindow:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:SplashWindow:error")

namespace ktl {
	//
	// NativeSplashWindow
	//
	NativeSplashWindow::NativeSplashWindow()
		: impl_(new impl_type())
	{}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE void NativeSplashWindow::initialize(size_type width, size_type height) {
		if (impl_->initialized()) {
			impl_.reset(new impl_type());
		}
		impl_->initialize(width, height);
	}
	KTL_INLINE void NativeSplashWindow::initialize(size_type width, size_type height, string_type const& identity) {
		if (impl_->initialized()) {
			impl_.reset(new impl_type());
		}
		impl_->initialize(width, height, identity);
	}
	KTL_INLINE void NativeSplashWindow::uninitialize() {
		impl_.reset(new impl_type());
	}
	//
	//	SUMMARY: 読み込み系メソッド
	//
	KTL_INLINE bool NativeSplashWindow::loadImages(tTJSString const& storage) {
		sprig::com_ptr<::IStream> in(::TVPCreateIStream(storage, TJS_BS_READ));
		if (!in) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました: ")) + storage,
				sprig::krkr::bad_state
				);
			return false;
		}
		LARGE_INTEGER move;
		move.QuadPart = 0;
		ULARGE_INTEGER new_pos;
		if (FAILED(in->Seek(move, TJS_BS_SEEK_END, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		size_type file_size = static_cast<size_type>(new_pos.QuadPart);
		if (!file_size) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルが空です: ")) + storage,
				sprig::krkr::bad_state
				);
			return false;
		}
		move.QuadPart = 0;
		if (FAILED(in->Seek(move, TJS_BS_SEEK_SET, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		std::vector<byte_type> buffer(file_size);
		ULONG io_size = 0;
		if (FAILED(in->Read(&buffer[0], buffer.size(), &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
		}
		return impl_->loadImages(&buffer[0], io_size);
	}
	KTL_INLINE bool NativeSplashWindow::loadImagesFromOctet(tTJSVariantOctet const* source) {
		return impl_->loadImages(sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_length(source));
	}
	//
	//	SUMMARY: プロパテイ系メソッド
	//
	KTL_INLINE bool NativeSplashWindow::getVisible() const {
		return impl_->getVisible();
	}
	KTL_INLINE void NativeSplashWindow::setVisible(bool v) {
		impl_->setVisible(v);
	}
	KTL_INLINE bool NativeSplashWindow::getTopMost() const {
		return impl_->getTopMost();
	}
	KTL_INLINE void NativeSplashWindow::setTopMost(bool v) {
		impl_->setTopMost(v);
	}
	KTL_INLINE NativeSplashWindow::size_type NativeSplashWindow::getImageWidth() const {
		return impl_->getImageWidth();
	}
	KTL_INLINE NativeSplashWindow::size_type NativeSplashWindow::getImageHeight() const {
		return impl_->getImageHeight();
	}
	KTL_INLINE NativeSplashWindow::int_type NativeSplashWindow::getLeft() const {
		return impl_->getLeft();
	}
	KTL_INLINE NativeSplashWindow::int_type NativeSplashWindow::getTop() const {
		return impl_->getTop();
	}
	KTL_INLINE NativeSplashWindow::size_type NativeSplashWindow::getWidth() const {
		return impl_->getWidth();
	}
	KTL_INLINE NativeSplashWindow::size_type NativeSplashWindow::getHeight() const {
		return impl_->getHeight();
	}
	KTL_INLINE NativeSplashWindow::string_type const& NativeSplashWindow::getIdentity() const {
		return impl_->getIdentity();
	}
	KTL_INLINE bool NativeSplashWindow::initialized() const {
		return impl_->initialized();
	}

	//
	// SplashWindow
	//
	SplashWindow::SplashWindow() {}
	tjs_error TJS_INTF_METHOD SplashWindow::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("SplashWindow::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeSplashWindow>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD SplashWindow::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("SplashWindow::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE void SplashWindow::initialize(tTVInteger width, tTVInteger height) {
		instance_->initialize(
			boost::numeric_cast<NativeSplashWindow::int_type>(width),
			boost::numeric_cast<NativeSplashWindow::int_type>(height)
			);
	}
	KTL_INLINE void SplashWindow::initialize(tTVInteger width, tTVInteger height, tTJSVariantString const* identity) {
		instance_->initialize(
			boost::numeric_cast<NativeSplashWindow::int_type>(width),
			boost::numeric_cast<NativeSplashWindow::int_type>(height),
			sprig::str_cast<NativeSplashWindow::string_type>(sprig::krkr::tjs::as_c_str(identity))
			);
	}
	KTL_INLINE void SplashWindow::uninitialize() {
		instance_->uninitialize();
	}
	//
	//	SUMMARY: 読み込み系メソッド
	//
	KTL_INLINE bool SplashWindow::loadImages(tTJSVariantString const* storage) {
		return instance_->loadImages(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE bool SplashWindow::loadImagesFromOctet(tTJSVariantOctet const* source) {
		return instance_->loadImagesFromOctet(source);
	}
	//
	//	SUMMARY: プロパテイ系メソッド
	//
	KTL_INLINE bool SplashWindow::getVisible() const {
		return instance_->getVisible();
	}
	KTL_INLINE void SplashWindow::setVisible(bool v) {
		instance_->setVisible(v);
	}
	KTL_INLINE bool SplashWindow::getTopMost() const {
		return instance_->getTopMost();
	}
	KTL_INLINE void SplashWindow::setTopMost(bool v) {
		instance_->setTopMost(v);
	}
	KTL_INLINE tTVInteger SplashWindow::getImageWidth() const {
		return instance_->getImageWidth();
	}
	KTL_INLINE tTVInteger SplashWindow::getImageHeight() const {
		return instance_->getImageHeight();
	}
	KTL_INLINE tTVInteger SplashWindow::getLeft() const {
		return instance_->getLeft();
	}
	KTL_INLINE tTVInteger SplashWindow::getTop() const {
		return instance_->getTop();
	}
	KTL_INLINE tTVInteger SplashWindow::getWidth() const {
		return instance_->getWidth();
	}
	KTL_INLINE tTVInteger SplashWindow::getHeight() const {
		return instance_->getHeight();
	}
	KTL_INLINE tTJSString SplashWindow::getIdentity() const {
		return sprig::str_cast<tTJSString>(instance_->getIdentity());
	}
	KTL_INLINE bool SplashWindow::initialized() const {
		return instance_->initialized();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_SPLASH_WINDOW_SPLASH_WINDOW_HPP
