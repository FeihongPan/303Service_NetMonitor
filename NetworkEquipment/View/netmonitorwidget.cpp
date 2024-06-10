/***********************************************************************
* Copyright (C) 2024, iStar.
*
* File Name:        netmonitorwidget.cpp
* Description:      网络设备监控主页面
* Version:          V1.0.0
* Author:           Feihong Pan
* Date:             2024-1-16
* Modify Record：
***********************************************************************/

#include "netmonitorwidget.h"
#include "ui_netmonitorwidget.h"
#include "NetworkEquipment/Subscriber/networkequipmentsubscriber.h"
#include "NetworkEquipment/Command/cmd_netmonitor.h"
#include "GUIConfig/AppParameter.h"

NetMonitorWidget::NetMonitorWidget(QString name, QWidget *parent)
    : AuthorityWidget(name, parent)
    , ui(new Ui::NetMonitorWidget)
    , m_NetMonitorCalled(false)
{
    ui->setupUi(this);
    //    m_Publisher.LinkToClient(MqttClientManager::getInstance()->GetClient(0), PublishTopicPrefix);
    m_Vly = new QVBoxLayout(ui->groupBox);
}

NetMonitorWidget::~NetMonitorWidget()
{
    delete ui;
}

void NetMonitorWidget::UpdateAppState(S_Dic_StringBool &dic_AppState)
{
    m_dic_Appstate = dic_AppState;
    foreach (auto &strAppName, m_lst_App)
    {
#ifdef USEFORDEBUG
        auto bIsMC = (strAppName == "MC");
#endif
        if (!bIsMC)
        {
            #pragma region Checkbox{
            auto bAppState = m_dic_Appstate.values[strAppName];
            m_dic_ckb_Appstate[strAppName]->setChecked(bAppState);
            #pragma endregion};

            #pragma region ButtonState{
            if (bAppState)
            {
                m_dic_btn_Open[strAppName]->setEnabled(false);
                m_dic_btn_Close[strAppName]->setEnabled(true);
                m_dic_btn_Restart[strAppName]->setEnabled(true);
            }
            else
            {
                m_dic_btn_Open[strAppName]->setEnabled(true);
                m_dic_btn_Close[strAppName]->setEnabled(false);
                m_dic_btn_Restart[strAppName]->setEnabled(false);
                emit ViewStatusChanged(ViewStatus::Alarm);
            }
            #pragma endregion};
        }
    }
}

void NetMonitorWidget::setWidgets()
{
    m_lst_App = m_dic_Appstate.values.keys();
    foreach (auto &strAppName, m_lst_App)
    {
        QWidget *appwidget = new QWidget(this);
        QHBoxLayout *applayout = new QHBoxLayout(this);

        #pragma region AppName{
        auto led_app = new QLineEdit(this);
        led_app->setText(strAppName);
        led_app->setReadOnly(true);
        #pragma endregion};

        #pragma region AppState{
        auto ckb_state = new QCheckBox(this);
        ckb_state->setEnabled(false);
        ckb_state->setChecked(m_dic_Appstate.values[strAppName]);
        #pragma endregion};

        #pragma region Button{
        QPushButton *btn_open = new QPushButton(Tr("Open"));
        QPushButton *btn_close = new QPushButton(Tr("Close"));
        QPushButton *btn_restart = new QPushButton(Tr("Restart"));
        #pragma endregion};

        applayout->addWidget(led_app);
        applayout->addWidget(ckb_state);
        applayout->addWidget(btn_open);
        applayout->addWidget(btn_close);
        applayout->addWidget(btn_restart);
        appwidget->setLayout(applayout);

        m_dic_led_Appname.insert(strAppName, led_app);
        m_dic_ckb_Appstate.insert(strAppName, ckb_state);
        m_dic_btn_Open.insert(strAppName, btn_open);
        m_dic_btn_Close.insert(strAppName, btn_close);
        m_dic_btn_Restart.insert(strAppName, btn_restart);

        m_Vly->addWidget(appwidget);

#ifdef USEFORDEBUG
        auto bIsMC = (strAppName == "MC");
        if (bIsMC)
        {
            m_dic_ckb_Appstate[strAppName]->setChecked(true);
        }
#endif
    }
}

