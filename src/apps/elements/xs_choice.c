/**
 * \file
 * \brief Implements xs:choice tag
 * \author  Nandkishor Biradar
 * \date    15 August 2020
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

#include "xs_choice.h"
#include "util/util.h"

#include "xs_element.h"
#include "xs_schema.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t Choice_Descendant[TOTAL_CHOICE_DESCENDANTS] =
{
  [EN_choice_child_element].Name.String  = "xs:element",
  [EN_choice_child_element].Name.Length  = sizeof("xs:element") - 1,
  [EN_choice_child_element].MinOccur      = 0,
  [EN_choice_child_element].MaxOccur      = 64,
  [EN_choice_child_element].Callback      = traverse_up,

  [EN_choice_child_element].Target.Type  = EN_DYNAMIC,
  [EN_choice_child_element].Target.Allocate = allocate_child_schema_element,

  [EN_choice_child_element].Attribute_Quantity = TOTAL_CHILD_ELEMENT_ATTRIBUTES,
  [EN_choice_child_element].Attribute = child_element_attr,

  [EN_choice_child_element].Child_Quantity = TOTAL_ELEMENT_DESCENDANTS,
  [EN_choice_child_element].Child_Order    = EN_CHOICE,
  [EN_choice_child_element].Child          = Element_Descendant,
};

const xs_attribute_t Choice_attr[TOTAL_CHOICE_ATTRIBUTES] =
{
  [EN_choice_id].Name.String = "id",
  [EN_choice_id].Name.Length = sizeof("id") - 1,

  [EN_choice_id].Target.Type = EN_RELATIVE,
  [EN_choice_id].Target.Offset = offsetof(choice_t, attr.id),

  [EN_choice_id].Content.Type = EN_STRING_DYNAMIC,
  [EN_choice_id].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_choice_id].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_choice_id].Use = EN_OPTIONAL,

  [EN_choice_minOccurs].Name.String = "minOccurs",
  [EN_choice_minOccurs].Name.Length = sizeof("minOccurs") - 1,

  [EN_choice_minOccurs].Target.Type = EN_RELATIVE,
  [EN_choice_minOccurs].Target.Offset = offsetof(choice_t, attr.minOccurs),

  [EN_choice_minOccurs].Content.Type = EN_UINT32,
  [EN_choice_minOccurs].Content.Facet.Uint.MinValue = 0,
  [EN_choice_minOccurs].Content.Facet.Uint.MaxValue = UINT32_MAX,

  [EN_choice_minOccurs].Use = EN_OPTIONAL,

  [EN_choice_maxOccurs].Name.String = "maxOccurs",
  [EN_choice_maxOccurs].Name.Length = sizeof("maxOccurs") - 1,

  [EN_choice_maxOccurs].Target.Type = EN_RELATIVE,
  [EN_choice_maxOccurs].Target.Offset = offsetof(choice_t, attr.maxOccurs),

  [EN_choice_maxOccurs].Content.Type = EN_STRING,
  [EN_choice_maxOccurs].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_choice_maxOccurs].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_choice_maxOccurs].Use = EN_OPTIONAL,
};

const xs_element_t xs_choice =
{
  .Name.String  = "xs:choice",
  .Name.Length  = sizeof("xs:choice") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = traverse_up,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_choice,

  .Attribute_Quantity = TOTAL_CHOICE_ATTRIBUTES,
  .Attribute = Choice_attr,

  .Child_Quantity = TOTAL_CHOICE_DESCENDANTS,
  .Child_Order     = EN_CHOICE,
  .Child = Choice_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_choice(uint32_t occurrence, void* context)
{
  return allocate_element_type(context, sizeof(choice_t), XS_CHOICE_TAG);
}
