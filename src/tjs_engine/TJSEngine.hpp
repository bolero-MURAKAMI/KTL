/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_TJS_ENGINE_TJS_ENGINE_HPP
#define SRC_TJS_ENGINE_TJS_ENGINE_HPP

#include <boost/smart_ptr/make_shared.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "TJSEngineDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:TJSEngine:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:TJSEngine:error")

namespace ktl {
	//
	// NativeTJSEngine
	//
	NativeTJSEngine::NativeTJSEngine()
		: tjs_(boost::make_shared<TJSAdapter>())
	{}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type NativeTJSEngine::global() const {
		sprig::krkr::tjs::object_type obj(
			reinterpret_cast<iTJSDispatch2*>(tjs_->GetGlobalNoAddRef())
			);
		return obj;
	}

	//
	// TJSEngine
	//
	TJSEngine::TJSEngine() {}
	tjs_error TJS_INTF_METHOD TJSEngine::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("TJSEngine::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeTJSEngine>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD TJSEngine::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("TJSEngine::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: プロパティ系メソッド
	//
	KTL_INLINE sprig::krkr::tjs::object_type TJSEngine::global() const {
		return instance_->global();
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_TJS_ENGINE_TJS_ENGINE_HPP
