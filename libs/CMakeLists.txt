set(SUBDIRECTORIES 
  detector_impls
  evaluator_impls
)

foreach(VAR ${SUBDIRECTORIES})
	add_subdirectory(${VAR})
endforeach()