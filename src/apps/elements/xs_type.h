#ifndef XS_TYPE_H
#define XS_TYPE_H

/**
 * \file
 * \brief  Header file for containing common definitions for complexType and simpleType

 * \author  Nandkishor Biradar
 * \date  02 August 2020
 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */
#define ALL_TYPE_ATTRIBUTES(element)  \
  ADD_ATTRIBUTE(element, id, optinal)                   \
  ADD_ATTRIBUTE(element, name, required if global element)

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

#endif // XS_TYPE_H

