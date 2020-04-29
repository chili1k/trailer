#ifndef _INTERQUARTILE_H
#define _INTERQUARTILE_H

#include "Percentile.h"
#include "../Debug.h"

#define INTERQUARTILE_SIZE 16

class InterQuartiles {
    public:
        InterQuartiles():
            percentile25(INTERQUARTILE_SIZE, 0.25),
            percentile75(INTERQUARTILE_SIZE, 0.75),
            hasData(false) {
        }

        void add(float value) {
            if (!isWithinRange(value))
                return;

            percentile25.add(value);
            percentile75.add(value);

            if (percentile25.isFull()) {
                calculateIqr();
                hasData = true;
                clear();
            }
        }

        bool hasStats() {
            return hasData;
        }

        void clear() {
            percentile25.clear();
            percentile75.clear();
        }

        void calculateIqr() {
            float top = getTop();
            float bottom = getBottom();
            iqr = top - bottom;

            minAllowed = bottom - abs(1.5*bottom);
            maxAllowed = top + abs(1.5*top);
        }

        /**
         * calculateIqr must be called, before this is available.
         */
        float getIqr() {
            return iqr;
        }

        float getMinAllowed() {
            return minAllowed;
        }

        float getMaxAllowed() {
            return maxAllowed;
        }

        bool isWithinRange(float value) {
            if (!hasStats())
                return true;

//            return value >= minAllowed && value <= maxAllowed;
            return true;
        }
    private:
        Percentile percentile25;
        Percentile percentile75;
        float iqr;
        float minAllowed;
        float maxAllowed;
        bool hasData;

        float getBottom() {
            return percentile25.getPercentile();
        }

        float getTop() {
            return percentile75.getPercentile();
        }

};

#endif
