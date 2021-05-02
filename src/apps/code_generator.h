#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#define CONTEXT_STRING  ", void** context"

extern void generate_xml_source(const xs_element_t* const root, const options_t* const options);
extern void generate_print_file(const xs_element_t* const root);


#endif // CODE_GENERATOR_H

