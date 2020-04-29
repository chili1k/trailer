#ifndef _PERCENTILE_H
#define _PERCENTILE_H

#include "../util/List.h"

/**
 * Sliding window percentile.
 */
class Percentile {
    public:
        Percentile(unsigned int sampleSize, float percentile): bottom(sampleSize*percentile), top(sampleSize*(1-percentile)) { }

        void clear() {
            bottom.clear();
            top.clear();
        }

        bool isFull() {
            return bottom.isFull() && top.isFull();
        }

        void add(float value) {
            if (isFull())
                return;

            if (!bottom.isFull()) {
                addBottom(value);
            } else {
                if (value > maxBottom) {
                    addTop(value);
                } else {
                    float maxBottomNow = this->maxBottom;
                    float maxBottomPosNow = this->maxBottomPos;
                    addTop(maxBottomNow);
                    removeBottom(maxBottomPosNow);
                    addBottom(value);
                }
            }

        }

        float getPercentile() {
            return maxBottom;
        }

    private:
        List<float> bottom;
        List<float> top;
        float maxBottom;
        float maxTop;
        int maxBottomPos;

        void addBottom(float value) {
            if (bottom.isEmpty() || value > maxBottom) {
                maxBottom = value;
                maxBottomPos = bottom.getSize();
            }
            bottom.add(value);
        }

        void removeBottom(int pos) {
            bottom.removePos(pos);
            float arr[MAX_LIST_SIZE];
            bottom.copyToArray(arr);
            // Find new max
            for (int i = 0; i < bottom.getSize(); i++) {
                if (i == 0 || arr[i] > maxBottom) {
                    maxBottom = arr[i];
                    maxBottomPos = i;
                }
            }
        }

        void addTop(float value) {
            if (top.isEmpty() || value > maxTop)
                maxTop = value;
            top.add(value);
        }
};

#endif
