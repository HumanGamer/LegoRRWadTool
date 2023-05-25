#include <malloc.h>
#include <string.h>
#include "wad.h"

void GetFileName(FILE* f, String buffer)
{
    S8 c;
    while ((c = (S8)fgetc(f)))
    {
        *buffer = c;
        buffer++;
    }
    *buffer = 0;
}

Wad* Wad_Open(CString filename)
{
    S8 header[4];
    FILE* f;

    Wad* wad = (Wad*)malloc(sizeof(Wad));

    // Load the wad
    f = wad->fWad = fopen(filename, "rb");
    if (!f)
    {
        return Null;
    }

    // Check to see if we actually have a wad file.
    if (fread(header, 1, 4, f) != 4)
    {
        fclose(f);
        return Null;
    }

    if (memcmp(header, "WWAD", 4) != 0)
    {
        fclose(f);
        return Null;
    }

    // Get the number of files to load
    if (fread(&wad->numFiles, 1, sizeof(S32), f) != sizeof(S32))
    {
        fclose(f);
        return Null;
    }

    // The names are first
    wad->wadNames = (String*)malloc(sizeof(String) * wad->numFiles);
    wad->fileNames = (String*)malloc(sizeof(String) * wad->numFiles);
    if (!wad->wadNames || !wad->fileNames)
    {
        fclose(f);
        Platform_Free(wad->wadNames);
        Platform_Free(wad->fileNames);
        return NULL;
    }

    memset(wad->wadNames, 0, sizeof(String) * wad->numFiles);
    memset(wad->fileNames, 0, sizeof(String) * wad->numFiles);

    // Reads the names of the files in the wad and on disk
    for (S32 i = 0; i < wad->numFiles; i++)
    {
        char buffer[4096];
        GetFileName(f, buffer);

        wad->wadNames[i] = (String)malloc(strlen(buffer) + 1);
        strcpy(wad->wadNames[i], buffer);
    }

    for (S32 i = 0; i < wad->numFiles; i++)
    {
        char buffer[4096];
        GetFileName(f, buffer);

        wad->fileNames[i] = (String)malloc(strlen(buffer) + 1);
        strcpy(wad->fileNames[i], buffer);
    }

    // Now we get the data.  Address of file, compression
    wad->wadEntries = (WadEntry*)malloc(sizeof(WadEntry) * wad->numFiles);
    if (!wad->wadEntries)
    {
        Platform_FreeArray(wad->wadNames, wad->numFiles);
        Platform_FreeArray(wad->fileNames, wad->numFiles);

        fclose(f);
        return Null;
    }

    if (fread(wad->wadEntries, 1, sizeof(WadEntry) * wad->numFiles, f) != (S32)(sizeof(WadEntry) * wad->numFiles))
    {

        Platform_FreeArray(wad->wadNames, wad->numFiles);
        Platform_FreeArray(wad->fileNames, wad->numFiles);
        Platform_Free(wad->wadEntries);

        fclose(f);
        return Null;
    }

    return wad;
}

void Wad_Close(Wad* wad)
{
    free(wad);
}

S32 Wad_IsFileInWad(Wad* wad, CString filename)
{
    S32 i;
    for (i = 0; i < wad->numFiles; i++)
    {
        if (!_stricmp(wad->fileNames[i], filename))
            return i;
    }

    return -1;
}

Bool Wad_FileOpen(Wad* wad, CString filename, U8* data, S32* outSize)
{
    void* ptr;
    S32 index = Wad_IsFileInWad(wad, filename);

    if (index == -1)
    {
        *outSize = 0;
        return False;
    }

    ptr = malloc(wad->wadEntries[index].fileLength);
    if (!ptr)
    {
        *outSize = 0;
        return False;
    }

    fseek(wad->fWad, wad->wadEntries[index].addr, SEEK_SET);
    if (fread(ptr, wad->wadEntries[index].fileLength, 1, wad->fWad) != 1)
    {
        free(ptr);
        *outSize = 0;
        return False;
    }

    // TODO: Handle Decompression

    *outSize = wad->wadEntries[index].fileLength;
    memcpy(data, ptr, wad->wadEntries[index].fileLength);

    free(ptr);

    return True;
}
