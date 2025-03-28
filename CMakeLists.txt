cmake_minimum_required(VERSION 3.27)
project(json)

set(CMAKE_CXX_STANDARD 23)
add_executable(json test.cpp json.hpp input.cpp input.hpp
	json_array.cpp json_array.hpp
	json_bool.cpp json_bool.hpp
	json_element.hpp
	json_null.cpp json_null.hpp
	json_number.cpp json_number.hpp
	json_object.cpp json_object.hpp
	json_string.cpp json_string.hpp
	json_parse.cpp json_parse.hpp
	json_types.hpp)