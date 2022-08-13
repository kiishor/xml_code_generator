/**
 * \file
 * \brief   Helper function to set the facet properties

 * \author  Nandkishor Biradar
 * \date    12 August 2022
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>

#include "parse_xml.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void* get_facet_max_target(const xml_content_t* const content)
{
  switch(content->Type)
  {
  case EN_DECIMAL:
    return (void*)&content->Facet.Decimal.MaxValue;

	case EN_DOUBLE:
    return (void*)&content->Facet.Double.MaxValue;

	case EN_INT8:
	case EN_INT16:
	case EN_INT32:
    return (void*)&content->Facet.Int.MaxValue;

	case EN_UINT8:
	case EN_UINT16:
	case EN_UINT32:
    return (void*)&content->Facet.Uint.MaxValue;

	case EN_INT64:
    return (void*)&content->Facet.Long.MaxValue;

	case EN_UINT64:
    return (void*)&content->Facet.Ulong.MaxValue;

  default:
    return NULL;
  }
}

void* get_facet_min_target(const xml_content_t* const content)
{
  switch(content->Type)
  {
  case EN_DECIMAL:
    return (void*)&content->Facet.Decimal.MinValue;

	case EN_DOUBLE:
    return (void*)&content->Facet.Double.MinValue;

	case EN_INT8:
	case EN_INT16:
	case EN_INT32:
    return (void*)&content->Facet.Int.MinValue;

	case EN_UINT8:
	case EN_UINT16:
	case EN_UINT32:
    return (void*)&content->Facet.Uint.MinValue;

	case EN_INT64:
    return (void*)&content->Facet.Long.MinValue;

	case EN_UINT64:
    return (void*)&content->Facet.Ulong.MinValue;

  default:
    return NULL;
  }
}

void exclude_facet_max_value(xml_content_t* const content)
{
  switch(content->Type)
  {
  case EN_DECIMAL:
    content->Facet.Decimal.MaxValue -= 1;
    break;

	case EN_DOUBLE:
    content->Facet.Double.MaxValue -= 1;
    break;

	case EN_INT8:
	case EN_INT16:
	case EN_INT32:
    content->Facet.Int.MaxValue -= 1;
    break;

	case EN_UINT8:
	case EN_UINT16:
	case EN_UINT32:
    content->Facet.Uint.MaxValue -= 1;
    break;

	case EN_INT64:
    content->Facet.Long.MaxValue -= 1;
    break;

	case EN_UINT64:
    content->Facet.Ulong.MaxValue -= 1;
    break;

  default:
    break;
  }
}

void exclude_facet_min_value(xml_content_t* const content)
{
  switch(content->Type)
  {
  case EN_DECIMAL:
    content->Facet.Decimal.MinValue += 1;
    break;

	case EN_DOUBLE:
    content->Facet.Double.MinValue += 1;
    break;

	case EN_INT8:
	case EN_INT16:
	case EN_INT32:
    content->Facet.Int.MinValue += 1;
    break;

	case EN_UINT8:
	case EN_UINT16:
	case EN_UINT32:
    content->Facet.Uint.MinValue += 1;
    break;

	case EN_INT64:
    content->Facet.Long.MinValue += 1;
    break;

	case EN_UINT64:
    content->Facet.Ulong.MinValue += 1;
    break;

  default:
    break;
  }
}
