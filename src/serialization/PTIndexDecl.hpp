#pragma once

#include <sprig/external/tp_stub.hpp>
#include <ktl/pt_index_table.hpp>

namespace ktl {
	//
	// PTIndex
	//
	class PTIndex
		: public tTJSDispatch
	{
	public:
		typedef PTIndexTable<tTJSString> index_table_type;
		typedef index_table_type::string_type string_type;
		typedef index_table_type::table_type table_type;
		typedef tjs_int size_type;
	private:
		index_table_type index_table_;
	public:
		void reserve(size_type size);
		void resize(size_type size);
	public:
		tjs_error TJS_INTF_METHOD FuncCall(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD PropGetByNum(
			tjs_uint32 flag,
			tjs_int num,
			tTJSVariant* result,
			iTJSDispatch2* objthis
			);
	};
}	// namespace ktl
