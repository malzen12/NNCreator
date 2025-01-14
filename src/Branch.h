#pragma once

#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>

class Branch {
  using KeyType = std::size_t;
  using Set = std::set<KeyType>;
  using HashSet = std::unordered_set<KeyType>;
  using HashMapOfHashSet = std::unordered_map<KeyType, HashSet>;
  using HashMap = std::unordered_map<KeyType, KeyType>;
  using Queue = std::queue<KeyType>;

public:
  struct BranchReturnValue {
    HashMapOfHashSet merger;
    HashMapOfHashSet layersBranch;
  };

  template<class T>
  static T hashSetFirst(const std::unordered_set<T>& set) {
    if (set.empty()) return T{};
    return *set.begin();
  }

  template<class T>
  static T hashSetSecond(const std::unordered_set<T>& set) {
    if (set.size() < 2) return T{};
    return *std::next(set.begin());
  }

public:
  explicit Branch(const HashMapOfHashSet& vLayers, const HashMap& vLayerInputCounter);

  BranchReturnValue calculate(KeyType head);

private:
  void bypassGraph(KeyType index, KeyType branchValue);
  void split(KeyType iCurrent);
  void ifCompletePopQueue(KeyType index);
  void merge();
  void ifMergerInMerge(KeyType index, KeyType branchValue);
  void ifSplitterInMerge(KeyType index, KeyType branchValue);
  void clear();
  void init(KeyType head);

  void addToQueueIfSplitter(KeyType value);
  void addToQueueIfMergerIsFull(KeyType value);
  void mergeCalculate(KeyType temp);
  void ifNormal(KeyType index, KeyType branchValue);
  void ifMerger(KeyType index, KeyType branchValue);
  void firstVisit(KeyType index, KeyType branchValue);
  void ifBranchIsEmpty(KeyType index, KeyType branchValue);
  void subsequentVisit(KeyType index);

  bool isSplitter(KeyType index) const;
  bool isMerger(KeyType index) const;
  bool isExit(KeyType index) const;
  bool isNormal(KeyType index) const;

  bool isNotVisited(KeyType index) const;
  bool isVisitedOnce(KeyType index) const;
  bool isCompleted(KeyType index) const;

private:
  const HashMapOfHashSet& m_layersForward;
  const HashMap& m_layerInputCounter;
  HashMapOfHashSet m_layersBranch;
  HashMapOfHashSet m_merger;
  HashMap m_splitter;
  Queue m_mergerQueue;
  Queue m_splitterQueue;
  Set m_activeBranchSet;
  KeyType m_branchValue{0};
};
