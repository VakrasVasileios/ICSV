#include "gui.hpp"
#include "icsv/detector/arch/architecture_holder.hpp"
#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include "icsv/detector/report_center.hpp"
#include "../entity_manager.hpp"
#include "../material_factory.hpp"

namespace ICSVapp {

#define BUFFSIZE 400

auto
IcsvGui::Get() -> IcsvGui& {
  static IcsvGui singleton;
  return singleton;
}

void
IcsvGui::Display(void) {
  ImGui::Begin("Menu");
  if (ImGui::CollapsingHeader("Smell Configuration")) {
    ShowConfigSelect();
    ImGui::Separator();
  }
  if (ImGui::CollapsingHeader("Export")) {
    ShowExportTab();
    ImGui::Separator();
  }
  if (ImGui::CollapsingHeader("Camera Settings")) {
    ShowCameraSettings();
    ImGui::Separator();
  }
  if (ImGui::CollapsingHeader("Environment Settings")) {
    ShowSkyboxSettings();
    ImGui::Separator();
  }
  if (ImGui::CollapsingHeader("Sort Axises")) {
    ShowSortingSettings();
    ImGui::Separator();
  }
  if (ImGui::CollapsingHeader("Smell Color Pallet")) {
    ShowSmellColorPallet();
    ImGui::Separator();
  }
  if (ImGui::CollapsingHeader("Smell Eval Configs")) {
    ShowEvalConfigs();
    ImGui::Separator();
  }

  ShowDetectorReport();
  ImGui::End();
}

void
IcsvGui::ShowConfigSelect(void) {
  static std::string det_conf_file   = "";
  static std::string graph_conf_file = "";
  static char        graph_buf[BUFFSIZE];
  static char        det_buf[BUFFSIZE];
  static bool        changed = false;

  ImGui::Text("Dectector Config File");
  ImGui::InputText("Config", det_buf, BUFFSIZE);
  if (ImGui::Button("Select", ImVec2(100, 20))) {
    m_conf_brwsr.SetTitle("Select Dectector Config");
    m_conf_brwsr.Open();
  }
  if (m_conf_brwsr.HasSelected()) {
    std::strncpy(det_buf, det_conf_file.c_str(), det_conf_file.size());
    if (m_conf_brwsr.GetSelected().c_str() != det_conf_file) {
      det_conf_file = m_conf_brwsr.GetSelected();
      icsv::detector::deseriallize_detector_config(m_conf_brwsr.GetSelected());
      MaterialFactory::Get().DeseriallizeConfig(m_conf_brwsr.GetSelected());
      changed = true;
    }
  }

  ImGui::Separator();

  ImGui::Text("Architecture Graph File");
  ImGui::InputText("Graph", graph_buf, BUFFSIZE);
  if (ImGui::Button("Select ", ImVec2(100, 20))) {
    m_graph_brwsr.SetTitle("Select Architecture Graph");
    m_graph_brwsr.Open();
  }
  if (m_graph_brwsr.HasSelected()) {
    std::strncpy(graph_buf, graph_conf_file.c_str(), graph_conf_file.size());
    if (m_graph_brwsr.GetSelected().c_str() != graph_conf_file) {
      graph_conf_file = m_graph_brwsr.GetSelected();
      icsv::detector::arch::deseriallize_arch_conf(m_graph_brwsr.GetSelected());
      changed = true;
    }
  }
  m_conf_brwsr.Display();
  m_graph_brwsr.Display();

  ShowSmellButton(changed);
}

void
IcsvGui::ShowDetectorReport(void) {
  if (m_rep_dspld != nullptr) {
    ImGui::Begin("Detector Report");
    ImGui::MenuItem("Smell Tag: ");
    ImGui::SameLine();
    ImGui::Text("%s", m_rep_dspld->smell_tag.c_str());
    ImGui::Separator();

    ImGui::MenuItem("Message: ");
    ImGui::SameLine();
    ImGui::Text("%s", m_rep_dspld->message.c_str());
    ImGui::Separator();

    ImGui::MenuItem("Level: ");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(m_rep_dspld->level).c_str());
    ImGui::Separator();

    ImGui::MenuItem("File: ");
    ImGui::SameLine();
    ImGui::Text("%s", m_rep_dspld->src_info.file.c_str());
    ImGui::Separator();

    ImGui::MenuItem("Line: ");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(m_rep_dspld->src_info.line).c_str());
    ImGui::Separator();

    ImGui::MenuItem("Column: ");
    ImGui::SameLine();
    ImGui::Text("%s", std::to_string(m_rep_dspld->src_info.col).c_str());
    ImGui::Separator();

    ImGui::MenuItem("Structure: ");
    ImGui::SameLine();
    ImGui::Text("%s", m_rep_dspld->src_info.strct.c_str());
    ImGui::Separator();

    ImGui::MenuItem("Method: ");
    ImGui::SameLine();
    ImGui::Text("%s", m_rep_dspld->src_info.method.c_str());
  }
  ImGui::End();
}

