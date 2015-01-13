/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_TJS_ENGINE_TJS_ENGINE_DECL_HPP
#define SRC_TJS_ENGINE_TJS_ENGINE_DECL_HPP

#include <string>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>
#include <ktl/tjs_adapter.hpp>

namespace ktl {
	//
	// NativeTJSEngine
	//
	class NativeTJSEngine {
		boost::shared_ptr<TJSAdapter> tjs_;
	public:
		NativeTJSEngine();
	public:
		//
		//	SUMMARY: プロパティ系メソッド
		//
		sprig::krkr::tjs::object_type global() const;
	};

	//
	// TJSEngine
	//
	class TJSEngine
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeTJSEngine> instance_;
	public:
		TJSEngine();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: プロパティ系メソッド
		//
		sprig::krkr::tjs::object_type global() const;
	};
}	// namespace ktl

#endif	// #ifndef SRC_TJS_ENGINE_TJS_ENGINE_DECL_HPP
