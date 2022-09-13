#pragma once

#include <string>
#include <unordered_map>

#define REGISTER_EVALUATOR(tag, eval) \
  icsv::detector::EvaluationCenter::Get().RegisterEvaluator(tag, eval);

#define EVAL_SMELL(tag, curr_lvl) \
  icsv::detector::EvaluationCenter::Get().EvaluateSmell(tag, curr_lvl)

namespace icsv::detector {

class SmellEvaluator;

class EvaluationCenter final {
public:
  static auto Get() -> EvaluationCenter&;

  void RegisterEvaluator(const std::string& tag, SmellEvaluator* ev);

  void DeseriallizeConfig(const std::string& file_path);

  auto EvaluateSmell(const std::string& tag, unsigned curr_lvl)
      -> SmellEvaluator::SmellLevel;

private:
  using Evaluators = std::unordered_map<std::string, SmellEvaluator*>;

private:
  Evaluators m_eval_reg;

  EvaluationCenter()                        = default;
  EvaluationCenter(const EvaluationCenter&) = delete;
  EvaluationCenter(EvaluationCenter&&)      = delete;
  ~EvaluationCenter()                       = default;
};

}  // namespace icsv::detector