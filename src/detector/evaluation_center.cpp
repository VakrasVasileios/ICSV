#include "icsv/detector/evaluation_center.hpp"
#include "icsv/detector/smell_evaluator.hpp"
#include <assert.h>
#include <fstream>
#include <iostream>
#include <json/json.h>

namespace icsv::detector {

EvaluationCenter::~EvaluationCenter() {
  for (auto& ev : m_eval_reg)
    delete ev.second;
  m_eval_reg.clear();
}

auto
EvaluationCenter::Get() -> EvaluationCenter& {
  static EvaluationCenter singleton;
  return singleton;
}

void
EvaluationCenter::RegisterEvaluator(const std::string& tag,
                                    SmellEvaluator*    ev) {
  if (m_eval_reg.find(tag) != m_eval_reg.end())
    std::cout << "Newly registered evaluator with tag \"" << tag
              << "\" overwrites existing one!\n";

  m_eval_reg[tag] = ev;
}

void
EvaluationCenter::DeseriallizeConfig(const std::string& file_path) {
  Json::Value   doc;
  std::ifstream file(file_path);
  assert(file.is_open());
  file >> doc;
  assert(doc.isObject());
  file.close();

  for (auto& eval : m_eval_reg)
    (eval.second)->DeserializeConfig(doc);
}

auto
EvaluationCenter::EvaluateSmell(const std::string& tag, const int curr_lvl)
    -> SmellEvaluator::SmellLevel {
  if (m_eval_reg.find(tag) == m_eval_reg.end()) {
    std::cout << "Could not find evaluator with tag \"" << tag
              << "\", returning current level.";
    return curr_lvl;
  } else
    return m_eval_reg[tag]->EvaluateSmell(curr_lvl);
}

}  // namespace icsv::detector
