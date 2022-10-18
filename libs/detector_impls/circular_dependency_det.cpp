#include "../evaluator_impls/range_based_eval.hpp"

#define TAG "Circular Dependency"
#include "icsv/detector/detector.hpp"

class CircularDependencyDet : Detector {  // TODO: Implement class
public:
  CircularDependencyDet() : Detector(TAG) {}
  ~CircularDependencyDet() override = default;

  void DetectSmell(const ArchData& arch) override;
};
#if (0)
static CircularDependencyDet* cdd = new CircularDependencyDet();
CREATE_RANGE_BASED_EVAL(TAG);
#endif
void
CircularDependencyDet::DetectSmell(const ArchData& arch) {}
