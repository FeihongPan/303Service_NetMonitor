/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        netmonitorview.h
* Description:      网络设备监控主页面
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record：
***********************************************************************/

#ifndef NETMONITORVIEW_H
#define NETMONITORVIEW_H

#include <QWidget>
#include "AuthorityComponent/authoritywidget.h"
#include "NetworkEquipment/Data/s_netmonitor.h"

class NetMonitorWidget;

namespace Ui
{
    class NetMonitorView;
}

class NetMonitorView : public AuthorityWidget
{
    Q_OBJECT

public:
    explicit NetMonitorView(QString name, QWidget *parent = nullptr);
    ~NetMonitorView();

protected:
    /// <summary>
    /// 接收来自NetworkEquipmentSubscriber的信号，进行相应操作
    /// </summary>
    virtual void setConnections() override;

private:
    /// <summary>
    /// 生成对应IP的子控件并进行初始化
    /// </summary>
    /// <param name="strIp">对应ip</param>
    /// <param name="pWidget">NetMonitorWidget指针</param>
    void initializeNetWidget(const QString& strIp, NetMonitorWidget* pWidget);

    /// <summary>
    /// 确定子控件在主界面的位置
    /// </summary>
    /// <param name="pWidget">NetMonitorWidget指针</param>
    void setNetWidgetPlace(NetMonitorWidget* pWidget);

    void checkNetWidget();

    /// <summary>
    /// 通过接受来的信号传输给子控件进行相应操作
    /// </summary>
    /// <param name="strIp">对应ip</param>
    void updateAppState(const QString& strIp);

    /// <summary>
    /// 当有App断线进行报警操作
    /// </summary>
    void setViewState();

private:
    Ui::NetMonitorView *ui;

    S_Dic_AppState m_dic_AllAppState;

    QList<QString> m_lst_Ip;
    QList<QString> m_lst_Widget_Ip;
    QMap<QString, NetMonitorWidget *> m_dic_NetMonitorWidget;

    int m_nLeftHeight;
    int m_nMiddleHeight;
    int m_nRightHeight;
    int m_nStartWidth;
    bool m_bAllAppState;
};

#endif // NETMONITORVIEW_H
