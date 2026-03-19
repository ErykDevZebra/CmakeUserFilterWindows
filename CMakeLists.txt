cmake_minimum_required(VERSION 3.16)

# Define the project name and specify the required C++ standard
project(UserFilterLibrary)

# Specify the version (optional)
set(PROJECT_VERSION 1.0)

# Set the C++ standard for the project (you can change this if needed)
set(CMAKE_CXX_STANDARD 17)

if(NOT DEFINED ENV{AVS_PROFESSIONAL_SDK5_6})
    message(FATAL_ERROR "Environment variable AVS_PROFESSIONAL_SDK5_6 is not set. "
                      "Please set it to the root of the Aurora Vision Studio SDK installation.")
else()

    # Convert the environment variable path to a CMake-friendly path
    file(TO_CMAKE_PATH "$ENV{AVS_PROFESSIONAL_SDK5_6}" AVS_SDK_PATH)

    set(AVS_SDK_PATH "$ENV{AVS_PROFESSIONAL_SDK5_6}")
    message(STATUS "Using AVS Professional SDK from: ${AVS_SDK_PATH}")


    set(HEAD_DIR "./headers")
    set(SRC_DIR "./sources")

    include_directories(${HEAD_DIR})

    find_file(USER_FILTERS_DEF_FILE
              NAMES UserFilters.def
              PATHS "${AVS_SDK_PATH}/lib/x64"
              NO_DEFAULT_PATH)

    find_library(AVL_LITE_LIBRARY
                 NAMES AVL_Lite.lib
                 PATHS "${AVS_SDK_PATH}/lib/x64"
                 NO_DEFAULT_PATH)

    find_library(USER_FILTERS_LIBRARY
                 NAMES UserFilters.lib
                 PATHS "${AVS_SDK_PATH}/lib/x64"
                 NO_DEFAULT_PATH)



    # Link your target against both found libraries.
    if(AVL_LITE_LIBRARY AND USER_FILTERS_LIBRARY AND USER_FILTERS_DEF_FILE)
        message(STATUS "Found AVL_Lite library: ${AVL_LITE_LIBRARY}")
        message(STATUS "Found UserFilters library: ${USER_FILTERS_LIBRARY}")
        message(STATUS "Found UserFilters definitions: ${USER_FILTERS_DEF_FILE}")

        add_library(user_filter_library SHARED 
            ${SRC_DIR}/MyOwnFilter.cpp
            ${USER_FILTERS_DEF_FILE}
        )


        target_include_directories(user_filter_library PRIVATE
            "${AVS_SDK_PATH}/include"
        )

        # Link both libraries
        target_link_libraries(user_filter_library PRIVATE
            ${AVL_LITE_LIBRARY}
            ${USER_FILTERS_LIBRARY}
        )

        # Set the output name for the shared library
        set_target_properties(user_filter_library PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

        set_source_files_properties(${USER_FILTERS_DEF_FILE} PROPERTIES HEADER_FILE_ONLY TRUE)

        target_link_options(user_filter_library PRIVATE "/DEF:${USER_FILTERS_DEF_FILE}")

    else()
        # Provide a more specific error if one is missing.
        if(NOT AVL_LITE_LIBRARY OR NOT USER_FILTERS_LIBRARY)
            message(FATAL_ERROR "Could not find .lib in '${AVS_SDK_PATH}/SDK/lib/x64'!")
        endif()

        if(NOT USER_FILTERS_DEF_FILE)
            message(FATAL_ERROR "Could not find .def in '${AVS_SDK_PATH}/SDK/lib/x64'!")
        endif()
   
    endif()

endif()