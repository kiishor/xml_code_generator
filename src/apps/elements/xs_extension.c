/**
 * \file
 * \brief   implement the xs:extension

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

#include "xs_attribute.h"
#include "xs_extension.h"

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

static void* allocate_extension(uint32_t occurrence, void** context);
static void add_extension_tag(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t* extension_Descendant[] =
{
  &xs_attribute,
};

static const xs_attribute_t extension_Attr[] =
{
  [0].Name.String = "id",
  [0].Name.Length = sizeof("id") - 1,

  [0].Target.Type = EN_RELATIVE,
  [0].Target.Offset = offsetof(extension_t, attr.id),

  [0].Content.Type = XS_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_OPTIONAL,

  [1].Name.String = "base",
  [1].Name.Length = sizeof("base") - 1,

  [1].Target.Type = EN_RELATIVE,
  [1].Target.Offset = offsetof(extension_t, attr.base),

  [1].Content.Type = XS_STRING_DYNAMIC,
  [1].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [1].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [1].Use = EN_REQUIRED,
};

const xs_element_t xs_extension =
{
  .Name.String  = "xs:extension",
  .Name.Length  = sizeof("xs:extension") - 1,
  .MinOccur      = 0,
  .MaxOccur      = 64,

  .Callback      = add_extension_tag,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_extension,

  .Attribute_Quantity = ARRAY_LENGTH(extension_Attr),
  .Attribute = extension_Attr,

  .Child_Quantity = ARRAY_LENGTH(extension_Descendant),
  .Child_Type     = EN_CHOICE,
  .Child = extension_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static void* allocate_extension(uint32_t occurrence, void** context)
{
  extension_t* extension = calloc(1, sizeof(extension_t));
  extension->Type = XS_EXTENSION_TAG;
  tree_t* node = create_node(extension);
  add_descendant_node(*context, node);
  *context = node;
  return extension;
}

static void add_extension_tag(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}

