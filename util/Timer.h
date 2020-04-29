#ifndef Timer_h
#define Timer_h

class Timer {
    public:
        void start() {
            startTime = millis();
            _isStarted = true;
        }

        void restart() {
            start();
        }

        void stop() {
            _isStarted = false;
        }

        bool isStarted() {
            return _isStarted;
        }

        unsigned long elapsedMs() {
            return millis() - startTime;
        }
    private:
        bool _isStarted = false;
        unsigned long startTime;
};

#endif
