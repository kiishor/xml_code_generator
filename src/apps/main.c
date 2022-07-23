/**
 * \file
 * \brief   Tests the XML parser library

 * \author  Nandkishor Biradar
 * \date    14 September 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "apps/tree.h"
#include "parse_xml.h"
#include "apps/xsd.h"
#include "elements/xs_schema.h"
#include "elements/xs_element.h"
#include "code_generator.h"
#include "cargs.h"

/*
 *  ------------------------------ GLOBAL VARIABLES ------------------------------
 */

#define ADD_RESULT_CODE(code, description) [code] = #description,
const char* Result_Text[] = {XML_PARSER_RESULT};
#undef ADD_RESULT_CODE

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

void free_xsd_tree(const tree_t* node)
{
  if(node->Data)
  {
    free(node->Data);
  }

  if(node->Next)
  {
    free_xsd_tree(node->Next);
  }

  if(node->Descendant)
  {
    free_xsd_tree(node->Descendant);
  }

  free((void*)node);
}

//! Holds the configuration of command line arguments
static struct cag_option Cag_Options[] = {
  {
    .identifier = 'd',
    .access_letters = "dD",
    .access_name = "dynamic",
    .value_name = NULL,
    .description = "Uses allocate callback function to allow user to allocate memory dynamically of multiple occurrence of XML elements."
  },
  {
    .identifier = 'c',
    .access_letters = "cC",
    .access_name = "context",
    .value_name = NULL,
    .description = "Enable context"
  },
  {
    .identifier = 'b',
    .access_letters = "bB",
    .access_name = "callback",
    .value_name = NULL,
    .description = "Enable callback for each XML element."
  },
  {
    .identifier = 'h',
    .access_letters = "h",
    .access_name = "help",
    .description = "Shows the command help"
  }
};

int main(int argc, char *argv[])
{
  cag_option_context context;

   if(argc < 2)
    {
      printf("Usage: XML code generator [OPTION] input.xsd \n");
      cag_option_print(Cag_Options, CAG_ARRAY_SIZE(Cag_Options), stdout);
      return EXIT_FAILURE;
    }

  int exitCode = EXIT_SUCCESS;

  // Initialize options
  options_t options = {.Occurrence = ARRAY, .Context = "", .Content_Callback = false};
  cag_option_prepare(&context, Cag_Options, CAG_ARRAY_SIZE(Cag_Options), argc, argv);
  while(cag_option_fetch(&context))
  {
    switch(cag_option_get(&context))
    {
    case 'd':
      options.Occurrence = DYNAMIC;
      break;

    case 'c':
      options.Context = CONTEXT_STRING;
      break;

    case 'b':
      options.Content_Callback = true;
      break;

    default:
      printf("Unknown argument passed.\n");
      exitCode = EXIT_FAILURE;

    case 'h':
      printf("Usage: XML code generator [OPTION] input.xsd \n");
      cag_option_print(Cag_Options, CAG_ARRAY_SIZE(Cag_Options), stdout);
      return exitCode;
    }
  }

  FILE* fSchema = fopen(argv[argc - 1], "r");
  if(fSchema == NULL )
  {
    printf("Error in opening XML schema file: %s\n", argv[argc - 1]);
    return EXIT_FAILURE;
  }

  fseek(fSchema, 0, SEEK_END);
  size_t size = ftell(fSchema);
  fseek(fSchema, 0, SEEK_SET);

  char* schema = malloc(size);
  fread(schema, 1, size, fSchema);
  fclose(fSchema);

  // This holds all the schema elements in tree structure.
  tree_t schemaTree;
  memset(&schemaTree, 0, sizeof(schemaTree));
  tree_t* pXsdData = &schemaTree;

  xml_parse_result_t result = parse_xml(&xsd_root, schema, &schemaTree, &pXsdData);
  if(result == XML_PARSE_SUCCESS)
  {
    printf("Successfully parsed schema file\n");
    const xs_element_t* root = compile_xsd(pXsdData->Descendant, &options);
    generate_xml_source(root, &options);
    free_xsd_tree(schemaTree.Descendant);

#if GENERATE_PRINT_FUNCTION
    generate_print_file(root);
#endif // GENERATE_PRINT_FUNCTION
  }
  else
  {
    printf("%s\n", Result_Text[result]);
    exitCode = EXIT_FAILURE;
  }

  free(schema);
  return exitCode;
}

