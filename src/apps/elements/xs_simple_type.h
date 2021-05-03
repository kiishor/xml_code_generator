#ifndef XS_SIMPLE_TYPE_H
#define XS_SIMPLE_TYPE_H

/**
 * \file
 * \brief   header file for xs_simple_type.c

 * \author  Nandkishor Biradar
 * \date    18 August 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */
#include "xs_type.h"

 /*
 *  ------------------------------- DEFINITION -------------------------------
 */

#define ALL_SIMPLE_TYPE_DESCENDANTS(parent)   \
  ADD_DESCENDANT(parent, restriction)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_DESCENDANT(parent, element) EN_##parent##_##element,
typedef enum
{
  ALL_SIMPLE_TYPE_DESCENDANTS(simple)
  TOTAL_SIMPLE_TYPE_DESCENDANTS
}en_simpleType_Descendants;
#undef ADD_DESCENDANT

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */
typedef struct
{
  xsd_tag_t Type;
  xs_type_attribute_t attr;
}simpleType_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */
extern const xs_element_t simple_type_Descendant[TOTAL_SIMPLE_TYPE_DESCENDANTS];
extern const xs_attribute_t simple_type_Attr[TOTAL_TYPE_ATTRIBUTES];
extern const xs_element_t xs_simpleType;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
extern void* allocate_simple_type(uint32_t occurrence, void* context);

#endif // XS_SIMPLE_TYPE_H

