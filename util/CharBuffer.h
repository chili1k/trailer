#ifndef CharBuffer_h
#define CharBuffer_h

#define MAX_SIZE 500

/**
 *  Appends characters to a buffer until buffer is empty.
 */
class CharBuffer {
    public:
        CharBuffer() {
            reset();
        }

        void reset() {
            buffer[0] = '\0';
            size = 0;
        }

        /**
         * Returns true if it was added successfuly.
         */
        bool append(const char* src) {
            char *p = &buffer[size];
            while (*p) p++;

            // Don't add too many
            while ( (size < MAX_SIZE) && (*p++ = *src++) ) size++;

            // Safety
            if (size >= MAX_SIZE)
                buffer[MAX_SIZE-1] = '\0';

            return size <= MAX_SIZE;
        }

        bool canAppend(int srcSize) {
            return size + srcSize <= MAX_SIZE;
        }

        int getSize() {
            return size;
        }

        const char* getBuffer() {
            return buffer;
        }
    private:
        char buffer[MAX_SIZE];
        int size;
};

#endif
