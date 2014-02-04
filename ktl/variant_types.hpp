/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef KTL_VARIANT_TYPES_HPP
#define KTL_VARIANT_TYPES_HPP

#include <sprig/config/config.hpp>

#ifdef SPRIG_USING_PRAGMA_ONCE
#	pragma once
#endif	// #ifdef SPRIG_USING_PRAGMA_ONCE

#include <ktl/config.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/unique.hpp>
#include <boost/mpl/sort.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/min_element.hpp>
#include <boost/mpl/max_element.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/assert.hpp>
#include <sprig/external/tp_stub.hpp>

namespace ktl {
	//
	// VariantTypes
	//
	struct VariantTypes {
	public:
		typedef int value_type;
		typedef boost::mpl::vector_c<
			value_type,
			tvtVoid,
			tvtObject,
			tvtString,
			tvtOctet,
			tvtInteger,
			tvtReal
		> types;
		typedef boost::mpl::deref<
			boost::mpl::min_element<types>::type
		>::type min_type;
		typedef boost::mpl::deref<
			boost::mpl::max_element<types>::type
		>::type max_type;
	public:
		static value_type const min_value = min_type::value;
		static value_type const max_value = max_type::value;
	private:
		BOOST_MPL_ASSERT((boost::mpl::bool_<tvtVoid == 0>));
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to<
				min_type,
				boost::mpl::integral_c<value_type, 0>
			>
			));
		BOOST_MPL_ASSERT((
			boost::mpl::equal_to<
				max_type,
				boost::mpl::integral_c<value_type, boost::mpl::size<types>::value - 1>
			>
			));
		BOOST_MPL_ASSERT((
			boost::mpl::equal<
				boost::mpl::unique<types, boost::is_same<boost::mpl::_1, boost::mpl::_2> >::type,
				types
			>
			));
		BOOST_MPL_ASSERT((
			boost::mpl::equal<
				boost::mpl::sort<types>::type,
				types
			>
			));
	};
}	// namespace ktl

#endif	// #ifndef KTL_VARIANT_TYPES_HPP
