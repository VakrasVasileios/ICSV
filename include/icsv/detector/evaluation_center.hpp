#pragma once

#include <string>
#include <unordered_map>

namespace icsv::detector {

class ISmellEvaluator;

class EvaluationCenter final {
public:
  using SmellLevel = int;

public:
  static auto Get() -> EvaluationCenter&;

  void RegisterEvaluator(const std::string& tag, ISmellEvaluator* ev);

  void DeseriallizeConfig(const std::string& file_path);

  auto EvaluateSmell(const std::string& tag, int curr_lvl) -> SmellLevel;

  void DisplayEvalGui(void);

private:
  using Evaluators = std::unordered_map<std::string, ISmellEvaluator*>;

private:
  Evaluators m_eval_reg;

  EvaluationCenter()                        = default;
  EvaluationCenter(const EvaluationCenter&) = delete;
  EvaluationCenter(EvaluationCenter&&)      = delete;
  ~EvaluationCenter();
};

inline void
deserialize_det_conf(const std::string& path) {
  EvaluationCenter::Get().DeseriallizeConfig(path);
}

inline auto
evaluate_smell(const std::string& tag, int curr_lvl)
    -> EvaluationCenter::SmellLevel {
  return EvaluationCenter::Get().EvaluateSmell(tag, curr_lvl);
}

inline void
register_evaluator(const std::string& tag, ISmellEvaluator* ev) {
  EvaluationCenter::Get().RegisterEvaluator(tag, ev);
}

}  // namespace icsv::detector