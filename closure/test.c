// https://www.bilibili.com/video/BV16T4y1s7Ts/?spm_id_from=333.788.top_right_bar_window_history.content.click&vd_source=6c9b967b6684728a4f76e134dfddded6
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

struct skip_counter
{
    struct counter_i *vptr;
    int count;
};

static int skip_increase(void *counter)
{
    struct skip_counter *scounter = (struct skip_counter *)counter;
    scounter->count += 2;
    return scounter->count;
}

static const struct counter_i skip_counter_vbtl = {
    .increase = skip_increase
};

void skip_counter_init(struct skip_counter *counter)
{
    counter->vptr = &skip_counter_vbtl;
    counter->count = 0;
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

    struct counter_i **cskip = NULL;
    struct skip_counter counter1;
    skip_counter_init(&counter1);
    cskip = (struct counter_i**)&counter1;
    printf("skip %d\n", (*cskip)->increase(cskip));


    return 0;
}
