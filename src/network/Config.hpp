/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef SRC_NETWORK_CONFIG_HPP
#define SRC_NETWORK_CONFIG_HPP

#include <ktl/default_config.hpp>

#if defined(_MSC_VER)
#	pragma warning(disable : 4503)
#endif

//	CONFIG: KTL.Network
#define KTL_NETWORK_CONFIG_ON_REGIST_RUN
#define KTL_NETWORK_CONFIG_ON_UNREGIST_KILL

//	CONFIG: user config
#define SPRIG_CONFIG_LIB_USERCONFIG

#endif	// #ifndef SRC_NETWORK_CONFIG_HPP
