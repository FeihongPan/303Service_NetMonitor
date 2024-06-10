/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        netmonitorwidget.h
* Description:      网络设备监控主页面
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record：
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
    /// 设置IP
    /// </summary>
    /// <param name="strIp">该Widget对应的IP值param>
    inline void SetIp(const QString& strIp);

    /// <summary>
    /// 更新App状态，相应checkbox显示
    /// </summary>
    /// <param name="dic_AppState">对应IP部署软件的实时状态数据结构</param>
    void UpdateAppState(S_Dic_StringBool &dic_AppState);

protected:
    /// <summary>
    /// 根据App个数生成相应的按钮等控件个数
    /// </summary>
    virtual void setWidgets() override;

    virtual void setWidgetStyleSheet() override;

    virtual void setLanguageKeys() override;

    /// <summary>
    /// 链接每个按钮和IP, 点击按钮后发送相应消息给控制层，以及监控软件的连接状态显示
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
