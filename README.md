# c-avl-dictionary
C Language implementation of an AVL tree to represent a dictionary (key, value ->pairs). In order to support duplicates of (key, value) pairs every AVL node will represent a double linked list. The Dictionary will be used to solve some decripting problems.

## Table of Contents

1. [Getting Started](#start-description)
2. [Building](#build-description)
3. [Testing](#test-description)

<a name="start-description"></a>
## Getting Started

The program will generate some "*.o" files (object files) one for AVL-dictionary and one for Chipers.

### The functions from AVL-dictionary:

* **create_dict**
* **create_dict_node**
* **destroy_dict_node**
* **destroy_dict**
* **is_dict_empty**
* **dict_search**
* **dict_minimum**
* **dict_maximum**
* **dict_successor**
* **dict_predecessor**
* **update_height**
* **dict_rotate_left**
* **dict_rotate_right**
* **dict_get_balance**
* **dict_insert**
* **dict_delete**

Check the [avl_dict.c](/src/avl_dict.c) to see the documentation for every function

### The functions from Chiphers:

* **build_dict_from_file**
* **encrypt**
* **decrypt**
* **print_key**
* **inorder_key_query**
* **level_key_query**
* **range_key_query**

Check the [cipher.c](/src/cipher.c) to see the documentation for every function

### What does cipher.c ?

Alice and Bob want to send messages between them, but they want to secure the messages so they find about an encryption "vigenere" and have the following keys:

* **inorder_key** - the key is represented as a set of numbers that represent the starting position of every word taken from a file, also those numbers
represent how many times a character needs to be shifted to the left so it can be decrypted. The numbers are taken in sorted order from smaller number to bigger number
the duplicate words keys are also calculated.

* **level_key** - the key is represented as a set of numbers that represent the starting position of every word taken from a file. The members key are selected just from
an AVL tree level, the level of the avl tree is choosen such that the dictionary node with the longest double linked list would be on that level.

* **range_key** - the key is represented as a set of numbers that represent the starting position of every word taken from a file. For this method user has to choose two
keys that don't have to be exact and the members key will be thos keys that will be found in that value range.

For examples on how to generate a key for this chipher you should check [avl_dict_run.c](src/avl_dict_run.c) to see how to build a dictionary from a file to provide useful information
for the encryption.

<a name="build-description"></a>
## Building

In order to build the project make sure that you are on a LINUX machine or **WSL**, the enter the build folder and to run the *Makefile**:

```BASH
    cd build
    make

    # to clear all junk files and keep obj files
    make clean

    # to clear every thing
    make clean_all
```

After that you will get the object file **avl_dict.o, avl_dict_run.o chiper.o and avl_dict_run** that you can use in your future projects.

<a name="test-description"></a>
## Testing

In order to see how to work with project functions, I suggest to look up to [avl_dict_run.c](/src/avl_dict_run.c) file. This file is a collection
of tests to check every function, especially corener cases, like NULLs statements.

In order to run the tests and to see what tests are passed you should run:

```BASH
    ./avl_dict_run.sh
```

to see generated outputs without checking them you should run:

```BASH
    mkdir -p outputs
    ./avl_dict_run

    # to keep the outputs folder
    make clean

    # to delete it
    make clean_all
```

To see how the output is printed you should look to [THIS_FOLDER](build/reference/) and to look in avl_dict_run.c to see how the output is redirected
to your specific file.