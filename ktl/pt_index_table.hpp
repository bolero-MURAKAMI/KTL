/*=============================================================================
  Copyright (c) 2010-2014 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef KTL_PT_INDEX_TABLE_HPP
#define KTL_PT_INDEX_TABLE_HPP

#include <sprig/config/config.hpp>

#ifdef SPRIG_USING_PRAGMA_ONCE
#	pragma once
#endif	// #ifdef SPRIG_USING_PRAGMA_ONCE

#include <ktl/config.hpp>

#include <vector>
#include <sprig/external/tp_stub.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/krkr/macro.hpp>

namespace ktl {
	//
	// PTIndexTable
	//
	template<typename String>
	class PTIndexTable {
	public:
		typedef String string_type;
		typedef std::vector<string_type> table_type;
		typedef typename table_type::size_type size_type;
	private:
		string_type prefix_;
		table_type table_;
	public:
		PTIndexTable()
			: prefix_(SPRIG_KRKR_TJS_W("_"))
		{}
		explicit PTIndexTable(string_type const& prefix)
			: prefix_(prefix)
		{}
		void reserve(size_type size) {
			for (size_type i = table_.size(); i < size; ++i) {
				table_.push_back(
					prefix_ + sprig::str_cast<string_type>(i)
					);
			}
		}
		void resize(size_type size) {
			if (size > table_.size()) {
				reserve(size);
			} else {
				table_.resize(size);
			}
		}
		string_type const& at(size_type i) const {
			reserve(i + 1);
			return table_[i];
		}
		string_type const& operator[](size_type i) const {
			return table_.at(i);
		}
		template<typename OtherString>
		size_type get(OtherString const& str) const {
			return boost::lexical_cast<size_type>(str.c_str() + std::wcslen(prefix_.c_str()));
		}
	};
}	// namespace ktl

#endif	// #ifndef KTL_PT_INDEX_TABLE_HPP
