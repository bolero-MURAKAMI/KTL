/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_GEOMETRY_GEOMETRY_HPP
#define SRC_GEOMETRY_GEOMETRY_HPP

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/type_traits/common_type.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/is_valid.hpp>
#include <boost/geometry/algorithms/is_simple.hpp>
#include <boost/geometry/algorithms/intersects.hpp>
#include <boost/geometry/algorithms/touches.hpp>
#include <boost/geometry/algorithms/length.hpp>
#include <boost/geometry/algorithms/perimeter.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/geometry/algorithms/within.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/get_pointer.hpp>
#include <sprig/geometry/algorithms/is_not_implemented.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "GeometryDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:Geometry:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:Geometry:error")

namespace ktl {
	template<typename Result, typename Derived>
	class NativeGeometry::UnaryFuncBase {
	public:
		typedef Result result_type;
	protected:
		template<typename G>
		class default_caller {
		public:
			static result_type call(G const&) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("このジオメトリに対する操作は実装されていません"),
					sprig::krkr::bad_argument
					);
				return result_type();
			}
		};
	public:
		template<typename G>
		result_type operator()(G const& g) const {
			return Derived::template caller<G>::call(g);
		}
	};
	template<typename Result, typename Derived>
	class NativeGeometry::BinaryFuncBase {
	public:
		typedef Result result_type;
	protected:
		template<typename G0, typename G1>
		class default_caller {
		public:
			static result_type call(G0 const&, G1 const&) {
				KTL_ERROR(
					KTL_ERROR_SECTION,
					SPRIG_KRKR_TJS_W("このジオメトリに対する操作は実装されていません"),
					sprig::krkr::bad_argument
					);
				return result_type();
			}
		};
	public:
		template<typename G0, typename G1>
		result_type operator()(G0 const& g0, G1 const& g1) const {
			return Derived::template caller<G0, G1>::call(g0, g1);
		}
	};

	template<typename G0, typename G1>
	class NativeGeometry::DisjointDispatch
		: public boost::geometry::dispatch::disjoint<G0, G1>
	{};
	template<typename G0>
	class NativeGeometry::DisjointDispatch<G0, NativeGeometry::multi_point_type>
		: public boost::geometry::not_implemented<G0, multi_point_type>
	{};
	template<typename G1>
	class NativeGeometry::DisjointDispatch<NativeGeometry::multi_point_type, G1>
		: public boost::geometry::not_implemented<multi_point_type, G1>
	{};
	template<>
	class NativeGeometry::DisjointDispatch<NativeGeometry::multi_point_type, NativeGeometry::multi_point_type>
		: public boost::geometry::not_implemented<multi_point_type, multi_point_type>
	{};

#define KTL_GEOMETRY_UNARY_FUNC_DEF(NAME, RESULT, FUNC, DISP) \
	class NativeGeometry::NAME \
		: public UnaryFuncBase<RESULT, NAME> \
	{ \
		friend class UnaryFuncBase<RESULT, NAME>; \
	private: \
		template<typename G, bool = sprig::geometry::is_not_implemented<DISP<G> >::value> \
		class caller \
			: public UnaryFuncBase<RESULT, NAME>::template default_caller<G> \
		{}; \
		template<typename G> \
		class caller<G, false> { \
		public: \
			static RESULT call(G const& g) { \
				return FUNC(g); \
			} \
		}; \
	}
	KTL_GEOMETRY_UNARY_FUNC_DEF(IsValidFunc, bool, boost::geometry::is_valid, boost::geometry::dispatch::is_valid);
	KTL_GEOMETRY_UNARY_FUNC_DEF(IsSimpleFunc, bool, boost::geometry::is_simple, boost::geometry::dispatch::is_simple);
	//KTL_GEOMETRY_UNARY_FUNC_DEF(Intersects1Func, bool, boost::geometry::intersects, boost::common_type);
	//KTL_GEOMETRY_UNARY_FUNC_DEF(Touches1Func, bool, boost::geometry::touches, boost::common_type);
	KTL_GEOMETRY_UNARY_FUNC_DEF(LengthFunc, element_type, boost::geometry::length, boost::geometry::dispatch::length);
	KTL_GEOMETRY_UNARY_FUNC_DEF(PerimeterFunc, element_type, boost::geometry::perimeter, boost::geometry::dispatch::perimeter);
	KTL_GEOMETRY_UNARY_FUNC_DEF(AreaFunc, element_type, boost::geometry::area, boost::geometry::dispatch::area);
	KTL_GEOMETRY_UNARY_FUNC_DEF(CentroidFunc, point_type, boost::geometry::return_centroid<point_type>, boost::geometry::dispatch::centroid);

