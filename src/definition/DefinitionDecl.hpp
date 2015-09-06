/*=============================================================================
 Copyright (c) 2010-2015 Bolero MURAKAMI
 https://github.com/bolero-MURAKAMI/KTL

 Distributed under the Boost Software License, Version 1.0. (See accompanying
 file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_DEFINITION_DEFINITION_DECL_HPP
#define SRC_DEFINITION_DEFINITION_DECL_HPP

#include <string>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// DefinitionClass
	//
	class DefinitionClass : public tTJSDispatch {
	private:
		std::basic_string<tjs_char> name_;
		sprig::krkr::tjs::object_type base_;
		sprig::krkr::tjs::object_type func_;
		sprig::krkr::tjs::object_type propGetByNum_;
		sprig::krkr::tjs::object_type propSetByNum_;
	private:
		tjs_error callPropGetByNum(
			tjs_uint32 flag,
			tjs_int num,
			tTJSVariant* result,
			iTJSDispatch2* objthis
			);
	public:
		void name(std::basic_string<tjs_char> const& s);
		std::basic_string<tjs_char> const& name() const;
		void base(sprig::krkr::tjs::object_type const& obj);
		sprig::krkr::tjs::object_type base() const;
		void func(sprig::krkr::tjs::object_type const& obj);
		sprig::krkr::tjs::object_type func() const;
		void propGetByNum(sprig::krkr::tjs::object_type const& obj);
		sprig::krkr::tjs::object_type propGetByNum() const;
		void propSetByNum(sprig::krkr::tjs::object_type const& obj);
		sprig::krkr::tjs::object_type propSetByNum() const;
	public:
		DefinitionClass();
		explicit DefinitionClass(DefinitionClass const& dc, iTJSDispatch2* base_obj = 0);
		tjs_error TJS_INTF_METHOD FuncCall(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD FuncCallByNum(
			tjs_uint32 flag,
			tjs_int num,
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD PropGet(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant* result,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD PropGetByNum(
			tjs_uint32 flag,
			tjs_int num,
			tTJSVariant* result,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD PropSet(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant const* param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD PropSetByNum(
			tjs_uint32 flag,
			tjs_int num,
			tTJSVariant const* param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD GetCount(
			tjs_int* result,
			tjs_char const* membername,
			tjs_uint32* hint,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD GetCountByNum(
			tjs_int* result,
			tjs_int num,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD PropSetByVS(
			tjs_uint32 flag,
			tTJSVariantString* membername,
			tTJSVariant const* param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD EnumMembers(
			tjs_uint32 flag,
			tTJSVariantClosure* callback,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD DeleteMember(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD DeleteMemberByNum(
			tjs_uint32 flag,
			tjs_int num,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD Invalidate(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD InvalidateByNum(
			tjs_uint32 flag,
			tjs_int num,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD IsValid(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD IsValidByNum(
			tjs_uint32 flag,
			tjs_int num,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD CreateNew(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			iTJSDispatch2** result,
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD CreateNewByNum(
			tjs_uint32 flag,
			tjs_int num,
			iTJSDispatch2** result,
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD IsInstanceOf(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tjs_char const* classname,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD IsInstanceOfByNum(
			tjs_uint32 flag,
			tjs_int num,
			tjs_char const* classname,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD Operation(
			tjs_uint32 flag,
			tjs_char const* membername,
			tjs_uint32* hint,
			tTJSVariant* result,
			tTJSVariant const* param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD OperationByNum(
			tjs_uint32 flag,
			tjs_int num,
			tTJSVariant* result,
			tTJSVariant const* param,
			iTJSDispatch2* objthis
			);
		tjs_error TJS_INTF_METHOD NativeInstanceSupport(
			tjs_uint32 flag,
			tjs_int32 classid,
			iTJSNativeInstance** pointer
			);
		tjs_error TJS_INTF_METHOD ClassInstanceInfo(
			tjs_uint32 flag,
			tjs_uint num,
			tTJSVariant* value
			);
	};

	//
	// NativeDefinition
	//
	class NativeDefinition {
	public:
		NativeDefinition();
	public:
		//
		//	SUMMARY: 定義系メソッド
		//
		static tTJSVariant class_();
	};

	//
	// NativeClassDefinition
	//
	class NativeClassDefinition {
	private:
		boost::intrusive_ptr<DefinitionClass> def_;
	public:
		NativeClassDefinition();
	public:
		//
		//	SUMMARY: 定義系メソッド
		//
		tTJSVariant endDefinition();
		//
		//	SUMMARY: クラス定義系メソッド
		//
		void name(tjs_char const* s);
		void extends_(iTJSDispatch2* obj);
		void funcCall(iTJSDispatch2* obj);
		void propGetByNum(iTJSDispatch2* obj);
		void propSetByNum(iTJSDispatch2* obj);
	};

	//
	// Definition
	//
	class Definition
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeDefinition> instance_;
	public:
		Definition();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: 定義系メソッド
		//
		static tTJSVariant class_();
	};

	//
	// ClassDefinition
	//
	class ClassDefinition
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeClassDefinition> instance_;
	public:
		ClassDefinition();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: 定義系メソッド
		//
		tTJSVariant endDefinition();
		//
		//	SUMMARY: クラス定義系メソッド
		//
		void name(tTJSVariantString const* s);
		void extends_(iTJSDispatch2* obj);
		void funcCall(iTJSDispatch2* obj);
		void propGetByNum(iTJSDispatch2* obj);
		void propSetByNum(iTJSDispatch2* obj);
	};
}	// namespace ktl

#endif	// #ifndef SRC_DEFINITION_DEFINITION_DECL_HPP
