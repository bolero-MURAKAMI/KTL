/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "Geometry.hpp"

namespace ktl {
	//
	// CreateNativeClassGeometry
	//
#define TJS_NATIVE_CLASSID_NAME GeometryClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassGeometry, Geometry, Geometry)
	//
	//	SUMMARY: ジオメトリモデル系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(gmfPoint, Geometry::gmfPoint);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(gmfSegment, Geometry::gmfSegment);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(gmfBox, Geometry::gmfBox);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(gmfLinestring, Geometry::gmfLinestring);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(gmfRing, Geometry::gmfRing);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(gmfPolygon, Geometry::gmfPolygon);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(gmfMultiPoint, Geometry::gmfMultiPoint);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(gmfMultiLinestring, Geometry::gmfMultiLinestring);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(gmfMultiPolygon, Geometry::gmfMultiPolygon);
	//
	//	SUMMARY: 単項系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(isValid)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::isValid(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(isValid);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(isSimple)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::isSimple(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(isSimple);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(intersects)
	{
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		if (numparams >= 4) {
			SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
			SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
			SPRIG_KRKR_RESULT_SET(
				Geometry::intersects(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_CLOSURE(2), SPRIG_KRKR_ARG_VARIANT(3))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Geometry::intersects(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(intersects);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(touches)
	{
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		if (numparams >= 4) {
			SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
			SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
			SPRIG_KRKR_RESULT_SET(
				Geometry::touches(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_CLOSURE(2), SPRIG_KRKR_ARG_VARIANT(3))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Geometry::touches(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(touches);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(length)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::length(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(length);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(perimeter)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::perimeter(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(perimeter);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(area)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::area(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(area);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(centroid)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::centroid(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(centroid);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(envelope)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::envelope(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(envelope);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(translate)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
		SPRIG_KRKR_RESULT_SET(
			Geometry::translate(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(translate);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(scale)
	{
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(3);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_NUMPARAMS_CHECK_FOR(2, 3);
		if (numparams >= 4) {
			SPRIG_KRKR_TYPE_CHECK(3, tvtObject);
			SPRIG_KRKR_RESULT_SET(
				Geometry::scale(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Geometry::scale(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(scale);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(rotate)
	{
		SPRIG_KRKR_NUMPARAMS_EASY_CHECK(3);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_NUMPARAMS_CHECK_FOR(2, 3);
		if (numparams >= 4) {
			SPRIG_KRKR_TYPE_CHECK(3, tvtObject);
			SPRIG_KRKR_RESULT_SET(
				Geometry::rotate(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2), SPRIG_KRKR_ARG_VARIANT(3))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				Geometry::rotate(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(rotate);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(matrixTransform)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(3);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
		SPRIG_KRKR_RESULT_SET(
			Geometry::matrixTransform(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_VARIANT(2))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(matrixTransform);
	//
	//	SUMMARY: 二項系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(disjoint)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::disjoint(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_CLOSURE(2), SPRIG_KRKR_ARG_VARIANT(3))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(disjoint);
	// intersects
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(coveredBy)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::coveredBy(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_CLOSURE(2), SPRIG_KRKR_ARG_VARIANT(3))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(coveredBy);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(within)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::within(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_CLOSURE(2), SPRIG_KRKR_ARG_VARIANT(3))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(within);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(equals)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::equals(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_CLOSURE(2), SPRIG_KRKR_ARG_VARIANT(3))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(equals);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(overlaps)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::overlaps(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_CLOSURE(2), SPRIG_KRKR_ARG_VARIANT(3))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(overlaps);
	// touches
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(crosses)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::crosses(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_CLOSURE(2), SPRIG_KRKR_ARG_VARIANT(3))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(crosses);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(distance)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(4);
		SPRIG_KRKR_TYPE_CHECK(0, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(1, tvtInteger);
		SPRIG_KRKR_TYPE_CHECK(2, tvtObject);
		SPRIG_KRKR_TYPE_CHECK(3, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			Geometry::distance(SPRIG_KRKR_ARG_CLOSURE(0), SPRIG_KRKR_ARG_VARIANT(1), SPRIG_KRKR_ARG_CLOSURE(2), SPRIG_KRKR_ARG_VARIANT(3))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(distance);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_geometry
// unregist_ktl_geometry
//
static void regist_ktl_geometry() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Geometry:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddNewMember(global, SPRIG_KRKR_TJS_W("Geometry"), ktl::CreateNativeClassGeometry());
	}
}
static void unregist_ktl_geometry() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:Geometry:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global, SPRIG_KRKR_TJS_W("Geometry"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_geometry);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_geometry);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:Geometry:information"));
