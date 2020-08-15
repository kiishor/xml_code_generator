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
  fprintf(header, "\n#include \"libs/parse_xml.h\"\n");

  return header;
}

static inline void close_header_file(FILE* const header, const char* const name)
{
  fprintf(header, "\nextern %s_t %s;\n", name, name);
  fprintf(header, "\nextern const xs_element_t xml_root;\n");
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
  fprintf(source, "\n%s_t %s;\n\n", name, name);
  return source;
}

static inline void write_target(const target_address_t* const target,
                                uint32_t index,
                                const char* const parent,
                                const char* const element,
                                bool target_size,
                                FILE* const source)
{
  fprintf(source, "    [%u].Target.Type    = %s,\n", index, xs_target_address_type[target->Type]);
  switch(target->Type)
  {
  case EN_STATIC:
    if(parent)
    {
      fprintf(source, "    [%u].Target.Address = &%s.%s,\n", index, parent, element);
      if(target_size)
      {
        fprintf(source, "    [%u].Target.Size    = sizeof(%s_t),\n", index, element);
      }
    }
    else
    {
      fprintf(source, "    [%u].Target.Address = &%s,\n", index, element);
    }
    break;

  case EN_DYNAMIC:
    fprintf(source, "    [%u].Target.Allocate = allocate_%s,\n", index, element);
    break;

  case EN_RELATIVE:
    fprintf(source, "    [%u].Target.Offset  = offsetof(%s_t, %s),\n", index, parent, element);
    if(target_size)
    {
      fprintf(source, "    [%u].Target.Size    = sizeof(%s_t),\n", index, element);
    }
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

  bool use_linked_list = (((xsd_element_t*)element)->Allocate == LINKED_LIST_ALLOCATE);
  if(use_linked_list)
  {
    fprintf(header, "\ntypedef struct %s_t %s_t;\n", element->Name.String, element->Name.String);
    fprintf(header, "\nstruct %s_t\n{\n", element->Name.String);
  }
  else
  {
    fprintf(header, "\ntypedef struct\n{\n");
  }

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

  if(use_linked_list)
  {
    fprintf(header, "    %s_t* Next;\n", element->Name.String);
    fprintf(header, "};\n");
  }
  else
  {
    fprintf(header, "}%s_t;\n", element->Name.String);
  }
}

static inline void write_source(const xs_element_t* const element,
                                FILE* const source)
{
  const xs_element_t* const child = element->Child;
  if(element->Child_Quantity)
  {
    for(uint32_t i = 0; i < element->Child_Quantity; i++)
    {
      if(child[i].Target.Type == EN_DYNAMIC)
      {
        fprintf(source, "\nstatic void* allocate_%s(uint32_t occurrence, void** context);\n", child[i].Name.String);
      }
    }

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
      bool target_size = (child[i].MaxOccur > 1);
      write_target(&child[i].Target, i, element->Name.String, child[i].Name.String,
                   target_size, source);
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
        write_target(&attribute[i].Target, i, element->Name.String,
                     attribute[i].Name.String, false, source);
        write_content(&attribute[i].Content, i, source);
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
                         FILE* const source)
{
  uint32_t quantity = element->Child_Quantity;
  const xs_element_t* const child = element->Child;
  while(quantity)
  {
    quantity--;
    if(child[quantity].Child_Quantity || child[quantity].Attribute_Quantity)
    {
      write_source_code(&child[quantity], header, source);
    }
  }

  if(element->Name.String)
  {
    write_header(element, header);
  }

  write_source(element, source);
}

static inline void write_xml_root(FILE* const source)
{
  fprintf(source, "\nconst xs_element_t xml_root =\n{\n");
  fprintf(source, "    .Child_Quantity = 1,\n");
  fprintf(source, "    .Child_Type     = EN_CHOICE,\n");
  fprintf(source, "    .Child = root_descendant,\n");
  fprintf(source, "};\n");
}

static inline void write_functions(const xs_element_t* const element, FILE* const source)
{
  uint32_t quantity = element->Child_Quantity;
  const xs_element_t* const child = element->Child;
  while(quantity)
  {
    quantity--;
    if(child[quantity].Child_Quantity || child[quantity].Attribute_Quantity)
    {
      write_functions(&child[quantity], source);
    }
  }

  for(uint32_t i = 0; i < element->Child_Quantity; i++)
  {
    if(child[i].Target.Type == EN_DYNAMIC)
    {
      const char* const element_name = element->Name.String;
      const char* const child_name = child[i].Name.String;

      bool use_linked_list = (((xsd_element_t*)&child[i])->Allocate == LINKED_LIST_ALLOCATE);
      fprintf(source, "\nstatic void* allocate_%s(uint32_t occurrence, void** context)\n{\n", child[i].Name.String);
      if(use_linked_list)
      {
        fprintf(source, "    void* const target = calloc(sizeof(%s_t), 1);\n", child_name);
        fprintf(source, "    if(%s.%s == NULL)\n    {\n", element_name, child_name);
        fprintf(source, "        %s.%s = target;\n", element_name, child_name);
        fprintf(source, "        return target;\n");
        fprintf(source, "    }\n\n");
        fprintf(source, "    %s_t* node = %s.%s;\n", child_name, element_name, child_name);
        fprintf(source, "    while(node->Next)\n    {\n");
        fprintf(source, "        node = node->Next;\n");
        fprintf(source, "    }\n\n");
        fprintf(source, "    node->Next = target;\n");
        fprintf(source, "    return target;\n");
      }
      fprintf(source, "}\n");
    }
  }
}

void generate_xml_source(const xs_element_t* const root)
{
  const char* const name = root->Child[0].Name.String;
  FILE* header_file = create_header_file(name);
  FILE* source_file = create_source_file(name);
  write_source_code(root, header_file, source_file);
  write_xml_root(source_file);
  write_functions(root, source_file);
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
    use_linked_list = (((xsd_element_t*)element)->Allocate == LINKED_LIST_ALLOCATE);

    if(use_linked_list)
    {
      fprintf(file, "\n%sconst %s_t* %s = %s;\n", space, element->Name.String,
                                                element->Name.String, variable);
      fprintf(file, "%swhile(%s != NULL)\n%s{\n", space, element->Name.String, space);
      strcat(variable, "->");
    }
    else
    {
      fprintf(file, "\n%sfor(uint32_t i = 0; i < %u; i++)\n%s{\n", space,
                                                      element->MaxOccur, space);
      strcat(variable, "[i].");
    }

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
    fprintf(file, "%s}\n\n", space);
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

  fprintf(file, "\n}\n");
  fclose(file);
}

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */
