#ifndef XS_SIMPLE_CONTENT_H
#define XS_SIMPLE_CONTENT_H

/**
 * \file
 * \brief   header file for xs_simple_content.c

 * \author  Nandkishor Biradar
 * \date    14 August 2019

 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */
#define ALL_SIMPLE_CONTENT_DESCENDANTS  \
  ADD_DESCENDANT(extension)             \
  ADD_DESCENDANT(restriction)

#define ALL_SIMPLE_CONTENT_ATTRIBUTES   \
  ADD_ATTRIBUTE(id)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_DESCENDANT(descendant)  EN_simple_content_##descendant,
typedef enum
{
  ALL_SIMPLE_CONTENT_DESCENDANTS
  TOTAL_SIMPLE_CONTENT_DESCENDANTS
}en_simple_content_descendants;
#undef ADD_DESCENDANT

#define ADD_ATTRIBUTE(attr)     EN_simnple_content_##attr,
typedef enum
{
  ALL_SIMPLE_CONTENT_ATTRIBUTES
  TOTAL_SIMPLE_CONTENT_ATTRIBUTES
}en_simple_content_attributes;
#undef ADD_ATTRIBUTE

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;             // Optional
}simple_content_attribute_t;

typedef struct
{
  xsd_tag_t Type;
  simple_content_attribute_t attr;
}simpleContent_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */
extern const xs_element_t simpleContent_Descendant[TOTAL_SIMPLE_CONTENT_DESCENDANTS];
extern const xs_attribute_t simpleContent_Attr[TOTAL_SIMPLE_CONTENT_ATTRIBUTES];
extern const xs_element_t xs_simpleContent;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
extern void* allocate_simple_content(uint32_t occurrence, void** context);
extern void add_simple_content(uint32_t occurrence, void* content, void** context);

#endif // XS_SIMPLE_CONTENT_H

