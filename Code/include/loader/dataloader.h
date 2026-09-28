/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   dataloader.h
 * Author: ltsach
 *
 * Created on September 2, 2024, 4:01 PM
 */

#ifndef DATALOADER_H
#define DATALOADER_H
#include "tensor/xtensor_lib.h"
#include "loader/dataset.h"

using namespace std;

template<typename DType, typename LType>
class DataLoader{
public:
    class Iterator; //forward declaration for class Iterator
    
private:
    Dataset<DType, LType>* ptr_dataset;
    int batch_size;
    bool shuffle;
    bool drop_last;
    int nbatch;
    ulong_tensor item_indices;
    int m_seed;

    xt::xarray<Batch<DType, LType> *> ba;
    int len;
    xt::xarray<int> save_data_label;

    
public:
    DataLoader(Dataset<DType, LType> *ptr_dataset, int batch_size, bool shuffle = true, bool drop_last = false, int seed = -1)
        : ptr_dataset(ptr_dataset), batch_size(batch_size), shuffle(shuffle), m_seed(seed)
    {
        nbatch = ptr_dataset->len() / batch_size;
        item_indices = xt::arange(0, ptr_dataset->len());

        // TODO implement
        this->ptr_dataset = ptr_dataset;
        this->batch_size = batch_size;
        this->shuffle = shuffle;
        this->drop_last = drop_last;

        TensorDataset<DType, LType> *ptr = dynamic_cast<TensorDataset<DType, LType> *>(ptr_dataset);
        xt::xarray<DType> t = ptr->get_data();
        xt::xarray<LType> l = ptr->get_label();
        save_data_label = xt::arange<int>(ptr->len());

        this->len = ptr->len();
        int rest = len % batch_size;
        int n = len / batch_size; // tạo ra n = len / batch_size

        if (shuffle)
        {
            if (seed >= 0)
            {
                xt::random::seed(seed);
                xt::random::shuffle(save_data_label);
            }
            else
            {
                xt::random::shuffle(save_data_label);
            }
        }
        int start = 0;
        int end = 0;

        ba = xt::xarray<Batch<DType, LType> *>({(size_t)n + 2});
        for (int i = 0; i < n + 2; ++i)
        {
            ba(i) = nullptr;
        }

        if (n == 0 and drop_last == 0) ba(0) = nullptr;
        else if (n == 0 && drop_last == 1) ba(0) = nullptr;
        else if (rest == 0)
        {
            for (int i = 0; i < n; ++i)
            {
                start = i * batch_size;
                end = start + batch_size - 1;

                int size_sub_array = end - start + 1;
                xt::xarray<int> sub_arr = xt::empty<int>({size_sub_array});
                int idx = 0;
                for (int j = start; j <= end; ++j)
                {
                    sub_arr(idx) = save_data_label(j);
                    idx++;
                }
                // Lấy sub_data và sub_label theo các chỉ số đã xáo trộn
                xt::xarray<DType> sub_data = xt::view(t, xt::keep(sub_arr), xt::all(), xt::all());
                xt::xarray<LType> sub_label;
                if (l.size() == 1)
                    sub_label = l;
                else
                    sub_label = xt::view(l, xt::keep(sub_arr), xt::all(), xt::all());

                // Tạo batch mới với dữ liệu và nhãn tương ứng
                Batch<DType, LType> *batch_new = new Batch(sub_data, sub_label);
                // ba.add(batch_new);
                ba(i) = batch_new;
            }
        }
        else
        {
            if (drop_last == true)
            {
                // không gộp mà bỏ luôn phần dư
                for (int i = 0; i < n; ++i)
                {
                    if (i == n)
                    {
                        start = len - rest;
                        end = len - 1;
                    }
                    else
                    {
                        start = i * batch_size;
                        end = start + batch_size - 1;
                    }
                    int size_sub_array = end - start + 1;
                    xt::xarray<int> sub_arr = xt::empty<int>({size_sub_array});
                    int idx = 0;
                    for (int j = start; j <= end; ++j)
                    {
                        sub_arr(idx) = save_data_label(j);
                        idx++;
                    }
                    // Lấy sub_data và sub_label theo các chỉ số đã xáo trộn
                    xt::xarray<DType> sub_data = xt::view(t, xt::keep(sub_arr), xt::all(), xt::all());
                    xt::xarray<LType> sub_label;
                    if (l.size() == 1)
                        sub_label = l;
                    else
                        sub_label = xt::view(l, xt::keep(sub_arr), xt::all(), xt::all());

                    // Tạo batch mới với dữ liệu và nhãn tương ứng
                    Batch<DType, LType> *batch_new = new Batch(sub_data, sub_label);
                    // ba.add(batch_new);
                    ba(i) = batch_new;
                }
            }
            else
            {
                // batch cuối cùng có rest phần tử gộp vào
                for (int i = 0; i < n; ++i)
                {
                    if (i == n - 1)
                    {
                        start = i * batch_size;
                        end = len - 1;
                    }
                    else
                    {
                        start = i * batch_size;
                        end = start + batch_size - 1;
                    }
                    int size_sub_array = end - start + 1;
                    xt::xarray<int> sub_arr = xt::empty<int>({size_sub_array});

                    int idx = 0;
                    for (int j = start; j <= end; ++j)
                    {
                        sub_arr(idx) = save_data_label(j);
                        idx++;
                    }
                    // Lấy sub_data và sub_label theo các chỉ số đã xáo trộn
                    xt::xarray<DType> sub_data = xt::view(t, xt::keep(sub_arr), xt::all(), xt::all());
                    xt::xarray<LType> sub_label;

                    if (l.size() == 1)
                        sub_label = l;
                    else
                        sub_label = xt::view(l, xt::keep(sub_arr), xt::all(), xt::all());

                    // Tạo batch mới với dữ liệu và nhãn tương ứng
                    Batch<DType, LType> *batch_new = new Batch(sub_data, sub_label);
                    // ba.add(batch_new);
                    ba(i) = batch_new;
                }
            }
        }
    }
    virtual ~DataLoader()
    {
        // TODO implement
        for (int i = 0; i < ba.size(); ++i)
        {
            // if (ba[i] != NULL)
            //   delete ba[i];
            if (ba(i) != NULL)
                delete ba(i);
        }
    }

