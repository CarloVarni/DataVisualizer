cmake_minimum_required( VERSION 3.10 FATAL_ERROR )
project ( DataVisualizer )

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ROOT
list( APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS} )
find_package(ROOT REQUIRED COMPONENTS RIO Net ASImage Minuit)
include( ${ROOT_USE_FILE} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include )

# My Code
include_directories( header/EventDataModel
		     header/Core
		     header/Algorithms/Readers
		     header/Algorithms/Filters
		     header/Algorithms/HistogramMakers
		     header/Algorithms/Plotters )

SET( HEADER_FILES header/EventDataModel/Data.hpp 
     		  header/EventDataModel/MultiData.hpp
		  header/EventDataModel/Histogram_1D.hpp 
		  header/EventDataModel/Histogram_2D.hpp 
		  header/Core/IHolder.hpp
		  header/Core/EventContext.hpp
		  header/Core/BaseAlgorithm.hpp
		  header/Core/Scheduler.hpp
		  header/Core/Messager.hpp
		  header/Algorithms/Readers/SeedingPerformanceRootReaderAlgorithm.hpp
		  header/Algorithms/Readers/RootFileReaderAlgorithm.hpp
		  header/Algorithms/Readers/TxtFileReaderAlgorithm.hpp
		  header/Algorithms/Filters/FilteringAlgorithm.hpp
		  header/Algorithms/HistogramMakers/DataHistogramMakerAlgorithm.hpp
		  header/Algorithms/HistogramMakers/MultiDataHistogramMakerAlgorithm.hpp	
		  header/Algorithms/HistogramMakers/RootFileHistogramMakerAlgorithm.hpp
		  header/Algorithms/Plotters/PlotComparisonAlgorithm.hpp
		  header/Algorithms/Plotters/PlotterAlgorithm.hpp )

		  
SET( SRC_FILES src/Core/BaseAlgorithm.cpp
     	       src/Core/Scheduler.cpp
	       src/Algorithms/Readers/RootFileReaderAlgorithm.cpp
	       src/Algorithms/Readers/SeedingPerformanceRootReaderAlgorithm.cpp
	       src/Algorithms/HistogramMakers/DataHistogramMakerAlgorithm.cpp
	       src/Algorithms/HistogramMakers/MultiDataHistogramMakerAlgorithm.cpp
	       src/Algorithms/HistogramMakers/RootFileHistogramMakerAlgorithm.cpp
	       src/Algorithms/Plotters/PlotComparisonAlgorithm.cpp
	       src/Algorithms/Plotters/PlotterAlgorithm.cpp )


add_library( DataVisualizerLib SHARED ${SRC_FILES} ${HEADER_FILES} )

target_link_libraries( DataVisualizerLib ${ROOT_LIBRARIES} )
set_target_properties(DataVisualizerLib PROPERTIES LINKER_LANGUAGE CXX)

# Executables
add_executable( testRootReader macros/testRootReader.cpp)
target_link_libraries( testRootReader DataVisualizerLib )

add_executable( muon_efficiency_check macros/muon_efficiency_check.cpp )
target_link_libraries( muon_efficiency_check DataVisualizerLib )

add_executable( timing_studies_EDM macros/timing_studies_EDM.cpp )
target_link_libraries( timing_studies_EDM DataVisualizerLib )
