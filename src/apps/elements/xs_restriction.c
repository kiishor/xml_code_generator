/**
 * \file
 * \brief   implement the xs:restriction

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

#include "xs_simple_type.h"
#include "xs_attribute.h"
#include "xs_facet.h"
#include "xs_restriction.h"
#include "xs_schema.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

#define ADD_FACET(facet, ...)      \
  [EN_restriction_##facet].Name.String  = "xs:" #facet,               \
  [EN_restriction_##facet].Name.Length  = sizeof("xs:" #facet) - 1,   \
  [EN_restriction_##facet].MinOccur     = 0,                          \
  [EN_restriction_##facet].MaxOccur     = 64,                         \
  [EN_restriction_##facet].Callback     = traverse_up,                \
  [EN_restriction_##facet].Target.Type  = EN_DYNAMIC,                 \
  [EN_restriction_##facet].Target.Allocate    = allocate_##facet,     \
  [EN_restriction_##facet].Attribute_Quantity = TOTAL_FACET_ATTRIBUTES, \
  [EN_restriction_##facet].Attribute          = facet_Attr,

const xs_element_t restriction_Descendant[TOTAL_RESTRICTION_DESCENDANTS] =
{
  [EN_restriction_attribute].Name.String = "xs:attribute",
  [EN_restriction_attribute].Name.Length = sizeof("xs:attribute") - 1,
  [EN_restriction_attribute].MinOccur    = 0,
  [EN_restriction_attribute].MaxOccur    = 64,
  [EN_restriction_attribute].Callback    = traverse_up,
  [EN_restriction_attribute].Target.Type = EN_DYNAMIC,
  [EN_restriction_attribute].Target.Allocate    = allocate_attribute,
  [EN_restriction_attribute].Target.Size        = sizeof(attribute_t),
  [EN_restriction_attribute].Attribute_Quantity = TOTAL_ATTRIBUTE_ATTRIBUTES,
  [EN_restriction_attribute].Attribute      = attribute_Attr,
  [EN_restriction_attribute].Child_Quantity = TOTAL_ATTRIBUTE_DESCENDANTS,
  [EN_restriction_attribute].Child_Order    = EN_CHOICE,
  [EN_restriction_attribute].Child          = attribute_Descendant,

  [EN_restriction_simpleType].Name.String  = "xs:simpleType",
  [EN_restriction_simpleType].Name.Length  = sizeof("xs:simpleType") - 1,
  [EN_restriction_simpleType].MinOccur     = 0,
  [EN_restriction_simpleType].MaxOccur     = 64,
  [EN_restriction_simpleType].Callback     = traverse_up,
  [EN_restriction_simpleType].Target.Type  = EN_DYNAMIC,
  [EN_restriction_simpleType].Target.Allocate    = allocate_simple_type,
  [EN_restriction_simpleType].Attribute_Quantity = TOTAL_TYPE_ATTRIBUTES,
  [EN_restriction_simpleType].Attribute      = simple_type_Attr,
  [EN_restriction_simpleType].Child_Quantity = TOTAL_SIMPLE_TYPE_DESCENDANTS,
  [EN_restriction_simpleType].Child_Order    = EN_CHOICE,
  [EN_restriction_simpleType].Child          = simple_type_Descendant,

  ALL_FACETS
};
#undef ADD_FACET

const xs_attribute_t restriction_Attr[TOTAL_RESTRICTION_ATTRIBUTES] =
{
  [EN_restriction_id].Name.String = "id",
  [EN_restriction_id].Name.Length = sizeof("id") - 1,
  [EN_restriction_id].Target.Type = EN_RELATIVE,
  [EN_restriction_id].Target.Offset = offsetof(restriction_t, attr.id),
  [EN_restriction_id].Content.Type = EN_STRING_DYNAMIC,
  [EN_restriction_id].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_restriction_id].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_restriction_id].Use = EN_OPTIONAL,

  [EN_restriction_base].Name.String = "base",
  [EN_restriction_base].Name.Length = sizeof("base") - 1,
  [EN_restriction_base].Target.Type = EN_RELATIVE,
  [EN_restriction_base].Target.Offset = offsetof(restriction_t, attr.base),
  [EN_restriction_base].Content.Type = EN_STRING_DYNAMIC,
  [EN_restriction_base].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_restriction_base].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_restriction_base].Use = EN_REQUIRED,
};

const xs_element_t xs_restriction =
{
  .Name.String = "xs:restriction",
  .Name.Length = sizeof("xs:restriction") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = traverse_up,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_restriction,

  .Attribute_Quantity = TOTAL_RESTRICTION_ATTRIBUTES,
  .Attribute = restriction_Attr,

  .Child_Quantity = TOTAL_RESTRICTION_DESCENDANTS,
  .Child_Order     = EN_RANDOM,
  .Child = restriction_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_restriction(uint32_t occurrence, void* context)
{
  return allocate_element_type(context, sizeof(restriction_t), XS_RESTRICTION_TAG);
}


