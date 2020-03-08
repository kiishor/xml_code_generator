/**
 * \file
 * \brief   Library for parsing XML.

 * \author  Nandkishor Biradar
 * \date    19 August 2019
 */

/*
 *  ------------------------------ INCLUDE FILES ------------------------------
 */
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "libs/parse_xml.h"

/*
 *  -------------------------------- DEFINITION --------------------------------
 */


#define ASSERT_SOURCE(src, token, result, error)  \
{                                                 \
  if(src != token)                                \
  {                                               \
    result = error;                               \
    return NULL;                                  \
  }                                               \
}
#define ASSERT_PTR(src, result)    if(src == NULL)  return result;

#define ASSERT_RESULT(result, code) if(result != code) return NULL

/*
 *	--------------------------- FORWARD DECLARATION ---------------------------
 */

static inline const char* parse_parent_element(const xs_element_t* const parent,
                                               const char* source,
                                               void* target,
                                               xml_parse_result_t* const result,
                                               void** context);
/*
 *  ------------------------------ FUNCTION BODY ------------------------------
 */

static inline const char* skip_whitespace(const char* source)
{
  do
  {
    switch(*source)
    {
    case '\0':
      return NULL;

    case '\r':
    case '\n':
    case '\t':
    case ' ':
      source++;
      break;

    default:
      return source;
    }
  }while(1);
}

static inline const char* extract_element_tag(const char* source)
{
  while(1)
  {
    switch(*source)
    {
    case '\0':
      return NULL;

    case '\r':
    case '\n':
    case '\t':
    case ' ':

    case '>':
    case '/':
      return source;

    default:
      source++;
      break;
    }
  }
}

static inline const char* extract_attribute_tag(const char* source)
{
  while(1)
  {
    switch(*source)
    {
    case '\0':
      return NULL;

    case '\r':
    case '\n':
    case '\t':
    case ' ':

    case '=':
      return source;

    default:
      source++;
      break;
    }
  }
}

static inline void* get_target_address(const target_address_t* const address,
                                        void* target, uint32_t occurrence,
                                        void** context)
{
  switch(address->Type)
  {
  case EN_STATIC:
    return address->Address + (occurrence * address->Size);

  case EN_DYNAMIC:
    return address->Allocate(occurrence, context);

  case EN_RELATIVE:
    return target + (occurrence * address->Size) + address->Offset;

  default:
    return NULL;
  }
}

static inline const char* extract_content(const xml_content_t* const content, void* target, const char* source, char token)
{
  const char* const tag = source;
  source = strchr(source, token);
  ASSERT_PTR(source, NULL);
  if(target == NULL)
  {
    return ++source;
  }
  size_t length = source - tag;
  switch(content->Type)
  {
  case XS_STRING_STATIC:
    if((length < content->Facet.String.MinLength) &&
       (length > content->Facet.String.MaxLength))
    {
      return NULL;
    }
    memcpy(target, tag, length);
    ((char*)target)[length] = '\0';
    break;

  case XS_STRING_DYNAMIC:
    if((length < content->Facet.String.MinLength) &&
       (length > content->Facet.String.MaxLength))
    {
      return NULL;
    }

    char* data = malloc(length);
    ASSERT_PTR(data, NULL);
    memcpy(data, tag, length);
    data[length] = '\0';
    (*(uint32_t*)target) = ((uint32_t)(data));
    break;

  case XS_UNSIGNED:
  {
    uint32_t value = strtoul(tag, NULL, 10);
    if((value < content->Facet.Uint.MinValue) &&
       (value > content->Facet.Uint.MaxValue))
    {
      return NULL;
    }
    (*(uint32_t*)target) = value;
    break;

  }
  default:
    return NULL;
  }
  return ++source;
}

static inline const char* parse_attribute(const xs_attribute_t* const attribute,
                                          const char* source, void* target,
                                          void** context)
{
  source = skip_whitespace(source);
  ASSERT_PTR(source, NULL);

  if(*source++ != '=')
  {
    return NULL;
  }

  source = skip_whitespace(source);
  ASSERT_PTR(source, NULL);

  if(*source++ != '"')
  {
    return NULL;
  }

  return extract_content(&attribute->Content, get_target_address(&attribute->Target, target, 0, context), source, '"');
}

static inline xml_parse_result_t validate_attributes(const xs_element_t* const element,
                                                        const bool* const occurrence)
{
  for(uint32_t i = 0; i < element->Attribute_Quantity; i++)
  {
    if(!occurrence[i] && element->Attribute[i].Use == EN_REQUIRED)
    {
      return XML_ATTRIBUTE_NOT_FOUND;
    }
  }
  return XML_PARSE_SUCCESS;
}


static inline xml_parse_result_t validate_empty_element(const xs_element_t* const element)
{
  for(uint32_t i = 0; i < element->Child_Quantity; i++)
  {
    if(element->Child[i]->MinOccur > 0)
    {
      return XML_ELEMENT_MAX_OCCURRENCE_ERR;
    }
  }

  if(element->Content.Type != EN_NONE)
  {
    return XML_CONTENT_ERROR;
  }

  return XML_PARSE_SUCCESS;
}

#undef ASSERT_PTR
#define ASSERT_PTR(src, result, error)    \
  if(src == NULL)                         \
  {                                       \
    *result = error;                      \
    return NULL;                          \
  }

static inline const char* validate_element(const xs_element_t* const element,
                                           const char* source,
                                           xml_parse_result_t* const result)
{
  const char* const tag = source;
  source = extract_element_tag(source);
  ASSERT_PTR(source, result, XML_SYNTAX_ERROR);
  size_t length = source - tag;

  if((length != element->Name.Length) &&
     (strncmp(element->Name.String, tag, length) != 0))
  {
    *result = XML_END_TAG_NOT_FOUND;
    return NULL;
  }

  source = skip_whitespace(source);
  ASSERT_PTR(source, result, XML_SYNTAX_ERROR);
  ASSERT_SOURCE(*source++, '>', *result, XML_SYNTAX_ERROR);
  return source;
}


static inline const char* parse_element(const xs_element_t* const element,
                                               const char* source, void* target,
                                               xml_parse_result_t* const result,
                                               void** context)
{
  bool occurrence[element->Attribute_Quantity];
  uint32_t attribute_occurred = 0;

  for(uint32_t i = 0; i < element->Attribute_Quantity; i++)
  {
    occurrence[i] = false;
  }

  while(1)
  {
    source = skip_whitespace(source);
    ASSERT_PTR(source, result, XML_SOURCE_STRING_END_ERR);

    switch(*source)
    {
    case '/':
      source++;
      ASSERT_SOURCE(*source++, '>', *result, XML_SYNTAX_ERROR);
      *result = validate_attributes(element, occurrence);
      ASSERT_RESULT(*result, XML_PARSE_SUCCESS);
      *result = validate_empty_element(element);
      ASSERT_RESULT(*result, XML_PARSE_SUCCESS);
      return source;

    case '>':
      *result = validate_attributes(element, occurrence);
      ASSERT_RESULT(*result, XML_PARSE_SUCCESS);
      source++;
      if(element->Child_Quantity > 0)
      {
        source = parse_parent_element(element, source, target, result, context);
        if(source == NULL)
        {
          return NULL;
        }
      }
      else if(element->Content.Type != EN_NONE)
      {
        source = extract_content(&element->Content, target, source, '<');
        ASSERT_PTR(source, result, XML_CONTENT_ERROR);
        ASSERT_SOURCE(*source++, '/', *result, XML_SYNTAX_ERROR);
      }
      else
      {
        source = skip_whitespace(source);
        ASSERT_PTR(source, result, XML_SYNTAX_ERROR);
        ASSERT_SOURCE(*source++, '<', *result, XML_SYNTAX_ERROR);
        ASSERT_SOURCE(*source++, '/', *result, XML_SYNTAX_ERROR);
      }
      return validate_element(element, source, result);
    }

    if(attribute_occurred == element->Attribute_Quantity)
    {
      *result = XML_SYNTAX_ERROR;
      return NULL;
    }

    const char* const tag = source;
    source = extract_attribute_tag(source);
    ASSERT_PTR(source, result, XML_SOURCE_STRING_END_ERR);
    size_t length = source - tag;
    uint32_t i = 0;
    while(1)
    {
      if((length == element->Attribute[i].Name.Length) &&
         (strncmp(tag, element->Attribute[i].Name.String, length) == 0))
      {
        if(occurrence[i])
        {
          *result = XML_DUPLICATE_ATTRIBUTE;
          return NULL;
        }

        occurrence[i] = true;
        source = parse_attribute(&element->Attribute[i], source, target, context);
        ASSERT_PTR(source, result, XML_SYNTAX_ERROR);
        attribute_occurred++;
        break;
      }

      if(++i == element->Attribute_Quantity)
      {
        *result = XML_ATTRIBUTE_NOT_FOUND;
        return NULL;
      }
    }
  }
}

