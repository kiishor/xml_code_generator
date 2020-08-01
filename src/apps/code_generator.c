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
 *  -------------------------------- DEFINITION --------------------------------
 */


/*
 *  ---------------------------- GLOBAL VARIABLES -----------------------------
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

static inline const char* get_format(char* buffer, const char* const source1,
                                     const char* const source2)
{
  strcpy(buffer, source1);
  strcat(buffer, source2);
  return buffer;
}
static inline void write_target(const target_address_t* const target,
                                const char* const format, const char* const parent,
                                const char* const element, FILE* const source)
{
  char buffer[64];

  fprintf(source, get_format(buffer, format, ".Target.Type    = %s,\n"), xs_target_address_type[target->Type]);
  switch(target->Type)
  {
  case EN_STATIC:
    if(parent)
    {
      fprintf(source, get_format(buffer, format, ".Target.Address = &%s.%s,\n"), parent, element);
    }
    else
    {
      fprintf(source, get_format(buffer, format, ".Target.Address = &%s,\n"), element);
    }
    break;

  case EN_DYNAMIC:
    break;

  case EN_RELATIVE:
    fprintf(source, get_format(buffer, format, ".Target.Offset  = offsetof(%s_t, %s),\n"), parent, element);
    fprintf(source, get_format(buffer, format, ".Target.Size    = sizeof(%s_t),\n"), parent);
    break;

  default:
    break;
  }
}

static inline void write_content(const xml_content_t* const content,
                                 const char* const format,
                                 FILE* const source)
{
  char buffer[64];

  fprintf(source, get_format(buffer, format, ".Content.Type   = %s,\n"), xml_content_type[content->Type]);

  switch(content->Type)
  {
  case EN_STRING_DYNAMIC:
  case EN_CHAR_ARRAY:
    fprintf(source, get_format(buffer, format, ".Content.Facet.String.MinLength = %u,\n"), content->Facet.String.MinLength);
    fprintf(source, get_format(buffer, format, ".Content.Facet.String.MaxLength = %u,\n"), content->Facet.String.MaxLength);
    break;

  case EN_ENUM_STRING:
  case EN_ENUM_UINT:
  case EN_ENUM_INT:
    fprintf(source, get_format(buffer, format, ".Content.Facet.Enum.Quantity = %u,\n"), content->Facet.Enum.Quantity);
  break;

  case EN_DECIMAL:
    fprintf(source, get_format(buffer, format, ".Content.Facet.Decimal.MinValue = %g,\n"), content->Facet.Decimal.MinValue);
    fprintf(source, get_format(buffer, format, ".Content.Facet.Decimal.MaxValue = %g,\n"), content->Facet.Decimal.MaxValue);
    break;

  case EN_UNSIGNED:
    fprintf(source, get_format(buffer, format, ".Content.Facet.Uint.MinValue = %u,\n"), content->Facet.Uint.MinValue);
    fprintf(source, get_format(buffer, format, ".Content.Facet.Uint.MaxValue = %u,\n"), content->Facet.Uint.MaxValue);
    break;

  case EN_INTEGER:
    fprintf(source, get_format(buffer, format, ".Content.Facet.Int.MinValue = %d,\n"), content->Facet.Int.MinValue);
    fprintf(source, get_format(buffer, format, ".Content.Facet.Int.MaxValue = %d,\n"), content->Facet.Int.MaxValue);
    break;
  }
}

// Reorder all the complex xs_element_t
void generate_xml_source(const xs_element_t* const element, FILE* const header,
                         FILE* const source)
{
  uint32_t quantity = element->Child_Quantity;
  const xs_element_t* const* child = element->Child;
  while(quantity)
  {
    quantity--;
    if(child[quantity]->Child_Quantity || child[quantity]->Attribute_Quantity)
    {
      generate_xml_source(child[quantity], header, source);
    }
  }

  if(element->Name.String)
  {
    fprintf(header, "\ntypedef struct \n{\n");
    for(uint32_t i = 0; i < element->Child_Quantity; i++)
    {
      if(child[i]->Child_Quantity || child[i]->Attribute_Quantity)
      {
        fprintf(header, "    %s_t %s;\n", child[i]->Name.String, child[i]->Name.String);
      }
      else
      {
        fprintf(header, "    %s %s;\n", xml_data_type[child[i]->Content.Type], child[i]->Name.String);
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

  fprintf(source, "\n");
  for(uint32_t i = 0; i < element->Child_Quantity; i++)
  {
    fprintf(source, "static const xs_element_t %s_element;\n", child[i]->Name.String);
  }

  if(element->Name.String)
  {
    fprintf(source, "\nstatic const xs_element_t* %s_descendant[] =\n{\n", element->Name.String);
  }
  else
  {
    fprintf(source, "\nconst xs_element_t* root_descendant[] =\n{\n");
  }

  for(uint32_t i = 0; i < element->Child_Quantity; i++)
  {
    fprintf(source, "    &%s_element,\n", child[i]->Name.String);
  }
  fprintf(source, "};\n");

  if(element->Attribute_Quantity)
  {
    const xs_attribute_t* const attribute = element->Attribute;
    fprintf(source, "\nstatic const xs_attribute_t %s_attribute[] =\n{\n", element->Name.String);
    char buffer[16];
    for(uint32_t i = 0; i < element->Attribute_Quantity; i++)
    {
      fprintf(source, "    [%u].Name.String = \"%s\",\n", i, attribute[i].Name.String);
      fprintf(source, "    [%u].Name.Length = %u,\n", i, (uint32_t)attribute[i].Name.Length);

      if(attribute[i].Content.Type != EN_NO_XML_DATA_TYPE)
      {
        sprintf(buffer, "    [%u]", i);
        write_target(&attribute[i].Target, buffer, element->Name.String, attribute[i].Name.String, source);
        write_content(&attribute[i].Content, buffer, source);
      }
      fprintf(source, "    [%u].Use         = %s,\n", i, xml_attribute_use_type[attribute[i].Use]);
    }
    fprintf(source, "};\n\n");
  }

  if(element->Child_Quantity)
  {
    for(uint32_t i = 0; i < element->Child_Quantity; i++)
    {
      fprintf(source, "\nstatic const xs_element_t %s_element =\n{\n", child[i]->Name.String);
      fprintf(source, "    .Name.String = \"%s\",\n", child[i]->Name.String);
      fprintf(source, "    .Name.Length = %u,\n", child[i]->Name.Length);
      fprintf(source, "    .MinOccur    = %u,\n", child[i]->MinOccur);
      fprintf(source, "    .MaxOccur    = %u,\n", child[i]->MaxOccur);
      fprintf(source, "    .Callback    = NULL,\n");
      write_target(&child[i]->Target, "    ", element->Name.String, child[i]->Name.String, source);
      write_content(&child[i]->Content, "    ", source);

      if(child[i]->Attribute_Quantity)
      {
        fprintf(source, "    .Attribute_Quantity = %u,\n", child[i]->Attribute_Quantity);
        fprintf(source, "    .Attribute = %s_attribute,\n", child[i]->Name.String);

      }
      if(child[i]->Child_Quantity)
      {
        fprintf(source, "    .Child_Quantity = %u,\n", child[i]->Child_Quantity);
        fprintf(source, "    .Child_Type     = %s,\n", xml_child_order[child[i]->Child_Type]);
        fprintf(source, "    .Child          = %s_descendant,\n", child[i]->Name.String);
      }
      fprintf(source, "};\n");
    }
  }
}

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */



