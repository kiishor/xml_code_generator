#ifndef XS_ELEMENT_H
#define XS_ELEMENT_H
/**
 * \file
 * \brief  Header file for xs_element.c

 * \author  Nandkishor Biradar
 * \date  14 August 2019
 */


/*
 *  ------------------------------- DEFINITION -------------------------------
 */

#define ALL_ELEMENT_DESCENDANTS(parent)   \
  ADD_DESCENDANT(parent, complexType)  \
  ADD_DESCENDANT(parent, simpleType)

#define ALL_ELEMENT_ATTRIBUTES(element) \
  ADD_ATTRIBUTE(element, id, string_t)        \
  ADD_ATTRIBUTE(element, name, string_t)      \
  ADD_ATTRIBUTE(element, type, string_t)      \

#define ALL_CHILD_ELEMENT_ATTRIBUTES(element)   \
  ADD_ATTRIBUTE(element, ref, string_t)       \
  ADD_ATTRIBUTE(element, minOccurs, uint32_t) \
  ADD_ATTRIBUTE(element, maxOccurs, string_t)

#define ALL_GLOBAL_ELEMENT_ATTRIBUTES(element)  \
  ADD_ATTRIBUTE(element, substitutiongroup, string_t)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */

#define ADD_DESCENDANT(parent, descendant)   EN_##parent##_##descendant,
typedef enum
{
  ALL_ELEMENT_DESCENDANTS(element)
  TOTAL_ELEMENT_DESCENDANTS
}en_element_descendants;
#undef ADD_DESCENDANT

#define ADD_ATTRIBUTE(element, attr, ...)  EN_##element##_##attr,
typedef enum
{
  ALL_ELEMENT_ATTRIBUTES(global)
  ALL_GLOBAL_ELEMENT_ATTRIBUTES(global)
  TOTAL_GLOBAL_ELEMENT_ATTRIBUTES
}en_global_element_attributes;
#undef ADD_ATTRIBUTE

#define ADD_ATTRIBUTE(element, attr, ...)  EN_##element##_##attr,
typedef enum
{
  ALL_ELEMENT_ATTRIBUTES(child)
  ALL_CHILD_ELEMENT_ATTRIBUTES(child)
  TOTAL_CHILD_ELEMENT_ATTRIBUTES
}en_child_element_attributes;
#undef ADD_ATTRIBUTE


/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;         // Optional
  string_t name;       // required if global element
  string_t type;
}common_element_attribute_t;

#define ADD_ATTRIBUTE(element, attr, type)  type attr;
typedef struct
{
  ALL_ELEMENT_ATTRIBUTES(global)
  ALL_GLOBAL_ELEMENT_ATTRIBUTES(global)
}global_element_attribute_t;
#undef ADD_ATTRIBUTE

#define ADD_ATTRIBUTE(element, attr, type)  type attr;
typedef struct
{
  ALL_ELEMENT_ATTRIBUTES(child)
  ALL_CHILD_ELEMENT_ATTRIBUTES(child)
}child_element_attribute_t;
#undef ADD_ATTRIBUTE

typedef struct
{
  xsd_tag_t Type;
  union
  {
    global_element_attribute_t global;
    child_element_attribute_t child;
  };
}element_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xs_global_element;
extern const xs_element_t xs_child_element;
extern const xs_element_t Element_Descendant[TOTAL_ELEMENT_DESCENDANTS];
extern const xs_attribute_t child_element_attr[TOTAL_CHILD_ELEMENT_ATTRIBUTES];
extern const xs_attribute_t global_element_attr[TOTAL_GLOBAL_ELEMENT_ATTRIBUTES];

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
void add_global_element(uint32_t occurrence, void* content, void** context);
void add_child_element(uint32_t occurrence, void* content, void** context);

void* allocate_global_schema_element(uint32_t, void**);
void* allocate_child_schema_element(uint32_t, void**);

#endif // XS_ELEMENT_H
