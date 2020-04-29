#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void print(int pid, long start_time, long end_time) {
    static const long TENB = 1000000000;
    printk("[Project1] %d %ld.%09ld %ld.%09ld\n", pid, start_time / TENB, start_time % TENB, end_time / TENB, end_time % TENB);
}
