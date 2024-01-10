#include "./arg_based_eval.hpp"

MultiArgsEvaluator::~MultiArgsEvaluator() {
  for (auto ref : m_args) {
    delete ref.second;
    ref.second = nullptr;
  }
  m_args.clear();
}