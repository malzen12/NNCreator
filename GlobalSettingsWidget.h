#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class GlobalSettingsWidget: public QWidget
{
    Q_OBJECT

public:
    GlobalSettingsWidget();

signals:
    void inputSizeChanged(const std::vector<std::size_t>& );
    void makeXml();

private slots:
    void onUpdateInputSize();

private:
    void initGUI();
    void createConnections();

//    QLineEdit* m_pSavePathEdit;
//    QPushButton* m_pConfirmPathButton;

    QLineEdit* m_pInputSizeEdit;
    QPushButton* m_pConfirmSizeButton;

    QPushButton* m_pMakeXmlButton;

    std::vector<std::size_t> m_vInputSize;
};

