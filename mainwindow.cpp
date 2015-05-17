#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "string.h"

#include "opencv/cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/ml/ml.hpp"

#include"rna_ocv.h"

#include <QtCore/QTime>
#include <QMessageBox>

#include <XimeaCap.h>

RNA_OCV rede;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinBoxCanais->setValue(3);
    ui->tableWidgetCam->setRowCount(ui->spinBoxCam->text().toInt());
    ui->tableWidgetCam->setColumnCount(1);
    ui->tableWidgetCam->setItem(2,0,new QTableWidgetItem("2"));
    ui->tableWidgetCam->setItem(0,0,new QTableWidgetItem(QString::number(ui->spinBoxCols->text().toInt()*ui->spinBoxLins->text().toInt()*ui->spinBoxCanais->text().toInt())));
    ui->tableWidgetCam->setItem(1,0,new QTableWidgetItem("3"));


    ui->tableWidgetDB->setRowCount(ui->spinBoxAms->text().toInt());
    ui->tableWidgetDB->setColumnCount(2);

    ui->tableWidgetClasses->setRowCount(2);
    ui->tableWidgetClasses->setColumnCount(2);


}

MainWindow::~MainWindow()
{
    delete ui;
}

cv::Mat MatToLinha(cv::Mat matEnt);

void LerDados(std::vector<std::string> end,  cv::Mat &dados, cv::Mat &classes, std::vector<std::string> classesD, int totaldeAmos);

void LerDados(std::vector<std::string> end, cv::Mat &dados, cv::Mat &classes, std::vector<std::string> classesD, int totaldeAmos){


    for(int i = 0;i<totaldeAmos;i++){

        std::cout<<"End: "<<end[i]<<std::endl;
        cv::Mat img = cv::imread(end[i]);

        //if(img.channels()>1){
        //    cv::cvtColor(img,img,CV_BGR2GRAY);
        //}

        if(img.channels() == 1)
            if(img.type() != CV_32F){
                img.convertTo(img,CV_32F);
            }
        if(img.channels() == 3)
            if(img.type() != CV_32FC3){
                img.convertTo(img,CV_32FC3);
            }

        cv::Mat imgL = MatToLinha(img);
        dados.push_back(imgL);

        for(size_t c = 0;c<classesD.size();c++){

            //std::cout<<classesD[c]<<" = "<<endt.substr(endt.size()-4-classesD[c].size(),classesD[c].size())<<std::endl;
            if(classesD[c] == end[i].substr(end[i].size()-4-classesD[c].size(),classesD[c].size())){
                classes.at<float>(i,c) = 1.f;
            }
        }



        //std::cout<<"imgL "<<imgL<<std::endl;
    }
    std::cout<<"Classes "<<classes<<std::endl;
}

cv::Mat MatToLinha(cv::Mat matEnt){





    int nCanais = matEnt.channels();
    int step = matEnt.cols*nCanais;
    std::cout<<"Step= "<<step<<std::endl;
    std::cout<<"Canais= "<<nCanais<<std::endl;
    cv::Mat matSaida(1,matEnt.cols*matEnt.rows*nCanais,CV_32F);

    if(nCanais == 3){
        for(int i = 0; i<matEnt.rows;i++)
            for(int j = 0; j<matEnt.cols;j++){
                //std::cout<<"index: "<<i*step+nCanais*j<<std::endl;
                cv::Vec3f intensity =  matEnt.at<cv::Vec3f>(i, j);
                matSaida.at<float>(0,i*step+nCanais*j+0) = intensity.val[0];
                matSaida.at<float>(0,i*step+nCanais*j+1) = intensity.val[1];
                matSaida.at<float>(0,i*step+nCanais*j+2) = intensity.val[2];
            }
    }else{
        for(int i = 0; i<matEnt.rows;i++)
            for(int j = 0; j<matEnt.cols;j++){
                //std::cout<<"index: "<<i*matEnt.cols+j<<std::endl;
                matSaida.at<float>(0,i*step+j) = matEnt.at<float>(i,j);
            }
    }

    //std::cout<<"Entrada: "<<matEnt<<std::endl;
    //std::cout<<"Saida: "<<matSaida<<std::endl;

    return matSaida;
}

