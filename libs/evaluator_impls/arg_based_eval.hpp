#pragma once

#include "icsv/detector/smell_evaluator.hpp"
#include <map>

class MultiArgsEvaluator final : public icsv::detector::SmellEvaluator {
public:
  MultiArgsEvaluator(const std::string& tag);
  ~MultiArgsEvaluator() override;

  auto EvaluateSmell(const std::string& eval_name, int init_lvl) -> SmellLevel;
  auto EvaluateSmell(const std::string& eval_name,
                     const std::string& field,
                     const std::string& str) -> SmellLevel;

  auto EvaluateSmell(int) -> SmellLevel override { return -1; }
  auto ReEvaluateSmell(int) -> SmellLevel override { return -1; }  // TODO

  void DisplayGui(void) override {}

  void AddEvaluator(icsv::detector::SmellEvaluator* eval);

private:
  using ArgEvalReg = std::map<std::string, SmellEvaluator*>;

private:
  ArgEvalReg m_args;
};