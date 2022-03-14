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

    /* per_indexes represents each array beginning position */
    static void sort(ValueType *begin, const int *per_indexes, int merge_numer) {
        for (int merge_times = merge_numer / 2; merge_times > 0; merge_times /= 2) {
            auto factor = merge_numer / merge_times;
            // for example 16 -> 8 -> 4 -> 2 -> 1
            for (int j = 0; j < merge_times; ++j) {
                int left = per_indexes[j * factor];
                int sub = (int) ((j + 1.0 / 2) * factor);
                int mid = per_indexes[sub];
                int right = per_indexes[(j + 1) * factor] - 1;
                if (left >= right) continue;
                merge(begin, left, mid, right);
            }
        }
    }


    struct Compare {
        unsigned int position;// which array
        ValueType value;

        bool operator<(const Compare &rhs) const {
            return *value > *(rhs.value);
        }
    };

    /* per_indexes represents each array beginning position */
    static void priority_sort(ValueType *begin, const int *per_indexes, int merge_numer) {
        int per_indexes_copy[merge_numer];
        std::copy(per_indexes, per_indexes + merge_numer, per_indexes_copy);//no need the last one

        auto temp_container = new ValueType[per_indexes[merge_numer]];//re malloc a space
        std::priority_queue<Compare> queue;
        for (unsigned int each_array_position = 0; each_array_position < merge_numer; ++each_array_position) {
            queue.push(Compare{each_array_position, *(begin + per_indexes[each_array_position])});//initial values
        }

        auto temp_index = 0;// represents in subscript of the temp_container
        while (!queue.empty()) {//sort begin
            auto i = queue.top();
            queue.pop();
            temp_container[temp_index++] = i.value;// chose the smallest one
            if (per_indexes_copy[i.position] + 1 < per_indexes[i.position + 1])//if still have value
                queue.push({i.position, *(begin + (++per_indexes_copy[i.position]))});
        }

        //赋值回去
        std::copy(temp_container, temp_container + temp_index, begin);
        delete[] temp_container;//free
    }
};

#endif //CAMPUSCARDBACKEND_MERGESORT_H
