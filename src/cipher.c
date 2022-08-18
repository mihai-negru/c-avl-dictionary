#include "include/cipher.h"

#define INIT_LEN 1
#define LETTER_LEN 26
#define WORD_SEPARATOR ",.? \n\r"

/**
 * @brief Function to build a dictionary from an input file
 * 
 * @param filename name of the file to open
 * @param dict a pointer to an allocated dictionary object
 */
void build_dict_from_file(const char * const filename, dict_t *dict) {
	/* Check if input data is valid */
	if((filename == NULL) || (dict == NULL)) {
		return;
	}

	FILE *fin = NULL;

	if ((fin = fopen(filename, "r")) == NULL) {
		printf("Error on openning the file for reading\n");
		return;
	}

	char *line_buff = malloc(BUFFER_LEN);
	int32_t start_of_new_elem = 0;

	/* Read all the lines from the file */
	while (fgets(line_buff, BUFFER_LEN, fin) != NULL) {
		char *word = NULL, *save_ptr = NULL;

		word = __strtok_r(line_buff, WORD_SEPARATOR, &save_ptr);

		/* Extract all words from line and insert in dictionary */
		while (word != NULL) {
			dict_insert(dict, word, &start_of_new_elem);

			start_of_new_elem += (int32_t)strlen(word);

			word = __strtok_r(NULL, WORD_SEPARATOR, &save_ptr);
		}
	}

	free(line_buff);

	fclose(fin);
}

/**
 * @brief Function to encrypt a text message
 * 
 * @param infile input file to read message
 * @param outfile output file to write message
 * @param key pointer to a range_t object representing ecryption key
 */
void encrypt(const char * const infile, const char * const outfile, range_t *key) {
	FILE *fin  = fopen(infile,  "r");
	FILE *fout = fopen(outfile, "w");

	if (fin == NULL) {
		return;
	}

	char *buff = malloc(BUFFER_LEN + 1);
	char c;

	size_t idx = 0;

	while (fgets(buff, BUFFER_LEN, fin) != NULL) {
		size_t buff_size = strlen(buff);

		for (size_t iter = 0; iter < buff_size; ++iter) {
			if ((buff[iter] != ' ') && (buff[iter] != '\n') && (buff[iter] != '\r')) {
				c = ((toupper(buff[iter]) - 'A') + key->index[idx] % 26) % 26 + 'A';
				++idx;
			} else {
			 	c = buff[iter];
			}

			fprintf(fout, "%c", c);

			if (idx == key->size) {
				idx = 0;
			}
		}
	}

	free(buff);
	fclose(fin);
	fclose(fout);
}

/**
 * @brief Function to decrypt a text message
 * 
 * @param infile input file to read message
 * @param outfile output file to write message
 * @param key pointer to a range_t object representing ecryption key
 */
void decrypt(const char * const infile, const char * const outfile, range_t *key) {
 	
	FILE *fin  = fopen(infile,  "r");
	FILE *fout = fopen(outfile, "w");

	if (fin == NULL) {
		return;
	}

	char *buff = malloc(BUFFER_LEN + 1);
	char c;

	size_t idx = 0;

	while (fgets(buff, BUFFER_LEN, fin) != NULL) {
		size_t buff_size = strlen(buff);

		for (size_t iter = 0; iter < buff_size; ++iter) {
			if ((buff[iter] != ' ') && (buff[iter] != '\n') && (buff[iter] != '\r')) {
				c = ((toupper(buff[iter]) - 'A') - (key->index[idx] % 26) + 26) % 26 + 'A';
				++idx;
			} else {
			 	c = buff[iter];
			}

			fprintf(fout, "%c", c);

			if (idx == key->size) {
				idx = 0;
			}
		}
	}

	free(buff);
	fclose(fin);
	fclose(fout);
}

/**
 * @brief Function to print an encrypted key
 * 
 * @param filename name of the file to write key
 * @param key range_t object key to print
 */
void print_key(const char * const filename, range_t *key) {
	FILE *fin = fopen(filename, "w");

	if (key == NULL) {
		fprintf(fin, "No key provided!\n");
		fclose(fin);

		return;
	}

	fprintf(fin, "Decryption key of length %lu is:\n", key->size);

	for (size_t iter = 0; iter < key->size; ++iter) {
		fprintf(fin, "%d ", key->index[iter] % 26);

		if ((iter + 1) % 10 == 0) {
			fprintf(fin, "\n");
		}
	}

	fclose(fin);
}

/**
 * @brief Function to find the inorder key range
 * 
 * @param dict pointer to an allocated dictionary object
 * @return range_t* the inorder key range
 */
range_t* inorder_key_query(dict_t *dict) {
	if ((dict == NULL) || (dict->root == NULL)) {
		return NULL;
	}

	range_t *new_range = malloc(sizeof(*new_range));

	if (new_range != NULL) {
		new_range->capacity = INIT_LEN;
		new_range->size = 0;

		new_range->index = malloc(sizeof(*new_range->index) * new_range->capacity);

		if (new_range->index != NULL) {
			dict_node_t *min_node = dict_minimum(dict->root);

			while (min_node != NULL) {
				if (new_range->size == new_range->capacity) {
					new_range->capacity *= 2;
					
					new_range->index = realloc(new_range->index, sizeof(*new_range->index) * new_range->capacity);
					
					if (new_range->index == NULL) {
						printf("Error at realloc\n");
						exit(EXIT_FAILURE);
					}
				}

				new_range->index[new_range->size] = (*(int32_t *)min_node->value) % LETTER_LEN;

				++(new_range->size);

				min_node = min_node->next;
			}
		}
	}

	return new_range;
}

