#ifndef KTL_TJS_ADAPTER_HPP
#define KTL_TJS_ADAPTER_HPP

#include <sprig/config/config.hpp>

#ifdef SPRIG_USING_PRAGMA_ONCE
#	pragma once
#endif	// #ifdef SPRIG_USING_PRAGMA_ONCE

#include <ktl/config.hpp>
#include <ktl/config/lib/ktl_tjs_adapter.hpp>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace ktl {
	//
	// TJSAdapterImpl
	//
	class TJSAdapterImpl;
	//
	// TJSAdapter
	//
	class TJSAdapter
		: private boost::noncopyable
	{
	private:
		boost::shared_ptr<TJSAdapterImpl> impl_;
	public:
		TJSAdapter();
		~TJSAdapter() throw();
		void/*iTJSDispatch2*/* GetGlobal();
		void/*iTJSDispatch2*/* GetGlobalNoAddRef() const;
	};
}	// namespace ktl

#endif	// #ifndef KTL_TJS_ADAPTER_HPP
