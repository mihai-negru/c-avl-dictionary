#include "include/avl_dict.h"
#include "include/cipher.h"

#define ASSERT(fout, cond, msg) if (!(cond)) { failed(fout, msg); return; } else passed(fout, msg);

void failed(FILE *fout, const char * const msg) {
	fprintf(fout, "%s ...... failed\n", msg);
	fclose(fout);
}

void passed(FILE *fout, const char * const msg) {
	fprintf(fout, "%s ...... passed\n", msg);
}

void* create_long(const void *value) {
	long *l = malloc(sizeof(*l));
	*l = *(long *)value;
	return l;
}

void destroy_long(void *value) {
	free(value);
}

int32_t compare_long(const void *a, const void *b) {
	if(*(long*)a < *(long*)b) return -1;
	if(*(long*)a > *(long*)b) return  1;
	return 0;
}

void* create_str_element(const void *str){
	char *elem = malloc(ELEMENT_DICT_LENGTH + 1);
	strncpy(elem, (const char *)str, ELEMENT_DICT_LENGTH);
	elem[ELEMENT_DICT_LENGTH] = '\0';
	return elem;
}

void destroy_str_element(void *elem){
	free((char *)elem);
}

void* create_index_info(const void *index){
	int32_t *idx = malloc(sizeof(*idx));
	*idx = *(int*)index;
	return idx;
}

void destroy_index_info(void *index){
	free(index);
}

int32_t compare_str(const void *str1, const void *str2) {
	if (strncmp((const char *)str1,(const char *)str2, ELEMENT_DICT_LENGTH) > 0)
		return 1;
	else if (strncmp((const char *)str1,(const char *)str2, ELEMENT_DICT_LENGTH) < 0)
		return -1;
	
	return 0;
}

void print_dot_aux(dict_node_t *node, FILE *fout, uint8_t type) {
	if (node->left != NULL) {
		if (type == 1) {
			fprintf(fout, "    %d -> %d;\n", *((int32_t *)node->key), *((int32_t *)node->left->key));
		} else {
			fprintf(fout, "    %s -> %s;\n", (const char *)node->key, (const char *)node->left->key);
		}

		print_dot_aux(node->left, fout, type);
	}

	if (node->right != NULL) {
		if (type == 1) {
			fprintf(fout, "    %d -> %d;\n", *((int32_t *)node->key), *((int32_t *)node->right->key));
		} else {
			fprintf(fout, "    %s -> %s;\n", (const char *)node->key, (const char *)node->right->key);
		}

		print_dot_aux(node->right, fout, type);
	}
}

void print_dot(dict_node_t *root, FILE *fout, uint8_t type) {
	fprintf(fout, "digraph BST {\n");

	if (type == 1) {
		fprintf(fout, "    node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=green];\n");
	} else {
		fprintf(fout, "    node [fontname=\"Arial\", shape=circle, style=filled, fillcolor=yellow];\n");
	}

	if (root != NULL) {
		fprintf(fout, "\n");
	} else if ((root->right != NULL) && (root->left != NULL)) {
		if (type == 1) {
			fprintf(fout, "    %d;\n", *((int32_t *)root->key));
		} else {
			fprintf(fout, "    %s;\n", ((const char *)root->key));
		}
	} else {
		print_dot_aux(root, fout, type);
	}

	fprintf(fout, "}\n");
}

void create_simple_tree(dict_t **dict) {
	long values[] = {5, 3, 2, 4, 7, 6, 8};

	dict_node_t *node = create_dict_node((*dict), values, values);

	if (node == NULL) {
		return;
	}

	(*dict)->root = node;

	node = create_dict_node((*dict), values + 1, values + 1);
	(*dict)->root->left = node;
	node->parent = (*dict)->root;

	node = create_dict_node((*dict), values + 2, values + 2);
	(*dict)->root->left->left = node;
	node->parent = (*dict)->root->left;

	node = create_dict_node((*dict), values + 3, values + 3);
	(*dict)->root->left->right = node;
	node->parent = (*dict)->root->left;

	node = create_dict_node((*dict), values + 4, values + 4);
	(*dict)->root->right = node;
	node->parent = (*dict)->root;

	node = create_dict_node((*dict), values + 5, values + 5);
	(*dict)->root->right->left = node;
	node->parent = (*dict)->root->right;

	node = create_dict_node((*dict), values + 6, values + 6);
	(*dict)->root->right->right = node;
	node->parent = (*dict)->root->right;
}

