cmake_minimum_required (VERSION 3.8)

# SET C AND CXX COMPILER
if(UNIX)
	set (CMAKE_C_COMPILER "/usr/bin/gcc")
	set (CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

project ("RoboCupImageProcessingModule")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find packages
find_package(OpenCV 4 REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(glog CONFIG REQUIRED)

include_directories(
	${OpenCV_INCLUDE_DIRS}
	${YAML_CPP_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}/HSV_Filter
	${PROJECT_SOURCE_DIR}/Exceptions
	${PROJECT_SOURCE_DIR}/Detectors
	${PROJECT_SOURCE_DIR}/Monitor
	${PROJECT_SOURCE_DIR}/Monitor/OpenCV_Monitor
	${PROJECT_SOURCE_DIR}/Monitor/FrameBufferMonitor
	${PROJECT_SOURCE_DIR}/ThirdParty/Enum
	${PROJECT_SOURCE_DIR}/WorkingFlow
	${PROJECT_SOURCE_DIR}/Configs
	${PROJECT_SOURCE_DIR}/Configs/DetectorConfigs
	${PROJECT_SOURCE_DIR}/Configs/FruitDetectorConfigs
)

set(MainFiles ${PROJECT_SOURCE_DIR}/main.cpp)

aux_source_directory(${PROJECT_SOURCE_DIR}/HSV_Filter HSV_FilterFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Exceptions ExceptionFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Detectors DetectorFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Monitor/ MonitorFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Monitor/FrameBufferMonitor MonitorFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Monitor/OpenCV_Monitor MonitorFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/WorkingFlow WorkingFlowFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Configs ConfigFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Configs/DetectorConfigs DetectorConfigFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Configs/FruitDetectorConfigs FruitDetectorConfigFiles)

add_executable (RoboCupImageProcessingModule
	${MainFiles}
	${ExceptionFiles}
	${DetectorFiles}
	${HSV_FilterFiles}
	${MonitorFiles}
	${WorkingFlowFiles}
	${ConfigFiles}
	${DetectorConfigFiles}
	${FruitDetectorConfigFiles}
)

target_link_libraries(RoboCupImageProcessingModule yaml-cpp glog::glog ${OpenCV_LIBS})
