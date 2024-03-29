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

#include "parse_xml.h"
#include "apps/tree.h"
#include "apps/xsd.h"

#include "xs_complex_type.h"
#include "xs_sequence.h"
#include "xs_choice.h"
#include "xs_simple_content.h"
#include "xs_attribute.h"
#include "xs_schema.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t ComplexType_Descendant[TOTAL_COMPLEX_TYPE_DESCENDANT] =
{
  [EN_complex_sequence].Name.String  = "xs:sequence",
  [EN_complex_sequence].Name.Length  = sizeof("xs:sequence") - 1,
  [EN_complex_sequence].MinOccur     = 0,
  [EN_complex_sequence].MaxOccur     = 64,
  [EN_complex_sequence].Callback     = traverse_up,
  [EN_complex_sequence].Target.Type  	  = EN_DYNAMIC,
  [EN_complex_sequence].Target.Allocate = allocate_sequence,
  [EN_complex_sequence].Attribute_Quantity = TOTAL_SEQUENCE_ATTRIBUTES,
  [EN_complex_sequence].Attribute 		     = sequence_attr,
  [EN_complex_sequence].Child_Quantity = TOTAL_SEQUENCE_DESCENDANTS,
  [EN_complex_sequence].Child_Order    = EN_CHOICE,
  [EN_complex_sequence].Child 		     = Sequence_Descendant,

  [EN_complex_choice].Name.String  = "xs:choice",
  [EN_complex_choice].Name.Length  = sizeof("xs:choice") - 1,
  [EN_complex_choice].MinOccur     = 0,
  [EN_complex_choice].MaxOccur     = 64,
  [EN_complex_choice].Callback     = traverse_up,
  [EN_complex_choice].Target.Type  	  = EN_DYNAMIC,
  [EN_complex_choice].Target.Allocate = allocate_choice,
  [EN_complex_choice].Attribute_Quantity = TOTAL_CHOICE_ATTRIBUTES,
  [EN_complex_choice].Attribute 		     = Choice_attr,
  [EN_complex_choice].Child_Quantity = TOTAL_CHOICE_DESCENDANTS,
  [EN_complex_choice].Child_Order    = EN_CHOICE,
  [EN_complex_choice].Child 		     = Choice_Descendant,

  [EN_complex_simpleContent].Name.String  = "xs:simpleContent",
  [EN_complex_simpleContent].Name.Length  = sizeof("xs:simpleContent") - 1,
  [EN_complex_simpleContent].MinOccur     = 0,
  [EN_complex_simpleContent].MaxOccur     = 64,
  [EN_complex_simpleContent].Callback     = traverse_up,
  [EN_complex_simpleContent].Target.Type  	 = EN_DYNAMIC,
  [EN_complex_simpleContent].Target.Allocate = allocate_simple_content,
  [EN_complex_simpleContent].Attribute_Quantity = TOTAL_SIMPLE_CONTENT_ATTRIBUTES,
  [EN_complex_simpleContent].Attribute          = simpleContent_Attr,
  [EN_complex_simpleContent].Child_Quantity = TOTAL_SIMPLE_CONTENT_DESCENDANTS,
  [EN_complex_simpleContent].Child_Order    = EN_CHOICE,
  [EN_complex_simpleContent].Child          = simpleContent_Descendant,

  [EN_complex_attribute].Name.String = "xs:attribute",
  [EN_complex_attribute].Name.Length = sizeof("xs:attribute") - 1,
  [EN_complex_attribute].MinOccur    = 0,
  [EN_complex_attribute].MaxOccur    = 64,
  [EN_complex_attribute].Callback    = traverse_up,
  [EN_complex_attribute].Target.Type     = EN_DYNAMIC,
  [EN_complex_attribute].Target.Allocate = allocate_attribute,
  [EN_complex_attribute].Attribute_Quantity = TOTAL_ATTRIBUTE_ATTRIBUTES,
  [EN_complex_attribute].Attribute          = attribute_Attr,
  [EN_complex_attribute].Child_Quantity = TOTAL_ATTRIBUTE_DESCENDANTS,
  [EN_complex_attribute].Child_Order    = EN_CHOICE,
  [EN_complex_attribute].Child          = attribute_Descendant,
};

