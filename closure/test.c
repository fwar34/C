#include <stdio.h>

typedef int (*increase_fn)(void *counter);

// 虚函数表
struct counter_i
{
    increase_fn increase;
};

struct normal_counter
{
    // 虚函数表指针，指向虚函数表
    const struct counter_i *vptr;
    int count;
};

int normal_counter_increase(struct normal_counter *self);

static const struct counter_i normal_counter_interface = {
    .increase = (increase_fn)normal_counter_increase
};

void normal_counter_init(struct normal_counter *self)
{
    self->count = 0;
    // 将 self 的虚函数表指针指向虚函数表 normal_counter_interface
    self->vptr = &normal_counter_interface;
}

int normal_counter_increase(struct normal_counter *self)
{
    return ++self->count;
}

int main(int argc, char *argv[])
{
    struct counter_i **c = NULL;
    struct normal_counter counter0;
    int ret = 0;
    normal_counter_init(&counter0);

    c = (struct counter_i **)&counter0;
    ret = (*c)->increase(c);
    printf("%d\n", ret);

    return 0;
}
