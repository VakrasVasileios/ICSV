set(FILES 
  detector_manager.cpp
	architecture_holder.cpp
	report_center.cpp
	smell_evaluator.cpp
)
set(SUBDIRECTORIES )

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()