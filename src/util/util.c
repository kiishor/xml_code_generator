/**
 * \file
 * \brief   contains the common utility functions

 * \author  Nandkishor Biradar
 * \date    19 June 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "libs/parse_xml.h"

#include "apps/tree.h"
#include "apps/list.h"
#include "util/util.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void traverse_up(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}


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

const attribute_list_t* search_attribute_node(const attribute_list_t* list, const string_t* const reference)
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

const element_list_t* search_element_node(const element_list_t* list, const string_t* const reference)
{
  while(list)
  {
    if(list->Element == NULL)
    {
      return NULL;
    }

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
