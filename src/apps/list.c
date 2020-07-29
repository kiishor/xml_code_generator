/**
 * \file
 * \brief  Handles the linked list data structure

 * \author  Nandkishor Biradar
 * \date  10 March 2020
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>
#include <stdlib.h>

#include "apps/list.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

list_t* add_list_node(list_t* list, size_t list_size)
{
  while(list->Next != NULL)
  {
    list = list->Next;
  }

  list->Next = calloc(list_size, 1);;
  return list->Next;
}



