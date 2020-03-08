#ifndef XSD_H
#define XSD_H

/**
 * \file
 * \brief   Contains all the XML schema supported tags

 * \author  Nandkishor Biradar
 * \date    18 August 2019
 */
 
/*
 *	-------------------- INCLUDE FILES --------------------
 */
#include "tree.h"


/*
 *	-------------------- DEFINITION --------------------
 */

#define DEFAULT_MIN_STRING_LENGTH 1
#define DEFAULT_MAX_STRING_LENGTH 128

/*
 *	-------------------- ENUMERATION --------------------
 */

typedef enum
{
  XS_SCHEMA_TAG,
  XS_ROOT_ELEMENT_TAG,
  XS_CHILD_ELEMENT_TAG,
  XS_COMPLEX_TAG,
  XS_SEQUENCE_TAG,
  XS_SIMPLE_CONTENT_TAG,
  XS_EXTENSION_TAG,
  XS_ATTRIBUTE_TAG,
  XS_RESTRICTION_TAG,
  XS_SIMPLE_TYPE_TAG,

  XS_ENUMERATION_TAG,
}xsd_tag_t;

/*
 *	-------------------- STRUCTURE --------------------
 */

/*
 *	-------------------- EXPORTED FUNCTION --------------------
 */

#endif // XSD_H

