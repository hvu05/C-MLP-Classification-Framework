/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   ReLU.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:44 PM
 */

#include "layer/ReLU.h"

#include "ann/functions.h"
#include "sformat/fmt_lib.h"

ReLU::ReLU(string name) {
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "ReLU_" + to_string(++m_unLayer_idx);
}

ReLU::ReLU(const ReLU& orig) { m_sName = "ReLU_" + to_string(++m_unLayer_idx); }

ReLU::~ReLU() {}

xt::xarray<double> ReLU::forward(xt::xarray<double> X) {
  // Todo CODE YOUR
  //cout << "relu 1\n";
  this->m_aMask = X >= 0;
  //cout << "relu 2\n";
  xt::xarray<double> res = xt::where(m_aMask, X, 0.0);
  //cout << "relu 3\n";
  return res;
}
xt::xarray<double> ReLU::backward(xt::xarray<double> DY) {
  // Todo CODE YOUR
  //cout << "bbbrelu 1\n";
  
  xt::xarray<double> res = xt::where(m_aMask, DY, 0.0);
  //cout << "bbbrelu 2\n";
  return res;
}

string ReLU::get_desc() {
  string desc = fmt::format("{:<10s}, {:<15s}:", "ReLU", this->getname());
  return desc;
}