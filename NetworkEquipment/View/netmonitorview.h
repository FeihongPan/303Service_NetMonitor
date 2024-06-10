/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        netmonitorview.h
* Description:      �����豸�����ҳ��
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record��
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
    /// ��������NetworkEquipmentSubscriber���źţ�������Ӧ����
    /// </summary>
    virtual void setConnections() override;

private:
    /// <summary>
    /// ���ɶ�ӦIP���ӿؼ������г�ʼ��
    /// </summary>
    /// <param name="strIp">��Ӧip</param>
    /// <param name="pWidget">NetMonitorWidgetָ��</param>
    void initializeNetWidget(const QString& strIp, NetMonitorWidget* pWidget);

    /// <summary>
    /// ȷ���ӿؼ����������λ��
    /// </summary>
    /// <param name="pWidget">NetMonitorWidgetָ��</param>
    void setNetWidgetPlace(NetMonitorWidget* pWidget);

    void checkNetWidget();

    /// <summary>
    /// ͨ�����������źŴ�����ӿؼ�������Ӧ����
    /// </summary>
    /// <param name="strIp">��Ӧip</param>
    void updateAppState(const QString& strIp);

    /// <summary>
    /// ����App���߽��б�������
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
