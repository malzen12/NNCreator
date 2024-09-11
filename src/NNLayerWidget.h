#pragma once

#include "NNLayerParams.h"
#include <QApplication>
#include <QLabel>

class NNLayerWidget : public QWidget {
  Q_OBJECT
  using KeyType = std::size_t;
  using Vector = std::vector<KeyType>;

public:
  explicit NNLayerWidget(KeyType sId, const std::shared_ptr<NNLayerParams>& spParams);
  NNLayerWidget(const NNLayerWidget& another);

  static auto makeSizeString(const Vector& vSize = {}) -> QString;
  static auto createId() -> KeyType;

  // auto copy() const -> NNLayerWidget*;

  auto getId() const -> KeyType;
  auto getParams() const noexcept -> const std::shared_ptr<NNLayerParams>&;
  auto getInputSize() const -> const std::vector<Vector>&;
  auto getGrabbedPos() const noexcept -> const QPoint&;

  void setParams(const std::shared_ptr<NNLayerParams>& spParams) noexcept;
  void setMoved(bool value);
  void changeId(KeyType id);

  bool isGrabbed() const;
  bool isActive() const;
  bool isMoved() const;

  void mousePressEvent(QMouseEvent* pEvent) final;
  // void mouseMoveEvent(QMouseEvent* pEvent) final;
  void mouseReleaseEvent(QMouseEvent* pEvent) final;

  void makeActive(bool bActive);
  void toActive();

  void deleteLayer();

  void resetInputSize();
  void addInputSize(const Vector& vInputSize);

  auto calcOutputSize() const -> Vector;

signals:
  void becomeActive(KeyType);
  void makeForward(KeyType);
  void addToActive(KeyType);
  void delFromActive(KeyType);
  void collectIsMoved(KeyType);
  void setIsMoved(bool);
  void grabbed(KeyType, bool);
  void waitToDrag();

private:
  void initGUI();
  void updateStyle();
  void releaseCtrlLeft();
  void pressLeft(QMouseEvent* pEvent);

  auto createName() const -> QString;

private:
  KeyType m_sId;
  QPoint m_GrabbedPos;

  bool m_isActive;
  bool m_isGrabbed;
  bool m_isValidParams;
  bool m_isMoved;

  std::shared_ptr<NNLayerParams> m_spParams;
  std::vector<Vector> m_vInputSizes;

  QLabel* m_pInputLabel;
  QLabel* m_pNameLabel;
  QLabel* m_pOutputLabel;
};
