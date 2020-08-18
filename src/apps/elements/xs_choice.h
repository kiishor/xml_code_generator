#ifndef XS_CHOICE_H
#define XS_CHOICE_H

/**
 * \file
 * \brief   Header file for xs_choice.c
 * \author  Nandkishor Biradar
 * \date    18 August 2020
 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */
#define ALL_CHOICE_DESCENDANTS  \
  ADD_DESCENDANT(child_element)

#define ALL_CHOICE_ATTRIBUTES  \
  ADD_ATTRIBUTE(id)           \
  ADD_ATTRIBUTE(minOccurs)    \
  ADD_ATTRIBUTE(maxOccurs)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_DESCENDANT(descendant)    EN_choice_##descendant,
typedef enum
{
  ALL_CHOICE_DESCENDANTS
  TOTAL_CHOICE_DESCENDANTS
}en_choice_descendants;
#undef ADD_DESCENDANT

#define ADD_ATTRIBUTE(attr)    EN_choice_##attr,
typedef enum
{
  ALL_CHOICE_ATTRIBUTES
  TOTAL_CHOICE_ATTRIBUTES
}en_choice_attributes;
#undef ADD_ATTRIBUTE

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;           // Optional
  uint32_t minOccurs;
  string_t maxOccurs;
}choice_attr_t;

typedef struct
{
  xsd_tag_t Type;
  choice_attr_t attr;
}choice_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */
extern const xs_element_t Choice_Descendant[TOTAL_CHOICE_DESCENDANTS];
extern const xs_attribute_t Choice_attr[TOTAL_CHOICE_ATTRIBUTES];
extern const xs_element_t xs_choice;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

extern void* allocate_choice(uint32_t occurrence, void** context);

#endif // XS_CHOICE_H
