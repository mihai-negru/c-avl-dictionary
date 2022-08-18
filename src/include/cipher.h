#ifndef CIPHER_H_
#define CIPHER_H_

#include "avl_dict.h"

#define BUFFER_LEN 1024
#define ELEMENT_DICT_LENGTH 5

typedef struct range_s {
	int32_t *index;
	size_t size;
	size_t capacity;	
} range_t;

void 			build_dict_from_file			(const char * const filename, dict_t *dict);

void 			encrypt							(const char * const infile, const char * const outfile, range_t *key);
void 			decrypt							(const char * const infile, const char * const outfile, range_t *key);
void 			print_key						(const char * const filename, range_t *key);

range_t* 		inorder_key_query				(dict_t *dict);
range_t* 		level_key_query					(dict_t *dict);
range_t* 		range_key_query					(dict_t *dict, const char *left, const char *right);

#endif /* CIPHER_H_ */