static inline const char* parse_parent_element(const xs_element_t* const parent,
                                               const char* source,
                                               void* target,
                                               xml_parse_result_t* const result,
                                               void** context)
{

  uint32_t occurrence[parent->Child_Quantity];
  uint32_t element_index = 0;

  for(uint32_t i = 0; i < parent->Child_Quantity; i++)
  {
    occurrence[i] = 0;
  }

  while(1)
  {
    source = skip_whitespace(source);
    if(source == NULL)
    {
      *result = XML_SOURCE_STRING_END_ERR;
      return NULL;
    }

    ASSERT_SOURCE(*source++, '<', *result, XML_INVALID_START_TOKEN_ERR);

    switch(*source)
    {
    case '?':
    case '!':
      source = strchr(source, '>');
      ASSERT_PTR(source, result, XML_SOURCE_STRING_END_ERR);
      source++;
      continue;

    case '/':
      for(uint32_t i = 0; i < parent->Child_Quantity; i++)
      {
        if(occurrence[i] < parent->Child[i]->MinOccur)
        {
          *result = XML_ELEMENT_MIN_OCCURRENCE_ERR;
          return NULL;
        }
      }
      *result = XML_PARSE_SUCCESS;
      return ++source;
    }

    const char* const tag = source;
    source = extract_element_tag(source);
    ASSERT_PTR(source, result, XML_SOURCE_STRING_END_ERR);
    size_t length = source - tag;

    switch(parent->Child_Type)
    {
    case EN_CHOICE:
      element_index = 0;
      while(1)
      {
        if((length == parent->Child[element_index]->Name.Length) &&
          (strncmp(tag, parent->Child[element_index]->Name.String, length) == 0))
        {
          break;
        }

        if(++element_index == parent->Child_Quantity)
        {
          *result = XML_ELEMENT_NOT_FOUND_ERR;
          return NULL;
        }
      }
      break;

    case EN_SEQUENCE:
      while(1)
      {
        if((length == parent->Child[element_index]->Name.Length) &&
          (strncmp(tag, parent->Child[element_index]->Name.String, length) == 0))
        {
          break;
        }

        if(occurrence[element_index] < parent->Child[element_index]->MinOccur)
        {
          *result = XML_ELEMENT_MIN_OCCURRENCE_ERR;
          return NULL;
        }

        if(++element_index == parent->Child_Quantity)
        {
          *result = XML_ELEMENT_NOT_FOUND_ERR;
          return NULL;
        }
      }
      break;
    }
    const xs_element_t* const element = parent->Child[element_index];
    if(++occurrence[element_index] > element->MaxOccur)
    {
      *result = XML_ELEMENT_MAX_OCCURRENCE_ERR;
      return NULL;
    }

    target = get_target_address(&element->Target, target, occurrence[element_index], context);

    source = parse_element(element, source, target, result, context);
    if(source == NULL)
    {
      return NULL;
    }
    element->Callback(occurrence[element_index], target, context);
    if(parent->Name.String == NULL)
    {
      *result = XML_PARSE_SUCCESS;
      return NULL;
    }
  }
}

xml_parse_result_t parse_xml(const xs_element_t* root, const char* source, void** context)
{
  xml_parse_result_t result;
  parse_parent_element(root, source, NULL, &result, context);
  return result;
}
