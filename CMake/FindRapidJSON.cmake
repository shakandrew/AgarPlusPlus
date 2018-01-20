find_path(RAPIDJSON_INCLUDE_DIR
        NAMES
        "rapidjson/rapidjson.h"
        PATHS
        ${RAPIDJSON_ROOT}/include
        )

if (NOT RAPIDJSON_INCLUDE_DIR)
    message(FATAL_ERROR "RapidJson headers NOT found.")
    set(RAPIDJSON_FOUND FALSE)
else()
    message(STATUS "RapidJson headers found.")
    set(RAPIDJSON_FOUND TRUE)
endif ()

mark_as_advanced(
        RAPIDJSON_INCLUDE_DIR
)
