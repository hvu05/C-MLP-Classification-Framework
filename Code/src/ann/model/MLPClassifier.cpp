/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Model.cpp
 * Author: ltsach
 * 
 * Created on September 1, 2024, 5:09 PM
 */

#include "model/MLPClassifier.h"
#include "optim/IParamGroup.h"
#include "tensor/xtensor_lib.h"
#include "sformat/fmt_lib.h"
#include <filesystem> //require C++17
namespace fs = std::filesystem;
#include <fstream>
#include <sstream>
#include "ann/functions.h"
#include "layer/FCLayer.h"
#include "layer/ReLU.h"
#include "layer/Sigmoid.h"
#include "layer/Tanh.h"
#include "layer/Softmax.h"
#include "metrics/ClassMetrics.h"




//Constructors and Destructors
MLPClassifier::MLPClassifier(string cfg_filename, string sModelName):
    IModel(cfg_filename, sModelName){
}

MLPClassifier::MLPClassifier(
    string cfg_filename, string sModelName,
    ILayer** seq, int size): 
    IModel(cfg_filename, sModelName){
    //layer to m_layers:
    //cout << "size of COnstructor in MLPClassifier: " << size << endl;
    for(int idx=0; idx < size; idx++) m_layers.add(seq[idx]);
}

MLPClassifier::MLPClassifier(const MLPClassifier& orig):
    IModel(orig.m_cfg_filename, orig.m_sModelName){
    //copy list (in the assignment operator of DLinkedList)
    m_layers = orig.m_layers; 
}

MLPClassifier::~MLPClassifier() {
    for(auto ptr_layer: m_layers) delete ptr_layer;
}

//for the inference mode: begin
double_tensor MLPClassifier::predict(double_tensor X, bool make_decision){
    //SWITCH to inference mode
    bool old_mode = this->m_trainable;
    this->set_working_mode(false);
    
    //DO the inference
    
    //YOUR CODE IS HERE
    //// BEGIN YOUR CODE ////////////
    double_tensor Y = forward(X);
    //// END YOUR CODE ///////////
    
    //RESTORE the previous mode
    this->set_working_mode(old_mode);
    
    //RETURN
    if(make_decision) return Y;
    else return xt::argmax(Y, -1);
}

double_tensor MLPClassifier::predict(DataLoader<double, double>* pLoader, bool make_decision){
    bool old_mode = this->m_trainable;
    this->set_working_mode(false);
    
    double_tensor results = 0;
    bool first_batch = true;
    
    cout << "Prediction: Started" << endl;
    string info = fmt::format("{:<6s}/{:<12s}|{:<50s}\n",
                    "Batch", "Total Batch", "Num. of samples processed");
    cout << info;
    
    int total_batch = pLoader->get_total_batch(); 
    int batch_idx = 1;
    unsigned long long nsamples = 0;
    for(auto batch: *pLoader){
        //YOUR CODE IS HERE
       double_tensor datas_batch = batch.getData();
       //double_tensor labels_batch = batch.getLabel();
        double_tensor output = forward(datas_batch);
        if(first_batch == true)  results = output;
        else results += output;
    }
    cout << "Prediction: End" << endl;
    
    //restore the old mode
    this->set_working_mode(old_mode);
    
    if(make_decision) return results;
    else return xt::argmax(results, -1);
}


double_tensor MLPClassifier::evaluate(DataLoader<double, double>* pLoader){
    //cout << "15.1 ";
    bool old_mode = this->m_trainable;
    //cout << "15.2 \n";
    this->set_working_mode(false);
    //cout << "size of m_layer: " << m_layers.size() << endl;
    //cout << "15.3 ";
    ClassMetrics meter(this->get_num_classes());
    //cout << "15.4 ";
    meter.reset_metrics();
    //cout << "15.5 ";
    double_tensor metrics;
    //YOUR CODE IS HERE
    //cout << "15.6 ";
    for(auto batch : *pLoader)
    {
    //cout << "15.7 ";
        double_tensor datas_batch = batch.getData();
    //cout << "15.8 ";
        double_tensor Y = forward(datas_batch);

    //cout << "15.9 ";
        double_tensor Y_TRUE = xt::argmax(batch.getLabel(), 1);
    //cout << "15.10 ";
        double_tensor Y_PREDICT = xt::argmax(Y, 1);
        
    //cout << "15.11 ";
        meter.accumulate(Y_TRUE, Y_PREDICT);
       // metrics += meter.accumulate(Y_TRUE, Y_PREDICT);
    //cout << "15.12 ";
    }
    metrics = meter.get_metrics();
    //cout << "15.13 ";
    this->set_working_mode(old_mode);
    //cout << "15.14 ";
    return metrics;
}
//for the inference mode:end