#define KTL_GEOMETRY_BINARY_FUNC_DEF(NAME, RESULT, FUNC, DISP) \
	class NativeGeometry::NAME \
		: public BinaryFuncBase<RESULT, NAME> \
	{ \
		friend class BinaryFuncBase<RESULT, NAME>; \
	private: \
		template<typename G0, typename G1, bool = sprig::geometry::is_not_implemented<DISP<G0, G1> >::value> \
		class caller \
			: public BinaryFuncBase<RESULT, NAME>::template default_caller<G0, G1> \
		{}; \
		template<typename G0, typename G1> \
		class caller<G0, G1, false> { \
		public: \
			static RESULT call(G0 const& g0, G1 const& g1) { \
				return FUNC(g0, g1); \
			} \
		}; \
	}
	KTL_GEOMETRY_BINARY_FUNC_DEF(DisjointFunc, bool, boost::geometry::disjoint, DisjointDispatch);
	KTL_GEOMETRY_BINARY_FUNC_DEF(CoveredByFunc, bool, boost::geometry::covered_by, boost::geometry::dispatch::covered_by);
	KTL_GEOMETRY_BINARY_FUNC_DEF(WithinFunc, bool, boost::geometry::within, boost::geometry::dispatch::within);
	KTL_GEOMETRY_BINARY_FUNC_DEF(EqualsFunc, bool, boost::geometry::equals, boost::geometry::dispatch::equals);
	KTL_GEOMETRY_BINARY_FUNC_DEF(OverlapsFunc, bool, boost::geometry::overlaps, boost::geometry::dispatch::overlaps);
	KTL_GEOMETRY_BINARY_FUNC_DEF(TouchesFunc, bool, boost::geometry::touches, boost::geometry::dispatch::touches);
	//KTL_GEOMETRY_BINARY_FUNC_DEF(CrossesFunc, bool, boost::geometry::crosses, boost::geometry::dispatch::crosses);
	//KTL_GEOMETRY_BINARY_FUNC_DEF(IntersectsFunc, bool, boost::geometry::intersects, boost::geometry::dispatch::intersects);
	KTL_GEOMETRY_BINARY_FUNC_DEF(DistanceFunc, element_type, boost::geometry::distance, boost::geometry::dispatch::distance);

	KTL_INLINE NativeGeometry::point_type NativeGeometry::to_point(tTJSVariantClosure const& g) {
		return point_type(
			sprig::krkr::tjs::GetPropValue<tTVReal>(0, g),
			sprig::krkr::tjs::GetPropValue<tTVReal>(1, g)
			);
	}
	KTL_INLINE NativeGeometry::segment_type NativeGeometry::to_segment(tTJSVariantClosure const& g) {
		return segment_type(
			to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(0, g).AsObjectClosureNoAddRef()),
			to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(1, g).AsObjectClosureNoAddRef())
			);
	}
	KTL_INLINE NativeGeometry::box_type NativeGeometry::to_box(tTJSVariantClosure const& g) {
		return box_type(
			to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(0, g).AsObjectClosureNoAddRef()),
			to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(1, g).AsObjectClosureNoAddRef())
			);
	}
	KTL_INLINE NativeGeometry::linestring_type NativeGeometry::to_linestring(tTJSVariantClosure const& g) {
		linestring_type result;
		tjs_int count = sprig::krkr::tjs::GetPropValue<tjs_int>(g, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			result.push_back(
				to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(i, g).AsObjectClosureNoAddRef())
				);
		}
		return result;
	}
	KTL_INLINE NativeGeometry::ring_type NativeGeometry::to_ring(tTJSVariantClosure const& g) {
		ring_type result;
		tjs_int count = sprig::krkr::tjs::GetPropValue<tjs_int>(g, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			result.push_back(
				to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(i, g).AsObjectClosureNoAddRef())
				);
		}
		return result;
	}
	KTL_INLINE NativeGeometry::polygon_type NativeGeometry::to_polygon(tTJSVariantClosure const& g) {
		polygon_type result;
		boost::geometry::exterior_ring(result)
			= to_ring(sprig::krkr::tjs::GetPropValue<tTJSVariant>(0, g).AsObjectClosureNoAddRef())
			;
		{
			tTJSVariantClosure inners = sprig::krkr::tjs::GetPropValue<tTJSVariant>(1, g).AsObjectClosureNoAddRef();
			tjs_int inners_count = sprig::krkr::tjs::GetPropValue<tjs_int>(inners, SPRIG_KRKR_TJS_W("count"));
			for (tjs_int i = 0; i != inners_count; ++i) {
				boost::geometry::interior_rings(result).push_back(
					to_ring(sprig::krkr::tjs::GetPropValue<tTJSVariant>(i, inners).AsObjectClosureNoAddRef())
					);
			}
		}
		return result;
	}
	KTL_INLINE NativeGeometry::multi_point_type NativeGeometry::to_multi_point(tTJSVariantClosure const& g) {
		multi_point_type result;
		tjs_int count = sprig::krkr::tjs::GetPropValue<tjs_int>(g, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			result.push_back(
				to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(i, g).AsObjectClosureNoAddRef())
				);
		}
		return result;
	}
	KTL_INLINE NativeGeometry::multi_linestring_type NativeGeometry::to_multi_linestring(tTJSVariantClosure const& g) {
		multi_linestring_type result;
		tjs_int count = sprig::krkr::tjs::GetPropValue<tjs_int>(g, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			result.push_back(
				to_linestring(sprig::krkr::tjs::GetPropValue<tTJSVariant>(i, g).AsObjectClosureNoAddRef())
				);
		}
		return result;
	}
	KTL_INLINE NativeGeometry::multi_polygon_type NativeGeometry::to_multi_polygon(tTJSVariantClosure const& g) {
		multi_polygon_type result;
		tjs_int count = sprig::krkr::tjs::GetPropValue<tjs_int>(g, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			result.push_back(
				to_polygon(sprig::krkr::tjs::GetPropValue<tTJSVariant>(i, g).AsObjectClosureNoAddRef())
				);
		}
		return result;
	}
	KTL_INLINE NativeGeometry::translate_type NativeGeometry::to_translate(tTJSVariant const& s) {
		tTJSVariantClosure c = s.AsObjectClosureNoAddRef();
		return translate_type(
			sprig::krkr::tjs::GetPropValue<tTVReal>(0, c),
			sprig::krkr::tjs::GetPropValue<tTVReal>(1, c)
			);
	}
	KTL_INLINE NativeGeometry::translate_type NativeGeometry::to_negate_translate(tTJSVariant const& s) {
		tTJSVariantClosure c = s.AsObjectClosureNoAddRef();
		return translate_type(
			-sprig::krkr::tjs::GetPropValue<tTVReal>(0, c),
			-sprig::krkr::tjs::GetPropValue<tTVReal>(1, c)
			);
	}
	KTL_INLINE NativeGeometry::scale_type NativeGeometry::to_scale(tTJSVariant const& s) {
		if (s.Type() == tvtObject) {
			tTJSVariantClosure c = s.AsObjectClosureNoAddRef();
			return scale_type(
				sprig::krkr::tjs::GetPropValue<tTVReal>(0, c),
				sprig::krkr::tjs::GetPropValue<tTVReal>(1, c)
				);
		}
		return scale_type(
				static_cast<tTVReal>(s)
			);
	}
	KTL_INLINE NativeGeometry::rotate_type NativeGeometry::to_rotate(tTJSVariant const& s) {
		return rotate_type(
			static_cast<tTVReal>(s)
			);
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(point_type const& g) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = result_type(result_obj, false);
		}
		sprig::krkr::tjs::SetPropValue(tTJSVariant(g.x()), 0, sprig::get_pointer(result));
		sprig::krkr::tjs::SetPropValue(tTJSVariant(g.y()), 1, sprig::get_pointer(result));
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(segment_type const& g) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = result_type(result_obj, false);
		}
		sprig::krkr::tjs::SetPropValue(to_variant(g.first), 0, sprig::get_pointer(result));
		sprig::krkr::tjs::SetPropValue(to_variant(g.second), 1, sprig::get_pointer(result));
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(box_type const& g) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = result_type(result_obj, false);
		}
		sprig::krkr::tjs::SetPropValue(to_variant(g.min_corner()), 0, sprig::get_pointer(result));
		sprig::krkr::tjs::SetPropValue(to_variant(g.max_corner()), 1, sprig::get_pointer(result));
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(linestring_type const& g) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = result_type(result_obj, false);
		}
		for (tjs_int i = 0, size = g.size(); i != size; ++i) {
			sprig::krkr::tjs::SetPropValue(to_variant(g.at(i)), i, sprig::get_pointer(result));
		}
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(ring_type const& g) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = result_type(result_obj, false);
		}
		for (tjs_int i = 0, size = g.size(); i != size; ++i) {
			sprig::krkr::tjs::SetPropValue(to_variant(g.at(i)), i, sprig::get_pointer(result));
		}
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(polygon_type const& g) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = result_type(result_obj, false);
		}
		sprig::krkr::tjs::SetPropValue(to_variant(boost::geometry::exterior_ring(g)), 0, sprig::get_pointer(result));
		{
			result_type inners;
			{
				iTJSDispatch2* inners_obj = 0;
				sprig::krkr::tjs::CreateNewObject(
					sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
					&inners_obj, 0, 0, 0
					);
				inners = result_type(inners_obj, false);
			}
			for (tjs_int i = 0, size = boost::geometry::interior_rings(g).size(); i != size; ++i) {
				sprig::krkr::tjs::SetPropValue(to_variant(boost::geometry::interior_rings(g).at(i)), i, sprig::get_pointer(inners));
			}
			sprig::krkr::tjs::SetPropValue(tTJSVariant(inners.get(), inners.get()), 1, sprig::get_pointer(result));
		}
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(multi_point_type const& g) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = result_type(result_obj, false);
		}
		for (tjs_int i = 0, size = g.size(); i != size; ++i) {
			sprig::krkr::tjs::SetPropValue(to_variant(g.at(i)), i, sprig::get_pointer(result));
		}
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(multi_linestring_type const& g) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = result_type(result_obj, false);
		}
		for (tjs_int i = 0, size = g.size(); i != size; ++i) {
			sprig::krkr::tjs::SetPropValue(to_variant(g.at(i)), i, sprig::get_pointer(result));
		}
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(multi_polygon_type const& g) {
		typedef sprig::krkr::tjs::object_type result_type;
		result_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = result_type(result_obj, false);
		}
		for (tjs_int i = 0, size = g.size(); i != size; ++i) {
			sprig::krkr::tjs::SetPropValue(to_variant(g.at(i)), i, sprig::get_pointer(result));
		}
		return tTJSVariant(result.get(), result.get());
	}
	template<typename F>
	KTL_INLINE typename F::result_type NativeGeometry::dispatch(F fn, tTJSVariantClosure const& g, flag_type f) {
		typedef typename F::result_type result_type;
		result_type result = result_type();
		switch (f) {
		case gmfPoint:
			result = fn(to_point(g));
			break;
		case gmfSegment:
			result = fn(to_segment(g));
			break;
		case gmfBox:
			result = fn(to_box(g));
			break;
		case gmfLinestring:
			result = fn(to_linestring(g));
			break;
		case gmfRing:
			result = fn(to_ring(g));
			break;
		case gmfPolygon:
			result = fn(to_polygon(g));
			break;
		case gmfMultiPoint:
			result = fn(to_multi_point(g));
			break;
		case gmfMultiLinestring:
			result = fn(to_multi_linestring(g));
			break;
		case gmfMultiPolygon:
			result = fn(to_multi_polygon(g));
			break;
		}
		return result;
	}
	template<typename F>
	KTL_INLINE typename F::result_type NativeGeometry::dispatch(F fn, tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, flag_type f0, flag_type f1) {
		typedef typename F::result_type result_type;
		result_type result = result_type();
		switch (f0) {
		case gmfPoint:
			switch (f1) {
			case gmfPoint:
				result = fn(to_point(g0), to_point(g1));
				break;
			case gmfSegment:
				result = fn(to_point(g0), to_segment(g1));
				break;
			case gmfBox:
				result = fn(to_point(g0), to_box(g1));
				break;
			case gmfLinestring:
				result = fn(to_point(g0), to_linestring(g1));
				break;
			case gmfRing:
				result = fn(to_point(g0), to_ring(g1));
				break;
			case gmfPolygon:
				result = fn(to_point(g0), to_polygon(g1));
				break;
			case gmfMultiPoint:
				result = fn(to_point(g0), to_multi_point(g1));
				break;
			case gmfMultiLinestring:
				result = fn(to_point(g0), to_multi_linestring(g1));
				break;
			case gmfMultiPolygon:
				result = fn(to_point(g0), to_multi_polygon(g1));
				break;
			}
			break;
		case gmfSegment:
			switch (f1) {
			case gmfPoint:
				result = fn(to_segment(g0), to_point(g1));
				break;
			case gmfSegment:
				result = fn(to_segment(g0), to_segment(g1));
				break;
			case gmfBox:
				result = fn(to_segment(g0), to_box(g1));
				break;
			case gmfLinestring:
				result = fn(to_segment(g0), to_linestring(g1));
				break;
			case gmfRing:
				result = fn(to_segment(g0), to_ring(g1));
				break;
			case gmfPolygon:
				result = fn(to_segment(g0), to_polygon(g1));
				break;
			case gmfMultiPoint:
				result = fn(to_segment(g0), to_multi_point(g1));
				break;
			case gmfMultiLinestring:
				result = fn(to_segment(g0), to_multi_linestring(g1));
				break;
			case gmfMultiPolygon:
				result = fn(to_segment(g0), to_multi_polygon(g1));
				break;
			}
			break;
		case gmfBox:
			switch (f1) {
			case gmfPoint:
				result = fn(to_box(g0), to_point(g1));
				break;
			case gmfSegment:
				result = fn(to_box(g0), to_segment(g1));
				break;
			case gmfBox:
				result = fn(to_box(g0), to_box(g1));
				break;
			case gmfLinestring:
				result = fn(to_box(g0), to_linestring(g1));
				break;
			case gmfRing:
				result = fn(to_box(g0), to_ring(g1));
				break;
			case gmfPolygon:
				result = fn(to_box(g0), to_polygon(g1));
				break;
			case gmfMultiPoint:
				result = fn(to_box(g0), to_multi_point(g1));
				break;
			case gmfMultiLinestring:
				result = fn(to_box(g0), to_multi_linestring(g1));
				break;
			case gmfMultiPolygon:
				result = fn(to_box(g0), to_multi_polygon(g1));
				break;
			}
			break;
		case gmfLinestring:
			switch (f1) {
			case gmfPoint:
				result = fn(to_linestring(g0), to_point(g1));
				break;
			case gmfSegment:
				result = fn(to_linestring(g0), to_segment(g1));
				break;
			case gmfBox:
				result = fn(to_linestring(g0), to_box(g1));
				break;
			case gmfLinestring:
				result = fn(to_linestring(g0), to_linestring(g1));
				break;
			case gmfRing:
				result = fn(to_linestring(g0), to_ring(g1));
				break;
			case gmfPolygon:
				result = fn(to_linestring(g0), to_polygon(g1));
				break;
			case gmfMultiPoint:
				result = fn(to_linestring(g0), to_multi_point(g1));
				break;
			case gmfMultiLinestring:
				result = fn(to_linestring(g0), to_multi_linestring(g1));
				break;
			case gmfMultiPolygon:
				result = fn(to_linestring(g0), to_multi_polygon(g1));
				break;
			}
			break;
		case gmfRing:
			switch (f1) {
			case gmfPoint:
				result = fn(to_ring(g0), to_point(g1));
				break;
			case gmfSegment:
				result = fn(to_ring(g0), to_segment(g1));
				break;
			case gmfBox:
				result = fn(to_ring(g0), to_box(g1));
				break;
			case gmfLinestring:
				result = fn(to_ring(g0), to_linestring(g1));
				break;
			case gmfRing:
				result = fn(to_ring(g0), to_ring(g1));
				break;
			case gmfPolygon:
				result = fn(to_ring(g0), to_polygon(g1));
				break;
			case gmfMultiPoint:
				result = fn(to_ring(g0), to_multi_point(g1));
				break;
			case gmfMultiLinestring:
				result = fn(to_ring(g0), to_multi_linestring(g1));
				break;
			case gmfMultiPolygon:
				result = fn(to_ring(g0), to_multi_polygon(g1));
				break;
			}
			break;
		case gmfPolygon:
			switch (f1) {
			case gmfPoint:
				result = fn(to_polygon(g0), to_point(g1));
				break;
			case gmfSegment:
				result = fn(to_polygon(g0), to_segment(g1));
				break;
			case gmfBox:
				result = fn(to_polygon(g0), to_box(g1));
				break;
			case gmfLinestring:
				result = fn(to_polygon(g0), to_linestring(g1));
				break;
			case gmfRing:
				result = fn(to_polygon(g0), to_ring(g1));
				break;
			case gmfPolygon:
				result = fn(to_polygon(g0), to_polygon(g1));
				break;
			case gmfMultiPoint:
				result = fn(to_polygon(g0), to_multi_point(g1));
				break;
			case gmfMultiLinestring:
				result = fn(to_polygon(g0), to_multi_linestring(g1));
				break;
			case gmfMultiPolygon:
				result = fn(to_polygon(g0), to_multi_polygon(g1));
				break;
			}
			break;
		case gmfMultiPoint:
			switch (f1) {
			case gmfPoint:
				result = fn(to_multi_point(g0), to_point(g1));
				break;
			case gmfSegment:
				result = fn(to_multi_point(g0), to_segment(g1));
				break;
			case gmfBox:
				result = fn(to_multi_point(g0), to_box(g1));
				break;
			case gmfLinestring:
				result = fn(to_multi_point(g0), to_linestring(g1));
				break;
			case gmfRing:
				result = fn(to_multi_point(g0), to_ring(g1));
				break;
			case gmfPolygon:
				result = fn(to_multi_point(g0), to_polygon(g1));
				break;
			case gmfMultiPoint:
				result = fn(to_multi_point(g0), to_multi_point(g1));
				break;
			case gmfMultiLinestring:
				result = fn(to_multi_point(g0), to_multi_linestring(g1));
				break;
			case gmfMultiPolygon:
				result = fn(to_multi_point(g0), to_multi_polygon(g1));
				break;
			}
			break;
		case gmfMultiLinestring:
			switch (f1) {
			case gmfPoint:
				result = fn(to_multi_linestring(g0), to_point(g1));
				break;
			case gmfSegment:
				result = fn(to_multi_linestring(g0), to_segment(g1));
				break;
			case gmfBox:
				result = fn(to_multi_linestring(g0), to_box(g1));
				break;
			case gmfLinestring:
				result = fn(to_multi_linestring(g0), to_linestring(g1));
				break;
			case gmfRing:
				result = fn(to_multi_linestring(g0), to_ring(g1));
				break;
			case gmfPolygon:
				result = fn(to_multi_linestring(g0), to_polygon(g1));
				break;
			case gmfMultiPoint:
				result = fn(to_multi_linestring(g0), to_multi_point(g1));
				break;
			case gmfMultiLinestring:
				result = fn(to_multi_linestring(g0), to_multi_linestring(g1));
				break;
			case gmfMultiPolygon:
				result = fn(to_multi_linestring(g0), to_multi_polygon(g1));
				break;
			}
			break;
		case gmfMultiPolygon:
			switch (f1) {
			case gmfPoint:
				result = fn(to_multi_polygon(g0), to_point(g1));
				break;
			case gmfSegment:
				result = fn(to_multi_polygon(g0), to_segment(g1));
				break;
			case gmfBox:
				result = fn(to_multi_polygon(g0), to_box(g1));
				break;
			case gmfLinestring:
				result = fn(to_multi_polygon(g0), to_linestring(g1));
				break;
			case gmfRing:
				result = fn(to_multi_polygon(g0), to_ring(g1));
				break;
			case gmfPolygon:
				result = fn(to_multi_polygon(g0), to_polygon(g1));
				break;
			case gmfMultiPoint:
				result = fn(to_multi_polygon(g0), to_multi_point(g1));
				break;
			case gmfMultiLinestring:
				result = fn(to_multi_polygon(g0), to_multi_linestring(g1));
				break;
			case gmfMultiPolygon:
				result = fn(to_multi_polygon(g0), to_multi_polygon(g1));
				break;
			}
			break;
		}
		return result;
	}
	template<typename G, typename S>
	KTL_INLINE G NativeGeometry::return_transform(G const& g, S const& s) {
		G result;
		boost::geometry::transform(g, result, s);
		return result;
	}
	template<typename S>
	KTL_INLINE tTJSVariant NativeGeometry::transform(tTJSVariantClosure const& g, flag_type f, S const& s) {
		tTJSVariant result;
		switch (f) {
		case gmfPoint:
			result = to_variant(return_transform(to_point(g), s));
			break;
		case gmfSegment:
			result = to_variant(return_transform(to_segment(g), s));
			break;
		case gmfBox:
			result = to_variant(return_transform(to_box(g), s));
			break;
		case gmfLinestring:
			result = to_variant(return_transform(to_linestring(g), s));
			break;
		case gmfRing:
			result = to_variant(return_transform(to_ring(g), s));
			break;
		case gmfPolygon:
			result = to_variant(return_transform(to_polygon(g), s));
			break;
		case gmfMultiPoint:
			result = to_variant(return_transform(to_multi_point(g), s));
			break;
		case gmfMultiLinestring:
			result = to_variant(return_transform(to_multi_linestring(g), s));
			break;
		case gmfMultiPolygon:
			result = to_variant(return_transform(to_multi_polygon(g), s));
			break;
		}
		return result;
	}
	template<typename S>
	KTL_INLINE tTJSVariant NativeGeometry::transform(tTJSVariantClosure const& g, flag_type f, S const& s, tTJSVariant const& o) {
		tTJSVariant result;
		switch (f) {
		case gmfPoint:
			result = to_variant(return_transform(return_transform(return_transform(to_point(g), to_negate_translate(o)), s), to_translate(o)));
			break;
		case gmfSegment:
			result = to_variant(return_transform(return_transform(return_transform(to_segment(g), to_negate_translate(o)), s), to_translate(o)));
			break;
		case gmfBox:
			result = to_variant(return_transform(return_transform(return_transform(to_box(g), to_negate_translate(o)), s), to_translate(o)));
			break;
		case gmfLinestring:
			result = to_variant(return_transform(return_transform(return_transform(to_linestring(g), to_negate_translate(o)), s), to_translate(o)));
			break;
		case gmfRing:
			result = to_variant(return_transform(return_transform(return_transform(to_ring(g), to_negate_translate(o)), s), to_translate(o)));
			break;
		case gmfPolygon:
			result = to_variant(return_transform(return_transform(return_transform(to_polygon(g), to_negate_translate(o)), s), to_translate(o)));
			break;
		case gmfMultiPoint:
			result = to_variant(return_transform(return_transform(return_transform(to_multi_point(g), to_negate_translate(o)), s), to_translate(o)));
			break;
		case gmfMultiLinestring:
			result = to_variant(return_transform(return_transform(return_transform(to_multi_linestring(g), to_negate_translate(o)), s), to_translate(o)));
			break;
		case gmfMultiPolygon:
			result = to_variant(return_transform(return_transform(return_transform(to_multi_polygon(g), to_negate_translate(o)), s), to_translate(o)));
			break;
		}
		return result;
	}
	//
	// NativeGeometry
	//
	NativeGeometry::NativeGeometry() {}
	//
	//	SUMMARY: 単項系メソッド
	//
	KTL_INLINE bool NativeGeometry::isValid(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(IsValidFunc(), g, f);
	}
	KTL_INLINE bool NativeGeometry::isSimple(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(IsSimpleFunc(), g, f);
	}
	//KTL_INLINE bool NativeGeometry::intersects(tTJSVariantClosure const& g, flag_type f) {
	//	return dispatch(Intersects1Func(), g, f);
	//}
	//KTL_INLINE bool NativeGeometry::touches(tTJSVariantClosure const& g, flag_type f) {
	//	return dispatch(Touches1Func(), g, f);
	//}
	KTL_INLINE NativeGeometry::element_type NativeGeometry::length(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(LengthFunc(), g, f);
	}
	KTL_INLINE NativeGeometry::element_type NativeGeometry::perimeter(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(PerimeterFunc(), g, f);
	}
	KTL_INLINE NativeGeometry::element_type NativeGeometry::area(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(AreaFunc(), g, f);
	}
	KTL_INLINE tTJSVariant NativeGeometry::centroid(tTJSVariantClosure const& g, flag_type f) {
		typedef sprig::krkr::tjs::object_type result_type;
		point_type p = dispatch(CentroidFunc(), g, f);
		result_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				&result_obj, 0, 0, 0
				);
			result = result_type(result_obj, false);
		}
		sprig::krkr::tjs::SetPropValue(tTJSVariant(p.x()), 0, sprig::get_pointer(result));
		sprig::krkr::tjs::SetPropValue(tTJSVariant(p.y()), 1, sprig::get_pointer(result));
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE tTJSVariant NativeGeometry::translate(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t) {
		return transform(g, f, to_translate(t));
	}
	KTL_INLINE tTJSVariant NativeGeometry::scale(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t) {
		return transform(g, f, to_scale(t));
	}
	KTL_INLINE tTJSVariant NativeGeometry::scale(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t, tTJSVariant const& o) {
		return transform(g, f, to_scale(t), o);
	}
	KTL_INLINE tTJSVariant NativeGeometry::rotate(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t) {
		return transform(g, f, to_rotate(t));
	}
	KTL_INLINE tTJSVariant NativeGeometry::rotate(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t, tTJSVariant const& o) {
		return transform(g, f, to_rotate(t), o);
	}
	//
	//	SUMMARY: 二項系メソッド
	//
	KTL_INLINE bool NativeGeometry::disjoint(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, flag_type f0, flag_type f1) {
		return dispatch(DisjointFunc(), g0, g1, f0, f1);
	}
	KTL_INLINE bool NativeGeometry::coveredBy(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, flag_type f0, flag_type f1) {
		return dispatch(CoveredByFunc(), g0, g1, f0, f1);
	}
	KTL_INLINE bool NativeGeometry::within(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, flag_type f0, flag_type f1) {
		return dispatch(WithinFunc(), g0, g1, f0, f1);
	}
	KTL_INLINE bool NativeGeometry::equals(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, flag_type f0, flag_type f1) {
		return dispatch(EqualsFunc(), g0, g1, f0, f1);
	}
	KTL_INLINE bool NativeGeometry::overlaps(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, flag_type f0, flag_type f1) {
		return dispatch(OverlapsFunc(), g0, g1, f0, f1);
	}
	//KTL_INLINE bool NativeGeometry::touches(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, flag_type f0, flag_type f1) {
	//	return dispatch(TouchesFunc(), g0, g1, f0, f1);
	//}
	//KTL_INLINE bool NativeGeometry::crosses(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, flag_type f0, flag_type f1) {
	//	return dispatch(CrossesFunc(), g0, g1, f0, f1);
	//}
	//KTL_INLINE bool NativeGeometry::intersects(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, flag_type f0, flag_type f1) {
	//	return dispatch(IntersectsFunc(), g0, g1, f0, f1);
	//}
	KTL_INLINE NativeGeometry::element_type NativeGeometry::distance(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, flag_type f0, flag_type f1) {
		return dispatch(DistanceFunc(), g0, g1, f0, f1);
	}

	//
	// Geometry
	//
	Geometry::Geometry() {}
	tjs_error TJS_INTF_METHOD Geometry::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Geometry::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeGeometry>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD Geometry::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("Geometry::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: 単項系メソッド
	//
	KTL_INLINE bool Geometry::isValid(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::isValid(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE bool Geometry::isSimple(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::isSimple(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	//KTL_INLINE bool Geometry::intersects(tTJSVariantClosure const& g, tTVInteger f) {
	//	return NativeGeometry::intersects(
	//		g,
	//		boost::numeric_cast<NativeGeometry::flag_type>(f)
	//		);
	//}
	//KTL_INLINE bool Geometry::touches(tTJSVariantClosure const& g, tTVInteger f) {
	//	return NativeGeometry::touches(
	//		g,
	//		boost::numeric_cast<NativeGeometry::flag_type>(f)
	//		);
	//}
	KTL_INLINE tTVReal Geometry::length(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::length(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTVReal Geometry::perimeter(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::perimeter(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTVReal Geometry::area(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::area(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTJSVariant Geometry::centroid(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::centroid(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTJSVariant Geometry::translate(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t) {
		return NativeGeometry::translate(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f),
			t
			);
	}
	KTL_INLINE tTJSVariant Geometry::scale(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t) {
		return NativeGeometry::scale(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f),
			t
			);
	}
	KTL_INLINE tTJSVariant Geometry::scale(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t, tTJSVariant const& o) {
		return NativeGeometry::scale(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f),
			t, o
			);
	}
	KTL_INLINE tTJSVariant Geometry::rotate(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t) {
		return NativeGeometry::rotate(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f),
			t
			);
	}
	KTL_INLINE tTJSVariant Geometry::rotate(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t, tTJSVariant const& o) {
		return NativeGeometry::rotate(
			g,
			boost::numeric_cast<NativeGeometry::flag_type>(f),
			t, o
			);
	}
	//
	//	SUMMARY: 二項系メソッド
	//
	KTL_INLINE bool Geometry::disjoint(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, tTVInteger f0, tTVInteger f1) {
		return NativeGeometry::disjoint(
			g0, g1,
			boost::numeric_cast<NativeGeometry::flag_type>(f0),
			boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::coveredBy(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, tTVInteger f0, tTVInteger f1) {
		return NativeGeometry::coveredBy(
			g0, g1,
			boost::numeric_cast<NativeGeometry::flag_type>(f0),
			boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::within(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, tTVInteger f0, tTVInteger f1) {
		return NativeGeometry::within(
			g0, g1,
			boost::numeric_cast<NativeGeometry::flag_type>(f0),
			boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::equals(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, tTVInteger f0, tTVInteger f1) {
		return NativeGeometry::equals(
			g0, g1,
			boost::numeric_cast<NativeGeometry::flag_type>(f0),
			boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::overlaps(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, tTVInteger f0, tTVInteger f1) {
		return NativeGeometry::overlaps(
			g0, g1,
			boost::numeric_cast<NativeGeometry::flag_type>(f0),
			boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	//KTL_INLINE bool Geometry::touches(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, tTVInteger f0, tTVInteger f1) {
	//	return NativeGeometry::touches(
	//		g0, g1,
	//		boost::numeric_cast<NativeGeometry::flag_type>(f0),
	//		boost::numeric_cast<NativeGeometry::flag_type>(f1)
	//		);
	//}
	//KTL_INLINE bool Geometry::crosses(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, tTVInteger f0, tTVInteger f1) {
	//	return NativeGeometry::crosses(
	//		g0, g1,
	//		boost::numeric_cast<NativeGeometry::flag_type>(f0),
	//		boost::numeric_cast<NativeGeometry::flag_type>(f1)
	//		);
	//}
	//KTL_INLINE bool Geometry::intersects(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, tTVInteger f0, tTVInteger f1) {
	//	return NativeGeometry::intersects(
	//		g0, g1,
	//		boost::numeric_cast<NativeGeometry::flag_type>(f0),
	//		boost::numeric_cast<NativeGeometry::flag_type>(f1)
	//		);
	//}
	KTL_INLINE tTVReal Geometry::distance(tTJSVariantClosure const& g0, tTJSVariantClosure const& g1, tTVInteger f0, tTVInteger f1) {
		return NativeGeometry::distance(
			g0, g1,
			boost::numeric_cast<NativeGeometry::flag_type>(f0),
			boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_GEOMETRY_GEOMETRY_HPP
