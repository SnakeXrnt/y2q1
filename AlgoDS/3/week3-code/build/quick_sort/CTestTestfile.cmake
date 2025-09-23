# CMake generated Testfile for 
# Source directory: /home/nw1728/y2q1/AlgoDS/3/week3-code/quick_sort
# Build directory: /home/nw1728/y2q1/AlgoDS/3/week3-code/build/quick_sort
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(quick_sort_test "python3" "/home/nw1728/y2q1/AlgoDS/3/week3-code/scripts/test_sorting.py" "quick_sort" "/home/nw1728/y2q1/AlgoDS/3/week3-code/quick_sort/tests/quick_sort_test_cases")
set_tests_properties(quick_sort_test PROPERTIES  WORKING_DIRECTORY "/home/nw1728/y2q1/AlgoDS/3/week3-code/build/quick_sort" _BACKTRACE_TRIPLES "/home/nw1728/y2q1/AlgoDS/3/week3-code/quick_sort/CMakeLists.txt;10;add_test;/home/nw1728/y2q1/AlgoDS/3/week3-code/quick_sort/CMakeLists.txt;0;")
