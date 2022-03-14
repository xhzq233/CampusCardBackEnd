//
// Created by 夏侯臻 on 2022/2/26.
//

#ifndef CAMPUSCARDBACKEND_CIRCULARARRAY_H
#define CAMPUSCARDBACKEND_CIRCULARARRAY_H

typedef unsigned int Subscript;

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
    typedef unsigned int Size;
    typedef std::function<void(const unsigned int &index, const ValueType &value)> Range;

    Size size;
    /* current data index , always have value */
    Subscript current_index;
    Subscript start_index;

    inline explicit CircularArray(Size size, Subscript start = 0) : size(size),
                                                                    start_index(start) {
        if (start >= size)
            throw;
        current_index = start;
        _data = new ValueType[size];
    };

    // delete copy methods
    CircularArray(const CircularArray &) = delete;

    // disable copied CircularArray
    CircularArray &operator=(const CircularArray &) = delete;

    [[nodiscard]] inline Size count() const {
        return start_index > current_index ? current_index + size - start_index : current_index - start_index;
    }

    [[nodiscard]] inline ValueType &operator[](Subscript subscript) const {
        return _data[subscript];
    }

    [[nodiscard]] inline const ValueType *operator()() const {
        return _data;
    }

    [[nodiscard]] inline ValueType top() const {
        return _data[current_index];
    }

    [[nodiscard]] inline ValueType bottom() const {
        return _data[(start_index + 1) % size];
    }

    inline void push_back(ValueType value) {
        current_index = (current_index + 1) % size;
        if (current_index == start_index) {
            start_index = (start_index + 1) % size;
            delete _data[start_index]; // reserve one place
        }
        _data[current_index] = value;
    }

    /// return subscript meet that condition
    /// for example, compare = 3, array is 0, 1, 2, 2, 3
    /// return 3 whose subscript of value **last** less than 3
    template<class value_t>
    [[nodiscard]] inline Subscript halfSearch(value_t compare) const {
        if (*top() < compare) {
            return current_index;
        } else if (compare < *bottom()) {
            return start_index;
        } else {
            if (current_index < start_index) {
                int left = start_index + 1, right = current_index + size, mid;
                //half search
                while (left <= right) {
                    mid = (left + right) / 2;
                    if (compare < *_data[mid % size]) {
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
                    if (compare < *_data[mid]) {
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
            _data[current_index] = value;
            if (current_index == start_index) {
                start_index = (start_index + 1) % size; // forward one step
                delete _data[start_index];// reserve one place
            }
        } else if (*value < *bottom()) {
            _data[start_index] = value;
            start_index = start_index == 0 ? size - 1 : start_index - 1;
            if (current_index == start_index) {
                delete _data[current_index];// reserve one place
                current_index = current_index == 0 ? size - 1 : start_index - 1; // back one step
            }
        } else {// value ranged from bottom to top
            int mid;
            Subscript move_index;
            if (current_index < start_index) {// add extra size used on circular search
                int left = start_index + 1, right = current_index + size;
                //half search
                while (left <= right) {
                    mid = (left + right) / 2;
                    if (*(_data[mid % size]) > *value) {
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
                    if (*(_data[mid]) > *value) {
                        right = mid - 1;
                    } else {
                        left = mid + 1;
                    }
                }
                move_index = current_index + 1;
            }

            //movement
            while (move_index > mid) {
                std::swap(_data[move_index % size], _data[(move_index - 1) % size]);
                move_index--;
            }

            _data[move_index % size] = value;
            current_index = (current_index + 1) % size;
            if (current_index == start_index) {
                start_index = (start_index + 1) % size; // forward one step
                delete _data[start_index];// reserve one place
            }
        }
    }

    // delete pointer
    ~CircularArray() {
        for_loop([](auto _, auto value) {
            delete value; //must add this, otherwise it can't be deleted correctly
        });
        delete[] _data;
    }

private:
    ValueType *_data{nullptr};
public:
    // only iterate none null values
    void for_loop(const Range &range) const {
        Subscript last;
        if (current_index < start_index) {
            last = current_index + size + 1;
            for (int i = start_index + 1; i < last; ++i) {
                range(i % size, _data[i % size]);
            }
        } else {
            last = current_index + 1;
            for (int i = (start_index + 1) % size; i < last; ++i) {
                range(i, _data[i]);
            }
        }
    }

    void for_loop(const Subscript start, const Subscript end, const Range &range) const {
        if (end < start) {
            Subscript last = end + size + 1;
            for (unsigned int i = start + 1; i < last; ++i) {
                range(i % size, _data[i % size]);
            }
        } else {
            for (unsigned int i = (start + 1) % size; i < end + 1; ++i) {
                range(i, _data[i]);
            }
        }
    }

};

#endif //CAMPUSCARDBACKEND_CIRCULARARRAY_H
