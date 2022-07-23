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

#define DEFAULT_MIN_STRING_LENGTH 0
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

//! List of Methods to handle multiple occurrence
typedef enum
{
  ARRAY,        //!< Use Array to handle multiple occurrence
  DYNAMIC       //!< Use allocate callback to allocate elements dynamically
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

//! Global options received from command line
typedef struct
{
  en_occurrence_t Occurrence;

  //! Add context to the function callbacks signature
  //! true: Callback functions signature contains context
  //! false: Callback functions signature doesn't contains context
  //! Default - false
  const char* Context;

//! Every content element shall have a callback to print the content.
//! true: Specify empty callback function
//! false: Do not specify callback function
//! Default - false.
  bool Content_Callback;
}options_t;

typedef struct
{
  unresolved_tag_t Ref_List;          //!< List of elements with unresolved "ref"
  unresolved_tag_t Type_List;         //!< List of elements with unresolved "type"
  unresolved_tag_t Attr_Ref_List;     //!< List of attributes with unresolved "ref"
  unresolved_tag_t Attr_Type_List;    //!< List of attributes with unresolved "type"

  attribute_list_t Attribute_List;     //!< List of attributes found in the schema
  simpleType_list_t SimpleType_List;    //!< List of simple types found in the schema
  complexType_list_t ComplexType_List;   //!< List of complext types found in the schema

  const options_t* Options;           //!< Command line options
}context_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const char* const XsdTag[TOTAL_XSD_TAGS];

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
extern xs_element_t* compile_xsd(const tree_t* tree, const options_t* const options);

#endif // XSD_H

