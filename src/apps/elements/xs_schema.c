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

#include "apps/tree.h"

#include "common.h"

#include "libs/parse_xml.h"
#include "xs_schema.h"

#include "apps/xsd.h"
#include "xs_element.h"
#include "xs_complex_type.h"
#include "xs_attribute.h"
#include "xs_simple_type.h"

/*
 *  -------------------------------- STRUCTURE --------------------------------
 */

typedef struct
{
  size_t ElementQuantity;
  element_t* Elements;
}schema_t;



/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

static void add_schema(uint32_t Occurrence, void *Content, void** context);

/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
 */

schema_t Schema;
tree_t SchemaTree =
{
  .Data = &Schema,
};
tree_t* pXsdData = &SchemaTree;


const xs_element_t* schemaDescendant[] =
{
  &xs_global_element,
  &xs_complexType,
  &xs_attribute,
  &xs_simpleType,
};

static const xs_attribute_t schema_attr[] =
{
  [0].Name.String = "xmlns:xsi",
  [0].Name.Length = sizeof("xmlns:xsi") - 1,

  [1].Name.String = "xmlns:xs",
  [1].Name.Length = sizeof("xmlns:xs") - 1,

  [2].Name.String = "xmlns:hfp",
  [2].Name.Length = sizeof("xmlns:hfp") - 1,
};

static const xs_element_t xsd_schema =
{
  .Name.String = "xs:schema",
  .Name.Length = sizeof("xs:schema") - 1,
  .MinOccur     = 1,
  .MaxOccur     = 1,
  .Callback     = add_schema,

  .Target.Type    = EN_STATIC,
  .Target.Address = &Schema,

  .Attribute_Quantity = ARRAY_LENGTH(schema_attr),
  .Attribute          = schema_attr,

  .Child_Quantity = ARRAY_LENGTH(schemaDescendant),
  .Child_Type      = EN_CHOICE,
  .Child          = schemaDescendant,
};

const xs_element_t* const xsd_roots = &xsd_schema;
const xs_element_t xsd_root =
{
  .Child_Quantity = 1,
  .Child_Type     = EN_CHOICE,
  .Child = &xsd_roots,
};

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static void add_schema(uint32_t Occurrence, void *Content, void** context)
{

}
