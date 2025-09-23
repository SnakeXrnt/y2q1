# CMake generated Testfile for 
# Source directory: /home/nw1728/y2q1/AlgoDS/1/week1-code/sorted_arrays
# Build directory: /home/nw1728/y2q1/AlgoDS/1/week1-code/build/sorted_arrays
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(sorted_array_test_insert "python3" "/home/nw1728/y2q1/AlgoDS/1/week1-code/scripts/test_target.py" "sorted_arrays" "/home/nw1728/y2q1/AlgoDS/1/week1-code/sorted_arrays/tests/insert_cases")
set_tests_properties(sorted_array_test_insert PROPERTIES  WORKING_DIRECTORY "/home/nw1728/y2q1/AlgoDS/1/week1-code/build/sorted_arrays" _BACKTRACE_TRIPLES "/home/nw1728/y2q1/AlgoDS/1/week1-code/sorted_arrays/CMakeLists.txt;5;add_test;/home/nw1728/y2q1/AlgoDS/1/week1-code/sorted_arrays/CMakeLists.txt;0;")
add_test(sorted_array_test_contains "python3" "/home/nw1728/y2q1/AlgoDS/1/week1-code/scripts/test_target.py" "sorted_arrays" "/home/nw1728/y2q1/AlgoDS/1/week1-code/sorted_arrays/tests/contains_cases")
set_tests_properties(sorted_array_test_contains PROPERTIES  WORKING_DIRECTORY "/home/nw1728/y2q1/AlgoDS/1/week1-code/build/sorted_arrays" _BACKTRACE_TRIPLES "/home/nw1728/y2q1/AlgoDS/1/week1-code/sorted_arrays/CMakeLists.txt;9;add_test;/home/nw1728/y2q1/AlgoDS/1/week1-code/sorted_arrays/CMakeLists.txt;0;")
add_test(sorted_array_test_remove "python3" "/home/nw1728/y2q1/AlgoDS/1/week1-code/scripts/test_target.py" "sorted_arrays" "/home/nw1728/y2q1/AlgoDS/1/week1-code/sorted_arrays/tests/remove_cases")
set_tests_properties(sorted_array_test_remove PROPERTIES  WORKING_DIRECTORY "/home/nw1728/y2q1/AlgoDS/1/week1-code/build/sorted_arrays" _BACKTRACE_TRIPLES "/home/nw1728/y2q1/AlgoDS/1/week1-code/sorted_arrays/CMakeLists.txt;13;add_test;/home/nw1728/y2q1/AlgoDS/1/week1-code/sorted_arrays/CMakeLists.txt;0;")
