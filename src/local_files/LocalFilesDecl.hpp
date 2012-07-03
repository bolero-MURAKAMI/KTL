#pragma once

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <boost/filesystem/operations.hpp>
#include <sprig/external/tp_stub.hpp>
#include <sprig/krkr/tjs.hpp>

namespace ktl {
	//
	// NativeLocalFiles
	//
	class NativeLocalFiles {
	public:
		typedef boost::filesystem::file_type file_type_flag_type;
		typedef boost::filesystem::perms perms_flag_type;
		typedef boost::uintmax_t uint_type;
		typedef std::time_t time_type;
	public:
		static tTJSString getLocalName(tjs_char const* path);
		static tTJSString getErrorString(boost::system::error_code const& error);
	public:
		NativeLocalFiles();
	public:
		//
		//	SUMMARY: ファイルタイプ系フラグ
		//
		static file_type_flag_type const ftfStatusError = boost::filesystem::status_error;
		static file_type_flag_type const ftfFileNotFound = boost::filesystem::file_not_found;
		static file_type_flag_type const ftfRegularFile = boost::filesystem::regular_file;
		static file_type_flag_type const ftfDirectoryFile = boost::filesystem::directory_file;
		static file_type_flag_type const ftfSymlinkFile = boost::filesystem::symlink_file;
		static file_type_flag_type const ftfBlockFile = boost::filesystem::block_file;
		static file_type_flag_type const ftfCharacterFile = boost::filesystem::character_file;
		static file_type_flag_type const ftfFIFOFile = boost::filesystem::fifo_file;
		static file_type_flag_type const ftfSocketFile = boost::filesystem::socket_file;
		static file_type_flag_type const ftfReparseFile = boost::filesystem::reparse_file;
		static file_type_flag_type const ftfTypeUnknown = boost::filesystem::type_unknown;
		//
		//	SUMMARY: パーミッション系フラグ
		//
		static perms_flag_type const pfNoPerms = boost::filesystem::no_perms;
		static perms_flag_type const pfOwnerRead = boost::filesystem::owner_read;
		static perms_flag_type const pfOwnerWrite = boost::filesystem::owner_write;
		static perms_flag_type const pfOwnerExe = boost::filesystem::owner_exe;
		static perms_flag_type const pfOwnerAll = boost::filesystem::owner_all;
		static perms_flag_type const pfGroupRead = boost::filesystem::group_read;
		static perms_flag_type const pfGroupWrite = boost::filesystem::group_write;
		static perms_flag_type const pfGroupExe = boost::filesystem::group_exe;
		static perms_flag_type const pfGroupAll = boost::filesystem::group_all;
		static perms_flag_type const pfOthersRead = boost::filesystem::others_read;
		static perms_flag_type const pfOthersWrite = boost::filesystem::others_write;
		static perms_flag_type const pfOthersExe = boost::filesystem::others_exe;
		static perms_flag_type const pfOthersAll = boost::filesystem::others_all;
		static perms_flag_type const pfAllAll = boost::filesystem::all_all;
		static perms_flag_type const pfUIDOnExe = boost::filesystem::set_uid_on_exe;
		static perms_flag_type const pfGIDOnExe = boost::filesystem::set_gid_on_exe;
		static perms_flag_type const pfStickyBit = boost::filesystem::sticky_bit;
		static perms_flag_type const pfPermsMask = boost::filesystem::perms_mask;
		static perms_flag_type const pfPermsNotKnown = boost::filesystem::perms_not_known;
		static perms_flag_type const pfAddPerms = boost::filesystem::add_perms;
		static perms_flag_type const pfRemovePerms = boost::filesystem::remove_perms;
		static perms_flag_type const pfSymlinkPerms = boost::filesystem::symlink_perms;
	public:
		//
		//	SUMMARY: コピー系メソッド
		//
		static void copy(tjs_char const* from, tjs_char const* to);
		static void copyDirectory(tjs_char const* from, tjs_char const* to);
		static void copyFile(tjs_char const* from, tjs_char const* to);
		static void copySymlink(tjs_char const* existing_symlink, tjs_char const* new_symlink);
		//
		//	SUMMARY: 作成系メソッド
		//
		static bool createDirectories(tjs_char const* p);
		static bool createDirectory(tjs_char const* p);
		static void createDirectorySymlink(tjs_char const* to, tjs_char const* new_symlink);
		static void createHardLink(tjs_char const* to, tjs_char const* new_hard_link);
		static void createSymlink(tjs_char const* to, tjs_char const* new_symlink);
		//
		//	SUMMARY: 情報系メソッド
		//
		static bool exists(tjs_char const* p);
		static bool equivalent(tjs_char const* p1, tjs_char const* p2);
		static uint_type fileSize(tjs_char const* p);
		static uint_type hardLinkCount(tjs_char const* p);
		static bool isDirectory(tjs_char const* p);
		static bool isEmpty(tjs_char const* p);
		static bool isOther(tjs_char const* p);
		static bool isRegularFile(tjs_char const* p);
		static bool isSymlink(tjs_char const* p);
		static time_type getLastWriteTime(tjs_char const* p);
		static void setLastWriteTime(tjs_char const* p, time_type new_time);
		static void permissions(tjs_char const* p, perms_flag_type prms);
		static tTJSString readSymlink(tjs_char const* p);
		//
		//	SUMMARY: 削除系メソッド
		//
		static bool remove(tjs_char const* p);
		static uint_type removeAll(tjs_char const* p);
		//
		//	SUMMARY: 変更系メソッド
		//
		static void rename(tjs_char const* old_p, tjs_char const* new_p);
		static void resizeFile(tjs_char const* p, uint_type new_size);
		//
		//	SUMMARY: ステータス系メソッド
		//
		static tTJSVariant space(tjs_char const* p);
		static uint_type spaceCapacity(tjs_char const* p);
		static uint_type spaceFree(tjs_char const* p);
		static uint_type spaceAvailable(tjs_char const* p);
		static tTJSVariant status(tjs_char const* p);
		static file_type_flag_type statusType(tjs_char const* p);
		static perms_flag_type statusPermissions(tjs_char const* p);
		static bool statusKnown(file_type_flag_type s);
		static tTJSVariant symlinkStatus(tjs_char const* p);
		static file_type_flag_type symlinkStatusType(tjs_char const* p);
		static perms_flag_type symlinkStatusPermissions(tjs_char const* p);
		//
		//	SUMMARY: 取得系メソッド
		//
		static tTJSString tempDirectoryPath();
		static tTJSString uniquePath(tjs_char const* model = SPRIG_KRKR_TJS_W("%%%%-%%%%-%%%%-%%%%"));
		//
		//	SUMMARY: 列挙系メソッド
		//
		static tTJSVariant enumDirectoryEntry(
			tjs_char const* p,
			tTJSVariant const* sort_comp = 0,
			bool recurse = false,
			int level_max = -1,
			bool symlink_recurse = false
			);
	};

