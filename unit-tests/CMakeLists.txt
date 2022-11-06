set(FILES
	arch_deserialization.cpp
	smell_detectors.cpp
	tests.cpp
)

find_package(GTest REQUIRED)

set(TESTS ${PROJECT_NAME}_tests)
set(TESTS_LIBS gtest gmock)

add_executable(${TESTS} tests.cpp)
target_link_libraries(${TESTS} PRIVATE ${PROJECT_NAME} ${TESTS_LIBS})

if(FILES)
	target_sources(${TESTS} PUBLIC ${FILES})
endif()