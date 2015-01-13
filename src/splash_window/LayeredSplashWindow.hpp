/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_SPLASH_WINDOW_LAYERED_SPLASH_WINDOW_HPP
#define SRC_SPLASH_WINDOW_LAYERED_SPLASH_WINDOW_HPP

#include <vector>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/optional/optional.hpp>
#include <sprig/external/windows.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "WindowCommon.hpp"
#include "LayeredSplashWindowDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:LayeredSplashWindow:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:LayeredSplashWindow:error")

namespace ktl {
	//
	// NativeLayeredSplashWindow
	//
	NativeLayeredSplashWindow::NativeLayeredSplashWindow()
		: impl_(new impl_type())
	{}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE void NativeLayeredSplashWindow::initialize() {
		if (impl_->initialized()) {
			impl_.reset(new impl_type());
		}
		impl_->initialize();
	}
	KTL_INLINE void NativeLayeredSplashWindow::initialize(string_type const& identity) {
		if (impl_->initialized()) {
			impl_.reset(new impl_type());
		}
		impl_->initialize(identity);
	}
	KTL_INLINE void NativeLayeredSplashWindow::uninitialize() {
		impl_.reset(new impl_type());
	}
	//
	//	SUMMARY: 読み込み系メソッド
	//
	KTL_INLINE bool NativeLayeredSplashWindow::loadImages(tTJSString const& storage) {
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
	KTL_INLINE bool NativeLayeredSplashWindow::loadImagesFromOctet(tTJSVariantOctet const* source) {
		return impl_->loadImages(sprig::krkr::tjs::octet_data(source), sprig::krkr::tjs::octet_length(source));
	}
	//
	//	SUMMARY: 更新系メソッド
	//
	KTL_INLINE bool NativeLayeredSplashWindow::update(
		boost::optional<int_type> left,
		boost::optional<int_type> top,
		boost::optional<byte_type> opacity
		)
	{
		return impl_->update(
			left ? *left : impl_->getLeft(),
			top ? *top : impl_->getTop(),
			opacity ? *opacity : impl_->getOpacity()
			);
	}
	KTL_INLINE bool NativeLayeredSplashWindow::updateToCenter(
		boost::optional<byte_type> opacity
		)
	{
		return impl_->updateToCenter(
			opacity ? *opacity : impl_->getOpacity()
			);
	}
	//
	//	SUMMARY: プロパテイ系メソッド
	//
	KTL_INLINE bool NativeLayeredSplashWindow::getVisible() const {
		return impl_->getVisible();
	}
	KTL_INLINE void NativeLayeredSplashWindow::setVisible(bool v) {
		impl_->setVisible(v);
	}
	KTL_INLINE bool NativeLayeredSplashWindow::getTopMost() const {
		return impl_->getTopMost();
	}
	KTL_INLINE void NativeLayeredSplashWindow::setTopMost(bool v) {
		impl_->setTopMost(v);
	}
	KTL_INLINE NativeLayeredSplashWindow::size_type NativeLayeredSplashWindow::getImageWidth() const {
		return impl_->getImageWidth();
	}
	KTL_INLINE NativeLayeredSplashWindow::size_type NativeLayeredSplashWindow::getImageHeight() const {
		return impl_->getImageHeight();
	}
	KTL_INLINE NativeLayeredSplashWindow::int_type NativeLayeredSplashWindow::getLeft() const {
		return impl_->getLeft();
	}
	KTL_INLINE NativeLayeredSplashWindow::int_type NativeLayeredSplashWindow::getTop() const {
		return impl_->getTop();
	}
	KTL_INLINE NativeLayeredSplashWindow::size_type NativeLayeredSplashWindow::getWidth() const {
		return impl_->getWidth();
	}
	KTL_INLINE NativeLayeredSplashWindow::size_type NativeLayeredSplashWindow::getHeight() const {
		return impl_->getHeight();
	}
	KTL_INLINE NativeLayeredSplashWindow::byte_type NativeLayeredSplashWindow::getOpacity() const {
		return impl_->getOpacity();
	}
	KTL_INLINE NativeLayeredSplashWindow::string_type const& NativeLayeredSplashWindow::getIdentity() const {
		return impl_->getIdentity();
	}
	KTL_INLINE bool NativeLayeredSplashWindow::initialized() const {
		return impl_->initialized();
	}

	//
	// LayeredSplashWindow
	//
	LayeredSplashWindow::LayeredSplashWindow() {}
	tjs_error TJS_INTF_METHOD LayeredSplashWindow::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("LayeredSplashWindow::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeLayeredSplashWindow>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD LayeredSplashWindow::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("LayeredSplashWindow::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 初期化系メソッド
	//
	KTL_INLINE void LayeredSplashWindow::initialize() {
		instance_->initialize();
	}
	KTL_INLINE void LayeredSplashWindow::initialize(tTJSVariantString const* identity) {
		instance_->initialize(
			sprig::str_cast<NativeLayeredSplashWindow::string_type>(sprig::krkr::tjs::as_c_str(identity))
			);
	}
	KTL_INLINE void LayeredSplashWindow::uninitialize() {
		instance_->uninitialize();
	}
	//
	//	SUMMARY: 読み込み系メソッド
	//
	KTL_INLINE bool LayeredSplashWindow::loadImages(tTJSVariantString const* storage) {
		return instance_->loadImages(
			sprig::krkr::tjs::as_c_str(storage)
			);
	}
	KTL_INLINE bool LayeredSplashWindow::loadImagesFromOctet(tTJSVariantOctet const* source) {
		return instance_->loadImagesFromOctet(source);
	}
	//
	//	SUMMARY: 更新系メソッド
	//
	KTL_INLINE bool LayeredSplashWindow::update(
		boost::optional<tTVInteger> left,
		boost::optional<tTVInteger> top,
		boost::optional<tTVInteger> opacity
		)
	{
		return instance_->update(
			left
				? boost::optional<NativeLayeredSplashWindow::int_type>(boost::numeric_cast<NativeLayeredSplashWindow::int_type>(*left))
				: boost::none
				,
			top
				? boost::optional<NativeLayeredSplashWindow::int_type>(boost::numeric_cast<NativeLayeredSplashWindow::int_type>(*top))
				: boost::none
				,
			opacity
				? boost::optional<NativeLayeredSplashWindow::byte_type>(sprig::numeric::overflow_cast<NativeLayeredSplashWindow::byte_type>(*opacity))
				: boost::none
			);
	}
	KTL_INLINE bool LayeredSplashWindow::updateToCenter(
		boost::optional<tTVInteger> opacity
		)
	{
		return instance_->updateToCenter(
			opacity
				? boost::optional<NativeLayeredSplashWindow::byte_type>(sprig::numeric::overflow_cast<NativeLayeredSplashWindow::byte_type>(*opacity))
				: boost::none
			);
	}
	//
	//	SUMMARY: プロパテイ系メソッド
	//
	KTL_INLINE bool LayeredSplashWindow::getVisible() const {
		return instance_->getVisible();
	}
	KTL_INLINE void LayeredSplashWindow::setVisible(bool v) {
		instance_->setVisible(v);
	}
	KTL_INLINE bool LayeredSplashWindow::getTopMost() const {
		return instance_->getTopMost();
	}
	KTL_INLINE void LayeredSplashWindow::setTopMost(bool v) {
		instance_->setTopMost(v);
	}
	KTL_INLINE tTVInteger LayeredSplashWindow::getImageWidth() const {
		return instance_->getImageWidth();
	}
	KTL_INLINE tTVInteger LayeredSplashWindow::getImageHeight() const {
		return instance_->getImageHeight();
	}
	KTL_INLINE tTVInteger LayeredSplashWindow::getLeft() const {
		return instance_->getLeft();
	}
	KTL_INLINE tTVInteger LayeredSplashWindow::getTop() const {
		return instance_->getTop();
	}
	KTL_INLINE tTVInteger LayeredSplashWindow::getWidth() const {
		return instance_->getWidth();
	}
	KTL_INLINE tTVInteger LayeredSplashWindow::getHeight() const {
		return instance_->getHeight();
	}
	KTL_INLINE tTVInteger LayeredSplashWindow::getOpacity() const {
		return instance_->getOpacity();
	}
	KTL_INLINE tTJSString LayeredSplashWindow::getIdentity() const {
		return sprig::str_cast<tTJSString>(instance_->getIdentity());
	}
	KTL_INLINE bool LayeredSplashWindow::initialized() const {
		return instance_->initialized();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_SPLASH_WINDOW_LAYERED_SPLASH_WINDOW_HPP
