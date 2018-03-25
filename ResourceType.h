//
// Created by guoguo on 18-3-25.
//

#ifndef ANDROIDRESOURCETOOL_RESOURCETYPE_H
#define ANDROIDRESOURCETOOL_RESOURCETYPE_H

#include <stdint-gcc.h>

enum {
    RES_NULL_TYPE = 0x0000,
    RES_STRING_POOL_TYPE = 0x0001,
    RES_TABLE_TYPE = 0x0002,
    RES_XML_TYPE = 0x0003,

    // Chunk types in RES_XML_TYPE
    RES_XML_FIRST_CHUNK_TYPE = 0x0100,
    RES_XML_START_NAMESPACE_TYPE = 0x0100,
    RES_XML_END_NAMESPACE_TYPE = 0x0101,
    RES_XML_START_ELEMENT_TYPE = 0x0102,
    RES_XML_END_ELEMENT_TYPE = 0x0103,
    RES_XML_CDATA_TYPE = 0x0104,
    RES_XML_LAST_CHUNK_TYPE = 0x017f,
    // This contains a uint32_t array mapping strings in the string
    // pool back to resource identifiers.  It is optional.
            RES_XML_RESOURCE_MAP_TYPE = 0x0180,

    // Chunk types in RES_TABLE_TYPE
            RES_TABLE_PACKAGE_TYPE = 0x0200,
    RES_TABLE_TYPE_TYPE = 0x0201,
    RES_TABLE_TYPE_SPEC_TYPE = 0x0202,
    RES_TABLE_LIBRARY_TYPE = 0x0203
};

enum {
    // If set, the string index is sorted by the string values (based
    // on strcmp16()).
    SORTED_FLAG = 1<<0,

    // String pool is encoded in UTF-8
    UTF8_FLAG = 1<<8
};

enum {
    // The 'data' is either 0 or 1, specifying this resource is either
    // undefined or empty, respectively.
            TYPE_NULL = 0x00,
    // The 'data' holds a ResTable_ref, a reference to another resource
    // table entry.
            TYPE_REFERENCE = 0x01,
    // The 'data' holds an attribute resource identifier.
            TYPE_ATTRIBUTE = 0x02,
    // The 'data' holds an index into the containing resource table's
    // global value string pool.
            TYPE_STRING = 0x03,
    // The 'data' holds a single-precision floating point number.
            TYPE_FLOAT = 0x04,
    // The 'data' holds a complex number encoding a dimension value,
    // such as "100in".
            TYPE_DIMENSION = 0x05,
    // The 'data' holds a complex number encoding a fraction of a
    // container.
            TYPE_FRACTION = 0x06,
    // The 'data' holds a dynamic ResTable_ref, which needs to be
    // resolved before it can be used like a TYPE_REFERENCE.
            TYPE_DYNAMIC_REFERENCE = 0x07,
    // The 'data' holds an attribute resource identifier, which needs to be resolved
    // before it can be used like a TYPE_ATTRIBUTE.
            TYPE_DYNAMIC_ATTRIBUTE = 0x08,

    // Beginning of integer flavors...
            TYPE_FIRST_INT = 0x10,

    // The 'data' is a raw integer value of the form n..n.
            TYPE_INT_DEC = 0x10,
    // The 'data' is a raw integer value of the form 0xn..n.
            TYPE_INT_HEX = 0x11,
    // The 'data' is either 0 or 1, for input "false" or "true" respectively.
            TYPE_INT_BOOLEAN = 0x12,

    // Beginning of color integer flavors...
            TYPE_FIRST_COLOR_INT = 0x1c,

    // The 'data' is a raw integer value of the form #aarrggbb.
            TYPE_INT_COLOR_ARGB8 = 0x1c,
    // The 'data' is a raw integer value of the form #rrggbb.
            TYPE_INT_COLOR_RGB8 = 0x1d,
    // The 'data' is a raw integer value of the form #argb.
            TYPE_INT_COLOR_ARGB4 = 0x1e,
    // The 'data' is a raw integer value of the form #rgb.
            TYPE_INT_COLOR_RGB4 = 0x1f,

    // ...end of integer flavors.
            TYPE_LAST_COLOR_INT = 0x1f,

    // ...end of integer flavors.
            TYPE_LAST_INT = 0x1f
};

enum {
    // Where the unit type information is.  This gives us 16 possible
    // types, as defined below.
            COMPLEX_UNIT_SHIFT = 0,
    COMPLEX_UNIT_MASK = 0xf,

    // TYPE_DIMENSION: Value is raw pixels.
            COMPLEX_UNIT_PX = 0,
    // TYPE_DIMENSION: Value is Device Independent Pixels.
            COMPLEX_UNIT_DIP = 1,
    // TYPE_DIMENSION: Value is a Scaled device independent Pixels.
            COMPLEX_UNIT_SP = 2,
    // TYPE_DIMENSION: Value is in points.
            COMPLEX_UNIT_PT = 3,
    // TYPE_DIMENSION: Value is in inches.
            COMPLEX_UNIT_IN = 4,
    // TYPE_DIMENSION: Value is in millimeters.
            COMPLEX_UNIT_MM = 5,

