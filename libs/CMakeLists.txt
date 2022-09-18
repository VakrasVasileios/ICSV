set(FILES # Detector Interface Implementations
	large_file_smell.cpp
	long_identifier_smell.cpp
	nested_scopes_smell.cpp
	dependency_count_smell.cpp

	range_based_smell_eval.hpp
	range_based_smell_eval.cpp
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()