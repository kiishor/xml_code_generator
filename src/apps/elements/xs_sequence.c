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
#include "xs_sequence.h"
#include "util/util.h"

#include "xs_element.h"

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

static void* allocate_sequence(uint32_t occurrence, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t* Sequence_Descendant[] =
{
  &xs_child_element,
};

static const xs_attribute_t sequence_attr[] =
{
  [0].Name.String = "id",
  [0].Name.Length = sizeof("id") - 1,

  [0].Target.Type = EN_RELATIVE,
  [0].Target.Offset = offsetof(sequence_t, attr.id),

  [0].Content.Type = XS_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_OPTIONAL,

  [1].Name.String = "minOccurs",
  [1].Name.Length = sizeof("minOccurs") - 1,

  [1].Target.Type = EN_RELATIVE,
  [1].Target.Offset = offsetof(sequence_t, attr.minOccurs),

  [1].Content.Type = XS_UNSIGNED,
  [1].Content.Facet.Uint.MinValue = 0,
  [1].Content.Facet.Uint.MaxValue = UINT32_MAX,

  [1].Use = EN_OPTIONAL,

  [2].Name.String = "maxOccurs",
  [2].Name.Length = sizeof("maxOccurs") - 1,

  [2].Target.Type = EN_RELATIVE,
  [2].Target.Offset = offsetof(sequence_t, attr.maxOccurs),

  [2].Content.Type = XS_UNSIGNED,
  [2].Content.Facet.Uint.MinValue = 0,
  [2].Content.Facet.Uint.MaxValue = UINT32_MAX,

  [2].Use = EN_OPTIONAL,
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

static void* allocate_sequence(uint32_t occurrence, void** context)
{
  sequence_t* sequence = calloc(1, sizeof(sequence_t));
  sequence->Type = XS_SEQUENCE_TAG;
  tree_t* node = create_node(sequence);
  add_descendant_node(*context, node);
  *context = node;
  return sequence;
}

