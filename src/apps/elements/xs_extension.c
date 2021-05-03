/**
 * \file
 * \brief   implement the xs:extension

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
#include "apps/xsd.h"
#include "apps/tree.h"

#include "xs_attribute.h"
#include "xs_extension.h"
#include "xs_schema.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t extension_Descendant[TOTAL_EXTENSION_DESCENDANTS] =
{
  [EN_extension_attribute].Name.String = "xs:attribute",
  [EN_extension_attribute].Name.Length    = sizeof("xs:attribute") - 1,
  [EN_extension_attribute].MinOccur    = 0,
  [EN_extension_attribute].MaxOccur    = 64,
  [EN_extension_attribute].Callback      = traverse_up,
  [EN_extension_attribute].Target.Type  = EN_DYNAMIC,
  [EN_extension_attribute].Target.Allocate = allocate_attribute,
  [EN_extension_attribute].Target.Size = sizeof(attribute_t),
  [EN_extension_attribute].Attribute_Quantity = TOTAL_ATTRIBUTE_ATTRIBUTES,
  [EN_extension_attribute].Attribute = attribute_Attr,
  [EN_extension_attribute].Child_Quantity = TOTAL_ATTRIBUTE_DESCENDANTS,
  [EN_extension_attribute].Child_Order    = EN_CHOICE,
  [EN_extension_attribute].Child          = attribute_Descendant,
};

const xs_attribute_t extension_Attr[] =
{
  [EN_extension_id].Name.String = "id",
  [EN_extension_id].Name.Length = sizeof("id") - 1,
  [EN_extension_id].Target.Type = EN_RELATIVE,
  [EN_extension_id].Target.Offset = offsetof(extension_t, attr.id),
  [EN_extension_id].Content.Type = EN_STRING_DYNAMIC,
  [EN_extension_id].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_extension_id].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_extension_id].Use = EN_OPTIONAL,

  [EN_extension_base].Name.String = "base",
  [EN_extension_base].Name.Length = sizeof("base") - 1,
  [EN_extension_base].Target.Type = EN_RELATIVE,
  [EN_extension_base].Target.Offset = offsetof(extension_t, attr.base),
  [EN_extension_base].Content.Type = EN_STRING_DYNAMIC,
  [EN_extension_base].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_extension_base].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_extension_base].Use = EN_REQUIRED,
};

const xs_element_t xs_extension =
{
  .Name.String  = "xs:extension",
  .Name.Length  = sizeof("xs:extension") - 1,
  .MinOccur      = 0,
  .MaxOccur      = 64,

  .Callback      = traverse_up,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_extension,

  .Attribute_Quantity = TOTAL_EXTENSION_ATTRIBUTES,
  .Attribute = extension_Attr,

  .Child_Quantity = TOTAL_EXTENSION_DESCENDANTS,
  .Child_Order    = EN_CHOICE,
  .Child          = extension_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_extension(uint32_t occurrence, void* context)
{
  return allocate_element_type(context, sizeof(extension_t), XS_EXTENSION_TAG);
}

