/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   Softmax.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"

#include <filesystem>  //require C++17

#include "ann/functions.h"
#include "sformat/fmt_lib.h"
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name) : m_nAxis(axis)
{
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax &orig) {}

Softmax::~Softmax() {}
// void print_shape_S(const xt::xarray<double>& arr, string name) {
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
// xt::xarray<double> Softmax::forward(xt::xarray<double> X)
// {
//   /////////////////////////////////////////////////////
//   /////////////// TỰ CHO BIẾN m_nAxis = -1
//   //m_nAxis = -1;
//   /////////////////////////////////////////////////////
//   ////////////////////////////////////////////////////

//   // Todo CODE YOUR
//   //cout << "sof 1\n";

//   auto shape = X.shape();
//   //cout << "sof 2\n";
//  m_nAxis = positive_index(m_nAxis, shape.size());

//   xt::xarray<double> Xmax = xt::amax(X, {m_nAxis});
  
//   //cout << "sof 5\n";
//   shape[m_nAxis] = 1;
//   //cout << "sof 6\n";
//   Xmax = xt::expand_dims(Xmax, m_nAxis);
//   Xmax = xt::broadcast(Xmax, shape);
 

//   X = xt::exp(X - Xmax);

//   //cout << "sof 8\n";
//   xt::xarray<double> SX = xt::sum(X, {m_nAxis});

//   //cout << "sof 9\n";
//   SX = xt::expand_dims(SX, m_nAxis);
//   SX = xt::broadcast(SX, shape);

//   X = X / SX;

//   //cout << "sof 11\n";
//   m_aCached_Y = X;
//   //cout << "sof 12\n";
//   return m_aCached_Y;
// }
xt::xarray<double> Softmax::forward(xt::xarray<double> X)
{
  xt::xarray<double> Y = xt::zeros_like(X);
  for(size_t i = 0; i < X.shape()[0]; ++i)
  {
    xt::xarray<double> temp = xt::view(X, i);
    xt::view(Y, i) = softmax(temp, -1);
  }
  m_aCached_Y = Y;
  return Y;
}
xt::xarray<double> Softmax::backward(xt::xarray<double> DY)
{
    // Khởi tạo một mảng trống để bắt đầu ghép
    xt::xarray<double> DZ = xt::zeros<double>(DY.shape());
    for (size_t i = 0; i < DY.shape()[0]; ++i)
    {
        xt::xarray<double> y = xt::view(m_aCached_Y, i);
        xt::xarray<double> dy = xt::view(DY, i);

        xt::xarray<double> tensor_diag = xt::diag(y);
        auto outer_y = xt::linalg::outer(y, y);
        auto jacob = tensor_diag - outer_y;
        auto res = xt::linalg::dot(jacob, dy);

        xt::view(DZ, i) = res;
    }
    return DZ;
}


// xt::xarray<double> Softmax::backward(xt::xarray<double> DY)
// {
//   //cout << "bbbsof 1\n";
//   cout << "In Softmax bw, input "; print_shape_S(DY, "DY");
//   xt::xarray<double> tensor_diag = xt::diag(m_aCached_Y);
//   //cout << "bbbsof 2\n";
//   print_shape_S(m_aCached_Y, "m_aCached_Y");
//   auto pro = xt::linalg::outer(m_aCached_Y, m_aCached_Y);
//   //cout << "bbbsof 3\n";
//   print_shape_S(pro, "outer m_aCached_Y");
//   xt::xarray<double> ja = tensor_diag - pro;
//   //cout << "bbbsof 4\n";
//   xt::xarray<double> res = xt::linalg::dot(ja, DY);
//   //cout << "bbbsof 5\n";
//   return res;
// }

string Softmax::get_desc() {
  string desc = fmt::format("{:<10s}, {:<15s}: {:4d}", "Softmax",
                            this->getname(), m_nAxis);
  return desc;
}
