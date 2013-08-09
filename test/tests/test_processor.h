#include "processor.h"

using namespace laf;

class TestProcessor : public Processor {
  public:
    TestProcessor(int inputs = 0, int outputs = 0) :
        Processor(inputs, outputs),
        did_process_(false), dependencies_handled_(false) { }

    Processor* clone() const { return new TestProcessor(*this); }
    void process() {
      did_process_ = true;
      dependencies_handled_ = true;

      for (int i = 0; i < numInputs(); ++i) {
        if (inputs_[i]->source && inputs_[i]->source->owner) {
          const TestProcessor* input =
              static_cast<const TestProcessor*>(inputs_[i]->source->owner);
          if (!input->did_process())
            dependencies_handled_ = false;
        }
      }
    }
    bool did_process() const { return did_process_; }
    bool dependencies_handled() const { return dependencies_handled_; }

  protected:
    bool did_process_;
    bool dependencies_handled_;
};
