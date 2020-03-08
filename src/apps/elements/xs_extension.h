#ifndef XS_EXTENSION_H
#define XS_EXTENSION_H

/**
 * \file
 * \brief   Header file for xs:extension tag

 * \author  Nandkishor Biradar
 * \date  19 June 2019
 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;             // Optional
  string_t base;
}extension_attribute_t;

typedef struct
{
  xsd_tag_t Type;
  extension_attribute_t attr;
}extension_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xs_extension;

#endif // XS_EXTENSION_H

