/**
 * \file
 * \brief   implement the xs:attribute

 * \author  Nandkishor Biradar
 * \date    14 August 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parse_xml.h"
#include "apps/tree.h"
#include "apps/xsd.h"

#include "xs_simple_type.h"
#include "xs_attribute.h"
#include "xs_schema.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t attribute_Descendant[TOTAL_ATTRIBUTE_DESCENDANTS] =
{
  [EN_attribute_simpleType].Name.String  = "xs:simpleType",
  [EN_attribute_simpleType].Name.Length  = sizeof("xs:simpleType") - 1,
  [EN_attribute_simpleType].MinOccur     = 0,
  [EN_attribute_simpleType].MaxOccur     = 64,
  [EN_attribute_simpleType].Callback      = traverse_up,

  [EN_attribute_simpleType].Target.Type  = EN_DYNAMIC,
  [EN_attribute_simpleType].Target.Allocate = allocate_simple_type,

  [EN_attribute_simpleType].Attribute_Quantity = TOTAL_TYPE_ATTRIBUTES,
  [EN_attribute_simpleType].Attribute = simple_type_Attr,

  [EN_attribute_simpleType].Child_Quantity = TOTAL_SIMPLE_TYPE_DESCENDANTS,
  [EN_attribute_simpleType].Child_Order    = EN_CHOICE,
  [EN_attribute_simpleType].Child          = simple_type_Descendant,
};

const xs_attribute_t attribute_Attr[TOTAL_ATTRIBUTE_ATTRIBUTES] =
{
  [EN_attribute_id].Name.String = "id",
  [EN_attribute_id].Name.Length = sizeof("id") - 1,

  [EN_attribute_id].Target.Type = EN_RELATIVE,
  [EN_attribute_id].Target.Offset = offsetof(attribute_t, attr.id),

  [EN_attribute_id].Content.Type = EN_STRING_DYNAMIC,
  [EN_attribute_id].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_attribute_id].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_attribute_id].Use = EN_OPTIONAL,

  [EN_attribute_name].Name.String = "name",
  [EN_attribute_name].Name.Length = sizeof("name") - 1,

  [EN_attribute_name].Target.Type = EN_RELATIVE,
  [EN_attribute_name].Target.Offset = offsetof(attribute_t, attr.name),

  [EN_attribute_name].Content.Type = EN_STRING_DYNAMIC,
  [EN_attribute_name].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_attribute_name].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_attribute_name].Use = EN_OPTIONAL,

  [EN_attribute_type].Name.String = "type",
  [EN_attribute_type].Name.Length = sizeof("type") - 1,

  [EN_attribute_type].Target.Type = EN_RELATIVE,
  [EN_attribute_type].Target.Offset = offsetof(attribute_t, attr.type),

  [EN_attribute_type].Content.Type = EN_STRING_DYNAMIC,
  [EN_attribute_type].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_attribute_type].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_attribute_type].Use = EN_OPTIONAL,

  [EN_attribute_ref].Name.String = "ref",
  [EN_attribute_ref].Name.Length = sizeof("ref") - 1,

  [EN_attribute_ref].Target.Type = EN_RELATIVE,
  [EN_attribute_ref].Target.Offset = offsetof(attribute_t, attr.ref),

  [EN_attribute_ref].Content.Type = EN_STRING_DYNAMIC,
  [EN_attribute_ref].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_attribute_ref].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_attribute_ref].Use = EN_OPTIONAL,

  [EN_attribute_use].Name.String = "use",
  [EN_attribute_use].Name.Length = sizeof("use") - 1,

  [EN_attribute_use].Target.Type = EN_RELATIVE,
  [EN_attribute_use].Target.Offset = offsetof(attribute_t, attr.use),

  [EN_attribute_use].Content.Type = EN_STRING_DYNAMIC,
  [EN_attribute_use].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_attribute_use].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_attribute_use].Use = EN_OPTIONAL,

  [EN_attribute_Default].Name.String = "default",
  [EN_attribute_Default].Name.Length = sizeof("default") - 1,

  [EN_attribute_Default].Target.Type = EN_RELATIVE,
  [EN_attribute_Default].Target.Offset = offsetof(attribute_t, attr.Default),

  [EN_attribute_Default].Content.Type = EN_STRING_DYNAMIC,
  [EN_attribute_Default].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_attribute_Default].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_attribute_Default].Use = EN_OPTIONAL,

  [EN_attribute_fixed].Name.String = "fixed",
  [EN_attribute_fixed].Name.Length = sizeof("fixed") - 1,

  [EN_attribute_fixed].Target.Type = EN_RELATIVE,
  [EN_attribute_fixed].Target.Offset = offsetof(attribute_t, attr.fixed),

  [EN_attribute_fixed].Content.Type = EN_STRING_DYNAMIC,
  [EN_attribute_fixed].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_attribute_fixed].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_attribute_fixed].Use = EN_OPTIONAL,
};

const xs_element_t xs_attribute =
{
  .Name.String = "xs:attribute",
  .Name.Length    = sizeof("xs:attribute") - 1,

  .MinOccur    = 0,
  .MaxOccur    = 64,

  .Callback    = traverse_up,

  .Target.Type     = EN_DYNAMIC,
  .Target.Allocate = allocate_attribute,
  .Target.Size     = sizeof(attribute_t),

  .Attribute_Quantity = TOTAL_ATTRIBUTE_ATTRIBUTES,
  .Attribute          = attribute_Attr,

  .Child_Quantity = TOTAL_ATTRIBUTE_DESCENDANTS,
  .Child_Order    = EN_CHOICE,
  .Child          = attribute_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_attribute(uint32_t occurrence, void* context)
{
  return allocate_element_type(context, sizeof(attribute_t), XS_ATTRIBUTE_TAG);
}
