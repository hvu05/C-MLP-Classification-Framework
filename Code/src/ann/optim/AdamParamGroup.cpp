/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   AdamParamGroup.cpp
 * Author: ltsach
 * 
 * Created on October 8, 2024, 1:43 PM
 */

#include "optim/AdamParamGroup.h"

AdamParamGroup::AdamParamGroup(double beta1, double beta2):
    m_beta1(beta1), m_beta2(beta2){
    //Create some maps:
    //cout << "Constructor of ADAMParamGroup\n";
    m_pParams = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pGrads = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pFirstMomment = new xmap<string, xt::xarray<double>*>(
            &stringHash,
            0.75,
            0,
            xmap<string, xt::xarray<double>*>::freeValue);
    m_pSecondMomment = new xmap<string, xt::xarray<double>*>(
            &stringHash,
            0.75,
            0,
            xmap<string, xt::xarray<double>*>::freeValue);
    //
    m_step_idx = 1;
    m_beta1_t = m_beta1;
    m_beta2_t = m_beta2;
}

AdamParamGroup::AdamParamGroup(const AdamParamGroup& orig):
    m_beta1(orig.m_beta1), m_beta2(orig.m_beta2){
    m_pParams = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pGrads = new xmap<string, xt::xarray<double>*>(&stringHash);
    m_pFirstMomment = new xmap<string, xt::xarray<double>*>(
            &stringHash,
            0.75,
            0,
            xmap<string, xt::xarray<double>*>::freeValue);
    m_pSecondMomment = new xmap<string, xt::xarray<double>*>(
            &stringHash,
            0.75,
            0,
            xmap<string, xt::xarray<double>*>::freeValue);
    //copy:
    *m_pParams = *orig.m_pParams;
    *m_pGrads = *orig.m_pGrads;
    *m_pFirstMomment = *orig.m_pFirstMomment;
    *m_pSecondMomment = *orig.m_pSecondMomment;
    //
    m_step_idx = 1;
    m_beta1_t = m_beta1;
    m_beta2_t = m_beta2;
}

AdamParamGroup::~AdamParamGroup() {
    if(m_pFirstMomment != nullptr) delete m_pFirstMomment;
    if(m_pSecondMomment != nullptr) delete m_pSecondMomment;
}
// void print_shape(const xt::xarray<double>& arr, string name) {
//     auto shape = arr.shape(); // Lấy shape của mảng
//     std::cout << "Shape of " << name <<" : (";
//     for (size_t i = 0; i < shape.size(); ++i) {
//         std::cout << shape[i]; // In kích thước từng chiều
//         if (i < shape.size() - 1) {
//             std::cout << ", "; // Thêm dấu phẩy nếu không phải phần tử cuối
//         }
//     }
//     std::cout << ")" << std::endl; // Kết thúc với dấu ngoặc
// }
void AdamParamGroup::register_param(string param_name, xt::xarray<double> *ptr_param, xt::xarray<double> *ptr_grad)
{
    // YOUR CODE IS HERE
    // cout << "in AdamParamGroup resgister_param: "; print_shape(*ptr_param, "ptr_param");
    // cout << "in resgister_param: "; print_shape(*ptr_grad, "ptr_grad");
    //cout << "register_param in AdamParamGroup\n";
    this->m_pParams->put(param_name, ptr_param);
    this->m_pGrads->put(param_name, ptr_grad);

    this->m_pFirstMomment->put(param_name, new double_tensor);
    this->m_pSecondMomment->put(param_name, new double_tensor);
}
void AdamParamGroup::register_sample_count(unsigned long long* pCounter){
    m_pCounter = pCounter;
}

void AdamParamGroup::zero_grad()
{
    // YOUR CODE IS HERE
    // cái này là lấy giống bên AdaParamGroup ...
    DLinkedList<string> keys = m_pGrads->keys();
    for (auto key : keys)
    {
        xt::xarray<double> *pGrad = m_pGrads->get(key);
        xt::xarray<double> *pFirstMomment = m_pFirstMomment->get(key);
        xt::xarray<double> *pSecondMomment = m_pSecondMomment->get(key);
        xt::xarray<double> *pParam = m_pParams->get(key);
        *pGrad = xt::zeros<double>(pParam->shape());
        *pFirstMomment = xt::zeros<double>(pParam->shape());
        *pSecondMomment = xt::zeros<double>(pParam->shape());
    }
    // reset sample_counter
    *m_pCounter = 0;
}

void AdamParamGroup::step(double lr)
{
    // YOUR CODE IS HERE
   // cout << "adamparamgroup\n";
    // UPDATE step_idx:
    m_step_idx += 1;
    m_beta1_t *= m_beta1;
    m_beta2_t *= m_beta2;
    DLinkedList<string> keys = m_pGrads->keys();
    for (auto key : keys)
    {
        // Lấy gradient, moment bậc nhất và bậc hai, và tham số hiện tại
        xt::xarray<double> &grad_P = *m_pGrads->get(key);
        xt::xarray<double> &first_moment = *m_pFirstMomment->get(key);
        xt::xarray<double> &second_moment = *m_pSecondMomment->get(key);
        xt::xarray<double> &P = *m_pParams->get(key);

        // Cập nhật moment bậc nhất và bậc hai
        first_moment = m_beta1 * first_moment + (1 - m_beta1) * grad_P;
        second_moment = m_beta2 * second_moment + (1 - m_beta2) * xt::pow(grad_P, 2);

        // Hiệu chỉnh bias cho moment
        xt::xarray<double> first_moment_kb = first_moment / (1 - m_beta1_t);
        xt::xarray<double> second_moment_kb = second_moment / (1 - m_beta2_t);

        // Cập nhật tham số
        P = P - lr * first_moment_kb / (xt::sqrt(second_moment_kb) + 1e-7);
    }
}
