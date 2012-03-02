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
		static file_type_flag_type status(tjs_char const* p);
		static bool statusKnown(file_type_flag_type s);
		static file_type_flag_type symlinkStatus(tjs_char const* p);
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
		static tTVInteger status(tTJSVariantString const* p);
		static bool statusKnown(tTVInteger s);
		static tTVInteger symlinkStatus(tTJSVariantString const* p);
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
