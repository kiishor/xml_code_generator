#ifndef XS_RESTRICTION_H
#define XS_RESTRICTION_H

/**
 * \file
 * \brief   Header file for xs_restriction.h

 * \author  Nandkishor Biradar
 * \date    18 August 2019
 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */



#define ALL_RESTRICTION_DESCENDANTS   \
  ADD_DESCENDANT(attribute)           \
  ADD_DESCENDANT(simpleType)          \

#define ALL_RESTRICTION_ATTRIBUTES  \
  ADD_ATTRIBUTE(id)                 \
  ADD_ATTRIBUTE(base)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_DESCENDANT(descendant)    EN_restriction_##descendant,
#define ADD_FACET(facet, ...)         EN_restriction_##facet,
typedef enum
{
  ALL_RESTRICTION_DESCENDANTS
  ALL_FACETS
  TOTAL_RESTRICTION_DESCENDANTS
}en_restriction_descendant;
#undef ADD_DESCENDANT
#undef ADD_FACET

#define ADD_ATTRIBUTE(attr)     EN_restriction_##attr,
typedef enum
{
  ALL_RESTRICTION_ATTRIBUTES
  TOTAL_RESTRICTION_ATTRIBUTES
}en_restriction_attributes;
#undef ADD_ATTRIBUTE

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
extern const xs_element_t restriction_Descendant[TOTAL_RESTRICTION_DESCENDANTS];
extern const xs_attribute_t restriction_Attr[TOTAL_RESTRICTION_ATTRIBUTES];
extern const xs_element_t xs_restriction;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

extern void* allocate_restriction(uint32_t occurrence, void* context);


#endif // XS_RESTRICTION_H

