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

#include "parse_xml.h"
#include "apps/xsd.h"
#include "apps/tree.h"

#include "xs_extension.h"
#include "xs_restriction.h"
#include "xs_simple_content.h"
#include "xs_schema.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t simpleContent_Descendant[TOTAL_SIMPLE_CONTENT_DESCENDANTS] =
{
  [EN_simple_content_extension].Name.String  = "xs:extension",
  [EN_simple_content_extension].Name.Length  = sizeof("xs:extension") - 1,
  [EN_simple_content_extension].MinOccur     = 0,
  [EN_simple_content_extension].MaxOccur     = 64,
  [EN_simple_content_extension].Callback     = traverse_up,

  [EN_simple_content_extension].Target.Type  	= EN_DYNAMIC,
  [EN_simple_content_extension].Target.Allocate = allocate_extension,

  [EN_simple_content_extension].Attribute_Quantity = TOTAL_EXTENSION_ATTRIBUTES,
  [EN_simple_content_extension].Attribute 		   = extension_Attr,

  [EN_simple_content_extension].Child_Quantity = TOTAL_EXTENSION_DESCENDANTS,
  [EN_simple_content_extension].Child_Order     = EN_CHOICE,
  [EN_simple_content_extension].Child = extension_Descendant,

  [EN_simple_content_restriction].Name.String = "xs:restriction",
  [EN_simple_content_restriction].Name.Length = sizeof("xs:restriction") - 1,
  [EN_simple_content_restriction].MinOccur    = 0,
  [EN_simple_content_restriction].MaxOccur    = 64,
  [EN_simple_content_restriction].Callback    = traverse_up,

  [EN_simple_content_restriction].Target.Type     = EN_DYNAMIC,
  [EN_simple_content_restriction].Target.Allocate = allocate_restriction,

  [EN_simple_content_restriction].Attribute_Quantity = TOTAL_RESTRICTION_ATTRIBUTES,
  [EN_simple_content_restriction].Attribute          = restriction_Attr,

  [EN_simple_content_restriction].Child_Quantity = TOTAL_RESTRICTION_DESCENDANTS,
  [EN_simple_content_restriction].Child_Order     = EN_RANDOM,
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
  .Callback      = traverse_up,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_simple_content,

  .Attribute_Quantity = TOTAL_SIMPLE_CONTENT_ATTRIBUTES,
  .Attribute = simpleContent_Attr,

  .Child_Quantity = TOTAL_SIMPLE_CONTENT_DESCENDANTS,
  .Child_Order     = EN_CHOICE,
  .Child = simpleContent_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_simple_content(uint32_t occurrence, void* context)
{
  return allocate_element_type(context, sizeof(simpleContent_t), XS_SIMPLE_CONTENT_TAG);
}