void MainWindow::on_btnIncANN_clicked()
{

    int nCols = ui->spinBoxCols->text().toInt();
    int nLins = ui->spinBoxLins->text().toInt();
    int nCanais = ui->spinBoxCanais->text().toInt();
    int nEnts = nCols*nLins*nCanais;
    int nCam = ui->spinBoxCam->text().toInt();
    int nAmts = ui->spinBoxAms->text().toInt();
    int nTest = ui->spinBoxTest->text().toInt();
    std::vector<std::string> qualif;
    std::vector<std::string> ends;

    for(int k = 0;k<ui->tableWidgetDB->rowCount();k++){
        qualif.push_back(ui->tableWidgetDB->item(k,1)->text().toStdString());
        ends.push_back(ui->tableWidgetDB->item(k,0)->text().toStdString());
    }

    std::cout<<qualif[0]<<std::endl;

    std::vector<std::string> classes;
    for(int l = 0;l<ui->tableWidgetClasses->rowCount();l++){
        classes.push_back(ui->tableWidgetClasses->item(l,0)->text().toStdString());
    }


    cv::Mat camadas(nCam,1,CV_32S);

    for(int i = 0;i<nCam;i++){
        camadas.at<int>(i,0) =ui->tableWidgetCam->item(i,0)->text().toInt();
    }

    rede.Iniciar(nEnts,camadas,nAmts);

    if(ui->spinBoxCanais->text().toInt() == 1)
        rede.CarregarTreino(ends,classes,nAmts,false);
    if(ui->spinBoxCanais->text().toInt() == 3)
        rede.CarregarTreino(ends,classes,nAmts,true);

    //int interacoes = redeN.train(dadosTreino, dadosTreinoClass,cv::Mat(),cv::Mat(),params);
    //std::cout<<"Fim do treinamento! N. Interações: "<<interacoes<<std::endl;
    if(ui->radioButtonMLP->isChecked()) rede.TreinarMLP();
    if(ui->radioButtonKNN->isChecked()) rede.TreinarKNN(ui->spinBoxTest->text().toInt());
    if(ui->radioButtonSVM->isChecked()) rede.TreinarSVM();
    //    QTime time;
    //    time.start();
    //    cv::Mat imgTeste = cv::imread("/home/lele/Desenvolvimento/Programas/Teste/FotosLatasREXAM/Lata10R.jpg");

    //    if(ui->radioButtonMLP->isChecked()){
    //        cv::Mat resultadoClass(1,camadas.at<int>(nCam-1,0),CV_32F);
    //        resultadoClass = rede.ProcessarMLP(imgTeste);
    //        std::cout<<"Resultado MLP: "<<resultadoClass<<std::endl;
    //    }
    //    if(ui->radioButtonKNN->isChecked()){
    //        float resultadoKNN = rede.ProcessarKNN(imgTeste,ui->spinBoxTest->text().toInt());
    //        std::cout<<"Resultado KNN: "<<resultadoKNN<<std::endl;
    //    }
    //    if(ui->radioButtonSVM->isChecked()){
    //        float resultadoSMV = rede.ProcessarSVM(imgTeste);
    //        std::cout<<"Resultado KNN: "<<resultadoSMV<<std::endl;
    //    }
    //    std::cout<<"Tempo: "<<time.restart()<<std::endl;

    QMessageBox::information(this, "", "FIM DO TREINAMENTO!");


}

void MainWindow::on_spinBoxCam_valueChanged(int arg1)
{
    ui->tableWidgetCam->setRowCount(ui->spinBoxCam->text().toInt());
    ui->tableWidgetCam->setColumnCount(1);
    ui->tableWidgetCam->setItem(0,ui->spinBoxCam->text().toInt()-1,new QTableWidgetItem("2"));
    ui->tableWidgetCam->setItem(0,0,new QTableWidgetItem(QString::number(ui->spinBoxCols->text().toInt()*ui->spinBoxLins->text().toInt()*ui->spinBoxCanais->text().toInt())));

}

