/**
 * \file
 * \brief   Contains all the XML schema supported tags

 * \author  Nandkishor Biradar
 * \date    12 March 2020
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "libs/parse_xml.h"

#include "apps/xsd.h"
#include "elements/xs_element.h"
#include "elements/xs_complex_type.h"
#include "elements/xs_attribute.h"
#include "elements/xs_simple_type.h"
#include "elements/xs_restriction.h"
#include "apps/xs_data_type.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */
static inline void parse_child_element(const tree_t* node, xs_element_t* const element, context_t* context);

static inline xs_element_t* parse_sequence(const tree_t* sequence, xs_element_t* const element,
                             context_t* context)
{
  const tree_t* node = sequence->Descendant;

  // find the child element quantity
  uint32_t quantity = 0;
  while(node)
  {
    if((*(xsd_tag_t*)node->Data) != XS_CHILD_ELEMENT_TAG)
    {
      return NULL;
    }
    quantity++;
    node = node->Next;
  }
  element->Child_Quantity = quantity;
  const xs_element_t* child_elements = calloc(sizeof(xs_element_t), quantity);
  element->Child = child_elements;
  node = sequence->Descendant;
  uint32_t index = 0;
  while(node)
  {
    parse_child_element(node, &child_elements[index++], context);
    node = node->Next;
  }
  return element;
}

static inline xs_attribute_use_t parse_attribute_use(const string_t* use)
{
  for(uint32_t i = 0; i < TOTAL_XSD_ATTRIBUTE_USE_VALUES; i++)
  {
    if((use->Length == sizeof(xs_attribute_use_type[i].Length)) &&
       (!memcmp(use->String, xs_attribute_use_type[i].String, use->Length)))
    {
        return i;
    }
  }
  return TOTAL_XSD_ATTRIBUTE_USE_VALUES;
}

static inline void get_xml_type(const string_t* const type,
                                xml_content_t* const content)
{
    content->Type = EN_NO_XML_DATA_TYPE;
    for(uint32_t i = 0; i < TOTAL_XSD_DATA_TYPE; i++)
    {
      if((type->Length == xs_data_type[i].Length) &&
         (!memcmp(type->String, xs_data_type[i].String, type->Length)))
      {
        convert_xsd_data_type(content, i);
        return;
      }
    }
}

static inline void parse_attribute(const tree_t* node, xs_attribute_t* const attribute,
                     context_t* context)
{
  attribute_t* source = node->Data;
  xs_attribute_use_t use = EN_OPTIONAL;
  if(source->attr.use.String)
  {
    source->attr.use.Length = strlen(source->attr.use.String);
    use = parse_attribute_use(&source->attr.use);
  }

  string_t* const ref = &source->attr.ref;
  if(ref->String)
  {
    ref->Length = strlen(ref->String);
    const attribute_list_t* const attr_list =
                          search_attribute_node(&context->Attribute_List, ref);
    memcpy(attribute, attr_list->Attribute, sizeof(xs_attribute_t));
    attribute->Use = use;
    return;
  }

  attribute->Use = use;
  attribute->Name.Length = strlen(source->attr.name.String);
  attribute->Name.String = source->attr.name.String;
  attribute->Target.Type = EN_RELATIVE;

  string_t* const type = &source->attr.type;
  if(type->String)
  {
    type->Length = strlen(type->String);
    get_xml_type(type, &attribute->Content);

    if(attribute->Content.Type == EN_NO_XML_DATA_TYPE)
    {
      const element_list_t* simpleType_List = search_element_node(&context->SimpleType_List, type);
      if(simpleType_List)
      {
        const xs_element_t* const element = simpleType_List->Element;
        memcpy(&attribute->Content, &element->Content, sizeof(xml_content_t));
      }
    }
  }
}

