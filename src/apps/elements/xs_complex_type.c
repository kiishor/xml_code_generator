/**
 * \file
 * \brief  Implements the xs:complexType schema tag

 * \author  Nandkishor Biradar
 * \date  29 September 2019
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

#include "xs_complex_type.h"
#include "xs_sequence.h"
#include "xs_simple_content.h"
#include "xs_attribute.h"

/*
 *  --------------------------- FORWARD DECLARATION ---------------------------
 */

static void* allocate_complex_type(uint32_t occurrence,void** context);
static void add_complex_type(uint32_t occurrence, void* content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t* ComplexType_Descendant[] =
{
  &xs_sequence,
  &xs_simpleContent,
  &xs_attribute,
};

static const xs_attribute_t complexType_Attr[] =
{
  [0].Name.String = "id",
  [0].Name.Length = sizeof("id") - 1,

  [0].Target.Type   = EN_RELATIVE,
  [0].Target.Offset = offsetof(complexType_t, attr.id),

  [0].Content.Type = XS_STRING_DYNAMIC,
  [0].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [0].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [0].Use = EN_OPTIONAL,

  [1].Name.String = "name",
  [1].Name.Length = sizeof("name") - 1,

  [1].Target.Type = EN_RELATIVE,
  [1].Target.Offset = offsetof(complexType_t, attr.name),

  [1].Content.Type = XS_STRING_DYNAMIC,
  [1].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [1].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [1].Use = EN_OPTIONAL,
};

const xs_element_t xs_complexType =
{
  .Name.String  = "xs:complexType",
  .Name.Length  = sizeof("xs:complexType") - 1,
  .MinOccur      = 0,
  .MaxOccur      = 64,

  .Callback      = add_complex_type,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_complex_type,
  .Target.Size   = sizeof(complexType_t),

  .Attribute_Quantity = ARRAY_LENGTH(complexType_Attr),
  .Attribute = complexType_Attr,

  .Child_Quantity = ARRAY_LENGTH(ComplexType_Descendant),
  .Child_Type     = EN_CHOICE,
  .Child = ComplexType_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static void* allocate_complex_type(uint32_t occurrence, void** context)
{
  complexType_t* complexType = calloc(1, sizeof(complexType_t));
  complexType->Type = XS_COMPLEX_TAG;
  tree_t* node = create_node(complexType);
  add_descendant_node(*context, node);
  *context = node;
  return complexType;
}

static void add_complex_type(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}
