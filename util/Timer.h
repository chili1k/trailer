#ifndef Timer_h
#define Timer_h

class Timer {
    public:
        void start() {
            startTime = millis();
            isStarted = true;
        }

        void restart() {
            start();
        }

        void stop() {
            isStarted = false;
        }

        unsigned long elapsedMs() {
            return millis() - startTime;
        }
    private:
        bool isStarted = false;
        unsigned long startTime;
};

#endif
