/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_GEOMETRY_GEOMETRY_HPP
#define SRC_GEOMETRY_GEOMETRY_HPP

#include <sstream>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/foreach.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/is_empty.hpp>
#include <boost/geometry/algorithms/is_valid.hpp>
#include <boost/geometry/algorithms/is_simple.hpp>
#include <boost/geometry/algorithms/intersects.hpp>
#include <boost/geometry/algorithms/touches.hpp>
#include <boost/geometry/algorithms/length.hpp>
#include <boost/geometry/algorithms/perimeter.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/centroid.hpp>
#include <boost/geometry/algorithms/envelope.hpp>
#include <boost/geometry/algorithms/convex_hull.hpp>
#include <boost/geometry/algorithms/correct.hpp>
#include <boost/geometry/algorithms/simplify.hpp>
#include <boost/geometry/algorithms/transform.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>
#include <boost/geometry/algorithms/covered_by.hpp>
#include <boost/geometry/algorithms/within.hpp>
#include <boost/geometry/algorithms/equals.hpp>
#include <boost/geometry/algorithms/overlaps.hpp>
#include <boost/geometry/algorithms/crosses.hpp>
#include <boost/geometry/algorithms/relate.hpp>
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/geometry/algorithms/relation.hpp>
#include <boost/geometry/algorithms/expand.hpp>
#include <boost/geometry/algorithms/not_implemented.hpp>
#include <boost/geometry/io/wkt/read.hpp>
#include <boost/geometry/io/wkt/write.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/get_pointer.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/com_ptr.hpp>
#include <sprig/com_ptr/unknown.hpp>
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
	public:
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
			template<typename P0>
			static result_type call(G const& g, P0 const&) {
				return call(g);
			}
		};
	public:
		template<typename G>
		result_type operator()(G const& g) const {
			return Derived::template caller<G>::call(g);
		}
		template<typename G, typename P0>
		result_type operator()(G const& g, P0 const& p0) const {
			return Derived::template caller<G>::call(g, p0);
		}
	};
	template<typename Result, typename Derived>
	class NativeGeometry::BinaryFuncBase {
	public:
		typedef Result result_type;
	public:
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
			template<typename P0>
			static result_type call(G0 const& g0, G1 const& g1, P0 const&) {
				return call(g0, g1);
			}
		};
	public:
		template<typename G0, typename G1>
		result_type operator()(G0 const& g0, G1 const& g1) const {
			return Derived::template caller<G0, G1>::call(g0, g1);
		}
		template<typename G0, typename G1, typename P0>
		result_type operator()(G0 const& g0, G1 const& g1, P0 const& p0) const {
			return Derived::template caller<G0, G1>::call(g0, g1, p0);
		}
	};

	template<typename G>
	class NativeGeometry::Intersects1Dispatch
		: public boost::geometry::not_implemented<G>
	{};
	template<>
	class NativeGeometry::Intersects1Dispatch<NativeGeometry::linestring_type>
		: public boost::mpl::identity<NativeGeometry::linestring_type>
	{};
	template<>
	class NativeGeometry::Intersects1Dispatch<NativeGeometry::ring_type>
		: public boost::mpl::identity<NativeGeometry::ring_type>
	{};
	template<>
	class NativeGeometry::Intersects1Dispatch<NativeGeometry::polygon_type>
		: public boost::mpl::identity<NativeGeometry::polygon_type>
	{};
	template<>
	class NativeGeometry::Intersects1Dispatch<NativeGeometry::multi_linestring_type>
		: public boost::mpl::identity<NativeGeometry::multi_linestring_type>
	{};
	template<>
	class NativeGeometry::Intersects1Dispatch<NativeGeometry::multi_polygon_type>
		: public boost::mpl::identity<NativeGeometry::multi_polygon_type>
	{};

	template<typename G>
	class NativeGeometry::Touches1Dispatch
		: public boost::geometry::not_implemented<G>
	{};
	template<>
	class NativeGeometry::Touches1Dispatch<NativeGeometry::linestring_type>
		: public boost::mpl::identity<NativeGeometry::linestring_type>
	{};
	template<>
	class NativeGeometry::Touches1Dispatch<NativeGeometry::ring_type>
		: public boost::mpl::identity<NativeGeometry::ring_type>
	{};
	template<>
	class NativeGeometry::Touches1Dispatch<NativeGeometry::polygon_type>
		: public boost::mpl::identity<NativeGeometry::polygon_type>
	{};
	template<>
	class NativeGeometry::Touches1Dispatch<NativeGeometry::multi_linestring_type>
		: public boost::mpl::identity<NativeGeometry::multi_linestring_type>
	{};
	template<>
	class NativeGeometry::Touches1Dispatch<NativeGeometry::multi_polygon_type>
		: public boost::mpl::identity<NativeGeometry::multi_polygon_type>
	{};

	template<typename G>
	class NativeGeometry::ConvexHullDispatch
		: public boost::geometry::dispatch::convex_hull<G>
	{};
	template<>
	class NativeGeometry::ConvexHullDispatch<NativeGeometry::point_type>
		: public boost::geometry::not_implemented<NativeGeometry::point_type>
	{};
	template<>
	class NativeGeometry::ConvexHullDispatch<NativeGeometry::segment_type>
		: public boost::geometry::not_implemented<NativeGeometry::segment_type>
	{};

	template<typename G0, typename G1>
	class NativeGeometry::DisjointDispatch
		: public boost::geometry::dispatch::disjoint<G0, G1>
	{};
	template<>
	class NativeGeometry::DisjointDispatch<NativeGeometry::multi_point_type, NativeGeometry::ring_type>
		: public boost::geometry::not_implemented<multi_point_type, ring_type>
	{};
	template<>
	class NativeGeometry::DisjointDispatch<NativeGeometry::multi_point_type, NativeGeometry::polygon_type>
		: public boost::geometry::not_implemented<multi_point_type, polygon_type>
	{};
	template<>
	class NativeGeometry::DisjointDispatch<NativeGeometry::multi_point_type, NativeGeometry::multi_polygon_type>
		: public boost::geometry::not_implemented<multi_point_type, multi_polygon_type>
	{};
	template<>
	class NativeGeometry::DisjointDispatch<NativeGeometry::ring_type, NativeGeometry::multi_point_type>
		: public boost::geometry::not_implemented<ring_type, multi_point_type>
	{};
	template<>
	class NativeGeometry::DisjointDispatch<NativeGeometry::polygon_type, NativeGeometry::multi_point_type>
		: public boost::geometry::not_implemented<polygon_type, multi_point_type>
	{};
	template<>
	class NativeGeometry::DisjointDispatch<NativeGeometry::multi_polygon_type, NativeGeometry::multi_point_type>
		: public boost::geometry::not_implemented<multi_polygon_type, multi_point_type>
	{};

	template<typename G0, typename G1>
	class NativeGeometry::TouchesDispatch
		: public boost::geometry::dispatch::touches<G0, G1>
	{};
	template<>
	class NativeGeometry::TouchesDispatch<NativeGeometry::point_type, NativeGeometry::point_type>
		: public boost::geometry::not_implemented<point_type, point_type>
	{};
	template<>
	class NativeGeometry::TouchesDispatch<NativeGeometry::point_type, NativeGeometry::box_type>
		: public boost::geometry::not_implemented<point_type, box_type>
	{};
	template<>
	class NativeGeometry::TouchesDispatch<NativeGeometry::point_type, NativeGeometry::multi_point_type>
		: public boost::geometry::not_implemented<point_type, multi_point_type>
	{};
	template<>
	class NativeGeometry::TouchesDispatch<NativeGeometry::box_type, NativeGeometry::point_type>
		: public boost::geometry::not_implemented<box_type, point_type>
	{};
	template<>
	class NativeGeometry::TouchesDispatch<NativeGeometry::multi_point_type, NativeGeometry::point_type>
		: public boost::geometry::not_implemented<multi_point_type, point_type>
	{};
	template<>
	class NativeGeometry::TouchesDispatch<NativeGeometry::polygon_type, NativeGeometry::multi_linestring_type>
		: public boost::geometry::not_implemented<polygon_type, multi_linestring_type>
	{};
	template<>
	class NativeGeometry::TouchesDispatch<NativeGeometry::multi_linestring_type, NativeGeometry::polygon_type>
		: public boost::geometry::not_implemented<multi_linestring_type, polygon_type>
	{};

	template<typename G0, typename G1>
	class NativeGeometry::CrossesDispatch
		: public boost::geometry::dispatch::crosses<G0, G1>
	{};
	template<>
	class NativeGeometry::CrossesDispatch<NativeGeometry::ring_type, NativeGeometry::linestring_type>
		: public boost::geometry::not_implemented<ring_type, linestring_type>
	{};
	template<>
	class NativeGeometry::CrossesDispatch<NativeGeometry::ring_type, NativeGeometry::multi_linestring_type>
		: public boost::geometry::not_implemented<ring_type, multi_linestring_type>
	{};
	template<>
	class NativeGeometry::CrossesDispatch<NativeGeometry::polygon_type, NativeGeometry::linestring_type>
		: public boost::geometry::not_implemented<polygon_type, linestring_type>
	{};
	template<>
	class NativeGeometry::CrossesDispatch<NativeGeometry::polygon_type, NativeGeometry::multi_linestring_type>
		: public boost::geometry::not_implemented<polygon_type, multi_linestring_type>
	{};
	template<>
	class NativeGeometry::CrossesDispatch<NativeGeometry::multi_polygon_type, NativeGeometry::linestring_type>
		: public boost::geometry::not_implemented<multi_polygon_type, linestring_type>
	{};
	template<>
	class NativeGeometry::CrossesDispatch<NativeGeometry::multi_polygon_type, NativeGeometry::multi_linestring_type>
		: public boost::geometry::not_implemented<multi_polygon_type, multi_linestring_type>
	{};

	template<typename G0, typename G1>
	class NativeGeometry::RelateDispatch
		: public boost::geometry::dispatch::relate<G0, G1>
	{};
	template<>
	class NativeGeometry::RelateDispatch<NativeGeometry::ring_type, NativeGeometry::linestring_type>
		: public boost::geometry::not_implemented<ring_type, linestring_type>
	{};
	template<>
	class NativeGeometry::RelateDispatch<NativeGeometry::ring_type, NativeGeometry::multi_linestring_type>
		: public boost::geometry::not_implemented<ring_type, multi_linestring_type>
	{};
	template<>
	class NativeGeometry::RelateDispatch<NativeGeometry::polygon_type, NativeGeometry::linestring_type>
		: public boost::geometry::not_implemented<polygon_type, linestring_type>
	{};
	template<>
	class NativeGeometry::RelateDispatch<NativeGeometry::polygon_type, NativeGeometry::multi_linestring_type>
		: public boost::geometry::not_implemented<polygon_type, multi_linestring_type>
	{};
	template<>
	class NativeGeometry::RelateDispatch<NativeGeometry::multi_polygon_type, NativeGeometry::linestring_type>
		: public boost::geometry::not_implemented<multi_polygon_type, linestring_type>
	{};
	template<>
	class NativeGeometry::RelateDispatch<NativeGeometry::multi_polygon_type, NativeGeometry::multi_linestring_type>
		: public boost::geometry::not_implemented<multi_polygon_type, multi_linestring_type>
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
			template<typename P0> \
			static RESULT call(G const& g, P0 const& p0) { \
				return FUNC(g, p0); \
			} \
		}; \
	}
