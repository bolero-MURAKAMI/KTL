/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_SPLASH_WINDOW_LAYERED_SPLASH_WINDOW_DECL_HPP
#define SRC_SPLASH_WINDOW_LAYERED_SPLASH_WINDOW_DECL_HPP

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <boost/optional/optional.hpp>
#include <sprig/external/tp_stub.hpp>

#include "WindowCommon.hpp"

namespace ktl {
	//
	// NativeLayeredSplashWindow
	//
	class NativeLayeredSplashWindow {
	public:
		typedef LayeredSplash impl_type;
		typedef boost::scoped_ptr<impl_type> impl_instance_type;
		typedef impl_type::size_type size_type;
		typedef impl_type::int_type int_type;
		typedef impl_type::string_type string_type;
		typedef impl_type::byte_type byte_type;
	private:
		impl_instance_type impl_;
	public:
		NativeLayeredSplashWindow();
	public:
		//
		//	SUMMARY: 初期化系メソッド
		//
		void initialize();
		void initialize(string_type const& identity);
		void uninitialize();
		//
		//	SUMMARY: 読み込み系メソッド
		//
		bool loadImages(tTJSString const& storage);
		bool loadImagesFromOctet(tTJSVariantOctet const* source);
		//
		//	SUMMARY: 更新系メソッド
		//
		bool update(
			boost::optional<int_type> left = boost::none,
			boost::optional<int_type> top = boost::none,
			boost::optional<byte_type> opacity = boost::none
			);
		bool updateToCenter(
			boost::optional<byte_type> opacity = boost::none
			);
		//
		//	SUMMARY: プロパテイ系メソッド
		//
		bool getVisible() const;
		void setVisible(bool v);
		bool getTopMost() const;
		void setTopMost(bool v);
		size_type getImageWidth() const;
		size_type getImageHeight() const;
		int_type getLeft() const;
		int_type getTop() const;
		size_type getWidth() const;
		size_type getHeight() const;
		byte_type getOpacity() const;
		string_type const& getIdentity() const;
		bool initialized() const;
	};

	//
	// LayeredSplashWindow
	//
	class LayeredSplashWindow
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeLayeredSplashWindow> instance_;
	public:
		LayeredSplashWindow();
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
		void initialize();
		void initialize(tTJSVariantString const* identity);
		void uninitialize();
		//
		//	SUMMARY: 読み込み系メソッド
		//
		bool loadImages(tTJSVariantString const* storage);
		bool loadImagesFromOctet(tTJSVariantOctet const* source);
		//
		//	SUMMARY: 更新系メソッド
		//
		bool update(
			boost::optional<tTVInteger> left = boost::none,
			boost::optional<tTVInteger> top = boost::none,
			boost::optional<tTVInteger> opacity = boost::none
			);
		bool updateToCenter(
			boost::optional<tTVInteger> opacity = boost::none
			);
		//
		//	SUMMARY: プロパテイ系メソッド
		//
		bool getVisible() const;
		void setVisible(bool v);
		bool getTopMost() const;
		void setTopMost(bool v);
		tTVInteger getImageWidth() const;
		tTVInteger getImageHeight() const;
		tTVInteger getLeft() const;
		tTVInteger getTop() const;
		tTVInteger getWidth() const;
		tTVInteger getHeight() const;
		tTVInteger getOpacity() const;
		tTJSString getIdentity() const;
		bool initialized() const;
	};
}	// namespace ktl

#endif	// #ifndef SRC_SPLASH_WINDOW_LAYERED_SPLASH_WINDOW_DECL_HPP
