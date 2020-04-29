#ifndef GyroQuartiles_h
#define GyroQuartiles_h

#include "Gyro.h"
#include "math/InterQuartiles.h"
#include "util/Timer.h"
#include "util/SlidingWindowList.h"

#define COLLECT_INTERVAL 20

class GyroQuartiles {
    public:
        GyroQuartiles():
            slidingWindowPitch(INTERQUARTILE_SIZE),
            slidingWindowRoll(INTERQUARTILE_SIZE) {}

        void setup(Gyro *gyro) {
            this->gyro = gyro;
            this->timer.start();
        }

        void loop() {
            if (!shouldCollect())
                return;

            float *ypr = gyro->getYPR();
            float pitch = ypr[1];
            float roll = ypr[2];

            if (!iqrPitch.isWithinRange(pitch) || !iqrRoll.isWithinRange(roll)) {
                return;
            }

            slidingWindowPitch.add(pitch);
            slidingWindowRoll.add(roll);

            if (slidingWindowPitch.isFull())
                calculateIqr();
        }

        bool hasStats() {
            return iqrPitch.hasStats() && iqrRoll.hasStats();
        }

        float getIqrPitch() {
            return iqrPitch.getIqr();
        }

        float getIqrRoll() {
            return iqrRoll.getIqr();
        }

        float getMinIqrPitch() {
            return iqrPitch.getMinAllowed();
        }
        float getMinIqrRoll() {
            return iqrRoll.getMinAllowed();
        }
        float getMaxIqrPitch() {
            return iqrPitch.getMaxAllowed();
        }
        float getMaxIqrRoll() {
            return iqrRoll.getMaxAllowed();
        }

    private:
        Gyro *gyro;
        SlidingWindowList slidingWindowPitch;
        SlidingWindowList slidingWindowRoll;
        InterQuartiles iqrPitch;
        InterQuartiles iqrRoll;
        Timer timer;

        bool shouldCollect() {
            if (timer.elapsedMs() >= COLLECT_INTERVAL) {
                timer.restart();
                return true;
            }

            return false;
        }

        void calculateIqr() {
            float pitchSamples[MAX_LIST_SIZE];
            float rollSamples[MAX_LIST_SIZE];
            slidingWindowPitch.copyToArray(pitchSamples);
            slidingWindowRoll.copyToArray(rollSamples);

            iqrPitch.clear();
            iqrRoll.clear();
            for (int i = 0; i < slidingWindowPitch.getSize(); i++) {
                iqrPitch.add(pitchSamples[i]);
                iqrRoll.add(rollSamples[i]);
            }

            iqrPitch.calculateIqr();
            iqrRoll.calculateIqr();
        }
};

#endif
