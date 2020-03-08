#ifndef XS_SEQUENCE_H
#define XS_SEQUENCE_H

/**
 * \file
 * \brief   Header file for xs_sequence.c

 * \author  Nandkishor Biradar
 * \date    18 June 2019
 */

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  string_t id;           // Optional
  uint32_t minOccurs;
  uint32_t maxOccurs;
}sequence_attr_t;

typedef struct
{
  xsd_tag_t Type;
  sequence_attr_t attr;
}sequence_t;

/*
 *  ------------------------ EXTERNAL GLOBAL VARIABLES ------------------------
 */

extern const xs_element_t xs_sequence;

#endif // XS_SEQUENCE_H
