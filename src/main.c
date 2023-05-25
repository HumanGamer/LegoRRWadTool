#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include "wad.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("Usage: %s <wadfile>\n", argv[0]);
        return 1;
    }

    CString inputFile = argv[1];
    CString inputPath = Platform_GetFilePath(inputFile, Null);

    Wad* wad = Wad_Open(inputFile);
    if (wad == Null)
    {
        printf("Wad_Open failed for file '%s'\n", inputFile);
        return 1;
    }

    printf("numFiles: %d\n", wad->numFiles);
    for (S32 i = 0; i < wad->numFiles; i++)
    {
        printf("wadNames[%d]: %s\n", i, wad->wadNames[i]);
        S32 size = wad->wadEntries[i].fileLength;
        U8* data = (U8*)malloc(size);
        if (!Wad_FileOpen(wad, wad->fileNames[i], data, &size))
        {
            printf("Wad_FileOpen failed for file '%s'\n", wad->wadNames[i]);
            return 1;
        }

        // prepend the input path to the output filename
        U32 bufSize = strlen(inputPath) + strlen(wad->wadNames[i]) + 2; // both string lengths + 1 for the slash + 1 for the null terminator
        String outFilename = (String)malloc(bufSize);
        sprintf_s(outFilename, bufSize, "%s\\%s", inputPath, wad->wadNames[i]);

        //char buffer[1024];
        //Platform_GetFilePath(buffer, 1024, outFilename);
        //Platform_CreateDirectories(buffer);

        CString dir = Platform_GetFilePath(outFilename, Null);
        Platform_CreateDirectories(dir);
        free((void*)dir);

        FILE* outFile = fopen(outFilename, "wb");
        fwrite(data, size, 1, outFile);
        fclose(outFile);

        free(data);
    }

    Wad_Close(wad);
    return 0;
}
