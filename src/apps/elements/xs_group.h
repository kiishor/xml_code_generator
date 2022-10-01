#ifndef XS_GROUP_H
#define XS_GROUP_H

/**
 * \file
 * \brief   Header file for xs_group.c
 * \author  Nandkishor Biradar
 * \date    14 August 2022
 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */
#define ALL_GROUP_DESCENDANTS  \
  ADD_DESCENDANT(choice)       \
  ADD_DESCENDANT(sequence)

#define ALL_GROUP_ATTRIBUTES  \
  ADD_ATTRIBUTE(id)           \
  ADD_ATTRIBUTE(name)           \
  ADD_ATTRIBUTE(minOccurs)    \
  ADD_ATTRIBUTE(maxOccurs)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_DESCENDANT(descendant)    EN_group_##descendant,
typedef enum
{
  ALL_GROUP_DESCENDANTS
  TOTAL_GROUP_DESCENDANTS
}en_group_descendants;
#undef ADD_DESCENDANT

#define ADD_ATTRIBUTE(attr)    EN_group_##attr,
typedef enum
{
  ALL_GROUP_ATTRIBUTES
  TOTAL_GROUP_ATTRIBUTES
}en_group_attributes;
#undef ADD_ATTRIBUTE

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;           // Optional
  string_t name;
  uint32_t minOccurs;
  string_t maxOccurs;
}group_attr_t;

typedef struct
{
  xsd_tag_t Type;
  group_attr_t attr;
}group_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */
extern const xs_element_t Group_Descendant[TOTAL_GROUP_DESCENDANTS];
extern const xs_attribute_t Group_attr[TOTAL_GROUP_ATTRIBUTES];
extern const xs_element_t xs_group;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

extern void* allocate_group(uint32_t occurrence, void* context);

#endif // XS_GROUP_H
