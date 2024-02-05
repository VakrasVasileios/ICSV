#include <assert.h>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include "range_based_eval.hpp"
#include "regex_based_eval.hpp"
#include "arg_based_eval.hpp"
#include "icsv/detector/smell_evaluator.hpp"
#include "icsv/detector/evaluation_center.hpp"

namespace icsv::detector {

EvaluationCenter::~EvaluationCenter() {
#if 0
  m_config_file.replace(m_config_file.find(".json"), 6, "_edited.json");
  SeriallizeToFile(m_config_file);
#endif

  for (auto& ev : m_eval_reg)
    delete ev.second;
  m_eval_reg.clear();
}

auto
EvaluationCenter::Get() -> EvaluationCenter& {
  static EvaluationCenter singleton;
  return singleton;
}

auto
EvaluationCenter::GetEvaluator(const std::string& tag) -> SmellEvaluator* {
  assert(m_eval_reg.contains(tag));
  if (!m_eval_reg.contains(tag))
    return nullptr;

  return m_eval_reg.at(tag);
}

void
EvaluationCenter::RegisterEvaluator(const std::string& tag,
                                    SmellEvaluator*    ev) {
  if (m_eval_reg.find(tag) != m_eval_reg.end())
    std::cout << "Newly registered evaluator with tag \"" << tag
              << "\" overwrites existing one!\n";

  m_eval_reg[tag] = ev;
}

bool
EvaluationCenter::RemoveEvaluator(const std::string& tag) {
  return m_eval_reg.erase(tag);
}

void
EvaluationCenter::DeseriallizeConfig(const std::string& file_path) {
  m_config_file = file_path;
  std::ifstream file(file_path);
  assert(file.is_open());
  Json::Value config_doc;
  file >> config_doc;
  assert(config_doc.isObject());
  file.close();

  assert(config_doc["smells"].isArray());

  for (auto smell : config_doc["smells"]) {
    if (smell["type"] == "bool") {
      MakeBoolEval(smell);
    }
    if (smell["type"] == "range") {
      MakeRangeEval(smell);
    }
    if (smell["type"] == "regex") {
      MakeRegexEval(smell);
    }
    if (smell["type"] == "args") {
      MakeArgsEval(smell);
    }
  }
}

void
EvaluationCenter::DisplayEvalGui(void) {
  for (auto& e : m_eval_reg) {
    e.second->DisplayGui();
  }
}

auto
EvaluationCenter::MakeRegexEval(Json::Value smell) -> RegexEvaluator* {
  auto* eval = new RegexEvaluator(smell["tag"].asString());
  eval->SetDescription(smell["description"].asString());

  eval->SetRange(smell["range"]["min"].asInt(), smell["range"]["max"].asInt());

  for (auto reg : smell["regex_array"])
    eval->AddRegex(reg["tag"].asString(), reg["regex"].asString());

  for (auto fld : smell["fields"]) {
    auto fld_tag = fld["tag"].asString();
    eval->AddField(fld_tag);
    eval->AssignRegexToField(fld_tag, fld["assigned_regex"].asString());
  }

  return eval;
}

auto
EvaluationCenter::MakeRangeEval(Json::Value smell) -> RangeEvaluator* {
  auto* eval = new RangeEvaluator(smell["tag"].asString(),
                                  smell["range"]["min"].asInt(),
                                  smell["range"]["max"].asInt());
  eval->SetDescription(smell["description"].asString());

  return eval;
}

auto
EvaluationCenter::MakeBoolEval(Json::Value smell) -> RangeEvaluator* {
  auto* eval = new RangeEvaluator(smell["tag"].asString(), 0, 1);
  eval->SetDescription(smell["description"].asString());

  return eval;
}

auto
EvaluationCenter::MakeArgsEval(Json::Value smell) -> MultiArgsEvaluator* {
  auto* eval = new MultiArgsEvaluator(smell["tag"].asString());
  eval->SetDescription(smell["description"].asString());

  for (auto arg : smell["args"]) {
    if (arg["type"] == "bool") {
      auto* arg_e = MakeBoolEval(arg);
      m_eval_reg.erase(arg_e->GetTag());
      assert(m_eval_reg.contains(arg_e->GetTag()) == false);

      eval->AddEvaluator(arg_e);
    }
    if (arg["type"] == "range") {
      auto* arg_e = MakeRangeEval(arg);
      m_eval_reg.erase(arg_e->GetTag());
      assert(m_eval_reg.contains(arg_e->GetTag()) == false);

      eval->AddEvaluator(arg_e);
    }
    if (arg["type"] == "regex") {
      auto* arg_e = MakeRegexEval(arg);
      m_eval_reg.erase(arg_e->GetTag());
      assert(m_eval_reg.contains(arg_e->GetTag()) == false);

      eval->AddEvaluator(arg_e);
    }
  }

  return eval;
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

void
EvaluationCenter::SeriallizeToFile(const std::string& file_name) {
  std::ofstream file;
  file.open(file_name);

  if (file.is_open()) {
    file << "{\n\"smells\":[\n";
    bool init = false;

    for (auto& e : m_eval_reg) {
      if (init)
        file << ",\n";
      file << "{\n";
      file << e.second->Seriallize();
      file << "}";
      init = true;
    }
    file << "]\n}";

    file.close();
  } else {
    std::cout
        << "Could not seriallize evaluation configurations to given file: "
        << file_name << std::endl;
  }
}

}  // namespace icsv::detector
