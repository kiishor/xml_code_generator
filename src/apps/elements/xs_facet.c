/**
 * \file
 * \brief   implement the xs:enumeration, xs:length, xs:maxInclusive, xs:minInclusive,
      xs:pattern, xs:maxLength, xs:minLength facets

 * \author  Nandkishor Biradar
 * \date    18 August 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parse_xml.h"
#include "apps/xsd.h"
#include "apps/tree.h"
#include "xs_facet.h"
#include "xs_schema.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_attribute_t facet_Attr[TOTAL_FACET_ATTRIBUTES] =
{
  [0].Name.String = "value",
  [0].Name.Length = sizeof("value") - 1,

  [0].Target.Type = EN_RELATIVE,
  [0].Target.Offset = offsetof(xs_facet_t, attr.value),

  [0].Content.Type = EN_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_REQUIRED,
};

#define ADD_FACET(tag, ...)                     \
const xs_element_t xs_##tag =                   \
{                                               \
  .Name.String  = "xs:" #tag,                   \
  .Name.Length  = sizeof("xs:" #tag) - 1,       \
  .MinOccur     = 0,                            \
  .MaxOccur     = 64,                           \
  .Callback      = traverse_up,                 \
                                                \
  .Target.Type  = EN_DYNAMIC,                   \
  .Target.Allocate = allocate_##tag,            \
                                                \
  .Attribute_Quantity = TOTAL_FACET_ATTRIBUTES, \
  .Attribute = facet_Attr,                      \
};

ALL_FACETS

#undef ADD_FACET

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

#define ADD_FACET(facet, FACET)  \
void* allocate_##facet(uint32_t occurrence, void* context) \
{                                                               \
  return allocate_element_type(context, sizeof(xs_facet_t), XS_##FACET##_TAG); \
}

ALL_FACETS
#undef ADD_FACET
