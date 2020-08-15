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
#include "elements/xs_schema.h"
#include "apps/xsd.h"
#include "code_generator.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    printf("Please enter the schema file name\n");
    return 1;
  }

  FILE* fSchema = fopen(argv[1], "r");
  if(fSchema == NULL )
  {
    printf("Error in opening XML schema file\n");
    return 2;
  }

  fseek(fSchema, 0, SEEK_END);
  size_t size = ftell(fSchema);
  fseek(fSchema, 0, SEEK_SET);

  char* schema = malloc(size);
  fread(schema, 1, size, fSchema);
  fclose(fSchema);

  xml_parse_result_t result = parse_xml(&xsd_root, schema, (void**)&pXsdData);
  if(result == XML_PARSE_SUCCESS)
  {
    printf("Parsing completed successfully\n");
    options_t options = {.Occurrence = UNSPECIFIED};
    const xsd_element_t* root = compile_xsd(pXsdData->Descendant, &options);
    generate_xml_source((xs_element_t*)root);
    generate_print_file((xs_element_t*)root);
  }
  else
  {
    printf("Failed to parse XML file\n");
  }

  free(schema);

  return 0;
}


