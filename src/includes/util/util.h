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

 //! A generic linked list structure
typedef struct
{
  list_t List;        //!< Pointer to next node in the list
  const void* Data;   //!< Data
}linked_list_t;

//! element linked list
typedef struct
{
  list_t List;                  //!< Linked list
  const xs_element_t* Element;  //!< pointer to element
}element_list_t;

//! Global element linked list
typedef struct
{
  list_t List;                  //!< Linked list
  const xs_element_t* Element;  //!< pointer to element
  //!< Reference count of global element. This keeps track if any element is referred by another element or type.
  //!< Ref_Count > 0 indicates that it is a child element not a root element.
  uint32_t Ref_Count;
}global_element_list_t;

//! Attribute linked list
typedef struct
{
  list_t List;    //!< Pointer to next node in the list
  const xs_attribute_t* Attribute;    //!< Attribute
}attribute_list_t;

//! simpleType linked list
typedef struct
{
  list_t List;    //!< Pointer to next node in the list
  const xs_element_t* Element;    //!< simpleType
}simpleType_list_t;

//! ComplexType linked list
typedef struct
{
  list_t List;      //!< Pointer to next node in the list
  const xs_element_t* Element;        //!< complexType
}complexType_list_t;

//! Linked list of unresolved elements/attributes either by reference or type
typedef struct
{
  list_t List;                  //!< Linked list
  union
  {
    const xs_element_t* Element;          //!< Unresolved element
    const xs_attribute_t* Attribute;      //!< Unresolved attribute
  };
  string_t Tag;                 //!< reference or type name
}unresolved_tag_t;

/*
 *  ---------------------------- EXPORTED FUNCTION ----------------------------
 */

extern void traverse_up( uint32_t occurrence, void* content, void* context);
extern void add_node(linked_list_t* list, const void* data);
extern void add_unresolved_tag(unresolved_tag_t* list, void* element, const string_t* const tag);
extern void add_global_element(global_element_list_t* list, const xs_element_t* const element);
extern const linked_list_t* search_node(linked_list_t* list, const string_t* const reference);

#endif // UTIL_H