const xs_attribute_t complexType_Attr[] =
{
  [EN_complexType_id].Name.String = "id",
  [EN_complexType_id].Name.Length = sizeof("id") - 1,
  [EN_complexType_id].Target.Type   = EN_RELATIVE,
  [EN_complexType_id].Target.Offset = offsetof(complexType_t, attr.id),
  [EN_complexType_id].Content.Type = EN_STRING_DYNAMIC,
  [EN_complexType_id].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_complexType_id].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_complexType_id].Use = EN_OPTIONAL,

  [EN_complexType_name].Name.String = "name",
  [EN_complexType_name].Name.Length = sizeof("name") - 1,
  [EN_complexType_name].Target.Type = EN_RELATIVE,
  [EN_complexType_name].Target.Offset = offsetof(complexType_t, attr.name),
  [EN_complexType_name].Content.Type = EN_STRING_DYNAMIC,
  [EN_complexType_name].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_complexType_name].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_complexType_name].Use = EN_OPTIONAL,

  [EN_complexType_abstract].Name.String = "abstract",
  [EN_complexType_abstract].Name.Length = sizeof("abstract") - 1,
  [EN_complexType_abstract].Target.Type = EN_RELATIVE,
  [EN_complexType_abstract].Target.Offset = offsetof(complexType_t, attr.abstract),
  [EN_complexType_abstract].Content.Type = EN_STRING_DYNAMIC,
  [EN_complexType_abstract].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_complexType_abstract].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_complexType_abstract].Use = EN_OPTIONAL,

  [EN_complexType_mixed].Name.String = "mixed",
  [EN_complexType_mixed].Name.Length = sizeof("mixed") - 1,
  [EN_complexType_mixed].Target.Type = EN_RELATIVE,
  [EN_complexType_mixed].Target.Offset = offsetof(complexType_t, attr.mixed),
  [EN_complexType_mixed].Content.Type = EN_STRING_DYNAMIC,
  [EN_complexType_mixed].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_complexType_mixed].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_complexType_mixed].Use = EN_OPTIONAL,

  [EN_complexType_block].Name.String = "block",
  [EN_complexType_block].Name.Length = sizeof("block") - 1,
  [EN_complexType_block].Target.Type = EN_RELATIVE,
  [EN_complexType_block].Target.Offset = offsetof(complexType_t, attr.block),
  [EN_complexType_block].Content.Type = EN_STRING_DYNAMIC,
  [EN_complexType_block].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_complexType_block].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_complexType_block].Use = EN_OPTIONAL,

  [EN_complexType_Final].Name.String = "final",
  [EN_complexType_Final].Name.Length = sizeof("final") - 1,
  [EN_complexType_Final].Target.Type = EN_RELATIVE,
  [EN_complexType_Final].Target.Offset = offsetof(complexType_t, attr.Final),
  [EN_complexType_Final].Content.Type = EN_STRING_DYNAMIC,
  [EN_complexType_Final].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_complexType_Final].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,
  [EN_complexType_Final].Use = EN_OPTIONAL,
};

const xs_element_t xs_complexType =
{
  .Name.String  = "xs:complexType",
  .Name.Length  = sizeof("xs:complexType") - 1,
  .MinOccur      = 0,
  .MaxOccur      = 64,

  .Callback      = traverse_up,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_complex_type,
  .Target.Size   = sizeof(complexType_t),

  .Attribute_Quantity = TOTAL_COMPLEX_TYPE_ATTRIBUTES,
  .Attribute = complexType_Attr,

  .Child_Quantity = TOTAL_COMPLEX_TYPE_DESCENDANT,
  .Child_Order    = EN_RANDOM,
  .Child          = ComplexType_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_complex_type(uint32_t occurrence, void* context)
{
  return allocate_element_type(context, sizeof(complexType_t), XS_COMPLEX_TAG);
}