	//
	// LocalFiles
	//
	class LocalFiles
		: public tTJSNativeInstance
	{
	private:
		boost::shared_ptr<NativeLocalFiles> instance_;
	public:
		LocalFiles();
		tjs_error TJS_INTF_METHOD Construct(
			tjs_int numparams,
			tTJSVariant** param,
			iTJSDispatch2* tjs_obj
			);
		void TJS_INTF_METHOD Invalidate();
	public:
		//
		//	SUMMARY: ファイルタイプ系フラグ
		//
		static tTVInteger const ftfStatusError = NativeLocalFiles::ftfStatusError;
		static tTVInteger const ftfFileNotFound = NativeLocalFiles::ftfFileNotFound;
		static tTVInteger const ftfRegularFile = NativeLocalFiles::ftfRegularFile;
		static tTVInteger const ftfDirectoryFile = NativeLocalFiles::ftfDirectoryFile;
		static tTVInteger const ftfSymlinkFile = NativeLocalFiles::ftfSymlinkFile;
		static tTVInteger const ftfBlockFile = NativeLocalFiles::ftfBlockFile;
		static tTVInteger const ftfCharacterFile = NativeLocalFiles::ftfCharacterFile;
		static tTVInteger const ftfFIFOFile = NativeLocalFiles::ftfFIFOFile;
		static tTVInteger const ftfSocketFile = NativeLocalFiles::ftfSocketFile;
		static tTVInteger const ftfReparseFile = NativeLocalFiles::ftfReparseFile;
		static tTVInteger const ftfTypeUnknown = NativeLocalFiles::ftfTypeUnknown;
		//
		//	SUMMARY: パーミッション系フラグ
		//
		static tTVInteger const pfNoPerms = NativeLocalFiles::pfNoPerms;
		static tTVInteger const pfOwnerRead = NativeLocalFiles::pfOwnerRead;
		static tTVInteger const pfOwnerWrite = NativeLocalFiles::pfOwnerWrite;
		static tTVInteger const pfOwnerExe = NativeLocalFiles::pfOwnerExe;
		static tTVInteger const pfOwnerAll = NativeLocalFiles::pfOwnerAll;
		static tTVInteger const pfGroupRead = NativeLocalFiles::pfGroupRead;
		static tTVInteger const pfGroupWrite = NativeLocalFiles::pfGroupWrite;
		static tTVInteger const pfGroupExe = NativeLocalFiles::pfGroupExe;
		static tTVInteger const pfGroupAll = NativeLocalFiles::pfGroupAll;
		static tTVInteger const pfOthersRead = NativeLocalFiles::pfOthersRead;
		static tTVInteger const pfOthersWrite = NativeLocalFiles::pfOthersWrite;
		static tTVInteger const pfOthersExe = NativeLocalFiles::pfOthersExe;
		static tTVInteger const pfOthersAll = NativeLocalFiles::pfOthersAll;
		static tTVInteger const pfAllAll = NativeLocalFiles::pfAllAll;
		static tTVInteger const pfUIDOnExe = NativeLocalFiles::pfUIDOnExe;
		static tTVInteger const pfGIDOnExe = NativeLocalFiles::pfGIDOnExe;
		static tTVInteger const pfStickyBit = NativeLocalFiles::pfStickyBit;
		static tTVInteger const pfPermsMask = NativeLocalFiles::pfPermsMask;
		static tTVInteger const pfPermsNotKnown = NativeLocalFiles::pfPermsNotKnown;
		static tTVInteger const pfAddPerms = NativeLocalFiles::pfAddPerms;
		static tTVInteger const pfRemovePerms = NativeLocalFiles::pfRemovePerms;
		static tTVInteger const pfSymlinkPerms = NativeLocalFiles::pfSymlinkPerms;
	public:
		//
		//	SUMMARY: コピー系メソッド
		//
		static void copy(tTJSVariantString const* from, tTJSVariantString const* to);
		static void copyDirectory(tTJSVariantString const* from, tTJSVariantString const* to);
		static void copyFile(tTJSVariantString const* from, tTJSVariantString const* to);
		static void copySymlink(tTJSVariantString const* existing_symlink, tTJSVariantString const* new_symlink);
		//
		//	SUMMARY: 作成系メソッド
		//
		static bool createDirectories(tTJSVariantString const* p);
		static bool createDirectory(tTJSVariantString const* p);
		static void createDirectorySymlink(tTJSVariantString const* to, tTJSVariantString const* new_symlink);
		static void createHardLink(tTJSVariantString const* to, tTJSVariantString const* new_hard_link);
		static void createSymlink(tTJSVariantString const* to, tTJSVariantString const* new_symlink);
		//
		//	SUMMARY: 情報系メソッド
		//
		static bool exists(tTJSVariantString const* p);
		static bool equivalent(tTJSVariantString const* p1, tTJSVariantString const* p2);
		static tTVInteger fileSize(tTJSVariantString const* p);
		static tTVInteger hardLinkCount(tTJSVariantString const* p);
		static bool isDirectory(tTJSVariantString const* p);
		static bool isEmpty(tTJSVariantString const* p);
		static bool isOther(tTJSVariantString const* p);
		static bool isRegularFile(tTJSVariantString const* p);
		static bool isSymlink(tTJSVariantString const* p);
		static tTVInteger getLastWriteTime(tTJSVariantString const* p);
		static void setLastWriteTime(tTJSVariantString const* p, tTVInteger new_time);
		static void permissions(tTJSVariantString const* p, tTVInteger prms);
		static tTJSString readSymlink(tTJSVariantString const* p);
		//
		//	SUMMARY: 削除系メソッド
		//
		static bool remove(tTJSVariantString const* p);
		static tTVInteger removeAll(tTJSVariantString const* p);
		//
		//	SUMMARY: 変更系メソッド
		//
		static void rename(tTJSVariantString const* old_p, tTJSVariantString const* new_p);
		static void resizeFile(tTJSVariantString const* p, tTVInteger new_size);
		//
		//	SUMMARY: ステータス系メソッド
		//
		static tTJSVariant space(tTJSVariantString const* p);
		static tTVInteger spaceCapacity(tTJSVariantString const* p);
		static tTVInteger spaceFree(tTJSVariantString const* p);
		static tTVInteger spaceAvailable(tTJSVariantString const* p);
		static tTJSVariant status(tTJSVariantString const* p);
		static tTVInteger statusType(tTJSVariantString const* p);
		static tTVInteger statusPermissions(tTJSVariantString const* p);
		static bool statusKnown(tTVInteger s);
		static tTJSVariant symlinkStatus(tTJSVariantString const* p);
		static tTVInteger symlinkStatusType(tTJSVariantString const* p);
		static tTVInteger symlinkStatusPermissions(tTJSVariantString const* p);
		//
		//	SUMMARY: 取得系メソッド
		//
		static tTJSString tempDirectoryPath();
		static tTJSString uniquePath(tTJSVariantString const* model = 0);
		//
		//	SUMMARY: 列挙系メソッド
		//
		static tTJSVariant enumDirectoryEntry(
			tTJSVariantString const* p,
			tTJSVariant const* sort_comp = 0,
			bool recurse = false,
			tTVInteger level_max = -1,
			bool symlink_recurse = false
			);
	};
}	// namespace ktl
