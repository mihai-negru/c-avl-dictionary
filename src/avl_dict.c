#include "include/avl_dict.h"

#define MAX(a, b) (((a) >= (b))?(a):(b))

/**
 * @brief Function to create a dictionary object.
 * 
 * @param create_key Function to create a key object
 * @param destroy_key Function to destroy a key object
 * @param create_value Function to create a value object
 * @param destroy_value Function to destroy a value object
 * @param cmp Function two compare two keys
 * @return dict_t* pointer to an allocated dictionary object or NULL
 */
dict_t* create_dict(create_func create_key, destroy_func destroy_key, create_func create_value, destroy_func destroy_value, compare_func cmp) {
	/* Allocate the dictionary object */
	dict_t *new_dict = malloc(sizeof(*new_dict));

	/* Check if dictionary was allocated successfully */
	if (new_dict != NULL) {

		/* Set default values for dictionary object */
		new_dict->root = NULL;
		new_dict->create_key = create_key;
		new_dict->destroy_key = destroy_key;
		new_dict->create_value = create_value;
		new_dict->destroy_value = destroy_value;
		new_dict->cmp = cmp;
		new_dict->size = 0;
	}

	/* Return the new allocated dictionary or NULL */
	return new_dict;
}

/**
 * @brief Function to create a dictionary node object on the heap memory.
 * 
 * @param dict pointer to an allocated dictionary object
 * @param key pointer to the key location
 * @param value pointer to the value location
 * @return dict_node_t* pointer to allocated dictionary node object or NULL
 */
dict_node_t* create_dict_node(dict_t *dict, const void *key, const void *value) {
	/* Check if input data is valid */
	if (dict == NULL) {
		return NULL;
	}

	/* Allocate a new dictionary node on heap */
	dict_node_t *new_node = malloc(sizeof(*new_node));
	
	/* Check if new node was allocated successfully */
	if (new_node != NULL) {

		/* Create key and value */
		new_node->key = dict->create_key(key);
		new_node->value = dict->create_value(value);

		/* Set default node metadata */
		new_node->parent = new_node->right = new_node->left = NULL;
		new_node->next = new_node->prev = new_node->end = NULL;
		new_node->height = 1;
	}
	
	/* Return new node or NULL */
	return new_node;
}

/**
 * @brief Function to free loaded memory for a dictionary node object
 * 
 * @param dict pointer to an allocated dictionary object
 * @param delete_node the dictionary node object to delete
 */
void destroy_dict_node(dict_t *dict, dict_node_t *delete_node) {
	/* Check if input data is valid */
	if ((dict == NULL) || (delete_node == NULL)) {
		return;
	}

	/* Destroy the content */
	dict->destroy_key(delete_node->key);
	dict->destroy_value(delete_node->value);

	/* Destroy the node */
	free(delete_node);
}

/**
 * @brief Function to free all memory loaded for the dictionary object
 * 
 * @param dict pointer to an allocated dictionary object
 */
void destroy_dict(dict_t *dict) {
	/* Check if input data is valid */
	if (dict == NULL) {
		return;
	}

	dict_node_t *min_node = dict_minimum(dict->root);

	/* Iterate through all dictionary nodes */
	while (min_node != NULL) {
		dict_node_t *delete_node = min_node;

		min_node = min_node->next;

		/* Free node */
		destroy_dict_node(dict, delete_node);
	}

	/* Free dictionary */
	free(dict);
}

/**
 * @brief Function to check if a dictionary object is empty
 * 
 * @param dict pointer to an allocated dictionary object
 * @return uint8_t 1 if dictionary is empty, 0 otherwise
 */
uint8_t is_dict_empty(dict_t *dict) {
	if ((dict == NULL) || (dict->root != NULL)) {
		return 0;
	}

	return 1;
}

/**
 * @brief Function to search for a desired key starting from 
 * the "start_node".
 * 
 * @param dict pointer to an allocated dictionary object
 * @param start_node pointer to an allocated dictionary node to start searching
 * @param key pointer to a key location
 * @return dict_node_t* pointer to a dictionary node containing the key data
 */
