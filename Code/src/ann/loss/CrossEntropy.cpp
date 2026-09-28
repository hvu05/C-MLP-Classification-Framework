/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   CrossEntropy.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:47 PM
 */

#include "loss/CrossEntropy.h"

#include "ann/functions.h"

CrossEntropy::CrossEntropy(LossReduction reduction) : ILossLayer(reduction) {}

CrossEntropy::CrossEntropy(const CrossEntropy& orig) : ILossLayer(orig) {}

CrossEntropy::~CrossEntropy() {}
// void print_shape_CR(const xt::xarray<double>& arr, string name) {
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
double CrossEntropy::forward(xt::xarray<double> X, xt::xarray<double> t)
{
  // Todo CODE YOUR

  m_aYtarget = t;
  m_aCached_Ypred = X;
  // double epsilon = 1e-7;
  // int N_norm = X.shape(0);
  // if (m_eReduction == REDUCE_SUM) N_norm = 1;
  // else if (m_eReduction == REDUCE_MEAN) N_norm = t.shape()[0];
  // double loss = 0;
  // for (size_t i = 0; i < X.shape()[0]; ++i)
  // {
  //   auto x = xt::view(X, i);
  //   auto t_t = xt::view(t, i);

  //   loss += xt::linalg::dot(t_t, xt::log(x))();
  // }
  // loss = -loss / N_norm;
  // return loss;
  return cross_entropy(X, t, m_eReduction);
}
xt::xarray<double> CrossEntropy::backward()
{
  // Todo CODE YOUR
  double epsilon = 1e-7;
  int N_norm = m_aCached_Ypred.shape(0);

  if (m_eReduction == REDUCE_SUM) N_norm = 1;
  else if (m_eReduction == REDUCE_MEAN) N_norm = m_aCached_Ypred.shape()[0];
  //cout << "cross 2\n";
  //cout << "cross 3\n";
  xt::xarray<double> temp = N_norm * (m_aCached_Ypred + epsilon);
  //cout << "cross 4\n";
  xt::xarray<double> DY = -(m_aYtarget/temp);

  return DY;
}