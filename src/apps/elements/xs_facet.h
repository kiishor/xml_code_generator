#ifndef XS_FACET_H
#define XS_FACET_H

/**
 * \file
 * \brief   Header file for xs_facet.c

 * \author  Nandkishor Biradar
 * \date    18 August 2019
 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */
#define TOTAL_FACET_ATTRIBUTES 1


/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t value;             // Optional
}xs_facet_attribute_t;

typedef struct
{
  xsd_tag_t Type;
  xs_facet_attribute_t attr;
}xs_facet_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */
extern const xs_attribute_t facet_Attr[TOTAL_FACET_ATTRIBUTES];

extern const xs_element_t xs_minInclusive;
extern const xs_element_t xs_maxInclusive;
extern const xs_element_t xs_length;
extern const xs_element_t xs_enumeration;
extern const xs_element_t xs_pattern;
extern const xs_element_t xs_maxLength;
extern const xs_element_t xs_minLength;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

#define ADD_FACET(tag, ...) extern void* allocate_##tag(uint32_t occurrence, void* context);

ALL_FACETS;

#undef ADD_FACET


#endif // XS_FACET_H

