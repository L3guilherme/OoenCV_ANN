#include "rna_ocv.h"

RNA_OCV::RNA_OCV()
{

}

RNA_OCV::~RNA_OCV()
{

}

void RNA_OCV::Iniciar(int nEntradas, cv::Mat camadas,int nAmostras){
    matsTreino.create(0,nEntradas,CV_32FC1);

    matsClassTreino = cv::Mat::zeros(nAmostras, camadas.at<int>(camadas.rows-1,0), CV_32FC1);

    matsClassTreino = matsClassTreino-1;
    //std::cout<<"matsClassTreino "<<matsClassTreino<<std::endl;

    matsClassKNNTreino = cv::Mat::zeros(nAmostras,1,CV_32FC1);
    matsClassKNNTreino = matsClassKNNTreino - 1;


    std::cout<<"camadas"<<camadas<<std::endl;

    try{
        redeMLP.create(camadas, CvANN_MLP::SIGMOID_SYM);
    }catch(cv::Exception e){
        std::cout<<e.err<<std::endl;
    }

}

void RNA_OCV::LerDados(std::vector<std::string> end, cv::Mat &dados, cv::Mat &classes, std::vector<std::string> classesD, int totaldeAmos,bool cCor){


    for(int i = 0;i<totaldeAmos;i++){

        std::cout<<"End: "<<end[i]<<std::endl;
        cv::Mat img = cv::imread(end[i]);

        if(!cCor && img.channels()>1){
            cv::cvtColor(img,img,CV_BGR2GRAY);
        }

        if(img.channels() == 1)
            if(img.type() != CV_32FC1){
                img.convertTo(img,CV_32FC1);
            }
        if(img.channels() == 3)
            if(img.type() != CV_32FC3){
                img.convertTo(img,CV_32FC3);
            }


        cv::Mat imgL = img.reshape(1,1);//MatToLinha(img);
        imgL = imgL/255.f;
        dados.push_back(imgL);

        for(size_t c = 0;c<classesD.size();c++){

            //std::cout<<classesD[c]<<" = "<<endt.substr(endt.size()-4-classesD[c].size(),classesD[c].size())<<std::endl;
            if(classesD[c] == end[i].substr(end[i].size()-4-classesD[c].size(),classesD[c].size())){
                cv::Mat camadas = redeMLP.get_layer_sizes();
                if(camadas.at<int>(0,camadas.cols-1) == 1){
                    if(c==0)
                        classes.at<float>(i,0) = (float)1.f;
                    if(c==1)
                        classes.at<float>(i,0) = (float)-1.f;
                }else{
                    classes.at<float>(i,c) = 1.f;
                }


                if(c==0)
                    matsClassKNNTreino.at<float>(i,0) = (float)1.f;
                if(c==1)
                    matsClassKNNTreino.at<float>(i,0) = (float)-1.f;
            }
        }



        //std::cout<<"imgL "<<imgL<<std::endl;
    }
    std::cout<<"Classes "<<classes<<std::endl;
}

cv::Mat RNA_OCV::ProcessarMLP(cv::Mat entrada){

    if(entrada.rows>1){
        cv::Mat entTmp;
        entTmp = entrada.reshape(1,1);
        entTmp.copyTo(entrada);
    }

    if(entrada.type() != CV_32FC1){
        entrada.convertTo(entrada,CV_32FC1);
    }
    entrada = entrada/255.f;

    cv::Mat saida;
    redeMLP.predict(entrada,saida);
    return saida;
}

void RNA_OCV::CarregarTreino(std::vector<std::string> end, std::vector<std::string> classesD, int totaldeAmos,bool cCor){
    LerDados(end,matsTreino,matsClassTreino,classesD,totaldeAmos,cCor);
}

void RNA_OCV::CarregarTeste(std::vector<std::string> end, std::vector<std::string> classesD, int totaldeAmos){
    LerDados(end,matsTeste,matsClassTeste,classesD,totaldeAmos);
}

