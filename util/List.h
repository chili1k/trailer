/*
 * Simple linked list implementaiton with fixed size.
 */

#ifndef __LIST_H
#define __LIST_H

#define MAX_LIST_SIZE 20

template <class T>
class List {
    public:
        List(int maxSize) {
            this->maxSize = maxSize <= MAX_LIST_SIZE ? maxSize : MAX_LIST_SIZE;
            clear();
        }

        int getMaxSize() {
            return maxSize;
        }

        void copyToArray(T array[]) {
            ListItem *item = firstItem;
            for (int i = 0; i < size; i++) {
                array[i] = item->value;
                item = item->next;
            }
        }

        void clear() {
            this->size = 0;

            // set up initial linked list
            for (int i = 0; i < maxSize-1; i++)
                items[i].next = &items[i+1];

            firstItem = lastItem = &items[0];
        }

        void add(T value) {
            if (isFull())
                return;

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

        void removeValue(T value) {
            ListItem *item = firstItem;
            for (int i = 0; i < size; i++) {
                if (item->value == value) {
                    removePos(i);
                    return;
                }
            }
        }

        void removePos(int pos) {
            if (pos < 0 || pos >= size)
                return;

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

        int maxSize;
        int size;
        ListItem *firstItem, *lastItem;

        ListItem *getItem(int pos) {
            ListItem *item = firstItem;
            for (int i = 0; i < pos; i++)
                item = item->next;
            return item;
        }
};

#endif