void MainWindow::on_spinBoxLins_valueChanged(int arg1)
{
    ui->tableWidgetCam->setItem(0,0,new QTableWidgetItem(QString::number(ui->spinBoxCols->text().toInt()*ui->spinBoxLins->text().toInt()*ui->spinBoxCanais->text().toInt())));
}

void MainWindow::on_spinBoxCols_valueChanged(int arg1)
{
    ui->tableWidgetCam->setItem(0,0,new QTableWidgetItem(QString::number(ui->spinBoxCols->text().toInt()*ui->spinBoxLins->text().toInt()*ui->spinBoxCanais->text().toInt())));
}

void MainWindow::on_spinBoxAms_valueChanged(int arg1)
{
    ui->tableWidgetDB->setColumnCount(2);
    ui->tableWidgetDB->setRowCount(ui->spinBoxAms->text().toInt());
}

void MainWindow::on_tableWidgetCam_itemChanged(QTableWidgetItem *item)
{
    int s = ui->tableWidgetCam->item(ui->tableWidgetCam->rowCount()-1,0)->text().toInt();
    if(s == 1){
        ui->tableWidgetClasses->setRowCount(2);
    }else{
        ui->tableWidgetClasses->setRowCount(s);
    }
}

void MainWindow::on_pushButtonCarrDB_clicked()
{
    std::string end = ui->lineEditEnd->text().toStdString();
    int vIn = 0;
    int vFinal = 0;
    for(int i = 0;i<ui->tableWidgetClasses->rowCount();i++){
        vFinal += ui->tableWidgetClasses->item(i,1)->text().toInt();
        int cont = 0;
        for(int j = vIn; j<vFinal;j++){
            std::string endt = end+std::to_string(cont)+ui->tableWidgetClasses->item(i,0)->text().toStdString()+".jpg";
            ui->tableWidgetDB->setItem(j,0,new QTableWidgetItem(QString::fromStdString(endt)));
            ui->tableWidgetDB->setItem(j,1,new QTableWidgetItem(ui->tableWidgetClasses->item(i,0)->text()));
            cont++;
        }

        vIn += ui->tableWidgetClasses->item(i,1)->text().toInt();

    }
}

void MainWindow::on_pushButton_clicked()
{
    cv::Mat imgTeste = cv::imread(ui->lineEditEnd->text().toStdString());
    if(imgTeste.empty()){
        float valor[1];
        valor[0] = ui->lineEditEnd->text().toFloat();
        cv::Mat imgTesteTmp(1,1,CV_32FC1,valor);
        imgTesteTmp.copyTo(imgTeste);
    }
    cv::imshow("Entrada",imgTeste);

    QTime time;
    time.start();
    int can = ui->spinBoxCanais->text().toInt();

    if(can == 1){
        cv::cvtColor(imgTeste,imgTeste,CV_BGR2GRAY);
    }

    if(ui->radioButtonMLP->isChecked()){
        cv::Mat resultadoClass = rede.ProcessarMLP(imgTeste);
        std::cout<<"Resultado MLP: "<<resultadoClass<<std::endl;
        double min = -1.f;
        double max = -1.f;
        cv::Point min_loc, max_loc;
        cv::minMaxLoc(resultadoClass, &min, &max, &min_loc, &max_loc);
        std::cout<<"Tempo: "<<time.restart()<<std::endl;
        QString str = QString("Resultado MLP:  (%1,%2) = %3\n").arg(max_loc.x).arg(max_loc.y).arg(max);
        QMessageBox::information(this, "Resultado", str);
    }
    if(ui->radioButtonKNN->isChecked()){
        float resultadoKNN = rede.ProcessarKNN(imgTeste,ui->spinBoxTest->text().toInt());
        std::cout<<"Resultado KNN: "<<resultadoKNN<<std::endl;
        std::cout<<"Tempo: "<<time.restart()<<std::endl;
        QString str = QString("Resultado KNN:  %1\n").arg(resultadoKNN);
        QMessageBox::information(this, "Resultado", str);
    }
    if(ui->radioButtonSVM->isChecked()){
        float resultadoSMV = rede.ProcessarSVM(imgTeste);
        std::cout<<"Resultado SMV: "<<resultadoSMV<<std::endl;
        std::cout<<"Tempo: "<<time.restart()<<std::endl;
        QString str = QString("Resultado SMV:  %1\n").arg(resultadoSMV);
        QMessageBox::information(this, "Resultado", str);
    }






}

