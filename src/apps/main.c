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

#include "libs/parse_xml.h"
#include "elements/xs_schema.h"

/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

int main(int argc, char *argv[])
{

  if(argc < 3)
  {
    printf("Please enter the schema file name and Target source file\n");
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

  FILE* cSource = fopen(argv[2], "w");
  if(cSource == NULL)
  {
    printf("Error in creating C source file\n");
  }

  xml_parse_result_t result = parse_xml(&xsd_root, schema, &pXsdData);
  if(result == XML_PARSE_SUCCESS)
  {
    printf("Parsing completed successfully\n");
  }
  else
  {
    printf("Failed to parse XML file\n");
  }

  free(schema);
  fclose(fSchema);
  fclose(cSource);

  return 0;
}


