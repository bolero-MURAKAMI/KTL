/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#define KTL_LIB_BUILD

#include <sprig/config/config.hpp>

#include <boost/smart_ptr/intrusive_ptr.hpp>
#include <boost/noncopyable.hpp>

#include <tjs2/tjs.h>
#include <ktl/tjs_adapter.hpp>

namespace {
	//
	// intrusive_ptr_add_ref
	// intrusive_ptr_release
	//
	// overload: (tTJS*)
	//
	SPRIG_INLINE void intrusive_ptr_add_ref(tTJS* p) {
		p->AddRef();
	}
	SPRIG_INLINE void intrusive_ptr_release(tTJS* p) {
		p->Release();
	}
}	// anonymous-namespace

namespace ktl {
	//
	// TJSAdapterImpl
	//
	class TJSAdapterImpl
		: private boost::noncopyable
	{
	private:
		boost::intrusive_ptr<tTJS> tjs_;
	public:
		TJSAdapterImpl()
			: tjs_(new tTJS(), false)
		{}
		~TJSAdapterImpl() throw() {
			tjs_->Shutdown();
		}
		tTJS const* operator->() const {
			return tjs_.get();
		}
		tTJS* operator->() {
			return tjs_.get();
		}
	};
	//
	// TJSAdapter
	//
	TJSAdapter::TJSAdapter()
		: impl_(new TJSAdapterImpl())
	{}
	TJSAdapter::~TJSAdapter() {}
	void/*iTJSDispatch2*/* TJSAdapter::GetGlobal() {
		return (*impl_)->GetGlobal();
	}
	void/*iTJSDispatch2*/* TJSAdapter::GetGlobalNoAddRef() const {
		return (*impl_)->GetGlobalNoAddRef();
	}
}	// namespace ktl
