#include "../evaluator_impls/range_based_eval.hpp"
#include <map>
#include <set>

#define TAG "Circular Dependency"
#include "icsv/detector/detector.hpp"

class CircularDependencyDet : Detector {
  using Dependencies = icsv::detector::arch::Dependencies;
  using Map          = std::map<std::string, std::list<std::string>>;

public:
  CircularDependencyDet() : Detector(TAG) {}
  ~CircularDependencyDet() override = default;

  void DetectSmell(const ArchData& arch) override;

private:
  struct Entry {
    bool inStack = true;
    int  index;
    int  lowLink;

    Entry(int in, int ll) : index(in), lowLink(ll) {}
  };  // Entry

  using MetaMap = std::map<std::string, Entry>;

  class Graph {
  public:
    Graph(std::list<std::string> nds, Map a_list)
        : m_nodes(nds), m_arrows(a_list), m_tarjan(m_arrows, m_nodes) {}

    void FindCircuits(void);
    auto GetCircuits(void) -> std::list<std::list<std::string>> {
      return m_circuits;
    }

  private:
    class Tarjan final {
    public:
      Tarjan(Map& a, std::list<std::string>& n) : arrows(a), nodes(n) {}
      ~Tarjan() = default;

      auto operator()() -> std::list<std::list<std::string>>;

    private:
      int                               index = 0;
      std::stack<std::string>           stck;
      std::list<std::list<std::string>> result;
      MetaMap                           meta;

      std::map<std::string, std::list<std::string>>& arrows;
      std::list<std::string>&                        nodes;

      void Connect(const std::string& node);
      void CleanUp(void);
    };  // Tarjan

    void AddCircuit(const std::string& strt_node);
    bool Circuit(const std::string& node);

  private:
    std::map<std::string, std::list<std::string>> m_arrows;
    std::list<std::string>                        m_nodes;
    std::list<std::list<std::string>>             m_circuits;
    Tarjan                                        m_tarjan;

    std::string                 start_node;
    std::list<std::string>      stck;
    std::map<std::string, bool> blocked;
  };  // Graph

private:
  auto GetNodes(const Dependencies& deps) -> std::list<std::string>;
  auto AdjacencyList(const Dependencies& deps) -> Map;
};  // CircularDependencyDet

static CircularDependencyDet* cdd = new CircularDependencyDet();

void
CircularDependencyDet::DetectSmell(const ArchData& arch) {
  auto  nodes    = GetNodes(arch.dependencies);
  auto  adj_list = AdjacencyList(arch.dependencies);
  Graph g        = Graph(nodes, adj_list);

  g.FindCircuits();
  auto circuits = g.GetCircuits();

  for (auto& crc : circuits) {
    DetectorReport rep;
    rep.smell_tag  = TAG;
    rep.init_level = crc.size();
    rep.level      = icsv::detector::evaluate_smell(TAG, crc.size());
    if (rep.level > 0) {
      auto strct
          = std::find_if(arch.structures.begin(),
                         arch.structures.end(),
                         [&crc](auto& strc) {
                           return std::string(crc.front()) == strc.signature;
                         });
      rep.message = "Structure: `" + crc.front()
          + "` is part of this dependency cycle: ";

      for (auto c : crc) {
        if (c != crc.back())
          rep.message += "`" + c + "` -> ";
      }
      rep.message += "`" + crc.back() + "`\n";
      rep.src_info.file  = strct->src_info.file;
      rep.src_info.col   = strct->src_info.col;
      rep.src_info.line  = strct->src_info.line;
      rep.src_info.strct = crc.front();

      icsv::detector::register_report(TAG, rep);
    }
  }
}

auto
CircularDependencyDet::GetNodes(const Dependencies& deps)
    -> std::list<std::string> {
  std::list<std::string> node_l;

  for (auto& dep : deps)
    node_l.push_back(dep.from);

  return node_l;
}

auto
CircularDependencyDet::AdjacencyList(const Dependencies& deps) -> Map {
  Map l;

  for (auto& dep : deps)
    l[dep.from].push_back(dep.to);

  return l;
}

void
CircularDependencyDet::Graph::Tarjan::Connect(const std::string& node) {
  auto entr = Entry(index, index);

  meta.emplace(node, entr);
  stck.push(node);
  ++index;

  for (auto node : arrows) {
    for (auto& adj : node.second) {
      if (!meta.contains(adj)) {
        Connect(adj);
        entr.lowLink = std::min(entr.lowLink, meta.at(adj).lowLink);
      } else if (meta.at(adj).inStack)
        entr.lowLink = std::min(entr.lowLink, meta.at(adj).index);
    }
  }

  if (entr.lowLink == entr.index) {
    auto scc = std::list<std::string>();
    auto adj = std::string();

    do {
      adj = stck.top();
      stck.pop();
      meta.at(adj).inStack = false;
      scc.push_back(adj);
    } while (node != adj);

    result.push_back(scc);
  }
}

void
CircularDependencyDet::Graph::Tarjan::CleanUp(void) {
  meta.clear();
  while (!stck.empty())
    stck.pop();
  result.clear();
}

auto
CircularDependencyDet::Graph::Tarjan::operator()(void)
    -> std::list<std::list<std::string>> {
  CleanUp();

  for (auto& n : nodes) {
    if (!meta.contains(n))
      Connect(n);
  }

  return result;
}

void
CircularDependencyDet::Graph::FindCircuits(void) {
  for (auto n : m_nodes) {
    start_node     = n;
    auto node_list = m_arrows[n];
    for (auto nd : node_list)
      Circuit(nd);
  }
}

void
CircularDependencyDet::Graph::AddCircuit(const std::string& strt_node) {
  m_circuits.push_back(stck);
  m_circuits.back().push_back(strt_node);
}

bool
CircularDependencyDet::Graph::Circuit(const std::string& node) {
  bool found = false;

  stck.push_back(node);
  blocked[node]  = true;
  auto node_list = m_arrows[node];
  if (!node_list.empty()) {
    for (auto n : node_list) {
      auto blckd = blocked.find(n);
      if (n == start_node) {
        found = true;
        AddCircuit(start_node);
      } else if (blckd != blocked.end() && !blckd->second) {
        if (Circuit(n))
          found = true;
      }
    }
  }

  if (found)
    blocked[node] = false;

  stck.pop_back();

  return found;
}
