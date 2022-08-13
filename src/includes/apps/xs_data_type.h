#ifndef XS_DATA_TYPE_H
#define XS_DATA_TYPE_H

/**
 * \file
 * \brief   header file for xs_data_type.c

 * \author  Nandkishor Biradar
 * \date    02 May 2020
 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */

//! List of values of "use" specified in the XML schema.
#define ALL_XSD_ATTRIBUTE_USE_VALUE   \
  ADD_USE_TYPE(EN_OPTIONAL, optional)   \
  ADD_USE_TYPE(EN_REQUIRED, required)   \
  ADD_USE_TYPE(EN_PROHIBITED, prohibited)

//! List of target type to store the XML content.
#define ALL_TARGET_ADDRESS_TYPE \
  ADD_TYPE(EN_STATIC)   \
  ADD_TYPE(EN_DYNAMIC)  \
  ADD_TYPE(EN_RELATIVE)

#define ALL_XSD_DATA_TYPE  \
ADD_DATA_TYPE(XS_STRING,               xs:string,             char*,    string)                   \
ADD_DATA_TYPE(XS_DECIMAL,              xs:decimal,            float,    float/double)             \
ADD_DATA_TYPE(XS_INTEGER,              xs:integer,            int32_t,  int32_t/int64_t)          \
ADD_DATA_TYPE(XS_BYTE,                 xs:byte,               int8_t,   A 8-bit signed integer)   \
ADD_DATA_TYPE(XS_INT,                  xs:int,                int32_t,  A signed 32-bit integer)  \
ADD_DATA_TYPE(XS_LONG,                 xs:long,               int64_t,  A signed 64-bit integer)  \
ADD_DATA_TYPE(XS_NEGATIVE_INTEGER,     xs:negativeInteger,    int32_t,  An integer containing only negative values (..,-2,-1))       \
ADD_DATA_TYPE(XS_NON_NEGATIVE_INTEGER, xs:nonNegativeInteger, uint32_t, An integer containing only non-negative values (0,1,2,..))   \
ADD_DATA_TYPE(XS_NON_POSITIVE_INTEGER, xs:nonPositiveInteger, int32_t,  An integer containing only non-positive values (..,-2,-1,0)) \
ADD_DATA_TYPE(XS_POSITIVE_INTEGER,     xs:positiveInteger,    uint32_t, An integer containing only positive values (1,2,..))         \
ADD_DATA_TYPE(XS_SHORT,                xs:short,              int16_t,  A signed 16-bit integer)     \
ADD_DATA_TYPE(XS_UNSIGNED_LONG,        xs:unsignedLong,       uint64_t, An unsigned 64-bit integer)  \
ADD_DATA_TYPE(XS_UNSIGNED_INT,         xs:unsignedInt,        uint32_t, An unsigned 32-bit integer)  \
ADD_DATA_TYPE(XS_UNSIGNED_SHORT,       xs:unsignedShort,      uint16_t, An unsigned 16-bit integer)  \
ADD_DATA_TYPE(XS_UNSIGNED_BYTE,        xs:unsignedByte,       uint8_t,  An unsigned 8-bit integer)   \
ADD_DATA_TYPE(XS_BOOLEAN,              xs:boolean,            bool,     boolean)  \
ADD_DATA_TYPE(XS_DOUBLE,               xs:double,             double,   double)   \
ADD_DATA_TYPE(XS_FLOAT,                xs:float,              float,    float)    \
ADD_DATA_TYPE(XS_DATE,                 xs:date,               char*,    string)   \
ADD_DATA_TYPE(XS_TIME,                 xs:time,               char*,    string)   \
ADD_DATA_TYPE(XS_DATE_TIME,            xs:dateTime,           char*,    string)   \
ADD_DATA_TYPE(XS_DURATION,             xs:duration,           uint32_t, string)   \
ADD_DATA_TYPE(XS_ANYURI,               xs:anyURI,             char*,    string)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */

#define ADD_DATA_TYPE(TAG, ...)  TAG,
typedef enum
{
  ALL_XSD_DATA_TYPE
  TOTAL_XSD_DATA_TYPE
}xs_data_type_t;
#undef ADD_DATA_TYPE

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const string_t xs_data_type[TOTAL_XSD_DATA_TYPE];
extern const string_t xs_attribute_use_type[TOTAL_XSD_ATTRIBUTE_USE_VALUES];
extern const char* const xml_attribute_use_type[TOTAL_XSD_ATTRIBUTE_USE_VALUES];
extern const char* const xml_data_type[];
extern const char* const xs_target_address_type[TOTAL_TARGET_ADDRESS_TYPE];
extern const char* const xml_content_type[TOTAL_XML_CONTENT_TYPES];
extern void convert_xsd_data_type(xml_content_t* const content, xs_data_type_t type);
extern const char* const xml_child_order[];

#endif // XS_DATA_TYPE_H

