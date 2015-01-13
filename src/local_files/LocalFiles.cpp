/*=============================================================================
  Copyright (c) 2010-2015 Bolero MURAKAMI
  https://github.com/bolero-MURAKAMI/KTL

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "Config.hpp"

#include <sprig/external/tp_stub.hpp>
#include <sprig/external/ncbind/ncbind.hpp>
#include <sprig/krkr/tjs.hpp>
#include <sprig/krkr/macro.hpp>
#include <sprig/krkr/ncb.hpp>
#include <ktl/config.hpp>
#include <ktl/information.hpp>

#include "LocalFiles.hpp"

namespace ktl {
	//
	// CreateNativeClassLocalFiles
	//
#define TJS_NATIVE_CLASSID_NAME LocalFilesClassID
	SPRIG_KRKR_BEGIN_CREATE_NATIVE_CLASS(CreateNativeClassLocalFiles, LocalFiles, LocalFiles)
	//
	//	SUMMARY: ファイルタイプ系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfStatusError, LocalFiles::ftfStatusError);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfFileNotFound, LocalFiles::ftfFileNotFound);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfRegularFile, LocalFiles::ftfRegularFile);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfDirectoryFile, LocalFiles::ftfDirectoryFile);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfSymlinkFile, LocalFiles::ftfSymlinkFile);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfBlockFile, LocalFiles::ftfBlockFile);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfCharacterFile, LocalFiles::ftfCharacterFile);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfFIFOFile, LocalFiles::ftfFIFOFile);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfSocketFile, LocalFiles::ftfSocketFile);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfReparseFile, LocalFiles::ftfReparseFile);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(ftfTypeUnknown, LocalFiles::ftfTypeUnknown);
	//
	//	SUMMARY: パーミッション系フラグ
	//
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfNoPerms, LocalFiles::pfNoPerms);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfOwnerRead, LocalFiles::pfOwnerRead);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfOwnerWrite, LocalFiles::pfOwnerWrite);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfOwnerExe, LocalFiles::pfOwnerExe);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfOwnerAll, LocalFiles::pfOwnerAll);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfGroupRead, LocalFiles::pfGroupRead);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfGroupWrite, LocalFiles::pfGroupWrite);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfGroupExe, LocalFiles::pfGroupExe);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfGroupAll, LocalFiles::pfGroupAll);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfOthersRead, LocalFiles::pfOthersRead);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfOthersWrite, LocalFiles::pfOthersWrite);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfOthersExe, LocalFiles::pfOthersExe);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfOthersAll, LocalFiles::pfOthersAll);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfAllAll, LocalFiles::pfAllAll);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfUIDOnExe, LocalFiles::pfUIDOnExe);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfGIDOnExe, LocalFiles::pfGIDOnExe);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfStickyBit, LocalFiles::pfStickyBit);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfPermsMask, LocalFiles::pfPermsMask);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfPermsNotKnown, LocalFiles::pfPermsNotKnown);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfAddPerms, LocalFiles::pfAddPerms);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfRemovePerms, LocalFiles::pfRemovePerms);
	SPRIG_KRKR_NATIVE_CONSTANT_PROP_DECL(pfSymlinkPerms, LocalFiles::pfSymlinkPerms);
	//
	//	SUMMARY: コピー系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(copy)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		LocalFiles::copy(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(copy);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(copyDirectory)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		LocalFiles::copyDirectory(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(copyDirectory);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(copyFile)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		LocalFiles::copyFile(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(copyFile);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(copySymlink)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		LocalFiles::copySymlink(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(copySymlink);
	//
	//	SUMMARY: 作成系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(createDirectories)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			LocalFiles::createDirectories(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(createDirectories);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(createDirectory)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			LocalFiles::createDirectory(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(createDirectory);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(createDirectorySymlink)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		LocalFiles::createDirectorySymlink(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(createDirectorySymlink);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(createHardLink)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		LocalFiles::createHardLink(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(createHardLink);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(createSymlink)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		LocalFiles::createSymlink(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(createSymlink);
	//
	//	SUMMARY: 情報系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(exists)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::exists(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(exists);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(equivalent)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::equivalent(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(equivalent);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(fileSize)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::fileSize(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(fileSize);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(hardLinkCount)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::hardLinkCount(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(hardLinkCount);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(isDirectory)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::isDirectory(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(isDirectory);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(isEmpty)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::isEmpty(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(isEmpty);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(isOther)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::isOther(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(isOther);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(isRegularFile)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::isRegularFile(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(isRegularFile);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(isSymlink)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::isSymlink(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(isSymlink);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(getLastWriteTime)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::getLastWriteTime(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(getLastWriteTime);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(setLastWriteTime)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		LocalFiles::setLastWriteTime(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(setLastWriteTime);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(permissions)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		LocalFiles::permissions(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(permissions);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(readSymlink)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::readSymlink(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(readSymlink);
	//
	//	SUMMARY: 削除系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(remove)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			LocalFiles::remove(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(remove);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(removeAll)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_INVOKE_RESULT_SET(
			LocalFiles::removeAll(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(removeAll);
	//
	//	SUMMARY: 変更系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(rename)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_TYPE_CHECK(1, tvtString);
		LocalFiles::rename(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_STRING(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(rename);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(resizeFile)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(2);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		LocalFiles::resizeFile(SPRIG_KRKR_ARG_STRING(0), SPRIG_KRKR_ARG_VARIANT(1));
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(resizeFile);
	//
	//	SUMMARY: ステータス系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(space)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::space(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(space);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(spaceCapacity)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::spaceCapacity(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(spaceCapacity);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(spaceFree)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::spaceFree(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(spaceFree);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(spaceAvailable)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::spaceAvailable(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(spaceAvailable);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(status)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::status(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(status);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(statusType)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::statusType(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(statusType);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(statusPermissions)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::statusPermissions(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(statusPermissions);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(statusKnown)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtInteger);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::statusKnown(SPRIG_KRKR_ARG_VARIANT(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(statusKnown);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(symlinkStatus)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::symlinkStatus(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(symlinkStatus);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(symlinkStatusType)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::symlinkStatusType(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(symlinkStatusType);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(symlinkStatusPermissions)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::symlinkStatusPermissions(SPRIG_KRKR_ARG_STRING(0))
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(symlinkStatusPermissions);
	//
	//	SUMMARY: 取得系メソッド
	//
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(LocalFiles, currentPath, currentPath);
	SPRIG_KRKR_NATIVE_STATIC_PROP_DECL_VARIANT_GETONLY(LocalFiles, tempDirectoryPath, tempDirectoryPath);
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(uniquePath)
	{
		if (numparams >= 1) {
			SPRIG_KRKR_TYPE_CHECK(0, tvtString);
			SPRIG_KRKR_RESULT_SET(
				LocalFiles::uniquePath(SPRIG_KRKR_ARG_STRING(0))
				);
		} else {
			SPRIG_KRKR_RESULT_SET(
				LocalFiles::uniquePath()
				);
		}
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(uniquePath);
	//
	//	SUMMARY: 列挙系メソッド
	//
	SPRIG_KRKR_BEGIN_NATIVE_METHOD_DECL(enumDirectoryEntry)
	{
		SPRIG_KRKR_NUMPARAMS_CHECK(1);
		SPRIG_KRKR_TYPE_CHECK(0, tvtString);
		SPRIG_KRKR_RESULT_SET(
			LocalFiles::enumDirectoryEntry(
				SPRIG_KRKR_ARG_STRING(0),
				SPRIG_KRKR_ARG_DEFAULT(1, 0),
				SPRIG_KRKR_ARG_BOOL_DEFAULT(2, false),
				SPRIG_KRKR_ARG_VARIANT_DEFAULT(3, -1),
				SPRIG_KRKR_ARG_BOOL_DEFAULT(4, false)
				)
			);
		return TJS_S_OK;
	}
	SPRIG_KRKR_END_NATIVE_METHOD_DECL(enumDirectoryEntry);

	SPRIG_KRKR_END_CREATE_NATIVE_CLASS();
#undef TJS_NATIVE_CLASSID_NAME
}	// namespace ktl

//
// regist_ktl_local_files
// unregist_ktl_local_files
//
static void regist_ktl_local_files() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:LocalFiles:regist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::AddMember(global.get(), SPRIG_KRKR_TJS_W("LocalFiles"), ktl::CreateNativeClassLocalFiles());
	}
}
static void unregist_ktl_local_files() {
	SPRIG_KRKR_SECTION(SPRIG_KRKR_TJS_W("KTL:LocalFiles:unregist"), SPRIG_KRKR_LOG_LEVEL_NOTIFICATION);
	sprig::krkr::tjs::object_type global(::TVPGetScriptDispatch(), false);
	if (global) {
		sprig::krkr::tjs::DeleteMember(global.get(), SPRIG_KRKR_TJS_W("LocalFiles"));
	}
}
NCB_POST_REGIST_CALLBACK(regist_ktl_local_files);
NCB_PRE_UNREGIST_CALLBACK(unregist_ktl_local_files);

//
// information
//
KTL_INFORMATION(SPRIG_KRKR_TJS_W("KTL:LocalFiles:information"));
