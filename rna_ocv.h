#ifndef RNA_OCV_H
#define RNA_OCV_H

#include "opencv/cv.h"
#include "opencv2/opencv.hpp"
#include "opencv2/ml/ml.hpp"


class RNA_OCV
{
public:
    RNA_OCV();
    ~RNA_OCV();
    void Iniciar(int nEntradas, cv::Mat camadas, int nAmostras);
    void CarregarTreino(std::vector<std::string> end, std::vector<std::string> classesD, int totaldeAmos, bool cCor = true);
    void CarregarTeste(std::vector<std::string> end,  std::vector<std::string> classesD, int totaldeAmos);
    void CarregarTreino(cv::Mat entradas, cv::Mat saidas);
    cv::Mat ProcessarMLP(cv::Mat entrada);
    void TreinarMLP(int maxInt = 1000, float mudancaMin = 0.000001);
    void TreinarKNN(int k);
    float ProcessarKNN(cv::Mat entrada, int k);
    void TreinarSVM(int maxInt = 1000, float mudancaMin = 0.000001);
    float ProcessarSVM(cv::Mat entrada);
private:
    void LerDados(std::vector<std::string> end,  cv::Mat &dados, cv::Mat &classes, std::vector<std::string> classesD, int totaldeAmos, bool cCor = true);
    cv::Mat matsTreino;
    cv::Mat matsTeste;
    cv::Mat matsClassTreino;
    cv::Mat matsClassTeste;
    CvANN_MLP redeMLP;
    CvKNearest redeKNN;
    cv::Mat matsClassKNNTreino;
    CvSVM redeSVM;
};

#endif // RNA_OCV_H
