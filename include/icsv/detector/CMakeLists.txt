set(FILES 
  detector.hpp
  detector_manager.hpp
  report_center.hpp
  detector_report.hpp
  smell_evaluator.hpp
  detector_config.hpp
)
set(SUBDIRECTORIES
  arch
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()