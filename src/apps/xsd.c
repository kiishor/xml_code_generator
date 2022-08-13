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

#include "parse_xml.h"

#include "apps/xsd.h"
#include "apps/facet.h"
#include "elements/xs_element.h"
#include "elements/xs_complex_type.h"
#include "elements/xs_attribute.h"
#include "elements/xs_simple_type.h"
#include "elements/xs_restriction.h"
#include "elements/xs_extension.h"
#include "elements/xs_simple_content.h"
#include "elements/xs_facet.h"
#include "apps/xs_data_type.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */
static inline void parse_child_element(const tree_t* node, xs_element_t* const element,
                                       const context_t* const context);

static inline xs_element_t* parse_sequence(const tree_t* const sequence, xs_element_t* const element,
                                           const context_t* const context)
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
  xs_element_t* child_elements = calloc(sizeof(xs_element_t), quantity);
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

static inline xs_attribute_use_t parse_attribute_use(const string_t* const use)
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

/** \brief Assigns the content type based on the schema type name.

 * This function checks whether the give type is basic XML type (xs:string, xs:decimal, ..).
 * Function returns false if given type is not a basic XML type.
 *
 * \param content xml_content_t* const Content of an element/atrrtibute to set type
 * \param type const string_t* const Type of an element/attribute in string
 * \param context const context_t* const Context of compiler
 * \return bool Returns true if given type is found in the list of supported basic types. Otherwise returns false.
 *
 */
static inline bool parse_base_type(xml_content_t* const content, const string_t* const type,
                                   const context_t* const context)
{
  content->Type = EN_NO_XML_DATA_TYPE;
  for(uint32_t i = 0; i < TOTAL_XSD_DATA_TYPE; i++)
  {
    if((type->Length == xs_data_type[i].Length) &&
       (!memcmp(type->String, xs_data_type[i].String, type->Length)))
    {
      convert_xsd_data_type(content, i);
      return true;
    }
  }
  return false;
}

static inline void parse_attribute(const tree_t* const node, xs_attribute_t* const attribute,
                                   const context_t* const context)
{
  attribute_t* source = node->Data;
  xs_attribute_use_t use = EN_OPTIONAL;
  if(source->attr.use.String)
  {
    source->attr.use.Length = strlen(source->attr.use.String);
    use = parse_attribute_use(&source->attr.use);
  }
  attribute->Use = use;

  string_t* const ref = &source->attr.ref;
  if(ref->String)
  {
    ref->Length = strlen(ref->String);
    add_unresolved_tag((unresolved_tag_t*)&context->Attr_Ref_List, attribute, ref);
    return;
  }

  attribute->Name.Length = strlen(source->attr.name.String);
  attribute->Name.String = source->attr.name.String;
  attribute->Target.Type = EN_RELATIVE;

  string_t* const type = &source->attr.type;
  if(type->String)
  {
    type->Length = strlen(type->String);
    // Parse element type.
    if(!parse_base_type(&attribute->Content, type, context))
    {
      // Could not found the given type. It may be simple type.
      add_unresolved_tag((unresolved_tag_t*)&context->Attr_Type_List, attribute, type);
    }
  }
}

static inline void set_attribute(const tree_t* node,
                                 xs_element_t* const element)
{
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
}

