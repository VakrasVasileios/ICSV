set(FILES # Code-Smell detector extensions
	large_file_smell.cpp
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()