//for the training mode:begin
void MLPClassifier::compile(IOptimizer* pOptimizer, ILossLayer* pLossLayer, IMetrics* pMetricLayer){
    this->m_pOptimizer = pOptimizer;
    this->m_pLossLayer = pLossLayer;
    this->m_pMetricLayer = pMetricLayer;
    
    for(auto pLayer: m_layers){
        if(pLayer->has_learnable_param()){
            string name = pLayer->getname();
            IParamGroup* pGroup = pOptimizer->create_group(name);
            pLayer->register_params(pGroup);
        }
    }
}
    
void MLPClassifier::set_working_mode(bool trainable){
    m_trainable = trainable;
    for(auto pLayer: m_layers){
        pLayer->set_working_mode(trainable);
    }
}
void print_shape_MLP(const xt::xarray<double>& arr, string name) {
    auto shape = arr.shape(); // Lấy shape của mảng
    //std::cout << "Shape of " << name <<" : (";
    for (size_t i = 0; i < shape.size(); ++i) {
        std::cout << shape[i]; // In kích thước từng chiều
        if (i < shape.size() - 1) {
            std::cout << ", "; // Thêm dấu phẩy nếu không phải phần tử cuối
        }
    }
    std::cout << ")  "; // Kết thúc với dấu ngoặc
}
//protected: for the training mode: begin
double_tensor MLPClassifier::forward(double_tensor X){
    //YOUR CODE IS HERE
    print_shape_MLP(X, "shape of input: ");
    double_tensor Y = X;
    //cout << "9.1 ";
    //cout << "m_layer in MLPCLassifier: " << m_layers.size() << endl;
    // int i = 0;
    for(auto lay : m_layers)
    {
       // cout << "-------------------------------" << Y << "------------------------------" << endl;
        //cout << "9.2 ";
        Y = lay->forward(Y);
        //cout << "9.3 ";
        break;
    }
    return Y;
}
void MLPClassifier::backward(){
    //YOUR CODE IS HERE
    //cout << "11.1 ";
    xt::xarray<double> dY = this->m_pLossLayer->backward();
    //cout << dY << endl;
    //cout << "11.2 ";
    for(auto pLayer = m_layers.bbegin(); pLayer != m_layers.bend(); pLayer++)
    {
        //cout << "11.3 ";
        dY = (*pLayer)->backward(dY);
         //cout << "after---------- " ;
    }
    // phải gán denta_Y vô đâu nữa chứ ...
}
//protected: for the training mode: end


/*
 * save(string base_path):
 *  + base_path: 
 
 */
bool MLPClassifier::save(string model_path){
    try{
        //prepare folder and file names
        model_path = trim(model_path);
        if(fs::exists(model_path)){
            //USE the specified path
            fs::remove_all(model_path); //remove all files related
        }
        else{
            //USE the DEFAULT path
            model_path = m_pConfig->get_new_checkpoint(this->m_sModelName);

        }
        cout << model_path << ": creation" << endl;
        fs::create_directories(model_path);
        string arch_file =  fs::path(model_path)/
                            fs::path(m_pConfig->get("arch_file", "arch.txt"));

        //open stream
        ofstream datastream(arch_file);
        if(!datastream.is_open()){
            cerr << arch_file << ": couldn't open for writing" << endl;
            throw "Model architecture file '" + arch_file + "': can not open for writing.";
        }
        //write header
        //write data
        datastream << "model name: " << this->m_sModelName << endl;
        for(auto pLayer: m_layers){
            string desc = pLayer->get_desc();
            datastream << desc << endl;
            pLayer->save(model_path);
        }

        //close stream
        datastream.close();
        return true;
    }
    catch(exception& e){
        string message = fmt::format("MLPClassifier::save: failed; model_path={:s}",
                model_path);
        cerr << message << endl;
        cerr << e.what() << endl;
        return false;
    }
}

