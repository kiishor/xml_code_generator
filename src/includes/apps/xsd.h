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
  ADD_TAG(XS_CHOICE_TAG,          xs:choice)        \
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

//! List of allocate callback type
typedef enum
{
  NO_ALLOCATE,          //!< Do not insert allocate callback
  SIMPLE_ALLOCATE,      //!< use Simple calloc to allocate memory
  LINKED_LIST_ALLOCATE, //!< Implement linked list based allocation
}en_allocate_type;

typedef enum
{
  NO_CALLBACK,          //!< Do not insert callback

  //! insert printf functions in the callback.
  //! If target type is dynamic then free the memory
  PRINT_CALLBACK,
}en_callback_type;

//! List of Methods to handle multiple occurrence
//! 0 - is for unspecified input
typedef enum
{
  UNSPECIFIED,
  ARRAY,
  LINKED_LIST,
  DYNAMIC
}en_occurrence_t;

// List of parents of restriction or extension
typedef enum
{
  SIMPLE_TYPE_PARENT,
  SIMPLE_CONTENT_PARENT,
  COMPLEX_CONTENT_PARENT
}en_restriction_parent;

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

// Global options received from command line
typedef struct
{
  en_occurrence_t Occurrence;
  bool content_callback;  // Every content element shall have a callback to print the content.
}options_t;

typedef struct
{
  reference_list_t Element_List;
  attribute_list_t Attribute_List;
  element_list_t   ComplexType_List;
  element_list_t   SimpleType_List;
  const options_t* Options;
}context_t;

typedef struct
{
  xs_element_t Element;
  en_allocate_type Allocate;
  en_callback_type Callback;
}xsd_element_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const char* const XsdTag[TOTAL_XSD_TAGS];

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
extern xsd_element_t* compile_xsd(const tree_t* tree, const options_t* const options);

#endif // XSD_H

