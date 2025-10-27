# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_ePaper_SRCS 变量
aux_source_directory(. DIR_MP28167_SRCS)

include_directories(../Config)

# 生成链接库
add_library(MP28167 ${DIR_MP28167_SRCS})
target_link_libraries(MP28167 PUBLIC Config)