// This function is also used for parsing extension also.
// restriction and extension are quite similar except restriction has more
// descendant present than extension.
// This program assumes the schema is valid and extension does not contains
// the descendants that are not supposed to be part of it.
static inline void parse_restriction(const tree_t* const tree,
                                     xs_element_t* const element,
                                     const context_t* const context,
                                     en_restriction_parent parent)
{
  restriction_t* const restriction = tree->Data;
  string_t* const type = &restriction->attr.base;
  type->Length = strlen(type->String);

  if(!parse_base_type(&element->Content, type, context))
  {
    // Could not found the given type. It may be simple type or complex type.
    add_unresolved_tag((unresolved_tag_t*)&context->Type_List, element, type);
  }

  if(parent == SIMPLE_CONTENT_PARENT)
  {
    set_attribute(tree->Descendant, element);
  }

  uint32_t attr_index = 0;
  const tree_t* node = tree->Descendant;
  while(node)
  {
    const xsd_tag_t* const tag = node->Data;
    switch(*tag)
    {
    case XS_ATTRIBUTE_TAG:
       parse_attribute(node, (xs_attribute_t*)&element->Attribute[attr_index++], context);
      break;

    case XS_LENGTH_TAG:
    {
      const xs_facet_t* const facet = (xs_facet_t*)tag;
      uint32_t length = strtoul(facet->attr.value.String, NULL, 10);
      element->Content.Facet.String.MinLength = length;
      element->Content.Facet.String.MaxLength = length;
      break;
    }

    case XS_MAXLENGTH_TAG:
    {
      const xs_facet_t* const facet = (xs_facet_t*)tag;
      uint32_t length = strtoul(facet->attr.value.String, NULL, 10);
      element->Content.Facet.String.MaxLength = length;
      break;
    }

    case XS_MINLENGTH_TAG:
    {
      const xs_facet_t* const facet = (xs_facet_t*)tag;
      uint32_t length = strtoul(facet->attr.value.String, NULL, 10);
      element->Content.Facet.String.MinLength = length;
      break;
    }

    case XS_MAXEXCLUSIVE_TAG:
    {
      const xs_facet_t* const facet = (xs_facet_t*)tag;
      const string_t* const value = &facet->attr.value;
      void* target = get_facet_max_target(&element->Content);
      extract_content(&element->Content, target, value->String, value->Length);
      exclude_facet_max_value(&element->Content);
      break;
    }

    case XS_MAXINCLUSIVE_TAG:
    {
      const xs_facet_t* const facet = (xs_facet_t*)tag;
      const string_t* const value = &facet->attr.value;
      void* target = get_facet_max_target(&element->Content);
      extract_content(&element->Content, target, value->String, value->Length);
      break;
    }

    case XS_MINEXCLUSIVE_TAG:
    {
      const xs_facet_t* const facet = (xs_facet_t*)tag;
      const string_t* const value = &facet->attr.value;
      void* target = get_facet_min_target(&element->Content);
      extract_content(&element->Content, target, value->String, value->Length);
      exclude_facet_min_value(&element->Content);
      break;
    }

    case XS_MININCLUSIVE_TAG:
    {
      const xs_facet_t* const facet = (xs_facet_t*)tag;
      const string_t* const value = &facet->attr.value;
      void* target = get_facet_min_target(&element->Content);
      extract_content(&element->Content, target, value->String, value->Length);
      break;
    }

    default:
//      assert(false);
      break;
    }
    node = node->Next;
  }
}

static inline void parse_simple_content(const tree_t* const tree,
                                        xs_element_t* const element,
                                        const context_t* const context)
{
  const tree_t* const node = tree->Descendant;
  const xsd_tag_t* const tag = node->Data;
  switch(*tag)
  {
  case XS_RESTRICTION_TAG:
  case XS_EXTENSION_TAG:
    parse_restriction(node, element, context, SIMPLE_CONTENT_PARENT);
    break;

  default:
    break;
  }
}

static inline void parse_complex_element(const tree_t* const tree,
                                    xs_element_t* const element,
                                    const context_t* const context)
{
  set_attribute(tree->Descendant, element);

  const tree_t* node = tree->Descendant;
  uint32_t attr_index = 0;

  while(node)
  {
    const xsd_tag_t* const tag = node->Data;
    switch(*tag)
    {
    case XS_SEQUENCE_TAG:
      element->Child_Order = EN_SEQUENCE;
      parse_sequence(node, element, context);
      break;

    case XS_CHOICE_TAG:
      element->Child_Order = EN_CHOICE;
      parse_sequence(node, element, context);
      break;

    case XS_SIMPLE_CONTENT_TAG:
      parse_simple_content(node, element, context);
      break;

    case XS_ATTRIBUTE_TAG:
       parse_attribute(node, (xs_attribute_t*)&element->Attribute[attr_index++], context);
      break;

    default:
      assert(false);
      return;
    }
    node = node->Next;
  }
}

static inline void parse_complex_type(const tree_t* const tree, xs_element_t* const element,
                                      context_t* const context)
{
  complexType_t* complexType = tree->Data;
  assert(complexType->attr.name.String);

  element->Name.Length = strlen(complexType->attr.name.String);
  element->Name.String = complexType->attr.name.String;

  parse_complex_element(tree, element, context);
  add_node((linked_list_t*)&context->ComplexType_List, element);
}

// xs:simpleType
static inline void parse_simple_element(const tree_t* const tree,
                                        xs_element_t* const element,
                                        const context_t* const context)
{
  const tree_t* node = tree->Descendant;
  assert((*(xsd_tag_t*)node->Data) == XS_RESTRICTION_TAG);

  parse_restriction(node, element, context, SIMPLE_TYPE_PARENT);
  return;
}

