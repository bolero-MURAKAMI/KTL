#pragma once

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/scoped_ptr.hpp>
#include <sprig/external/tp_stub.hpp>

#include "WindowCommon.hpp"

namespace ktl {
	//
	// NativeSplashWindow
	//
	class NativeSplashWindow {
	public:
		typedef Splash impl_type;
		typedef boost::scoped_ptr<impl_type> impl_instance_type;
		typedef impl_type::size_type size_type;
		typedef impl_type::int_type int_type;
		typedef impl_type::string_type string_type;
		typedef impl_type::byte_type byte_type;
	private:
		impl_instance_type impl_;
	public:
		NativeSplashWindow();
	public:
		//
		//	SUMMARY: 初期化系メソッド
		//
		void initialize(size_type width, size_type height);
		void initialize(size_type width, size_type height, string_type const& identity);
		void uninitialize();
		//
		//	SUMMARY: 読み込み系メソッド
		//
		bool loadImages(tTJSString const& storage);
		bool loadImagesFromOctet(tTJSVariantOctet const* source);
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
		string_type const& getIdentity() const;
		bool initialized() const;
	};

	//
	// SplashWindow
	//
	class SplashWindow
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeSplashWindow> instance_;
	public:
		SplashWindow();
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
		void initialize(tTVInteger width, tTVInteger height);
		void initialize(tTVInteger width, tTVInteger height, tTJSVariantString const* identity);
		void uninitialize();
		//
		//	SUMMARY: 読み込み系メソッド
		//
		bool loadImages(tTJSVariantString const* storage);
		bool loadImagesFromOctet(tTJSVariantOctet const* source);
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
		tTJSString getIdentity() const;
		bool initialized() const;
	};
}	// namespace ktl