void NetMonitorWidget::setWidgetStyleSheet()
{
    #pragma region groupBox{
    QSize m_Size = QSize(570, 110 + 65 * m_lst_App.count());
    resize(m_Size);

    ui->lbl_App->setText("App");
    ui->lbl_State->setText("State");
    ui->lbl_App->setStyleSheet(GlobalStyleSheet->fontStyle_normal_12);
    ui->lbl_State->setStyleSheet(GlobalStyleSheet->fontStyle_normal_12);

    ui->groupBox->resize(m_Size);
    ui->groupBox->SetheaderFontSize(16);
    ui->groupBox->SetHeaderText(Tr("IP: " + m_strIp));
    ui->groupBox->setStyleSheet(GlobalStyleSheet->GroupBoxStyle + GlobalStyleSheet->fontStyle_normal_12);
    m_Vly->addStretch();
    m_Vly->setContentsMargins(0, 100, 0, 0);
    #pragma endregion};

    #pragma region Button{
    foreach (auto &strAppName, m_lst_App)
    {
        m_dic_led_Appname[strAppName]->setAlignment(Qt::AlignHCenter);
        m_dic_led_Appname[strAppName]->setStyleSheet(GlobalStyleSheet->LineEdit_Dark + GlobalStyleSheet->fontStyle_normal_12);
        m_dic_led_Appname[strAppName]->setFixedSize(150, 40);
        m_dic_ckb_Appstate[strAppName]->setFixedSize(30, 30);
        m_dic_btn_Open[strAppName]->setFixedSize(81, 30);
        m_dic_btn_Close[strAppName]->setFixedSize(81, 30);
        m_dic_btn_Restart[strAppName]->setFixedSize(81, 30);
        m_dic_btn_Open[strAppName]->setStyleSheet(GlobalStyleSheet->Button_Border + GlobalStyleSheet->fontStyle_normal_12);
        m_dic_btn_Close[strAppName]->setStyleSheet(GlobalStyleSheet->Button_Border + GlobalStyleSheet->fontStyle_normal_12);
        m_dic_btn_Restart[strAppName]->setStyleSheet(GlobalStyleSheet->Button_Border + GlobalStyleSheet->fontStyle_normal_12);
    }
    ui->btn_NetState->setStyleSheet(GlobalStyleSheet->Button_Border + "QWidget{font-family:HarmonyOS Sans;font-weight:normal;font-size:10pt;}");
    #pragma endregion};
}

void NetMonitorWidget::setLanguageKeys()
{
    Tr("Open");
    Tr("Close");
    Tr("Restart");
}

void NetMonitorWidget::setConnections()
{
    foreach (auto &strAppName, m_lst_App)
    {
        S_AppOperator dic_AppNameInfo;
        dic_AppNameInfo.IP = m_strIp;
        dic_AppNameInfo.appName = strAppName;

        connect(m_dic_btn_Open[strAppName], &QPushButton::clicked, this, [ = ]
        {
#ifdef USEFORDEBUG
            auto bIsMC = (strAppName == "MC");
            if (bIsMC)
            {
                m_Publisher.Publish("NetworkEquipment", EnumConverter::ConvertToString(CMD_NetMonitor::CMD_AppOpenMC), true, dic_AppNameInfo.toRawJson());
                m_dic_ckb_Appstate[strAppName]->setChecked(true);
            }
#endif
            else
            {
                m_Publisher.Publish("NetworkEquipment", EnumConverter::ConvertToString(CMD_NetMonitor::CMD_AppOpen), true, dic_AppNameInfo.toRawJson());
            }
        });

        connect(m_dic_btn_Close[strAppName], &QPushButton::clicked, this, [ = ]
        {
            m_Publisher.Publish("NetworkEquipment", EnumConverter::ConvertToString(CMD_NetMonitor::CMD_AppClose), true, dic_AppNameInfo.toRawJson());
#ifdef USEFORDEBUG
            auto bIsMC = (strAppName == "MC");
            if (bIsMC)
            {
                m_dic_ckb_Appstate[strAppName]->setChecked(false);
            }
#endif
        });

        connect(m_dic_btn_Restart[strAppName], &QPushButton::clicked, this, [ = ]
        {
            m_Publisher.Publish("NetworkEquipment", EnumConverter::ConvertToString(CMD_NetMonitor::CMD_AppRestart), true, dic_AppNameInfo.toRawJson());
        });
    }

    /// <summary>
    /// 监测监控软件的连接状态显示
    /// </summary>
    connect(NetworkEquipmentSubscriber::getInstance(), &NetworkEquipmentSubscriber::sig_SendNetMonitorState, this, [ = ](QString strIp)
    {
        if (m_strIp == strIp)
        {
            m_NetMonitorCalled = true;
        }
    });

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [ = ]
    {
        if (m_NetMonitorCalled)
        {
            m_NetMonitorCalled = false;
            ui->btn_NetState->setText(Tr("Connected"));
            ui->btn_NetState->setChecked(true);
            timer->stop();
            timer->start(2000);
        }
        else
        {
            ui->btn_NetState->setText(Tr("Disconnected"));
            ui->btn_NetState->setChecked(false);
        }
    });
    timer->setInterval(2000);
    timer->start();

}
