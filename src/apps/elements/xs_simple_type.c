/**
 * \file
 * \brief   implement the xs:simpleType tag

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

#include "xs_restriction.h"
#include "xs_simple_type.h"

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

static void* allocate_simple_type(uint32_t occurrence, void** context);
static void add_simple_type_tag(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t* simple_type_Descendant[] =
{
  &xs_restriction,
};

static const xs_attribute_t simple_type_Attr[] =
{
  [0].Name.String = "id",
  [0].Name.Length = sizeof("id") - 1,

  [0].Target.Type = EN_RELATIVE,
  [0].Target.Offset = offsetof(simple_type_t, attr.id),

  [0].Content.Type = EN_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_OPTIONAL,

  [1].Name.String = "name",
  [1].Name.Length = sizeof("name") - 1,

  [1].Target.Type = EN_RELATIVE,
  [1].Target.Offset = offsetof(simple_type_t, attr.name),

  [1].Content.Type = EN_STRING_DYNAMIC,
  [1].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [1].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [1].Use = EN_OPTIONAL,
};

const xs_element_t xs_simpleType =
{
  .Name.String  = "xs:simpleType",
  .Name.Length  = sizeof("xs:simpleType") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_simple_type_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_simple_type,

  .Attribute_Quantity = ARRAY_LENGTH(simple_type_Attr),
  .Attribute = simple_type_Attr,

  .Child_Quantity = ARRAY_LENGTH(simple_type_Descendant),
  .Child_Type     = EN_CHOICE,
  .Child = simple_type_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static void* allocate_simple_type(uint32_t occurrence, void** context)
{
  simple_type_t* simpleType = calloc(1, sizeof(simple_type_t));
  simpleType->Type = XS_SIMPLE_TYPE_TAG;
  tree_t* node = create_node(simpleType);
  add_descendant_node(*context, node);
  *context = node;
  return simpleType;
}

static void add_simple_type_tag(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}



