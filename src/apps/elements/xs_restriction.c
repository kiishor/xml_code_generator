/**
 * \file
 * \brief   implement the xs:restriction

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

#include "xs_simple_type.h"
#include "xs_attribute.h"
#include "xs_facet.h"
#include "xs_restriction.h"

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

static void* allocate_restriction(uint32_t occurrence, void** context);
static void add_restriction_tag(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t* restriction_Descendant[] =
{
  &xs_attribute,
  &xs_simpleType,
  &xs_enumeration,
  &xs_length,
  &xs_minInclusive,
  &xs_maxInclusive,
  &xs_pattern,
  &xs_maxLength,
  &xs_minLength,
};

static const xs_attribute_t restriction_Attr[] =
{
  [0].Name.String = "id",
  [0].Name.Length = sizeof("id") - 1,

  [0].Target.Type = EN_RELATIVE,
  [0].Target.Offset = offsetof(restriction_t, attr.id),

  [0].Content.Type = XS_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_OPTIONAL,

  [1].Name.String = "base",
  [1].Name.Length = sizeof("base") - 1,

  [1].Target.Type = EN_RELATIVE,
  [1].Target.Offset = offsetof(restriction_t, attr.base),

  [1].Content.Type = XS_STRING_DYNAMIC,
  [1].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [1].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [1].Use = EN_REQUIRED,
};

const xs_element_t xs_restriction =
{
  .Name.String = "xs:restriction",
  .Name.Length = sizeof("xs:restriction") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_restriction_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_restriction,

  .Attribute_Quantity = ARRAY_LENGTH(restriction_Attr),
  .Attribute = restriction_Attr,

  .Child_Quantity = ARRAY_LENGTH(restriction_Descendant),
  .Child_Type     = EN_CHOICE,
  .Child = restriction_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static void* allocate_restriction(uint32_t occurrence, void** context)
{
  restriction_t* restriction = calloc(1, sizeof(restriction_t));
  restriction->Type = XS_RESTRICTION_TAG;
  tree_t* node = create_node(restriction);
  add_descendant_node(*context, node);
  *context = node;
  return restriction;
}

static void add_restriction_tag(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}


