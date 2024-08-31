#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>

struct BranchReturnValue{
  std::unordered_map<std::size_t, std::vector<std::size_t>> mergerMap;
  std::unordered_map<std::size_t, std::vector<std::size_t>> vLayersBranch;
};

class Branch{
public:
  explicit Branch(const std::unordered_map<std::size_t, std::unordered_set<std::size_t>>& vLayers,
                  const std::unordered_map<std::size_t, std::size_t>& vLayerInputCounter);

  BranchReturnValue calculate(std::size_t head);
private:
  void goToFirstUnhandledUntilFindMerge(std::size_t iCurrent, std::size_t branchValue);
  void splitter(std::size_t iCurrent);
  void merger();
  void clear();
  void init(std::size_t head);

  void addToQueueIfSplitter(std::size_t value);
  void addToQueueIfMergerIsFull(std::size_t value);
  void mergeCalculate(std::size_t temp);

  bool isSplitter(std::size_t value);
  bool isMerger(std::size_t value);
  bool isNormal(std::size_t value);
  bool isExit(std::size_t value);

  bool isNotVisited(std::size_t value);
  bool isVisitedOnce(std::size_t value);
  bool isCompleted(std::size_t value);
private:
  const std::unordered_map<std::size_t, std::unordered_set<std::size_t>>& vLayers;
  const std::unordered_map<std::size_t, std::size_t>& vLayerInputCounter;
  std::unordered_map<std::size_t, std::vector<std::size_t>> vLayersBranch;
  std::unordered_map<std::size_t, std::size_t> splitterMap;
  std::queue<std::size_t> splitterQueue;
  std::unordered_map<std::size_t, std::vector<std::size_t>> mergerMap;
  std::queue<std::size_t> mergerQueue;

  std::size_t m_branchValue = 0;
  std::set<std::size_t> activeBranchSet;
};
