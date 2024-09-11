#include "Branch.h"

Branch::Branch(const HashMapOfHashSet& vLayers, const HashMap& vLayerInputCounter)
    : m_layersForward(vLayers)
    , m_layerInputCounter(vLayerInputCounter) {}

auto Branch::calculate(KeyType head) -> BranchReturnValue {
  clear();
  init(head);

  while (! m_splitterQueue.empty() || ! m_mergerQueue.empty()) {
    split(m_splitterQueue.front());
    merge();
    m_branchValue = ! m_activeBranchSet.empty() ? *m_activeBranchSet.rbegin() + 1 : 0;
  }
  BranchReturnValue result{std::move(m_merger), std::move(m_layersBranch)};
  return result;
}

/*!
 * Take first unhandled and go deep down until find Merger.
 * When step on new vertex add to queue all children.
 */
void Branch::bypassGraph(KeyType index, KeyType branchValue) {
  if (isNormal(index)) {
    ifNormal(index, branchValue);
    return;
  }
  if (isMerger(index)) {
    ifMerger(index, branchValue);
    return;
  }
  if (isNotVisited(index))
    firstVisit(index, branchValue);
  else if (isVisitedOnce(index))
    subsequentVisit(index);

  ++m_splitter[index];
}

void Branch::split(KeyType index) {
  while (! m_splitterQueue.empty()) {
    auto current = hashSetFirst(m_layersBranch.at(index));
    bypassGraph(index, current);
    ifCompletePopQueue(index);
    index = m_splitterQueue.front();
  }
}

void Branch::ifCompletePopQueue(KeyType index) {
  if (isNormal(index) || isCompleted(index)) m_splitterQueue.pop();
}

void Branch::merge() {
  while (! m_mergerQueue.empty()) {
    auto index = m_mergerQueue.front();
    mergeCalculate(index);
    if (m_layersForward.at(index).empty()) {
      m_mergerQueue.pop();
      return;
    }
    auto next = hashSetFirst(m_layersForward.at(index));
    auto branch = hashSetFirst(m_layersBranch.at(index));
    if (isMerger(next))
      ifMergerInMerge(next, branch);
    else if (isSplitter(next))
      ifSplitterInMerge(next, branch);
    m_mergerQueue.pop();
  }
}

void Branch::ifMergerInMerge(KeyType index, KeyType branchValue) {
  m_merger[index].insert(branchValue);
  if (! isExit(index))
    addToQueueIfMergerIsFull(index);
  else
    mergeCalculate(index);
}

void Branch::ifSplitterInMerge(KeyType index, KeyType branchValue) {
  m_layersBranch[index].insert(branchValue);
  m_splitterQueue.push(index);
}

void Branch::clear() {
  m_layersBranch.clear();
  m_splitter.clear();
  m_merger.clear();
  m_activeBranchSet.clear();
  m_branchValue = 0;
}

void Branch::init(KeyType head) {
  m_splitterQueue.push(head);
  m_layersBranch[head].insert(0);
  m_activeBranchSet.insert(m_branchValue++);
}

void Branch::addToQueueIfSplitter(KeyType value) {
  if (m_layersForward.at(value).size() == 2) m_splitterQueue.push(value);
}

void Branch::addToQueueIfMergerIsFull(KeyType value) {
  if (m_merger[value].size() == 2) m_mergerQueue.push(value);
}

void Branch::mergeCalculate(KeyType index) {
  KeyType first = hashSetFirst(m_merger.at(index));
  KeyType second = hashSetSecond(m_merger.at(index));
  m_layersBranch[index].insert(std::min(first, second));
  m_activeBranchSet.erase(std::max(first, second));
}

void Branch::ifNormal(KeyType index, KeyType branchValue) {
  m_layersBranch[index].insert(branchValue);
  if (auto hashSet = m_layersForward.at(index); ! hashSet.empty()) {
    auto next = hashSetFirst(hashSet);
    addToQueueIfSplitter(next);
    bypassGraph(next, hashSetFirst(hashSet));
  }
}

void Branch::ifMerger(KeyType index, KeyType branchValue) {
  m_merger[index].insert(branchValue);
  addToQueueIfMergerIsFull(index);
}

void Branch::firstVisit(KeyType index, KeyType branchValue) {
  ifBranchIsEmpty(index, branchValue);
  auto next = hashSetFirst(m_layersForward.at(index));
  addToQueueIfSplitter(next);
  auto branch = hashSetFirst(m_layersBranch.at(index));
  bypassGraph(next, branch);
  auto other = hashSetSecond(m_layersForward.at(index));
  addToQueueIfSplitter(other);
}

void Branch::ifBranchIsEmpty(KeyType index, KeyType branchValue) {
  if (m_layersBranch.at(index).empty()) // if first enter
    m_layersBranch[index].insert(branchValue);
}

void Branch::subsequentVisit(KeyType index) {
  m_layersBranch[index].insert(m_branchValue);
  auto next = hashSetSecond(m_layersForward.at(index));
  bypassGraph(next, m_branchValue);
  m_activeBranchSet.insert(m_branchValue++);
}

bool Branch::isSplitter(KeyType index) const { return m_layersForward.at(index).size() == 2; }

bool Branch::isMerger(KeyType index) const { return m_layersForward.at(index).size() <= 1; }

bool Branch::isExit(KeyType index) const { return m_layersForward.at(index).size() == 0; }

bool Branch::isNormal(KeyType index) const {
  return m_layersForward.at(index).size() <= 1 && m_layerInputCounter.at(index) <= 1;
}

bool Branch::isNotVisited(KeyType index) const { return m_splitter.find(index) == m_splitter.end(); }

bool Branch::isVisitedOnce(KeyType index) const { return m_splitter.at(index) == 1; }

bool Branch::isCompleted(KeyType index) const { return m_splitter.at(index) == 2; }
