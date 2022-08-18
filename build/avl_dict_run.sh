#!/bin/bash

function init {
    pad=$(printf '%.1s' "."{1..70})
	padlength=71

    mkdir -p outputs

    make
    valgrind --leak-check=full --error-exitcode=1 ./avl_dict_run
    valgrind_error=$?

    printf "\n"
}

function print_result {
	printf "%s" "$1"
	printf "%*.*s" 0 $((padlength - ${#1} - ${#2} )) "$pad"
	printf "%s\n" "$2"
}

function check_test {
    begin_test='outputs/output_'
    begin_result='reference/result_'
    termination='.out'


    tests=( "dict_init" "dict_search" "dict_minmax" "dict_succ_pred" "dict_rotations" "dict_insert" "dict_delete" "dict_list_insert" "dict_list_delete")

    for i in ${!tests[@]}
    do
        element=${tests[$i]}
        testFileName=$begin_test$element$termination
        resultFileName=$begin_result$element$termination
        result=$(diff $testFileName $resultFileName | wc -l)

        if [ -f $testFileName ] && [ -f $resultFileName ] && [ "$result" -eq "0" ]; then
            print_result "Test ${tests[$i]} " " passed" 
        else
            print_result "Test ${tests[$i]} " " failed" 
        fi
    done

    printf "\n"

    result_build_tree_simple=$(diff reference/simple_key_tree.dot outputs/simple_key_tree.dot | wc -l)
    result_build_tree=$(diff reference/key_tree.dot outputs/key_tree.dot | wc -l)
    result=$(($result_build_tree_simple + $result_build_tree))

    if [ "$result" -eq "0" ]; then
        print_result "Test build_tree " " passed" 
    else
        print_result "Test build_tree " " failed" 
    fi


    tests=( "dict_inorder_key" "dict_level_key" "dict_range_key" )

    for i in ${!tests[@]}
    do
        element=${tests[$i]}
        testFileName=$begin_test$element$termination
        resultFileName=$begin_result$element$termination
        result=$(diff $testFileName $resultFileName | wc -l)

        if [ -f $testFileName ] && [ -f $resultFileName ] && [ "$result" -eq "0" ]; then
            print_result "Test ${tests[$i]} " " passed" 
        else
            print_result "Test ${tests[$i]} " " failed" 
        fi
    done

    printf "\n"

    if [[ ${valgrind_error} -eq 0 ]]; then
        print_result "Valgrind errors " " passed"
    else
        print_result "Valgrind errors " " failed"
    fi

    printf "\n"
}

init
check_test
make clean_all &> /dev/null
