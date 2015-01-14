/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_GEOMETRY_GEOMETRY_DECL_HPP
#define SRC_GEOMETRY_GEOMETRY_DECL_HPP

#include <boost/cstdint.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_point.hpp>
#include <boost/geometry/geometries/multi_linestring.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>
#include <boost/geometry/strategies/transform/matrix_transformers.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeGeometry
	//
	class NativeGeometry {
	public:
		typedef boost::uint_fast32_t flag_type;
		typedef tTVReal element_type;
		typedef boost::geometry::model::d2::point_xy<element_type> point_type;
		typedef boost::geometry::model::segment<point_type> segment_type;
		typedef boost::geometry::model::box<point_type> box_type;
		typedef boost::geometry::model::linestring<point_type> linestring_type;
		typedef boost::geometry::model::ring<point_type> ring_type;
		typedef boost::geometry::model::polygon<point_type> polygon_type;
		typedef boost::geometry::model::multi_point<point_type> multi_point_type;
		typedef boost::geometry::model::multi_linestring<linestring_type> multi_linestring_type;
		typedef boost::geometry::model::multi_polygon<polygon_type> multi_polygon_type;

		typedef boost::geometry::strategy::transform::translate_transformer<element_type, 2, 2> translate_type;
		typedef boost::geometry::strategy::transform::scale_transformer<element_type, 2, 2> scale_type;
		typedef boost::geometry::strategy::transform::rotate_transformer<boost::geometry::radian, element_type, 2, 2> rotate_type;
	private:
		template<typename Result, typename Derived>
		class UnaryFuncBase;
		template<typename Result, typename Derived>
		class BinaryFuncBase;

		template<typename G0, typename G1>
		class DisjointDispatch;

		class IsValidFunc;
		class IsSimpleFunc;
		class Intersects1Func;
		class Touches1Func;
		class LengthFunc;
		class PerimeterFunc;
		class AreaFunc;
		class CentroidFunc;

		class DisjointFunc;
		class CoveredByFunc;
		class WithinFunc;
		class EqualsFunc;
		class OverlapsFunc;
		class TouchesFunc;
		class CrossesFunc;
		class IntersectsFunc;
		class DistanceFunc;
	private:
		static point_type to_point(tTJSVariantClosure const& g);
		static segment_type to_segment(tTJSVariantClosure const& g);
		static box_type to_box(tTJSVariantClosure const& g);
		static linestring_type to_linestring(tTJSVariantClosure const& g);
		static ring_type to_ring(tTJSVariantClosure const& g);
		static polygon_type to_polygon(tTJSVariantClosure const& g);
		static multi_point_type to_multi_point(tTJSVariantClosure const& g);
		static multi_linestring_type to_multi_linestring(tTJSVariantClosure const& g);
		static multi_polygon_type to_multi_polygon(tTJSVariantClosure const& g);

		static translate_type to_translate(tTJSVariant const& s);
		static translate_type to_negate_translate(tTJSVariant const& s);
		static scale_type to_scale(tTJSVariant const& s);
		static rotate_type to_rotate(tTJSVariant const& s);

		static tTJSVariant to_variant(point_type const& g);
		static tTJSVariant to_variant(segment_type const& g);
		static tTJSVariant to_variant(box_type const& g);
		static tTJSVariant to_variant(linestring_type const& g);
		static tTJSVariant to_variant(ring_type const& g);
		static tTJSVariant to_variant(polygon_type const& g);
		static tTJSVariant to_variant(multi_point_type const& g);
		static tTJSVariant to_variant(multi_linestring_type const& g);
		static tTJSVariant to_variant(multi_polygon_type const& g);

		template<typename F>
		static typename F::result_type dispatch(F fn, tTJSVariantClosure const& g, flag_type f);
		template<typename F>
		static typename F::result_type dispatch(F fn, tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1);

		template<typename G, typename S>
		static G return_transform(G const& g, S const& s);
		template<typename S>
		static tTJSVariant transform(tTJSVariantClosure const& g, flag_type f, S const& s);
		template<typename S>
		static tTJSVariant transform(tTJSVariantClosure const& g, flag_type f, S const& s, tTJSVariant const& o);
	public:
		NativeGeometry();
	public:
		//
		//	SUMMARY: ジオメトリモデル系フラグ
		//
		static flag_type const gmfPoint = 0x00000001;
		static flag_type const gmfSegment = 0x00000002;
		static flag_type const gmfBox = 0x00000003;
		static flag_type const gmfLinestring  = 0x00000004;
		static flag_type const gmfRing = 0x00000005;
		static flag_type const gmfPolygon = 0x00000006;
		static flag_type const gmfMultiPoint = 0x00000007;
		static flag_type const gmfMultiLinestring  = 0x00000008;
		static flag_type const gmfMultiPolygon = 0x00000009;
	public:
		//
		//	SUMMARY: 単項系メソッド
		//
		static bool isValid(tTJSVariantClosure const& g, flag_type f);
		static bool isSimple(tTJSVariantClosure const& g, flag_type f);
		//static bool intersects(tTJSVariantClosure const& g, flag_type f);
		//static bool touches(tTJSVariantClosure const& g, flag_type f);
		static element_type length(tTJSVariantClosure const& g, flag_type f);
		static element_type perimeter(tTJSVariantClosure const& g, flag_type f);
		static element_type area(tTJSVariantClosure const& g, flag_type f);
		static tTJSVariant centroid(tTJSVariantClosure const& g, flag_type f);
		static tTJSVariant translate(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t);
		static tTJSVariant scale(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t);
		static tTJSVariant scale(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t, tTJSVariant const& o);
		static tTJSVariant rotate(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t);
		static tTJSVariant rotate(tTJSVariantClosure const& g, flag_type f, tTJSVariant const& t, tTJSVariant const& o);
		//
		//	SUMMARY: 二項系メソッド
		//
		static bool disjoint(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1);
		static bool coveredBy(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1);
		static bool within(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1);
		static bool equals(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1);
		static bool overlaps(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1);
		//static bool touches(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1);
		//static bool crosses(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1);
		//static bool intersects(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1);
		static element_type distance(tTJSVariantClosure const& g0, flag_type f0, tTJSVariantClosure const& g1, flag_type f1);
	};

	//
	// Geometry
	//
	class Geometry
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeGeometry> instance_;
	public:
		Geometry();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: ジオメトリモデル系フラグ
		//
		static tTVInteger const gmfPoint = NativeGeometry::gmfPoint;
		static tTVInteger const gmfSegment = NativeGeometry::gmfSegment;
		static tTVInteger const gmfBox = NativeGeometry::gmfBox;
		static tTVInteger const gmfLinestring  = NativeGeometry::gmfLinestring;
		static tTVInteger const gmfRing = NativeGeometry::gmfRing;
		static tTVInteger const gmfPolygon = NativeGeometry::gmfPolygon;
		static tTVInteger const gmfMultiPoint = NativeGeometry::gmfMultiPoint;
		static tTVInteger const gmfMultiLinestring  = NativeGeometry::gmfMultiLinestring;
		static tTVInteger const gmfMultiPolygon = NativeGeometry::gmfMultiPolygon;
	public:
		//
		//	SUMMARY: 単項系メソッド
		//
		static bool isValid(tTJSVariantClosure const& g, tTVInteger f);
		static bool isSimple(tTJSVariantClosure const& g, tTVInteger f);
		//static bool intersects(tTJSVariantClosure const& g, tTVInteger f);
		//static bool touches(tTJSVariantClosure const& g, tTVInteger f);
		static tTVReal length(tTJSVariantClosure const& g, tTVInteger f);
		static tTVReal perimeter(tTJSVariantClosure const& g, tTVInteger f);
		static tTVReal area(tTJSVariantClosure const& g, tTVInteger f);
		static tTJSVariant centroid(tTJSVariantClosure const& g, tTVInteger f);
		static tTJSVariant translate(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t);
		static tTJSVariant scale(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t);
		static tTJSVariant scale(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t, tTJSVariant const& o);
		static tTJSVariant rotate(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t);
		static tTJSVariant rotate(tTJSVariantClosure const& g, tTVInteger f, tTJSVariant const& t, tTJSVariant const& o);
		//
		//	SUMMARY: 二項系メソッド
		//
		static bool disjoint(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1);
		static bool coveredBy(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1);
		static bool within(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1);
		static bool equals(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1);
		static bool overlaps(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1);
		//static bool touches(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1);
		//static bool crosses(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1);
		//static bool intersects(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1);
		static tTVReal distance(tTJSVariantClosure const& g0, tTVInteger f0, tTJSVariantClosure const& g1, tTVInteger f1);
	};
}	// namespace ktl

#endif	// #ifndef SRC_GEOMETRY_GEOMETRY_DECL_HPP
