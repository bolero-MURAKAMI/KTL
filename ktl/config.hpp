#ifndef KTL_CONFIG_HPP
#define KTL_CONFIG_HPP

#include <sprig/config/config.hpp>

#ifdef SPRIG_USING_PRAGMA_ONCE
#	pragma once
#endif	// #ifdef SPRIG_USING_PRAGMA_ONCE

//
// KTL_INLINE
//
#ifndef KTL_INLINE
#	define KTL_INLINE SPRIG_INLINE
#endif	// #ifndef KTL_INLINE

//
// KTL_STORAGE_BUFFER_SIZE
//
#ifndef KTL_STORAGE_BUFFER_SIZE
#	define KTL_STORAGE_BUFFER_SIZE 0x4000
#endif	// #ifndef KTL_STORAGE_BUFFER_SIZE

#endif	// #ifndef KTL_CONFIG_HPP
