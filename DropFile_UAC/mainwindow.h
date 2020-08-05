#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <windows.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) ;

private:
    Ui::MainWindow *ui;

    BOOL ChangeWndMessageFilterOk(HWND hWnd, UINT nMessage, DWORD dwAction, PCHANGEFILTERSTRUCT chfit);

};
#endif // MAINWINDOW_H
