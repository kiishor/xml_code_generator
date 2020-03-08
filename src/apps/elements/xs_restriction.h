#ifndef XS_RESTRICTION_H
#define XS_RESTRICTION_H

/**
 * \file
 * \brief   Header file for xs_restriction.h

 * \author  Nandkishor Biradar
 * \date    18 August 2019
 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;             // Optional
  string_t base;
}restriction_attribute_t;

typedef struct
{
  xsd_tag_t Type;
  restriction_attribute_t attr;
}restriction_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xs_restriction;

#endif // XS_RESTRICTION_H

