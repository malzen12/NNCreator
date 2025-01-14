#pragma once

#include "NNLayerParams.h"
#include <sstream>
#include <QApplication>
#include <QLabel>

class NNWidget : public QWidget {
  Q_OBJECT
  using KeyType = std::size_t;
  using Vector = std::vector<KeyType>;
  using VectorOfVector = std::vector<Vector>;

public:
  explicit NNWidget(KeyType id = SIZE_MAX);
  explicit NNWidget(KeyType id = SIZE_MAX, QString name = "Widget");
  NNWidget(const NNWidget& another);

  static auto createId(KeyType id = SIZE_MAX) -> KeyType;
  static auto makeSizeString(const Vector& vSize = {}) -> QString;

  auto getId() const -> KeyType;
  auto getErrors() const -> const std::string&;
  auto getType() const -> const QString&;

  bool isGrabbed() const;
  bool isActive() const;
  bool isMoved() const;

  void setMoved(bool value);
  void changeId(KeyType id);

  void mousePressEvent(QMouseEvent* pEvent) final;
  // void mouseMoveEvent(QMouseEvent* pEvent) final;
  void mouseReleaseEvent(QMouseEvent* pEvent) final;

  void makeActive(bool bActive);
  void toActive();

  void deleteThis();

  virtual void updateStyle() = 0;
  virtual auto save() const -> std::string = 0;

  virtual auto getParams() const noexcept -> const std::shared_ptr<NNLayerParams>& = 0;
  virtual auto getInputSize() const -> const VectorOfVector& = 0;
  virtual auto getMainOutputSize() -> Vector = 0;
  virtual auto getOptionalOutputSize(KeyType index) -> Vector = 0;

  virtual void setParams(const std::shared_ptr<NNLayerParams>& spParams) noexcept = 0;

  virtual void load(std::stringstream& stream) = 0;

  virtual void resetInputSize() = 0;
  virtual void resetInputAndOutputSize() = 0;
  virtual void checkIsValid() = 0;
  virtual void addInputSize(const Vector& vInputSize) = 0;

  virtual auto calcOutputSize() -> const VectorOfVector& = 0;

signals:
  void becomeActive(KeyType);
  void makeForward(KeyType);
  void addToActive(KeyType);
  void delFromActive(KeyType);
  void collectIsMoved(KeyType);
  void setIsMoved(bool);
  void grabbed(KeyType, QPoint);
  void ungrabbed();

private:
  void releaseCtrlLeft();
  void pressLeft(QMouseEvent* pEvent);

protected:
  KeyType m_id;

  bool m_isActive;
  bool m_isValidParams;
  bool m_isGlobalError;
  bool m_isMoved;

  std::string m_strErrors;
  QString m_strType;
};
