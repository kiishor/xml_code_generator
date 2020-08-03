#ifndef XS_SCHEMA_H
#define XS_SCHEMA_H

/**
 * \file
 * \brief
 * \author
 * \date
 * \version
 */

/*
 *  ------------------------------- DEFINITION -------------------------------
 */

#define ALL_SCHEMA_DESCENDANTS    \
  ADD_DESCENDANT(element)  \
  ADD_DESCENDANT(complexType)     \
  ADD_DESCENDANT(attribute)       \
  ADD_DESCENDANT(simpleType)

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */
#define ADD_DESCENDANT(descendant)    EN_schema_##descendant,
typedef enum
{
  ALL_SCHEMA_DESCENDANTS
  TOTAL_SCHEMA_DESCENDANTS
}en_schema_descendats;
#undef ADD_DESCENDANT

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xsd_root;
extern tree_t* pXsdData;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
void add_schema(uint32_t Occurrence, void *Content, void** context);

#endif // XS_SCHEMA_H
