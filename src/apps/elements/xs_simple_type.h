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

 /*
 *  ------------------------------- DEFINITION -------------------------------
 */
#define ALL_TYPE_ATTRIBUTES(element)  \
  ADD_ATTRIBUTE(element, id, optinal)                   \
  ADD_ATTRIBUTE(element, name, required if global element)

#define ALL_SIMPLE_TYPE_DESCENDANTS(parent)   \
  ADD_DESCENDANT(parent, restriction)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_ATTRIBUTE(element, attr, ...) EN_##element##_##attr,
typedef enum
{
  ALL_TYPE_ATTRIBUTES(type)
  TOTAL_TYPE_ATTRIBUTES
}en_type_attributes;
#undef ADD_ATTRIBUTE

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

#define ADD_ATTRIBUTE(element, attr, ...)   string_t attr;
typedef struct
{
  ALL_TYPE_ATTRIBUTES(empty)
//  string_t id;         // Optional
//  string_t name;       // required if global element
}xs_type_attribute_t;
#undef ADD_ATTRIBUTE

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

