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
find_package(yaml-cpp REQUIRED)

include_directories(
	${OpenCV_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/HSV_Filter
	${PROJECT_SOURCE_DIR}/Detectors
	${PROJECT_SOURCE_DIR}/Monitor/FrameBufferMonitor
	${PROJECT_SOURCE_DIR}/ThirdParty/Enum
	${PROJECT_SOURCE_DIR}/WorkingFlow
)

set(MainFiles ${PROJECT_SOURCE_DIR}/main.cpp)

aux_source_directory(${PROJECT_SOURCE_DIR}/HSV_Filter HSV_FilterFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Detectors DetectorFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Monitor/FrameBufferMonitor FrameBufferMonitorFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/Monitor/OpenCV_Monitor OpenCV_MonitorFiles)
aux_source_directory(${PROJECT_SOURCE_DIR}/main.cpp WorkingFlowFiles)

add_executable (RoboCupImageProcessingModule 
	${MainFiles}
	${DetectorFiles}
	${HSV_FilterFiles}
	${FrameBufferMonitorFiles}
	${OpenCV_MonitorFiles}
	${WorkingFlowFiles}
)

target_link_libraries(RoboCupImageProcessingModule ${OpenCV_LIBS})
