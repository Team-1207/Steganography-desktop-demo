#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <bmp_stegano_master.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_select_file_clicked();

    void on_btn_write_clicked();

private:
    Ui::MainWindow *ui;

    void m_update_img();
    QString m_file_path;
    BmpSteganoMaster m_stegano_master;
};
#endif // MAINWINDOW_H
