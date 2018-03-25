#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "ResourceType.h"

int main() {
    const char* androidManifestPath = "../AndroidManifest.xml";
    initChunkListOfFile(androidManifestPath);
//    dumpAllChunk();
    
    ResChunkHeaderInfo_t* pStringPoolInfo =
            getChunkHeaderByType(RES_STRING_POOL_TYPE);

    ResStringPoolHeader_t* pStringPoolHeader =
            (ResStringPoolHeader_t*)malloc(sizeof(ResStringPoolHeader_t));
    if(pStringPoolHeader == NULL)   return 0;
    getStringPoolHeader(androidManifestPath, &pStringPoolHeader, pStringPoolInfo->mOffsetInFile);

    if(pStringPoolHeader == NULL)
        return 0;

    bool is16BitUnicode = pStringPoolHeader->mFlags == UTF8_FLAG ? false:true;

    int fd = open(androidManifestPath, O_RDONLY);
    if(fd < 0)return  errno;

    int mStringOffsetsSize = sizeof(uint32_t) *  pStringPoolHeader->mStringCount;
    uint32_t* mStringOffsets = (uint32_t*)malloc(mStringOffsetsSize);
    if(mStringOffsets == NULL)return errno;

    lseek(fd, (pStringPoolInfo->mOffsetInFile + pStringPoolHeader->mHeader.mHeaderSize), SEEK_SET);
    int readCount = read(fd,mStringOffsets, mStringOffsetsSize);
    for (int j = 0; j < pStringPoolHeader->mStringCount; ++j) {
        printf("String[%d] Offset: %d\n", j, mStringOffsets[j]);
    }
    if(readCount != mStringOffsetsSize) return  errno;
    if(is16BitUnicode){
        uint32_t mStringsSize = sizeof(char16_t) * pStringPoolHeader->mStringCount;
        char16_t* mStrings = (char16_t*)malloc(mStringsSize);
        if(mStrings == NULL)return -1;

        uint32_t stringsOffset = pStringPoolInfo->mOffsetInFile + pStringPoolHeader->mStringStart;
        lseek(fd, stringsOffset, SEEK_SET);
        readCount = read(fd, mStrings, mStringsSize);
        if(readCount != mStringsSize) return -1;
        printf("mStrings: %ls\n", mStrings);
        for (int i = 0; i < pStringPoolHeader->mStringCount; ++i) {
//            printf("String[%d], offset: %d, Content: %ls\n",
//            i, mStringOffsets[i], (mStringOffsets + mStringOffsets[i]));
//            printf("%ls\n",(mStrings+mStringOffsets[i]));
        }
        free(mStrings);
    } else{

        uint32_t mStringsSize = sizeof(char) * pStringPoolHeader->mStringCount;
        char* mStrings = (char*)malloc(mStringsSize);
        if(mStrings == NULL)return -1;

        uint32_t stringsOffset = pStringPoolInfo->mOffsetInFile + pStringPoolHeader->mStringStart;
        lseek(fd, stringsOffset, SEEK_SET);
        readCount = read(fd, mStrings, mStringsSize);
        if(readCount != mStringsSize) return -1;

        for (int i = 0; i < pStringPoolHeader->mStringCount; ++i) {
            printf("%s\n",mStrings+mStringOffsets[i]);
        }
        free(mStrings);
    }


    free(mStringOffsets);
    close(fd);



    free(pStringPoolHeader);
    freeChunkList();
    return 0;
}