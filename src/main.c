#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

int main(void)
{
    while (1)
    {
        k_msleep(1000);
        LOG_INF("Hello from %s", CONFIG_BOARD_TARGET);
    }

    return 0;
}