bool MLPClassifier::load(string model_path,  bool use_name_in_file){
    try{
        //verify the existing of model_path
        //cout << "14.1\n";
        if(!fs::exists(model_path)){
        //cout << "14.2\n";

            string message = fmt::format("{:s}: not exist.", model_path); 
            cerr << message << endl;
            return false;
        }

        //open a stream for the architecture file
        //cout << "14.3\n";
        string arch_file = model_path + "/" + "arch.txt";

        //cout << "===================================\nArch file name: " << arch_file << "=================================" <<endl;
        //cout << "14.4\n";
        ifstream datastream(arch_file);
        //cout << "14.5\n";
        if(!datastream.is_open()){
        //cout << "14.7\n";
            string message = fmt::format("{:s}: can not open for reading.", arch_file); 
            cerr << message << endl;
            return false;
        }
        //read header: to be here

        //read and parse lines
        //cout << "14.8\n";
        string line;
        while(getline(datastream, line)){
            //skip empty and comment line (started with #)
        //cout << "14.9\n";
            line = trim(line);
            if(line.size() == 0) continue;
            if(line[0] == '#') continue;

            //parse line
            char delimiter=':';
            istringstream linestream(line);
            string first, second;
            getline(linestream, first, delimiter); //first: maybe an empty string
            getline(linestream, second, delimiter); //second: maybe an empty string

            delimiter=',';
            istringstream partstream(first);
            string layer_type, layer_name;
            getline(partstream, layer_type, delimiter); //type: maybe an empty string
            getline(partstream, layer_name, delimiter); //name: maybe an empty string
            layer_type = trim(layer_type);
            layer_name = trim(layer_name);

            //create layers according to the layer type
            string new_name;
            if(use_name_in_file) new_name = layer_name;
            else new_name = "";

        //cout << "14.10\n";
            if(layer_type.compare("FC") == 0){
        //cout << "14.11\n";
                string w_file = model_path + "/" + layer_name + "_W.npy";
                string b_file = model_path + "/" + layer_name + "_b.npy";
                //note:: b_file: may not be used in FCLayer
        //cout << "14.11.1\n";
                 m_layers.add(new FCLayer(trim(second), w_file, b_file, new_name));
        //cout << "14.11.2\n";
            }
            if(layer_type.compare("ReLU") == 0){
        ///cout << "14.12\n";
                m_layers.add(new ReLU(new_name) );
            }
            if(layer_type.compare("Sigmoid") == 0){
        //cout << "14.13\n";
                m_layers.add(new Sigmoid(new_name) );
            }
            if(layer_type.compare("Tanh") == 0){
        //cout << "14.14\n";
                m_layers.add(new Tanh(new_name) );
            }
            if(layer_type.compare("Softmax") == 0){
        //cout << "14.15\n";
                int nAxis;
                try{
        //cout << "14.16\n";
                    nAxis = stoi(trim(second));
                }
                catch(std::invalid_argument& e){
        //cout << "14.17\n";
                    string message_1 = fmt::format("Can not read axis of Softmax from: {:s}", trim(second));
                    string message_2 = "Use 'axis=-1' instead.";
                    cerr << message_1 << endl;
                    cout << message_2 << endl;
                    nAxis = -1; 
                }
                m_layers.add(new Softmax(nAxis, new_name) );
            }
        }
        
        //close stream
        datastream.close();
        return true;
    }
    catch(exception& e){
        cerr << "In MLPClassifier::load(.,.):" << endl;
        cout << e.what() << endl;
        return false;
    }
    return true;
}

