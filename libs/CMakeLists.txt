set(FILES # Detector Interface Implementations
  # circular_dependency_smell.cpp
  dependency_count_smell.cpp
  function_overload_smell.cpp
	large_file_smell.cpp
	long_identifier_smell.cpp
  method_args_smell.cpp
  method_literals_smell.cpp
  method_locals_smell.cpp
	nested_scopes_smell.cpp
  virtual_destructor_smell.cpp
)

set(FILES # Evaluator Interface Implementations
  range_based_smell_eval.hpp
  range_based_smell_eval.cpp
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()