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
#include "apps/xs_data_type.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

#define ADD_TAG(TAG, NAME)  #NAME,
const char* const XsdTag[TOTAL_XSD_TAGS] = {ALL_XSD_TAG};
#undef ADD_TAG

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static void parse_element(const tree_t* node, xs_element_t* const element, context_t* context);

void add_attribute_node(attribute_list_t* list, const xs_attribute_t* attribute)
{
  if(list->Attribute)
  {
    list = (attribute_list_t*)add_list_node((list_t*)list, sizeof(attribute_list_t));
  }
  list->Attribute = attribute;
}

void add_element_node(element_list_t* list, const xs_element_t* element)
{
  if(list->Element)
  {
    list = (element_list_t*)add_list_node((list_t*)list, sizeof(element_list_t));
  }
  list->Element = element;
}

void add_reference_node(reference_list_t* list, void* element,
                                     const string_t* const reference)
{
  // TODO: instead of traversing from head to last node, use head and tail pointers
  // to linked list node.
  if(list->Element)
  {
    list = (reference_list_t*)add_list_node((list_t*)list, sizeof(reference_list_t));
  }

  list->Element = element;
  list->Reference.Length = strlen(reference->String);
  list->Reference.String = reference->String;
}


attribute_list_t* search_attribute_node(attribute_list_t* list, const string_t* const reference)
{
  while(list)
  {
    const string_t* const name = &list->Attribute->Name;
    if((name->Length == reference->Length) &&
       (!memcmp(name->String, reference->String, name->Length)))
    {
      return list;
    }
    list = (attribute_list_t*)list->List.Next;
  }
  return NULL;
}

element_list_t* search_element_node(element_list_t* list, const string_t* const reference)
{
  while(list)
  {
    const string_t* const name = &list->Element->Name;
    if((name->Length == reference->Length) &&
       (!memcmp(name->String, reference->String, name->Length)))
    {
      return list;
    }
    list = (element_list_t*)list->List.Next;
  }
  return NULL;
}

const tree_t* find_tag(const tree_t* tree, const string_t* const name, const xsd_tag_t tag)
{
  do
  {
    if((*(const xsd_tag_t*)tree->Data) == XS_COMPLEX_TAG)
    {
      return tree;
    }
    tree = tree->Next;
  }while(tree);
  return NULL;
}

xs_element_t* parse_common_element_attr(const tree_t* const node,
                                        xs_element_t* const element,
                                        const common_element_attribute_t* const attr)
{
  element->Name.Length = strlen(attr->name.String);
  element->Name.String = attr->name.String;

  if(attr->type.String)
  {
    element->Content.Type = TOTAL_XSD_DATA_TYPE;
    size_t type_length = strlen(attr->type.String);
    for(uint32_t i = 0; i < TOTAL_XSD_DATA_TYPE; i++)
    {
      if((type_length == strlen(xs_data_type[i])) &&
         (!memcmp(attr->type.String, xs_data_type[i], type_length)))
      {
        convert_xsd_data_type(&element->Content, i);
        element->Target.Type = EN_RELATIVE;
        break;
      }
    }

    if(element->Content.Type == TOTAL_XSD_DATA_TYPE)
    {
      const tree_t* const complexType = find_tag(node, &attr->type, XS_COMPLEX_TAG);
      if(complexType != NULL)
      {
        return element;
      }

      const tree_t* const simpleType = find_tag(node, &attr->type, XS_SIMPLE_TYPE_TAG);
      if(simpleType == NULL)
      {
       return NULL;
      }
    }
  }
  return element;
}

void parse_child_element(const tree_t* node, xs_element_t* const element, context_t* context)
{
  element_t* source = node->Data;

  element->MinOccur = source->child.minOccurs;
  element->MaxOccur = source->child.maxOccurs;

  if(source->child.ref.String)
  {
    source->child.ref.Length = strlen(source->child.ref.String);
    add_reference_node(&context->Element_List, element, &source->child.ref);
    return;
  }
  parse_element(node, element, context);

}

xs_element_t* parse_sequence(const tree_t* sequence, xs_element_t* const element,
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
  const xs_element_t** child_elements = malloc(sizeof(xs_element_t*) * quantity);
  element->Child = child_elements;
  node = sequence->Descendant;
  uint32_t index = 0;
  while(node)
  {
    child_elements[index] = calloc(sizeof(xs_element_t), 1);
    parse_child_element(node, child_elements[index++], context);
    node = node->Next;
  }
  return element;
}

xs_attribute_use_t parse_attribute_use(const string_t* use)
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

