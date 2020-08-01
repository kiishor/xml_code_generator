/**
 * \file
 * \brief  Implements xs:element tag

 * \author  Nandkishor Biradar
 * \date  14th August 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "common.h"
#include "libs/parse_xml.h"
#include "apps/xsd.h"
#include "apps/tree.h"

#include "xs_element.h"
#include "xs_complex_type.h"
#include "xs_simple_type.h"

/*
 *  --------------------------- FORWARD DECLARATION ---------------------------
 */

void add_global_element(uint32_t occurrence, void* content, void** context);
void add_child_element(uint32_t occurrence, void* content, void** context);

void* allocate_global_schema_element(uint32_t, void**);
void* allocate_child_schema_element(uint32_t, void**);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t* Element_Descendant[] =
{
  &xs_complexType,
  &xs_simpleType,
};

static const xs_attribute_t child_element_attr[] =
{
  [0].Name.String = "id",
  [0].Name.Length = sizeof("id") - 1,

  [0].Target.Type = EN_RELATIVE,
  [0].Target.Offset = offsetof(element_t, child.id),

  [0].Content.Type = EN_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_OPTIONAL,

  [1].Name.String = "name",
  [1].Name.Length = sizeof("name") - 1,

  [1].Target.Type = EN_RELATIVE,
  [1].Target.Offset = offsetof(element_t, child.name),

  [1].Content.Type = EN_STRING_DYNAMIC,
  [1].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [1].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [1].Use = EN_OPTIONAL,

  [2].Name.String = "type",
  [2].Name.Length = sizeof("type") - 1,

  [2].Target.Type = EN_RELATIVE,
  [2].Target.Offset = offsetof(element_t, child.type),

  [2].Content.Type = EN_STRING_DYNAMIC,
  [2].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [2].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [2].Use = EN_OPTIONAL,

  [3].Name.String = "ref",
  [3].Name.Length = sizeof("ref") - 1,

  [3].Target.Type = EN_RELATIVE,
  [3].Target.Offset = offsetof(element_t, child.ref),

  [3].Content.Type = EN_STRING_DYNAMIC,
  [3].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [3].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [3].Use = EN_OPTIONAL,

  [4].Name.String = "minOccurs",
  [4].Name.Length = sizeof("minOccurs") - 1,

  [4].Target.Type = EN_RELATIVE,
  [4].Target.Offset = offsetof(element_t, child.minOccurs),

  [4].Content.Type = EN_UNSIGNED,
  [4].Content.Facet.Uint.MinValue = 0,
  [4].Content.Facet.Uint.MaxValue = UINT32_MAX,

  [4].Use = EN_OPTIONAL,

  [5].Name.String = "maxOccurs",
  [5].Name.Length = sizeof("maxOccurs") - 1,

  [5].Target.Type = EN_RELATIVE,
  [5].Target.Offset = offsetof(element_t, child.maxOccurs),

  [5].Content.Type = EN_STRING,

  [5].Use = EN_OPTIONAL,
};

static const xs_attribute_t global_element_attr[] =
{
  [0].Name.String = "id",
  [0].Name.Length = sizeof("id") - 1,

  [0].Target.Type = EN_RELATIVE,
  [0].Target.Offset = offsetof(element_t, global.id),

  [0].Content.Type = EN_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_OPTIONAL,

  [1].Name.String = "name",
  [1].Name.Length = sizeof("name") - 1,

  [1].Target.Type = EN_RELATIVE,
  [1].Target.Offset = offsetof(element_t, global.name),

  [1].Content.Type = EN_STRING_DYNAMIC,
  [1].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [1].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [1].Use = EN_REQUIRED,

  [2].Name.String = "type",
  [2].Name.Length = sizeof("type") - 1,

  [2].Target.Type = EN_RELATIVE,
  [2].Target.Offset = offsetof(element_t, global.type),

  [2].Content.Type = EN_STRING_DYNAMIC,
  [2].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [2].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [2].Use = EN_OPTIONAL,
};

const xs_element_t xs_global_element =
{
  .Name.String  = "xs:element",
  .Name.Length  = sizeof("xs:element") - 1,
  .MinOccur      = 0,
  .MaxOccur      = 64,

  .Callback      = add_global_element,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_global_schema_element,

  .Attribute_Quantity = ARRAY_LENGTH(global_element_attr),
  .Attribute = global_element_attr,

  .Child_Quantity = ARRAY_LENGTH(Element_Descendant),
  .Child_Type     = EN_CHOICE,
  .Child = Element_Descendant,
};

const xs_element_t xs_child_element =
{
  .Name.String  = "xs:element",
  .Name.Length  = sizeof("xs:element") - 1,
  .MinOccur      = 0,
  .MaxOccur      = 64,

  .Callback      = add_child_element,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_child_schema_element,

  .Attribute_Quantity = ARRAY_LENGTH(child_element_attr),
  .Attribute = child_element_attr,

  .Child_Quantity = ARRAY_LENGTH(Element_Descendant),
  .Child_Type     = EN_CHOICE,
  .Child = Element_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_global_schema_element(uint32_t occurrence, void** context)
{
  element_t* element = calloc(1, sizeof(element_t));
  element->Type = XS_GLOBAL_ELEMENT_TAG;
  tree_t* node = create_node(element);
  add_descendant_node(*context, node);
  *context = node;
  return element;
}

void* allocate_child_schema_element(uint32_t occurrence, void** context)
{
  element_t* element = calloc(1, sizeof(element_t));
  element->Type = XS_CHILD_ELEMENT_TAG;
  element->child.minOccurs = 1;
  tree_t* node = create_node(element);
  add_descendant_node(*context, node);
  *context = node;
  return element;
}

void add_global_element(uint32_t occurrence, void *content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}

void add_child_element(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}

