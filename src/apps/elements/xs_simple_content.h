#ifndef XS_SIMPLE_CONTENT_H
#define XS_SIMPLE_CONTENT_H

/**
 * \file
 * \brief   header file for xs_simple_content.c

 * \author  Nandkishor Biradar
 * \date    14 August 2019

 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;             // Optional
}simple_content_attribute_t;

typedef struct
{
  xsd_tag_t Type;
  simple_content_attribute_t attr;
}simpleContent_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xs_simpleContent;

#endif // XS_SIMPLE_CONTENT_H

