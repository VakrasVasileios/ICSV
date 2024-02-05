#pragma once

#include <string>
#include <map>
#include "smell_evaluator.hpp"

class RegexEvaluator;
class RangeEvaluator;
class MultiArgsEvaluator;

namespace icsv::detector {

class EvaluationCenter final {
public:
  static auto Get() -> EvaluationCenter&;

  void RegisterEvaluator(const std::string& tag, SmellEvaluator* ev);

  bool RemoveEvaluator(const std::string& tag);

  void DeseriallizeConfig(const std::string& file_path);

  auto EvaluateSmell(const std::string& tag, int curr_lvl)
      -> SmellEvaluator::SmellLevel;

  void DisplayEvalGui(void);

  auto GetEvaluator(const std::string& tag) -> SmellEvaluator*;

  void SeriallizeToFile(const std::string& file_name);

private:
  using Evaluators = std::map<std::string, SmellEvaluator*>;

private:
  Evaluators  m_eval_reg;
  std::string m_config_file{ "" };

  EvaluationCenter()                        = default;
  EvaluationCenter(const EvaluationCenter&) = delete;
  EvaluationCenter(EvaluationCenter&&)      = delete;
  ~EvaluationCenter();

  auto MakeRegexEval(Json::Value smell) -> RegexEvaluator*;
  auto MakeRangeEval(Json::Value smell) -> RangeEvaluator*;
  auto MakeBoolEval(Json::Value smell) -> RangeEvaluator*;
  auto MakeArgsEval(Json::Value smell) -> MultiArgsEvaluator*;
};

inline void
deseriallize_detector_config(const std::string& path) {
  EvaluationCenter::Get().DeseriallizeConfig(path);
}

inline auto
evaluate_smell(const std::string& tag, int curr_lvl)
    -> SmellEvaluator::SmellLevel {
  return EvaluationCenter::Get().EvaluateSmell(tag, curr_lvl);
}

inline void
register_evaluator(const std::string& tag, SmellEvaluator* ev) {
  EvaluationCenter::Get().RegisterEvaluator(tag, ev);
}

inline bool
remove_evaluator(const std::string& tag) {
  return EvaluationCenter::Get().RemoveEvaluator(tag);
}

}  // namespace icsv::detector