static inline void parse_simple_type(const tree_t* const tree, xs_element_t* const element,
                                     context_t* const context)
{
  const simpleType_t* const simpleType = tree->Data;
  const char* const name = simpleType->attr.name.String;
  assert(name);

  element->Name.Length = strlen(name);
  element->Name.String = (char*)name;

  parse_simple_element(tree, element, context);
  add_node((linked_list_t*)&context->SimpleType_List, element);
}

static inline void parse_element(const tree_t* node, xs_element_t* const element,
                                 const context_t* const context)
{
  element_t* const source = node->Data;

  element->Name.Length = strlen(source->global.name.String);
  element->Name.String = source->global.name.String;

  string_t* const type = &source->global.type;
  if(type->String)
  {
    type->Length = strlen(type->String);
    // Parse element type.
    if(!parse_base_type(&element->Content, type, context))
    {
      // Could not found the given type. It may be simple type or complex type.
      add_unresolved_tag((unresolved_tag_t*)&context->Type_List, element, type);
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
      parse_simple_element(node, element, context);
      break;

    default:
      return;
    }
  }
}

static inline void set_target_type(xs_element_t* const element,
                                   en_occurrence_t occurrence)
{
  if((element->MaxOccur == 1) || (occurrence == ARRAY))
  {
    element->Target.Type = EN_RELATIVE;
    return;
  }

  element->Target.Type = EN_DYNAMIC;
}

static inline void parse_child_element(const tree_t* const node,
                                       xs_element_t* const element,
                                       const context_t* const context)
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
  set_target_type(element, context->Options->Occurrence);

  // If element has reference attribute, then add the element and its reference name
  // to the unresolved element list to resolve it later.
  if(source->child.ref.String)
  {
    source->child.ref.Length = strlen(source->child.ref.String);
    add_unresolved_tag((unresolved_tag_t*)&context->Ref_List, element, &source->child.ref);
    return;
  }
  parse_element(node, element, context);
}

static inline void resolve_element_types(unresolved_tag_t* element_list, const simpleType_list_t* simpleType_List,
                                         const complexType_list_t* complexType_List)
{
  while(element_list)
  {
    {
      const simpleType_list_t* const simpleType_Node =
            (simpleType_list_t*)search_node((linked_list_t*)simpleType_List, &element_list->Tag);
      if(simpleType_Node)
      {
        xs_element_t* const element = (xs_element_t*)element_list->Element;
        memcpy((void*)&element->Content, &simpleType_Node->Element->Content, sizeof(xml_content_t));
      }
    }

    const complexType_list_t* const complexType_Node =
              (complexType_list_t*)search_node((linked_list_t*)complexType_List, &element_list->Tag);
    if(complexType_Node)
    {
      xs_element_t* const element = (xs_element_t*)element_list->Element;
      const xs_element_t* const complexType = complexType_Node->Element;
      element->Attribute = complexType->Attribute;
      element->Attribute_Quantity = complexType->Attribute_Quantity;
      element->Child = complexType->Child;
      element->Child_Quantity = complexType->Child_Quantity;
      element->Child_Order = complexType->Child_Order;
      memcpy((void*)&element->Content, &complexType->Content, sizeof(xml_content_t));
    }

    element_list = (unresolved_tag_t*)element_list->List.Next;
  }
}

static inline void resolve_attribute_types(unresolved_tag_t* attribute_List, const simpleType_list_t* simpleType_List)
{
  while(attribute_List)
  {
    const simpleType_list_t* const simpleType_Node =
          (simpleType_list_t*)search_node((linked_list_t*)simpleType_List, &attribute_List->Tag);
    if(simpleType_Node)
    {
      xs_attribute_t* const attribute = (xs_attribute_t*)attribute_List->Attribute;
      memcpy((void*)&attribute->Content, &simpleType_Node->Element->Content, sizeof(xml_content_t));
    }
    attribute_List = (unresolved_tag_t*)attribute_List->List.Next;
  }
}

