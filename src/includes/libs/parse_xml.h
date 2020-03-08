#ifndef PARSE_XML_H
#define PARSE_XML_H

/**
 * \file
 * \brief   Header file for parse_xml.c.

 * \author  Nandkishor Biradar
 * \date    19 August 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include "xml_content.h"

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
typedef enum
{
  XML_PARSE_SUCCESS,
  XML_SYNTAX_ERROR,
  XML_SOURCE_STRING_END_ERR,
  XML_INVALID_START_TOKEN_ERR,
  XML_ELEMENT_NOT_FOUND_ERR,
  XML_ELEMENT_MAX_OCCURRENCE_ERR,
  XML_ELEMENT_MIN_OCCURRENCE_ERR,
  XML_NULL_TARGET_ADDRESS,
  XML_DUPLICATE_ATTRIBUTE,
  XML_ATTRIBUTE_NOT_FOUND,
  XML_CONTENT_ERROR,
  XML_END_TAG_NOT_FOUND,
}xml_parse_result_t;

typedef enum
{
  EN_STATIC,
  EN_DYNAMIC,
  EN_RELATIVE
}address_type_t;

//! List of Use values in the XML attribute
typedef enum
{
  EN_OPTIONAL,      //!< Use of attribute is optional in an element
  EN_PROHIBITED,    //!< Use of attribute is prohibited in an element
  EN_REQUIRED,      //!< Use of attribute is required in an element
}xs_attribute_use;

typedef enum
{
  EN_CHOICE,
  EN_SEQUENCE,
}element_descendant_type_t;

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  const char* String;
  size_t Length;
}string_t;

typedef void (*element_callback)(uint32_t occurrence, void* const content, void** context);
typedef void* (*allocate)(uint32_t occurrence, void** context);

typedef union
{
  void* Address;
  allocate Allocate;
  size_t Offset;
}address_t;

typedef struct
{
  address_type_t Type;
  address_t;
// Size of Target in case of multiple occurrence to calculate the next target address.
  uint32_t Size;
}target_address_t;

typedef struct
{
  string_t Name;
  target_address_t Target;
  xml_content_t Content;
  xs_attribute_use Use;
}xs_attribute_t;

typedef struct xs_element_t xs_element_t;

struct xs_element_t
{
  string_t Name;
  uint32_t MinOccur;
  uint32_t MaxOccur;
  element_callback Callback;

  target_address_t Target;
  xml_content_t Content;

  uint32_t Attribute_Quantity;
  const xs_attribute_t* Attribute;

  uint32_t Child_Quantity;
  element_descendant_type_t Child_Type;
  const xs_element_t* const* Child;
};

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
 extern xml_parse_result_t parse_xml(const xs_element_t* root, const char* source,
                                     void** context);

#endif // PARSE_XML_H

