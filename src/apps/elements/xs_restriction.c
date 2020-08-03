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

#include "common.h"
#include "libs/parse_xml.h"
#include "apps/xsd.h"
#include "apps/tree.h"

#include "xs_simple_type.h"
#include "xs_attribute.h"
#include "xs_facet.h"
#include "xs_restriction.h"

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

void* allocate_restriction(uint32_t occurrence, void** context);
void add_restriction_tag(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t restriction_Descendant[TOTAL_RESTRICTION_DESCENDANT] =
{
  [EN_restriction_attribute].Name.String = "xs:attribute",
  [EN_restriction_attribute].Name.Length    = sizeof("xs:attribute") - 1,
  [EN_restriction_attribute].MinOccur    = 0,
  [EN_restriction_attribute].MaxOccur    = 64,
  [EN_restriction_attribute].Callback      = add_attribute_tag,
  [EN_restriction_attribute].Target.Type  = EN_DYNAMIC,
  [EN_restriction_attribute].Target.Allocate = allocate_attribute,
  [EN_restriction_attribute].Target.Size = sizeof(attribute_t),
  [EN_restriction_attribute].Attribute_Quantity = TOTAL_ATTRIBUTE_ATTRIBUTES,
  [EN_restriction_attribute].Attribute = attribute_Attr,
  [EN_restriction_attribute].Child_Quantity = TOTAL_ATTRIBUTE_DESCENDANTS,
  [EN_restriction_attribute].Child_Type     = EN_CHOICE,
  [EN_restriction_attribute].Child = attribute_Descendant,

  [EN_restriction_simpleType].Name.String  = "xs:simpleType",
  [EN_restriction_simpleType].Name.Length  = sizeof("xs:simpleType") - 1,
  [EN_restriction_simpleType].MinOccur     = 0,
  [EN_restriction_simpleType].MaxOccur     = 64,
  [EN_restriction_simpleType].Callback      = add_simple_type_tag,
  [EN_restriction_simpleType].Target.Type  = EN_DYNAMIC,
  [EN_restriction_simpleType].Target.Allocate = allocate_simple_type,
  [EN_restriction_simpleType].Attribute_Quantity = TOTAL_TYPE_ATTRIBUTES,
  [EN_restriction_simpleType].Attribute = simple_type_Attr,
  [EN_restriction_simpleType].Child_Quantity = TOTAL_SIMPLE_TYPE_DESCENDANTS,
  [EN_restriction_simpleType].Child_Type     = EN_CHOICE,
  [EN_restriction_simpleType].Child = simple_type_Descendant,

  [EN_restriction_enumeration].Name.String  = "xs:enumeration",
  [EN_restriction_enumeration].Name.Length  = sizeof("xs:enumeration") - 1,
  [EN_restriction_enumeration].MinOccur     = 0,
  [EN_restriction_enumeration].MaxOccur     = 64,
  [EN_restriction_enumeration].Callback     = add_facet_tag,
  [EN_restriction_enumeration].Target.Type  = EN_DYNAMIC,
  [EN_restriction_enumeration].Target.Allocate = allocate_facet,
  [EN_restriction_enumeration].Attribute_Quantity = TOTAL_FACET_ATTRIBUTES,
  [EN_restriction_enumeration].Attribute = facet_Attr,

  [EN_restriction_length].Name.String  = "xs:length",
  [EN_restriction_length].Name.Length  = sizeof("xs:length") - 1,
  [EN_restriction_length].MinOccur     = 0,
  [EN_restriction_length].MaxOccur     = 64,
  [EN_restriction_length].Callback      = add_facet_tag,
  [EN_restriction_length].Target.Type  = EN_DYNAMIC,
  [EN_restriction_length].Target.Allocate = allocate_facet,
  [EN_restriction_length].Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  [EN_restriction_length].Attribute = facet_Attr,

  [EN_restriction_maxInclusive].Name.String  = "xs:maxInclusive",
  [EN_restriction_maxInclusive].Name.Length  = sizeof("xs:maxInclusive") - 1,
  [EN_restriction_maxInclusive].MinOccur     = 0,
  [EN_restriction_maxInclusive].MaxOccur     = 64,
  [EN_restriction_maxInclusive].Callback      = add_facet_tag,
  [EN_restriction_maxInclusive].Target.Type  = EN_DYNAMIC,
  [EN_restriction_maxInclusive].Target.Allocate = allocate_facet,
  [EN_restriction_maxInclusive].Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  [EN_restriction_maxInclusive].Attribute = facet_Attr,

  [EN_restriction_minInclusive].Name.String  = "xs:minInclusive",
  [EN_restriction_minInclusive].Name.Length  = sizeof("xs:minInclusive") - 1,
  [EN_restriction_minInclusive].MinOccur     = 0,
  [EN_restriction_minInclusive].MaxOccur     = 64,
  [EN_restriction_minInclusive].Callback      = add_facet_tag,
  [EN_restriction_minInclusive].Target.Type  = EN_DYNAMIC,
  [EN_restriction_minInclusive].Target.Allocate = allocate_facet,
  [EN_restriction_minInclusive].Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  [EN_restriction_minInclusive].Attribute = facet_Attr,

  [EN_restriction_pattern].Name.String  = "xs:pattern",
  [EN_restriction_pattern].Name.Length  = sizeof("xs:pattern") - 1,
  [EN_restriction_pattern].MinOccur     = 0,
  [EN_restriction_pattern].MaxOccur     = 64,
  [EN_restriction_pattern].Callback      = add_facet_tag,
  [EN_restriction_pattern].Target.Type  = EN_DYNAMIC,
  [EN_restriction_pattern].Target.Allocate = allocate_facet,
  [EN_restriction_pattern].Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  [EN_restriction_pattern].Attribute = facet_Attr,

  [EN_restriction_maxLength].Name.String  = "xs:maxLength",
  [EN_restriction_maxLength].Name.Length  = sizeof("xs:maxLength") - 1,
  [EN_restriction_maxLength].MinOccur     = 0,
  [EN_restriction_maxLength].MaxOccur     = 64,
  [EN_restriction_maxLength].Callback      = add_facet_tag,
  [EN_restriction_maxLength].Target.Type  = EN_DYNAMIC,
  [EN_restriction_maxLength].Target.Allocate = allocate_facet,
  [EN_restriction_maxLength].Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  [EN_restriction_maxLength].Attribute = facet_Attr,

  [EN_restriction_minLength].Name.String  = "xs:minLength",
  [EN_restriction_minLength].Name.Length  = sizeof("xs:minLength") - 1,
  [EN_restriction_minLength].MinOccur     = 0,
  [EN_restriction_minLength].MaxOccur     = 64,
  [EN_restriction_minLength].Callback      = add_facet_tag,
  [EN_restriction_minLength].Target.Type  = EN_DYNAMIC,
  [EN_restriction_minLength].Target.Allocate = allocate_facet,
  [EN_restriction_minLength].Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  [EN_restriction_minLength].Attribute = facet_Attr,
};

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
  .Callback      = add_restriction_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_restriction,

  .Attribute_Quantity = TOTAL_RESTRICTION_ATTRIBUTES,
  .Attribute = restriction_Attr,

  .Child_Quantity = TOTAL_RESTRICTION_DESCENDANT,
  .Child_Type     = EN_CHOICE,
  .Child = restriction_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_restriction(uint32_t occurrence, void** context)
{
  restriction_t* restriction = calloc(1, sizeof(restriction_t));
  restriction->Type = XS_RESTRICTION_TAG;
  tree_t* node = create_node(restriction);
  add_descendant_node(*context, node);
  *context = node;
  return restriction;
}

void add_restriction_tag(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}