static inline void resolve_element_references(const global_element_list_t* const element_list,
                                const unresolved_tag_t* reference_list)
{
  while(reference_list)
  {
    // Search unresolved referred element into list of global elements
    global_element_list_t* const node =
          (global_element_list_t* )search_node((linked_list_t*)element_list, &reference_list->Tag);
    assert(node != NULL);

    node->Ref_Count++;  // This global element is referred by another element. Hence it can not be a root element.

    // Store element property of target element before copying from reference element.
    xs_element_t* const target = (xs_element_t* )reference_list->Element;
    uint32_t minOccurs = target->MinOccur;
    uint32_t maxOccurs = target->MaxOccur;
    target_address_t target_address;
    memcpy(&target_address, &target->Target, sizeof(target_address_t));

    // Copy all the properties of reference element to target element
    memcpy(target, node->Element, sizeof(xs_element_t));

    // Restore original properties of target element.
    target->MinOccur = minOccurs;
    target->MaxOccur = maxOccurs;
    memcpy(&target->Target, &target_address, sizeof(target_address_t));
    reference_list = (unresolved_tag_t*)reference_list->List.Next;
  }
}

static inline void resolve_attribute_references(const attribute_list_t* const attribute_list,
                                                unresolved_tag_t* reference_list)
{
  while(reference_list)
  {
    const attribute_list_t* const attr_list =
                    (attribute_list_t* )search_node((linked_list_t*)attribute_list, &reference_list->Tag);
    bool use = reference_list->Attribute->Use;
    memcpy((void*)reference_list->Attribute, attr_list->Attribute, sizeof(xs_attribute_t));
    ((xs_attribute_t*)(reference_list->Attribute))->Use = use;
    reference_list = (unresolved_tag_t* )reference_list->List.Next;
  }
}

static inline xs_element_t* get_root(const global_element_list_t* list)
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
      root = (xs_element_t*)list->Element;
    }
    list = (global_element_list_t*)list->List.Next;
  }
  return root;
}

static inline xs_element_t* create_root(const global_element_list_t* const list)
{
  xs_element_t* root = calloc(sizeof(xs_element_t), 1);
  root->Child_Quantity = 1;
  root->Child_Order = EN_CHOICE;

  xs_element_t* const element = get_root(list);
  root->Child = element;
  element->Target.Type = EN_RELATIVE;
  element->MinOccur = 1;
  element->MaxOccur = 1;

  xs_attribute_t* const attributes = calloc(sizeof(xs_attribute_t), 2 + element->Attribute_Quantity);

  attributes[0].Name.String = "xmlns:xsi";
  attributes[0].Name.Length = sizeof("xmlns:xsi") - 1;

  attributes[1].Name.String = "xsi:noNamespaceSchemaLocation";
  attributes[1].Name.Length = sizeof("xsi:noNamespaceSchemaLocation") - 1;

  if(element->Attribute_Quantity)
  {
    memcpy(&attributes[2], element->Attribute, sizeof(xs_attribute_t) * element->Attribute_Quantity);
  }
  element->Attribute_Quantity += 2;
  free((void*)element->Attribute);
  element->Attribute = attributes;

  return (xs_element_t*)root;
}

xs_element_t* compile_xsd(const tree_t* const tree, const options_t* const options)
{
  global_element_list_t element_list;
  context_t context;

  memset(&element_list, 0, sizeof(global_element_list_t));
  memset(&context, 0, sizeof(context_t));
  context.Options = options;

  //Parse all the global elements, globaL complexType, attributes and simpleType tags
  const tree_t* node = tree;
  while(node)
  {
    const xsd_tag_t* const tag = node->Data;
    switch(*tag)
    {
    case XS_GLOBAL_ELEMENT_TAG:
    {
      xs_element_t* element = calloc(sizeof(xs_element_t), 1);
      parse_element(node, element, &context);
      add_global_element(&element_list, element);
      break;
    }

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
      add_node((linked_list_t*)&context.Attribute_List, attribute);
      break;
    }

    case XS_SIMPLE_TYPE_TAG:
    {
      xs_element_t* simpleType = calloc(sizeof(xs_element_t), 1);
      parse_simple_type(node, simpleType, &context);
      break;
    }

    default:
      break;
    }
    node = node->Next;
  }

  // Resolve all element with simple type or complex types
  if(context.Type_List.Element)
  {
    resolve_element_types(&context.Type_List, &context.SimpleType_List, &context.ComplexType_List);
  }

  // Resolve all element references
  if(context.Ref_List.Element)
  {
    resolve_element_references(&element_list, &context.Ref_List);
  }

  if(context.Attr_Ref_List.Element)
  {
    resolve_attribute_references(&context.Attribute_List, &context.Attr_Ref_List);
  }

  if(context.Attr_Type_List.Element)
  {
    resolve_attribute_types(&context.Attr_Type_List, &context.SimpleType_List);
  }

  // search and return root element.
  return create_root(&element_list);
}
