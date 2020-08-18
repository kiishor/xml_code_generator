#ifndef XS_COMPLEX_TYPE_H
#define XS_COMPLEX_TYPE_H

/**
 * \file
 * \brief  Header file for xs_complex_type.c

 * \author  Nandkishor Biradar
 * \date  29 September 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */
#include "xs_type.h"

/*
 *  ------------------------------- DEFINITION -------------------------------
 */

#define ALL_COMPLEX_TYPE_DESCENDANTS(parent) \
  ADD_DESCENDANT(parent, sequence)           \
  ADD_DESCENDANT(parent, choice)           \
  ADD_DESCENDANT(parent, simpleContent)      \
  ADD_DESCENDANT(parent, attribute)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */

#define ADD_DESCENDANT(parent, descendant)  EN_##parent##_##descendant,
typedef enum
{
  ALL_COMPLEX_TYPE_DESCENDANTS(complex)
  TOTAL_COMPLEX_TYPE_DESCENDANT
}en_complexType_Descendant;
#undef ADD_DESCENDANT

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  xsd_tag_t Type;
  xs_type_attribute_t attr;
}complexType_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xs_complexType;
extern const xs_element_t ComplexType_Descendant[TOTAL_COMPLEX_TYPE_DESCENDANT];
extern const xs_attribute_t complexType_Attr[TOTAL_TYPE_ATTRIBUTES];

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

extern void* allocate_complex_type(uint32_t occurrence,void** context);
extern void add_complex_type(uint32_t occurrence, void* content, void** context);

#endif // XS_COMPLEX_TYPE_H

