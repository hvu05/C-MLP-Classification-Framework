/*
 * File:   listheader.h
 * Author: ltsach
 *
 * Created on September 7, 2024, 10:51 PM
 */

#ifndef LISTHEADER_H
#define LISTHEADER_H

#include "list/DLinkedList.h"
#include "list/XArrayList.h"

template <class T>
using xvector = XArrayList<T>;

template <class T>
using xlist = DLinkedList<T>;

#endif /* LISTHEADER_H */
