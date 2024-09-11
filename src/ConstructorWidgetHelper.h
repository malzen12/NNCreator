#pragma once

#include "Branch.h"
#include "NNLayerWidget.h"
#include <sstream>

class PyPrinter {
  using KeyType = std::size_t;

public:
  template<class T>
  std::stringstream& operator<<(T t) {
    m_Stream << std::string(m_tab, '\t') << t;
    return m_Stream;
  }

  template<class T>
  void addToLine(T t) {
    m_Stream << t;
  }

  void endl();

  PyPrinter& operator++() {
    ++m_tab;
    return *this;
  }

  PyPrinter& operator--() {
    if (m_tab != 0) --m_tab;
    return *this;
  }

  auto getStreamRef() -> std::stringstream&;

  auto createName(const NNLayerWidget* pParam) -> std::string;
  auto createNameEnum(const NNLayerWidget* pParam) -> std::string;
  auto createNameMyRecurrent(const NNLayerWidget* pParam) -> std::string;

  void printCopy(const KeyType index, const Branch::BranchReturnValue& branch);
  void printNameForward(const NNLayerWidget* pLayer, const Branch::BranchReturnValue& branch);

  void createInitLValue(const NNLayerWidget* pCurrent);
  void createInitRValue(const NNLayerWidget* pCurrent);

private:
  std::stringstream m_Stream{};
  KeyType m_tab{};
};

class GraphManager : public QObject {
  Q_OBJECT

  using KeyType = std::size_t;
  using Vector = std::vector<KeyType>;
  using HashSet = std::unordered_set<KeyType>;
  using HashMapOfHashSet = std::unordered_map<KeyType, HashSet>;
  using HashMap = std::unordered_map<KeyType, KeyType>;
  using NNLayerContainer = std::unordered_map<KeyType, NNLayerWidget*>;

public:
  explicit GraphManager(const NNLayerContainer& m_vLayers);

  auto getFinishIndex() const noexcept -> KeyType;
  auto getStartIndex() const noexcept -> KeyType;
  auto getForward(KeyType index) const -> const HashSet&;
  bool isValid();

  auto print() -> std::string;

  void addEdge(KeyType from, KeyType to);
  void addVertex(KeyType index);
  void deleteEdge(KeyType from, KeyType to);
  void deleteVertex(KeyType index);
  void checkSizes(const Vector& vInputSize);

  void backwardDfsForSizes(NNLayerWidget* pCurrent);
  void setInvalid();

signals:
  void notValid(const std::string&);

private:
  void printInit();
  void printForward();
  void backwardDfs(const NNLayerWidget* pCurrent);
  void clear() noexcept;
  void init();
  void initStartAndFinish();

private:
  const NNLayerContainer& m_refvLayers;

  HashMapOfHashSet m_backward;
  HashMapOfHashSet m_layersIndex;

  HashMap m_layerInputCounter;
  HashSet m_calcucatedDfs;
  HashSet m_calcucatedDfsSize;

  KeyType iStart{};
  KeyType iFinish{};

  Branch::BranchReturnValue branch;

  PyPrinter pyPrinter{};

  bool m_isValid{true};
};