dict_node_t* dict_search(dict_t *dict, dict_node_t *start_node, const void *key) {
	/* Check if the input data is valid */
	if ((dict == NULL) || (start_node == NULL)) {
		return NULL;
	}

	/* Find the desired dictionary node */
	while (start_node != NULL) {
		if (dict->cmp(start_node->key, key) >= 1) {
			start_node = start_node->left;
		} else if (dict->cmp(start_node->key, key) <= -1) {
			start_node = start_node->right;
		} else {
			return start_node;
		}
	}

	/* No node was found */
	return NULL;
}

/**
 * @brief Function to find the minimum dictionary node
 * starting from "start_node"
 * 
 * @param start_node pointer to an allocated dictionary node to start searching
 * @return dict_node_t* pointer to a dictionary node containing the min key data
 */
dict_node_t* dict_minimum(dict_node_t *start_node) {
	if (start_node != NULL) {

		/* Find the minimum node */
		while (start_node->left != NULL) {
			start_node = start_node->left;
		}
	}

	/* Return minimum node or NULL */
	return start_node;
}

/**
 * @brief Function to find the maximum dictionary node
 * starting from "start_node"
 * 
 * @param start_node pointer to an allocated dictionary node to start searching
 * @return dict_node_t* pointer to a dictionary node containing the max key data
 */
dict_node_t* dict_maximum(dict_node_t *start_node) {
	if (start_node != NULL) {

		/* Find the maximum node */
		while (start_node->right != NULL) {
			start_node = start_node->right;
		}
	}

	/* Return maximum node or NULL */
	return start_node;
}

/**
 * @brief Function to find inorder successor of the input dictionary node
 * 
 * @param start_node pointer to the node to find its successor
 * @return dict_node_t* pointer to inorder successor or NULL
 */
dict_node_t* dict_successor(dict_node_t *start_node) {
	/* Check if input data is valid */
	if (start_node == NULL) {
		return NULL;
	}
	
	/* Check if node has a right sub-tree */
	if (start_node->right != NULL) {
		return dict_minimum(start_node->right);
	}
	
	dict_node_t *parent_node = start_node->parent;

	/* Compute the successor */
	while ((parent_node != NULL) && (parent_node->right == start_node)) {
		start_node = parent_node;
		parent_node = parent_node->parent;
	}

	/* Return the successor or NULL */
	return parent_node;
}


/**
 * @brief Function to find inorder predecessor of the input dictionary node
 * 
 * @param start_node pointer to the node to find its predecessor
 * @return dict_node_t* pointer to inorder predecessor or NULL
 */
dict_node_t* dict_predecessor(dict_node_t *start_node) {
	/* Check if input data is valid */
	if (start_node == NULL) {
		return NULL;
	}
	
	/* Check if node has a left sub-tree */
	if (start_node->left != NULL) {
		return dict_maximum(start_node->left);
	}
	
	dict_node_t *parent_node = start_node->parent;

	/* Compute the successor */
	while ((parent_node != NULL) && (parent_node->left == start_node)) {
		start_node = parent_node;
		parent_node = parent_node->parent;
	}

	/* Return the predecessor or NULL */
	return parent_node;
}

/**
 * @brief Function to update the height of a rotated node
 * 
 * @param fix_node pointer to an allocated dictionary node to fix
 */
void update_height(dict_node_t *fix_node) {
	/* Check if input node is valid */
	if (fix_node != NULL) {
		int64_t left_height = 0;
		int64_t right_height = 0;
		
		/* Extract left height */
		if (fix_node->left != NULL) {
			left_height  = fix_node->left->height;
		}

		/* Extract right height */
		if (fix_node->right != NULL) {
			right_height = fix_node->right->height;
		}
		
		/* Update the node height */
		fix_node->height = MAX(left_height, right_height) + 1;
	}
}

