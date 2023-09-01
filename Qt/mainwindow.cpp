#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto scene = new QGraphicsScene();// ui->gv_img->scene();
    qDebug() << "[MainWindow] scene: " << ui->gv_img->scene();
    ui->gv_img->setScene(scene);
    qDebug() << "[MainWindow] scene: " << ui->gv_img->scene();

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::m_update_img() {
    auto img = QImage(m_file_path);
    auto scene = ui->gv_img->scene();
    scene->clear();
    auto w = ui->gv_img->geometry().width();
    auto h = ui->gv_img->geometry().height();
    auto cx = ui->gv_img->geometry().center().x();
    auto cy = ui->gv_img->geometry().center().y();
    qDebug() << w << ":" << h << "; " << cx << cy;
    QGraphicsPixmapItem* spm = scene->addPixmap(QPixmap::fromImage(img).scaled(w - 5, h - 5, Qt::KeepAspectRatio));
    spm->setPos(ui->gv_img->geometry().center());
    spm->update();
    scene->update();
}

void MainWindow::on_btn_select_file_clicked() {
    ui->lbl_progress->setText("Выполняется чтение файла..");
    ui->te_hidden_text->clear();
    m_file_path = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "/home",
                                                     tr("Images (*.png *.bmp *.jpg)"));
    m_update_img();
    m_stegano_master.set_img(m_file_path);
    ui->te_hidden_text->setPlainText(m_stegano_master.read_hidden_text());
    ui->lbl_file_path->setText(QString("Путь к текщему файлу:\n") + m_file_path);
    ui->lbl_progress->setText("Файл прочитан");
}



void MainWindow::on_btn_write_clicked() {
    ui->lbl_progress->setText("Выполняется запись файла..");
    m_stegano_master.write_hidden_text(ui->te_hidden_text->toPlainText(),
                                       (ui->chb_alpha->checkState() == Qt::Checked));
    QString m_file_path = QFileDialog::getSaveFileName(this, tr("Save File"),
                                "/tmp/img_with_secret.png",
                                tr("png (*.png)"));

    m_stegano_master.save_img(m_file_path);
    m_update_img();
    ui->lbl_progress->setText("Файл записан");
    ui->lbl_file_path->setText(m_file_path);
}
