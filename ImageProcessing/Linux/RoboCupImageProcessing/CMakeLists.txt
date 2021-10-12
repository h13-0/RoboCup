cmake_minimum_required (VERSION 3.8)

# SET C AND CXX COMPILER
if(UNIX)
	set (CMAKE_C_COMPILER "/usr/bin/gcc")
	set (CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

project ("RoboCupImageProcessingModule")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find packages
find_package(Threads REQUIRED)
find_package(OpenCV 4 REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
## find_package(glog CONFIG REQUIRED)
include(${PROJECT_SOURCE_DIR}/CMake/FindGlog.cmake)
if(NOT GLOG_FOUND)
	if(WIN32)
		message(FATAL_ERROR "glog not found, try \"vcpkg install glog\"")
	elseif(UNIX)
		message(FATAL_ERROR "glog not found, try \"sudo apt-get install libgoogle-glog-dev\"")
	endif()
endif()

find_package(gflags CONFIG REQUIRED)


if(WIN32)

elseif(UNIX)
	set(PERIPHERY_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/ThirdParty/c-periphery/src)
	aux_source_directory(${PROJECT_SOURCE_DIR}/ThirdParty/c-periphery/src PERIPHERY_SOURCE_FILES)
	set(${PERIPHERY_LIBRARIES} "")
endif()

include_directories(
	${OpenCV_INCLUDE_DIRS}
	${YAML_CPP_INCLUDE_DIR}
	${GLOG_INCLUDE_DIRS}
	${PERIPHERY_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/HSV_Filter
	${PROJECT_SOURCE_DIR}/Exceptions
	${PROJECT_SOURCE_DIR}/Detectors
	${PROJECT_SOURCE_DIR}/Monitor
	${PROJECT_SOURCE_DIR}/Monitor/OpenCV_Monitor
	${PROJECT_SOURCE_DIR}/Monitor/FrameBufferMonitor
	${PROJECT_SOURCE_DIR}/ThirdParty/Enum
	${PROJECT_SOURCE_DIR}/WorkingFlow
	${PROJECT_SOURCE_DIR}/Configs
	${PROJECT_SOURCE_DIR}/Configs/ApplicationConfigs
	${PROJECT_SOURCE_DIR}/Configs/DetectorConfigs
	${PROJECT_SOURCE_DIR}/Configs/FruitDetectorConfigs
	${PROJECT_SOURCE_DIR}/Protocol
	${PROJECT_SOURCE_DIR}/Protocol/Serial
)

set(MainFiles ${PROJECT_SOURCE_DIR}/main.cpp)

aux_source_directory(${PROJECT_SOURCE_DIR}/HSV_Filter HSV_FilterFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Exceptions ExceptionFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Detectors DetectorFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Monitor/FrameBufferMonitor MonitorFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Monitor/OpenCV_Monitor MonitorFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/WorkingFlow WorkingFlowFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Configs ConfigFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Configs/ApplicationConfigs ApplicationConfigFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Configs/DetectorConfigs DetectorConfigFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Configs/FruitDetectorConfigs FruitDetectorConfigFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Protocol/Serial SerialProtocolFiles)

add_executable (RoboCupImageProcessingModule
	${MainFiles}
	${ExceptionFiles}
	${DetectorFiles}
	${HSV_FilterFiles}
	${MonitorFiles}
	${WorkingFlowFiles}
	${ConfigFiles}
	${ApplicationConfigFiles}
	${DetectorConfigFiles}
	${FruitDetectorConfigFiles}
	${PERIPHERY_SOURCE_FILES}
	${SerialProtocolFiles}
)

target_link_libraries(RoboCupImageProcessingModule
	yaml-cpp
	${GLOG_LIBRARIES}
	${OpenCV_LIBS}
	gflags
	${PERIPHERY_LIBRARIES}
	Threads::Threads
)
