#include <linux/linkage.h>
#include <linux/kernel.h>
#include <linux/timer.h>

asmlinkage long get_time(void) {
    static const long TENB = 1000000000;
    struct timespec t;
    getnstimeofday(&t);
    return t.tv_sec * TENB + t.tv_nsec;
}
