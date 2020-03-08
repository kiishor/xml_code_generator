#ifndef XS_ATTRIBUTE_H
#define XS_ATTRIBUTE_H

/**
 * \file
 * \brief   Header file for xs_attribute.c

 * \author  Nandkishor Biradar
 * \date    14 August 2019
 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;             // Optional
  string_t name;
  string_t type;
  string_t ref;
  string_t use;
  string_t Default;
  string_t fixed;

}xs_attribute_attribute_t;

typedef struct
{
  xsd_tag_t Type;
  xs_attribute_attribute_t attr;
}attribute_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xs_attribute;

#endif // XS_ATTRIBUTE_H

