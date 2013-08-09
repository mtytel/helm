#include "test_processor.h"
#include "processor_router.h"
#include "gtest/gtest.h"

#include <limits.h>
#include <math.h>

using namespace laf;

TEST(ProcessorRouterTest, SingleRouting) {
  ProcessorRouter pr;
  TestProcessor tp;
  pr.addProcessor(&tp);

  pr.process();

  EXPECT_TRUE(tp.did_process());
  EXPECT_TRUE(tp.dependencies_handled());
}

TEST(ProcessorRouterTest, LinearRouting) {
  ProcessorRouter pr;
  TestProcessor tp0(1,1);
  TestProcessor tp1(1,1);
  TestProcessor tp2(1,1);
  pr.addProcessor(&tp2);
  pr.addProcessor(&tp1);
  pr.addProcessor(&tp0);

  tp2.plug(&tp1);
  tp1.plug(&tp0);

  pr.process();

  EXPECT_TRUE(tp0.did_process());
  EXPECT_TRUE(tp0.dependencies_handled());
  EXPECT_TRUE(tp1.did_process());
  EXPECT_TRUE(tp1.dependencies_handled());
  EXPECT_TRUE(tp2.did_process());
  EXPECT_TRUE(tp2.dependencies_handled());
}

TEST(ProcessorRouterTest, DiamondRouting) {
  ProcessorRouter pr;
  TestProcessor tp0(1,2);
  TestProcessor tp1(1,1);
  TestProcessor tp2(1,1);
  TestProcessor tp3(2,1);
  pr.addProcessor(&tp2);
  pr.addProcessor(&tp1);
  pr.addProcessor(&tp3);
  pr.addProcessor(&tp0);

  tp3.plug(&tp1);
  tp3.plug(&tp2, 1);
  tp1.plug(&tp0);
  tp2.plug(tp0.output(1));

  pr.process();

  EXPECT_TRUE(tp0.did_process());
  EXPECT_TRUE(tp0.dependencies_handled());
  EXPECT_TRUE(tp1.did_process());
  EXPECT_TRUE(tp1.dependencies_handled());
  EXPECT_TRUE(tp2.did_process());
  EXPECT_TRUE(tp2.dependencies_handled());
  EXPECT_TRUE(tp2.did_process());
  EXPECT_TRUE(tp2.dependencies_handled());
}

TEST(ProcessorRouterTest, DenseRouterTree) {
  const int height = 4;
  const int width = 3;
  int total_leaves = pow(3.0, 5.0);
  std::vector<ProcessorRouter*> levels[height];
  std::vector<TestProcessor*> leaves;

  levels[0].push_back(new ProcessorRouter());

  // Create tree structure.
  for (int i = 1; i < height; ++i) {
    for (size_t r = 0; r < levels[i - 1].size(); ++r) {
      for (int j = 0; j < width; ++j) {
        ProcessorRouter* router = new ProcessorRouter();
        levels[i - 1][r]->addProcessor(router);
        levels[i].push_back(router);
      }
    }
  }

  // Add processors as the leaves.
  int leaf_number = 0;
  for (size_t r = 0; r < levels[height - 1].size(); ++r) {
    for (int j = 0; j < width; ++j) {
      TestProcessor* leaf =
          new TestProcessor(leaf_number, total_leaves - leaf_number);
      levels[height - 1][r]->addProcessor(leaf);
      leaves.push_back(leaf);
      leaf_number++;
    }
  }

  // Add data flow dependencies.
  for (size_t i = 0; i < leaves.size(); ++i) {
    for (size_t j = i + 1; j < leaves.size(); ++j) {
      leaves[j]->plug(leaves[i], j - i - 1);
    }
  }

  levels[0][0]->process();

  // Check if everything ran in the right order.
  for (size_t i = 0; i < leaves.size(); ++i) {
    EXPECT_TRUE(leaves[i]->did_process());
    EXPECT_TRUE(leaves[i]->dependencies_handled());
  }
}
