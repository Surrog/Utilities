cmake_minimum_required (VERSION 3.7)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the root and call cmake from there")
endif()

project (Utilities)

#compiler requirement
set (CMAKE_CXX_STANDARD 14)

set (XTSSLIB_SOURCES_DIR "${PROJECT_SOURCE_DIR}/xtsslib" CACHE PATH "source path to xtsslib dependancy, default to the case that submodule is initialized")

include ("${XTSSLIB_SOURCES_DIR}/utility.cmake")

find_boost_lib()

# shortcut for directories used for the compilation
set (RESOLVER_SERVER_DIR PATH ${PROJECT_SOURCE_DIR}/resolver_server)

# directories where to search headers
include_directories("${XTSSLIB_SOURCES_DIR}")
include_directories("${Boost_INCLUDE_DIRS}")
link_directories("${Boost_LIBRARY_DIRS}")
# sources in the generated_sources directory

set (NETCAT_SOURCES "${PROJECT_SOURCE_DIR}/basic_netcat/basic_netcat/main.cpp")
set (CAT_SOURCES "${PROJECT_SOURCE_DIR}/basic_netcat/cat/main.cpp")
set (RM_SOURCES 
"${PROJECT_SOURCE_DIR}/basic_rm/basic_rm/main.cpp"
"${PROJECT_SOURCE_DIR}/basic_rm/basic_rm/input.hpp"
"${PROJECT_SOURCE_DIR}/basic_rm/basic_rm/main.hpp"
)
set (CLEAN_TWIN_SOURCES 
"${PROJECT_SOURCE_DIR}/clean_twin/Clean_twin/main.cpp"
"${PROJECT_SOURCE_DIR}/clean_twin/Clean_twin/main.hpp"
"${PROJECT_SOURCE_DIR}/clean_twin/Clean_twin/File_comparer.hpp"
)
set (SEARCH_SOURCES 
"${PROJECT_SOURCE_DIR}/search/search/main.cpp"
"${PROJECT_SOURCE_DIR}/search/search/InputOutput.hpp"
"${PROJECT_SOURCE_DIR}/search/search/search_task.hpp"
"${PROJECT_SOURCE_DIR}/search/search/search_task.cpp"
)

add_executable(xnc ${NETCAT_SOURCES})
target_link_libraries(xnc ${Boost_LIBRARIES})

add_executable(xcat ${CAT_SOURCES})
target_link_libraries(xcat ${Boost_LIBRARIES})

add_executable(xrm ${RM_SOURCES})
target_link_libraries(xrm ${Boost_LIBRARIES})

add_executable(clean_twin ${CLEAN_TWIN_SOURCES})
target_link_libraries(clean_twin ${Boost_LIBRARIES})

add_executable(xsearch ${SEARCH_SOURCES})
target_link_libraries(xsearch ${Boost_LIBRARIES})
