/**
 * \file
 * \brief   implement the xs:enumeration, xs:length, xs:maxInclusive, xs:minInclusive,
      xs:pattern, xs:maxLength, xs:minLength facets

 * \author  Nandkishor Biradar
 * \date    18 August 2019
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
#include "xs_facet.h"

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

static void* allocate_facet(uint32_t occurrence, void** context);
static void add_facet_tag(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

static const xs_attribute_t facet_Attr[] =
{
  [0].Name.String = "value",
  [0].Name.Length = sizeof("value") - 1,

  [0].Target.Type = EN_RELATIVE,
  [0].Target.Offset = offsetof(xs_facet_t, attr.value),

  [0].Content.Type = EN_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_REQUIRED,
};

const xs_element_t xs_enumeration =
{
  .Name.String  = "xs:enumeration",
  .Name.Length  = sizeof("xs:enumeration") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_facet_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_facet,

  .Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  .Attribute = facet_Attr,
};

const xs_element_t xs_length =
{
  .Name.String  = "xs:length",
  .Name.Length  = sizeof("xs:length") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_facet_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_facet,

  .Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  .Attribute = facet_Attr,
};

const xs_element_t xs_maxInclusive =
{
  .Name.String  = "xs:maxInclusive",
  .Name.Length  = sizeof("xs:maxInclusive") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_facet_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_facet,

  .Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  .Attribute = facet_Attr,
};

const xs_element_t xs_minInclusive =
{
  .Name.String  = "xs:minInclusive",
  .Name.Length  = sizeof("xs:minInclusive") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_facet_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_facet,

  .Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  .Attribute = facet_Attr,
};

const xs_element_t xs_pattern =
{
  .Name.String  = "xs:pattern",
  .Name.Length  = sizeof("xs:pattern") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_facet_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_facet,

  .Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  .Attribute = facet_Attr,
};

const xs_element_t xs_maxLength =
{
  .Name.String  = "xs:maxLength",
  .Name.Length  = sizeof("xs:maxLength") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_facet_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_facet,

  .Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  .Attribute = facet_Attr,
};

const xs_element_t xs_minLength =
{
  .Name.String  = "xs:minLength",
  .Name.Length  = sizeof("xs:minLength") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_facet_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_facet,

  .Attribute_Quantity = ARRAY_LENGTH(facet_Attr),
  .Attribute = facet_Attr,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static void* allocate_facet(uint32_t occurrence, void** context)
{
  xs_facet_t* facet = calloc(1, sizeof(xs_facet_t));
  facet->Type = XS_ENUMERATION_TAG;
  tree_t* node = create_node(facet);
  add_descendant_node(*context, node);
  *context = node;
  return facet;
}

static void add_facet_tag(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}


