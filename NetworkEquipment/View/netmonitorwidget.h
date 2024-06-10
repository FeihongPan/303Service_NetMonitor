/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        netmonitorwidget.h
* Description:      �����豸�����ҳ��
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record��
***********************************************************************/

#ifndef NETMONITORWIDGET_H
#define NETMONITORWIDGET_H

#include <QWidget>
#include "AuthorityComponent/authoritywidget.h"
#include "BasicValue/serializabledic.h"
#include "NetworkEquipment/Data/s_netmonitor.h"

namespace Ui
{
    class NetMonitorWidget;
}

class NetMonitorWidget : public AuthorityWidget
{
    Q_OBJECT

public:
    explicit NetMonitorWidget(QString name, QWidget *parent = nullptr);
    ~NetMonitorWidget();

    /// <summary>
    /// ����IP
    /// </summary>
    /// <param name="strIp">��Widget��Ӧ��IPֵparam>
    inline void SetIp(const QString& strIp);

    /// <summary>
    /// ����App״̬����Ӧcheckbox��ʾ
    /// </summary>
    /// <param name="dic_AppState">��ӦIP���������ʵʱ״̬���ݽṹ</param>
    void UpdateAppState(S_Dic_StringBool &dic_AppState);

protected:
    /// <summary>
    /// ����App����������Ӧ�İ�ť�ȿؼ�����
    /// </summary>
    virtual void setWidgets() override;

    virtual void setWidgetStyleSheet() override;

    virtual void setLanguageKeys() override;

    /// <summary>
    /// ����ÿ����ť��IP, �����ť������Ӧ��Ϣ�����Ʋ㣬�Լ�������������״̬��ʾ
    /// </summary>
    virtual void setConnections() override;

private:
    Ui::NetMonitorWidget *ui;
    S_Dic_StringBool m_dic_Appstate;

    QList<QString> m_lst_App;
    QMap<QString, QLineEdit *> m_dic_led_Appname;
    QMap<QString, QCheckBox *> m_dic_ckb_Appstate;
    QMap<QString, QPushButton *> m_dic_btn_Open;
    QMap<QString, QPushButton *> m_dic_btn_Close;
    QMap<QString, QPushButton *> m_dic_btn_Restart;

    QString m_strIp;
    QVBoxLayout *m_Vly;
    bool m_NetMonitorCalled;
};

inline void NetMonitorWidget::SetIp(const QString &strIp)
{
    m_strIp = strIp;
}

#endif // NETMONITORWIDGET_H
