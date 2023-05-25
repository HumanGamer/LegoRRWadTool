#pragma once

#include <stdio.h>
#include "platform/platform.h"

typedef struct WadEntry
{
    S32 compression;
    S32 fileLength;
    S32 decompressedLength;
    union
    {
        S32 addr;
        U32 ptr;
    };
} WadEntry, *lpWadEntry;

typedef struct Wad
{
    const char* filename;
    FILE* fWad;

    char **fileNames;
    char **wadNames;
    WadEntry* wadEntries;
    S32 numFiles;
} Wad, *lpWad;

extern Wad* Wad_Open(CString filename);
extern void Wad_Close(Wad* wad);

extern S32 Wad_IsFileInWad(Wad* wad, CString filename);
extern Bool Wad_FileOpen(Wad* wad, CString filename, U8* data, S32* outSize);
