#ifndef XS_ELEMENT_H
#define XS_ELEMENT_H
/**
 * \file
 * \brief  Header file for xs_element.c

 * \author  Nandkishor Biradar
 * \date  14 August 2019
 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;         // Optional
  string_t name;       // required if global element
  string_t type;
}common_element_attribute_t;

typedef struct
{
  common_element_attribute_t;
  string_t substitutionGroup;
}global_element_attribute_t;

typedef struct
{
  common_element_attribute_t;
  string_t ref;
  uint32_t minOccurs;
  string_t maxOccurs;
}child_element_attribute_t;

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

#endif // XS_ELEMENT_H
