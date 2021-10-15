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
		     header/Algorithms/GraphMakers
		     header/Algorithms/ProfileMakers
		     header/Algorithms/EfficiencyMakers
		     header/Algorithms/Plotters )

SET( HEADER_FILES header/EventDataModel/Data.hpp 
     		  header/EventDataModel/MultiData.hpp
		  header/EventDataModel/Histogram_1D.hpp 
		  header/EventDataModel/Histogram_2D.hpp 
		  header/EventDataModel/Efficiency.hpp
		  header/EventDataModel/Graph.hpp
		  header/EventDataModel/Profile.hpp
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
		  header/Algorithms/GraphMakers/MultiDataGraphMakerAlgorithm.hpp
		  header/Algorithms/ProfileMakers/RootFileProfileMakerAlgorithm.hpp
		  header/Algorithms/EfficiencyMakers/RootFileEfficiencyMakerAlgorithm.hpp
		  header/Algorithms/Plotters/ComparisonPlotterAlgorithm.hpp
		  header/Algorithms/Plotters/HistogramComparisonPlotterAlgorithm.hpp
		  header/Algorithms/Plotters/GraphComparisonPlotterAlgorithm.hpp
		  header/Algorithms/Plotters/EfficiencyComparisonPlotterAlgorithm.hpp
		  header/Algorithms/Plotters/ProfileComparisonPlotterAlgorithm.hpp
		  header/Algorithms/Plotters/PlotterAlgorithm.hpp
		  header/Algorithms/Plotters/ProfilePlotterAlgorithm.hpp
		  header/Algorithms/Plotters/GraphPlotterAlgorithm.hpp
		  header/Algorithms/Plotters/Histogram1DPlotterAlgorithm.hpp
		  header/Algorithms/Plotters/Histogram2DPlotterAlgorithm.hpp
		  header/Algorithms/Plotters/Efficiency1DPlotterAlgorithm.hpp
		  header/Algorithms/Plotters/Efficiency2DPlotterAlgorithm.hpp
		  )

		  
SET( SRC_FILES src/EventDataModel/Efficiency.cpp
     	       src/EventDataModel/Profile.cpp
     	       src/Core/BaseAlgorithm.cpp
     	       src/Core/Scheduler.cpp
	       src/Core/EventContext.cpp
	       src/Algorithms/Readers/RootFileReaderAlgorithm.cpp
	       src/Algorithms/Readers/SeedingPerformanceRootReaderAlgorithm.cpp
	       src/Algorithms/HistogramMakers/DataHistogramMakerAlgorithm.cpp
	       src/Algorithms/HistogramMakers/MultiDataHistogramMakerAlgorithm.cpp
	       src/Algorithms/HistogramMakers/RootFileHistogramMakerAlgorithm.cpp
	       src/Algorithms/GraphMakers/MultiDataGraphMakerAlgorithm.cpp
	       src/Algorithms/ProfileMakers/RootFileProfileMakerAlgorithm.cpp
	       src/Algorithms/EfficiencyMakers/RootFileEfficiencyMakerAlgorithm.cpp
	       src/Algorithms/Plotters/ProfilePlotterAlgorithm.cpp
	       src/Algorithms/Plotters/GraphPlotterAlgorithm.cpp
	       src/Algorithms/Plotters/Histogram1DPlotterAlgorithm.cpp
	       src/Algorithms/Plotters/Histogram2DPlotterAlgorithm.cpp
	       src/Algorithms/Plotters/HistogramComparisonPlotterAlgorithm.cpp	
	       src/Algorithms/Plotters/GraphComparisonPlotterAlgorithm.cpp
	       src/Algorithms/Plotters/EfficiencyComparisonPlotterAlgorithm.cpp
	       src/Algorithms/Plotters/ProfileComparisonPlotterAlgorithm.cpp
	       src/Algorithms/Plotters/Efficiency1DPlotterAlgorithm.cpp
	       src/Algorithms/Plotters/Efficiency2DPlotterAlgorithm.cpp
	       )


add_library( DataVisualizerLib SHARED ${SRC_FILES} ${HEADER_FILES} )

target_link_libraries( DataVisualizerLib ${ROOT_LIBRARIES} )
set_target_properties(DataVisualizerLib PROPERTIES LINKER_LANGUAGE CXX)



# Executables
#add_executable( testRootReader macros/testRootReader.cpp)
#target_link_libraries( testRootReader DataVisualizerLib )

#add_executable( muon_efficiency_check macros/muon_efficiency_check.cpp )
#target_link_libraries( muon_efficiency_check DataVisualizerLib )

#add_executable( timing_studies_EDM macros/timing_studies_EDM.cpp )
#target_link_libraries( timing_studies_EDM DataVisualizerLib )

#add_executable( seed_finding_timing macros/seed_finding_timing.cxx )
#target_link_libraries( seed_finding_timing DataVisualizerLib )

#add_executable( efficiency_vs_nparticles macros/efficiency_vs_nparticles.cxx )
#target_link_libraries( efficiency_vs_nparticles DataVisualizerLib )

add_executable( ckf_performance macros/ckf_performance.cxx )
target_link_libraries( ckf_performance DataVisualizerLib )

add_executable( tracking_performance macros/tracking_performance.cxx )
target_link_libraries( tracking_performance DataVisualizerLib )
