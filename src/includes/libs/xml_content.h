#ifndef XML_CONTENT_H
#define XML_CONTENT_H

/**
 * \file
 * \brief   datatype for XML content.

 * \author  Nandkishor Biradar
 * \date    19 August 2019
 */

/*
 *  ------------------------------- ENUMERATION -------------------------------
 */

//! basic data type of an element or attribute
typedef enum
{
  EN_NONE,

  //! String will be dynamically allocated using malloc and the pointer is copied to the target
  XS_STRING_DYNAMIC,
  //! String will be directly copied to the target using memcpy
  XS_STRING_STATIC,

  EN_ARRAY,           //!< uint8_t array
  EN_ENUM,            //!< Enumeration
  EN_DECIMAL,         //!< Float data type
  EN_INTEGER,         //!< 32-bit signed integer
  XS_UNSIGNED,        //!< 32-bit unsigned integer
  EN_BOOL,            //!< Boolean data type
  EN_DATE,            //!< Date in "YYYY-MM-DD" format
  EN_TIME,            //!< Time in "HH:MM:SS" format
  EN_DURATION,        //!< Duration
}xml_content_type_t;

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  uint32_t MinValue;
  uint32_t MaxValue;
}uint_facet_t;

typedef struct
{
  int32_t MinValue;
  int32_t MaxValue;
}int_facet_t;

typedef struct
{
  uint32_t MinLength;
  uint32_t MaxLength;
}string_facet_t;

typedef struct
{
  float MinValue;
  float MaxValue;
}decimal_facet_t;

typedef union
{
  uint_facet_t Uint;
  int_facet_t Int;
  string_facet_t String;
  decimal_facet_t Decimal;
}facet_t;

typedef struct
{
  xml_content_type_t Type;
  facet_t Facet;
}xml_content_t;

#endif // XML_CONTENT_H
