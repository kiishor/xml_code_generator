#ifndef TREE_H
#define TREE_H

/**
 * \file
 * \brief  Header file for tree.c

 * \author  Nandkishor Biradar
 * \date  15 June 2019
 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct tree_t tree_t;

struct tree_t
{
  const void* Data;
  tree_t* Parent;
  tree_t* Next;
  tree_t* Descendant;
};

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
extern tree_t* create_node(void* data);
extern void add_descendant_node(tree_t* const parent, tree_t* const node);

#endif // TREE_H
