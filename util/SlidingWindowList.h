#ifndef SlidingWindowList_h
#define SlidingWindowList_h

#include "List.h"

class SlidingWindowList {
    public:
        SlidingWindowList(int size): list(size) {}

        void add(float value) {
            if (list.isFull())
                list.removePos(0);

            list.add(value);
        }

        void copyToArray(float array[]) {
            list.copyToArray(array);
        }

        bool isFull() {
            return list.isFull();
        }

        int getSize() {
            return list.getSize();
        }
    private:
        List<float> list;
};

#endif
