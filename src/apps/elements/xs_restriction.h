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
#define ALL_RESTRICTION_DESCENDANTS \
  ADD_DESCENDANT(attribute)         \
  ADD_DESCENDANT(simpleType)        \
  ADD_DESCENDANT(enumeration)       \
  ADD_DESCENDANT(length)            \
  ADD_DESCENDANT(minInclusive)      \
  ADD_DESCENDANT(maxInclusive)      \
  ADD_DESCENDANT(pattern)           \
  ADD_DESCENDANT(maxLength)         \
  ADD_DESCENDANT(minLength)

#define ALL_RESTRICTION_ATTRIBUTES  \
  ADD_ATTRIBUTE(id)                 \
  ADD_ATTRIBUTE(base)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_DESCENDANT(descendant)    EN_restriction_##descendant,
typedef enum
{
  ALL_RESTRICTION_DESCENDANTS
  TOTAL_RESTRICTION_DESCENDANTS
}en_restriction_descendant;
#undef ADD_DESCENDANT

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

