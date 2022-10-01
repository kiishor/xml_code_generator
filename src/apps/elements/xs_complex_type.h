#ifndef XS_COMPLEX_TYPE_H
#define XS_COMPLEX_TYPE_H

/**
 * \file
 * \brief  Header file for xs_complex_type.c

 * \author  Nandkishor Biradar
 * \date  29 September 2019
 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */

#define ALL_COMPLEX_TYPE_DESCENDANTS(parent) \
  ADD_DESCENDANT(parent, sequence)           \
  ADD_DESCENDANT(parent, choice)             \
  ADD_DESCENDANT(parent, simpleContent)      \
  ADD_DESCENDANT(parent, attribute)          \
  ADD_DESCENDANT(parent, group)

  #define ALL_COMPLEX_TYPE_ATTRIBUTES  \
  ADD_ATTRIBUTE(id)                    \
  ADD_ATTRIBUTE(name)                  \
  ADD_ATTRIBUTE(abstract)              \
  ADD_ATTRIBUTE(mixed)                 \
  ADD_ATTRIBUTE(block)                 \
  ADD_ATTRIBUTE(Final)

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

#define ADD_ATTRIBUTE(attr)    EN_complexType_##attr,
typedef enum
{
  ALL_COMPLEX_TYPE_ATTRIBUTES
  TOTAL_COMPLEX_TYPE_ATTRIBUTES
}en_complexType_attributes;
#undef ADD_ATTRIBUTE

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;           // Optional
  string_t name;
  string_t abstract;
  string_t mixed;
  string_t block;
  string_t Final;
}complexType_attr_t;

typedef struct
{
  xsd_tag_t Type;
  complexType_attr_t attr;
}complexType_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xs_complexType;
extern const xs_element_t ComplexType_Descendant[TOTAL_COMPLEX_TYPE_DESCENDANT];
extern const xs_attribute_t complexType_Attr[TOTAL_COMPLEX_TYPE_ATTRIBUTES];

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

extern void* allocate_complex_type(uint32_t occurrence,void* context);

#endif // XS_COMPLEX_TYPE_H