#define KTL_GEOMETRY_UNARY_FUNC_VARIANT_DEF(NAME, RESULT, FUNC, DISP) \
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
				return to_variant(FUNC(g)); \
			} \
			template<typename P0> \
			static RESULT call(G const& g, P0 const& p0) { \
				return to_variant(FUNC(g, p0)); \
			} \
		}; \
	}
	KTL_GEOMETRY_UNARY_FUNC_DEF(IsEmptyFunc, bool, boost::geometry::is_empty, boost::geometry::dispatch::is_empty);
	KTL_GEOMETRY_UNARY_FUNC_DEF(IsValidFunc, bool, boost::geometry::is_valid, boost::geometry::dispatch::is_valid);
	KTL_GEOMETRY_UNARY_FUNC_DEF(IsSimpleFunc, bool, boost::geometry::is_simple, boost::geometry::dispatch::is_simple);
	KTL_GEOMETRY_UNARY_FUNC_DEF(Intersects1Func, bool, boost::geometry::intersects, Intersects1Dispatch);
	KTL_GEOMETRY_UNARY_FUNC_DEF(Touches1Func, bool, boost::geometry::touches, Touches1Dispatch);
	KTL_GEOMETRY_UNARY_FUNC_DEF(LengthFunc, element_type, boost::geometry::length, boost::geometry::dispatch::length);
	KTL_GEOMETRY_UNARY_FUNC_DEF(PerimeterFunc, element_type, boost::geometry::perimeter, boost::geometry::dispatch::perimeter);
	KTL_GEOMETRY_UNARY_FUNC_DEF(AreaFunc, element_type, boost::geometry::area, boost::geometry::dispatch::area);
	KTL_GEOMETRY_UNARY_FUNC_DEF(CentroidFunc, point_type, boost::geometry::return_centroid<point_type>, boost::geometry::dispatch::centroid);
	KTL_GEOMETRY_UNARY_FUNC_DEF(EnvelopeFunc, box_type, boost::geometry::return_envelope<box_type>, boost::geometry::dispatch::envelope);
	KTL_GEOMETRY_UNARY_FUNC_DEF(ConvexHullFunc, ring_type, return_convex_hull<ring_type>, ConvexHullDispatch);
	KTL_GEOMETRY_UNARY_FUNC_VARIANT_DEF(CorrectFunc, tTJSVariant, return_correct, boost::geometry::dispatch::correct);
	KTL_GEOMETRY_UNARY_FUNC_VARIANT_DEF(SimplifyFunc, tTJSVariant, return_simplify, boost::geometry::dispatch::simplify);

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
			template<typename P0> \
			static RESULT call(G0 const& g0, G1 const& g1, P0 const& p0) { \
				return FUNC(g0, g1, p0); \
			} \
		}; \
	}
	KTL_GEOMETRY_BINARY_FUNC_DEF(DisjointFunc, bool, boost::geometry::disjoint, DisjointDispatch);
	KTL_GEOMETRY_BINARY_FUNC_DEF(IntersectsFunc, bool, boost::geometry::intersects, DisjointDispatch);
	KTL_GEOMETRY_BINARY_FUNC_DEF(CoveredByFunc, bool, boost::geometry::covered_by, boost::geometry::dispatch::covered_by);
	KTL_GEOMETRY_BINARY_FUNC_DEF(WithinFunc, bool, boost::geometry::within, boost::geometry::dispatch::within);
	KTL_GEOMETRY_BINARY_FUNC_DEF(EqualsFunc, bool, boost::geometry::equals, boost::geometry::dispatch::equals);
	KTL_GEOMETRY_BINARY_FUNC_DEF(OverlapsFunc, bool, boost::geometry::overlaps, boost::geometry::dispatch::overlaps);
	KTL_GEOMETRY_BINARY_FUNC_DEF(TouchesFunc, bool, boost::geometry::touches, TouchesDispatch);
	KTL_GEOMETRY_BINARY_FUNC_DEF(CrossesFunc, bool, boost::geometry::crosses, CrossesDispatch);
	KTL_GEOMETRY_BINARY_FUNC_DEF(RelateFunc, bool, boost::geometry::relate, RelateDispatch);
	KTL_GEOMETRY_BINARY_FUNC_DEF(DistanceFunc, element_type, boost::geometry::distance, boost::geometry::dispatch::distance);
	KTL_GEOMETRY_BINARY_FUNC_DEF(RelationFunc, boost::geometry::de9im::matrix, boost::geometry::relation, boost::geometry::dispatch::relate);
	KTL_GEOMETRY_BINARY_FUNC_DEF(ExpandFunc, box_type, return_expand, boost::geometry::dispatch::expand);

	KTL_INLINE NativeGeometry::point_type NativeGeometry::to_point(tTJSVariantClosure const& g) {
		return point_type(
			sprig::krkr::tjs::GetPropValue<tTVReal>(g, 0),
			sprig::krkr::tjs::GetPropValue<tTVReal>(g, 1)
			);
	}
	KTL_INLINE NativeGeometry::segment_type NativeGeometry::to_segment(tTJSVariantClosure const& g) {
		return segment_type(
			to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, 0).AsObjectClosureNoAddRef()),
			to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, 1).AsObjectClosureNoAddRef())
			);
	}
	KTL_INLINE NativeGeometry::box_type NativeGeometry::to_box(tTJSVariantClosure const& g) {
		return box_type(
			to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, 0).AsObjectClosureNoAddRef()),
			to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, 1).AsObjectClosureNoAddRef())
			);
	}
	KTL_INLINE NativeGeometry::linestring_type NativeGeometry::to_linestring(tTJSVariantClosure const& g) {
		linestring_type result;
		tjs_int count = sprig::krkr::tjs::GetPropValue<tjs_int>(g, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			result.push_back(
				to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, i).AsObjectClosureNoAddRef())
				);
		}
		return result;
	}
	KTL_INLINE NativeGeometry::ring_type NativeGeometry::to_ring(tTJSVariantClosure const& g) {
		ring_type result;
		tjs_int count = sprig::krkr::tjs::GetPropValue<tjs_int>(g, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			result.push_back(
				to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, i).AsObjectClosureNoAddRef())
				);
		}
		return result;
	}
	KTL_INLINE NativeGeometry::polygon_type NativeGeometry::to_polygon(tTJSVariantClosure const& g) {
		polygon_type result;
		boost::geometry::exterior_ring(result)
			= to_ring(sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, 0).AsObjectClosureNoAddRef())
			;
		{
			tTJSVariantClosure inners = sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, 1).AsObjectClosureNoAddRef();
			tjs_int inners_count = sprig::krkr::tjs::GetPropValue<tjs_int>(inners, SPRIG_KRKR_TJS_W("count"));
			for (tjs_int i = 0; i != inners_count; ++i) {
				boost::geometry::interior_rings(result).push_back(
					to_ring(sprig::krkr::tjs::GetPropValue<tTJSVariant>(inners, i).AsObjectClosureNoAddRef())
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
				to_point(sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, i).AsObjectClosureNoAddRef())
				);
		}
		return result;
	}
	KTL_INLINE NativeGeometry::multi_linestring_type NativeGeometry::to_multi_linestring(tTJSVariantClosure const& g) {
		multi_linestring_type result;
		tjs_int count = sprig::krkr::tjs::GetPropValue<tjs_int>(g, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			result.push_back(
				to_linestring(sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, i).AsObjectClosureNoAddRef())
				);
		}
		return result;
	}
	KTL_INLINE NativeGeometry::multi_polygon_type NativeGeometry::to_multi_polygon(tTJSVariantClosure const& g) {
		multi_polygon_type result;
		tjs_int count = sprig::krkr::tjs::GetPropValue<tjs_int>(g, SPRIG_KRKR_TJS_W("count"));
		for (tjs_int i = 0; i != count; ++i) {
			result.push_back(
				to_polygon(sprig::krkr::tjs::GetPropValue<tTJSVariant>(g, i).AsObjectClosureNoAddRef())
				);
		}
		return result;
	}
	KTL_INLINE NativeGeometry::translate_type NativeGeometry::to_translate(tTJSVariant const& s) {
		tTJSVariantClosure c = s.AsObjectClosureNoAddRef();
		return translate_type(
			sprig::krkr::tjs::GetPropValue<tTVReal>(c, 0),
			sprig::krkr::tjs::GetPropValue<tTVReal>(c, 1)
			);
	}
	KTL_INLINE NativeGeometry::translate_type NativeGeometry::to_negate_translate(tTJSVariant const& s) {
		tTJSVariantClosure c = s.AsObjectClosureNoAddRef();
		return translate_type(
			-sprig::krkr::tjs::GetPropValue<tTVReal>(c, 0),
			-sprig::krkr::tjs::GetPropValue<tTVReal>(c, 1)
			);
	}
	KTL_INLINE NativeGeometry::scale_type NativeGeometry::to_scale(tTJSVariant const& s) {
		if (s.Type() == tvtObject) {
			tTJSVariantClosure c = s.AsObjectClosureNoAddRef();
			return scale_type(
				sprig::krkr::tjs::GetPropValue<tTVReal>(c, 0),
				sprig::krkr::tjs::GetPropValue<tTVReal>(c, 1)
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
	KTL_INLINE NativeGeometry::matrix_type NativeGeometry::to_matrix(tTJSVariant const& s) {
		tTJSVariantClosure c = s.AsObjectClosureNoAddRef();
		tTJSVariantClosure cs[3] = {
			sprig::krkr::tjs::GetPropValue<tTJSVariant>(c, 0).AsObjectClosureNoAddRef(),
			sprig::krkr::tjs::GetPropValue<tTJSVariant>(c, 1).AsObjectClosureNoAddRef(),
			sprig::krkr::tjs::GetPropValue<tTJSVariant>(c, 2).AsObjectClosureNoAddRef()
		};
		return matrix_type(
			sprig::krkr::tjs::GetPropValue<tTVReal>(cs[0], 0),
			sprig::krkr::tjs::GetPropValue<tTVReal>(cs[0], 1),
			sprig::krkr::tjs::GetPropValue<tTVReal>(cs[0], 2),
			sprig::krkr::tjs::GetPropValue<tTVReal>(cs[1], 0),
			sprig::krkr::tjs::GetPropValue<tTVReal>(cs[1], 1),
			sprig::krkr::tjs::GetPropValue<tTVReal>(cs[1], 2),
			sprig::krkr::tjs::GetPropValue<tTVReal>(cs[2], 0),
			sprig::krkr::tjs::GetPropValue<tTVReal>(cs[2], 1),
			sprig::krkr::tjs::GetPropValue<tTVReal>(cs[2], 2)
			);
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(point_type const& g) {
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			0, 0, 0
			);
		sprig::krkr::tjs::AddMember(result, 0, tTJSVariant(g.x()));
		sprig::krkr::tjs::AddMember(result, 1, tTJSVariant(g.y()));
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(segment_type const& g) {
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			0, 0, 0
			);
		sprig::krkr::tjs::AddMember(result, 0, to_variant(g.first));
		sprig::krkr::tjs::AddMember(result, 1, to_variant(g.second));
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(box_type const& g) {
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			0, 0, 0
			);
		sprig::krkr::tjs::AddMember(result, 0, to_variant(g.min_corner()));
		sprig::krkr::tjs::AddMember(result, 1, to_variant(g.max_corner()));
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(linestring_type const& g) {
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			0, 0, 0
			);
		for (tjs_int i = 0, size = g.size(); i != size; ++i) {
			sprig::krkr::tjs::AddMember(result, i, to_variant(g.at(i)));
		}
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(ring_type const& g) {
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			0, 0, 0
			);
		for (tjs_int i = 0, size = g.size(); i != size; ++i) {
			sprig::krkr::tjs::AddMember(result, i, to_variant(g.at(i)));
		}
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(polygon_type const& g) {
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			0, 0, 0
			);
		sprig::krkr::tjs::AddMember(result, 0, to_variant(boost::geometry::exterior_ring(g)));
		{
			sprig::krkr::tjs::object_type inners = sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
				0, 0, 0
				);
			for (tjs_int i = 0, size = boost::geometry::interior_rings(g).size(); i != size; ++i) {
				sprig::krkr::tjs::AddMember(inners, i, to_variant(boost::geometry::interior_rings(g).at(i)));
			}
			sprig::krkr::tjs::AddMember(result, 1, sprig::krkr::tjs::as_object_closure_variant(inners));
		}
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(multi_point_type const& g) {
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			0, 0, 0
			);
		for (tjs_int i = 0, size = g.size(); i != size; ++i) {
			sprig::krkr::tjs::AddMember(result, i, to_variant(g.at(i)));
		}
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(multi_linestring_type const& g) {
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			0, 0, 0
			);
		for (tjs_int i = 0, size = g.size(); i != size; ++i) {
			sprig::krkr::tjs::AddMember(result, i, to_variant(g.at(i)));
		}
		return sprig::krkr::tjs::as_object_closure_variant(result);
	}
	KTL_INLINE tTJSVariant NativeGeometry::to_variant(multi_polygon_type const& g) {
		sprig::krkr::tjs::object_type result = sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			0, 0, 0
			);
		for (tjs_int i = 0, size = g.size(); i != size; ++i) {
			sprig::krkr::tjs::AddMember(result, i, to_variant(g.at(i)));
		}
		return sprig::krkr::tjs::as_object_closure_variant(result);
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
	template<typename F, typename P0>
	KTL_INLINE typename F::result_type NativeGeometry::dispatch(F fn, tTJSVariantClosure const& g, flag_type f, P0 const& p0) {
		typedef typename F::result_type result_type;
		result_type result = result_type();
		switch (f) {
		case gmfPoint:
			result = fn(to_point(g), p0);
			break;
		case gmfSegment:
			result = fn(to_segment(g), p0);
			break;
		case gmfBox:
			result = fn(to_box(g), p0);
			break;
		case gmfLinestring:
			result = fn(to_linestring(g), p0);
			break;
		case gmfRing:
			result = fn(to_ring(g), p0);
			break;
		case gmfPolygon:
			result = fn(to_polygon(g), p0);
			break;
		case gmfMultiPoint:
			result = fn(to_multi_point(g), p0);
			break;
		case gmfMultiLinestring:
			result = fn(to_multi_linestring(g), p0);
			break;
		case gmfMultiPolygon:
			result = fn(to_multi_polygon(g), p0);
			break;
		}
		return result;
	}
	template<typename F>
	KTL_INLINE typename F::result_type NativeGeometry::dispatch(F fn, tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
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
	template<typename F, typename P0>
	KTL_INLINE typename F::result_type NativeGeometry::dispatch(F fn, tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1, P0 const& p0) {
		typedef typename F::result_type result_type;
		result_type result = result_type();
		switch (f0) {
		case gmfPoint:
			switch (f1) {
			case gmfPoint:
				result = fn(to_point(g0), to_point(g1), p0);
				break;
			case gmfSegment:
				result = fn(to_point(g0), to_segment(g1), p0);
				break;
			case gmfBox:
				result = fn(to_point(g0), to_box(g1), p0);
				break;
			case gmfLinestring:
				result = fn(to_point(g0), to_linestring(g1), p0);
				break;
			case gmfRing:
				result = fn(to_point(g0), to_ring(g1), p0);
				break;
			case gmfPolygon:
				result = fn(to_point(g0), to_polygon(g1), p0);
				break;
			case gmfMultiPoint:
				result = fn(to_point(g0), to_multi_point(g1), p0);
				break;
			case gmfMultiLinestring:
				result = fn(to_point(g0), to_multi_linestring(g1), p0);
				break;
			case gmfMultiPolygon:
				result = fn(to_point(g0), to_multi_polygon(g1), p0);
				break;
			}
			break;
		case gmfSegment:
			switch (f1) {
			case gmfPoint:
				result = fn(to_segment(g0), to_point(g1), p0);
				break;
			case gmfSegment:
				result = fn(to_segment(g0), to_segment(g1), p0);
				break;
			case gmfBox:
				result = fn(to_segment(g0), to_box(g1), p0);
				break;
			case gmfLinestring:
				result = fn(to_segment(g0), to_linestring(g1), p0);
				break;
			case gmfRing:
				result = fn(to_segment(g0), to_ring(g1), p0);
				break;
			case gmfPolygon:
				result = fn(to_segment(g0), to_polygon(g1), p0);
				break;
			case gmfMultiPoint:
				result = fn(to_segment(g0), to_multi_point(g1), p0);
				break;
			case gmfMultiLinestring:
				result = fn(to_segment(g0), to_multi_linestring(g1), p0);
				break;
			case gmfMultiPolygon:
				result = fn(to_segment(g0), to_multi_polygon(g1), p0);
				break;
			}
			break;
		case gmfBox:
			switch (f1) {
			case gmfPoint:
				result = fn(to_box(g0), to_point(g1), p0);
				break;
			case gmfSegment:
				result = fn(to_box(g0), to_segment(g1), p0);
				break;
			case gmfBox:
				result = fn(to_box(g0), to_box(g1), p0);
				break;
			case gmfLinestring:
				result = fn(to_box(g0), to_linestring(g1), p0);
				break;
			case gmfRing:
				result = fn(to_box(g0), to_ring(g1), p0);
				break;
			case gmfPolygon:
				result = fn(to_box(g0), to_polygon(g1), p0);
				break;
			case gmfMultiPoint:
				result = fn(to_box(g0), to_multi_point(g1), p0);
				break;
			case gmfMultiLinestring:
				result = fn(to_box(g0), to_multi_linestring(g1), p0);
				break;
			case gmfMultiPolygon:
				result = fn(to_box(g0), to_multi_polygon(g1), p0);
				break;
			}
			break;
		case gmfLinestring:
			switch (f1) {
			case gmfPoint:
				result = fn(to_linestring(g0), to_point(g1), p0);
				break;
			case gmfSegment:
				result = fn(to_linestring(g0), to_segment(g1), p0);
				break;
			case gmfBox:
				result = fn(to_linestring(g0), to_box(g1), p0);
				break;
			case gmfLinestring:
				result = fn(to_linestring(g0), to_linestring(g1), p0);
				break;
			case gmfRing:
				result = fn(to_linestring(g0), to_ring(g1), p0);
				break;
			case gmfPolygon:
				result = fn(to_linestring(g0), to_polygon(g1), p0);
				break;
			case gmfMultiPoint:
				result = fn(to_linestring(g0), to_multi_point(g1), p0);
				break;
			case gmfMultiLinestring:
				result = fn(to_linestring(g0), to_multi_linestring(g1), p0);
				break;
			case gmfMultiPolygon:
				result = fn(to_linestring(g0), to_multi_polygon(g1), p0);
				break;
			}
			break;
		case gmfRing:
			switch (f1) {
			case gmfPoint:
				result = fn(to_ring(g0), to_point(g1), p0);
				break;
			case gmfSegment:
				result = fn(to_ring(g0), to_segment(g1), p0);
				break;
			case gmfBox:
				result = fn(to_ring(g0), to_box(g1), p0);
				break;
			case gmfLinestring:
				result = fn(to_ring(g0), to_linestring(g1), p0);
				break;
			case gmfRing:
				result = fn(to_ring(g0), to_ring(g1), p0);
				break;
			case gmfPolygon:
				result = fn(to_ring(g0), to_polygon(g1), p0);
				break;
			case gmfMultiPoint:
				result = fn(to_ring(g0), to_multi_point(g1), p0);
				break;
			case gmfMultiLinestring:
				result = fn(to_ring(g0), to_multi_linestring(g1), p0);
				break;
			case gmfMultiPolygon:
				result = fn(to_ring(g0), to_multi_polygon(g1), p0);
				break;
			}
			break;
		case gmfPolygon:
			switch (f1) {
			case gmfPoint:
				result = fn(to_polygon(g0), to_point(g1), p0);
				break;
			case gmfSegment:
				result = fn(to_polygon(g0), to_segment(g1), p0);
				break;
			case gmfBox:
				result = fn(to_polygon(g0), to_box(g1), p0);
				break;
			case gmfLinestring:
				result = fn(to_polygon(g0), to_linestring(g1), p0);
				break;
			case gmfRing:
				result = fn(to_polygon(g0), to_ring(g1), p0);
				break;
			case gmfPolygon:
				result = fn(to_polygon(g0), to_polygon(g1), p0);
				break;
			case gmfMultiPoint:
				result = fn(to_polygon(g0), to_multi_point(g1), p0);
				break;
			case gmfMultiLinestring:
				result = fn(to_polygon(g0), to_multi_linestring(g1), p0);
				break;
			case gmfMultiPolygon:
				result = fn(to_polygon(g0), to_multi_polygon(g1), p0);
				break;
			}
			break;
		case gmfMultiPoint:
			switch (f1) {
			case gmfPoint:
				result = fn(to_multi_point(g0), to_point(g1), p0);
				break;
			case gmfSegment:
				result = fn(to_multi_point(g0), to_segment(g1), p0);
				break;
			case gmfBox:
				result = fn(to_multi_point(g0), to_box(g1), p0);
				break;
			case gmfLinestring:
				result = fn(to_multi_point(g0), to_linestring(g1), p0);
				break;
			case gmfRing:
				result = fn(to_multi_point(g0), to_ring(g1), p0);
				break;
			case gmfPolygon:
				result = fn(to_multi_point(g0), to_polygon(g1), p0);
				break;
			case gmfMultiPoint:
				result = fn(to_multi_point(g0), to_multi_point(g1), p0);
				break;
			case gmfMultiLinestring:
				result = fn(to_multi_point(g0), to_multi_linestring(g1), p0);
				break;
			case gmfMultiPolygon:
				result = fn(to_multi_point(g0), to_multi_polygon(g1), p0);
				break;
			}
			break;
		case gmfMultiLinestring:
			switch (f1) {
			case gmfPoint:
				result = fn(to_multi_linestring(g0), to_point(g1), p0);
				break;
			case gmfSegment:
				result = fn(to_multi_linestring(g0), to_segment(g1), p0);
				break;
			case gmfBox:
				result = fn(to_multi_linestring(g0), to_box(g1), p0);
				break;
			case gmfLinestring:
				result = fn(to_multi_linestring(g0), to_linestring(g1), p0);
				break;
			case gmfRing:
				result = fn(to_multi_linestring(g0), to_ring(g1), p0);
				break;
			case gmfPolygon:
				result = fn(to_multi_linestring(g0), to_polygon(g1), p0);
				break;
			case gmfMultiPoint:
				result = fn(to_multi_linestring(g0), to_multi_point(g1), p0);
				break;
			case gmfMultiLinestring:
				result = fn(to_multi_linestring(g0), to_multi_linestring(g1), p0);
				break;
			case gmfMultiPolygon:
				result = fn(to_multi_linestring(g0), to_multi_polygon(g1), p0);
				break;
			}
			break;
		case gmfMultiPolygon:
			switch (f1) {
			case gmfPoint:
				result = fn(to_multi_polygon(g0), to_point(g1), p0);
				break;
			case gmfSegment:
				result = fn(to_multi_polygon(g0), to_segment(g1), p0);
				break;
			case gmfBox:
				result = fn(to_multi_polygon(g0), to_box(g1), p0);
				break;
			case gmfLinestring:
				result = fn(to_multi_polygon(g0), to_linestring(g1), p0);
				break;
			case gmfRing:
				result = fn(to_multi_polygon(g0), to_ring(g1), p0);
				break;
			case gmfPolygon:
				result = fn(to_multi_polygon(g0), to_polygon(g1), p0);
				break;
			case gmfMultiPoint:
				result = fn(to_multi_polygon(g0), to_multi_point(g1), p0);
				break;
			case gmfMultiLinestring:
				result = fn(to_multi_polygon(g0), to_multi_linestring(g1), p0);
				break;
			case gmfMultiPolygon:
				result = fn(to_multi_polygon(g0), to_multi_polygon(g1), p0);
				break;
			}
			break;
		}
		return result;
	}
	template<typename R, typename G>
	KTL_INLINE R NativeGeometry::return_convex_hull(G const& g) {
		R result;
		boost::geometry::convex_hull(g, result);
		return result;
	}
	template<typename G>
	KTL_INLINE G NativeGeometry::return_correct(G const& g) {
		G result = g;
		boost::geometry::correct(result);
		return result;
	}
	template<typename G, typename D>
	KTL_INLINE G NativeGeometry::return_simplify(G const& g, D const& d) {
		G result;
		boost::geometry::simplify(g, result, d);
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
	template<typename G1, typename G2>
	KTL_INLINE G1 NativeGeometry::return_expand(G1 const& g1, G2 const& g2) {
		G1 result = g1;
		boost::geometry::expand(result, g2);
		return result;
	}

	template<typename G>
	KTL_INLINE G NativeGeometry::read_wkt(std::string const& s) {
		G result;
		boost::geometry::read_wkt(s, result);
		return result;
	}
	template<typename Elem, typename Traits>
	KTL_INLINE void NativeGeometry::write_wkt(std::basic_ostream<Elem, Traits>& os, tTJSVariantClosure const& g, flag_type f) {
		switch (f) {
		case gmfPoint:
			os << boost::geometry::wkt(to_point(g));
			break;
		case gmfSegment:
			os << boost::geometry::wkt(to_segment(g));
			break;
		case gmfBox:
			os << boost::geometry::wkt(to_box(g));
			break;
		case gmfLinestring:
			os << boost::geometry::wkt(to_linestring(g));
			break;
		case gmfRing:
			os << boost::geometry::wkt(to_ring(g));
			break;
		case gmfPolygon:
			os << boost::geometry::wkt(to_polygon(g));
			break;
		case gmfMultiPoint:
			os << boost::geometry::wkt(to_multi_point(g));
			break;
		case gmfMultiLinestring:
			os << boost::geometry::wkt(to_multi_linestring(g));
			break;
		case gmfMultiPolygon:
			os << boost::geometry::wkt(to_multi_polygon(g));
			break;
		}
	}

	KTL_INLINE bool NativeGeometry::loadStorage(
		binary_type& binary,
		tjs_char const* storage
		)
	{
		sprig::com_ptr<IStream> in(::TVPCreateIStream(storage, TJS_BS_READ));
		if (!in) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		LARGE_INTEGER move;
		move.QuadPart = 0;
		ULARGE_INTEGER new_pos;
		if (FAILED(in->Seek(move, TJS_BS_SEEK_END, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		size_type file_size = static_cast<size_type>(new_pos.QuadPart);
		if (!file_size) {
			binary.clear();
			return true;
		}
		move.QuadPart = 0;
		if (FAILED(in->Seek(move, TJS_BS_SEEK_SET, &new_pos))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルシークに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		binary.resize(file_size);
		ULONG io_size = 0;
		if (FAILED(in->Read(&binary[0], file_size, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイル読込に失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return false;
		}
		return true;
	}
	KTL_INLINE NativeGeometry::int_type NativeGeometry::saveStorage(
		tjs_char const* storage,
		void const* data,
		ULONG length
		)
	{
		sprig::com_ptr<IStream> out(::TVPCreateIStream(storage, TJS_BS_WRITE));
		if (!out) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				tTJSString(SPRIG_KRKR_TJS_W("ファイルオープンに失敗しました: ")) + storage,
				sprig::krkr::internal_error
				);
			return -1;
		}
		if (!length) {
			return length;
		}
		ULONG io_size = 0;
		if (FAILED(out->Write(data, length, &io_size))) {
			KTL_ERROR(
				KTL_ERROR_SECTION,
				SPRIG_KRKR_TJS_W("ファイル書込に失敗しました"),
				sprig::krkr::internal_error
				);
			return io_size;
		}
		return io_size;
	}
	//
	// NativeGeometry
	//
	NativeGeometry::NativeGeometry() {}
	//
	//	SUMMARY: 単項系メソッド
	//
	KTL_INLINE bool NativeGeometry::isEmpty(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(IsEmptyFunc(), g, f);
	}
	KTL_INLINE bool NativeGeometry::isValid(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(IsValidFunc(), g, f);
	}
	KTL_INLINE bool NativeGeometry::isSimple(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(IsSimpleFunc(), g, f);
	}
	KTL_INLINE bool NativeGeometry::intersects(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(Intersects1Func(), g, f);
	}
	KTL_INLINE bool NativeGeometry::touches(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(Touches1Func(), g, f);
	}
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
		return to_variant(dispatch(CentroidFunc(), g, f));
	}
	KTL_INLINE tTJSVariant NativeGeometry::envelope(tTJSVariantClosure const& g, flag_type f) {
		return to_variant(dispatch(EnvelopeFunc(), g, f));
	}
	KTL_INLINE tTJSVariant NativeGeometry::convexHull(tTJSVariantClosure const& g, flag_type f) {
		return to_variant(dispatch(ConvexHullFunc(), g, f));
	}
	KTL_INLINE tTJSVariant NativeGeometry::correct(tTJSVariantClosure const& g, flag_type f) {
		return dispatch(CorrectFunc(), g, f);
	}
	KTL_INLINE tTJSVariant NativeGeometry::simplify(tTJSVariantClosure const& g, flag_type f, element_type const& d) {
		return dispatch(SimplifyFunc(), g, f, d);
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
	KTL_INLINE tTJSVariant NativeGeometry::matrixTransform(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t) {
		return transform(g, f, to_matrix(t));
	}
	//
	//	SUMMARY: 二項系メソッド
	//
	KTL_INLINE bool NativeGeometry::disjoint(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return dispatch(DisjointFunc(), g0, f0, g1, f1);
	}
	KTL_INLINE bool NativeGeometry::intersects(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return dispatch(IntersectsFunc(), g0, f0, g1, f1);
	}
	KTL_INLINE bool NativeGeometry::coveredBy(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return dispatch(CoveredByFunc(), g0, f0, g1, f1);
	}
	KTL_INLINE bool NativeGeometry::within(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return dispatch(WithinFunc(), g0, f0, g1, f1);
	}
	KTL_INLINE bool NativeGeometry::equals(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return dispatch(EqualsFunc(), g0, f0, g1, f1);
	}
	KTL_INLINE bool NativeGeometry::overlaps(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return dispatch(OverlapsFunc(), g0, f0, g1, f1);
	}
	KTL_INLINE bool NativeGeometry::touches(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return dispatch(TouchesFunc(), g0, f0, g1, f1);
	}
	KTL_INLINE bool NativeGeometry::crosses(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return dispatch(CrossesFunc(), g0, f0, g1, f1);
	}
	KTL_INLINE bool NativeGeometry::relate(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1, char const* t) {
		return dispatch(RelateFunc(), g0, f0, g1, f1, boost::geometry::de9im::mask(t));
	}
	KTL_INLINE NativeGeometry::element_type NativeGeometry::distance(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return dispatch(DistanceFunc(), g0, f0, g1, f1);
	}
	KTL_INLINE tTJSVariant NativeGeometry::relation(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return sprig::str_cast<tTJSString>(dispatch(RelationFunc(), g0, f0, g1, f1).str());
	}
	KTL_INLINE tTJSVariant NativeGeometry::expand(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1) {
		return to_variant(dispatch(ExpandFunc(), g0, f0, g1, f1));
	}
	//
	//	SUMMARY: IO系メソッド
	//
	KTL_INLINE tTJSVariant NativeGeometry::readWKT(tjs_char const* source, flag_type f) {
		std::string s = sprig::str_cast<std::string>(source);
		tTJSVariant result;
		switch (f) {
		case gmfPoint:
			result = to_variant(read_wkt<point_type>(s));
			break;
		case gmfSegment:
			result = to_variant(read_wkt<segment_type>(s));
			break;
		case gmfBox:
			result = to_variant(read_wkt<box_type>(s));
			break;
		case gmfLinestring:
			result = to_variant(read_wkt<linestring_type>(s));
			break;
		case gmfRing:
			result = to_variant(read_wkt<ring_type>(s));
			break;
		case gmfPolygon:
			result = to_variant(read_wkt<polygon_type>(s));
			break;
		case gmfMultiPoint:
			result = to_variant(read_wkt<multi_point_type>(s));
			break;
		case gmfMultiLinestring:
			result = to_variant(read_wkt<multi_linestring_type>(s));
			break;
		case gmfMultiPolygon:
			result = to_variant(read_wkt<multi_polygon_type>(s));
			break;
		}
		return result;
	}
	KTL_INLINE tTJSVariant NativeGeometry::readStorageWKT(tjs_char const* storage, flag_type f) {
		binary_type binary;
		if (!loadStorage(binary, storage)) {
			return false;
		}
		if (binary.empty()
			|| binary.size() % sizeof(tjs_char)
			|| reinterpret_cast<tjs_char const*>(&binary[0])[0] != SPRIG_KRKR_TJS_W('\xFEFF')
			)
		{
			binary.insert(binary.end(), sizeof(tjs_nchar), 0);
			return readWKT(
				sprig::str_cast<std::basic_string<tjs_char> >(
					reinterpret_cast<tjs_nchar const*>(&binary[0])
					).c_str()
					,
				f
				);
		} else {
			binary.insert(binary.end(), sizeof(tjs_char), 0);
			return readWKT(reinterpret_cast<tjs_char const*>(&binary[0]), f);
		}
		return tTJSVariant();
	}
	KTL_INLINE tTJSVariant NativeGeometry::writeWKTToString(tTJSVariantClosure const& g, flag_type f) {
		std::basic_ostringstream<tjs_char> os;
		write_wkt(os, g, f);
		return tTJSVariant(os.str().c_str());
	}
	KTL_INLINE NativeGeometry::int_type NativeGeometry::writeStorageWKT(tjs_char const* storage, tTJSVariantClosure const& g, flag_type f) {
		std::basic_ostringstream<tjs_char> os;
		os << SPRIG_KRKR_TJS_W('\xFEFF');
		write_wkt(os, g, f);
		std::basic_string<tjs_char> string = os.str();
		return saveStorage(
			storage,
			string.size()
				? &string[0]
				: 0
				,
			string.size() * sizeof(tjs_char)
			);
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
	KTL_INLINE bool Geometry::isEmpty(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::isEmpty(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE bool Geometry::isValid(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::isValid(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE bool Geometry::isSimple(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::isSimple(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE bool Geometry::intersects(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::intersects(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE bool Geometry::touches(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::touches(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTVReal Geometry::length(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::length(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTVReal Geometry::perimeter(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::perimeter(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTVReal Geometry::area(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::area(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTJSVariant Geometry::centroid(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::centroid(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTJSVariant Geometry::envelope(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::envelope(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTJSVariant Geometry::convexHull(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::convexHull(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTJSVariant Geometry::correct(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::correct(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTJSVariant Geometry::simplify(tTJSVariantClosure const& g, tTVInteger f, tTVReal d) {
		return NativeGeometry::simplify(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f),
			d
			);
	}
	KTL_INLINE tTJSVariant Geometry::translate(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t) {
		return NativeGeometry::translate(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f),
			t
			);
	}
	KTL_INLINE tTJSVariant Geometry::scale(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t) {
		return NativeGeometry::scale(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f),
			t
			);
	}
	KTL_INLINE tTJSVariant Geometry::scale(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t, tTJSVariant const& o) {
		return NativeGeometry::scale(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f),
			t, o
			);
	}
	KTL_INLINE tTJSVariant Geometry::rotate(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t) {
		return NativeGeometry::rotate(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f),
			t
			);
	}
	KTL_INLINE tTJSVariant Geometry::rotate(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t, tTJSVariant const& o) {
		return NativeGeometry::rotate(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f),
			t, o
			);
	}
	KTL_INLINE tTJSVariant Geometry::matrixTransform(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t) {
		return NativeGeometry::matrixTransform(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f),
			t
			);
	}
	//
	//	SUMMARY: 二項系メソッド
	//
	KTL_INLINE bool Geometry::disjoint(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::disjoint(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::intersects(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::intersects(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::coveredBy(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::coveredBy(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::within(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::within(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::equals(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::equals(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::overlaps(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::overlaps(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::touches(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::touches(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::crosses(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::crosses(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE bool Geometry::relate(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1, tTJSVariantString const* t) {
		return NativeGeometry::relate(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1),
			sprig::str_cast<std::string>(sprig::krkr::tjs::as_c_str(t)).c_str()
			);
	}
	KTL_INLINE tTVReal Geometry::distance(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::distance(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE tTJSVariant Geometry::relation(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::relation(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	KTL_INLINE tTJSVariant Geometry::expand(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1) {
		return NativeGeometry::expand(
			g0, boost::numeric_cast<NativeGeometry::flag_type>(f0),
			g1, boost::numeric_cast<NativeGeometry::flag_type>(f1)
			);
	}
	//
	//	SUMMARY: IO系メソッド
	//
	KTL_INLINE tTJSVariant Geometry::readWKT(tTJSVariantString const* source, tTVInteger f) {
		return NativeGeometry::readWKT(
			sprig::krkr::tjs::as_c_str(source),
			boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTJSVariant Geometry::readStorageWKT(tTJSVariantString const* storage, tTVInteger f) {
		return NativeGeometry::readStorageWKT(
			sprig::krkr::tjs::as_c_str(storage),
			boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTJSVariant Geometry::writeWKTToString(tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::writeWKTToString(
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
	KTL_INLINE tTVInteger Geometry::writeStorageWKT(tTJSVariantString const* storage, tTJSVariantClosure const& g, tTVInteger f) {
		return NativeGeometry::writeStorageWKT(
			sprig::krkr::tjs::as_c_str(storage),
			g, boost::numeric_cast<NativeGeometry::flag_type>(f)
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION

#endif	// #ifndef SRC_GEOMETRY_GEOMETRY_HPP
