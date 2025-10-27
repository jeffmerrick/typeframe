# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_ePaper_SRCS 变量
aux_source_directory(. DIR_Power_Management_Lib_SRCS)

include_directories(../Config)
include_directories(../INA219)
include_directories(../PCF85063A)
# 生成链接库
add_library(Power_Management_Lib ${DIR_Power_Management_Lib_SRCS})
target_link_libraries(Power_Management_Lib PUBLIC Config PCF85063A INA219 pico_multicore)