void MainWindow::on_pushButtonInic_clicked()
{
    int nCols = ui->spinBoxCols->text().toInt();
    int nLins = ui->spinBoxLins->text().toInt();
    int nCanais = ui->spinBoxCanais->text().toInt();
    int nEnts = nCols*nLins*nCanais;
    int nCam = ui->spinBoxCam->text().toInt();
    int nAmts = ui->spinBoxAms->text().toInt();


    cv::Mat camadas(nCam,1,CV_32S);

    for(int i = 0;i<nCam;i++){
        camadas.at<int>(i,0) =ui->tableWidgetCam->item(i,0)->text().toInt();
    }
    rede.Iniciar(nEnts,camadas,nAmts);

}

void MainWindow::on_spinBoxCanais_valueChanged(int arg1)
{
    ui->tableWidgetCam->setItem(0,0,new QTableWidgetItem(QString::number(ui->spinBoxCols->text().toInt()*ui->spinBoxLins->text().toInt()*ui->spinBoxCanais->text().toInt())));
}

void MainWindow::on_pushButtonTeste_clicked()
{

    int nCam = ui->spinBoxCam->text().toInt();
    int nAmts = ui->spinBoxAms->text().toInt();

    int x = ui->spinBoxTest->text().toInt();
    cv::Mat entrada(x*nAmts,1,CV_32FC1);
    cv::Mat saida(x*nAmts,1,CV_32FC1);

    cv::Mat camadas(nCam,1,CV_32S);

    for(int i = 0;i<nCam;i++){
        camadas.at<int>(i,0) =ui->tableWidgetCam->item(i,0)->text().toInt();
    }

    for(int k = 0; k<x;k++)
        for(int i = 0; i<nAmts;i++){
            entrada.at<float>(k*nAmts+i,0) = i;
            saida.at<float>(k*nAmts+i,0) = 2*(i*i);
        }

    rede.Iniciar(1,camadas,nAmts);

    rede.CarregarTreino(entrada,saida);

    if(ui->radioButtonMLP->isChecked()) rede.TreinarMLP();
    if(ui->radioButtonKNN->isChecked()) rede.TreinarKNN(ui->spinBoxTest->text().toInt());
    if(ui->radioButtonSVM->isChecked()) rede.TreinarSVM();




}

void MainWindow::on_tableWidgetCam_itemSelectionChanged()
{

}

bool parar = false;

void MainWindow::on_pushButton_XIMEA_clicked()
{
    parar = false;
    XimeaCap cap(1024,1024,90,0,true);

    for(;;){
        cv::Mat input = cap.getMat();
        if(!input.empty()){
            cv::Mat imgMenor;

            cv::cvtColor(input,input,cv::COLOR_BGR2RGB);

            cv::imshow("input",input);

            cv::resize(input,imgMenor,cv::Size(input.cols/2,input.rows/2),0,0,cv::INTER_LINEAR);

            QImage image = QImage((uint8_t*) imgMenor.data,imgMenor.cols,imgMenor.rows,imgMenor.step,QImage::Format_RGB888);

            QPixmap pixma = QPixmap::fromImage(image);

            ui->imgXIMEA->setPixmap(pixma);

            ui->imgXIMEA->setFixedSize(pixma.size());

            cv::waitKey(10);
            if (parar) break;

        }else{
            std::cout<<"Matriz Vazia!"<<std::endl;
        }
    }
    cap.StopXimea();
}


void MainWindow::on_pushButton_Parar_clicked()
{
    parar = true;
}
