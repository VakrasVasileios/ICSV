set(FILES 
  detector_manager.cpp
	report_center.cpp
	smell_evaluator.cpp
	evaluation_center.cpp
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