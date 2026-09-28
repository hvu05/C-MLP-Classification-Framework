/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   SGDParamGroup.cpp
 * Author: ltsach
 * 
 * Created on October 7, 2024, 9:45 PM
 */

#include "optim/SGDParamGroup.h"


// void print_shape(const xt::xarray<double>& arr, string name) {
//     auto shape = arr.shape(); // Lấy shape của mảng
//     std::cout << "Shape of " << name <<" : (";
//     for (size_t i = 0; i < shape.size(); ++i) {
//         std::cout << shape[i]; // In kích thước từng chiều
//         if (i < shape.size() - 1) {
//             std::cout << ", "; // Thêm dấu phẩy nếu không phải phần tử cuối
//         }
//     }
//     std::cout << ")  "; // Kết thúc với dấu ngoặc
// }
SGDParamGroup::SGDParamGroup() {
    m_pParams = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pGrads = new xmap<string, xt::xarray<double>*>(&stringHash);


    // tự viết thêm để test
    //cout << "size of m_pParams " << m_pParams->size() << endl;
    //cout << "size of m_pGrands " << m_pGrads->size() << endl;
    DLinkedList<string> keys = m_pGrads->keys();
    int i = 1;
    for(auto key: keys){
        xt::xarray<double>& P = *m_pParams->get(key);
        xt::xarray<double>& grad_P = *m_pGrads->get(key);
        // cout << "Implement at index: " << i++ << endl;
        // print_shape(P, "P");
        // print_shape(grad_P, "grad_P");
    }
    // END tự viết thêm để test

}

SGDParamGroup::SGDParamGroup(const SGDParamGroup& orig) {
}

SGDParamGroup::~SGDParamGroup() {
}

void SGDParamGroup::register_param(string param_name, xt::xarray<double>* ptr_param, xt::xarray<double>* ptr_grad){
    
    
    //cout << "in SGDParamGroup resgister_param: "; print_shape(*ptr_param, "ptr_param");
    //cout << "in resgister_param: "; print_shape(*ptr_grad, "ptr_grad");


    m_pParams->put(param_name, ptr_param);
    m_pGrads->put(param_name, ptr_grad);
}
void SGDParamGroup::register_sample_count(unsigned long long* pCounter){
    m_pCounter = pCounter;
}
void SGDParamGroup::zero_grad(){
    DLinkedList<string> keys = m_pGrads->keys();
    for(auto key: keys){
        xt::xarray<double>* pGrad = m_pGrads->get(key);
        xt::xarray<double>* pParam = m_pParams->get(key);
        *pGrad = xt::zeros<double>(pParam->shape());
    }
    //reset sample_counter
    *m_pCounter = 0;
}

void SGDParamGroup::step(double lr){
    //cout << "SGDParamGroup\n";
    DLinkedList<string> keys = m_pGrads->keys();
    // for(auto key: keys){
    //     xt::xarray<double>& P = *m_pParams->get(key);
    //     xt::xarray<double>& grad_P = *m_pGrads->get(key);
    //     print_shape(P, "P");
    //     print_shape(grad_P, "grad_P");
    //     P = P - lr*grad_P;

       
    // }
    int i = 1;
    //cout << "size of keys(LinkList) in SGDParamGroup::step: " << keys.size() << endl;
    for(auto key: keys){
        xt::xarray<double>& P = *m_pParams->get(key);
        xt::xarray<double>& grad_P = *m_pGrads->get(key);
        //cout << "Implement at index: " << i++ << endl;
        //print_shape(P, "P");
        //print_shape(grad_P, "grad_P");

        P = P - lr*grad_P;
    }
    // tự thêm
    /////////begin
    //zero_grad();
}
