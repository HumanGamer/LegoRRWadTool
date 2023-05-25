#pragma once

#include "types.h"

/// Get path to the file without the filename
extern CString Platform_GetFilePath(CString file, U32* outLength);

/// Creates the directory if it doesn't exist
extern Bool Platform_CreateDirectory(CString path);

/// Recursively creates the directories if they don't exist
extern void Platform_CreateDirectories(CString path);

extern void Platform_Free(void* ptr);
extern void Platform_FreeArray(void* ptr, U32 numElements);

