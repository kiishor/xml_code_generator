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

#include "common.h"
#include "libs/parse_xml.h"
#include "apps/tree.h"
#include "apps/xsd.h"

#include "xs_simple_type.h"
#include "xs_attribute.h"


/*
 *  --------------------------- FORWARD DECLARATION ---------------------------
 */

static void* allocate_attribute(uint32_t occurrence, void** context);
static void add_attribute_tag(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t* attribute_Descendant[] =
{
  &xs_simpleType,
};

static const xs_attribute_t attribute_Attr[] =
{
  [0].Name.String = "id",
  [0].Name.Length = sizeof("id") - 1,

  [0].Target.Type = EN_RELATIVE,
  [0].Target.Offset = offsetof(attribute_t, attr.id),

  [0].Content.Type = EN_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_OPTIONAL,

  [1].Name.String = "name",
  [1].Name.Length = sizeof("name") - 1,

  [1].Target.Type = EN_RELATIVE,
  [1].Target.Offset = offsetof(attribute_t, attr.name),

  [1].Content.Type = EN_STRING_DYNAMIC,
  [1].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [1].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [1].Use = EN_OPTIONAL,

  [2].Name.String = "type",
  [2].Name.Length = sizeof("type") - 1,

  [2].Target.Type = EN_RELATIVE,
  [2].Target.Offset = offsetof(attribute_t, attr.type),

  [2].Content.Type = EN_STRING_DYNAMIC,
  [2].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [2].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [2].Use = EN_OPTIONAL,

  [3].Name.String = "ref",
  [3].Name.Length = sizeof("ref") - 1,

  [3].Target.Type = EN_RELATIVE,
  [3].Target.Offset = offsetof(attribute_t, attr.ref),

  [3].Content.Type = EN_STRING_DYNAMIC,
  [3].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [3].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [3].Use = EN_OPTIONAL,

  [4].Name.String = "use",
  [4].Name.Length = sizeof("use") - 1,

  [4].Target.Type = EN_RELATIVE,
  [4].Target.Offset = offsetof(attribute_t, attr.use),

  [4].Content.Type = EN_STRING_DYNAMIC,
  [4].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [4].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [4].Use = EN_OPTIONAL,

  [5].Name.String = "default",
  [5].Name.Length = sizeof("default") - 1,

  [5].Target.Type = EN_RELATIVE,
  [5].Target.Offset = offsetof(attribute_t, attr.Default),

  [5].Content.Type = EN_STRING_DYNAMIC,
  [5].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [5].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [5].Use = EN_OPTIONAL,

  [6].Name.String = "fixed",
  [6].Name.Length = sizeof("fixed") - 1,

  [6].Target.Type = EN_RELATIVE,
  [6].Target.Offset = offsetof(attribute_t, attr.fixed),

  [6].Content.Type = EN_STRING_DYNAMIC,
  [6].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [6].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [6].Use = EN_OPTIONAL,
};

const xs_element_t xs_attribute =
{
  .Name.String = "xs:attribute",
  .Name.Length    = sizeof("xs:attribute") - 1,

  .MinOccur    = 0,
  .MaxOccur    = 64,

  .Callback      = add_attribute_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_attribute,
  .Target.Size = sizeof(attribute_t),

  .Attribute_Quantity = ARRAY_LENGTH(attribute_Attr),
  .Attribute = attribute_Attr,

  .Child_Quantity = ARRAY_LENGTH(attribute_Descendant),
  .Child_Type     = EN_CHOICE,
  .Child = attribute_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static void* allocate_attribute(uint32_t occurrence, void** context)
{
  attribute_t* attribute = calloc(1, sizeof(attribute_t));
  attribute->Type = XS_ATTRIBUTE_TAG;
  tree_t* node = create_node(attribute);
  add_descendant_node(*context, node);
  *context = node;
  return attribute;
}

static void add_attribute_tag(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}