void
IcsvGui::ShowExportTab(void) {
  static char evals[BUFFSIZE];
  static char reps[BUFFSIZE];
  static int  lvl_min = 0, lvl_max = 10;

  ImGui::InputText("File", evals, BUFFSIZE);
  if (ImGui::Button("Export Evaluator Configs")) {
    icsv::detector::EvaluationCenter::Get().SeriallizeToFile(evals);
  }

  ImGui::Separator();

  ImGui::InputText("File##2", reps, BUFFSIZE);
  ImGui::DragIntRange2("Smell Level Range",
                       &lvl_min,
                       &lvl_max,
                       1,
                       0,
                       10,
                       "%d",
                       NULL,
                       ImGuiSliderFlags_AlwaysClamp);
  if (ImGui::Button("Export Reports")) {
    icsv::detector::ReportCenter::Get().SeriallizeToFile(reps,
                                                         lvl_min,
                                                         lvl_max);
  }
}

void
IcsvGui::ShowSmellButton(bool& changed) {
  if (ImGui::Button("Evaluate Code", ImVec2(250, 50))) {
    if (changed) {
      m_rep_dspld = nullptr;
      icsv::detector::clear_reports();
      clear_entities();
      icsv::detector::use_smell_detectors();
      const auto& replst = icsv::detector::ReportCenter::Get().GetReportList();

      double x = 0, z = 0;
      auto   tag = replst.front()->smell_tag;
      for (auto* rep : replst) {
        if (tag != rep->smell_tag) {
          x++;
          z   = 0;
          tag = rep->smell_tag;
        }
        if (rep->level > 0) {
          auto   unif_scale = 0.2;
          double y          = unif_scale * rep->level / 2;
          create_icsv_entity(rep,
                             Ogre::Vector3f(x, y / 2, z),
                             Ogre::Vector3f(unif_scale, y, unif_scale));
          z++;
        }
      }
      changed = false;
    }
  }
}

void
IcsvGui::ShowSkyboxSettings(void) {
  ImGui::Text("Skybox Color:");
  static bool   alpha        = true;
  static bool   alpha_bar    = true;
  static bool   side_preview = true;
  static int    display_mode = 0;
  static int    picker_mode  = 0;
  static ImVec4 color        = ImVec4(114.0f / 255.0f,
                               144.0f / 255.0f,
                               154.0f / 255.0f,
                               200.0f / 255.0f);

  ImGui::Checkbox("With Alpha", &alpha);
  ImGui::Checkbox("With Alpha Bar", &alpha_bar);
  ImGui::Checkbox("With Side Preview", &side_preview);

  ImGui::Combo("Display Mode",
               &display_mode,
               "Auto/Current\0None\0RGB Only\0HSV Only\0Hex Only\0");
  ImGuiColorEditFlags flags = 0;
  if (!alpha)
    flags |= ImGuiColorEditFlags_NoAlpha;  // This is by default if you call
                                           // ColorPicker3() instead of
                                           // ColorPicker4()
  if (alpha_bar)
    flags |= ImGuiColorEditFlags_AlphaBar;
  if (!side_preview)
    flags |= ImGuiColorEditFlags_NoSidePreview;
  if (picker_mode == 1)
    flags |= ImGuiColorEditFlags_PickerHueBar;
  if (picker_mode == 2)
    flags |= ImGuiColorEditFlags_PickerHueWheel;
  if (display_mode == 1)
    flags |= ImGuiColorEditFlags_NoInputs;  // Disable all RGB/HSV/Hex displays
  if (display_mode == 2)
    flags |= ImGuiColorEditFlags_DisplayRGB;  // Override display mode
  if (display_mode == 3)
    flags |= ImGuiColorEditFlags_DisplayHSV;
  if (display_mode == 4)
    flags |= ImGuiColorEditFlags_DisplayHex;
  ImGui::ColorPicker4("MyColor##4", (float*) &color, flags, NULL);
}

void
IcsvGui::ShowCameraSettings(void) {
  ImGui::Separator();
  ImGui::InputFloat("Camera Speed", m_cam_data.cam_speed);
  ImGui::Separator();
  ImGui::InputFloat("Camera Rotation Speed", m_cam_data.rot_smooth);
  ImGui::Separator();
}

std::list<Selectable*> sort_method_list
    = { new Selectable("Smell Tag"), new Selectable("Message"),
        new Selectable("Level"),     new Selectable("File"),
        new Selectable("Structure"), new Selectable("Method") };

#define X_LABEL "Sort X axis by: "
#define Z_LABEL "Sort Z axis by: "

void
IcsvGui::ShowSortingSettings(void) {
  static Selectable *x = nullptr, *z = nullptr;
  static std::string x_label = X_LABEL;
  ImGui::BeginListBox(x_label.c_str());
  for (auto* i : sort_method_list) {
    if (!i->m_sel) {
      ImGui::Selectable(i->m_tag.c_str(), &(i->m_sel));
      if (i->m_sel) {
        if (x != nullptr)
          x->m_sel = false;
        x       = i;
        x_label = X_LABEL + x->m_tag;
      }
    }
  }
  ImGui::EndListBox();
  static std::string z_label = Z_LABEL;
  ImGui::BeginListBox(z_label.c_str());
  for (auto* i : sort_method_list) {
    if (!i->m_sel) {
      ImGui::Selectable(i->m_tag.c_str(), &(i->m_sel));
      if (i->m_sel) {
        if (z != nullptr)
          z->m_sel = false;
        z       = i;
        z_label = Z_LABEL + z->m_tag;
      }
    }
  }
  ImGui::EndListBox();

  if (ImGui::Button("Sort", ImVec2(100, 20))) {
    SortEntityList(x->m_tag);
    ICSVapp::EntityManager::Get().RepositionEnttsOnAxisX();
    SortEntityList(z->m_tag);
    ICSVapp::EntityManager::Get().RepositionEnttsOnAxisZ();
  }
}

void  // TODO
IcsvGui::ShowSmellColorPallet(void) {
  static ImGuiColorEditFlags flags
      = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB;

  ImGui::Separator();

  static auto tags
      = icsv::detector::DetectorManager::Get().GetAllDetectorTags();

  static float color[4];

  std::string smell_slctd = "";

  if (!tags.empty()) {
    ImGui::BeginListBox("Smells");

    // bool sel = false;

    // for (auto t : tags) {
    //   ImGui::Selectable(t.c_str(), sel);
    //   if (sel) {
    //     smell_slctd = t;
    //     mat         = EntityManager::Get().FindEntityIf(
    //         [t](auto& e) { return e.GetDetectorReport()->smell_tag == t;
    //         })->;
    //     sel = false;
    //   }
    // }

    ImGui::EndListBox();
  }

  ImGui::ColorPicker4(smell_slctd.c_str(), color, flags);

  if (ImGui::Button("Apply Color")) {
  }
}

void
IcsvGui::ShowEvalConfigs(void) {
  icsv::detector::EvaluationCenter::Get().DisplayEvalGui();
  ImGui::Separator();
  if (ImGui::Button("Re-Render Evaluation")) {
    SortEntityList("Smell Tag");
    std::string tag = "";
    int         x   = -1;
    for (auto& e : ICSVapp::EntityManager::Get().GetEntityList()) {
      if (e->GetDetectorReport()->level > 0) {
        if (tag != e->GetDetectorReport()->smell_tag) {
          x++;
          tag = e->GetDetectorReport()->smell_tag;
        }
        double y = 0.2 * e->GetDetectorReport()->level / 2;
        e->SetPosition(x, y / 2, 0);
        e->SetScale(0.2, y, 0.2);
      }
    }
    SortEntityList("Level");
    ICSVapp::EntityManager::Get().RepositionEnttsOnAxisZ();
  }
}

auto smell_sort = [](IcsvEntity* ent1, IcsvEntity* ent2) {
  return ent1->GetDetectorReport()->smell_tag
      < ent2->GetDetectorReport()->smell_tag;
};

auto lvl_sort = [](IcsvEntity* ent1, IcsvEntity* ent2) {
  return ent1->GetDetectorReport()->level < ent2->GetDetectorReport()->level;
};

auto msg_sort = [](IcsvEntity* ent1, IcsvEntity* ent2) {
  return ent1->GetDetectorReport()->message
      < ent2->GetDetectorReport()->message;
};

auto strct_sort = [](IcsvEntity* ent1, IcsvEntity* ent2) {
  return ent1->GetDetectorReport()->src_info.strct
      < ent2->GetDetectorReport()->src_info.strct;
};

auto methd_sort = [](IcsvEntity* ent1, IcsvEntity* ent2) {
  return ent1->GetDetectorReport()->src_info.method
      < ent2->GetDetectorReport()->src_info.method;
};

auto file_sort = [](IcsvEntity* ent1, IcsvEntity* ent2) {
  return ent1->GetDetectorReport()->src_info.file
      < ent2->GetDetectorReport()->src_info.file;
};

void
IcsvGui::SortEntityList(const std::string& by_tag) {
  if (by_tag == "Smell Tag") {
    EntityManager::Get().SortEnttsWith(smell_sort);
  } else if (by_tag == "Message") {
    EntityManager::Get().SortEnttsWith(msg_sort);
  } else if (by_tag == "Level") {
    EntityManager::Get().SortEnttsWith(lvl_sort);
  } else if (by_tag == "Structure") {
    EntityManager::Get().SortEnttsWith(strct_sort);
  } else if (by_tag == "Method") {
    EntityManager::Get().SortEnttsWith(methd_sort);
  } else if (by_tag == "File") {
    EntityManager::Get().SortEnttsWith(file_sort);
  }
}

}  // namespace ICSVapp
