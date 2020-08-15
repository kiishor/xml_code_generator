/**
 * \file
 * \brief   datatype for XML Schema
 * \author  Nandkishor Biradar
 * \date    02 May 2020
 */


/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */
#include <stdint.h>
#include <float.h>

#include "parse_xml.h"
#include "apps/xs_data_type.h"

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

#define ADD_DATA_TYPE(TAG, NAME, ...)  [TAG].String = #NAME, [TAG].Length = sizeof(#NAME) - 1,
const string_t xs_data_type[TOTAL_XSD_DATA_TYPE] = {ALL_XSD_DATA_TYPE};
#undef ADD_DATA_TYPE

#define ADD_USE_TYPE(TAG, NAME)  [TAG].String = #NAME, [TAG].Length = sizeof(#NAME) - 1,
const string_t xs_attribute_use_type[TOTAL_XSD_ATTRIBUTE_USE_VALUES] = {
  ALL_XSD_ATTRIBUTE_USE_VALUE
};
#undef ADD_USE_TYPE

#define ADD_USE_TYPE(TAG, NAME)  #TAG,
const char* const xml_attribute_use_type[TOTAL_XSD_ATTRIBUTE_USE_VALUES] = {
  ALL_XSD_ATTRIBUTE_USE_VALUE
};
#undef ADD_USE_TYPE

#define ADD_TYPE(TYPE)  #TYPE,
const char* const xs_target_address_type[TOTAL_TARGET_ADDRESS_TYPE] = {
  ALL_TARGET_ADDRESS_TYPE
};
#undef ADD_TYPE

#define ADD_CONTENT(type, ...)  #type,
const char* const xml_content_type[TOTAL_XML_CONTENT_TYPES] = {ALL_XML_CONTENT_TYPES};
#undef ADD_CONTENT

const char* const xml_data_type[] =
{
  [EN_STRING] = "string_t",
  [EN_STRING_DYNAMIC] = "char*",
  [EN_CHAR_ARRAY] = "char*",
  [EN_DECIMAL] = "float",
  [EN_INTEGER] = "int32_t",
  [EN_UNSIGNED] = "uint32_t",
  [EN_BOOL] = "bool",
  [EN_DATE] = "xs_date_t",
  [EN_TIME] = "xs_time_t",
  [EN_DATE_TIME] = "xs_date_time_t",
  [EN_DURATION] = "xs_duration_t",
};

const char* const xml_child_order[] =
{
  [EN_CHOICE] = "EN_CHOICE",
  [EN_SEQUENCE] = "EN_SEQUENCE"
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void convert_xsd_data_type(xml_content_t* const content, xs_data_type_t type)
{
  switch(type)
  {
  case XS_STRING:
    content->Type = EN_STRING_DYNAMIC;
    content->Facet.String.MinLength = 0;
    content->Facet.String.MaxLength = UINT32_MAX;
    break;

  case XS_DECIMAL:
  case XS_FLOAT:
    content->Type = EN_DECIMAL;
    content->Facet.Decimal.MinValue = -FLT_MAX;
    content->Facet.Decimal.MaxValue = FLT_MAX;
    break;

  case XS_INTEGER:
  case XS_INT:
    content->Type = EN_INTEGER;
    content->Facet.Int.MinValue = INT32_MIN;
    content->Facet.Int.MaxValue = INT32_MAX;
    break;

  case XS_NEGATIVE_INTEGER:
    content->Type = EN_INTEGER;
    content->Facet.Int.MinValue = INT32_MIN;
    content->Facet.Int.MaxValue = -1;
    break;

  case XS_NON_NEGATIVE_INTEGER:
  case XS_UNSIGNED_INT:
    content->Type = EN_UNSIGNED;
    content->Facet.Uint.MinValue = 0;
    content->Facet.Uint.MaxValue = UINT32_MAX;
    break;

  case XS_NON_POSITIVE_INTEGER:
    content->Type = EN_INTEGER;
    content->Facet.Int.MinValue = INT32_MIN;
    content->Facet.Int.MaxValue = 0;
    break;

  case XS_POSITIVE_INTEGER:
    content->Type = EN_UNSIGNED;
    content->Facet.Uint.MinValue = 1;
    content->Facet.Uint.MaxValue = UINT32_MAX;
    break;

  case XS_BOOLEAN:
    content->Type = EN_BOOL;
    break;

  case XS_DATE:
    content->Type = EN_DATE;
    break;

  case XS_TIME:
    content->Type = EN_TIME;
    break;

  case XS_DATE_TIME:
    content->Type = EN_DATE_TIME;
    break;

  case XS_DURATION:
    content->Type = EN_DURATION;
    break;
  }
}