void parse_attribute(const tree_t* node, xs_attribute_t* const attribute,
                     attribute_list_t* attr_list)
{
  attribute_t* source = node->Data;
  xs_attribute_use_t use = EN_OPTIONAL;
  if(source->attr.use.String)
  {
    source->attr.use.Length = strlen(source->attr.use.String);
    use = parse_attribute_use(&source->attr.use);
  }

  if(source->attr.ref.String)
  {
    source->attr.ref.Length = strlen(source->attr.ref.String);
    attr_list = search_attribute_node(attr_list, &source->attr.ref);
    memcpy(attribute, attr_list->Attribute, sizeof(xs_attribute_t));
    attribute->Use = use;
    return;
  }

  attribute->Use = use;
  attribute->Name.Length = strlen(source->attr.name.String);
  attribute->Name.String = source->attr.name.String;

  if(source->attr.type.String)
  {
    attribute->Content.Type = TOTAL_XSD_DATA_TYPE;
    size_t type_length = strlen(source->attr.type.String);
    for(uint32_t i = 0; i < TOTAL_XSD_DATA_TYPE; i++)
    {
      if((type_length == strlen(xs_data_type[i])) &&
         (!memcmp(source->attr.type.String, xs_data_type[i], type_length)))
      {
        convert_xsd_data_type(&attribute->Content, i);
        attribute->Target.Type = EN_RELATIVE;
        break;
      }
    }

    if(attribute->Content.Type == TOTAL_XSD_DATA_TYPE)
    {
      const tree_t* const simpleType = find_tag(node, &source->attr.type, XS_SIMPLE_TYPE_TAG);
      if(simpleType == NULL)
      {
        return;
      }
    }
  }
}

void parse_complex_element(const tree_t* complexType,
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
       parse_attribute(node, &attributes[attr_index++], &context->Attribute_List);
      break;

    default:
      return;
    }
    node = node->Next;
  }
}

static void parse_element(const tree_t* node, xs_element_t* const element, context_t* context)
{
  const element_t* source = node->Data;

  parse_common_element_attr(node, element, &source->global);

  node = node->Descendant;
  if(node)
  {
    const xsd_tag_t* const tag = node->Data;
    printf("%s\t", XsdTag[*tag]);

    switch(*tag)
    {
    case XS_COMPLEX_TAG:
      parse_complex_element(node, element, context);
      return;

    case XS_SIMPLE_TYPE_TAG:
      break;

    default:
      return;
    }
  }
}

const element_t* find_global_element(const tree_t* tree, const string_t* const name)
{
  do
  {
    if(*((const xsd_tag_t*)tree->Data) == XS_GLOBAL_ELEMENT_TAG)
    {
      const string_t* const elementName = &((const element_t* const)tree->Data)->global.name;
      if((elementName->Length == name->Length) && (elementName->String == name->String))
      {
        return ((const element_t*)tree->Data);
      }
    }
    tree = tree->Next;
  }while(tree);

  return NULL;
}

void resolve_element_references(element_list_t* const element_list,
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

xs_element_t* get_root(element_list_t* list)
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

xs_element_t* parse_simple_type(const tree_t* node, xs_element_t* element)
{
  return NULL;
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
    printf("%s\t", XsdTag[*tag]);

    switch(*tag)
    {
    case XS_COMPLEX_TAG:
      printf("Name: %s\n", ((const complexType_t* const)tag)->attr.name.String);
      break;

    case XS_ATTRIBUTE_TAG:
      printf("Name: %s\n", ((const attribute_t* const)tag)->attr.name.String);
      xs_attribute_t* attribute = calloc(sizeof(xs_attribute_t), 1);
      parse_attribute(node, attribute, &context.Attribute_List);
      add_attribute_node(&context.Attribute_List, attribute);
      break;

    case XS_SIMPLE_TYPE_TAG:
      printf("Name: %s\n", ((const simple_type_t* const)tag)->attr.name.String);
      break;

    default:
      printf("\n");
      break;
    }
    node = node->Next;
  }

  node = tree;
  while(node)
  {
    const xsd_tag_t* const tag = node->Data;
    printf("%s\t", XsdTag[*tag]);

    switch(*tag)
    {
    case XS_GLOBAL_ELEMENT_TAG:
      printf("Name: %s\n",((const element_t* const)tag)->global.name.String);
      xs_element_t* element = calloc(sizeof(xs_element_t), 1);
      parse_element(node, element, &context);
      add_element_node(&element_list, element);
      break;

    default:
      printf("\n");
      break;
    }
    node = node->Next;
  }

  if(context.Element_List.Element)
  {
    resolve_element_references(&element_list, &context.Element_List);
  }

  xs_element_t* root = get_root(&element_list);
  return root;
}

void print_xsd(const tree_t* const tree, uint32_t level)
{
  for(uint32_t i = 0; i < level; i++)
  {
    printf("\t");
  }

  const xsd_tag_t* const tag = tree->Data;
  printf("%s\t", XsdTag[*tag]);

  switch(*tag)
  {
  case XS_GLOBAL_ELEMENT_TAG:
    printf("Name: %s\n",((const element_t* const)tag)->global.name.String);
    break;

  case XS_CHILD_ELEMENT_TAG:
    printf("Name: %s\n",((const element_t* const)tag)->child.name.String);
    break;

  case XS_COMPLEX_TAG:
    printf("Name: %s\n", ((const complexType_t* const)tag)->attr.name.String);
    break;

  case XS_ATTRIBUTE_TAG:
    printf("Name: %s\n", ((const attribute_t* const)tag)->attr.name.String);
    break;

  case XS_SIMPLE_TYPE_TAG:
    printf("Name: %s\n", ((const simple_type_t* const)tag)->attr.name.String);
    break;

  default:
    printf("\n");
    break;
  }

  if(tree->Descendant)
  {
    print_xsd(tree->Descendant, level + 1);
  }

  if(tree->Next)
  {
    print_xsd(tree->Next, level);
  }
}
