//
// Created by guoguo on 18-3-25.
//
#include <cstdio>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <list>
#include <cerrno>

#include <stdint-gcc.h>
#include <cstring>

using namespace std;

#include "ResourceType.h"

list<ResChunkHeaderInfo_t*> gChunkList;

void getStringPoolHeader(const char* fileName, ResStringPoolHeader_t** pStringPoolHeader,
                         uint32_t headerOffset){
    int fd = open(fileName, O_RDONLY);
    if(fd < 0 )return ;

    lseek(fd, headerOffset, SEEK_SET);
    int readCount =
            read(fd, (*pStringPoolHeader), sizeof(ResStringPoolHeader_t));
    if(readCount != sizeof(ResStringPoolHeader_t)){
        (*pStringPoolHeader) = NULL;
        close(fd);
    }

    close(fd);
}

ResChunkHeaderInfo_t* getChunkHeaderByType(uint16_t type){
    list<ResChunkHeaderInfo_t*>::iterator it;
    ResChunkHeaderInfo_t* retHeader = NULL;
    for (it = gChunkList.begin();
         it != gChunkList.end() ;
         it++) {
        if((*it)->mChunkHeader->mChunkType == type){
            retHeader = (*it);
        }
    }

    return retHeader;
}


int32_t initChunkListOfFile(const char* fileName){
    struct stat fstat;
    stat(fileName, &fstat);
    int fd = open(fileName, O_RDONLY);
    if(fd < 0 )return errno;

    uint32_t nextChunkOffset = 0;
    uint8_t tempHeaderBuffer[8] = {0x00};
    int readCount = 0;


    while (nextChunkOffset < fstat.st_size){
        lseek(fd, nextChunkOffset, SEEK_SET);
        readCount = read(fd, tempHeaderBuffer, sizeof(tempHeaderBuffer));
        if(readCount != sizeof(tempHeaderBuffer)){
            close(fd);
            return errno;
        }
        ResChunkHeader_t *temp = (ResChunkHeader_t*)malloc(sizeof(ResChunkHeader_t));
        if(temp == NULL) {
            close(fd);
            return errno;
        }
        memset(temp, 0x00, sizeof(ResChunkHeader_t));
        memcpy(temp, tempHeaderBuffer, sizeof(ResChunkHeader_t));

        ResChunkHeaderInfo_t* tempChunkInfo =
                (ResChunkHeaderInfo_t* )malloc(sizeof(ResChunkHeaderInfo_t));
        tempChunkInfo->mChunkHeader = temp;
        tempChunkInfo->mOffsetInFile = nextChunkOffset;
        if( temp->mChunkType == RES_XML_TYPE){ //in First header, the chunkSize is file size
            nextChunkOffset += 8;
        } else {
            nextChunkOffset += temp->mChunkSize;
        }
        gChunkList.push_back(tempChunkInfo);
    }
    close(fd);
    return 0;
}

void freeChunkList(){
    list<ResChunkHeaderInfo_t*>::iterator it;
    for (it = gChunkList.begin();
        it != gChunkList.end() ;
         it++) {
        free((*it)->mChunkHeader);
        free((*it));
    }
}

void dunmpChunkInfo(ResChunkHeaderInfo_t* pInfo){
    if(pInfo == NULL)
        return;
    printf("Type: ");
    switch (pInfo->mChunkHeader->mChunkType){
        case RES_NULL_TYPE:
            printf("RES_NULL_TYPE\n");
            break;
        case RES_STRING_POOL_TYPE:
            printf("RES_STRING_POOL_TYPE\n");
            break;
        case RES_TABLE_TYPE:
            printf("RES_TABLE_TYPE\n");
            break;
        case RES_XML_TYPE:
            printf("RES_XML_TYPE\n");
            break;
        case RES_XML_START_NAMESPACE_TYPE:
            printf("RES_XML_START_NAMESPACE_TYPE\n");
            break;
        case RES_XML_END_NAMESPACE_TYPE:
            printf("RES_XML_END_NAMESPACE_TYPE\n");
            break;
        case RES_XML_START_ELEMENT_TYPE:
            printf("RES_XML_START_ELEMENT_TYPE\n");
            break;
        case RES_XML_END_ELEMENT_TYPE:
            printf("RES_XML_END_ELEMENT_TYPE\n");
            break;
        case RES_XML_CDATA_TYPE:
            printf("RES_XML_CDATA_TYPE\n");
            break;
        case RES_XML_LAST_CHUNK_TYPE:
            printf("RES_XML_LAST_CHUNK_TYPE\n");
            break;

        case RES_XML_RESOURCE_MAP_TYPE:
            printf("RES_XML_RESOURCE_MAP_TYPE\n");
            break;

        case RES_TABLE_PACKAGE_TYPE:
            printf("RES_TABLE_PACKAGE_TYPE\n");
            break;
        case RES_TABLE_TYPE_TYPE:
            printf("RES_TABLE_TYPE_TYPE\n");
            break;
        case RES_TABLE_TYPE_SPEC_TYPE:
            printf("RES_TABLE_TYPE_SPEC_TYPE\n");
            break;
        case RES_TABLE_LIBRARY_TYPE:
            printf("RES_TABLE_LIBRARY_TYPE\n");
            break;
    }
    printf("ChunkOffset: %d\n", pInfo->mOffsetInFile);
    printf("HeaderSize: %d\n", pInfo->mChunkHeader->mHeaderSize);
    printf("ChunkSize: %d\n", pInfo->mChunkHeader->mChunkSize);
}

void dumpAllChunk(){
    list<ResChunkHeaderInfo_t*>::iterator it;
    for (it = gChunkList.begin();
         it != gChunkList.end() ;
         it++) {
        dunmpChunkInfo(*it);

    }
}