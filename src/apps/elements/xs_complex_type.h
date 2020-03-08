#ifndef XS_COMPLEX_TYPE_H
#define XS_COMPLEX_TYPE_H

/**
 * \file
 * \brief  Header file for xs_complex_type.c

 * \author  Nandkishor Biradar
 * \date  29 September 2019
 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;         // Optional
  string_t name;       // required if root element
}complex_type_attribute_t;

typedef struct
{
  xsd_tag_t Type;
  complex_type_attribute_t attr;
}complexType_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xs_complexType;

#endif // XS_COMPLEX_TYPE_H

