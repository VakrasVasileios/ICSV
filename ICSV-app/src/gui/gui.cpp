#include "gui.hpp"
#include "icsv/detector/arch/architecture_holder.hpp"
#include "icsv/detector/detector_manager.hpp"
#include "icsv/detector/evaluation_center.hpp"
#include "icsv/detector/report_center.hpp"
#include "../entity_manager.hpp"
#include "../material_factory.hpp"
#include "../chart.hpp"
#include <imgui/imgui_internal.h>

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
  ShowExitPopUp();

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
      if (!det_conf_file.ends_with(".json"))
        std::cout << "\tThe selected file is not a json file.\n";
      else {
        try {
          MaterialFactory::Get().DeseriallizeConfig(m_conf_brwsr.GetSelected());
          icsv::detector::deseriallize_detector_config(
              m_conf_brwsr.GetSelected());
          changed = true;
        } catch (std::exception* e) {
          std::cout << "Invalid configuration file. " << e->what() << '\n';
        }
      }
    }
  }

  ImGui::Separator();

  ImGui::Text("Architecture Graph File");
  ImGui::InputText("Graph", graph_buf, BUFFSIZE);
  if (ImGui::Button("Select##2", ImVec2(100, 20))) {
    m_graph_brwsr.SetTitle("Select Architecture Graph");
    m_graph_brwsr.Open();
  }
  if (m_graph_brwsr.HasSelected()) {
    std::strncpy(graph_buf, graph_conf_file.c_str(), graph_conf_file.size());
    if (m_graph_brwsr.GetSelected().c_str() != graph_conf_file) {
      graph_conf_file = m_graph_brwsr.GetSelected();
      if (!graph_conf_file.ends_with(".json"))
        std::cout << "\tThe selected file is not a json file.\n";
      else {
        try {
          icsv::detector::arch::deseriallize_arch_conf(
              m_graph_brwsr.GetSelected());
          changed = true;
        } catch (std::exception* e) {
          std::cout << "Invalid architecture graph file. " << e->what() << '\n';
        }
      }
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

#define EXIT_BTN_SIZE (ImVec2(200, 75))

namespace {
bool
BeginCentered(const char* name, bool* open) {
  ImGuiIO& io = ImGui::GetIO();
  ImVec2   pos(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
  ImGui::SetNextWindowPos(pos, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize
      | ImGuiWindowFlags_NoSavedSettings;
  return ImGui::Begin(name, open, flags);
}

}  // namespace

void
IcsvGui::ShowExitPopUp(void) {
  if (m_render_exit_pop) {
    BeginCentered("Exit", &m_render_exit_pop);
    ImGui::BringWindowToDisplayFront(ImGui::GetCurrentWindowRead());
    ImGui::SetWindowFontScale(5);
    ImGui::Text("%s", "Do you want to EXIT ICSV-app?");
    if (ImGui::Button("Yes", EXIT_BTN_SIZE))
      m_exit_call();
    ImGui::SameLine();
    if (ImGui::Button("No", EXIT_BTN_SIZE))
      m_render_exit_pop = false;

    ImGui::End();
  }
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

auto smell_eq = [](IcsvEntity* ent1, IcsvEntity* ent2) {
  return ent1->GetDetectorReport()->smell_tag
      == ent2->GetDetectorReport()->smell_tag;
};

auto lvl_eq = [](IcsvEntity* ent1, IcsvEntity* ent2) {
  return ent1->GetDetectorReport()->level == ent2->GetDetectorReport()->level;
};

void
IcsvGui::ShowSmellButton(bool& changed) {
  if (ImGui::Button("Evaluate Code", ImVec2(250, 50))) {
    if (changed) {
      m_rep_dspld = nullptr;
      icsv::detector::clear_reports();
      clear_entities();
      icsv::detector::use_smell_detectors();
      const auto& replst = icsv::detector::ReportCenter::Get().GetReportList();

      auto unit_scale = 0.2;
      for (auto* rep : replst) {
        if (rep->level > 0) {
          double y = unit_scale * rep->level / 2;
          create_icsv_entity(rep,
                             Ogre::Vector3f(0, y / 2, 0),
                             Ogre::Vector3f(unit_scale, y, unit_scale));
        } else {
          create_icsv_entity(rep,
                             Ogre::Vector3f(-50, 0.05, -50),
                             Ogre::Vector3f(0.2, 0.1, 0.2))
              ->SetVisibility(false);
        }
      }
      Chart::Get().SetXaxis(ReportMembers::SMELL_TAG_m);
      Chart::Get().SetZaxis(ReportMembers::LEVEL_m);
      Chart::Get().DrawChart();
      changed = false;
    }
  }
}

void
IcsvGui::ShowSkyboxSettings(void) {
#if (0)
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
#endif
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
    = { new Selectable("Smell Tag", ReportMembers::SMELL_TAG_m),
        new Selectable("Level", ReportMembers::LEVEL_m),
        new Selectable("File", ReportMembers::FILE_m),
        new Selectable("Structure", ReportMembers::STRUCTURE_m),
        new Selectable("Method", ReportMembers::METHOD_m) };

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
    Chart::Get().SetXaxis(x->m_memb);
    Chart::Get().SetZaxis(z->m_memb);
    Chart::Get().DrawChart();
  }
}

void
IcsvGui::ShowSmellColorPallet(void) {
  static ImGuiColorEditFlags flags
      = ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_DisplayRGB;

  ImGui::Separator();

  static auto tags
      = icsv::detector::DetectorManager::Get().GetAllDetectorTags();

  static std::string smell_slctd = "";

  static MaterialFacPtr mat;

  if (!tags.empty()) {
    ImGui::LabelText("##S.M.", "%s", "Smell Materials");
    ImGui::BeginListBox("##SmellMat");

    static bool sel = false;

    for (auto t : tags) {
      ImGui::Selectable(t.c_str(), &sel);
      if (sel) {
        sel         = false;
        smell_slctd = t;
        mat         = MaterialFactory::Get().GetMaterialFor(smell_slctd);
      }
    }

    ImGui::EndListBox();
  }
  ImGui::Text("%s%s", "Chosen Smell: ", smell_slctd.c_str());

  static int chosen = AMBIENT;

  if (mat.operator bool()) {  // chosen material is not null
    ImGui::RadioButton("Ambient", &chosen, AMBIENT);
    ImGui::SameLine();
    ImGui::RadioButton("Diffuse", &chosen, DIFFUSE);
    ImGui::SameLine();
    ImGui::RadioButton("Specular", &chosen, SPECULAR);

    ImGui::ColorPicker4(smell_slctd.c_str(), mat->m_color[chosen], flags);
  }

  if (ImGui::Button("Apply Color")) {

    mat->m_material->setAmbient(mat->m_color[AMBIENT][0],
                                mat->m_color[AMBIENT][1],
                                mat->m_color[AMBIENT][2]);

    mat->m_material->setDiffuse(mat->m_color[DIFFUSE][0],
                                mat->m_color[DIFFUSE][1],
                                mat->m_color[DIFFUSE][2],
                                mat->m_color[DIFFUSE][3]);

    mat->m_material->setSpecular(mat->m_color[SPECULAR][0],
                                 mat->m_color[SPECULAR][1],
                                 mat->m_color[SPECULAR][2],
                                 mat->m_color[SPECULAR][3]);
  }
}

void
IcsvGui::ShowEvalConfigs(void) {
  icsv::detector::EvaluationCenter::Get().DisplayEvalGui();

  ImGui::Separator();

  if (ImGui::Button("Re-Render Evaluation")) {

    for (auto* ref : EntityManager::Get().GetEntityList())
      if (ref->GetDetectorReport()->level == 0)
        ref->SetVisibility(false);
      else
        ref->SetScale(0.2, ref->GetDetectorReport()->level * 0.1, 0.2);

    Chart::Get().SetXaxis(ReportMembers::SMELL_TAG_m);
    Chart::Get().SetZaxis(ReportMembers::LEVEL_m);
    Chart::Get().DrawChart();
  }
}

void
IcsvGui::SortEntityList(icsv::detector::ReportMembers x_axis,
                        icsv::detector::ReportMembers z_axis) {

  static auto _sort = [&](IcsvEntity* e1, IcsvEntity* e2) {
    auto s1 = (*e1->GetDetectorReport())(x_axis)
        + (*e1->GetDetectorReport())(z_axis);
    auto s2 = (*e2->GetDetectorReport())(x_axis)
        + (*e2->GetDetectorReport())(z_axis);
    return s1 < s2;
  };

  EntityManager::Get().SortEnttsWith(_sort);
}

}  // namespace ICSVapp
