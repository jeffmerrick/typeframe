# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_ePaper_SRCS 变量
aux_source_directory(. DIR_INA219_SRCS)

include_directories(../Config)

# 生成链接库
add_library(INA219 ${DIR_INA219_SRCS})
target_link_libraries(INA219 PUBLIC Config)