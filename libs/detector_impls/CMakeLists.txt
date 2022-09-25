set(FILES # Detector Interface Implementations
  # circular_dependency_det.cpp
  dependency_count_det.cpp
  function_overload_det.cpp
	large_file_det.cpp
	long_identifier_det.cpp
  method_args_det.cpp
  method_literals_det.cpp
  method_locals_det.cpp
	nested_scopes_det.cpp
  virtual_destructor_det.cpp
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()