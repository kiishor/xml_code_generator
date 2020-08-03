/**
 * \file
 * \brief   Contains all the XML schema supported tags

 * \author  Nandkishor Biradar
 * \date    12 March 2020
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libs/parse_xml.h"
#include "apps/xs_data_type.h"

#include "apps/tree.h"
#include "apps/xsd.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */
FILE* create_header_file(const char* const name)
{
  char file_name[strlen(name) + 2];
  strcpy(file_name, name);
  strcat(file_name, ".h");

  FILE* header = fopen( file_name, "w");


  fprintf(header, "#ifndef %s_H_INCLUDED\n#define %s_H_INCLUDED\n", name, name);
  fprintf(header, "\n#include<stdint.h>\n#include<stdbool.h>\n");
  fprintf(header, "\n\n#include\"libs/parse_xml.h\"\n");

  return header;
}

void close_header_file(FILE* const header, const char* const name)
{
  fprintf(header, "\nextern %s_t %s;\n", name, name);
  fprintf(header, "\nextern const xs_element_t xml_root;\n");
  fprintf(header, "\n#endif\n");
  fclose(header);
}

FILE* create_source_file(const char* const name)
{
  char file_name[strlen(name) + 2];
  strcpy(file_name, name);
  strcat(file_name, ".c");

  FILE* source = fopen( file_name, "w");

  fprintf(source, "#include<stdint.h>\n#include<stdbool.h>\n#include<float.h>\n\n#include \"%s.h\"\n\n", name);
  fprintf(source, "\n%s_t %s;\n\n", name, name);

  return source;
}

void close_source_file(FILE* const source, const char* const name)
{
  fprintf(source, "const xs_element_t xml_root =\n{\n");
  fprintf(source, "    .Child_Quantity = 1,\n");
  fprintf(source, "    .Child_Type     = EN_CHOICE,\n");
  fprintf(source, "    .Child = root_descendant,\n");
  fprintf(source, "};\n");
  fclose(source);
}

static inline void write_target(const target_address_t* const target,
                                uint32_t index,
                                const char* const parent,
                                const char* const element, FILE* const source)
{
  fprintf(source, "    [%u].Target.Type    = %s,\n", index, xs_target_address_type[target->Type]);
  switch(target->Type)
  {
  case EN_STATIC:
    if(parent)
    {
      fprintf(source, "    [%u].Target.Address = &%s.%s,\n", index, parent, element);
    }
    else
    {
      fprintf(source, "    [%u].Target.Address = &%s,\n", index, element);
    }
    break;

  case EN_DYNAMIC:
    break;

  case EN_RELATIVE:
    fprintf(source, "    [%u].Target.Offset  = offsetof(%s_t, %s),\n", index, parent, element);
    fprintf(source, "    [%u].Target.Size    = sizeof(%s_t),\n", index, parent);
    break;

  default:
    break;
  }
}

static inline void write_content(const xml_content_t* const content,
                                 uint32_t index,
                                 FILE* const source)
{

  fprintf(source, "    [%u].Content.Type   = %s,\n", index, xml_content_type[content->Type]);

  switch(content->Type)
  {
  case EN_STRING_DYNAMIC:
  case EN_CHAR_ARRAY:
    fprintf(source, "    [%u].Content.Facet.String.MinLength = %u,\n", index, content->Facet.String.MinLength);
    fprintf(source, "    [%u].Content.Facet.String.MaxLength = %u,\n", index, content->Facet.String.MaxLength);
    break;

  case EN_ENUM_STRING:
  case EN_ENUM_UINT:
  case EN_ENUM_INT:
    fprintf(source, "    [%u].Content.Facet.Enum.Quantity = %u,\n", index, content->Facet.Enum.Quantity);
  break;

  case EN_DECIMAL:
    fprintf(source, "    [%u].Content.Facet.Decimal.MinValue = %g,\n", index, content->Facet.Decimal.MinValue);
    fprintf(source, "    [%u].Content.Facet.Decimal.MaxValue = %g,\n", index, content->Facet.Decimal.MaxValue);
    break;

  case EN_UNSIGNED:
    fprintf(source, "    [%u].Content.Facet.Uint.MinValue = %u,\n", index, content->Facet.Uint.MinValue);
    fprintf(source, "    [%u].Content.Facet.Uint.MaxValue = %u,\n", index, content->Facet.Uint.MaxValue);
    break;

  case EN_INTEGER:
    fprintf(source, "    [%u].Content.Facet.Int.MinValue = %d,\n", index, content->Facet.Int.MinValue);
    fprintf(source, "    [%u].Content.Facet.Int.MaxValue = %d,\n", index, content->Facet.Int.MaxValue);
    break;
  }
}

