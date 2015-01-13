/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_GEOMETRY_CONFIG_HPP
#define SRC_GEOMETRY_CONFIG_HPP

#include <ktl/default_config.hpp>

//	CONFIG: user config
#define SPRIG_CONFIG_LIB_USERCONFIG

//	CONFIG: geometry
#define BOOST_GEOMETRY_IMPLEMENTATION_STATUS_BUILD true
#include <boost/mpl/identity.hpp>
#include <boost/geometry/algorithms/not_implemented.hpp>

#endif	// #ifndef SRC_GEOMETRY_CONFIG_HPP
