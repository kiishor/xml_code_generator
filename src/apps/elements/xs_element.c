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

#include "parse_xml.h"
#include "apps/xsd.h"
#include "apps/tree.h"

#include "xs_element.h"
#include "xs_complex_type.h"
#include "xs_simple_type.h"
#include "xs_schema.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

const xs_element_t Element_Descendant[TOTAL_ELEMENT_DESCENDANTS] =
{
  [EN_element_complexType].Name.String  = "xs:complexType",
  [EN_element_complexType].Name.Length  = sizeof("xs:complexType") - 1,
  [EN_element_complexType].MinOccur      = 0,
  [EN_element_complexType].MaxOccur      = 64,

  [EN_element_complexType].Callback      = traverse_up,

  [EN_element_complexType].Target.Type  = EN_DYNAMIC,
  [EN_element_complexType].Target.Allocate = allocate_complex_type,
  [EN_element_complexType].Target.Size   = sizeof(complexType_t),

  [EN_element_complexType].Attribute_Quantity = TOTAL_TYPE_ATTRIBUTES,
  [EN_element_complexType].Attribute = complexType_Attr,

  [EN_element_complexType].Child_Quantity = TOTAL_COMPLEX_TYPE_DESCENDANT,
  [EN_element_complexType].Child_Order    = EN_RANDOM,
  [EN_element_complexType].Child          = ComplexType_Descendant,

  [EN_element_simpleType].Name.String  = "xs:simpleType",
  [EN_element_simpleType].Name.Length  = sizeof("xs:simpleType") - 1,
  [EN_element_simpleType].MinOccur     = 0,
  [EN_element_simpleType].MaxOccur     = 64,
  [EN_element_simpleType].Callback     = traverse_up,

  [EN_element_simpleType].Target.Type  = EN_DYNAMIC,
  [EN_element_simpleType].Target.Allocate = allocate_simple_type,

  [EN_element_simpleType].Attribute_Quantity = TOTAL_TYPE_ATTRIBUTES,
  [EN_element_simpleType].Attribute = simple_type_Attr,

  [EN_element_simpleType].Child_Quantity = TOTAL_SIMPLE_TYPE_DESCENDANTS,
  [EN_element_simpleType].Child_Order    = EN_CHOICE,
  [EN_element_simpleType].Child          = simple_type_Descendant,
};

const xs_attribute_t child_element_attr[TOTAL_CHILD_ELEMENT_ATTRIBUTES] =
{
  [EN_child_id].Name.String = "id",
  [EN_child_id].Name.Length = sizeof("id") - 1,

  [EN_child_id].Target.Type = EN_RELATIVE,
  [EN_child_id].Target.Offset = offsetof(element_t, child.id),

  [EN_child_id].Content.Type = EN_STRING_DYNAMIC,
  [EN_child_id].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_child_id].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_child_id].Use = EN_OPTIONAL,

  [EN_child_name].Name.String = "name",
  [EN_child_name].Name.Length = sizeof("name") - 1,

  [EN_child_name].Target.Type = EN_RELATIVE,
  [EN_child_name].Target.Offset = offsetof(element_t, child.name),

  [EN_child_name].Content.Type = EN_STRING_DYNAMIC,
  [EN_child_name].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_child_name].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_child_name].Use = EN_OPTIONAL,

  [EN_child_type].Name.String = "type",
  [EN_child_type].Name.Length = sizeof("type") - 1,

  [EN_child_type].Target.Type = EN_RELATIVE,
  [EN_child_type].Target.Offset = offsetof(element_t, child.type),

  [EN_child_type].Content.Type = EN_STRING_DYNAMIC,
  [EN_child_type].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_child_type].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_child_type].Use = EN_OPTIONAL,

  [EN_child_ref].Name.String = "ref",
  [EN_child_ref].Name.Length = sizeof("ref") - 1,

  [EN_child_ref].Target.Type = EN_RELATIVE,
  [EN_child_ref].Target.Offset = offsetof(element_t, child.ref),

  [EN_child_ref].Content.Type = EN_STRING_DYNAMIC,
  [EN_child_ref].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_child_ref].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_child_ref].Use = EN_OPTIONAL,

  [EN_child_minOccurs].Name.String = "minOccurs",
  [EN_child_minOccurs].Name.Length = sizeof("minOccurs") - 1,

  [EN_child_minOccurs].Target.Type = EN_RELATIVE,
  [EN_child_minOccurs].Target.Offset = offsetof(element_t, child.minOccurs),

  [EN_child_minOccurs].Content.Type = EN_UNSIGNED,
  [EN_child_minOccurs].Content.Facet.Uint.MinValue = 0,
  [EN_child_minOccurs].Content.Facet.Uint.MaxValue = UINT32_MAX,

  [EN_child_minOccurs].Use = EN_OPTIONAL,

  [EN_child_maxOccurs].Name.String = "maxOccurs",
  [EN_child_maxOccurs].Name.Length = sizeof("maxOccurs") - 1,

  [EN_child_maxOccurs].Target.Type = EN_RELATIVE,
  [EN_child_maxOccurs].Target.Offset = offsetof(element_t, child.maxOccurs),

  [EN_child_maxOccurs].Content.Type = EN_STRING,

  [EN_child_maxOccurs].Use = EN_OPTIONAL,
};