void RNA_OCV::TreinarMLP(int maxInt,float mudancaMin){
    CvANN_MLP_TrainParams params;
    CvTermCriteria criteria;
    criteria.max_iter = maxInt;
    criteria.epsilon = mudancaMin;
    criteria.type = CV_TERMCRIT_ITER | CV_TERMCRIT_EPS;
    params.train_method = CvANN_MLP_TrainParams::BACKPROP;
    params.bp_dw_scale = 0.01f;
    params.bp_moment_scale = 0.01f;
    params.term_crit = criteria;

    std::cout<<"Treinando..."<<std::endl;
    cv::Mat cam = redeMLP.get_layer_sizes();
    std::cout<<"camadas= "<<cam<<std::endl;
    std::cout<<"cols Treino= "<<matsTreino.cols<<std::endl;
    std::cout<<"rows Treino= "<<matsTreino.rows<<std::endl;
    std::cout<<"cols Saida= "<<matsClassTreino.cols<<std::endl;
    std::cout<<"rows Saida= "<<matsClassTreino.rows<<std::endl;
    std::cout<<"tipo Treino= "<<matsTreino.type()<<std::endl;;
    std::cout<<"tipo Saida= "<<matsClassTreino.type()<<std::endl;
    //std::cout<<"Matrix de Entradas:"<<std::endl;
    //std::cout<<matsTreino<<std::endl;
    //std::cout<<"Matrix de Saidas:"<<std::endl;
    //std::cout<<matsClassTreino<<std::endl;
    int interacoes = redeMLP.train(matsTreino, matsClassTreino,cv::Mat(),cv::Mat(),params);
    std::cout<<"Fim do treinamento! N. Interações: "<<interacoes<<std::endl;
}

void RNA_OCV::TreinarKNN(int k){
    std::cout<<"Saida KNN"<<matsClassKNNTreino<<std::endl;
    std::cout<<"tipo Treino= "<<matsTreino.type()<<std::endl;;
    std::cout<<"tipo Saida= "<<matsClassKNNTreino.type()<<std::endl;
    //std::cout<<"Matrix de Entradas:"<<std::endl;
    //std::cout<<matsTreino<<std::endl;
    //std::cout<<"Matrix de Saidas:"<<std::endl;
    //std::cout<<matsClassKNNTreino<<std::endl;
    std::cout<<"Treinando KNN..."<<std::endl;
    redeKNN.train(matsTreino,matsClassKNNTreino,cv::Mat(),false,k);
    std::cout<<"Fim Treino KNN!!!"<<std::endl;
}

float RNA_OCV::ProcessarKNN(cv::Mat entrada,int k){
    if(entrada.rows>1){
        cv::Mat entTmp;
        entTmp = entrada.reshape(1,1);
        entTmp.copyTo(entrada);
    }

    if(entrada.type() != CV_32FC1){
        entrada.convertTo(entrada,CV_32FC1);
    }

    entrada = entrada/255.f;

    float saida = redeKNN.find_nearest(entrada,k);
    return saida;
}

void RNA_OCV::TreinarSVM(int maxInt,float mudancaMin){
    CvSVMParams params;
    params.svm_type    = CvSVM::C_SVC;
    params.kernel_type = CvSVM::RBF;
    params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, maxInt, mudancaMin);
    std::cout<<"tipo Treino= "<<matsTreino.type()<<std::endl;;
    std::cout<<"tipo Saida= "<<matsClassKNNTreino.type()<<std::endl;
    //std::cout<<"Matrix de Entradas:"<<std::endl;
    //std::cout<<matsTreino<<std::endl;
    //std::cout<<"Matrix de Saidas:"<<std::endl;
    //std::cout<<matsClassKNNTreino<<std::endl;
    std::cout<<"Treinando SVM..."<<std::endl;
    redeSVM.train(matsTreino,matsClassKNNTreino,cv::Mat(),cv::Mat(),params);
    std::cout<<"Fim Treino SVM!!!"<<std::endl;
}

float RNA_OCV::ProcessarSVM(cv::Mat entrada){
    if(entrada.rows>1){
        cv::Mat entTmp;
        entTmp = entrada.reshape(1,1);
        entTmp.copyTo(entrada);
    }
    if(entrada.type() != CV_32FC1){
        entrada.convertTo(entrada,CV_32FC1);
    }

    entrada = entrada/255.f;

    float saida = redeSVM.predict(entrada);
    return saida;
}

void RNA_OCV::CarregarTreino(cv::Mat entradas, cv::Mat saidas){
    entradas.copyTo(matsTreino);
    saidas.copyTo(matsClassTreino);
    saidas.copyTo(matsClassKNNTreino);
}