    // TYPE_FRACTION: A basic fraction of the overall size.
            COMPLEX_UNIT_FRACTION = 0,
    // TYPE_FRACTION: A fraction of the parent size.
            COMPLEX_UNIT_FRACTION_PARENT = 1,

    // Where the radix information is, telling where the decimal place
    // appears in the mantissa.  This give us 4 possible fixed point
    // representations as defined below.
            COMPLEX_RADIX_SHIFT = 4,
    COMPLEX_RADIX_MASK = 0x3,

    // The mantissa is an integral number -- i.e., 0xnnnnnn.0
            COMPLEX_RADIX_23p0 = 0,
    // The mantissa magnitude is 16 bits -- i.e, 0xnnnn.nn
            COMPLEX_RADIX_16p7 = 1,
    // The mantissa magnitude is 8 bits -- i.e, 0xnn.nnnn
            COMPLEX_RADIX_8p15 = 2,
    // The mantissa magnitude is 0 bits -- i.e, 0x0.nnnnnn
            COMPLEX_RADIX_0p23 = 3,

    // Where the actual value is.  This gives us 23 bits of
    // precision.  The top bit is the sign.
            COMPLEX_MANTISSA_SHIFT = 8,
    COMPLEX_MANTISSA_MASK = 0xffffff
};

enum {
    // The value is not defined.
    DATA_NULL_UNDEFINED = 0,
    // The value is explicitly defined as empty.
    DATA_NULL_EMPTY = 1
};

typedef struct ResValue {
    uint16_t mSizeInBytes;
    uint8_t mRes0;//always set to 0
    uint8_t mDataType;
    uint32_t mComplexData;
}ResValue_t;

typedef struct ResChunkHeader {
    uint16_t mChunkType;

    uint16_t mHeaderSize;

    uint32_t mChunkSize;
}ResChunkHeader_t;

typedef struct ResTableHeader {
    struct ResChunkHeader mHeader;
    uint32_t mPackageCount; // number of ResTablePackage
}ResTableHeader_t;

typedef struct ResTablePackage {
    struct ResChunkHeader mHeader;

    uint32_t mPackageID; // 0 means is not a base package

    uint16_t mPackageName[128];

    uint32_t mTypeStringsOffset;

    uint32_t mLastPublicType;

    uint32_t mKeyStringsOffset;

    uint32_t mLastPublicKey;

    uint32_t mTypeIdOffset;

}ResTablePackage_t;
typedef struct ResChunkHeader ResXMLTreeHeader;

typedef struct ResXMLTreeNode {
    struct ResChunkHeader mHeader;
    uint32_t mLineNumber;
    uint32_t mCommentIndex;//index in string pool
}ResXMLTreeNode_t;

typedef struct ResString{
    uint16_t mStringLength;
    char16_t *mString;
}ResString_t;

typedef struct ResStringPoolHeader {
    struct ResChunkHeader mHeader;

    uint32_t mStringCount;
    uint32_t mStyleCount;
    uint32_t mFlags;
    uint32_t mStringStart;
    uint32_t mStyleStart;
}ResStringPoolHeader_t;

typedef struct ResMapHeader {
    struct ResChunkHeader mHeader;
    uint32_t mResIds[0];
}ResMapHeader_t;

typedef struct ResXMLNamespace {
    struct ResXMLTreeNode mNode;
    uint32_t mPrefixIndex; //index in string pool
    uint32_t mUriIndex; //index in string pool
}ResXMLNamespace_t;

typedef struct ResXMLCData {
    struct ResXMLTreeNode mNode;
    uint32_t mDataIndex; //index in string pool
    uint32_t mUriIndex; //index in string pool
}ResXMLCData_t;

typedef struct ResNamespace ResStartElement;
typedef struct ResEndElement {
    struct ResXMLTreeNode mNode;
    uint32_t mNamespaceIndex;
    uint32_t mElementName;//if element is CDATA,
    // it is raw character. if not,
    //it is index of string pool
}ResEndElement_t;

typedef struct ResNodeAttributeHeader {
    uint32_t mNamespaceIndex;
    uint32_t mElementName;

    uint16_t mAttributeStart;
    uint16_t mAttributeSize;
    uint16_t mAttributeCount;
    uint16_t mIdIndex;
    uint16_t mClassIndex;
    uint16_t mStyleIndex;
}ResNodeAttributeHeader_t;

typedef struct ResNodeAttribute {
    uint32_t mNamespaceIndex;
    uint32_t mAttributeName;
    uint32_t mRawValueIndex;

}ResNodeAttribute_t;

typedef struct ResChunkHeaderInfo{
    struct ResChunkHeader* mChunkHeader;
    uint32_t mOffsetInFile;
}ResChunkHeaderInfo_t;




int32_t initChunkListOfFile(const char* fileName);
void getStringPoolHeader(const char* fileName, ResStringPoolHeader_t** pStringPoolHeader,
                         uint32_t headerOffset);
ResChunkHeaderInfo_t* getChunkHeaderByType(uint16_t type);
void freeChunkList();
void dunmpChunkInfo(ResChunkHeaderInfo_t* pInfo);
void dumpAllChunk();

#endif //ANDROIDRESOURCETOOL_RESOURCETYPE_H
