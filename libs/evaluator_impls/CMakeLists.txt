set(FILES # Evaluator Interface Implementations
  range_based_eval.cpp
  range_based_eval.hpp
  regex_based_eval.cpp
  regex_based_eval.hpp
)

if(FILES)
	target_sources(${PROJECT_NAME} PUBLIC ${FILES})
endif()