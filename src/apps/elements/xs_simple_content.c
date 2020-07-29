/**
 * \file
 * \brief   implement the xs:simpleContent tag

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

#include "xs_extension.h"
#include "xs_restriction.h"
#include "xs_simple_content.h"

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

static void* allocate_simple_content(uint32_t occurrence, void** context);
static void add_simple_content(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t* simpleContent_Descendant[] =
{
  &xs_extension,
  &xs_restriction,
};

static const xs_attribute_t simpleContent_Attr[] =
{
  [0].Name.String = "id",
  [0].Name.Length = sizeof("id") - 1,

  [0].Target.Type = EN_RELATIVE,
  [0].Target.Offset = offsetof(simpleContent_t, attr.id),

  [0].Content.Type = EN_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_OPTIONAL,
};

const xs_element_t xs_simpleContent =
{
  .Name.String  = "xs:simpleContent",
  .Name.Length  = sizeof("xs:simpleContent") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 64,
  .Callback      = add_simple_content,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_simple_content,

  .Child_Quantity = ARRAY_LENGTH(simpleContent_Descendant),
  .Child_Type     = EN_CHOICE,
  .Child = simpleContent_Descendant,
  .Attribute_Quantity = ARRAY_LENGTH(simpleContent_Attr),
  .Attribute = simpleContent_Attr,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static void* allocate_simple_content(uint32_t occurrence, void** context)
{
  simpleContent_t* simpleContent = calloc(1, sizeof(simpleContent_t));
  simpleContent->Type = XS_SIMPLE_CONTENT_TAG;
  tree_t* node = create_node(simpleContent);
  add_descendant_node(*context, node);
  *context = node;
  return simpleContent;
}

static void add_simple_content(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}
