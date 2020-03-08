/**
 * \file
 * \brief  Handles the tree data structure

 * \author  Nandkishor Biradar
 * \date  15 June 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>
#include <stdlib.h>

#include "apps/tree.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

tree_t* create_node(void* data)
{
  tree_t* node = (tree_t* )calloc(1, sizeof(tree_t));
  node->Data = data;
  return node;
}

void add_descendant_node(tree_t* const parent, tree_t* const node)
{

  node->Parent = parent;

  if(parent->Descendant == NULL)
  {
    parent->Descendant = node;
    return;
  }

  tree_t* descendant = parent->Descendant;

  while(descendant->Next != NULL)
  {
    descendant = descendant->Next;
  }

  descendant->Next = node;
  return;
}

