cmake_minimum_required (VERSION 2.6)
project (NeuronProject)

set(INCLUDE_DIRECTORY inc)
set(CMAKE_CXX_FLAGS "-O3 -W -Wall -pedantic -std=c++11")

enable_testing()
add_subdirectory(gtest)
include_directories(${gtest_SOURCE_DIR}/include${gtest_SOURCE_DIR})

# Finds .h/.hpp files
include_directories("${INCLUDE_DIRECTORY}")

add_executable (${PROJECT_NAME} src/Neuron.cpp src/main.cpp src/Simulation.cpp)
add_executable(Neuron_unittest src/Neuron.cpp src/Neuron_unittest.cpp)

target_link_libraries(Neuron_unittest gtest gtest_main)
add_test(Neuron_unittest Neuron_unittest)

# Doxygen documentation
# We check if doxygen is present
find_package(Doxygen)
if(DOXYGEN_FOUND)
	# We check if the Doxygen configuration file exists
	IF (NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
		message(FATAL_ERROR "Doxygen configuration file is needed to build the documentation:
		-${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in not found.
		-Generate one in the project folder with doxygen -g Doxyfile.in")
	ENDIF(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
	

	#We configure the doxyfile for make
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/inc
	)
ELSE(DOXYGEN_FOUND)
	message(FATAL_ERROR "Doxygen is needed to build the documentation.")
ENDIF(DOXYGEN_FOUND)
		