// Reorder all the complex xs_element_t
void generate_xml_source(const xs_element_t* const element, FILE* const header,
                         FILE* const source)
{
  uint32_t quantity = element->Child_Quantity;
  const xs_element_t* child = element->Child;
  while(quantity)
  {
    quantity--;
    if(child[quantity].Child_Quantity || child[quantity].Attribute_Quantity)
    {
      generate_xml_source(&child[quantity], header, source);
    }
  }

  if(element->Name.String)
  {
    fprintf(header, "\ntypedef struct \n{\n");
    for(uint32_t i = 0; i < element->Child_Quantity; i++)
    {
      if(child[i].Child_Quantity || child[i].Attribute_Quantity)
      {
        fprintf(header, "    %s_t %s;\n", child[i].Name.String, child[i].Name.String);
      }
      else
      {
        fprintf(header, "    %s %s;\n", xml_data_type[child[i].Content.Type], child[i].Name.String);
      }
    }

    const xs_attribute_t* const attributes = element->Attribute;
    for(uint32_t i = 0; i < element->Attribute_Quantity; i++)
    {
      if(attributes[i].Content.Type == EN_NO_XML_DATA_TYPE)
      {
        continue;
      }
      fprintf(header, "    %s %s;\n", xml_data_type[attributes[i].Content.Type],
                                      attributes[i].Name.String);
    }
    fprintf(header, "}%s_t;\n", element->Name.String);
  }

  if(element->Child_Quantity)
  {
    if(element->Name.String)
    {
      fprintf(source, "\nstatic const xs_element_t %s_descendant[] =\n{\n", element->Name.String);
    }
    else
    {
      fprintf(source, "\nconst xs_element_t root_descendant[] =\n{\n");
    }

    uint32_t i = 0;
    do
    {
      fprintf(source, "    [%u].Name.String = \"%s\",\n", i, child[i].Name.String);
      fprintf(source, "    [%u].Name.Length = %u,\n", i, child[i].Name.Length);
      fprintf(source, "    [%u].MinOccur    = %u,\n", i, child[i].MinOccur);
      fprintf(source, "    [%u].MaxOccur    = %u,\n", i, child[i].MaxOccur);
      fprintf(source, "    [%u].Callback    = NULL,\n", i);
      write_target(&child[i].Target, i, element->Name.String, child[i].Name.String, source);
      write_content(&child[i].Content, i, source);

      if(child[i].Attribute_Quantity)
      {
        fprintf(source, "    [%u].Attribute_Quantity = %u,\n", i, child[i].Attribute_Quantity);
        fprintf(source, "    [%u].Attribute          = %s_attribute,\n", i, child[i].Name.String);

      }
      if(child[i].Child_Quantity)
      {
        fprintf(source, "    [%u].Child_Quantity = %u,\n", i, child[i].Child_Quantity);
        fprintf(source, "    [%u].Child_Type     = %s,\n", i, xml_child_order[child[i].Child_Type]);
        fprintf(source, "    [%u].Child          = %s_descendant,\n", i, child[i].Name.String);
      }

      if(++i == element->Child_Quantity)
      {
        break;
      }
      fprintf(source, "\n");

    }while(1);

  fprintf(source, "};\n");
  }

  if(element->Attribute_Quantity)
  {
    const xs_attribute_t* const attribute = element->Attribute;
    fprintf(source, "\nstatic const xs_attribute_t %s_attribute[] =\n{\n", element->Name.String);
    uint32_t i = 0;
    do
    {
      fprintf(source, "    [%u].Name.String = \"%s\",\n", i, attribute[i].Name.String);
      fprintf(source, "    [%u].Name.Length = %u,\n", i, (uint32_t)attribute[i].Name.Length);

      if(attribute[i].Content.Type != EN_NO_XML_DATA_TYPE)
      {
        write_target(&attribute[i].Target, i, element->Name.String, attribute[i].Name.String, source);
        write_content(&attribute[i].Content, i, source);
      }
      fprintf(source, "    [%u].Use         = %s,\n", i, xml_attribute_use_type[attribute[i].Use]);

      if(++i == element->Attribute_Quantity)
      {
        break;
      }
      fprintf(source, "\n");
    }while(1);

    fprintf(source, "};\n\n");
  }


}

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */



