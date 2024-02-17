#include "chart.hpp"
#include "entity_manager.hpp"
#include "statistics.hpp"

namespace ICSVapp {

auto
Chart::Get(void) -> Chart& {
  static Chart singleton;
  return singleton;
}

Chart::~Chart() {
  for (auto* ln : m_chart_lines) {
    destroy_manual_object(ln);
  }
  m_chart_lines.clear();
}

/*
 * Return true on successful insertion
 */
bool
Chart::IsLabelUnique(const ChartLabelList& lst, const std::string& caption) {
  auto iter = std::find_if(lst.begin(), lst.end(), [&caption](const Label& l) {
    return l.get()->getCaption() == caption;
  });

  return (iter == lst.end()) ? true : false;
}

auto
Chart::IndexOfCaption(const ChartLabelList& lst, const std::string& caption)
    -> int {
  for (int i = 0; i < (int) lst.size(); i++) {
    if (lst[i].get()->getCaption() == caption) {
      return i;
    }
  }
  return -1;
}

void
Chart::ClearChart(void) {
  for (auto node : m_node_lst) {
    node.second->detachAllObjects();
    destroy_node(node.second);
  }
  m_node_lst.clear();

  m_x_labels.clear();
  m_z_labels.clear();

  clear_chart_tags();

  for (auto* ln : m_chart_lines) {
    destroy_manual_object(ln);
  }
  m_chart_lines.clear();
}

bool
Chart::AxisToStringPred(IcsvEntity* ent1, IcsvEntity* ent2) {
  auto* det1 = ent1->GetDetectorReport();
  auto* det2 = ent2->GetDetectorReport();
  auto  str1 = (*det1)(m_x_axis) + (*det1)(m_z_axis);
  auto  str2 = (*det2)(m_x_axis) + (*det2)(m_z_axis);
  return str1 == str2;
}

bool
Chart::XAxisPred(IcsvEntity* ent1, IcsvEntity* ent2) {
  return (*ent1->GetDetectorReport())(m_x_axis)
      == (*ent2->GetDetectorReport())(m_x_axis);
}

bool
Chart::ZAxisPred(IcsvEntity* ent1, IcsvEntity* ent2) {
  return (*ent1->GetDetectorReport())(m_z_axis)
      == (*ent2->GetDetectorReport())(m_z_axis);
}

void
Chart::AssignLabelsToAxis(const IcsvEnttIter& begin,
                          const IcsvEnttIter& end,
                          const AxisType      axis) {
  ChartLabelList& lbl_lst  = (axis == m_x_axis) ? m_x_labels : m_z_labels;
  auto            traveler = begin;
  auto            lbegin   = begin;

  if (begin == end) {
    std::cout << "Entity list is empty, rendering empty chart\n";
    return;
  }

  auto caption = (*(*traveler)->GetDetectorReport())(axis);

  if (IsLabelUnique(lbl_lst, caption)) {
    auto* node = EntityManager::Get().RequestChildNode();
    auto  lbl  = create_chart_tag(caption, node);
    lbl_lst.push_back(lbl);

    m_node_lst[lbl.get()->getCaption()] = node;
  }

  while (traveler != end) {
    if ((*(*traveler)->GetDetectorReport())(axis)
        != (*(*lbegin)->GetDetectorReport())(axis)) {

      caption = (*(*traveler)->GetDetectorReport())(axis);

      if (IsLabelUnique(lbl_lst, caption)) {
        auto* node = EntityManager::Get().RequestChildNode();
        auto  lbl  = create_chart_tag(caption, node);
        lbl_lst.push_back(lbl);

        m_node_lst[lbl.get()->getCaption()] = node;
      }

      lbegin = traveler;
    }
    traveler++;
  }
}

using RepMemb = icsv::detector::ReportMembers;

void
Chart::DrawChart(void) {
  /// Initialize sorting funcs
  static auto _sort = [&](IcsvEntity* e1, IcsvEntity* e2) {
    bool x = false, z = false;
    if (m_x_axis == RepMemb::LEVEL_m) {
      x = e1->GetDetectorReport()->level < e2->GetDetectorReport()->level;
    } else {
      x = (*e1->GetDetectorReport())(m_x_axis)
          < (*e2->GetDetectorReport())(m_x_axis);
    }
    if (m_z_axis == RepMemb::LEVEL_m) {
      z = e1->GetDetectorReport()->level < e2->GetDetectorReport()->level;
    } else {
      z = (*e1->GetDetectorReport())(m_z_axis)
          < (*e2->GetDetectorReport())(m_z_axis);
    }

    return x & z;
  };

  static auto lbl_sort = [](const Label& m1, const Label& m2) {
    return m1.get()->getCaption() < m2.get()->getCaption();
  };

  static auto num_lbl_sort = [](const Label& m1, const Label& m2) {
    return std::stoi(m1.get()->getCaption().c_str())
        < std::stoi(m2.get()->getCaption().c_str());
  };

  static auto x_sort = [&](IcsvEntity* e1, IcsvEntity* e2) {
    if (m_x_axis == RepMemb::LEVEL_m)
      return e1->GetDetectorReport()->level < e2->GetDetectorReport()->level;

    return (*e1->GetDetectorReport())(m_x_axis)
        < (*e2->GetDetectorReport())(m_x_axis);
  };

  static auto z_sort = [&](IcsvEntity* e1, IcsvEntity* e2) {
    if (m_z_axis == RepMemb::LEVEL_m)
      return e1->GetDetectorReport()->level < e2->GetDetectorReport()->level;

    return (*e1->GetDetectorReport())(m_z_axis)
        < (*e2->GetDetectorReport())(m_z_axis);
  };

  /// Clear previous chart if it exists
  ClearChart();
#if (1)
  SmellStatistics::Get().ClearStats();
#endif
  auto entt_list = EntityManager::Get().GetEntityList();

  /// Sort entity list and get number of neighborhoods on the x axis
  EntityManager::Get().SortEnttsWith(x_sort);
  AssignLabelsToAxis(entt_list.begin(), entt_list.end(), m_x_axis);
  const int x_length = m_x_labels.size();

  if (m_x_axis == RepMemb::LEVEL_m)
    std::stable_sort(m_x_labels.begin(), m_x_labels.end(), num_lbl_sort);
  else
    std::stable_sort(m_x_labels.begin(), m_x_labels.end(), lbl_sort);

  /// Sort entity list and get number of neighborhoods on the z axis
  EntityManager::Get().SortEnttsWith(z_sort);
  AssignLabelsToAxis(entt_list.begin(), entt_list.end(), m_z_axis);
  const int z_length = m_z_labels.size();

  if (m_z_axis == RepMemb::LEVEL_m)
    std::stable_sort(m_z_labels.begin(), m_z_labels.end(), num_lbl_sort);
  else
    std::stable_sort(m_z_labels.begin(), m_z_labels.end(), lbl_sort);

  /// Sort entity list for both axises and set total neighborhoods
  EntityManager::Get().SortEnttsWith(_sort);
  // int total_neighborhoods = z_length * x_length;

  int max_hood_size = FindNeighborhoodSize(entt_list.begin(), entt_list.end());
  int block_size    = std::ceil(std::sqrt(max_hood_size));

  IcsvEnttIter traveler   = entt_list.begin();
  IcsvEnttIter hood_begin = entt_list.begin();
  IcsvEnttIter hood_end   = entt_list.begin();

  while (traveler != entt_list.end()) {
    while (hood_end != entt_list.end() && XAxisPred(*hood_begin, *hood_end))
      hood_end++;

    while (traveler != hood_end) {
      IcsvEnttIter cnt_iter   = hood_begin;
      int          local_size = 0;

      while (cnt_iter != hood_end
             && ZAxisPred(*cnt_iter,
                          *hood_begin)) {  // Find local Z neighborhood size
        if ((*cnt_iter)->GetDetectorReport()->level > 0) {
          local_size++;
        }
        cnt_iter++;
      }

      if (local_size == 0) {
        traveler   = cnt_iter;
        hood_begin = cnt_iter;
        continue;
      }

      const int    hood_side = std::ceil(std::sqrt(local_size));
      const double increment = (block_size / (hood_side == 1 ? 2 : hood_side));

      for (int i = 0; i < local_size && traveler != hood_end; i++) {
        if ((*traveler)->GetDetectorReport()->level > 0) {
          int x_hood
              = IndexOfCaption(m_x_labels,
                               (*(*traveler)->GetDetectorReport())(m_x_axis));
          int z_hood
              = IndexOfCaption(m_z_labels,
                               (*(*traveler)->GetDetectorReport())(m_z_axis));
          double x = (i % hood_side) + (x_hood * block_size) + increment;
          double y = (*traveler)->GetScale().y / 2;
          double z
              = (double) (i / hood_side) + (z_hood * block_size) + increment;
          (*traveler)->SetPosition(x, y, z);
        }
#if (1)
        SmellStatistics::Get()
            .AddFileIntensity((*traveler)->GetDetectorReport()->src_info.file,
                              (*traveler)->GetDetectorReport()->smell_tag,
                              (*traveler)->GetDetectorReport()->level);
        SmellStatistics::Get()
            .AddSmellIntensity((*traveler)->GetDetectorReport()->smell_tag,
                               (*traveler)->GetDetectorReport()->level);
#endif
        traveler++;
      }
    }
    hood_begin = hood_end;
  }

  auto zlbl_iter = m_z_labels.begin();
  for (int z = 0; z <= z_length; z++) {
    DrawChartLine({ -2.f, 0.f, static_cast<float>(z * block_size) },
                  { static_cast<float>(block_size * x_length + 2),
                    0.f,
                    static_cast<float>(z * block_size) });

    if (zlbl_iter != m_z_labels.end()) {
      auto* node = m_node_lst[zlbl_iter->get()->getCaption()];
      node->setPosition({ -2.f, 0.f, static_cast<float>(z * block_size) });

      zlbl_iter++;
    }
  }

  auto xlbl_iter = m_x_labels.begin();
  for (int x = 0; x <= x_length; x++) {
    DrawChartLine({ static_cast<float>(x * block_size), 0.f, -2.f },
                  { static_cast<float>(x * block_size),
                    0.f,
                    static_cast<float>(block_size * z_length + 2) });

    if (xlbl_iter != m_x_labels.end()) {
      auto* node = m_node_lst[xlbl_iter->get()->getCaption()];
      node->setPosition({ static_cast<float>(x * block_size), 0.f, -2.f });

      xlbl_iter++;
    }
  }
}

void
Chart::DrawChartLine(const Ogre::Vector3& pos1, const Ogre::Vector3& pos2) {
  auto point = EntityManager::Get()
                   .m_scnMan->getRootSceneNode()
                   ->createChildSceneNode();
  m_chart_lines.push_back(
      EntityManager::Get().CreateGridLine(point, pos1, pos2));
}

void
Chart::SetXaxis(const AxisType& fx) {
  m_x_axis = fx;
}

void
Chart::SetZaxis(const AxisType& fz) {
  m_z_axis = fz;
}

auto
Chart::FindNeighborhoodSize(const IcsvEnttIter& begin, const IcsvEnttIter& end)
    -> int {
  IcsvEnttIter hood_begin = begin;
  IcsvEnttIter traveler   = begin;

  int size = 0, cntr = 0;

  while (traveler != end) {
    if ((*traveler)->GetDetectorReport()->level > 0) {
      if (AxisToStringPred(*traveler, *hood_begin)) {
        cntr++;
      } else {
        size       = std::max(size, cntr);
        cntr       = 0;
        hood_begin = traveler;
        traveler--;

        m_total_neighborhoods++;
      }
    }
    traveler++;
  }

  return size;
}

}  // namespace ICSVapp