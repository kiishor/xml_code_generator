#ifndef XS_ATTRIBUTE_H
#define XS_ATTRIBUTE_H

/**
 * \file
 * \brief   Header file for xs_attribute.c

 * \author  Nandkishor Biradar
 * \date    14 August 2019
 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */
#define ALL_ATTRIBUTE_DESCENDANTS   \
  ADD_DESCENDANT(simpleType)

#define ALL_ATTRIBUTE_ATTRIBUTES    \
  ADD_ATTRIBUTE(id)                 \
  ADD_ATTRIBUTE(name)               \
  ADD_ATTRIBUTE(type)               \
  ADD_ATTRIBUTE(ref)                \
  ADD_ATTRIBUTE(use)                \
  ADD_ATTRIBUTE(Default)            \
  ADD_ATTRIBUTE(fixed)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_DESCENDANT(descendant)  EN_attribute_##descendant,
typedef enum
{
  ALL_ATTRIBUTE_DESCENDANTS
  TOTAL_ATTRIBUTE_DESCENDANTS
}en_attribute_descendants;
#undef ADD_DESCENDANT

#define ADD_ATTRIBUTE(attr)       EN_attribute_##attr,
typedef enum
{
  ALL_ATTRIBUTE_ATTRIBUTES
  TOTAL_ATTRIBUTE_ATTRIBUTES
}en_attribute_attributes;
#undef ADD_ATTRIBUTE

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */
#define ADD_ATTRIBUTE(attr)   string_t attr;
typedef struct
{
  ALL_ATTRIBUTE_ATTRIBUTES
}xs_attribute_attribute_t;
#undef ADD_ATTRIBUTE

typedef struct
{
  xsd_tag_t Type;
  xs_attribute_attribute_t attr;
}attribute_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */
extern const xs_element_t attribute_Descendant[TOTAL_ATTRIBUTE_DESCENDANTS];
extern const xs_attribute_t attribute_Attr[TOTAL_ATTRIBUTE_ATTRIBUTES];
extern const xs_element_t xs_attribute;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

extern void* allocate_attribute(uint32_t occurrence, void* context);


#endif // XS_ATTRIBUTE_H

