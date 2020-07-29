/**
 * \file
 * \brief   Implements the root element "xs:schema" tag

 * \author  Nandkishor Biradar
 * \date    19 June 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <float.h>

#include "common.h"
#include "libs/parse_xml.h"

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  char* name;
  char* address;
  char* city;
  char* country;
}shipTo;

typedef struct
{
  char* title;
  char* note;
  uint32_t Quantity;
  float price;
}item;

typedef struct
{
  char* orderPerson;
  shipTo ShipTo;
  item Item[2];
  char* orderId;
}shipOrder;

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

shipOrder ShipOrder;

static const xs_element_t shiptoElement;
static const xs_element_t orderpersonElement;
static const xs_element_t itemElement;

const xs_element_t* shipOrder_Descendant[] =
{
  &orderpersonElement,
  &shiptoElement,
  &itemElement,
};

static const xs_attribute_t shipOrder_attr[] =
{
  [0].Name.String = "xmlns:xsi",
  [0].Name.Length = sizeof("xmlns:xsi") - 1,

  [1].Name.String = "xsi:noNamespaceSchemaLocation",
  [1].Name.Length = sizeof("xsi:noNamespaceSchemaLocation") - 1,

  [2].Name.String = "orderid",
  [2].Name.Length = sizeof("orderid") - 1,
  [2].Target.Type    = EN_STATIC,
  [2].Target.Address = &ShipOrder.orderId,
  [2].Content.Type   = EN_STRING_DYNAMIC,
  [2].Content.Facet.String.MinLength = 1,
  [2].Content.Facet.String.MaxLength = 64,
  [2].Use = EN_REQUIRED

};

static const xs_element_t shipOrder_xsd =
{
  .Name.String = "shiporder",
  .Name.Length = sizeof("shiporder") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 1,
  .Callback     = NULL,

  .Attribute_Quantity = ARRAY_LENGTH(shipOrder_attr),
  .Attribute          = shipOrder_attr,

  .Child_Quantity   = ARRAY_LENGTH(shipOrder_Descendant),
  .Child_Type       = EN_SEQUENCE,
  .Child            = shipOrder_Descendant,
};

const xs_element_t* const xml_roots = &shipOrder_xsd;
const xs_element_t xml_root =
{
  .Child_Quantity = 1,
  .Child_Type     = EN_CHOICE,
  .Child = &xml_roots,
};

static const xs_element_t orderpersonElement =
{
  .Name.String = "orderperson",
  .Name.Length = sizeof("orderperson") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 1,
  .Callback     = NULL,

  .Target.Type    = EN_STATIC,
  .Target.Address = &ShipOrder.orderPerson,

  .Content.Type   = EN_STRING_DYNAMIC,
  .Content.Facet.String.MinLength = 1,
  .Content.Facet.String.MaxLength = 64,
};

static const xs_element_t name;
static const xs_element_t address;
static const xs_element_t city;
static const xs_element_t country;

static const xs_element_t* const ShipTO_Descendant[] =
{
  &name,
  &address,
  &city,
  &country
};

static const xs_element_t shiptoElement =
{
  .Name.String = "shipto",
  .Name.Length = sizeof("shipto") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 2,
  .Callback     = NULL,

  .Child_Quantity   = ARRAY_LENGTH(ShipTO_Descendant ),
  .Child_Type       = EN_SEQUENCE,
  .Child            = ShipTO_Descendant
};

static const xs_element_t name =
{
  .Name.String = "name",
  .Name.Length = sizeof("name") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 1,
  .Callback     = NULL,

  .Target.Type    = EN_STATIC,
  .Target.Address = &ShipOrder.ShipTo.name,

  .Content.Type   = EN_STRING_DYNAMIC,
  .Content.Facet.String.MinLength = 1,
  .Content.Facet.String.MaxLength = 64,
};

static const xs_element_t address =
{
  .Name.String = "address",
  .Name.Length = sizeof("address") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 1,
  .Callback     = NULL,

  .Target.Type    = EN_STATIC,
  .Target.Address = &ShipOrder.ShipTo.address,

  .Content.Type   = EN_STRING_DYNAMIC,
  .Content.Facet.String.MinLength = 1,
  .Content.Facet.String.MaxLength = 64,
};

static const xs_element_t city =
{
  .Name.String = "city",
  .Name.Length = sizeof("city") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 1,
  .Callback     = NULL,

  .Target.Type    = EN_STATIC,
  .Target.Address = &ShipOrder.ShipTo.city,

  .Content.Type   = EN_STRING_DYNAMIC,
  .Content.Facet.String.MinLength = 1,
  .Content.Facet.String.MaxLength = 64,
};

static const xs_element_t country =
{
  .Name.String = "country",
  .Name.Length = sizeof("country") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 1,
  .Callback     = NULL,

  .Target.Type    = EN_STATIC,
  .Target.Address = &ShipOrder.ShipTo.country,

  .Content.Type   = EN_STRING_DYNAMIC,
  .Content.Facet.String.MinLength = 1,
  .Content.Facet.String.MaxLength = 64,
};

static const xs_element_t title;
static const xs_element_t note;
static const xs_element_t quantity;
static const xs_element_t price;

static const xs_element_t* const item_Descendant[] =
{
  &title,
  &note,
  &quantity,
  &price
};

static const xs_element_t itemElement =
{
  .Name.String = "item",
  .Name.Length = sizeof("item") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 2,
  .Callback     = NULL,

  .Target.Type    = EN_STATIC,
  .Target.Address = &ShipOrder.Item,
  .Target.Size    = sizeof(item),

  .Child_Quantity   = ARRAY_LENGTH(item_Descendant),
  .Child_Type       = EN_SEQUENCE,
  .Child            = item_Descendant
};

static const xs_element_t title =
{
  .Name.String = "title",
  .Name.Length = sizeof("title") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 1,
  .Callback     = NULL,

  .Target.Type    = EN_RELATIVE,
  .Target.Offset  = offsetof(item, title),  // &ShipOrder.Item.title,

  .Content.Type   = EN_STRING_DYNAMIC,
  .Content.Facet.String.MinLength = 1,
  .Content.Facet.String.MaxLength = 64,
};

static const xs_element_t note =
{
  .Name.String = "note",
  .Name.Length = sizeof("note") - 1,
  .MinOccur     = 0,
  .MaxOccur     = 1,
  .Callback     = NULL,

  .Target.Type   = EN_RELATIVE,
  .Target.Offset = offsetof(item, note),

  .Content.Type   = EN_STRING_DYNAMIC,
  .Content.Facet.String.MinLength = 1,
  .Content.Facet.String.MaxLength = 64,
};

static const xs_element_t quantity =
{
  .Name.String = "quantity",
  .Name.Length = sizeof("quantity") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 1,
  .Callback     = NULL,

  .Target.Type   = EN_RELATIVE,
  .Target.Offset = offsetof(item, Quantity),

  .Content.Type   = EN_UNSIGNED,
  .Content.Facet.Uint.MinValue = 0,
  .Content.Facet.Uint.MaxValue = UINT32_MAX,
};

static const xs_element_t price =
{
  .Name.String = "price",
  .Name.Length = sizeof("price") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 1,
  .Callback     = NULL,

  .Target.Type   = EN_RELATIVE,
  .Target.Offset = offsetof(item, price),

  .Content.Type   = EN_DECIMAL,
  .Content.Facet.Decimal.MinValue = FLT_MIN,
  .Content.Facet.Decimal.MaxValue = FLT_MAX,
};




/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

