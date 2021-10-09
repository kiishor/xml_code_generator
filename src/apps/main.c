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
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

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

    case 'h':
      printf("Usage: XML code generator [OPTION] input.xsd \n");
      cag_option_print(Cag_Options, CAG_ARRAY_SIZE(Cag_Options), stdout);
      return EXIT_SUCCESS;
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

  tree_t SchemaTree;
  tree_t* pXsdData = &SchemaTree;

  xml_parse_result_t result = parse_xml(&xsd_root, schema, NULL, &pXsdData);
  if(result == XML_PARSE_SUCCESS)
  {
    printf("Parsing completed successfully\n");
    const xs_element_t* root = compile_xsd(pXsdData->Descendant, &options);
    generate_xml_source(root, &options);
    generate_print_file(root);
  }
  else
  {
    printf("Failed to parse XML file: %d\n", result);
  }

  free(schema);
  return EXIT_SUCCESS;
}

