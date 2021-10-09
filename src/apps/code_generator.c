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
#include <assert.h>

#include "parse_xml.h"
#include "apps/xs_data_type.h"

#include "apps/tree.h"
#include "apps/xsd.h"

#include "code_generator.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static inline FILE* create_header_file(const char* const name)
{
  char file_name[strlen(name) + 2];
  strcpy(file_name, name);
  strcat(file_name, ".h");

  FILE* header = fopen( file_name, "w");


  fprintf(header, "#ifndef %s_H_INCLUDED\n#define %s_H_INCLUDED\n", name, name);
  fprintf(header, "\n#include <stdint.h>\n#include <stdbool.h>\n");
  fprintf(header, "\n#include \"parse_xml.h\"\n");

  return header;
}

static inline void close_header_file(FILE* const header, const char* const name)
{
  fprintf(header, "\nextern const xs_element_t %s_root;\n", name);
  fprintf(header, "\n#endif\n");
  fclose(header);
}

static inline FILE* create_source_file(const char* const name)
{
  char file_name[strlen(name) + 2];
  strcpy(file_name, name);
  strcat(file_name, ".c");

  FILE* source = fopen( file_name, "w");

  fprintf(source, "#include <stdlib.h>\n#include <stdbool.h>\n#include <float.h>\n\n#include \"%s.h\"\n\n", name);
  return source;
}

static inline void write_target(const target_address_t* const target,
                                const char* const array,
                                const char* const parent,
                                const char* const element,
                                bool target_size,
                                FILE* const source)
{
  fprintf(source, "    %s.Target.Type    = %s,\n", array, xs_target_address_type[target->Type]);
  switch(target->Type)
  {
  case EN_STATIC:
    if(parent)
    {
      fprintf(source, "    %s.Target.Address = &%s.%s,\n", array, parent, element);
      if(target_size)
      {
        fprintf(source, "    %s.Target.Size    = sizeof(%s_t),\n", array, element);
      }
    }
    else
    {
      fprintf(source, "    %s.Target.Address = &%s,\n", array, element);
    }
    break;

  case EN_DYNAMIC:
    fprintf(source, "    %s.Target.Allocate = allocate_%s,\n", array, element);
    break;

  case EN_RELATIVE:
    if(parent)
    {
      fprintf(source, "    %s.Target.Offset  = offsetof(%s_t, %s),\n", array, parent, element);
    }
    else
    {
      fprintf(source, "    %s.Target.Offset  = 0,\n", array);
    }
    if(target_size)
    {
      fprintf(source, "    %s.Target.Size    = sizeof(%s_t),\n", array, element);
    }
    break;

  default:
    break;
  }
}

static inline void write_content(const xml_content_t* const content,
                                 const char* const array,
                                 FILE* const source)
{

  fprintf(source, "    %s.Content.Type   = %s,\n", array, xml_content_type[content->Type]);

  switch(content->Type)
  {
  case EN_STRING_DYNAMIC:
  case EN_CHAR_ARRAY:
    fprintf(source, "    %s.Content.Facet.String.MinLength = %u,\n", array, content->Facet.String.MinLength);
    fprintf(source, "    %s.Content.Facet.String.MaxLength = %u,\n", array, content->Facet.String.MaxLength);
    break;

  case EN_ENUM_STRING:
  case EN_ENUM_UINT:
  case EN_ENUM_INT:
    fprintf(source, "    %s.Content.Facet.Enum.Quantity = %u,\n", array, content->Facet.Enum.Quantity);
  break;

  case EN_DECIMAL:
    fprintf(source, "    %s.Content.Facet.Decimal.MinValue = %gf,\n", array, content->Facet.Decimal.MinValue);
    fprintf(source, "    %s.Content.Facet.Decimal.MaxValue = %gf,\n", array, content->Facet.Decimal.MaxValue);
    break;

  case EN_UNSIGNED:
    fprintf(source, "    %s.Content.Facet.Uint.MinValue = %u,\n", array, content->Facet.Uint.MinValue);
    fprintf(source, "    %s.Content.Facet.Uint.MaxValue = %u,\n", array, content->Facet.Uint.MaxValue);
    break;

  case EN_INTEGER:
    fprintf(source, "    %s.Content.Facet.Int.MinValue = %d,\n", array, content->Facet.Int.MinValue);
    fprintf(source, "    %s.Content.Facet.Int.MaxValue = %d,\n", array, content->Facet.Int.MaxValue);
    break;
  }
}

void handle_occurrences(address_type_t type, uint32_t maxOccur, char* pointer, char* array)
{
  switch(type)
  {
  case EN_RELATIVE:
  case EN_STATIC:
    sprintf(array, "[%u]", maxOccur);
    break;

  case EN_DYNAMIC:
    sprintf(pointer, "*");
    break;
  }
}

