#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

extern FILE* create_header_file(const char* name);
extern void close_header_file(FILE* header, const char* name);
extern FILE* create_source_file(const char* name);
extern void close_source_file(FILE* source, const char* name);
extern void generate_xml_source(const xs_element_t* const element, FILE* header,
                                FILE* source);

#endif // CODE_GENERATOR_H

