#include "platform.h"

#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _DEBUG
#include <stdio.h>
#endif

CString Platform_GetFilePath(CString file, U32* outLength)
{
    String dir = _strdup(file);
    for (S32 i = (S32)strlen(dir) - 1; i >= 0; i--)
    {
        if (dir[i] == '\\' || dir[i] == '/')
        {
            dir[i] = '\0';
            break;
        }
    }

    U32 len = strlen(dir);
    if (outLength)
        *outLength = len;

    return dir;
}

//U32 Platform_GetFilePath(U8* buffer, U32 bufferSize, CString filepath)
//{
//    String dir = _strdup(filepath);
//    for (S32 i = (S32)strlen(dir) - 1; i >= 0; i--)
//    {
//        if (dir[i] == '\\' || dir[i] == '/')
//        {
//            dir[i] = '\0';
//            break;
//        }
//    }
//
//    U32 len = strlen(dir);
//
//    strncpy_s(buffer, bufferSize, dir, len);
//    buffer[len] = '\0';
//    free(dir);
//
//    return len;
//}

Bool Platform_CreateDirectory(CString path)
{
#ifdef _WIN32
    if (CreateDirectory(path, Null) == S_OK)
        return True;

#ifdef _DEBUG
    U32 err = GetLastError();
    if (err == ERROR_ALREADY_EXISTS)
        printf("ERROR_ALREADY_EXISTS\n");
    else if (err == ERROR_PATH_NOT_FOUND)
        printf("ERROR_PATH_NOT_FOUND\n");
#endif

    return False;
#else
#error "Platform_CreateDirectory not implemented for this platform"
#endif
}

void Platform_CreateDirectories(CString path)
{
    // Recursively create the directories if they don't exist
    String dir = strdup(path);
    String p = dir;
    while (True)
    {
        if (*p == '\\' || *p == '/')
        {
            char c = *p;
            *p = '\0';
            Platform_CreateDirectory(dir);
            *p = c;
        } else if (*p == '\0')
        {
            Platform_CreateDirectory(dir);
            break;
        }
        p++;
    }
    free(dir);
}
