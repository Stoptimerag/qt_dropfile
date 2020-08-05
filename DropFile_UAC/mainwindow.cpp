#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    HWND hwnd = (HWND)this->winId();
    CHANGEFILTERSTRUCT chfit1 = { sizeof(CHANGEFILTERSTRUCT) };
    CHANGEFILTERSTRUCT chfit2 = { sizeof(CHANGEFILTERSTRUCT) };
    ChangeWndMessageFilterOk(hwnd, WM_DROPFILES, MSGFLT_ALLOW, &chfit1);
    ChangeWndMessageFilterOk(hwnd, 0x0049, MSGFLT_ALLOW, &chfit2);

    RevokeDragDrop((HWND)this->winId());
    DragAcceptFiles(hwnd, TRUE);

}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if(eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
    {
        MSG* pMsg = reinterpret_cast<MSG*>(message);

        if(pMsg->message == WM_DROPFILES)
        {
            HDROP hDropInfo = (HDROP)pMsg->wParam;

            wchar_t szFilePathName[_MAX_PATH] = {0};

            const UINT nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

            for (UINT nIndex=0 ; nIndex< nNumOfFiles; ++nIndex)
            {
                DragQueryFile(hDropInfo, nIndex, szFilePathName, _MAX_PATH);
                const QString currentfile = QString::fromWCharArray(szFilePathName);
                qDebug()<<"drop file = "<<currentfile;
                ui->label->setText(currentfile);
            }
            DragFinish(hDropInfo);
        }
    }
    return false;
}

BOOL MainWindow::ChangeWndMessageFilterOk(HWND hWnd, UINT nMessage, DWORD dwAction, PCHANGEFILTERSTRUCT chfit)
{
    typedef BOOL (WINAPI *ChangeWindowMessageFilterOkFn)(HWND, UINT, DWORD, PCHANGEFILTERSTRUCT);
    HMODULE hModUser32 = NULL;
    hModUser32 = LoadLibrary(L"user32.dll");
    if (hModUser32 == NULL)
    {
        qDebug()<<"load failed";
        return FALSE;
    }

    ChangeWindowMessageFilterOkFn pfnChangeWindowMessageFilter = (ChangeWindowMessageFilterOkFn) GetProcAddress(hModUser32, "ChangeWindowMessageFilterEx");
    if (pfnChangeWindowMessageFilter == NULL)
    {
        FreeLibrary(hModUser32);
        qDebug()<<"failed";
        return FALSE;
    }

    FreeLibrary(hModUser32);
    BOOL ret = pfnChangeWindowMessageFilter(hWnd, nMessage, dwAction, chfit);
    const QString strOut = QString("%1 ret = [%2] ExtStatus=[%3]\n").arg(__FUNCTION__, QString::number(ret), QString::number(chfit->ExtStatus));
    qDebug()<<strOut;
    return ret;
}


