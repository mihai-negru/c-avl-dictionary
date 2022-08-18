CFLAGS += -c -g -Wall -Wextra -Wpedantic \
                  -Wformat=2 -Wno-unused-parameter \
                  -Wshadow -Wwrite-strings -Wstrict-prototypes \
                  -Wold-style-definition -Wredundant-decls \
                  -Wnested-externs -Wmissing-include-dirs \
                  -Wjump-misses-init -Wlogical-op -O2

PATH_TO_FILES += ../src
PATH_TO_HEADER_FILES += $(PATH_TO_FILES)/include

FILES += $(PATH_TO_FILES)/avl_dict.c $(PATH_TO_FILES)/avl_dict_run.c \
		 $(PATH_TO_FILES)/cipher.c

.PHONY: all build clean clean_all

all: build
	@gcc *.o -o avl_dict_run

build: $(FILES)
	@gcc $(CFLAGS) $(FILES)

clean:
	@rm -rf avl_dict_run.o avl_dict_run outputs

clean_all:
	@rm -rf *.o avl_dict_run outputs