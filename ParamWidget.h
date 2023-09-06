#pragma once

#include <QWidget>
#include <QLabel>

#include "NNParam.h"

class ParamWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ParamWidget(const NNParam& crParam);

    const std::string& getName() const noexcept;

    const NNParam& collectValue();

private:
    void initGUI();

    NNParam m_Param;

    QLabel* m_pNameLabel;
    QWidget* m_pEditorWidget;
};

