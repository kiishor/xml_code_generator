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

