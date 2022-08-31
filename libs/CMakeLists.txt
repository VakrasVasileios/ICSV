set(FILES # Detector Interface Implementations
	large_file_smell.cpp
	long_identifier_smell.cpp
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()