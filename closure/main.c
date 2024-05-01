#include <stdio.h>

typedef int (*increase_fn)(void *counter);
struct counter_i
{
    increase_fn increase;
};

struct normal_counter
{
    // 虚函数表
    struct counter_i vbtl;
    int count;
};

static int normal_increase(void *counter)
{
    struct normal_counter *ncounter = (struct normal_counter *)counter;
    return ++ncounter->count;
}

void init_vbtl(struct normal_counter *vbtl)
{
    // 直接初始化虚函数表
    struct counter_i *interface = (struct counter_i *)vbtl;
    interface->increase = normal_increase;
}

struct normal_counter *get_normal_counter()
{
    static struct normal_counter *ret = NULL;
    if (!ret) {
        static struct normal_counter counter = {
            .count = 0
        };
        ret = &counter;
        init_vbtl(ret);
    }

    return ret;
}

int main(int argc, char *argv[])
{
    struct counter_i *c = NULL;

    struct normal_counter *counter = get_normal_counter();
    c = (struct counter_i *)counter;
    printf("%d\n", c->increase(counter));

    return 0;
}
