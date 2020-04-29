/*
 * Simple linked list implementaiton with fixed size.
 */

#ifndef __LIST_H
#define __LIST_H

#define MAX_LIST_SIZE 20

template <class T>
class List {
    public:
        List(unsigned int maxSize) {
            this->maxSize = maxSize;
            this->size = 0;

            // set up initial linked list
            for (unsigned int i = 0; i < maxSize-1; i++)
                items[i].next = &items[i+1];

            firstItem = lastItem = &items[0];
        }

        T getMin() { return min; }
        T getMax() { return max; }

        void add(T value) {
            setMinMax(value);

            ListItem *item;
            if (isEmpty()) item = firstItem;
            else item = lastItem->next;

            item->value = value;
            lastItem = item;

            size++;
        }

        T get(int pos) {
            return getItem(pos)->value;
        }

        void remove(int pos) {
            if (size == 1) {
                // no need to do anything
            } else if (pos == 0) {
                lastItem->next = firstItem;
                firstItem = firstItem->next;
            } else {
                ListItem *previous;
                ListItem *item = firstItem;
                for (int i = 0; i < pos; i++) {
                    previous = item;
                    item = item->next;
                }

                previous->next = item->next;
                // last item
                if (pos == size-1)
                    lastItem = previous;
                lastItem->next = item;
            }

            size--;
            findMinMax();
        }

        bool isFull() {
            return size == maxSize;
        }

        bool isEmpty() {
            return size == 0;
        }

        int getSize() {
            return size;
        }

        int find(T value) {
            ListItem *item = firstItem;
            for (int i = 0; i < size; i++) {
                if (item->value == value) return i;
                item = item->next;
            }
            return -1;
        }

    private:
        struct ListItem {
            T value;
            ListItem *next;
        };

        ListItem items[MAX_LIST_SIZE];

        unsigned int maxSize;
        unsigned int size;
        T min, max;
        ListItem *firstItem, *lastItem;

        ListItem *getItem(int pos) {
            ListItem *item = firstItem;
            for (int i = 0; i < pos; i++)
                item = item->next;
            return item;
        }

        void setMinMax(T number) {
            if (isEmpty()) {
                min = max = number;
            } else if (number > max) {
                max = number;
            } else if (number < min) {
                min = number;
            }
        }

        void findMinMax() {
            ListItem *item = firstItem;
            T min, max;
            min = max = item->value;
            for (int i = 0; i < size; i++) {
                if (item->value < min) min = item->value;
                if (item->value > max) max = item->value;
                item = item->next;
            }
            this->min = min;
            this->max = max;
        }
};

#endif
