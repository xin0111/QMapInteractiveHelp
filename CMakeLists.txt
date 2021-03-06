cmake_minimum_required(VERSION 3.10)

#指定Qt路径(按需设置)
#SET(CMAKE_PREFIX_PATH ${QT_PATH}/lib/cmake)

#设置sln名称
project(qmapinteractivehelp)
# 关闭控制台 ：+ WIN32
set(QT_USE_QTMAIN TRUE) 
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON) 
# 自动生成开关
set(CMAKE_AUTOMOC ON) 
#set(CMAKE_AUTOUIC ON) 
#set(CMAKE_AUTORCC ON) 		
# 查找lib
find_package(Qt5Core QUIET) 
find_package(Qt5Gui QUIET) 
find_package(Qt5Widgets QUIET)  	

add_definitions(-DQMAPINTERACTIVE_LIB)

# 查找 source
set(src_dir .)
foreach(dir ${src_dir})
	string(TOUPPER "${dir}" dir_u)			
	file(GLOB ${dir_u}_UI_FILES ${dir}/*.ui)
	file(GLOB ${dir_u}_H_FILES ${dir}/*.h)
	file(GLOB ${dir_u}_CXX_FILES ${dir}/*.cpp)	
	list(APPEND UI_FILES ${${dir_u}_UI_FILES})		
	list(APPEND ${PROJECT_NAME}_SRCS ${${dir_u}_H_FILES})
	list(APPEND HEADER_SRCS ${${dir_u}_CXX_FILES})
endforeach(dir ${src_dir})

# 自动生成的 mocs_compilation.cpp 包含所有配置的moc_*.cpp		
# ui_*.h 文件
#qt5_wrap_ui( UIC_SRCS ${UI_FILES}) 
# qrc_*.cpp 文件 注：不存在时需要注释掉
#set(RESOURCE_FILES ${PROJECT_NAME}.qrc) 
#qt5_add_resources(RCC_SRCS ${RESOURCE_FILES}) 

# 添加 source
add_library(${PROJECT_NAME} SHARED
				${${PROJECT_NAME}_SRCS}
				${HEADER_SRCS}
				${UIC_SRCS} 
				${RCC_SRCS})
				
# 依赖
target_link_libraries(${PROJECT_NAME} 
				Qt5::Core	
				Qt5::Gui
				Qt5::Widgets		
				)	
				
# Filter 设置				
source_group("Form Files" FILES ${UI_FILES})
source_group("Generated Files" FILES ${UIC_SRCS} ${RCC_SRCS} )
source_group("Resources" FILES ${RESOURCE_FILES})

# 运行环境
set(RUN_ENVIROMENT $(QTDIR)/bin%3b)					
set_target_properties(${PROJECT_NAME}   PROPERTIES
	VS_DEBUGGER_ENVIRONMENT "PATH=${RUN_ENVIROMENT}$(PATH)")

