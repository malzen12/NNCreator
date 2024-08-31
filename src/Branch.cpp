#include "Branch.h"

Branch::Branch(const std::unordered_map<std::size_t, std::unordered_set<std::size_t>>& vLayers,
               const std::unordered_map<std::size_t, std::size_t>& vLayerInputCounter)
  :vLayers(vLayers), vLayerInputCounter(vLayerInputCounter)
{}

void Branch::goToFirstUnhandledUntilFindMerge(std::size_t iCurrent, std::size_t branchValue){
  if(isNormal(iCurrent)){
    vLayersBranch[iCurrent].push_back(branchValue);
    if(!vLayers.at(iCurrent).empty()){
      addToQueueIfSplitter(*vLayers.at(iCurrent).cbegin());
      goToFirstUnhandledUntilFindMerge(*vLayers.at(iCurrent).cbegin(), vLayersBranch[iCurrent].front());
    }
    return;
  }
  if(isMerger(iCurrent)){
    mergerMap[iCurrent].push_back(branchValue);
    addToQueueIfMergerIsFull(iCurrent);
    return;
  }

  if(isNotVisited(iCurrent)){
    if(vLayersBranch[iCurrent].empty())//if first enter
      vLayersBranch[iCurrent].push_back(branchValue);
    addToQueueIfSplitter(*vLayers.at(iCurrent).cbegin());
    goToFirstUnhandledUntilFindMerge(*vLayers.at(iCurrent).cbegin(), vLayersBranch[iCurrent].front());
//    for(auto it = std::next(vLayers.at(iCurrent).cbegin()), ite = vLayers.at(iCurrent).cend(); it != ite; ++it)
//      addToQueueIfSplitter(*it);
    addToQueueIfSplitter(*std::next(vLayers.at(iCurrent).cbegin()));
  }else if(isVisitedOnce(iCurrent)){
    vLayersBranch[iCurrent].push_back(m_branchValue);
    goToFirstUnhandledUntilFindMerge(*std::next(vLayers.at(iCurrent).cbegin()), m_branchValue);
    activeBranchSet.insert(m_branchValue++);
  }
  ++splitterMap[iCurrent];
}

void Branch::splitter(std::size_t iCurrent)
{
  while(!splitterQueue.empty()){
    goToFirstUnhandledUntilFindMerge(iCurrent, vLayersBranch[iCurrent].front());

    if(isNormal(iCurrent) || isCompleted(iCurrent))
      splitterQueue.pop();
    iCurrent = splitterQueue.front();
  }
}

void Branch::merger()
{
  while(!mergerQueue.empty()){
    auto iCurrent = mergerQueue.front();
    mergeCalculate(iCurrent);
    if(!vLayers.at(iCurrent).empty()){
      std::size_t temp = *vLayers.at(iCurrent).cbegin();
      std::size_t branch = vLayersBranch[iCurrent].front();
      if(isMerger(temp)){
        mergerMap[temp].push_back(branch);
        if(!isExit(temp))
          addToQueueIfMergerIsFull(temp);
        else
          mergeCalculate(temp);
      }else if(isSplitter(temp)){
        vLayersBranch[temp].push_back(branch);
        splitterQueue.push(temp);
      }
    }
    mergerQueue.pop();
  }
}

void Branch::clear()
{
  vLayersBranch.clear();
  splitterMap.clear();
  mergerMap.clear();
  activeBranchSet.clear();
  m_branchValue = 0;
}

void Branch::init(std::size_t head)
{
  splitterQueue.push(head);
  vLayersBranch[head].push_back(0);
  activeBranchSet.insert(m_branchValue++);
}

BranchReturnValue Branch::calculate(std::size_t head){
  clear();
  init(head);

  while(!splitterQueue.empty() || !mergerQueue.empty()){
    splitter(splitterQueue.front());
    merger();
    m_branchValue = !activeBranchSet.empty() ? *activeBranchSet.rbegin() + 1 : 0;
  }
  BranchReturnValue result;
  result.mergerMap = std::move(mergerMap);
  result.vLayersBranch = std::move(vLayersBranch);
  return result;
}

void Branch::addToQueueIfSplitter(std::size_t value){
  if(vLayers.at(value).size() == 2)
    splitterQueue.push(value);
};
void Branch::addToQueueIfMergerIsFull(std::size_t value){
  if(mergerMap[value].size() == 2)
    mergerQueue.push(value);
};

bool Branch::isSplitter(std::size_t value){
  return vLayers.at(value).size() == 2;
}
bool Branch::isMerger(std::size_t value){
  return vLayers.at(value).size() <= 1;
}
bool Branch::isExit(std::size_t value){
  return vLayers.at(value).size() == 0;
}
bool Branch::isNormal(std::size_t value){
  return vLayers.at(value).size() <= 1
      && vLayerInputCounter.at(value) <= 1;
}

void Branch::mergeCalculate(std::size_t index){
  vLayersBranch[index].push_back(std::min(mergerMap[index].front(), mergerMap[index].back()));
  activeBranchSet.erase(std::max(mergerMap[index].front(), mergerMap[index].back()));
};

bool Branch::isNotVisited(std::size_t value){
  return splitterMap.find(value) == splitterMap.end();
}
bool Branch::isVisitedOnce(std::size_t value){
  return splitterMap[value] == 1;
}
bool Branch::isCompleted(std::size_t value){
  return splitterMap[value] == 2;
}