static inline void parse_complex_element(const tree_t* complexType,
                                    xs_element_t* const element,
                                    context_t* context)
{
  const tree_t* node = complexType->Descendant;
  // Look for attributes
  uint32_t quantity = 0;
  while(node)
  {
    if(*((xsd_tag_t*)node->Data) == XS_ATTRIBUTE_TAG)
    {
      quantity++;
    }
    node = node->Next;
  }

  xs_attribute_t* attributes;
  if(quantity)
  {
    element->Attribute_Quantity = quantity;
    attributes = calloc(sizeof(xs_attribute_t), quantity);
    element->Attribute = attributes;
  }

  uint32_t attr_index = 0;

  node = complexType->Descendant;
  while(node)
  {
    const xsd_tag_t* const tag = node->Data;
    switch(*tag)
    {
    case XS_SEQUENCE_TAG:
      element->Child_Type = EN_SEQUENCE;
      parse_sequence(complexType->Descendant, element, context);
      break;

    case XS_SIMPLE_CONTENT_TAG:
      break;

    case XS_ATTRIBUTE_TAG:
       parse_attribute(node, &attributes[attr_index++], context);
      break;

    default:
      assert(false);
      return;
    }
    node = node->Next;
  }
}

static inline void parse_complex_type(const tree_t* tree, xs_element_t* const element,
                        context_t* context)
{
  complexType_t* complexType = tree->Data;
  assert(complexType->attr.name.String);

  element->Name.Length = strlen(complexType->attr.name.String);
  element->Name.String = complexType->attr.name.String;

  parse_complex_element(tree, element, context);
  add_element_node(&context->ComplexType_List, element);
}

static inline void parse_simple_element(const tree_t* tree, xs_element_t* element)
{
  const tree_t* node = tree->Descendant;
  const restriction_t* const restriction = node->Data;
  assert(restriction->Type == XS_RESTRICTION_TAG);

  element->Content.Type = EN_NO_XML_DATA_TYPE;
  const char* const type = restriction->attr.base.String;
  size_t type_length = strlen(type);

  for(uint32_t i = 0; i < TOTAL_XSD_DATA_TYPE; i++)
  {
    if((type_length == xs_data_type[i].Length) &&
       (!memcmp(type, xs_data_type[i].String, type_length)))
    {
      convert_xsd_data_type(&element->Content, i);
      return;
    }
  }
    // TODO: support restriction facets

  return;
}

static inline void parse_simple_type(const tree_t* tree, xs_element_t* const element,
                       const element_list_t* const simpleType_List)
{
  simpleType_t* simpleType = tree->Data;
  const char* const name = simpleType->attr.name.String;
  assert(name);

  element->Name.Length = strlen(name);
  element->Name.String = name;

  parse_simple_element(tree, element);
  add_element_node(simpleType_List, element);
}

static inline void parse_element(const tree_t* node, xs_element_t* const element,
                                 context_t* context)
{
  const element_t* source = node->Data;

  element->Name.Length = strlen(source->global.name.String);
  element->Name.String = source->global.name.String;
  element->Target.Type = EN_RELATIVE;

  string_t* const type = &source->global.type;
  if(type->String)
  {
    type->Length = strlen(type->String);
    get_xml_type(type, &element->Content);

    if(element->Content.Type == EN_NO_XML_DATA_TYPE)
    {
      element_list_t* list = search_element_node(&context->ComplexType_List, type);
      if(list)
      {
        xs_element_t* complexType = list->Element;
        element->Attribute = complexType->Attribute;
        element->Attribute_Quantity = complexType->Attribute_Quantity;
        element->Child = complexType->Child;
        element->Child_Quantity = complexType->Child_Quantity;
        element->Child_Type = complexType->Child_Type;
      }
      else
      {
        const element_list_t* simpleType_List = search_element_node(&context->SimpleType_List, type);
        if(simpleType_List)
        {
          const xs_element_t* const simpleType = simpleType_List->Element;
          memcpy(&element->Content, &simpleType->Content, sizeof(xml_content_t));
        }
      }
    }
  }

  node = node->Descendant;
  if(node)
  {
    const xsd_tag_t* const tag = node->Data;
    switch(*tag)
    {
    case XS_COMPLEX_TAG:
      parse_complex_element(node, element, context);
      return;

    case XS_SIMPLE_TYPE_TAG:
      parse_simple_element(node, element);
      break;

    default:
      return;
    }
  }
}

