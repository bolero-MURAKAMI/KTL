#pragma once

#pragma warning(disable : 4819)
#pragma warning(disable : 4996)

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#ifndef _WIN32_WINNT
#	define _WIN32_WINNT 0x0501
#endif	// #ifndef _WIN32_WINNT
#ifndef WINVER
#	define WINVER 0x0501
#endif	// #ifndef WINVER

#define SPRIG_CONFIG_LIBINFO_MESSAGE
#define SPRIG_CONFIG_MUST_NOMINMAX

//	CONFIG: user config
#define SPRIG_CONFIG_LIB_USERCONFIG
