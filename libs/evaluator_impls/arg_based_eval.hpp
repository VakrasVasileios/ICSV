#pragma once

#include "icsv/detector/smell_evaluator.hpp"
#include "icsv/detector/evaluation_center.hpp"

template <class T>
concept Teval = std::derived_from<T, icsv::detector::ISmellEvaluator>;

template <Teval T, typename... Targs>
class ArgEvaluator : public T {
public:
  ArgEvaluator(Targs... args) : T(args...) {
    icsv::detector::remove_evaluator(this->m_tag);
  }
  virtual ~ArgEvaluator() override = default;
};

class MultiArgsEvaluator final : icsv::detector::ISmellEvaluator {
public:
  MultiArgsEvaluator(const std::string& tag);
  ~MultiArgsEvaluator() override;

private:
  using ArgEvalReg = std::map<std::string, ISmellEvaluator*>;

private:
  ArgEvalReg m_args;
};