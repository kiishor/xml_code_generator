#include<stdint.h>
#include<stdbool.h>
#include<float.h>

#include "shiporder.h"


shiporder_t shiporder;


static const xs_element_t title_element;
static const xs_element_t note_element;
static const xs_element_t quantity_element;
static const xs_element_t price_element;

static const xs_element_t* item_descendant[] =
{
    &title_element,
    &note_element,
    &quantity_element,
    &price_element,
};

static const xs_element_t title_element =
{
    .Name.String = "title",
    .Name.Length = 5,
    .MinOccur    = 1,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(item_t, title),
    .Target.Size    = sizeof(item_t),
    .Content.Type   = EN_STRING_DYNAMIC,
    .Content.Facet.String.MinLength = 0,
    .Content.Facet.String.MaxLength = 4294967295,
};

static const xs_element_t note_element =
{
    .Name.String = "note",
    .Name.Length = 4,
    .MinOccur    = 0,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(item_t, note),
    .Target.Size    = sizeof(item_t),
    .Content.Type   = EN_STRING_DYNAMIC,
    .Content.Facet.String.MinLength = 0,
    .Content.Facet.String.MaxLength = 4294967295,
};

static const xs_element_t quantity_element =
{
    .Name.String = "quantity",
    .Name.Length = 8,
    .MinOccur    = 1,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(item_t, quantity),
    .Target.Size    = sizeof(item_t),
    .Content.Type   = EN_UNSIGNED,
    .Content.Facet.Uint.MinValue = 1,
    .Content.Facet.Uint.MaxValue = 4294967295,
};

static const xs_element_t price_element =
{
    .Name.String = "price",
    .Name.Length = 5,
    .MinOccur    = 1,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(item_t, price),
    .Target.Size    = sizeof(item_t),
    .Content.Type   = EN_DECIMAL,
    .Content.Facet.Decimal.MinValue = -3.40282e+038,
    .Content.Facet.Decimal.MaxValue = 3.40282e+038,
};

static const xs_element_t name_element;
static const xs_element_t address_element;
static const xs_element_t city_element;
static const xs_element_t country_element;

static const xs_element_t* shipto_descendant[] =
{
    &name_element,
    &address_element,
    &city_element,
    &country_element,
};

static const xs_element_t name_element =
{
    .Name.String = "name",
    .Name.Length = 4,
    .MinOccur    = 1,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(shipto_t, name),
    .Target.Size    = sizeof(shipto_t),
    .Content.Type   = EN_STRING_DYNAMIC,
    .Content.Facet.String.MinLength = 0,
    .Content.Facet.String.MaxLength = 4294967295,
};

static const xs_element_t address_element =
{
    .Name.String = "address",
    .Name.Length = 7,
    .MinOccur    = 1,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(shipto_t, address),
    .Target.Size    = sizeof(shipto_t),
    .Content.Type   = EN_STRING_DYNAMIC,
    .Content.Facet.String.MinLength = 0,
    .Content.Facet.String.MaxLength = 4294967295,
};

static const xs_element_t city_element =
{
    .Name.String = "city",
    .Name.Length = 4,
    .MinOccur    = 1,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(shipto_t, city),
    .Target.Size    = sizeof(shipto_t),
    .Content.Type   = EN_STRING_DYNAMIC,
    .Content.Facet.String.MinLength = 0,
    .Content.Facet.String.MaxLength = 4294967295,
};

static const xs_element_t country_element =
{
    .Name.String = "country",
    .Name.Length = 7,
    .MinOccur    = 1,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(shipto_t, country),
    .Target.Size    = sizeof(shipto_t),
    .Content.Type   = EN_STRING_DYNAMIC,
    .Content.Facet.String.MinLength = 0,
    .Content.Facet.String.MaxLength = 4294967295,
};

static const xs_element_t orderperson_element;
static const xs_element_t shipto_element;
static const xs_element_t item_element;

static const xs_element_t* shiporder_descendant[] =
{
    &orderperson_element,
    &shipto_element,
    &item_element,
};

static const xs_attribute_t shiporder_attribute[] =
{
    [0].Name.String = "xmlns:xsi",
    [0].Name.Length = 9,
    [0].Use         = EN_OPTIONAL,
    [1].Name.String = "xsi:noNamespaceSchemaLocation",
    [1].Name.Length = 29,
    [1].Use         = EN_OPTIONAL,
    [2].Name.String = "orderid",
    [2].Name.Length = 7,
    [2].Target.Type    = EN_RELATIVE,
    [2].Target.Offset  = offsetof(shiporder_t, orderid),
    [2].Target.Size    = sizeof(shiporder_t),
    [2].Content.Type   = EN_STRING_DYNAMIC,
    [2].Content.Facet.String.MinLength = 0,
    [2].Content.Facet.String.MaxLength = 4294967295,
    [2].Use         = EN_REQUIRED,
};


static const xs_element_t orderperson_element =
{
    .Name.String = "orderperson",
    .Name.Length = 11,
    .MinOccur    = 1,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(shiporder_t, orderperson),
    .Target.Size    = sizeof(shiporder_t),
    .Content.Type   = EN_STRING_DYNAMIC,
    .Content.Facet.String.MinLength = 0,
    .Content.Facet.String.MaxLength = 4294967295,
};

static const xs_element_t shipto_element =
{
    .Name.String = "shipto",
    .Name.Length = 6,
    .MinOccur    = 1,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(shiporder_t, shipto),
    .Target.Size    = sizeof(shiporder_t),
    .Content.Type   = EN_NO_XML_DATA_TYPE,
    .Child_Quantity = 4,
    .Child_Type     = EN_SEQUENCE,
    .Child          = shipto_descendant,
};

static const xs_element_t item_element =
{
    .Name.String = "item",
    .Name.Length = 4,
    .MinOccur    = 1,
    .MaxOccur    = 4294967295,
    .Callback    = NULL,
    .Target.Type    = EN_RELATIVE,
    .Target.Offset  = offsetof(shiporder_t, item),
    .Target.Size    = sizeof(shiporder_t),
    .Content.Type   = EN_NO_XML_DATA_TYPE,
    .Child_Quantity = 4,
    .Child_Type     = EN_SEQUENCE,
    .Child          = item_descendant,
};

static const xs_element_t shiporder_element;

const xs_element_t* root_descendant[] =
{
    &shiporder_element,
};

static const xs_element_t shiporder_element =
{
    .Name.String = "shiporder",
    .Name.Length = 9,
    .MinOccur    = 1,
    .MaxOccur    = 1,
    .Callback    = NULL,
    .Target.Type    = EN_STATIC,
    .Target.Address = &shiporder,
    .Content.Type   = EN_NO_XML_DATA_TYPE,
    .Attribute_Quantity = 3,
    .Attribute = shiporder_attribute,
    .Child_Quantity = 3,
    .Child_Type     = EN_SEQUENCE,
    .Child          = shiporder_descendant,
};
const xs_element_t xml_root =
{
    .Child_Quantity = 1,
    .Child_Type     = EN_CHOICE,
    .Child = root_descendant,
};