/**
 * @brief Function to find how many nodes with the same key 
 * exists in the dictionary.
 * 
 * @param dict pointer to an allocated dictionary object
 * @param start_node node to start counting the same nodes
 * @return int32_t the frequency of the start node
 */
static int32_t dict_node_freq(dict_t *dict, dict_node_t *start_node) {
	if (start_node == NULL) {
		return 0;
	}

	dict_node_t *iter = start_node;

	int32_t node_freq = 0;

	while ((iter != NULL) && (dict->cmp(iter->key, start_node->key) == 0)) {
		++node_freq;
		iter = iter->next;
	}

	return node_freq;
}

/**
 * @brief Function to find the dictionary node with the maximum frequency
 * 
 * @param dict pointer to an allocated dictionary object
 * @return dict_node_t* the maximum frequency node or NULL
 */
static dict_node_t* get_dict_max_freq_node(dict_t *dict) {
	if ((dict == NULL) || (dict->root == NULL)) {
		return NULL;
	}

	dict_node_t *min_node = dict_minimum(dict->root);

	dict_node_t *max_freq_node = NULL;

	int32_t max_freq = -1;

	while (min_node != NULL) {
		int32_t node_freq = dict_node_freq(dict, min_node);

		if (node_freq >= max_freq) {
			max_freq_node = min_node;
			max_freq = node_freq;
		}

		min_node = min_node->end->next;
	}

	return max_freq_node;
}

/**
 * @brief Function to get the level of a key node
 * 
 * @param dict pointer to an allocated dictionary object
 * @param key pointer to key location to find its level
 * @return int32_t level of the key node object
 */
int32_t get_dict_node_level(dict_t *dict, const void *key) {
	if ((dict == NULL) || (dict->root == NULL)) {
		return 0;
	}

	int32_t node_level = 1;

	dict_node_t *iter = dict->root;

	while (iter != NULL) {
		if (dict->cmp(iter->key, key) > 0) {
			++node_level;
			iter = iter->left;
		} else if (dict->cmp(iter->key, key) < 0) {
			++node_level;
			iter = iter->right;
		} else {
			return node_level;
		}
	}

	return 0;
}

/**
 * @brief Function to find the level key range
 * 
 * @param dict pointer to an allocated dictionary object
 * @return range_t* the inorder key range
 */
range_t* level_key_query(dict_t *dict) {
	if ((dict == NULL) || (dict->root == NULL)) {
		return NULL;
	}

	range_t *new_range = malloc(sizeof(*new_range));

	if (new_range != NULL) {
		new_range->capacity = INIT_LEN;
		new_range->size = 0;

		new_range->index = malloc(sizeof(*new_range->index) * new_range->capacity);

		if (new_range->index != NULL) {
			int32_t level_of_max_node = get_dict_node_level(dict, get_dict_max_freq_node(dict)->key);

			dict_node_t *min_node = dict_minimum(dict->root);

			while (min_node != NULL) {
				if (get_dict_node_level(dict, min_node->key) != level_of_max_node) {
					min_node = min_node->end->next;
				} else {
					if (new_range->size == new_range->capacity) {
						new_range->capacity *= 2;
				
						new_range->index = realloc(new_range->index, sizeof(*new_range->index) * new_range->capacity);
					
						if (new_range->index == NULL) {
							printf("Error at realloc\n");
							exit(EXIT_FAILURE);
						}
					}

					new_range->index[new_range->size] = (*(int32_t *)min_node->value) % LETTER_LEN;

					++(new_range->size);
					
					min_node = min_node->next;
				}
			}
		}
	}

	return new_range;
}

/**
 * @brief Function to find the range key range
 * 
 * @param dict pointer to an allocated dictionary object
 * @return range_t* the inorder key range
 */
range_t* range_key_query(dict_t *dict, const char *left, const char *right) {
	if ((dict == NULL) || (dict->root == NULL)) {
		return NULL;
	}

	range_t *new_range = malloc(sizeof(*new_range));

	if (new_range != NULL) {
		new_range->capacity = INIT_LEN;
		new_range->size = 0;

		new_range->index = malloc(sizeof(*new_range->index) * new_range->capacity);

		if (new_range->index != NULL) {
			dict_node_t *min_node = dict_minimum(dict->root);

			while ((min_node != NULL) && (dict->cmp(min_node->key, left) < 0)) {
				min_node = min_node->end->next;
			}
			
			while ((min_node != NULL) && (dict->cmp(min_node->key, right) <= 0)) {
				if (new_range->size == new_range->capacity) {
					new_range->capacity *= 2;

					new_range->index = realloc(new_range->index, sizeof(*new_range->index) * new_range->capacity);
					
					if (new_range->index == NULL) {
						printf("Error ar realloc\n");
						exit(EXIT_FAILURE);
					}
				}

				new_range->index[new_range->size] = (*(int32_t *)min_node->value) % LETTER_LEN;

				++(new_range->size);

				min_node = min_node->next;
			}
		}
	}

	return new_range;
}
