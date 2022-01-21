/**
 * \file
 * \brief   Implements the root element "xs:schema" tag

 * \author  Nandkishor Biradar
 * \date    19 June 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "apps/tree.h"

#include "parse_xml.h"

#include "apps/xsd.h"
#include "xs_element.h"
#include "xs_complex_type.h"
#include "xs_attribute.h"
#include "xs_simple_type.h"
#include "xs_schema.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */


static const xs_element_t schemaDescendant[TOTAL_SCHEMA_DESCENDANTS] =
{
  [EN_schema_element].Name.String  = "xs:element",
  [EN_schema_element].Name.Length  = sizeof("xs:element") - 1,
  [EN_schema_element].MinOccur      = 0,
  [EN_schema_element].MaxOccur      = 64,
  [EN_schema_element].Callback      = traverse_up,
  [EN_schema_element].Target.Type  = EN_DYNAMIC,
  [EN_schema_element].Target.Allocate = allocate_global_schema_element,
  [EN_schema_element].Attribute_Quantity = TOTAL_GLOBAL_ELEMENT_ATTRIBUTES,
  [EN_schema_element].Attribute = global_element_attr,
  [EN_schema_element].Child_Quantity = TOTAL_ELEMENT_DESCENDANTS,
  [EN_schema_element].Child_Order     = EN_CHOICE,
  [EN_schema_element].Child          = Element_Descendant,

  [EN_schema_complexType].Name.String  = "xs:complexType",
  [EN_schema_complexType].Name.Length  = sizeof("xs:complexType") - 1,
  [EN_schema_complexType].MinOccur      = 0,
  [EN_schema_complexType].MaxOccur      = 64,
  [EN_schema_complexType].Callback      = traverse_up,
  [EN_schema_complexType].Target.Type  = EN_DYNAMIC,
  [EN_schema_complexType].Target.Allocate = allocate_complex_type,
  [EN_schema_complexType].Target.Size   = sizeof(complexType_t),
  [EN_schema_complexType].Attribute_Quantity = TOTAL_TYPE_ATTRIBUTES,
  [EN_schema_complexType].Attribute = complexType_Attr,
  [EN_schema_complexType].Child_Quantity = TOTAL_COMPLEX_TYPE_DESCENDANT,
  [EN_schema_complexType].Child_Order     = EN_RANDOM,
  [EN_schema_complexType].Child = ComplexType_Descendant,

  [EN_schema_attribute].Name.String = "xs:attribute",
  [EN_schema_attribute].Name.Length    = sizeof("xs:attribute") - 1,
  [EN_schema_attribute].MinOccur    = 0,
  [EN_schema_attribute].MaxOccur    = 64,
  [EN_schema_attribute].Callback      = traverse_up,
  [EN_schema_attribute].Target.Type  = EN_DYNAMIC,
  [EN_schema_attribute].Target.Allocate = allocate_attribute,
  [EN_schema_attribute].Target.Size = sizeof(attribute_t),
  [EN_schema_attribute].Attribute_Quantity = TOTAL_ATTRIBUTE_ATTRIBUTES,
  [EN_schema_attribute].Attribute = attribute_Attr,
  [EN_schema_attribute].Child_Quantity = TOTAL_ATTRIBUTE_DESCENDANTS,
  [EN_schema_attribute].Child_Order     = EN_CHOICE,
  [EN_schema_attribute].Child = attribute_Descendant,

  [EN_schema_simpleType].Name.String  = "xs:simpleType",
  [EN_schema_simpleType].Name.Length  = sizeof("xs:simpleType") - 1,
  [EN_schema_simpleType].MinOccur     = 0,
  [EN_schema_simpleType].MaxOccur     = 64,
  [EN_schema_simpleType].Callback      = traverse_up,
  [EN_schema_simpleType].Target.Type  = EN_DYNAMIC,
  [EN_schema_simpleType].Target.Allocate = allocate_simple_type,
  [EN_schema_simpleType].Attribute_Quantity = TOTAL_TYPE_ATTRIBUTES,
  [EN_schema_simpleType].Attribute = simple_type_Attr,
  [EN_schema_simpleType].Child_Quantity = TOTAL_SIMPLE_TYPE_DESCENDANTS,
  [EN_schema_simpleType].Child_Order     = EN_CHOICE,
  [EN_schema_simpleType].Child = simple_type_Descendant,

};

static const xs_attribute_t schema_attr[] =
{
  [0].Name.String = "xmlns:xsi",
  [0].Name.Length = sizeof("xmlns:xsi") - 1,

  [1].Name.String = "xmlns:xs",
  [1].Name.Length = sizeof("xmlns:xs") - 1,

  [2].Name.String = "xmlns:hfp",
  [2].Name.Length = sizeof("xmlns:hfp") - 1,
};

const xs_element_t xsd_root =
{
  .Name.String = "xs:schema",
  .Name.Length = sizeof("xs:schema") - 1,
  .MinOccur    = 1,
  .MaxOccur    = 1,
  .Callback    = NULL,

  .Target.Type   = EN_RELATIVE,
  .Target.Offset = 0,

  .Attribute_Quantity = sizeof(schema_attr) / sizeof(xs_attribute_t),
  .Attribute          = schema_attr,
  .Ignore_Attribute   = true,

  .Child_Quantity = TOTAL_SCHEMA_DESCENDANTS,
  .Child_Order    = EN_RANDOM,
  .Child          = schemaDescendant,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* allocate_element_type(tree_t** schemaTree, size_t size, xsd_tag_t type)
{
  xsd_tag_t* elementType = calloc(1, size);
  *elementType = type;
  tree_t* node = create_node(elementType);
  add_descendant_node(*schemaTree, node);
  *schemaTree = node;
  return elementType;
}