static inline void write_header(const xs_element_t* const element,
                                FILE* const header)
{
  const xs_element_t* const child = element->Child;

  fprintf(header, "\ntypedef struct\n{\n");
  if(element->Content.Type != EN_NO_XML_DATA_TYPE)
  {
    fprintf(header, "    %s %s;\n", xml_data_type[element->Content.Type],
                                     element->Name.String);
  }

  for(uint32_t i = 0; i < element->Child_Quantity; i++)
  {
    char pointer[4] = "";
    char array[16] = "";

    if(child[i].MaxOccur > 1)
    {
      handle_occurrences(child[i].Target.Type, child[i].MaxOccur, pointer, array);
    }

    if(child[i].Child_Quantity || child[i].Attribute_Quantity)
    {
      fprintf(header, "    %s_t%s %s%s;\n", child[i].Name.String,
                                         pointer, child[i].Name.String, array);
    }
    else
    {
      fprintf(header, "    %s%s %s%s;\n", xml_data_type[child[i].Content.Type],
                                       pointer, child[i].Name.String, array);
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

static inline void write_source(const xs_element_t* const element,
                                FILE* const source, const options_t* const options)
{
  const xs_element_t* const child = element->Child;
  if(element->Child_Quantity)
  {
    for(uint32_t i = 0; i < element->Child_Quantity; i++)
    {
      if(child[i].Target.Type == EN_DYNAMIC)
      {
        fprintf(source, "\nstatic void* allocate_%s(uint32_t occurrence%s);\n", child[i].Name.String, options->Context);
      }
    }

    bool useArray = false;
    if(element->Name.String)
    {
      fprintf(source, "\nstatic const xs_element_t %s_descendant[] =\n{\n", element->Name.String);
      useArray = true;
    }
    else
    {
      fprintf(source, "\nconst xs_element_t %s_root[] =\n{\n", element->Child[0].Name.String);
    }

    uint32_t i = 0;
    do
    {
      char array[16] = "";
      if(useArray)
      {
         sprintf(array, "[%u]", i);
      }
      fprintf(source, "    %s.Name.String = \"%s\",\n", array, child[i].Name.String);
      fprintf(source, "    %s.Name.Length = %u,\n", array, child[i].Name.Length);
      fprintf(source, "    %s.MinOccur    = %u,\n", array, child[i].MinOccur);
      fprintf(source, "    %s.MaxOccur    = %u,\n", array, child[i].MaxOccur);
      if(options->Content_Callback)
      {
        fprintf(source, "    %s.Callback    = NULL,\n", array);
      }
      bool target_size = (child[i].MaxOccur > 1);
      write_target(&child[i].Target, array, element->Name.String, child[i].Name.String,
                   target_size, source);
      write_content(&child[i].Content, array, source);

      if(child[i].Attribute_Quantity)
      {
        fprintf(source, "    %s.Attribute_Quantity = %u,\n", array, child[i].Attribute_Quantity);
        fprintf(source, "    %s.Attribute          = %s_attribute,\n", array, child[i].Name.String);

      }
      if(child[i].Child_Quantity)
      {
        fprintf(source, "    %s.Child_Quantity = %u,\n", array, child[i].Child_Quantity);
        fprintf(source, "    %s.Child_Order    = %s,\n", array, xml_child_order[child[i].Child_Order]);
        fprintf(source, "    %s.Child          = %s_descendant,\n", array, child[i].Name.String);
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
        char array[16] = "";
        sprintf(array, "[%u]", i);
        write_target(&attribute[i].Target, array, element->Name.String,
                     attribute[i].Name.String, false, source);
        write_content(&attribute[i].Content, array, source);
      }
      fprintf(source, "    [%u].Use         = %s,\n", i, xml_attribute_use_type[attribute[i].Use]);

      if(++i == element->Attribute_Quantity)
      {
        break;
      }
      fprintf(source, "\n");
    }while(1);

    fprintf(source, "};\n");
  }
}

// Reorder all the complex xs_element_t
static inline void write_source_code(const xs_element_t* const element, FILE* const header,
                         FILE* const source, const options_t* const options)
{
  uint32_t quantity = element->Child_Quantity;
  const xs_element_t* const child = element->Child;
  while(quantity)
  {
    quantity--;
    if(child[quantity].Child_Quantity || child[quantity].Attribute_Quantity)
    {
      write_source_code(&child[quantity], header, source, options);
    }
  }

  if(element->Name.String)
  {
    write_header(element, header);
  }

  write_source(element, source, options);
}

static inline void write_functions(const xs_element_t* const element, FILE* const source, const options_t* const options)
{
  uint32_t quantity = element->Child_Quantity;
  const xs_element_t* const child = element->Child;
  while(quantity)
  {
    quantity--;
    if(child[quantity].Child_Quantity || child[quantity].Attribute_Quantity)
    {
      write_functions(&child[quantity], source, options);
    }
  }

  for(uint32_t i = 0; i < element->Child_Quantity; i++)
  {
    if(child[i].Target.Type == EN_DYNAMIC)
    {
      fprintf(source, "\nstatic void* allocate_%s(uint32_t occurrence%s)\n{\n", child[i].Name.String, options->Context);
      fprintf(source, "}\n");
    }
  }
}

void generate_xml_source(const xs_element_t* const root,  const options_t* const options)
{
  const char* const name = root->Child[0].Name.String;
  FILE* header_file = create_header_file(name);
  FILE* source_file = create_source_file(name);
  write_source_code(root, header_file, source_file, options);
  write_functions(root, source_file, options);
  close_header_file(header_file, name);
  fclose(source_file);
}

static inline void get_content_format(const xml_content_t* const content, char* format)
{
  switch(content->Type)
  {
  case EN_STRING_DYNAMIC:
  case EN_ENUM_STRING:
    strcpy(format, "%s");
    break;

  case EN_CHAR_ARRAY:
    sprintf(format, "%%%us", content->Facet.String.MaxLength);
    break;

  case EN_DECIMAL:
    strcpy(format, "%f");
    break;

  case EN_UNSIGNED:
  case EN_ENUM_UINT:
    strcpy(format, "%u");
    break;

  case EN_INTEGER:
  case EN_ENUM_INT:
  case EN_BOOL:
    strcpy(format, "%d");
    break;
  }
}

static inline void write_print_function(const xs_element_t* const element, FILE* const file,
                             char* space, char* variable)
{
  size_t waterMark = strlen(variable);
  if(element->Name.String)
  {
    strcat(variable, element->Name.String);
  }

  bool use_linked_list = false;
  if(element->MaxOccur > 1)
  {
    if(element->Target.Type == EN_DYNAMIC)
    {
      return;
    }
      fprintf(file, "\n%sfor(uint32_t i = 0; i < %u; i++)\n%s{\n", space,
                                                      element->MaxOccur, space);
      strcat(variable, "[i].");

    strcat(space, "    ");
  }
  else if(strlen(variable))
  {
    strcat(variable, ".");
  }

  if(element->Content.Type != EN_NO_XML_DATA_TYPE)
  {
    const char* const name = element->Name.String;
    char content[4] = {'\0'};
    get_content_format(&element->Content, content);

    if(use_linked_list)
    {
      fprintf(file, "%sprintf(\"%s: %s\\n\", %s->%s);\n",
              space, name, content, name, name);
    }
    else
    {
      fprintf(file, "%sprintf(\"%s: %s\\n\", %s%s);\n", space, name, content, variable, name);
    }
  }

  for(uint32_t i = 0; i < element->Attribute_Quantity; i++)
  {
    if(element->Attribute[i].Content.Type != EN_NO_XML_DATA_TYPE)
    {
      const char* const name = element->Attribute[i].Name.String;
      char content[4] = {'\0'};
      get_content_format(&element->Attribute[i].Content, content);

      if(use_linked_list)
      {
        const char* const element_name = element->Name.String;
        fprintf(file, "%sprintf(\"%s: %s\\n\", %s->%s);\n",
                space, name, content, element_name, name);
      }
      else
      {
        fprintf(file, "%sprintf(\"%s: %s\\n\", %s%s);\n", space, name, content, variable, name);
      }
    }
  }

  const xs_element_t* const child = element->Child;
  for(uint32_t i = 0; i < element->Child_Quantity; i++)
  {
    if(child[i].Attribute_Quantity || child[i].Child_Quantity)
    {
      write_print_function(&child[i], file, space, variable);
    }
    else if(child[i].Content.Type != EN_NO_XML_DATA_TYPE)
    {
      const char* const name = child[i].Name.String;
      char content[4];
      get_content_format(&child[i].Content, content);

      if(use_linked_list)
      {
        const char* const element_name = element->Name.String;
        fprintf(file, "%sprintf(\"%s: %s\\n\", %s->%s);\n",
                space, name, content, element_name, name);
      }
      else
      {
        fprintf(file, "%sprintf(\"%s: %s\\n\", %s%s);\n", space, name, content, variable, name);
      }
    }
  }

  if(element->MaxOccur > 1)
  {
    if(use_linked_list)
    {
      fprintf(file, "\n%s%s = %s->Next;\n", space, element->Name.String, element->Name.String);
    }
    uint32_t i = strlen(space) - 4;
    space[i] = '\0';
    fprintf(file, "%s}\n", space);
  }
  variable[waterMark] = '\0';
}

void generate_print_file(const xs_element_t* const root)
{
  const char* const name = root->Child[0].Name.String;
  char file_name[strlen(name) + sizeof("_print.c")] ;
  strcpy(file_name, name);
  strcat(file_name, "_print.c");
  FILE* file = fopen(file_name, "w");

  fprintf(file, "#include <stdlib.h>\n#include <stdbool.h>\n#include <stdio.h>\n\n#include \"%s.h\"\n\n", name);
  fprintf(file, "void print_%s(void)\n{\n", name);
  char space[512] = "    ";
  char sourceName[512] = "";

  write_print_function(root, file, space, sourceName);

  fprintf(file, "}\n");
  fclose(file);
}

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */
