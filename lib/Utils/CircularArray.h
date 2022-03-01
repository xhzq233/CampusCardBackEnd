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
    typedef std::function<void(const unsigned int &, const ValueType &value)> Range;

    Size size;
    /* current data index , always have value */
    Index current_index;
    Index start_index;

    inline explicit CircularArray(Size size, Index start = 0) : size(size),
                                                                start_index(start) {
        if (start >= size)
            throw;
        current_index = start;
        data = new ValueType[size]{nullptr};
    };

    // delete copy methods
    CircularArray(const CircularArray &) = delete;

    // disable copied CircularArray
    CircularArray &operator=(const CircularArray &) = delete;

    [[nodiscard]] inline Size count() const {
        return start_index > current_index ? current_index + size - start_index : current_index - start_index;
    }

    inline ValueType &operator[](Index subscript) const {
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

    /**
     * return subscript meet the conditions
     *
     * for example, compare = 2
     * return the subscript of value which **last** less than 2
     * */
    template<class value_t>
    [[nodiscard]] inline Index halfSearch(value_t compare) {
        if (*top() < compare) {
            return (current_index + 1) % size;
        } else if (compare < *bottom()) {
            return start_index;
        } else {
            if (current_index < start_index) {
                int left = start_index + 1, right = current_index + size, mid;
                //half search
                while (left <= right) {
                    mid = (left + right) / 2;
                    if (compare < *data[mid % size]) {
                        right = mid - 1;
                    } else {
                        left = mid + 1;
                    }
                }
                return (left - 1) % size;
            } else {
                int left = start_index + 1, right = current_index, mid;
                //half search
                while (left <= right) {
                    mid = (left + right) / 2;
                    if (compare < *data[mid]) {
                        right = mid - 1;
                    } else {
                        left = mid + 1;
                    }
                }
                return left - 1;
            }
        }
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
        } else {// value ranged from bottom to top
            int mid;
            Index move_index;
            if (current_index < start_index) {// add extra size used on circular search
                int left = start_index + 1, right = current_index + size;
                //half search
                while (left <= right) {
                    mid = (left + right) / 2;
                    if (*(data[mid % size]) > *value) {
                        right = mid - 1;
                    } else {
                        left = mid + 1;
                    }
                }
                move_index = current_index + size + 1;
            } else {
                int left = start_index + 1, right = current_index;
                //half search
                while (left <= right) {
                    mid = (left + right) / 2;
                    if (*(data[mid]) > *value) {
                        right = mid - 1;
                    } else {
                        left = mid + 1;
                    }
                }
                move_index = current_index + 1;
            }

            //movement
            while (move_index > mid) {
                std::swap(data[move_index % size], data[(move_index - 1) % size]);
                move_index--;
            }

            data[move_index % size] = value;
            current_index = (current_index + 1) % size;
            if (current_index == start_index) {
                start_index = (start_index + 1) % size; // forward one step
                delete data[start_index];// reserve one place
            }
        }
    }

    // delete pointer
    ~CircularArray() {
        for_loop([](auto _, ValueType value) {
            delete value; //must add this, otherwise it can't be deleted correctly
        });
        delete[] data;
    }

private:
    ValueType *data{nullptr};
public:
    // only iterate none null values
    void for_loop(Range range) const {
        Index last;
        if (current_index < start_index) {
            last = current_index + size + 1;
            for (int i = (start_index + 1) % size; i < last; ++i) {
                range(i % size, data[i % size]);
            }
        } else {
            last = current_index + 1;
            for (int i = (start_index + 1) % size; i < last; ++i) {
                range(i, data[i]);
            }
        }
    }

    void for_loop(Index start, Index end, Range range) const {
        if (end < start) {
            Index last;
            last = end + size;
            for (unsigned int i = start; i < last; ++i) {
                range(i % size, data[i % size]);
            }
        } else {
            for (unsigned int i = start; i < end; ++i) {
                range(i, data[i]);
            }
        }
    }

};

#endif //CAMPUSCARDBACKEND_CIRCULARARRAY_H
