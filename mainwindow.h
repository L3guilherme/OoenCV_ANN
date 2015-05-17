#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
 #include <QTableWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btnIncANN_clicked();

    void on_spinBoxCam_valueChanged(int arg1);

    void on_spinBoxLins_valueChanged(int arg1);

    void on_spinBoxCols_valueChanged(int arg1);

    void on_spinBoxAms_valueChanged(int arg1);

    void on_tableWidgetCam_itemChanged(QTableWidgetItem *item);

    void on_pushButtonCarrDB_clicked();

    void on_pushButton_clicked();

    void on_pushButtonInic_clicked();

    void on_spinBoxCanais_valueChanged(int arg1);

    void on_pushButtonTeste_clicked();

    void on_tableWidgetCam_itemSelectionChanged();

    void on_pushButton_XIMEA_clicked();

    void on_pushButton_Parar_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
