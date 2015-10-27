#include <time.h>
unsigned long millis() {
    return clock() / 1000; 
}