/**
 * @brief Function to rotate to left the fix_node
 * 
 * @param dict pointer to an allocated dictionary object
 * @param fix_node pointer to an allocated dictionary node to rotate left
 */
void dict_rotate_left(dict_t *dict, dict_node_t *fix_node) {
	/* Check if rotation may happen */
	if ((dict == NULL) || (fix_node == NULL) || (fix_node->right == NULL)) {
		return;
	}

	dict_node_t *rotate_node = fix_node->right;

	/* Rotate to left */
	fix_node->right = rotate_node->left;

	if (rotate_node->left != NULL) {
		rotate_node->left->parent = fix_node;
	}

	/* Restore the links */
	rotate_node->left = fix_node;
	rotate_node->parent = fix_node->parent;
	fix_node->parent = rotate_node;

	/* Update parent links */
	if (rotate_node->parent != NULL) {
		if (dict->cmp(rotate_node->key, rotate_node->parent->key) >= 1) {
			rotate_node->parent->right = rotate_node;
		} else {
			rotate_node->parent->left = rotate_node;
		}
	} else {

		/* Update dictionary root */
		dict->root = rotate_node;
	}

	/* Update nodes height */
	update_height(fix_node);
	update_height(rotate_node);
}

/**
 * @brief Function to rotate to right the fix_node
 * 
 * @param dict pointer to an allocated dictionary object
 * @param fix_node pointer to an allocated dictionary node to rotate right
 */
void dict_rotate_right(dict_t *dict, dict_node_t *fix_node) {
	/* Check if rotation may happen */
	if ((dict == NULL) || (fix_node == NULL) || (fix_node->left == NULL)) {
		return;
	}

	dict_node_t *rotate_node = fix_node->left;

	/* Rotate to right */
	fix_node->left = rotate_node->right;

	if (rotate_node->right != NULL) {
		rotate_node->right->parent = fix_node;
	}

	/* Restore the links */
	rotate_node->right = fix_node;
	rotate_node->parent = fix_node->parent;
	fix_node->parent = rotate_node;

	/* Update parent links */
	if (rotate_node->parent != NULL) {
		if (dict->cmp(rotate_node->key, rotate_node->parent->key) >= 1) {
			rotate_node->parent->right = rotate_node;
		} else {
			rotate_node->parent->left = rotate_node;
		}
	} else {

		/* Update dictionary root */
		dict->root = rotate_node;
	}

	/* Update nodes height */
	update_height(fix_node);
	update_height(rotate_node);
}

/**
 * @brief Function to get the balance factor
 * 
 * @param fix_node pointer to node to find its balance factor
 * @return int64_t the balance factor
 */
int64_t dict_get_balance(dict_node_t *fix_node) {
	if (fix_node == NULL || ((fix_node->left == NULL) && (fix_node->right == NULL))) {
		return 0;
	}

	if ((fix_node->left != NULL) && (fix_node->right == NULL)) {
		return fix_node->left->height;
	}

	if ((fix_node->left == NULL) && (fix_node->right != NULL)) {
		return -fix_node->right->height;
	}

	return (fix_node->left->height - fix_node->right->height);
}

/**
 * @brief Function to fix a dictionary tree object after
 * insertion of one node
 * 
 * @param dict pointer to an allocated dictionary object
 * @param fix_node pointer to an allocated dictionary node to start fixing
 */
