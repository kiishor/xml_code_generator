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

#include "parse_xml.h"

#include "apps/tree.h"
#include "apps/list.h"
#include "util/util.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void traverse_up(uint32_t occurrence, void* content, void* context)
{
  tree_t** schemaTree = context;
  tree_t* node = *schemaTree;
  *schemaTree = node->Parent;
}

void add_node(linked_list_t* list, const void* data)
{
  if(list->Data)
  {
    list = (linked_list_t*)add_list_node((list_t*)list, sizeof(linked_list_t));
  }
  list->Data = data;
}


void add_unresolved_tag(unresolved_tag_t* list, void* element, const string_t* const tag)
{
  // TODO: instead of traversing from head to last node, use head and tail pointers
  // to linked list node.
  if(list->Element)
  {
    list = (unresolved_tag_t*)add_list_node((list_t*)list, sizeof(unresolved_tag_t));
  }

  list->Element = element;
  list->Tag.Length = strlen(tag->String);
  list->Tag.String = tag->String;
}

void add_global_element(global_element_list_t* list, const xs_element_t* const element)
{
  if(list->Element)
  {
    list = (global_element_list_t*)add_list_node((list_t*)list, sizeof(global_element_list_t));
  }

  list->Element = element;
}

/** \brief This function search the linked list node by its name. It supports element_list_t & attribute_list_t
 *
 * \param list const linked_list_t* Linked list to be searched
 * \param reference const string_t* const name of of node to find
 * \return const linked_list_t* node if found otherwise NULL.
 *
 */
const linked_list_t* search_node(linked_list_t* list, const string_t* const reference)
{
  while(list)
  {
    if(!list->Data)
    {
      return NULL;
    }

    const string_t* const name = list->Data;
    if((name->Length == reference->Length) &&
       (!memcmp(name->String, reference->String, name->Length)))
    {
      return list;
    }
    list = (linked_list_t*)list->List.Next;
  }
  return NULL;
}
