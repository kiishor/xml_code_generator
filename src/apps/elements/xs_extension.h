#ifndef XS_EXTENSION_H
#define XS_EXTENSION_H

/**
 * \file
 * \brief   Header file for xs:extension tag

 * \author  Nandkishor Biradar
 * \date  19 June 2019
 */

 /*
 *  ------------------------------- DEFINITION -------------------------------
 */
#define ALL_EXTENSION_DESCENDANTS \
  ADD_DESCENDANT(attribute)

#define ALL_EXTENSION_ATTRIBUTES  \
  ADD_ATTRIBUTE(id)               \
  ADD_ATTRIBUTE(base)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_DESCENDANT(descendant)    EN_extension_##descendant,
typedef enum
{
  ALL_EXTENSION_DESCENDANTS
  TOTAL_EXTENSION_DESCENDANTS
}en_extension_descendants;
#undef ADD_DESCENDANT

#define ADD_ATTRIBUTE(attr)   EN_extension_##attr,
typedef enum
{
  ALL_EXTENSION_ATTRIBUTES
  TOTAL_EXTENSION_ATTRIBUTES
}en_extension_attribute;
#undef ADD_ATTRIBUTE

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */
#define ADD_ATTRIBUTE(attr)   string_t attr;
typedef struct
{
  ALL_EXTENSION_ATTRIBUTES
}extension_attribute_t;
#undef ADD_ATTRIBUTE

typedef struct
{
  xsd_tag_t Type;
  extension_attribute_t attr;
}extension_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */
extern const xs_element_t extension_Descendant[TOTAL_EXTENSION_DESCENDANTS];
extern const xs_attribute_t extension_Attr[TOTAL_EXTENSION_ATTRIBUTES];
extern const xs_element_t xs_extension;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
extern void* allocate_extension(uint32_t occurrence, void** context);
extern void add_extension_tag(uint32_t occurrence, void* content, void** context);


#endif // XS_EXTENSION_H

