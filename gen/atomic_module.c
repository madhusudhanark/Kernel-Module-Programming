/*
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/atomic.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h> 
typedef struct {
    atomic_t ref_count;
    int data;
} shared_resource;

shared_resource *res;

static struct task_struct *thread1;
static struct task_struct *thread2;

void acquire_resource(shared_resource *r) {
    atomic_inc(&r->ref_count);
    pr_info("Resource acquired. Ref count = %d\n", atomic_read(&r->ref_count));
}

void release_resource(shared_resource *r) {
    if (atomic_dec_and_test(&r->ref_count)) {
        pr_info("Ref count is zero. Freeing resource with value = %d\n", r->data);
        kfree(r);
    } else {
        pr_info("Resource released. Ref count = %d\n", atomic_read(&r->ref_count));
    }
}

int thread_fn(void *arg) {
    shared_resource *r = (shared_resource *)arg;
    acquire_resource(r);
    ssleep(1);  // Simulate work
    release_resource(r);
    return 0;
}

static int __init refcount_module_init(void) {
    pr_info("Reference counting kernel module loaded.\n");

    res = kmalloc(sizeof(shared_resource), GFP_KERNEL);
    if (!res) return -ENOMEM;

    atomic_set(&res->ref_count, 1);
    res->data = 42;

    thread1 = kthread_run(thread_fn, res, "ref_thread1");
    thread2 = kthread_run(thread_fn, res, "ref_thread2");

    ssleep(2);  // Simulate main thread work
    release_resource(res);

    return 0;
}

static void __exit refcount_module_exit(void) {
    pr_info("Reference counting kernel module unloaded.\n");
}

module_init(refcount_module_init);
module_exit(refcount_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Copilot");
MODULE_DESCRIPTION("Kernel-space reference counting example using atomic operations");
*/
