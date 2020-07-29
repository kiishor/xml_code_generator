#ifndef UTIL_H
#define UTIL_H

/**
 * \file
 * \brief   Header file for util.c

 * \author  Nandkishor Biradar
 * \date    19 June 2019
 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  list_t List;
  const xs_element_t* Element;
  uint32_t Ref_Count;
}element_list_t;

typedef struct
{
  list_t List;
  const xs_attribute_t* Attribute;
}attribute_list_t;

typedef struct
{
  list_t List;
  const xs_element_t* Element;
  string_t Reference;
}reference_list_t;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */
extern void add_attribute_node(attribute_list_t* list, const xs_attribute_t* attribute);
extern void add_element_node(element_list_t* list, const xs_element_t* element);
extern void add_reference_node(reference_list_t* list, void* element,
                                     const string_t* const reference);
extern attribute_list_t* search_attribute_node(attribute_list_t* list,
                                               const string_t* const reference);
extern element_list_t* search_element_node(element_list_t* list,
                                           const string_t* const reference);
extern void traverse_up( uint32_t occurrence, void* content, void** context);

#endif // UTIL_H
