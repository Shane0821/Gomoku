cmake_minimum_required (VERSION 3.14)

project (GOMOKU)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED true)


# ============================
# Define directories
# ============================

set (DIR_SRC "./src")
set (DIR_JSON "./jsoncpp")

# ============================
# Includes
# ============================

include_directories (
    ${DIR_SRC}
    ${DIR_JSON}
)

# ============================
# Build
# ============================

aux_source_directory (${DIR_SRC} SRC)
aux_source_directory (${DIR_JSON} JSONCPP)

add_executable(gomoku main.cpp ${JSONCPP} ${SRC})