    //New method: from V2: begin
    int get_batch_size(){ return batch_size; }
    int get_sample_count(){ return ptr_dataset->len(); }
    int get_total_batch(){return int(ptr_dataset->len()/batch_size); }
    
    //New method: from V2: end
    /////////////////////////////////////////////////////////////////////////
    // The section for supporting the iteration and for-each to DataLoader //
    /// START: Section                                                     //
    /////////////////////////////////////////////////////////////////////////
 // TODO implement forech
  class Iterator
  {
  public:
    // TODO implement contructor
    Iterator(const xt::xarray<Batch<DType, LType>*> &batchs, size_t index_batch = 0)
    {
      this->batchs = batchs;
      this->index_batch = index_batch;
    }

    Iterator &operator=(const Iterator &iterator)
    {
      // TODO implement
      this->batchs = iterator.batchs;
      return *this;
    }

    Iterator &operator++()
    {
      index_batch++;
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator iterator = *this;
      ++*this;
      return iterator;
    }

    bool operator!=(const Iterator &other) const
    {
      // TODO implement
      return index_batch != other.index_batch;
    }

    Batch<DType, LType> operator*()
    {
      // TODO implement
      return *(batchs(index_batch));
    }

  private:
    xt::xarray<Batch<DType, LType>*> batchs;
    size_t index_batch;
  };

  Iterator begin()
  {
    return Iterator(this->ba, 0);
  }

  Iterator end()
  {
    //return Iterator(this->ba, ba.size());

    for(int i = 0; i < this->len / batch_size + 2; ++i)
    {
      if(ba(i) == nullptr) return Iterator(this->ba, i);
    }
    return Iterator(this->ba, 0);

    // size_t n = this->len / batch_size;
    // return Iterator(this->ba, n);
  }

};


#endif /* DATALOADER_H */