static void dict_insert_fix_up(dict_t *dict, dict_node_t *fix_node) {
	/* Check if fixing is needed */
	if ((dict == NULL) || (fix_node == NULL)) {
		return;
	}

	/* Fix every parent node */
	while (fix_node != NULL) {
		update_height(fix_node);

		/* Get balance factors */
		int64_t balance_fact = dict_get_balance(fix_node);
		int64_t balance_fact_left = dict_get_balance(fix_node->left);
		int64_t balance_fact_right = dict_get_balance(fix_node->right);

		/* Left-Left case */
		if ((balance_fact == 2) && (balance_fact_left == 1)) {
			dict_rotate_right(dict, fix_node);
		}

		/* Right-Right case */
		if ((balance_fact == -2) && (balance_fact_right == -1)) {
			dict_rotate_left(dict, fix_node);
		}

		/* Left-Right case */
		if ((balance_fact == 2) && (balance_fact_left == -1)) {
			dict_rotate_left(dict, fix_node->left);
			dict_rotate_right(dict, fix_node);
		}

		/* Right-Left case */
		if ((balance_fact == -2) && (balance_fact_right == 1)) {
			dict_rotate_right(dict, fix_node->right);
			dict_rotate_left(dict, fix_node);
		}

		/* Fix next parent */
		fix_node = fix_node->parent;
	}
}

/**
 * @brief Function to insert a new node into the dictionary obejct
 * 
 * @param dict pointer to an allocated dictionary object
 * @param key pointer to a key data
 * @param value pointer to a value data
 */
void dict_insert(dict_t *dict, const void *key, const void *value) {
	/* Check if input data is valid */
	if (dict == NULL) {
		return;
	}

	/* Check if dictionary is empty */
	if (is_dict_empty(dict) != 0) {
		dict_node_t *new_node = create_dict_node(dict, key, value);
		
		if (new_node != NULL) {
			new_node->end = new_node;
			dict->root = new_node;
		}

		dict->size = 1;
		
		return;
	}

	dict_node_t *iter = dict->root;

	dict_node_t *parent_iter = iter->parent;

	/* Find insert position */
	while (iter != NULL) {
		parent_iter = iter;

		if (dict->cmp(key, iter->key) > 0) {
			iter = iter->right;
		} else if (dict->cmp(key, iter->key) < 0) {
			iter = iter->left;
		} else {
			break;
		}
	}

	dict_node_t *new_node = create_dict_node(dict, key, value);

	if (new_node != NULL) {
		if (iter != NULL) {

			/* In the dictionary exists same (key, value) pair add to list*/

			new_node->next = iter->end->next;

			if (new_node->next != NULL) {
				new_node->next->prev = new_node;
			}

			iter->end->next = new_node;
			new_node->prev = iter->end;
			iter->end = new_node;
		} else {

			/* No dublicate was found so insert a new node */

			new_node->end = new_node;
			new_node->parent = parent_iter;

			if (dict->cmp(parent_iter->key, key) > 0) {
				parent_iter->left = new_node;
				new_node->next = parent_iter;
				new_node->prev = parent_iter->prev;

				if(parent_iter->prev != NULL) {
					parent_iter->prev->next = new_node;
				}

				parent_iter->prev = new_node;
			} else {
				parent_iter->right = new_node;
				new_node->prev = parent_iter;
				new_node->next = parent_iter->end->next;

				if (parent_iter->end->next != NULL) {
					parent_iter->end->next->prev = new_node;
				}
					
				parent_iter->end->next = new_node;
			}

			/* Fix the avl tree */
			dict_insert_fix_up(dict, parent_iter);
		}

		/* Increase the dictionary size */
		++(dict->size);
	}
}

/**
 * @brief Function to fix a dictionary tree object after
 * deletion of one node
 * 
 * @param dict pointer to an allocated dictionary object
 * @param fix_node pointer to an allocated dictionary node to start fixing
 */
static void dict_delete_fix_up(dict_t *dict, dict_node_t *fix_node) {
	/* Check if fixing is neededs */
	if ((dict == NULL) || (fix_node == NULL)) {
		return;
	}

	/* Fix parent nodes */
	while (fix_node != NULL) {
		update_height(fix_node);

		int64_t balance_fact = dict_get_balance(fix_node);
		int64_t balance_fact_left = dict_get_balance(fix_node->left);
		int64_t balance_fact_right = dict_get_balance(fix_node->right);

		/* Left-Left case */
		if ((balance_fact > 1) && (balance_fact_left >= 0)) {
			dict_rotate_right(dict, fix_node);
		}

		/* Right-Right case */
		if ((balance_fact < -1) && (balance_fact_right <= 0)) {
			dict_rotate_left(dict, fix_node);
		}

		/* Left-Right case */
		if ((balance_fact > 1) && (balance_fact_left < 0)) {
			dict_rotate_left(dict, fix_node->left);
			dict_rotate_right(dict, fix_node);
		}

		/* Right-Left case */
		if ((balance_fact < -1) && (balance_fact_right > 0)) {
			dict_rotate_right(dict, fix_node->right);
			dict_rotate_left(dict, fix_node);
		}

		/* Fix next parent node */
		fix_node = fix_node->parent;
	}
}

