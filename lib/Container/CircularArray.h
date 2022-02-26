//
// Created by 夏侯臻 on 2022/2/26.
//

#ifndef CAMPUSCARDBACKEND_CIRCULARARRAY_H
#define CAMPUSCARDBACKEND_CIRCULARARRAY_H

/*
 * CircularArray,
 * item nullable if u haven't pushed any data yet,
 *
 * template must be pointer
 * */
template<typename ValueType>
class CircularArray {
    static_assert(std::is_pointer<ValueType>(), "ValueType must be a pointer");
public:
    typedef unsigned int Index;
    typedef unsigned int Size;

    Size size;
    /* current data index , always have value */
    Index current_index;
    Index start_index;

    inline explicit CircularArray(Size size, Index start = 0) : size(size),
                                                                start_index(start) {
        if (start >= size)
            throw;
        current_index = start;
        data = new ValueType[size];
    };

    [[nodiscard]] inline Size count() const {
        return start_index > current_index ? current_index + 1 + size - start_index : current_index - start_index;
    }

    inline ValueType operator[](Index subscript) const {
        return data[subscript];
    }

    inline const ValueType *operator()() const {
        return data;
    }

    inline ValueType top() const {
        return data[current_index];
    }

    inline ValueType bottom() const {
        return data[(start_index + 1) % size];
    }

    inline void push_back(ValueType value) {
        current_index = (current_index + 1) % size;
        if (current_index == start_index) {
            start_index = (start_index + 1) % size;
            delete data[start_index]; // reserve one place
        }
        data[current_index] = value;
    }

    /* Massive memory movement, ValueType must comparable */
    inline void insert(ValueType value) {

        if (*value > *top()) {
            current_index = (current_index + 1) % size;
            data[current_index] = value;
            if (current_index == start_index) {
                delete data[start_index];// reserve one place
                start_index = (start_index + 1) % size; // forward one step
            }
        } else if (*value < *bottom()) {
            data[start_index] = value;
            start_index = start_index == 0 ? size - 1 : start_index - 1;
            if (current_index == start_index) {
                delete data[current_index];// reserve one place
                current_index = current_index == 0 ? size - 1 : start_index - 1; // back one step
            }
        } else {
            // value ranged from bottom to top
            // add extra size used on circular search
            int left = start_index + 1, right = current_index + size, mid;

            //half search
            while (left <= right) {
                mid = (left + right) / 2;
                if (*(data[mid % size]) > *value) {
                    right = mid - 1;
                } else {
                    left = mid + 1;
                }
            }
            Index index = current_index + size;
            //movement
            while (index >= mid)
                std::swap(data[index % size], data[(--index) % size]);
            data[index % size] = value;

            current_index = (current_index + 1) % size;

            if (current_index == start_index) {
                delete data[start_index];// reserve one place
                start_index = (start_index + 1) % size; // forward one step
            }
        }
    }


    // delete pointer
    ~CircularArray() {
        for (int i = 0; i < size; ++i)
            delete data[i];
        delete[] data;
    }

private:
    ValueType *data;

public:
    const ValueType *begin() const { return data + current_index; }

    const ValueType *end() const { return data + size; }

    const ValueType * range(){

    }
};

#endif //CAMPUSCARDBACKEND_CIRCULARARRAY_H
