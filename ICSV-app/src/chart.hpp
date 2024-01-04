#pragma once

#include <Ogre.h>
#include <list>
#include <set>
#include <functional>
#include "icsv/detector/detector_report.hpp"
#include "entity_manager.hpp"

namespace ICSVapp {

class IcsvEntity;

class Chart final {
public:
  using AxisType = icsv::detector::ReportMembers;
  using LineList = std::list<Ogre::ManualObject*>;

public:
  static auto Get(void) -> Chart&;

  void ClearChart(void);

  void DrawChart(void);

  void SetXaxis(const AxisType& fx);
  void SetZaxis(const AxisType& fz);

private:
  using IcsvEnttIter  = std::vector<IcsvEntity*>::iterator;
  using Label         = std::pair<MovableTextPtr, Ogre::SceneNode*>;
  using LabelComp     = std::function<bool(const Label&, const Label&)>;
  using ChartLabelSet = std::set<Label, LabelComp>;

private:
  LineList      m_chart_lines;
  ChartLabelSet m_x_labels;
  ChartLabelSet m_z_labels;
  int           m_total_neighborhoods = 1;

  AxisType m_x_axis;
  AxisType m_z_axis;

  Chart();
  Chart(Chart&&)      = delete;
  Chart(const Chart&) = delete;
  ~Chart();

  void DrawChartLine(const Ogre::Vector3& pos1, const Ogre::Vector3& pos2);

  inline bool AxisToStringPred(IcsvEntity* ent1, IcsvEntity* ent2);

  inline bool XAxisPred(IcsvEntity* ent1, IcsvEntity* ent2);
  inline bool ZAxisPred(IcsvEntity* ent1, IcsvEntity* ent2);

  using AxisPred = std::function<bool(IcsvEntity*, IcsvEntity*)>;

  void AssignLabelsToAxis(const IcsvEnttIter& begin,
                          const IcsvEnttIter& end,
                          const AxisType      axis);

  auto FindAxisLength(const IcsvEnttIter& begin,
                      const IcsvEnttIter& end,
                      const AxisPred&     pred) -> int;

  auto FindNeighborhoodSize(const IcsvEnttIter& begin, const IcsvEnttIter& end)
      -> int;
};

}  // namespace ICSVapp