/**
 * @brief Subroutine function fo the dict_delete
 * 
 * @param dict pointer to an allocated dict object
 * @param start_node pointer to a dictionary node object to start deleting
 * @param key pointer to key location to delete from dictionary
 */
static void dict_delete_helper(dict_t *dict, dict_node_t *start_node, const void *key) {
	if ((dict == NULL) || (key == NULL)) {
		return;
	}

	dict_node_t *iter = dict_search(dict, start_node, key);

	if (iter == NULL) {
		return;
	}

	dict_node_t *parent_iter = iter->parent;

	if (iter->end == iter) {
		if (iter->left && iter->right) {
			dict_node_t *successor_iter = dict_minimum(iter->right);

			dict->destroy_key(iter->key);
			dict->destroy_value(iter->value);
			iter->key = dict->create_key(successor_iter->key);
			iter->value = dict->create_value(successor_iter->value);

			dict_delete_helper(dict, iter->right, successor_iter->key);
		} else {
			if (iter->left != NULL) {
				iter->left->parent = parent_iter;

				if (parent_iter != NULL) {
					if (parent_iter->right == iter) {
						parent_iter->right = iter->left;
						iter->next->prev = iter->prev;
						iter->prev->next = iter->next;
					} else {
						parent_iter->left = iter->left;
						parent_iter->prev = iter->prev;

						if (iter->prev != NULL) {
							iter->prev->next = parent_iter;
						}
					}
				} else {
					dict->root = iter->left;
					dict->root->prev = NULL;
					dict->root->next = NULL;
				}
				
			} else if (iter->right != NULL) {
				iter->right->parent = parent_iter;

				if (parent_iter != NULL) {

					if (parent_iter->right == iter) {
						parent_iter->right = iter->right;
						parent_iter->next = iter->next;

						if (iter->next != NULL) {
							iter->next->prev = parent_iter;
						}
					} else {
						parent_iter->left = iter->right;
						iter->prev->next = iter->next;
						iter->next->prev = iter->prev;
					}			
				} else {
					dict->root = iter->right;
					dict->root->prev = NULL;
					dict->root->next = NULL;
				}
			} else {
				if (parent_iter != NULL) {
					if (parent_iter->right == iter) {
						parent_iter->right = NULL;
						parent_iter->next = iter->next;

						if (iter->next != NULL) {
							iter->next->prev = iter->prev;
						}
					}
					else {
						parent_iter->left = NULL;
						parent_iter->prev = iter->prev;

						if (iter->prev != NULL) {
							iter->prev->next = iter->next;
						}
					}
				} else {
					dict->root = NULL;
				}
			}

			destroy_dict_node(dict, iter);

			dict_delete_fix_up(dict, parent_iter);

			--(dict->size);
		}
	} else {
		dict_node_t *delete_node = iter->end;
		delete_node->prev->next = delete_node->next;

		if (delete_node->next != NULL) {
			delete_node->next->prev = delete_node->prev;
		}

		iter->end = delete_node->prev;

		destroy_dict_node(dict, delete_node);

		--(dict->size);
	}
}


/**
 * @brief Function to delete one key node from the dictionary
 * 
 * @param dict pointer to an allocated dictionary object
 * @param key pointer to a key data location to delete from dictionary 
 */
void dict_delete(dict_t *dict, const void *key) {
	dict_delete_helper(dict, dict->root, key);
}