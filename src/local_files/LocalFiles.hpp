#pragma once

#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>
#include <boost/foreach.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/range/reference.hpp>
#include <boost/range/value_type.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/str_compare.hpp>
#include <sprig/str_cast.hpp>
#include <sprig/predicate.hpp>
#include <sprig/numeric/conversion/cast.hpp>
#include <sprig/krkr/exception.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <ktl/config.hpp>
#include <ktl/error.hpp>

#include "LocalFilesDecl.hpp"

#define KTL_WARNING_SECTION SPRIG_KRKR_TJS_W("KTL:LocalFiles:warning")
#define KTL_ERROR_SECTION SPRIG_KRKR_TJS_W("KTL:LocalFiles:error")

namespace ktl {
	//
	// NativeLocalFiles
	//
	KTL_INLINE tTJSString NativeLocalFiles::getLocalName(tjs_char const* path) {
		tTJSString result(::TVPNormalizeStorageName(path));
		::TVPGetLocalName(result);
		return result;
	}
	KTL_INLINE tTJSString NativeLocalFiles::getErrorString(boost::system::error_code const& error) {
		return sprig::str_cast<tTJSString>(error.message());
	}
	NativeLocalFiles::NativeLocalFiles() {}
	//
	//	SUMMARY: コピー系メソッド
	//
	KTL_INLINE void NativeLocalFiles::copy(tjs_char const* from, tjs_char const* to) {
		boost::filesystem::copy(getLocalName(from).c_str(), getLocalName(to).c_str());
	}
	KTL_INLINE void NativeLocalFiles::copyDirectory(tjs_char const* from, tjs_char const* to) {
		boost::filesystem::copy_directory(getLocalName(from).c_str(), getLocalName(to).c_str());
	}
	KTL_INLINE void NativeLocalFiles::copyFile(tjs_char const* from, tjs_char const* to) {
		boost::filesystem::copy_file(getLocalName(from).c_str(), getLocalName(to).c_str());
	}
	KTL_INLINE void NativeLocalFiles::copySymlink(tjs_char const* existing_symlink, tjs_char const* new_symlink) {
		boost::filesystem::copy_symlink(getLocalName(existing_symlink).c_str(), getLocalName(new_symlink).c_str());
	}
	KTL_INLINE bool NativeLocalFiles::createDirectories(tjs_char const* p) {
		return boost::filesystem::create_directories(getLocalName(p).c_str());
	}
	KTL_INLINE bool NativeLocalFiles::createDirectory(tjs_char const* p) {
		return boost::filesystem::create_directory(getLocalName(p).c_str());
	}
	KTL_INLINE void NativeLocalFiles::createDirectorySymlink(tjs_char const* to, tjs_char const* new_symlink) {
		boost::filesystem::create_directory_symlink(getLocalName(to).c_str(), getLocalName(new_symlink).c_str());
	}
	KTL_INLINE void NativeLocalFiles::createHardLink(tjs_char const* to, tjs_char const* new_hard_link) {
		boost::filesystem::create_hard_link(getLocalName(to).c_str(), getLocalName(new_hard_link).c_str());
	}
	KTL_INLINE void NativeLocalFiles::createSymlink(tjs_char const* to, tjs_char const* new_symlink) {
		boost::filesystem::create_symlink(getLocalName(to).c_str(), getLocalName(new_symlink).c_str());
	}
	//
	//	SUMMARY: 情報系メソッド
	//
	KTL_INLINE bool NativeLocalFiles::exists(tjs_char const* p) {
		return boost::filesystem::exists(getLocalName(p).c_str());
	}
	KTL_INLINE bool NativeLocalFiles::equivalent(tjs_char const* p1, tjs_char const* p2) {
		return boost::filesystem::equivalent(getLocalName(p1).c_str(), getLocalName(p2).c_str());
	}
	KTL_INLINE NativeLocalFiles::uint_type NativeLocalFiles::fileSize(tjs_char const* p) {
		return boost::filesystem::file_size(getLocalName(p).c_str());
	}
	KTL_INLINE NativeLocalFiles::uint_type NativeLocalFiles::hardLinkCount(tjs_char const* p) {
		return boost::filesystem::hard_link_count(getLocalName(p).c_str());
	}
	KTL_INLINE bool NativeLocalFiles::isDirectory(tjs_char const* p) {
		return boost::filesystem::is_directory(getLocalName(p).c_str());
	}
	KTL_INLINE bool NativeLocalFiles::isEmpty(tjs_char const* p) {
		return boost::filesystem::is_empty(getLocalName(p).c_str());
	}
	KTL_INLINE bool NativeLocalFiles::isOther(tjs_char const* p) {
		return boost::filesystem::is_other(getLocalName(p).c_str());
	}
	KTL_INLINE bool NativeLocalFiles::isRegularFile(tjs_char const* p) {
		return boost::filesystem::is_regular_file(getLocalName(p).c_str());
	}
	KTL_INLINE bool NativeLocalFiles::isSymlink(tjs_char const* p) {
		return boost::filesystem::is_symlink(getLocalName(p).c_str());
	}
	KTL_INLINE NativeLocalFiles::time_type NativeLocalFiles::getLastWriteTime(tjs_char const* p) {
		return boost::filesystem::last_write_time(getLocalName(p).c_str());
	}
	KTL_INLINE void NativeLocalFiles::setLastWriteTime(tjs_char const* p, time_type new_time) {
		boost::filesystem::last_write_time(getLocalName(p).c_str(), new_time);
	}
	KTL_INLINE void NativeLocalFiles::permissions(tjs_char const* p, perms_flag_type prms) {
		boost::filesystem::permissions(getLocalName(p).c_str(), prms);
	}
	KTL_INLINE tTJSString NativeLocalFiles::readSymlink(tjs_char const* p) {
		return boost::filesystem::read_symlink(getLocalName(p).c_str()).c_str();
	}
	//
	//	SUMMARY: 削除系メソッド
	//
	KTL_INLINE bool NativeLocalFiles::remove(tjs_char const* p) {
		return boost::filesystem::remove(getLocalName(p).c_str());
	}
	KTL_INLINE NativeLocalFiles::uint_type NativeLocalFiles::removeAll(tjs_char const* p) {
		return boost::filesystem::remove_all(getLocalName(p).c_str());
	}
	//
	//	SUMMARY: 変更系メソッド
	//
	KTL_INLINE void NativeLocalFiles::rename(tjs_char const* old_p, tjs_char const* new_p) {
		boost::filesystem::rename(getLocalName(old_p).c_str(), getLocalName(new_p).c_str());
	}
	KTL_INLINE void NativeLocalFiles::resizeFile(tjs_char const* p, uint_type new_size) {
		boost::filesystem::resize_file(getLocalName(p).c_str(), new_size);
	}
	//
	//	SUMMARY: ステータス系メソッド
	//
	KTL_INLINE tTJSVariant NativeLocalFiles::space(tjs_char const* p) {
		boost::filesystem::space_info info(boost::filesystem::space(getLocalName(p).c_str()));
		sprig::krkr::tjs::object_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
				&result_obj,
				0,
				0,
				0
				);
			result = sprig::krkr::tjs::object_type(result_obj, false);
		}
		{
			tTJSVariant var(tTVInteger(info.capacity));
			sprig::krkr::tjs::AddMember(result.get(), SPRIG_KRKR_TJS_W("capacity"), &var);
		}
		{
			tTJSVariant var(tTVInteger(info.free));
			sprig::krkr::tjs::AddMember(result.get(), SPRIG_KRKR_TJS_W("free"), &var);
		}
		{
			tTJSVariant var(tTVInteger(info.available));
			sprig::krkr::tjs::AddMember(result.get(), SPRIG_KRKR_TJS_W("available"), &var);
		}
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE NativeLocalFiles::uint_type NativeLocalFiles::spaceCapacity(tjs_char const* p) {
		return boost::filesystem::space(getLocalName(p).c_str()).capacity;
	}
	KTL_INLINE NativeLocalFiles::uint_type NativeLocalFiles::spaceFree(tjs_char const* p) {
		return boost::filesystem::space(getLocalName(p).c_str()).free;
	}
	KTL_INLINE NativeLocalFiles::uint_type NativeLocalFiles::spaceAvailable(tjs_char const* p) {
		return boost::filesystem::space(getLocalName(p).c_str()).available;
	}
	KTL_INLINE tTJSVariant NativeLocalFiles::status(tjs_char const* p) {
		boost::filesystem::file_status stat(boost::filesystem::status(getLocalName(p).c_str()));
		sprig::krkr::tjs::object_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
				&result_obj,
				0,
				0,
				0
				);
			result = sprig::krkr::tjs::object_type(result_obj, false);
		}
		{
			tTJSVariant var(tTVInteger(stat.type()));
			sprig::krkr::tjs::AddMember(result.get(), SPRIG_KRKR_TJS_W("type"), &var);
		}
		{
			tTJSVariant var(tTVInteger(stat.permissions()));
			sprig::krkr::tjs::AddMember(result.get(), SPRIG_KRKR_TJS_W("permissions"), &var);
		}
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE NativeLocalFiles::file_type_flag_type NativeLocalFiles::statusType(tjs_char const* p) {
		return boost::filesystem::status(getLocalName(p).c_str()).type();
	}
	KTL_INLINE NativeLocalFiles::perms_flag_type NativeLocalFiles::statusPermissions(tjs_char const* p) {
		return boost::filesystem::status(getLocalName(p).c_str()).permissions();
	}
	KTL_INLINE bool NativeLocalFiles::statusKnown(file_type_flag_type s) {
		return boost::filesystem::status_known(boost::filesystem::file_status(s));
	}
	KTL_INLINE tTJSVariant NativeLocalFiles::symlinkStatus(tjs_char const* p) {
		boost::filesystem::file_status stat(boost::filesystem::symlink_status(getLocalName(p).c_str()));
		sprig::krkr::tjs::object_type result;
		{
			iTJSDispatch2* result_obj = 0;
			sprig::krkr::tjs::CreateNewObject(
				sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Dictionary")),
				&result_obj,
				0,
				0,
				0
				);
			result = sprig::krkr::tjs::object_type(result_obj, false);
		}
		{
			tTJSVariant var(tTVInteger(stat.type()));
			sprig::krkr::tjs::AddMember(result.get(), SPRIG_KRKR_TJS_W("type"), &var);
		}
		{
			tTJSVariant var(tTVInteger(stat.permissions()));
			sprig::krkr::tjs::AddMember(result.get(), SPRIG_KRKR_TJS_W("permissions"), &var);
		}
		return tTJSVariant(result.get(), result.get());
	}
	KTL_INLINE NativeLocalFiles::file_type_flag_type NativeLocalFiles::symlinkStatusType(tjs_char const* p) {
		return boost::filesystem::symlink_status(getLocalName(p).c_str()).type();
	}
	KTL_INLINE NativeLocalFiles::perms_flag_type NativeLocalFiles::symlinkStatusPermissions(tjs_char const* p) {
		return boost::filesystem::symlink_status(getLocalName(p).c_str()).permissions();
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE tTJSString NativeLocalFiles::tempDirectoryPath() {
		return boost::filesystem::temp_directory_path().c_str();
	}
	KTL_INLINE tTJSString NativeLocalFiles::uniquePath(tjs_char const* model) {
		return boost::filesystem::unique_path(model).c_str();
	}
	//
	//	SUMMARY: 列挙系メソッド
	//
	KTL_INLINE tTJSVariant NativeLocalFiles::enumDirectoryEntry(
		tjs_char const* p,
		tTJSVariant const* sort_comp,
		bool recurse,
		int level_max,
		bool symlink_recurse
		)
	{
		typedef std::vector<boost::filesystem::path> vector_type;

		boost::filesystem::path base(getLocalName(p).c_str());
		vector_type v;
		if (recurse) {
			typedef boost::filesystem::recursive_directory_iterator iterator_type;
			if (level_max >= 0) {
				for (
					iterator_type i = iterator_type(
						base,
						symlink_recurse
							? boost::filesystem::symlink_option::recurse
							: boost::filesystem::symlink_option::no_recurse
						),
						last = iterator_type()
						;
					i != last;
					++i
					)
				{
					i.no_push(i.level() == level_max);
					v.push_back(*i);
				}
			} else {
				std::copy(
					iterator_type(
						base,
						symlink_recurse
							? boost::filesystem::symlink_option::recurse
							: boost::filesystem::symlink_option::no_recurse
						),
					iterator_type(),
					std::back_inserter(v)
					);
			}
		} else {
			typedef boost::filesystem::directory_iterator iterator_type;
			std::copy(
				iterator_type(base),
				iterator_type(),
				std::back_inserter(v)
				);
		}
		iTJSDispatch2* result_obj = 0;
		sprig::krkr::tjs::CreateNewObject(
			sprig::krkr::tjs::GetTJSClassNoAddRef(SPRIG_KRKR_TJS_W("Array")),
			&result_obj,
			0,
			0,
			0
			);
		sprig::krkr::tjs::object_type obj(result_obj, false);
		if (sort_comp && sort_comp->Type() != tvtVoid) {
			typedef std::vector<sprig::krkr::tjs::string_type> string_vector_type;

			string_vector_type string_v;
			BOOST_FOREACH(boost::range_reference<vector_type>::type e, v){
				string_v.push_back(e.c_str());
			}
			switch (sort_comp->Type()) {
			case tvtString:
				{
					tjs_char const* comp_op = sprig::krkr::tjs::as_c_str(sort_comp->AsStringNoAddRef());
					if (sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("+")) == 0
						|| sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("a")) == 0
						)
					{
						boost::sort(string_v);
					} else if (sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("-")) == 0
						|| sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("z")) == 0
						)
					{
						boost::sort(string_v, std::greater<boost::range_value<string_vector_type>::type>());
					} else if (sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("ia")) == 0) {
						boost::sort(string_v, sprig::predicate::lexicographical_compare());
					} else if (sprig::str_compare(comp_op, SPRIG_KRKR_TJS_W("iz")) == 0) {
						boost::sort(string_v, sprig::predicate::lexicographical_compare<sprig::predicate::is_igreater>());
					}
				}
				break;
			default:
				return tTJSVariant();
			}
			{
				tjs_int num = 0;
				BOOST_FOREACH(boost::range_reference<string_vector_type>::type e, string_v){
					tTJSVariant param(e.c_str());
					sprig::krkr::tjs::PropSetByNum(
						obj.get(),
						TJS_MEMBERENSURE,
						num,
						&param,
						obj.get()
						);
					++num;
				}
			}
		} else {
			tjs_int num = 0;
			BOOST_FOREACH(boost::range_reference<vector_type>::type e, v){
				tTJSVariant param(e.c_str());
				sprig::krkr::tjs::PropSetByNum(
					obj.get(),
					TJS_MEMBERENSURE,
					num,
					&param,
					obj.get()
					);
				++num;
			}
		}
		return tTJSVariant(obj.get(), obj.get());
	}

	//
	// LocalFiles
	//
	LocalFiles::LocalFiles() {}
	tjs_error TJS_INTF_METHOD LocalFiles::Construct(
		tjs_int numparams,
		tTJSVariant** param,
		iTJSDispatch2* tjs_obj
		)
	{
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("LocalFiles::Construct"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_ = boost::make_shared<NativeLocalFiles>();
		return TJS_S_OK;
	}
	void TJS_INTF_METHOD LocalFiles::Invalidate() {
		SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("LocalFiles::Invalidate"), SPRIG_KRKR_LOG_LEVEL_NORMAL);
		instance_.reset();
	}
	//
	//	SUMMARY: コピー系メソッド
	//
	KTL_INLINE void LocalFiles::copy(tTJSVariantString const* from, tTJSVariantString const* to) {
		NativeLocalFiles::copy(
			sprig::krkr::tjs::as_c_str(from),
			sprig::krkr::tjs::as_c_str(to)
			);
	}
	KTL_INLINE void LocalFiles::copyDirectory(tTJSVariantString const* from, tTJSVariantString const* to) {
		NativeLocalFiles::copyDirectory(
			sprig::krkr::tjs::as_c_str(from),
			sprig::krkr::tjs::as_c_str(to)
			);
	}
	KTL_INLINE void LocalFiles::copyFile(tTJSVariantString const* from, tTJSVariantString const* to) {
		NativeLocalFiles::copyFile(
			sprig::krkr::tjs::as_c_str(from),
			sprig::krkr::tjs::as_c_str(to)
			);
	}
	KTL_INLINE void LocalFiles::copySymlink(tTJSVariantString const* existing_symlink, tTJSVariantString const* new_symlink) {
		NativeLocalFiles::copySymlink(
			sprig::krkr::tjs::as_c_str(existing_symlink),
			sprig::krkr::tjs::as_c_str(new_symlink)
			);
	}
	//
	//	SUMMARY: 作成系メソッド
	//
	KTL_INLINE bool LocalFiles::createDirectories(tTJSVariantString const* p) {
		return NativeLocalFiles::createDirectories(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE bool LocalFiles::createDirectory(tTJSVariantString const* p) {
		return NativeLocalFiles::createDirectory(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE void LocalFiles::createDirectorySymlink(tTJSVariantString const* to, tTJSVariantString const* new_symlink) {
		NativeLocalFiles::createDirectorySymlink(
			sprig::krkr::tjs::as_c_str(to),
			sprig::krkr::tjs::as_c_str(new_symlink)
			);
	}
	KTL_INLINE void LocalFiles::createHardLink(tTJSVariantString const* to, tTJSVariantString const* new_hard_link) {
		NativeLocalFiles::createHardLink(
			sprig::krkr::tjs::as_c_str(to),
			sprig::krkr::tjs::as_c_str(new_hard_link)
			);
	}
	KTL_INLINE void LocalFiles::createSymlink(tTJSVariantString const* to, tTJSVariantString const* new_symlink) {
		NativeLocalFiles::createSymlink(
			sprig::krkr::tjs::as_c_str(to),
			sprig::krkr::tjs::as_c_str(new_symlink)
			);
	}
	//
	//	SUMMARY: 情報系メソッド
	//
	KTL_INLINE bool LocalFiles::exists(tTJSVariantString const* p) {
		return NativeLocalFiles::exists(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE bool LocalFiles::equivalent(tTJSVariantString const* p1, tTJSVariantString const* p2) {
		return NativeLocalFiles::equivalent(
			sprig::krkr::tjs::as_c_str(p1),
			sprig::krkr::tjs::as_c_str(p2)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::fileSize(tTJSVariantString const* p) {
		return NativeLocalFiles::fileSize(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::hardLinkCount(tTJSVariantString const* p) {
		return NativeLocalFiles::hardLinkCount(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE bool LocalFiles::isDirectory(tTJSVariantString const* p) {
		return NativeLocalFiles::isDirectory(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE bool LocalFiles::isEmpty(tTJSVariantString const* p) {
		return NativeLocalFiles::isEmpty(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE bool LocalFiles::isOther(tTJSVariantString const* p) {
		return NativeLocalFiles::isOther(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE bool LocalFiles::isRegularFile(tTJSVariantString const* p) {
		return NativeLocalFiles::isRegularFile(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE bool LocalFiles::isSymlink(tTJSVariantString const* p) {
		return NativeLocalFiles::isSymlink(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::getLastWriteTime(tTJSVariantString const* p) {
		return NativeLocalFiles::getLastWriteTime(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE void LocalFiles::setLastWriteTime(tTJSVariantString const* p, tTVInteger new_time) {
		return NativeLocalFiles::setLastWriteTime(
			sprig::krkr::tjs::as_c_str(p),
			new_time
			);
	}
	KTL_INLINE void LocalFiles::permissions(tTJSVariantString const* p, tTVInteger prms) {
		return NativeLocalFiles::permissions(
			sprig::krkr::tjs::as_c_str(p),
			sprig::numeric::bit_cast<NativeLocalFiles::perms_flag_type>(prms)
			);
	}
	KTL_INLINE tTJSString LocalFiles::readSymlink(tTJSVariantString const* p) {
		return NativeLocalFiles::readSymlink(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	//
	//	SUMMARY: 削除系メソッド
	//
	KTL_INLINE bool LocalFiles::remove(tTJSVariantString const* p) {
		return NativeLocalFiles::remove(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::removeAll(tTJSVariantString const* p) {
		return NativeLocalFiles::removeAll(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	//
	//	SUMMARY: 変更系メソッド
	//
	KTL_INLINE void LocalFiles::rename(tTJSVariantString const* old_p, tTJSVariantString const* new_p) {
		NativeLocalFiles::rename(
			sprig::krkr::tjs::as_c_str(old_p),
			sprig::krkr::tjs::as_c_str(new_p)
			);
	}
	KTL_INLINE void LocalFiles::resizeFile(tTJSVariantString const* p, tTVInteger new_size) {
		NativeLocalFiles::resizeFile(
			sprig::krkr::tjs::as_c_str(p),
			new_size
			);
	}
	//
	//	SUMMARY: ステータス系メソッド
	//
	KTL_INLINE tTJSVariant LocalFiles::space(tTJSVariantString const* p) {
		return NativeLocalFiles::space(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::spaceCapacity(tTJSVariantString const* p) {
		return NativeLocalFiles::spaceCapacity(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::spaceFree(tTJSVariantString const* p) {
		return NativeLocalFiles::spaceFree(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::spaceAvailable(tTJSVariantString const* p) {
		return NativeLocalFiles::spaceAvailable(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTJSVariant LocalFiles::status(tTJSVariantString const* p) {
		return NativeLocalFiles::status(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::statusType(tTJSVariantString const* p) {
		return NativeLocalFiles::statusType(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::statusPermissions(tTJSVariantString const* p) {
		return NativeLocalFiles::statusPermissions(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE bool LocalFiles::statusKnown(tTVInteger s) {
		return NativeLocalFiles::statusKnown(
			sprig::numeric::bit_cast<NativeLocalFiles::file_type_flag_type>(s)
			);
	}
	KTL_INLINE tTJSVariant LocalFiles::symlinkStatus(tTJSVariantString const* p) {
		return NativeLocalFiles::symlinkStatus(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::symlinkStatusType(tTJSVariantString const* p) {
		return NativeLocalFiles::symlinkStatusType(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	KTL_INLINE tTVInteger LocalFiles::symlinkStatusPermissions(tTJSVariantString const* p) {
		return NativeLocalFiles::symlinkStatusPermissions(
			sprig::krkr::tjs::as_c_str(p)
			);
	}
	//
	//	SUMMARY: 取得系メソッド
	//
	KTL_INLINE tTJSString LocalFiles::tempDirectoryPath() {
		return NativeLocalFiles::tempDirectoryPath();
	}
	KTL_INLINE tTJSString LocalFiles::uniquePath(tTJSVariantString const* model) {
		return model
			? NativeLocalFiles::uniquePath(
				sprig::krkr::tjs::as_c_str(model)
				)
			: NativeLocalFiles::uniquePath()
			;
	}
	//
	//	SUMMARY: 列挙系メソッド
	//
	KTL_INLINE tTJSVariant LocalFiles::enumDirectoryEntry(
		tTJSVariantString const* p,
		tTJSVariant const* sort_comp,
		bool recurse,
		tTVInteger level_max,
		bool symlink_recurse
		)
	{
		return NativeLocalFiles::enumDirectoryEntry(
			sprig::krkr::tjs::as_c_str(p),
			sort_comp,
			recurse,
			boost::numeric_cast<int>(level_max),
			symlink_recurse
			);
	}
}	// namespace ktl

#undef KTL_WARNING_SECTION
#undef KTL_ERROR_SECTION
