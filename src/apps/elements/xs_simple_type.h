#ifndef XS_SIMPLE_TYPE_H
#define XS_SIMPLE_TYPE_H

/**
 * \file
 * \brief   header file for xs_simple_type.c

 * \author  Nandkishor Biradar
 * \date    18 August 2019
 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;             // Optional
  string_t name;
}xs_simple_type_attribute_t;

typedef struct
{
  xsd_tag_t Type;
  xs_simple_type_attribute_t attr;
}simple_type_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xs_simpleType;

#endif // XS_SIMPLE_TYPE_H

