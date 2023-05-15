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
  };

  using MetaMap = std::map<std::string, Entry>;

  class Graph {
  public:
    Graph(std::list<std::string>&& nds)
        : m_nodes(nds), m_tarjan(m_arrows, m_nodes) {}

  private:
    class Tarjan final {
    public:
      Tarjan(std::map<std::string, std::list<std::string>>& a,
             std::list<std::string>&                        n)
          : arrows(a), nodes(n) {}
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
    };

  private:
    std::map<std::string, std::list<std::string>> m_arrows;
    std::list<std::string>                        m_nodes;
    Tarjan                                        m_tarjan;
  };

private:
  auto GetNodes(const Dependencies& deps) -> std::list<std::string>;
  auto AdjacencyList(const Dependencies& deps) -> Map;
};

#if (0)
static CircularDependencyDet* cdd = new CircularDependencyDet();
CREATE_RANGE_BASED_EVAL(TAG);
#endif

void
CircularDependencyDet::DetectSmell(const ArchData& arch) {
  auto nodes    = GetNodes(arch.dependencies);
  auto adj_list = AdjacencyList(arch.dependencies);
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
