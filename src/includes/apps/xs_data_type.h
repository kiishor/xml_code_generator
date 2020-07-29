#ifndef XS_DATA_TYPE_H
#define XS_DATA_TYPE_H

/**
 * \file
 * \brief   header file for xs_data_type.c

 * \author  Nandkishor Biradar
 * \date    02 May 2020
 */

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

extern const char* const xs_data_type[TOTAL_XSD_DATA_TYPE];
extern const string_t xs_attribute_use_type[TOTAL_XSD_ATTRIBUTE_USE_VALUES];
extern const char* const xml_attribute_use_type[TOTAL_XSD_ATTRIBUTE_USE_VALUES];
extern const char* const xml_data_type[];
extern const char* const xs_target_address_type[TOTAL_TARGET_ADDRESS_TYPE];
extern const char* const xml_content_type[TOTAL_XML_CONTENT_TYPES];
extern void convert_xsd_data_type(xml_content_t* const content, xs_data_type_t type);
extern const char* const xml_child_order[];
#endif // XS_DATA_TYPE_H

