#include "chart.hpp"
#include "entity_manager.hpp"

namespace ICSVapp {

auto
Chart::Get(void) -> Chart& {
  static Chart singleton;
  return singleton;
}

Chart::Chart() {

  auto lblcomp = [](const Label& l1, const Label& l2) {
    return l1.first->getName() < l2.first->getName();
  };

  m_x_labels = std::set<Label, LabelComp>(lblcomp);
  m_z_labels = std::set<Label, LabelComp>(lblcomp);
}

Chart::~Chart() {
  for (auto* ln : m_chart_lines) {
    destroy_manual_object(ln);
  }
  m_chart_lines.clear();
}

void
Chart::ClearChart(void) {
  for (auto lbl : m_x_labels) {
    if (lbl.second != nullptr) {
      lbl.second->detachAllObjects();
      destroy_node(lbl.second);
      lbl.second = nullptr;
    }
  }
  m_x_labels.clear();

  for (auto lbl : m_z_labels) {
    if (lbl.second != nullptr) {
      lbl.second->detachAllObjects();
      destroy_node(lbl.second);
      lbl.second = nullptr;
    }
  }
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
#if (0)
  ChartLabelSet& lbl_lst  = (axis == m_x_axis) ? m_x_labels : m_z_labels;
  auto           traveler = begin;
  auto           lbegin   = begin;

  auto name = (*(*traveler)->GetDetectorReport())(axis);

  auto* node = EntityManager::Get().RequestChildNode();
  auto  lbl  = create_chart_tag(name, node);

  lbl_lst.insert({ lbl, node });

  while (traveler != end) {
    if ((*(*traveler)->GetDetectorReport())(axis)
        != (*(*lbegin)->GetDetectorReport())(axis)) {

      auto* node = EntityManager::Get().RequestChildNode();
      auto  lbl  = create_chart_tag(name, node);

      lbl_lst.insert({ lbl, node });

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
  const int x_length
      = FindAxisLength(entt_list.begin(),
                       entt_list.end(),
                       [&](auto e1, auto e2) { return XAxisPred(e1, e2); });
  AssignLabelsToAxis(entt_list.begin(), entt_list.end(), m_x_axis);

  /// Sort entity list and get number of neighborhoods on the z axis
  EntityManager::Get().SortEnttsWith(z_sort);
  const int z_length
      = FindAxisLength(entt_list.begin(),
                       entt_list.end(),
                       [&](auto e1, auto e2) { return ZAxisPred(e1, e2); });
  AssignLabelsToAxis(entt_list.begin(), entt_list.end(), m_z_axis);

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
      zlbl_iter->second->setPosition(
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
      xlbl_iter->second->setPosition(
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

auto
Chart::FindAxisLength(const IcsvEnttIter& begin,
                      const IcsvEnttIter& end,
                      const AxisPred&     pred) -> int {
  IcsvEnttIter x_begin  = begin;
  IcsvEnttIter traveler = begin;

  using IterPred
      = std::function<bool(const IcsvEnttIter&, const IcsvEnttIter&)>;

  auto iter_pred = [&](const IcsvEnttIter& e1, const IcsvEnttIter& e2) {
    return pred(*e1, *e2);
  };

  auto x_len_set = std::set<IcsvEnttIter, IterPred>(iter_pred);
  x_len_set.insert(traveler);

  while (traveler != end) {
    if (!pred(*traveler, *x_begin)) {
      x_begin = traveler;
      x_len_set.insert(traveler);
    }
    traveler++;
  }

  return x_len_set.size();
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

        m_total_neighborhoods++;
      }
    }
    traveler++;
  }

  return size;
}

}  // namespace ICSVapp