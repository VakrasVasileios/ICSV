#include "chart.hpp"
#include "entity_manager.hpp"

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
Chart::IsLabelUnique(ChartLabelList& lst, const std::string& caption) {
  auto iter = std::find_if(lst.begin(), lst.end(), [&caption](const Label& l) {
    return l.get()->getCaption() == caption;
  });

  return (iter == lst.end()) ? true : false;
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
#if (1)
  ChartLabelList& lbl_lst  = (axis == m_x_axis) ? m_x_labels : m_z_labels;
  auto            traveler = begin;
  auto            lbegin   = begin;

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
#endif
}

void
Chart::DrawChart(void) {
  /// Initialize sorting funcs
  static auto _sort = [&](IcsvEntity* e1, IcsvEntity* e2) {
    auto s1 = (*e1->GetDetectorReport())(m_x_axis)
        + (*e1->GetDetectorReport())(m_z_axis);
    auto s2 = (*e2->GetDetectorReport())(m_x_axis)
        + (*e2->GetDetectorReport())(m_z_axis);
    return s1 < s2;
  };

  static auto lbl_sort = [](const Label& m1, const Label& m2) {
    return m1.get()->getCaption() < m2.get()->getCaption();
  };

  static auto x_sort = [&](IcsvEntity* e1, IcsvEntity* e2) {
    return (*e1->GetDetectorReport())(m_x_axis)
        < (*e2->GetDetectorReport())(m_x_axis);
  };

  static auto z_sort = [&](IcsvEntity* e1, IcsvEntity* e2) {
    return (*e1->GetDetectorReport())(m_z_axis)
        < (*e2->GetDetectorReport())(m_z_axis);
  };

  /// Clear previous chart if it exists
  ClearChart();

  auto entt_list = EntityManager::Get().GetEntityList();

  /// Sort entity list and get number of neighborhoods on the x axis
  EntityManager::Get().SortEnttsWith(x_sort);
  AssignLabelsToAxis(entt_list.begin(), entt_list.end(), m_x_axis);
  const int x_length = m_x_labels.size();

  std::stable_sort(m_x_labels.begin(), m_x_labels.end(), lbl_sort);

  /// Sort entity list and get number of neighborhoods on the z axis
  EntityManager::Get().SortEnttsWith(z_sort);
  AssignLabelsToAxis(entt_list.begin(), entt_list.end(), m_z_axis);
  const int z_length = m_z_labels.size();

  std::stable_sort(m_z_labels.begin(), m_z_labels.end(), lbl_sort);

  /// Sort entity list for both axises and set total neighborhoods
  EntityManager::Get().SortEnttsWith(_sort);
  int total_neighborhoods = z_length * x_length;

  int block_size = std::ceil(std::sqrt(total_neighborhoods));

  int chart_x = 0, chart_z = 0;

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
      const double increment = block_size / (hood_side * 2);

      for (int i = 0; i < local_size && traveler != hood_end; i++) {
        if ((*traveler)->GetDetectorReport()->level > 0) {
          chart_z += 1 * (i % hood_side == 0);
          double y = (*traveler)->GetScale().y / 2;
          auto   x = i % hood_side;
          (*traveler)->SetPosition(chart_x + x + increment,
                                   y,
                                   chart_z + increment);
        }
        traveler++;
      }
    }
    hood_begin = hood_end;
    chart_x += block_size;
    chart_z = 0;
  }

  auto zlbl_iter = m_z_labels.begin();
  for (int z = 0; z <= z_length; z++) {
    DrawChartLine({ -2.f, 0.f, static_cast<float>(z * block_size) },
                  { static_cast<float>(block_size * x_length + 2),
                    0.f,
                    static_cast<float>(z * block_size) });

    if (zlbl_iter != m_z_labels.end()) {
      auto* node = m_node_lst[zlbl_iter->get()->getCaption()];
      node->setPosition(
          { -2.f, 0.f, static_cast<float>(z * block_size) + block_size / 2 });

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
      node->setPosition(
          { static_cast<float>(x * block_size) + block_size / 2, 0.f, -2.f });

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

}  // namespace ICSVapp