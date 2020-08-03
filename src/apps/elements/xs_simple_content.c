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

void* allocate_simple_content(uint32_t occurrence, void** context);
void add_simple_content(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t simpleContent_Descendant[TOTAL_SIMPLE_CONTENT_DESCENDANTS] =
{
  [EN_simple_content_extension].Name.String  = "xs:extension",
  [EN_simple_content_extension].Name.Length  = sizeof("xs:extension") - 1,
  [EN_simple_content_extension].MinOccur     = 0,
  [EN_simple_content_extension].MaxOccur     = 64,
  [EN_simple_content_extension].Callback     = add_extension_tag,

  [EN_simple_content_extension].Target.Type  	= EN_DYNAMIC,
  [EN_simple_content_extension].Target.Allocate = allocate_extension,

  [EN_simple_content_extension].Attribute_Quantity = ARRAY_LENGTH(extension_Attr),
  [EN_simple_content_extension].Attribute 		   = extension_Attr,

  [EN_simple_content_extension].Child_Quantity = ARRAY_LENGTH(extension_Descendant),
  [EN_simple_content_extension].Child_Type     = EN_CHOICE,
  [EN_simple_content_extension].Child = extension_Descendant,

  [EN_simple_content_restriction].Name.String = "xs:restriction",
  [EN_simple_content_restriction].Name.Length = sizeof("xs:restriction") - 1,
  [EN_simple_content_restriction].MinOccur    = 0,
  [EN_simple_content_restriction].MaxOccur    = 64,
  [EN_simple_content_restriction].Callback    = add_restriction_tag,

  [EN_simple_content_restriction].Target.Type     = EN_DYNAMIC,
  [EN_simple_content_restriction].Target.Allocate = allocate_restriction,

  [EN_simple_content_restriction].Attribute_Quantity = ARRAY_LENGTH(restriction_Attr),
  [EN_simple_content_restriction].Attribute          = restriction_Attr,

  [EN_simple_content_restriction].Child_Quantity = ARRAY_LENGTH(restriction_Descendant),
  [EN_simple_content_restriction].Child_Type     = EN_CHOICE,
  [EN_simple_content_restriction].Child          = restriction_Descendant,
};

const xs_attribute_t simpleContent_Attr[TOTAL_SIMPLE_CONTENT_ATTRIBUTES] =
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

  .Attribute_Quantity = TOTAL_SIMPLE_CONTENT_ATTRIBUTES,
  .Attribute = simpleContent_Attr,

  .Child_Quantity = TOTAL_SIMPLE_CONTENT_DESCENDANTS,
  .Child_Type     = EN_CHOICE,
  .Child = simpleContent_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_simple_content(uint32_t occurrence, void** context)
{
  simpleContent_t* simpleContent = calloc(1, sizeof(simpleContent_t));
  simpleContent->Type = XS_SIMPLE_CONTENT_TAG;
  tree_t* node = create_node(simpleContent);
  add_descendant_node(*context, node);
  *context = node;
  return simpleContent;
}

void add_simple_content(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}
