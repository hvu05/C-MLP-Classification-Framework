/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   Tanh.cpp
 * Author: ltsach
 *
 * Created on September 1, 2024, 7:03 PM
 */

#include "layer/Tanh.h"

#include "ann/functions.h"
#include "sformat/fmt_lib.h"

Tanh::Tanh(string name) {
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "Tanh_" + to_string(++m_unLayer_idx);
}

Tanh::Tanh(const Tanh& orig) { m_sName = "Tanh_" + to_string(++m_unLayer_idx); }

Tanh::~Tanh() {}

xt::xarray<double> Tanh::forward(xt::xarray<double> X) {
  //cout << "tanh 1\n";
  // Todo CODE YOUR
  xt::xarray<double> e_pow_X = xt::exp(X);
  //cout << "tanh 2\n";
  xt::xarray<double> e_pow_mins_X = xt::exp(-X);
  //cout << "tanh 3\n";
  m_aCached_Y = (e_pow_X - e_pow_mins_X)/(e_pow_X + e_pow_mins_X);
  //cout << "tanh 4\n";
  return m_aCached_Y;

}
xt::xarray<double> Tanh::backward(xt::xarray<double> DY) {
  // Todo CODE YOUR
  //cout << "bbbtanh 1\n";
  xt::xarray<double> DX = DY * (1 - m_aCached_Y * m_aCached_Y);
  //cout << "bbbtanh 2\n";
  return DX;
}

string Tanh::get_desc() {
  string desc = fmt::format("{:<10s}, {:<15s}:", "Tanh", this->getname());
  return desc;
}
