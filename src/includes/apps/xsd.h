#ifndef XSD_H
#define XSD_H

/**
 * \file
 * \brief   Contains all the XML schema supported tags

 * \author  Nandkishor Biradar
 * \date    18 August 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include "apps/tree.h"
#include "apps/list.h"
#include "util/util.h"

/*
 *  ------------------------------- DEFINITION -------------------------------
 */

#define DEFAULT_MIN_STRING_LENGTH 1
#define DEFAULT_MAX_STRING_LENGTH 128

#define ALL_XSD_TAG                                   \
  ADD_TAG(XS_SCHEMA_TAG,          xs:schema)          \
  ADD_TAG(XS_GLOBAL_ELEMENT_TAG,  xs:element)         \
  ADD_TAG(XS_CHILD_ELEMENT_TAG,   xs:element)         \
  ADD_TAG(XS_COMPLEX_TAG,         xs:complexType)      \
  ADD_TAG(XS_SEQUENCE_TAG,        xs:sequence)        \
  ADD_TAG(XS_SIMPLE_CONTENT_TAG,  xs:simpleContent)   \
  ADD_TAG(XS_EXTENSION_TAG,       xs:extension)       \
  ADD_TAG(XS_ATTRIBUTE_TAG,       xs:attribute)       \
  ADD_TAG(XS_RESTRICTION_TAG,     xs:restriction)     \
  ADD_TAG(XS_SIMPLE_TYPE_TAG,     xs:simpleType)      \
  ADD_TAG(XS_ENUMERATION_TAG,     xs:enumeration)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */

#define ADD_TAG(TAG, ...) TAG,
typedef enum
{
  ALL_XSD_TAG
  TOTAL_XSD_TAGS
}xsd_tag_t;
#undef ADD_TAG

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  reference_list_t Element_List;
  attribute_list_t Attribute_List;
  element_list_t   ComplexType_List;
  element_list_t   SimpleType_List;
}context_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const char* const XsdTag[TOTAL_XSD_TAGS];

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
extern xs_element_t* compile_xsd(const tree_t* tree);
extern void print_xsd(const tree_t* const tree, uint32_t level);

#endif // XSD_H

