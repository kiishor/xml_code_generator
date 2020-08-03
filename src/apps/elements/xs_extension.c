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

#include "common.h"
#include "libs/parse_xml.h"
#include "apps/xsd.h"
#include "apps/tree.h"

#include "xs_attribute.h"
#include "xs_extension.h"

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

void* allocate_extension(uint32_t occurrence, void** context);
void add_extension_tag(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t extension_Descendant[TOTAL_EXTENSION_DESCENDANTS] =
{
  [EN_extension_attribute].Name.String = "xs:attribute",
  [EN_extension_attribute].Name.Length    = sizeof("xs:attribute") - 1,
  [EN_extension_attribute].MinOccur    = 0,
  [EN_extension_attribute].MaxOccur    = 64,
  [EN_extension_attribute].Callback      = add_attribute_tag,
  [EN_extension_attribute].Target.Type  = EN_DYNAMIC,
  [EN_extension_attribute].Target.Allocate = allocate_attribute,
  [EN_extension_attribute].Target.Size = sizeof(attribute_t),
  [EN_extension_attribute].Attribute_Quantity = TOTAL_ATTRIBUTE_ATTRIBUTES,
  [EN_extension_attribute].Attribute = attribute_Attr,
  [EN_extension_attribute].Child_Quantity = TOTAL_ATTRIBUTE_DESCENDANTS,
  [EN_extension_attribute].Child_Type     = EN_CHOICE,
  [EN_extension_attribute].Child = attribute_Descendant,
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

  .Callback      = add_extension_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_extension,

  .Attribute_Quantity = ARRAY_LENGTH(extension_Attr),
  .Attribute = extension_Attr,

  .Child_Quantity = ARRAY_LENGTH(extension_Descendant),
  .Child_Type     = EN_CHOICE,
  .Child = extension_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_extension(uint32_t occurrence, void** context)
{
  extension_t* extension = calloc(1, sizeof(extension_t));
  extension->Type = XS_EXTENSION_TAG;
  tree_t* node = create_node(extension);
  add_descendant_node(*context, node);
  *context = node;
  return extension;
}

void add_extension_tag(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}

