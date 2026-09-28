/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   Optimizer.cpp
 * Author: ltsach
 * 
 * Created on September 2, 2024, 10:51 AM
 */

#include "optim/IOptimizer.h"
#include "list/DLinkedList.h"
#include <string>
using namespace std;

IOptimizer::IOptimizer(double learning_rate):
m_fLearningRate(learning_rate){
    m_pGroupMap = new xmap<string, IParamGroup*>(&stringHash,
            0.75,
            nullptr,
            &xmap<string, IParamGroup*>::freeValue);
}

IOptimizer::IOptimizer(const IOptimizer& orig):
m_fLearningRate(orig.m_fLearningRate){
}

IOptimizer::~IOptimizer() {
    if(m_pGroupMap != nullptr) delete m_pGroupMap;
}

void IOptimizer::step(){
    //cout << "12.1\n";
    DLinkedList<string> gkeys = m_pGroupMap->keys();
    //cout << "size of gkeys in IOptimizer::step: " << gkeys.size() << endl;
    //cout << "12.2\n";
    for(auto key: gkeys){
    //cout << "12.3\n";
        IParamGroup* pGroup = m_pGroupMap->get(key);
    //cout << "12.4\n";
        pGroup->step(m_fLearningRate);
    //cout << "12.5\n";
    }
    //cout << "12.6\n";

}
void IOptimizer::zero_grad(){
    DLinkedList<string> keys = m_pGroupMap->keys();
    for(auto key: keys){
        IParamGroup* pGroup = m_pGroupMap->get(key);
        pGroup->zero_grad();
    }
};