set(FILES 
  detector.hpp
  detector_manager.hpp
  architecture_holder.hpp
  report_center.hpp
  detector_report.hpp
)
set(SUBDIRECTORIES )

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()