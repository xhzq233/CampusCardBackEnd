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
        int pointer1 = left, pointer2 = mid + 1, temp_index = 0;

        while (pointer1 <= mid && pointer2 <= right)
            temp_container[temp_index++] =
                    *container[pointer1] > *container[pointer2] ? container[pointer2++] : container[pointer1++];

        //将可能剩下元素的放回去
        while (pointer1 <= mid)
            temp_container[temp_index++] = container[pointer1++];
        while (pointer2 <= right)
            temp_container[temp_index++] = container[pointer2++];

        //赋值回去
        std::copy(temp_container, temp_container + temp_index, container + left);

        delete[] temp_container;
    }

    static void separate(ValueType *container, int begin, int end) {
        if (begin >= end) {
            return;
        }
        int mid = (begin + end) / 2;
        separate(container, begin, mid);
        separate(container, mid + 1, end);
        merge(container, begin, mid, end);
    }

    static constexpr const char WORK_NUMBER = 16;

public:
    static void sort(ValueType *begin, int count) {
        int len = count / WORK_NUMBER;
        int per_index[WORK_NUMBER + 1]{0};
        for (int i = 1; i < WORK_NUMBER; ++i) {
            per_index[i] = per_index[i - 1] + len;
        }
        per_index[WORK_NUMBER] = count;
        std::function<void(void)> tasks[WORK_NUMBER];
        for (int i = 0; i < WORK_NUMBER; ++i) {
            tasks[i] = [begin = begin, per_index = per_index, index = i]() {
                separate(begin, per_index[index], per_index[index + 1] - 1);
            };
        }
        JoinableMultiWork works(Device::MAX_THREAD - 1, WORK_NUMBER, tasks);
        //all thread joined
        for (int merge_times = WORK_NUMBER; merge_times > 0; merge_times /= 2) {
            auto factor = WORK_NUMBER / merge_times;
            // for example 16 -> 8 -> 4 -> 2 -> 1
            for (int j = 0; j < merge_times; ++j) {
                int left = per_index[j * factor];
                int right = per_index[(j + 1) * factor] - 1;
                merge(begin, left, (left + right) / 2, right);
            }
        }
    }

};

#endif //CAMPUSCARDBACKEND_MERGESORT_H
