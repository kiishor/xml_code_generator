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

#include "apps/tree.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void traverse_up(uint32_t occurrence, void* content, void** context)
{
  tree_t* node = *context;
  *context = node->Parent;
}
