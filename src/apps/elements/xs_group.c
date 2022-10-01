/**
 * \file
 * \brief   Implements xs:group tag
 * \author  Nandkishor Biradar
 * \date    14 August 2022
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

#include "xs_group.h"
#include "xs_sequence.h"
#include "xs_choice.h"

#include "util/util.h"

#include "xs_element.h"
#include "xs_schema.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t Group_Descendant[TOTAL_GROUP_DESCENDANTS] =
{
  [EN_group_sequence].Name.String  = "xs:sequence",
  [EN_group_sequence].Name.Length  = sizeof("xs:sequence") - 1,
  [EN_group_sequence].MinOccur     = 0,
  [EN_group_sequence].MaxOccur     = 64,
  [EN_group_sequence].Callback     = traverse_up,
  [EN_group_sequence].Target.Type  	  = EN_DYNAMIC,
  [EN_group_sequence].Target.Allocate = allocate_sequence,
  [EN_group_sequence].Attribute_Quantity = TOTAL_SEQUENCE_ATTRIBUTES,
  [EN_group_sequence].Attribute 		     = sequence_attr,
  [EN_group_sequence].Child_Quantity = TOTAL_SEQUENCE_DESCENDANTS,
  [EN_group_sequence].Child_Order    = EN_CHOICE,
  [EN_group_sequence].Child 		     = Sequence_Descendant,

  [EN_group_choice].Name.String  = "xs:choice",
  [EN_group_choice].Name.Length  = sizeof("xs:choice") - 1,
  [EN_group_choice].MinOccur     = 0,
  [EN_group_choice].MaxOccur     = 64,
  [EN_group_choice].Callback     = traverse_up,
  [EN_group_choice].Target.Type  	  = EN_DYNAMIC,
  [EN_group_choice].Target.Allocate = allocate_choice,
  [EN_group_choice].Attribute_Quantity = TOTAL_CHOICE_ATTRIBUTES,
  [EN_group_choice].Attribute 		     = Choice_attr,
  [EN_group_choice].Child_Quantity = TOTAL_CHOICE_DESCENDANTS,
  [EN_group_choice].Child_Order    = EN_CHOICE,
  [EN_group_choice].Child 		     = Choice_Descendant,
};

const xs_attribute_t Group_attr[TOTAL_GROUP_ATTRIBUTES] =
{
  [EN_group_id].Name.String = "id",
  [EN_group_id].Name.Length = sizeof("id") - 1,
  [EN_group_id].Target.Type = EN_RELATIVE,
  [EN_group_id].Target.Offset = offsetof(group_t, attr.id),
  [EN_group_id].Content.Type = EN_STRING_DYNAMIC,
  [EN_group_id].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_group_id].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_group_id].Use = EN_OPTIONAL,

  [EN_group_name].Name.String = "name",
  [EN_group_name].Name.Length = sizeof("name") - 1,
  [EN_group_name].Target.Type = EN_RELATIVE,
  [EN_group_name].Target.Offset = offsetof(group_t, attr.name),
  [EN_group_name].Content.Type = EN_STRING_DYNAMIC,
  [EN_group_name].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_group_name].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_group_name].Use = EN_OPTIONAL,

  [EN_group_minOccurs].Name.String = "minOccurs",
  [EN_group_minOccurs].Name.Length = sizeof("minOccurs") - 1,
  [EN_group_minOccurs].Target.Type = EN_RELATIVE,
  [EN_group_minOccurs].Target.Offset = offsetof(group_t, attr.minOccurs),
  [EN_group_minOccurs].Content.Type = EN_UINT32,
  [EN_group_minOccurs].Content.Facet.Uint.MinValue = 0,
  [EN_group_minOccurs].Content.Facet.Uint.MaxValue = UINT32_MAX,
  [EN_group_minOccurs].Use = EN_OPTIONAL,

  [EN_group_maxOccurs].Name.String = "maxOccurs",
  [EN_group_maxOccurs].Name.Length = sizeof("maxOccurs") - 1,
  [EN_group_maxOccurs].Target.Type = EN_RELATIVE,
  [EN_group_maxOccurs].Target.Offset = offsetof(group_t, attr.maxOccurs),
  [EN_group_maxOccurs].Content.Type = EN_STRING,
  [EN_group_maxOccurs].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_group_maxOccurs].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_group_maxOccurs].Use = EN_OPTIONAL,
};

const xs_element_t xs_group =
{
  .Name.String  = "xs:group",
  .Name.Length  = sizeof("xs:group") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = traverse_up,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_group,

  .Attribute_Quantity = TOTAL_GROUP_ATTRIBUTES,
  .Attribute = Group_attr,

  .Child_Quantity = TOTAL_GROUP_DESCENDANTS,
  .Child_Order     = EN_CHOICE,
  .Child = Group_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_group(uint32_t occurrence, void* context)
{
  return allocate_element_type(context, sizeof(group_t), XS_GROUP_TAG);
}
