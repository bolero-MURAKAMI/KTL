#pragma once

#include <boost/array.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/array.hpp>
#include <sprig/call_traits.hpp>
#include <sprig/serialization/sprig/ublas/vector.hpp>
#include <sprig/serialization/sprig/ublas/matrix.hpp>

namespace ktl {
	//
	// CoordTrans
	//
	class CoordTrans {
	public:
		typedef int coord_element_type;
		typedef sprig::ublas::coordinate_2d<coord_element_type> coord_type;
		typedef double trans_element_type;
		typedef sprig::ublas::scaling_2d<trans_element_type> scaling_type;
		typedef sprig::ublas::rotation_2d<trans_element_type> rotation_type;
		typedef sprig::ublas::translation_2d<trans_element_type> translation_type;
		typedef coord_type size_type;
		typedef boost::array<coord_type, 2> rect_type;
		typedef boost::array<coord_type, 4> coords_type;
		typedef boost::numeric::ublas::matrix<trans_element_type> matrix_type;
		typedef boost::numeric::ublas::vector<trans_element_type> vector_type;
	private:
		scaling_type scaling_;
		rotation_type rotation_;
		translation_type translation_;
		rect_type rect_;
		coord_type center_;
		size_type size_;
		coords_type points_;
		coords_type transformed_points_;
		coords_type enclosed_points_;
		rect_type enclosed_rect_;
		size_type enclosed_size_;
	public:
		CoordTrans()
			: scaling_(1.0, 1.0)
		{}
		void update() {
			size_ = rect_[1] - rect_[0];

			points_[0] = rect_[0] - center_;
			points_[1] = coord_type(rect_[1].get<0>(), rect_[0].get<1>()) - center_;
			points_[2] = coord_type(rect_[0].get<0>(), rect_[1].get<1>()) - center_;
			points_[3] = rect_[1] - center_;

			matrix_type transformation = sprig::ublas::matrix_prod(
				scaling_.matrix(),
				rotation_.matrix(),
				translation_.matrix()
				);

			transformed_points_[0] = vector_type(boost::numeric::ublas::prod(points_[0].vector(), transformation));
			transformed_points_[1] = vector_type(boost::numeric::ublas::prod(points_[1].vector(), transformation));
			transformed_points_[2] = vector_type(boost::numeric::ublas::prod(points_[2].vector(), transformation));
			transformed_points_[3] = vector_type(boost::numeric::ublas::prod(points_[3].vector(), transformation));

			coords_type::size_type left_where = 0;
			coords_type::size_type top_where = 0;
			for (coords_type::size_type i = 1; i != 4; ++i) {
				if (transformed_points_[i].get<0>() < transformed_points_[left_where].get<0>()) {
					left_where = i;
				}
				if (transformed_points_[i].get<1>() < transformed_points_[top_where].get<1>()) {
					top_where = i;
				}
			}
			coord_type cutting(
				transformed_points_[left_where].get<0>(),
				transformed_points_[top_where].get<1>()
				);

			enclosed_points_[0] = transformed_points_[0] - cutting;
			enclosed_points_[1] = transformed_points_[1] - cutting;
			enclosed_points_[2] = transformed_points_[2] - cutting;
			enclosed_points_[3] = transformed_points_[3] - cutting;

			enclosed_rect_[0].assign(
				transformed_points_[left_where].get<0>(),
				transformed_points_[top_where].get<1>()
				);
			enclosed_rect_[1].assign(
				transformed_points_[3 - left_where].get<0>(),
				transformed_points_[3 - top_where].get<1>()
				);

			enclosed_size_ = enclosed_rect_[1] - enclosed_rect_[0];
		}
		scaling_type& refScaling() {
			return scaling_;
		}
		rotation_type& refRotation() {
			return rotation_;
		}
		translation_type& refTranslation() {
			return translation_;
		}
		rect_type& refRect() {
			return rect_;
		}
		coord_type& refCenter() {
			return center_;
		}
		scaling_type const& refScaling() const {
			return scaling_;
		}
		rotation_type const& refRotation() const {
			return rotation_;
		}
		translation_type const& refTranslation() const {
			return translation_;
		}
		rect_type const& refRect() const {
			return rect_;
		}
		coord_type const& refCenter() const {
			return center_;
		}
		sprig::call_traits<size_type>::getter_type getSize() const {
			return size_;
		}
		sprig::call_traits<coords_type>::getter_type getPoints() const {
			return points_;
		}
		sprig::call_traits<coords_type>::getter_type getTransformedPoints() const {
			return transformed_points_;
		}
		sprig::call_traits<coords_type>::getter_type getEnclosedPoints() const {
			return enclosed_points_;
		}
		sprig::call_traits<rect_type>::getter_type getEnclosedRect() const {
			return enclosed_rect_;
		}
		sprig::call_traits<size_type>::getter_type getEnclosedSize() const {
			return enclosed_size_;
		}
	private:
		friend class boost::serialization::access;

		template <typename Archive>
		void serialize(Archive& ar, unsigned int const) {
			ar
				& boost::serialization::make_nvp("scaling_", scaling_)
				& boost::serialization::make_nvp("rotation_", rotation_)
				& boost::serialization::make_nvp("translation_", translation_)
				& boost::serialization::make_nvp("rect_", rect_)
				& boost::serialization::make_nvp("center_", center_)
				;
		}
	};
}	// namespace ktl
