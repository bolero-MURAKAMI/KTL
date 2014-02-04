/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef KTL_SERIALIZATION_HPP
#define KTL_SERIALIZATION_HPP

#include <sprig/config/config.hpp>

#ifdef SPRIG_USING_PRAGMA_ONCE
#	pragma once
#endif	// #ifdef SPRIG_USING_PRAGMA_ONCE

#include <ktl/config.hpp>

#include <sprig/external/tp_stub.hpp>

namespace ktl {
	//
	// ISerializer
	//
	class ISerializer {
	public:
		typedef tTJSString key_type;
		typedef tTVInteger version_type;
	public:
		virtual ~ISerializer() {}
		virtual void save(
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param
			) const
			= 0;
		virtual void load(
			tTJSVariant* result,
			tjs_int numparams,
			tTJSVariant** param
			) const
			= 0;
		virtual version_type version() const = 0;
		virtual key_type identity() const = 0;
	};

	//
	// ISerializationStack
	//
	class ISerializationStack {
	public:
		typedef ISerializer::key_type key_type;
		typedef ISerializer::version_type version_type;
	public:
		virtual ~ISerializationStack() {}
		//	SUMMARY: 要素の値の設定を試みる
		//		: シリアライザの呼出が必要でないならば、真を返す
		//		: シリアライザの呼出が必要であるならば、偽を返す
		//		: シリアライズ中に呼ばれる
		virtual bool set(
			key_type const& key,
			tTJSVariant const* v,
			key_type const* ser	/* optional */
			)
			= 0;

		//	SUMMARY: 新しいスタックを積む
		//		: シリアライズ中に呼ばれる
		virtual void pushOnSave(
			key_type const& key,
			tTJSVariant const* v,
			key_type const& ser,
			version_type const ver,
			key_type const& identity
			)
			= 0;

		//	SUMMARY: スタックを戻す
		//		: シリアライズ中に呼ばれる
		virtual void popOnSave() = 0;

		//	SUMMARY: シリアライザの呼出が必要かどうか
		//		: 必要でないならば、真を返す
		//		: 必要であるならば、偽を返す
		//		: デシリアライズ中に呼ばれる
		virtual bool isLeaf(key_type const& key) = 0;

		//	SUMMARY: 要素の値を直接取得する
		//		: デシリアライズ中に呼ばれる
		virtual void get(key_type const& key, tTJSVariant* v) = 0;

		//	SUMMARY: 既存のスタックを積む
		//		: デシリアライズ中に呼ばれる
		virtual void pushOnLoad(key_type const& key) = 0;

		//	SUMMARY: スタックを戻す
		//		: シリアライザが返したインスタンスを登録する
		//		: デシリアライズ中に呼ばれる
		virtual void popOnLoad(tTJSVariant const* v) = 0;

		//	SUMMARY: 現在のスタックから、要素のシリアライザキーを返す
		//		: デシリアライズ中に呼ばれる
		virtual key_type serializer(key_type const& key) = 0;

		//	SUMMARY: 現在のスタックから、要素のシリアライザバージョンを返す
		//		: デシリアライズ中に呼ばれる
		virtual version_type version(key_type const& key) = 0;

		//	SUMMARY: 現在のスタックから、要素のシリアライザIDを返す
		//		: デシリアライズ中に呼ばれる
		virtual key_type identity(key_type const& key) = 0;
	};
}	// namespace ktl

#endif	// #ifndef KTL_SERIALIZATION_HPP
