#ifndef DICT_H_
#define DICT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

/**
 * @brief Definitions of some basic functions
 * 
 */
typedef void* 		(*create_func)		(const void *elem);
typedef void 		(*destroy_func)		(void *elem);
typedef int32_t 	(*compare_func)		(const void *elem1, const void *elem2);

/**
 * @brief Definition of a dictionary node object
 * 
 */
typedef struct dict_node_s{
	void *key; 								/* Pointer to key location */
	void *value;							/* Pointer to value location */
	struct dict_node_s *parent;				/* Pointer to parent node */
	struct dict_node_s *left;  				/* Pointer to left child */
	struct dict_node_s *right; 				/* Pointer to right child */
	struct dict_node_s *next; 				/* Pointer to next node */
	struct dict_node_s *prev; 				/* Pointer to previous node */
	struct dict_node_s *end; 				/* Pointer to end node */
	int64_t height;							/* Height of the node */
} dict_node_t;

/**
 * @brief Definition of a dictionary object
 * 
 */
typedef struct dict_s{
	dict_node_t *root;						/* Pointer to first node in the dictionary */
	create_func create_key;  				/* Function to create a key object */
	destroy_func destroy_key;				/* Function to destroy a key object */
	create_func create_value; 				/* Function to create a value object */
	destroy_func destroy_value; 			/* Function to destroy a value object */
	compare_func cmp; 						/* Function two compare two keys */
	size_t size;							/* The number of nodes in the dictionary */
} dict_t;


dict_t* 			create_dict					(create_func create_key, destroy_func destroy_key, create_func create_value, destroy_func destroy_value, compare_func cmp);
dict_node_t* 		create_dict_node			(dict_t *dict, const void *key, const void *value);
void 				destroy_dict_node			(dict_t *dict, dict_node_t *delete_node);
void 				destroy_dict				(dict_t *dict);

uint8_t 			is_dict_empty				(dict_t *dict);

dict_node_t* 		dict_search					(dict_t *dict, dict_node_t *start_node, const void *key);
dict_node_t* 		dict_minimum				(dict_node_t *start_node);
dict_node_t* 		dict_maximum				(dict_node_t *start_node);
dict_node_t* 		dict_successor				(dict_node_t *start_node);
dict_node_t* 		dict_predecessor			(dict_node_t *start_node);

void 				update_height				(dict_node_t *fix_node);
void 				dict_rotate_left			(dict_t *dict, dict_node_t *fix_node);
void 				dict_rotate_right			(dict_t *dict, dict_node_t *fix_node);
int64_t 			dict_get_balance			(dict_node_t *fix_node);

void 				dict_insert					(dict_t *dict, const void *key, const void *value);
void 				dict_delete					(dict_t *dict, const void *key);

#endif /* DICT_H_ */