void test_init(dict_t **dict) {
	FILE *fout = fopen("outputs/output_dict_init.out", "w");

	if (fout == NULL) {
		printf("Error opening file!\n");
		return;
	}

	(*dict) = create_dict(create_long, destroy_long, create_long, destroy_long, compare_long);

	ASSERT(fout, (*dict) != NULL, "create_dict-01");
	ASSERT(fout, (*dict)->create_key != NULL, "create_dict-02");
	ASSERT(fout, (*dict)->create_value != NULL, "create_dict-03");
	ASSERT(fout, (*dict)->destroy_key != NULL, "create_dict-04");
	ASSERT(fout, (*dict)->destroy_value != NULL, "create_dict-05");
	ASSERT(fout, (*dict)->cmp != NULL, "create_dict-06");
	ASSERT(fout, (*dict)->root == NULL, "create_dict-07");
	ASSERT(fout, (*dict)->size == 0, "create_dict-08");
	ASSERT(fout, is_dict_empty((*dict)) == 1, "is_dict_empty-01");

	fprintf(fout, "\nAll tests for init passed!\n");

	fclose(fout);
}

void test_search(dict_t **dict) {

	FILE *fout = fopen("outputs/output_dict_search.out", "w");

	if (fout == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if (*dict == NULL) {
		fprintf(fout, "Empty dict passed!\n");
		fclose(fout);
		return;
	}

	long value = 4;
	ASSERT(fout, (dict_search((*dict), (*dict)->root, &value) != NULL) == 1, "dict_search-01");

	value = 10;
	ASSERT(fout, (dict_search((*dict), (*dict)->root, &value) != NULL) == 0, "dict_search-02");

	value = 5;
	ASSERT(fout, dict_search((*dict), (*dict)->root, &value)->left == (*dict)->root->left, "dict_search-04");
	ASSERT(fout, dict_search((*dict), (*dict)->root, &value)->right == (*dict)->root->right, "dict_search-05");

	fprintf(fout, "\nAll tests for dict_search passed!\n");
	fclose(fout);
}

void test_minmax(dict_t **dict) {
	FILE *fout = fopen("outputs/output_dict_minmax.out", "w");

	if (fout == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if ((*dict == NULL) || ((*dict)->root == NULL)) {
		fprintf(fout, "Empty dict passed!\n");
		fclose(fout);
		return;
	}

	ASSERT(fout, dict_minimum((*dict)->root) != NULL, "dict_minmax-01");
	ASSERT(fout, *((long*)dict_minimum((*dict)->root)->key) == 2l, "dict_minmax-02");
	ASSERT(fout, dict_minimum((*dict)->root)->left == NULL, "dict_minmax-03");
	ASSERT(fout, *((long*)dict_minimum((*dict)->root->right)->key) == 6l, "dict_minmax-04");

	ASSERT(fout, dict_maximum((*dict)->root) != NULL, "dict_minmax-05");
	ASSERT(fout, *((long*)dict_maximum((*dict)->root)->key) == 8l, "dict_minmax-06");
	ASSERT(fout, dict_maximum((*dict)->root)->right == NULL, "dict_minmax-07");
	ASSERT(fout, *((long*)dict_maximum((*dict)->root->left)->key) == 4l, "dict_minmax-08");

	fprintf(fout, "\nAll tests for dict_minmax passed!\n");
	fclose(fout);
}

void test_succ_pred(dict_t **dict) {
	FILE *fout = fopen("outputs/output_dict_succ_pred.out", "w");
	long value;

	if (fout == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if ((*dict == NULL) || ((*dict)->root == NULL)) {
		fprintf(fout, "Empty dict passed!\n");
		fclose(fout);
		return;
	}

	ASSERT(fout, dict_successor((*dict)->root) != NULL, "dict_successor-01 ");

	ASSERT(fout, dict_successor(dict_maximum((*dict)->root)) == NULL, "dict_successor-02 ");

	value = 3;
	ASSERT(fout, *((long*)dict_successor(dict_search((*dict), (*dict)->root, &value))->key) == 4l, "dict_successor-03 ");

	value = 4;
	ASSERT(fout, *((long*)dict_successor(dict_search((*dict), (*dict)->root, &value))->key) == 5l, "dict_successor-04 ");

	ASSERT(fout, dict_predecessor((*dict)->root) != NULL, "dict_predecesor-01");

	ASSERT(fout, dict_predecessor(dict_minimum((*dict)->root)) == NULL, "dict_predecesor-02");

	value = 3;
	ASSERT(fout, *((long*)dict_predecessor(dict_search((*dict), (*dict)->root, &value))->key) == 2l, "dict_predecesor-03");

	value = 6;
	ASSERT(fout, *((long*)dict_predecessor(dict_search((*dict), (*dict)->root, &value))->key) == 5l, "dict_predecesor-04");

	fprintf(fout, "\nAll tests for dict_succ_pred passed!\n");
	fclose(fout);
}

void test_rotations(dict_t **dict) {
	FILE *fout = fopen("outputs/output_dict_rotations.out", "w");

	if (fout == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if (*dict == NULL) {
		fprintf(fout, "Empty dict passed!\n");
		fclose(fout);
		return;
	}

	dict_rotate_left((*dict), (*dict)->root);
	ASSERT(fout, *((long*)((*dict)->root)->key) == 7l, "dict_rotate-01");
	ASSERT(fout, *((long*)((*dict)->root->left)->key) == 5l, "dict_rotate-02");
	ASSERT(fout, *((long*)((*dict)->root->right)->key) == 8l, "dict_rotate-03");
	ASSERT(fout, *((long*)((*dict)->root->left->left)->key) == 3l, "dict_rotate-04");
	ASSERT(fout, *((long*)((*dict)->root->left->right)->key) == 6l, "dict_rotate-05");

	dict_rotate_right((*dict), (*dict)->root);
	ASSERT(fout, *((long*)((*dict)->root)->key) == 5l, "dict_rotate-06");
	ASSERT(fout, *((long*)((*dict)->root->left)->key) == 3l, "dict_rotate-07");
	ASSERT(fout, *((long*)((*dict)->root->right)->key) == 7l, "dict_rotate-08");
	ASSERT(fout, *((long*)((*dict)->root->right->left)->key) == 6l, "dict_rotate-09");
	ASSERT(fout, *((long*)((*dict)->root->right->right)->key) == 8l, "dict_rotate-10");

	fprintf(fout, "\nAll tests for rotations passed!\n");
	fclose(fout);
}

void test_insert(dict_t **dict) {
	FILE *fout = fopen("outputs/output_dict_insert.out", "w");

	if (fout == NULL) {
		printf("Error opening file!\n");
		return;
	}

	(*dict) = create_dict(create_long, destroy_long, create_long, destroy_long, compare_long);

	if (*dict == NULL) {
		fprintf(fout, "Empty dict passed!\n");
		fclose(fout);
		return;
	}

	long values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

	dict_insert(*(dict), values + 2, values + 2);
	ASSERT(fout, is_dict_empty(*dict) != 1, "dict_insert-01");
	ASSERT(fout, (*dict)->root->left == NULL, "dict_insert-02");
	ASSERT(fout, (*dict)->root->right == NULL, "dict_insert-03");

	dict_insert((*dict), values + 3, values + 3);
	dict_insert((*dict), values + 4, values + 4);
	ASSERT(fout, *((long*)(*dict)->root->left->key) == 2l, "dict_insert-04");
	ASSERT(fout, *((long*)(*dict)->root->right->key) == 4l, "dict_insert-05");

	dict_insert((*dict), values + 1, values + 1);
	dict_insert((*dict), values, values);
	ASSERT(fout, *((long*)(*dict)->root->left->key) == 1l, "dict_insert-06");
	ASSERT(fout, *((long*)(*dict)->root->left->left->key) == 0l, "dict_insert-07");
	ASSERT(fout, *((long*)(*dict)->root->left->right->key) == 2l, "dict_insert-08");

	dict_insert((*dict), values + 5, values + 5);
	dict_insert((*dict), values + 6, values + 6);
	ASSERT(fout, *((long*)(*dict)->root->right->key) == 5l,"dict_insert-09");
	ASSERT(fout, *((long*)(*dict)->root->right->left->key) == 4l,"dict_insert-10");
	ASSERT(fout, *((long*)(*dict)->root->right->right->key) == 6l,"dict_insert-11");

	dict_insert((*dict), values + 8, values + 8);
	dict_insert((*dict), values + 7, values + 7);
	ASSERT(fout, *((long*)(*dict)->root->right->right->key) == 7l,"dict_insert-12");
	ASSERT(fout, *((long*)(*dict)->root->right->right->left->key) == 6l,"dict_insert-13");
	ASSERT(fout, *((long*)(*dict)->root->right->right->right->key) == 8l,"dict_insert-14");

	fprintf(fout, "\nAll tests for dict_insert passed!\n");
	fclose(fout);
}

void test_delete(dict_t **dict) {
	FILE *fout = fopen("outputs/output_dict_delete.out", "w");

	if (fout == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if (*dict == NULL || (*dict)->root == NULL) {
		fprintf(fout, "Empty dict passed!\n");
		fclose(fout);
		return;
	}

	long value = 6;
	dict_delete((*dict),&value);
	ASSERT(fout, *((long*)(*dict)->root->key) == 3l, "dict_delete-01");
	ASSERT(fout, (*dict)->root->right->right->left == NULL, "dict_delete-02");

	value = 4;
	dict_delete((*dict),&value);
	ASSERT(fout, *((long*)(*dict)->root->right->key) == 7l, "dict_delete-03");
	ASSERT(fout, *((long*)(*dict)->root->right->left->key) == 5l, "dict_delete-04");
	ASSERT(fout, *((long*)(*dict)->root->right->right->key) == 8l, "dict_delete-05");

	value = 3;
	dict_delete((*dict),&value);
	ASSERT(fout, *((long*)(*dict)->root->key) == 5l, "dict_delete-06");
	ASSERT(fout, *((long*)(*dict)->root->right->key) == 7l, "dict_delete-07");
	ASSERT(fout, *((long*)(*dict)->root->right->right->key) == 8l, "dict_delete-08");
	ASSERT(fout, (*dict)->root->right->left == NULL, "dict_delete-09");

	value = 7;
	dict_delete((*dict),&value);
	ASSERT(fout, *((long*)(*dict)->root->right->key) == 8l, "dict_delete-10");
	ASSERT(fout, (*dict)->root->right->height == 1, "dict_delete-11");

	value = 8;
	dict_delete((*dict),&value);
	ASSERT(fout, *((long*)(*dict)->root->key) == 1l, "dict_delete-12");
	ASSERT(fout, *((long*)(*dict)->root->left->key) == 0l, "dict_delete-13");
	ASSERT(fout, *((long*)(*dict)->root->right->key) == 5l, "dict_delete-14");
	ASSERT(fout, *((long*)(*dict)->root->right->left->key) == 2l, "dict_delete-15");

	value = 0;
	dict_delete((*dict),&value);
	ASSERT(fout, *((long*)(*dict)->root->key) == 2l, "dict_delete-16");
	ASSERT(fout, *((long*)(*dict)->root->left->key) == 1l, "dict_delete-17");
	ASSERT(fout, *((long*)(*dict)->root->right->key) == 5l, "dict_delete-18");

	value = 2;
	dict_delete((*dict),&value);
	value = 1;
	dict_delete((*dict),&value);
	value = 5;
	dict_delete((*dict),&value);
	ASSERT(fout, (*dict)->root == NULL, "dict_delete-19");
	ASSERT(fout, (*dict)->size == 0, "dict_delete-20");

	fprintf(fout, "\nAll tests for dict_delete passed!\n");
	fclose(fout);
}

void test_list_insert(dict_t **dict) {
	FILE *fout = fopen("outputs/output_dict_list_insert.out", "w");

	if (fout == NULL) {
		printf("Error opening file!\n");
		return;
	}

	long values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
	for(size_t i = 0; i < sizeof(values)/sizeof(values[0]); i++)
		dict_insert(*(dict), values + i, values + i);

	if (*dict == NULL || (*dict)->root == NULL) {
		fprintf(fout, "Empty dict passed!\n");
		fclose(fout);
		return;
	}

	ASSERT(fout, dict_minimum((*dict)->root)->prev == NULL, "dict_list_insert-01");
	ASSERT(fout, dict_maximum((*dict)->root)->end->next == NULL, "dict_list_insert-02");
	ASSERT(fout, *((long*)(*dict)->root->key) == 3, "dict_list_insert-03");
	ASSERT(fout, (*dict)->root == (*dict)->root->end, "dict_list_insert-04");
	ASSERT(fout, (*dict)->root->prev == dict_predecessor((*dict)->root), "dict_list_insert-05");
	ASSERT(fout, (*dict)->root->next == dict_successor((*dict)->root), "dict_list_insert-06");

	dict_insert(*(dict), values + 3, values + 4);
	ASSERT(fout, (*dict)->root != (*dict)->root->end, "dict_list_insert-07");
	ASSERT(fout, (*dict)->root->next == (*dict)->root->end, "dict_list_insert-08");
	ASSERT(fout, (*dict)->root->prev == dict_predecessor((*dict)->root), "dict_list_insert-09");
	ASSERT(fout, (*dict)->root->end->next == dict_successor((*dict)->root), "dict_list_insert-10");

	dict_insert(*(dict), values + 3, values + 5);
	ASSERT(fout, *((long*)(*dict)->root->key) == *((long*)(*dict)->root->next->key), "dict_list_insert-11");
	ASSERT(fout, *((long*)(*dict)->root->key) == *((long*)(*dict)->root->next->next->key), "dict_list_insert-12");
	ASSERT(fout, (*dict)->root->end == (*dict)->root->next->next, "dict_list_insert-13");
	ASSERT(fout, (*dict)->root->next->next == dict_successor((*dict)->root)->prev, "dict_list_insert-14");
	ASSERT(fout, (*dict)->root == dict_predecessor((*dict)->root)->next, "dict_list_insert-15");

	dict_insert(*(dict),values, values);
	ASSERT(fout, *((long*)dict_minimum((*dict)->root)->key) == 0 , "dict_list_insert-16");
	ASSERT(fout, *((long*)dict_minimum((*dict)->root)->next->key) == 0 , "dict_list_insert-17");
	ASSERT(fout, *((long*)dict_minimum((*dict)->root)->end->next->key) == 1 , "dict_list_insert-18");
	ASSERT(fout, dict_minimum((*dict)->root)->prev == NULL , "dict_list_insert-19");
	ASSERT(fout, dict_minimum((*dict)->root)->prev == NULL , "dict_list_insert-20");
	ASSERT(fout, dict_minimum((*dict)->root)->next->next == dict_minimum((*dict)->root)->end->next , "dict_list_insert-21");

	dict_insert(*(dict), values + 8, values + 8);
	ASSERT(fout, *((long*)dict_maximum((*dict)->root)->key) == 8, "dict_list_insert-22");
	ASSERT(fout, *((long*)dict_maximum((*dict)->root)->next->key) == 8, "dict_list_insert-23");
	ASSERT(fout, *((long*)dict_maximum((*dict)->root)->prev->key) == 7l, "dict_list_insert-24");
	ASSERT(fout, dict_maximum((*dict)->root)->end->next == NULL , "dict_list_insert-25");
	ASSERT(fout, dict_maximum((*dict)->root)->next->next == NULL , "dict_list_insert-26");
	ASSERT(fout, dict_maximum((*dict)->root)->next->next == dict_maximum((*dict)->root)->end->next , "dict_list_insert-27");

	fprintf(fout, "\nAll tests for dict_list_insert passed!\n");
	fclose(fout);
}

void test_list_delete(dict_t **dict) {
	FILE *fout = fopen("outputs/output_dict_list_delete.out", "w");

	if (fout == NULL) {
		printf("Error opening file!\n");
		return;
	}

	if (*dict == NULL || (*dict)->root == NULL) {
		fprintf(fout, "Empty dict passed!\n");
		fclose(fout);
		return;
	}

	long value = 3;
	dict_delete((*dict), &value);
	ASSERT(fout, *((long*)(*dict)->root->key) == 3l, "dict_list_delete-01");
	ASSERT(fout, *((long*)(*dict)->root->next->key) == 3l, "dict_list_delete-02");
	ASSERT(fout, *((long*)(*dict)->root->value) == 3l, "dict_list_delete-03");
	ASSERT(fout, *((long*)(*dict)->root->end->value) == 4l, "dict_list_delete-04");

	ASSERT(fout, *((long*)(*dict)->root->end->key) == 3l, "dict_list_delete-05");
	ASSERT(fout, (*dict)->root->end->key == (*dict)->root->next->key, "dict_list_delete-06");
	ASSERT(fout, *((long*)(*dict)->root->end->next->key) == 4l, "dict_list_delete-07");
	ASSERT(fout, *((long*)(*dict)->root->prev->key) == 2l, "dict_list_delete-08");

	dict_delete((*dict), &value);
	ASSERT(fout, *((long*)(*dict)->root->key) == 3l, "dict_list_delete-09");
	ASSERT(fout, *((long*)(*dict)->root->next->key) == 4l, "dict_list_delete-10");
	ASSERT(fout, *((long*)(*dict)->root->end->key) == 3l, "dict_list_delete-11");
	ASSERT(fout, (*dict)->root->end->next == (*dict)->root->next, "dict_list_delete-12");
	ASSERT(fout, *((long*)(*dict)->root->end->next->key) == 4l, "dict_list_delete-13");
	ASSERT(fout, *((long*)(*dict)->root->prev->key) == 2l, "dict_list_delete-14");

	dict_delete((*dict), &value);
	ASSERT(fout, *((long*)(*dict)->root->key) == 4l, "dict_list_delete-15");
	ASSERT(fout, *((long*)(*dict)->root->next->key) == 5l, "dict_list_delete-16");
	ASSERT(fout, *((long*)(*dict)->root->end->key) == 4l, "dict_list_delete-17");
	ASSERT(fout, (*dict)->root->end->next == (*dict)->root->next, "dict_list_delete-18");
	ASSERT(fout, *((long*)(*dict)->root->end->next->key) == 5l, "dict_list_delete-19");
	ASSERT(fout, *((long*)(*dict)->root->prev->key) == 2l, "dict_list_delete-20");

	value = 4;
	dict_delete((*dict), &value);
	ASSERT(fout, *((long*)(*dict)->root->key) == 5l, "dict_list_delete-21");
	ASSERT(fout, *((long*)(*dict)->root->next->key) == 6l, "dict_list_delete-22");
	ASSERT(fout, *((long*)(*dict)->root->end->key) == 5l, "dict_list_delete-23");
	ASSERT(fout, (*dict)->root->end->next == (*dict)->root->next, "dict_list_delete-24");
	ASSERT(fout, *((long*)(*dict)->root->end->next->key) == 6l, "dict_list_delete-25");
	ASSERT(fout, *((long*)(*dict)->root->prev->key) == 2l, "dict_list_delete-26");

	value = 8;
	dict_delete((*dict), &value);
	ASSERT(fout, *((long*)dict_maximum((*dict)->root)->key) == 8, "dict_list_delete-27");
	ASSERT(fout, *((long*)dict_maximum((*dict)->root)->end->key) == 8, "dict_list_delete-28");
	ASSERT(fout, *((long*)dict_maximum((*dict)->root)->prev->key) == 7l, "dict_list_delete-29");
	ASSERT(fout, dict_maximum((*dict)->root)->end->next == NULL , "dict_list_delete-30");
	ASSERT(fout, dict_maximum((*dict)->root)->next == NULL , "dict_list_delete-31");

	dict_delete((*dict), &value);
	ASSERT(fout, *((long*)dict_maximum((*dict)->root)->key) == 7, "dict_list_delete-32");
	ASSERT(fout, *((long*)dict_maximum((*dict)->root)->end->key) == 7, "dict_list_delete-33");
	ASSERT(fout, *((long*)dict_maximum((*dict)->root)->prev->key) == 6l, "dict_list_delete-34");
	ASSERT(fout, dict_maximum((*dict)->root)->end->next == NULL , "dict_list_delete-35");
	ASSERT(fout, dict_maximum((*dict)->root)->next == NULL , "dict_list_delete-36");

	value = 0;
	dict_delete((*dict), &value);
	ASSERT(fout, *((long*)dict_minimum((*dict)->root)->key) == 0, "dict_list_delete-37");
	ASSERT(fout, *((long*)dict_minimum((*dict)->root)->end->key) == 0, "dict_list_delete-38");
	ASSERT(fout, *((long*)dict_minimum((*dict)->root)->next->key) == 1l, "dict_list_delete-39");
	ASSERT(fout, dict_minimum((*dict)->root)->prev == NULL , "dict_list_delete-40");

	dict_delete((*dict), &value);
	ASSERT(fout, *((long*)dict_minimum((*dict)->root)->key) == 1, "dict_list_delete-41");
	ASSERT(fout, *((long*)dict_minimum((*dict)->root)->end->key) == 1, "dict_list_delete-42");
	ASSERT(fout, *((long*)dict_minimum((*dict)->root)->next->key) == 2l, "dict_list_delete-43");
	ASSERT(fout, dict_minimum((*dict)->root)->prev == NULL , "dict_list_delete-44");

	fprintf(fout, "\nAll tests for dict_list_delete passed!\n");
	fclose(fout);
}

void test_free(dict_t **dict1, dict_t **dict2) {
	if ((*dict1) != NULL && (*dict1)->root != NULL) {
		destroy_dict_node((*dict1), (*dict1)->root->left->left);
		destroy_dict_node((*dict1), (*dict1)->root->left->right);
		destroy_dict_node((*dict1), (*dict1)->root->right->left);
		destroy_dict_node((*dict1), (*dict1)->root->right->right);
		destroy_dict_node((*dict1), (*dict1)->root->left);
		destroy_dict_node((*dict1), (*dict1)->root->right);
		destroy_dict_node((*dict1), (*dict1)->root);
		free(*dict1);
	}

	destroy_dict(*dict2);
}

void draw_tree(dict_t *dict, uint8_t type) {
	// type 1 - for int
	// type 2 - for strings

	FILE *fout = fopen("dict.dot", "w");
	print_dot(dict->root, fout, type);
	fclose(fout);

	if (system("dot dict.dot | neato -n -Tpng -o dict.png") == -1) {
		printf("Failed to create dot file\n");
	}
}

void test_build_tree(dict_t **dict) {

	FILE *fout = fopen("outputs/simple_key_tree.dot", "w");

	build_dict_from_file("inputs/simple_key.txt", (*dict));

	if (*dict == NULL) {
		fprintf(fout, "Empty dict passed!\n");
		fclose(fout);

		fout = fopen("outputs/key_tree.dot", "w");
		fprintf(fout, "Empty dict passed!\n");
		fclose(fout);
		return;
	}

	if ((*dict)->root == NULL) {
		fprintf(fout, "Invalid dict!\n");
		free(*dict);
		*dict = NULL;
		fclose(fout);
	} else {
		print_dot((*dict)->root, fout, 2);
		fclose(fout);
		if (system("dot outputs/simple_key_tree.dot | neato -n -Tpng -o outputs/simple_key_tree.png") == -1) {
			printf("failed to create dot file\n");
		}
	}

	destroy_dict(*dict);
	*dict = create_dict(create_str_element, destroy_str_element, create_index_info, destroy_index_info, compare_str);
	
	build_dict_from_file("inputs/key.txt", (*dict));

	fout = fopen("outputs/key_tree.dot", "w");

	if ((*dict)->root == NULL) {
		fprintf(fout, "Invalid dict!\n");
		free(*dict);
		*dict = NULL;
		fclose(fout);

	} else {
		print_dot((*dict)->root, fout, 2);
		fclose(fout);
		if (system("dot outputs/key_tree.dot | neato -n -Tpng -o outputs/key_tree.png") == -1) {
			printf("Failed to create dot file\n");
		}
	}
}

void test_inorder_key(dict_t **dict) {
	range_t *key = inorder_key_query((*dict));

	print_key("outputs/output_dict_inorder_key.out", key);

	if (key != NULL) {
		decrypt("inputs/cipher1.txt", "outputs/cipher1.txt", key);
		free(key->index);
		free(key);
	}
}

void test_level_key(dict_t **dict) {
	range_t *key = level_key_query((*dict));

	print_key("outputs/output_dict_level_key.out", key);

	if (key != NULL) {
		decrypt("inputs/cipher2.txt", "outputs/cipher2.txt", key);
		free(key->index);
		free(key);
	}
}

void test_range_key(dict_t **dict) {
	range_t *key = range_key_query((*dict), "CD", "GG");

	print_key("outputs/output_dict_range_key.out", key);

	if (key != NULL) {
		decrypt("inputs/cipher3.txt", "outputs/cipher3.txt", key);
		free(key->index);
		free(key);
	}
}

int main(void) {
	dict_t *dict1 = NULL;
	dict_t *dict2 = NULL;

	test_init(&dict1);
	create_simple_tree(&dict1);
	test_search(&dict1);
	test_minmax(&dict1);
	test_succ_pred(&dict1);
	test_rotations(&dict1);
	test_insert(&dict2);
	test_delete(&dict2);
	test_list_insert(&dict2);
	test_list_delete(&dict2);
	test_free(&dict1, &dict2);

	dict_t *dict = NULL;
	dict = create_dict(create_str_element, destroy_str_element, create_index_info, destroy_index_info, compare_str);

	test_build_tree(&dict);
	test_inorder_key(&dict);
	test_level_key(&dict);
	test_range_key(&dict);

	destroy_dict(dict);

	return 0;
}