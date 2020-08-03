/**
 * \file
 * \brief Implements xs:sequence tag

 * \author  Nandkishor Biradar
 * \date    18 June 2019
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

#include "xs_sequence.h"
#include "util/util.h"

#include "xs_element.h"

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

void* allocate_sequence(uint32_t occurrence, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t Sequence_Descendant[TOTAL_SEQUENCE_DESCENDANTS] =
{
  [EN_sequence_child_element].Name.String  = "xs:element",
  [EN_sequence_child_element].Name.Length  = sizeof("xs:element") - 1,
  [EN_sequence_child_element].MinOccur      = 0,
  [EN_sequence_child_element].MaxOccur      = 64,
  [EN_sequence_child_element].Callback      = add_child_element,

  [EN_sequence_child_element].Target.Type  = EN_DYNAMIC,
  [EN_sequence_child_element].Target.Allocate = allocate_child_schema_element,

  [EN_sequence_child_element].Attribute_Quantity = ARRAY_LENGTH(child_element_attr),
  [EN_sequence_child_element].Attribute = child_element_attr,

  [EN_sequence_child_element].Child_Quantity = TOTAL_ELEMENT_DESCENDANTS,
  [EN_sequence_child_element].Child_Type     = EN_CHOICE,
  [EN_sequence_child_element].Child          = Element_Descendant,
};

const xs_attribute_t sequence_attr[TOTAL_SEQUENCE_ATTRIBUTES] =
{
  [EN_sequence_id].Name.String = "id",
  [EN_sequence_id].Name.Length = sizeof("id") - 1,

  [EN_sequence_id].Target.Type = EN_RELATIVE,
  [EN_sequence_id].Target.Offset = offsetof(sequence_t, attr.id),

  [EN_sequence_id].Content.Type = EN_STRING_DYNAMIC,
  [EN_sequence_id].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_sequence_id].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_sequence_id].Use = EN_OPTIONAL,

  [EN_sequence_minOccurs].Name.String = "minOccurs",
  [EN_sequence_minOccurs].Name.Length = sizeof("minOccurs") - 1,

  [EN_sequence_minOccurs].Target.Type = EN_RELATIVE,
  [EN_sequence_minOccurs].Target.Offset = offsetof(sequence_t, attr.minOccurs),

  [EN_sequence_minOccurs].Content.Type = EN_UNSIGNED,
  [EN_sequence_minOccurs].Content.Facet.Uint.MinValue = 0,
  [EN_sequence_minOccurs].Content.Facet.Uint.MaxValue = UINT32_MAX,

  [EN_sequence_minOccurs].Use = EN_OPTIONAL,

  [EN_sequence_maxOccurs].Name.String = "maxOccurs",
  [EN_sequence_maxOccurs].Name.Length = sizeof("maxOccurs") - 1,

  [EN_sequence_maxOccurs].Target.Type = EN_RELATIVE,
  [EN_sequence_maxOccurs].Target.Offset = offsetof(sequence_t, attr.maxOccurs),

  [EN_sequence_maxOccurs].Content.Type = EN_UNSIGNED,
  [EN_sequence_maxOccurs].Content.Facet.Uint.MinValue = 0,
  [EN_sequence_maxOccurs].Content.Facet.Uint.MaxValue = UINT32_MAX,

  [EN_sequence_maxOccurs].Use = EN_OPTIONAL,
};

const xs_element_t xs_sequence =
{
  .Name.String  = "xs:sequence",
  .Name.Length  = sizeof("xs:sequence") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = traverse_up,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_sequence,

  .Attribute_Quantity = ARRAY_LENGTH(sequence_attr),
  .Attribute = sequence_attr,

  .Child_Quantity = ARRAY_LENGTH(Sequence_Descendant),
  .Child_Type     = EN_CHOICE,
  .Child = Sequence_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_sequence(uint32_t occurrence, void** context)
{
  sequence_t* sequence = calloc(1, sizeof(sequence_t));
  sequence->Type = XS_SEQUENCE_TAG;
  tree_t* node = create_node(sequence);
  add_descendant_node(*context, node);
  *context = node;
  return sequence;
}