const xs_attribute_t global_element_attr[] =
{
  [EN_global_id].Name.String = "id",
  [EN_global_id].Name.Length = sizeof("id") - 1,

  [EN_global_id].Target.Type = EN_RELATIVE,
  [EN_global_id].Target.Offset = offsetof(element_t, global.id),

  [EN_global_id].Content.Type = EN_STRING_DYNAMIC,
  [EN_global_id].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_global_id].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_global_id].Use = EN_OPTIONAL,

  [EN_global_name].Name.String = "name",
  [EN_global_name].Name.Length = sizeof("name") - 1,

  [EN_global_name].Target.Type = EN_RELATIVE,
  [EN_global_name].Target.Offset = offsetof(element_t, global.name),

  [EN_global_name].Content.Type = EN_STRING_DYNAMIC,
  [EN_global_name].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_global_name].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_global_name].Use = EN_REQUIRED,

  [EN_global_type].Name.String = "type",
  [EN_global_type].Name.Length = sizeof("type") - 1,

  [EN_global_type].Target.Type = EN_RELATIVE,
  [EN_global_type].Target.Offset = offsetof(element_t, global.type),

  [EN_global_type].Content.Type = EN_STRING_DYNAMIC,
  [EN_global_type].Content.Facet.String.MinLength = DEFAULT_MIN_STRING_LENGTH,
  [EN_global_type].Content.Facet.String.MaxLength = DEFAULT_MAX_STRING_LENGTH,

  [EN_global_type].Use = EN_OPTIONAL,
};

const xs_element_t xs_global_element =
{
  .Name.String  = "xs:element",
  .Name.Length  = sizeof("xs:element") - 1,
  .MinOccur      = 0,
  .MaxOccur      = 64,

  .Callback      = traverse_up,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_global_schema_element,

  .Attribute_Quantity = TOTAL_GLOBAL_ELEMENT_ATTRIBUTES,
  .Attribute = global_element_attr,

  .Child_Quantity = TOTAL_ELEMENT_DESCENDANTS,
  .Child_Order    = EN_CHOICE,
  .Child          = Element_Descendant,
};

const xs_element_t xs_child_element =
{
  .Name.String  = "xs:element",
  .Name.Length  = sizeof("xs:element") - 1,
  .MinOccur      = 0,
  .MaxOccur      = 64,

  .Callback      = traverse_up,

  .Target.Type  = EN_DYNAMIC,
  .Target.Allocate = allocate_child_schema_element,

  .Attribute_Quantity = TOTAL_CHILD_ELEMENT_ATTRIBUTES,
  .Attribute = child_element_attr,

  .Child_Quantity = TOTAL_ELEMENT_DESCENDANTS,
  .Child_Order    = EN_CHOICE,
  .Child          = Element_Descendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_global_schema_element(uint32_t occurrence, void* context)
{
  return allocate_element_type(context, sizeof(element_t), XS_GLOBAL_ELEMENT_TAG);
}

void* allocate_child_schema_element(uint32_t occurrence, void* context)
{
  element_t* element = allocate_element_type(context, sizeof(element_t), XS_CHILD_ELEMENT_TAG);
  element->child.minOccurs = 1;
  return element;
}

