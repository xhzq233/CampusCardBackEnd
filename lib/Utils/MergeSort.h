//
// Created by 夏侯臻 on 2022/2/27.
//

#ifndef CAMPUSCARDBACKEND_MERGESORT_H
#define CAMPUSCARDBACKEND_MERGESORT_H

#include "ThreadPool.h"

template<typename ValueType>
class MergeSort {
    static_assert(std::is_pointer<ValueType>(), "must be pointer");
private:
    /// 归并
    /// @param container 数组
    inline static void merge(ValueType *container, int left, int mid, int right) {
        auto temp_container = new ValueType[right - left + 1];
        int pointer1 = left, pointer2 = mid, temp_index = 0;

        while (pointer1 < mid && pointer2 <= right)
            temp_container[temp_index++] =
                    *container[pointer1] > *container[pointer2] ? container[pointer2++] : container[pointer1++];

        //将可能剩下元素的放回去
        while (pointer1 < mid)
            temp_container[temp_index++] = container[pointer1++];
        while (pointer2 <= right)
            temp_container[temp_index++] = container[pointer2++];

        //赋值回去
        std::copy(temp_container, temp_container + temp_index, container + left);
        delete[] temp_container;
    }

public:
    static void sort(ValueType *begin, const int *per_indexes, int merge_numer) {
        for (int merge_times = merge_numer / 2; merge_times > 0; merge_times /= 2) {
            auto factor = merge_numer / merge_times;
            // for example 16 -> 8 -> 4 -> 2 -> 1
            for (int j = 0; j < merge_times; ++j) {
                int left = per_indexes[j * factor];
                int sub = (int) ((j + 1.0 / 2) * factor);
                int mid = per_indexes[sub];
//                printf("times:%d,sub:%d, %d \n", merge_times, sub, mid);
                int right = per_indexes[(j + 1) * factor] - 1;
                if (left >= right) continue;
                merge(begin, left, mid, right);
            }
        }
    }
};

#endif //CAMPUSCARDBACKEND_MERGESORT_H
