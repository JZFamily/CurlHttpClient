#设置工程名
set(target "curl_http_client_testk8s")
set(OUTPUT_NAME "CurlHttpClientTestK8s")

#设置包含头文件路径
include_directories(${CURL_INCLUDE_PATH})
include_directories(${BOOST_INCLUDE_PATH})
include_directories(${NLOHMANN_INCLUDE_PATH})
include_directories("${PROJECT_SOURCE_DIR}/include")
#搜索文件，并设置工程目录
file(GLOB PUBLICS  "${PROJECT_SOURCE_DIR}/include/*.h")
source_group("public"  FILES  ${PUBLICS})

file(GLOB HEADERS  "*.h")
source_group("include"  FILES  ${HEADERS})


file(GLOB SOURCES  "*.cpp")
source_group("source"  FILES  ${SOURCES})

#汇总文件
set( ALL_LIST
${PUBLICS}
${HEADERS}
${SOURCES}
)
link_directories(${BOOST_LIBRARY_PATH})
#作为动态库
add_executable(${target} ${ALL_LIST})

#设置项目属性
set_target_properties(${target}  PROPERTIES ARCHIVE_OUTPUT_NAME "${OUTPUT_NAME}")
set_target_properties(${target}  PROPERTIES LIBRARY_OUTPUT_NAME "${OUTPUT_NAME}")
set_target_properties(${target}  PROPERTIES RUNTIME_OUTPUT_NAME "${OUTPUT_NAME}")
set_target_properties(${target}  PROPERTIES FOLDER "curl_http_client")

#设置链接库目录
message("${BOOST_LIBRARY_PATH}")

#设置链接库
if(WIN32)
target_link_libraries(${target}  curl_http_client)
else()
#
endif()