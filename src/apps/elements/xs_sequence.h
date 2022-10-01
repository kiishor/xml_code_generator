#ifndef XS_SEQUENCE_H
#define XS_SEQUENCE_H

/**
 * \file
 * \brief   Header file for xs_sequence.c

 * \author  Nandkishor Biradar
 * \date    18 June 2019
 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */
#define ALL_SEQUENCE_DESCENDANTS  \
  ADD_DESCENDANT(child_element)   \
  ADD_DESCENDANT(group)

#define ALL_SEQUENCE_ATTRIBUTES  \
  ADD_ATTRIBUTE(id)           \
  ADD_ATTRIBUTE(minOccurs)    \
  ADD_ATTRIBUTE(maxOccurs)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_DESCENDANT(descendant)    EN_sequence_##descendant,
typedef enum
{
  ALL_SEQUENCE_DESCENDANTS
  TOTAL_SEQUENCE_DESCENDANTS
}en_sequnce_descendants;
#undef ADD_DESCENDANT

#define ADD_ATTRIBUTE(attr)    EN_sequence_##attr,
typedef enum
{
  ALL_SEQUENCE_ATTRIBUTES
  TOTAL_SEQUENCE_ATTRIBUTES
}en_sequence_attributes;
#undef ADD_ATTRIBUTE

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;           // Optional
  uint32_t minOccurs;
  uint32_t maxOccurs;
}sequence_attr_t;

typedef struct
{
  xsd_tag_t Type;
  sequence_attr_t attr;
}sequence_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */
extern const xs_element_t Sequence_Descendant[TOTAL_SEQUENCE_DESCENDANTS];
extern const xs_attribute_t sequence_attr[TOTAL_SEQUENCE_ATTRIBUTES];
extern const xs_element_t xs_sequence;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

extern void* allocate_sequence(uint32_t occurrence, void* context);

#endif // XS_SEQUENCE_H
