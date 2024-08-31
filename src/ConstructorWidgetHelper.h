#pragma once


#include "Branch.h"
#include "NNLayerWidget.h"
#include <algorithm>
#include <iostream>
#include <sstream>





class PyPrinter{
public:
  template<class T>
  std::stringstream& operator<<(T t){
    Stream << std::string(tab, '\t') << t;
    return Stream;
  }

  template<class T>
  void addToLine(T t){
    Stream << t;
  }

  void endl(){
    Stream << std::endl;
  }

  PyPrinter& operator++(){
    ++tab;
    return *this;
  }
  PyPrinter& operator--(){
    if(tab != 0)
      --tab;
    return *this;
  }

  std::stringstream& getStreamRef();
  const std::string createName(const NNLayerWidget* pParam);;
  const std::string createNameEnum(const NNLayerWidget* pParam);;
  const std::string createNameMyRecurrent(const NNLayerWidget* pParam);;

  void printCopy(const std::size_t index, const BranchReturnValue& branch);
  void printNameForward(const NNLayerWidget* pLayer, const BranchReturnValue& branch);;

  void createInitLValue (const NNLayerWidget* pCurrent);;
  void createInitRValue (const NNLayerWidget* pCurrent);;
private:
  std::stringstream Stream{};
  std::size_t tab{};
};


class GraphManager: public QObject
{
  Q_OBJECT

public:
  explicit GraphManager(const std::unordered_map<std::size_t, NNLayerWidget*>& m_vLayers);
  const std::string print();
  std::size_t getFinishIndex();
  std::size_t getStartIndex();
  void addEdge(std::size_t from, std::size_t to);
  void addVertex(std::size_t index);
  void deleteEdge(std::size_t from, std::size_t to);
  void deleteVertex(std::size_t index);
  void checkSizes(const std::vector<std::size_t>& vInputSize);

  void backwardDfsForSizes(NNLayerWidget* pCurrent);;
  void setInvalid();

signals:
  void notValid(const std::string&);
private:
  void printInit();
  void printForward();

  void backwardDfs(const NNLayerWidget* pCurrent);;
  void clear();
  void init();
  void initContainers();
  bool isValid();
  void initStartAndFinish();
  void error(){throw(std::runtime_error("graph not valid"));};///< @todo
private:
  const std::unordered_map<std::size_t, NNLayerWidget*>& m_refvLayers;
  std::unordered_map<std::size_t, std::unordered_set<std::size_t>> m_backward;
  std::unordered_set<std::size_t> m_calcucatedDfs;
  std::unordered_set<std::size_t> m_calcucatedDfsSize;
  std::unordered_map<std::size_t, std::unordered_set<std::size_t>> m_vLayersIndex;
  std::unordered_map<std::size_t, std::size_t> m_vLayerInputCounter;

  std::size_t iStart{};
  std::size_t iFinish{};
  BranchReturnValue branch;
  PyPrinter pyPrinter{};
  bool m_isValid{true};
};