static inline void parse_child_element(const tree_t* node, xs_element_t* const element, context_t* context)
{
  element_t* source = node->Data;

  element->MinOccur = source->child.minOccurs;
  uint32_t maxOccurs = 1;

  if(source->child.maxOccurs.String)
  {
    if((source->child.maxOccurs.Length == (sizeof("unbounded") - 1)) &&
       (!memcmp(source->child.maxOccurs.String, "unbounded", sizeof("unbounded") - 1)))
    {
      maxOccurs = UINT32_MAX;
    }
    else
    {
      maxOccurs = strtoul(source->child.maxOccurs.String, NULL, 10);
    }
  }
  element->MaxOccur = maxOccurs;

  if(source->child.ref.String)
  {
    source->child.ref.Length = strlen(source->child.ref.String);
    add_reference_node(&context->Element_List, element, &source->child.ref);
    return;
  }
  parse_element(node, element, context);

}

static inline void resolve_element_references(element_list_t* const element_list,
                                reference_list_t* reference_list)
{
  while(reference_list)
  {
    element_list_t* node = search_element_node(element_list, &reference_list->Reference);
    assert(node != NULL);
    node->Ref_Count++;
    xs_element_t* target = reference_list->Element;

    uint32_t minOccurs = target->MinOccur;
    uint32_t maxOccurs = target->MaxOccur;

    memcpy(target, node->Element, sizeof(xs_element_t));
    target->MinOccur = minOccurs;
    target->MaxOccur = maxOccurs;
    reference_list = (reference_list_t*)reference_list->List.Next;
  }
}

static inline xs_element_t* get_root(const element_list_t* list)
{
  xs_element_t* root = NULL;
  while(list)
  {
    if(list->Ref_Count == 0)
    {
      if(root != NULL)
      {
        return NULL;
      }
      root = list->Element;
    }
    list = (element_list_t*)list->List.Next;
  }
  return root;
}

static inline xs_element_t* create_root(const element_list_t* list)
{
  xs_element_t* root = calloc(sizeof(xs_element_t), 1);
  root->Child_Quantity = 1;
  root->Child_Type = EN_CHOICE;

  xs_element_t* const element = get_root(list);
  root->Child = element;
  element->Target.Type = EN_STATIC;
  element->MinOccur = 1;
  element->MaxOccur = 1;

  xs_attribute_t* const attributes = calloc(sizeof(xs_attribute_t), 2 + element->Attribute_Quantity);

  attributes[0].Name.String = "xmlns:xsi";
  attributes[0].Name.Length = sizeof("xmlns:xsi") - 1;

  attributes[1].Name.String = "xsi:noNamespaceSchemaLocation";
  attributes[1].Name.Length = sizeof("xsi:noNamespaceSchemaLocation") - 1;

  if(element->Attribute_Quantity)
  {
    memcpy(&attributes[2], element->Attribute,
              sizeof(xs_attribute_t) * element->Attribute_Quantity);
  }
  element->Attribute_Quantity += 2;
  free(element->Attribute);
  element->Attribute = attributes;

  return root;
}

xs_element_t* compile_xsd(const tree_t* const tree)
{
  element_list_t element_list;
  context_t context;

  memset(&element_list, 0, sizeof(element_list_t));
  memset(&context, 0, sizeof(context_t));

  const tree_t* node = tree;
  while(node)
  {
    const xsd_tag_t* const tag = node->Data;
    switch(*tag)
    {
    case XS_COMPLEX_TAG:
    {
      xs_element_t* complexType = calloc(sizeof(xs_element_t), 1);
      parse_complex_type(node, complexType, &context);
      break;
    }

    case XS_ATTRIBUTE_TAG:
    {
      xs_attribute_t* attribute = calloc(sizeof(xs_attribute_t), 1);
      parse_attribute(node, attribute, &context);
      add_attribute_node(&context.Attribute_List, attribute);
      break;
    }

    case XS_SIMPLE_TYPE_TAG:
    {
      xs_element_t* simpleType = calloc(sizeof(xs_element_t), 1);
      parse_simple_type(node, simpleType, &context.SimpleType_List);
      break;
    }

    default:
      break;
    }
    node = node->Next;
  }

  node = tree;
  while(node)
  {
    const xsd_tag_t* const tag = node->Data;
    if(*tag == XS_GLOBAL_ELEMENT_TAG)
    {
      xs_element_t* element = calloc(sizeof(xs_element_t), 1);
      parse_element(node, element, &context);
      add_element_node(&element_list, element);
    }
    node = node->Next;
  }

  if(context.Element_List.Element)
  {
    resolve_element_references(&element_list, &context.Element_List);
  }

  return create_root(&element_list);
}
