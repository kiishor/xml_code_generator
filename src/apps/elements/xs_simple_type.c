/**
 * \file
 * \brief   implement the xs:simpleType tag

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

#include "xs_restriction.h"
#include "xs_simple_type.h"

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

void* allocate_simple_type(uint32_t occurrence, void** context);
void add_simple_type_tag(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t simple_type_Descendant[TOTAL_SIMPLE_TYPE_DESCENDANTS] =
{
  [EN_simple_restriction].Name.String = "xs:restriction",
  [EN_simple_restriction].Name.Length = sizeof("xs:restriction") - 1,
  [EN_simple_restriction].MinOccur     = 0,
  [EN_simple_restriction].MaxOccur     = 64,
  [EN_simple_restriction].Callback      = add_restriction_tag,
  [EN_simple_restriction].Target.Type  = EN_DYNAMIC,
  [EN_simple_restriction].Target.Allocate = allocate_restriction,
  [EN_simple_restriction].Attribute_Quantity = TOTAL_RESTRICTION_ATTRIBUTES,
  [EN_simple_restriction].Attribute = restriction_Attr,
  [EN_simple_restriction].Child_Quantity = TOTAL_RESTRICTION_DESCENDANTS,
  [EN_simple_restriction].Child_Order     = EN_RANDOM,
  [EN_simple_restriction].Child = restriction_Descendant,
};

const xs_attribute_t simple_type_Attr[TOTAL_TYPE_ATTRIBUTES] =
{
  [EN_type_id].Name.String = "id",
  [EN_type_id].Name.Length = sizeof("id") - 1,
  [EN_type_id].Target.Type = EN_RELATIVE,
  [EN_type_id].Target.Offset = offsetof(simpleType_t, attr.id),
  [EN_type_id].Content.Type = EN_STRING_DYNAMIC,
  [EN_type_id].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_type_id].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_type_id].Use = EN_OPTIONAL,

  [EN_type_name].Name.String = "name",
  [EN_type_name].Name.Length = sizeof("name") - 1,
  [EN_type_name].Target.Type = EN_RELATIVE,
  [EN_type_name].Target.Offset = offsetof(simpleType_t, attr.name),
  [EN_type_name].Content.Type = EN_STRING_DYNAMIC,
  [EN_type_name].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_type_name].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_type_name].Use = EN_OPTIONAL,
};

const xs_element_t xs_simpleType =
{
  .Name.String  = "xs:simpleType",
  .Name.Length  = sizeof("xs:simpleType") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_simple_type_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_simple_type,

  .Attribute_Quantity = TOTAL_TYPE_ATTRIBUTES,
  .Attribute = simple_type_Attr,

  .Child_Quantity = TOTAL_SIMPLE_TYPE_DESCENDANTS,
  .Child_Order     = EN_CHOICE,
  .Child = simple_type_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_simple_type(uint32_t occurrence, void** context)
{
  simpleType_t* simpleType = calloc(1, sizeof(simpleType_t));
  simpleType->Type = XS_SIMPLE_TYPE_TAG;
  tree_t* node = create_node(simpleType);
  add_descendant_node(*context, node);
  *context = node;
  return simpleType;
}

void add_simple_type_tag(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}



