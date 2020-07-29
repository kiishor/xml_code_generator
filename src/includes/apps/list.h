#ifndef LIST_H
#define LIST_H

/**
 * \file
 * \brief  Header file for list.c

 * \author  Nandkishor Biradar
 * \date  10 March 2020
 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct list_t list_t;

struct list_t
{
  list_t* Next;
};

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

extern list_t* add_list_node(list_t* list, size_t list_size);

#endif // LIST_H

