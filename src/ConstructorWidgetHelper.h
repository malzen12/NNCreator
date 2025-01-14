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

  void clear();

  auto createName(const NNWidget* pParam) -> std::string;
  auto createNameEnum(const NNWidget* pParam) -> std::string;
  auto createNameMyRecurrent(const NNWidget* pParam) -> std::string;

  void printCopy(const KeyType index, const Branch::BranchReturnValue& branch);
  void printNameForward(const NNWidget* pLayer, const Branch::BranchReturnValue& branch);

  void createInitLValue(const NNWidget* pCurrent);
  void createInitRValue(const NNWidget* pCurrent);

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
  using NNLayerContainer = std::unordered_map<KeyType, NNWidget*>;

public:
  explicit GraphManager(const NNLayerContainer& m_vLayers);

  auto getFinishIndex() const noexcept -> KeyType;
  auto getStartIndex() const noexcept -> KeyType;
  auto getForward(KeyType index) const -> const HashSet&;
  bool isValid();

  auto print() -> std::string;

  auto save() const -> std::string;

  void load(std::stringstream& stream);

  void addEdge(KeyType from, KeyType to);
  void addVertex(KeyType index);
  void deleteEdge(KeyType from, KeyType to);
  void deleteVertex(KeyType index);
  void checkSizes(const Vector& vInputSize);

  void backwardDfsForSizes(NNWidget* pCurrent);
  void setInvalid();
  void clear() noexcept;

signals:
  void notValid(const std::string&);

private:
  void printInit();
  void printForward();
  void backwardDfs(const NNWidget* pCurrent);

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
