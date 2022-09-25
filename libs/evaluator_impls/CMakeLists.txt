set(FILES # Evaluator Interface Implementations
range_based_eval.hpp
range_based_eval.cpp
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()