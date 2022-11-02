#include "icsv_entity.hpp"
#include <imgui.h>

namespace ICSVapp {

void
IcsvEntity::AdjustScaleBy(double x, double y, double z) {
  m_node->setScale(m_node->getScale() + Ogre::Vector3(x, y, z));
}

void
IcsvEntity::SetPosition(double x, double y, double z) {
  m_node->setPosition(x, y, z);
}

void
IcsvEntity::SetScale(double x, double y, double z) {
  m_node->setScale(x, y, z);
}

void
IcsvEntity::ShowDetectorReport() {
  ImGui::Begin("Detector Report");
  ImGui::BeginMenu("");

  ImGui::MenuItem("Message: ");
  ImGui::SameLine();
  ImGui::MenuItem(m_report->message.c_str());
  ImGui::Separator();

  ImGui::MenuItem("Level: ");
  ImGui::SameLine();
  ImGui::MenuItem(std::to_string(m_report->level).c_str());
  ImGui::Separator();

  ImGui::MenuItem("File: ");
  ImGui::SameLine();
  ImGui::MenuItem(m_report->src_info.file.c_str());
  ImGui::Separator();

  ImGui::MenuItem("Line: ");
  ImGui::SameLine();
  ImGui::MenuItem(std::to_string(m_report->src_info.line).c_str());
  ImGui::Separator();

  ImGui::MenuItem("Column: ");
  ImGui::SameLine();
  ImGui::MenuItem(std::to_string(m_report->src_info.col).c_str());
  ImGui::Separator();

  ImGui::MenuItem("Structure: ");
  ImGui::SameLine();
  ImGui::MenuItem(m_report->src_info.strct.c_str());
  ImGui::Separator();

  ImGui::MenuItem("Method: ");
  ImGui::SameLine();
  ImGui::MenuItem(m_report->src_info.method.c_str());

  ImGui::End();
}

}  // namespace ICSVapp