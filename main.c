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

    const char * inputFile = argv[1];

    // Get the input path to use for the output path
    char* inputPath = strdup(inputFile);
    char* p = inputPath + strlen(inputPath);
    while (*p != '\\' && p != inputPath)
    {
        p--;
    }
    if (p != inputPath)
    {
        *p = '\0';
    }
    else
    {
        *p = '\\';
        *(p + 1) = '\0';
    }

    Wad* wad = Wad_Open(inputFile);
    if (wad == Null)
    {
        printf("Wad_Open failed for file '%s'\n", inputFile);
        return 1;
    }

    printf("numFiles: %d\n", wad->numFiles);
    for (int i = 0; i < wad->numFiles; i++)
    {
        //const char* outFilename = wad->wadNames[i];
        printf("wadNames[%d]: %s\n", i, wad->wadNames[i]);
        S32 size;
        size = wad->wadEntries[i].fileLength;
        char* data = (char*)malloc(size);
        if (!Wad_FileOpen(wad, wad->fileNames[i], data, &size))
        {
            printf("Wad_FileOpen failed for file '%s'\n", wad->wadNames[i]);
            return 1;
        }

        // prepend the input path to the output filename
        char* outFilename = (char*)malloc(strlen(inputPath) + strlen(wad->wadNames[i]) + 2);
        strcpy(outFilename, inputPath);
        strcat(outFilename, "\\");
        strcat(outFilename, wad->wadNames[i]);


        // Recursively create the directories if they don't exist
        char* dir = strdup(outFilename);
        char* p = dir;
        while (*p)
        {
            if (*p == '\\')
            {
                *p = '\0';
                CreateDirectory(dir, Null);
                *p = '\\';
            }
            p++;
        }
        free(dir);

        FILE* outFile = fopen(outFilename, "wb");
        fwrite(data, size, 1, outFile);
        fclose(outFile);

        free(data);
    }

    Wad_Close(wad);
    return 0;
}
