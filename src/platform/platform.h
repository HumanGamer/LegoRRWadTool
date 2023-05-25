#pragma once

#include "types.h"

/// Get path to the file without the filename
extern CString Platform_GetFilePath(CString file, U32* outLength);
//extern U32 Platform_GetFilePath(U8* buffer, U32 bufferSize, CString filepath);

/// Creates the directory if it doesn't exist
extern Bool Platform_CreateDirectory(CString path);

/// Recursively creates the directories if they don't exist
extern void Platform_CreateDirectories(CString path);
