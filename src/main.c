#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "bmi_wrapper.h"

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

static void print_sensor_values(const struct sensor_value *acc, const struct sensor_value *gyr,
				const struct sensor_value *steps)
{
	LOG_INF("AX: %d.%06d; AY: %d.%06d; AZ: %d.%06d; "
		"GX: %d.%06d; GY: %d.%06d; GZ: %d.%06d; "
		"Steps: %d",
		acc[0].val1, acc[0].val2, acc[1].val1, acc[1].val2, acc[2].val1, acc[2].val2,
		gyr[0].val1, gyr[0].val2, gyr[1].val1, gyr[1].val2, gyr[2].val1, gyr[2].val2,
		steps->val1);
}

int main(void)
{
	int ret = configure_bmi270();
	if (ret != 0) {
		LOG_ERR("Failed to configure BMI270: %d", ret);
		return -1;
	}

	struct sensor_value acc[3];
	struct sensor_value gyr[3];
	struct sensor_value steps;

	while (1) {
		k_msleep(1000);

		ret = read_data_bmi270(acc, gyr, &steps);
		if (ret != 0) {
			LOG_ERR("Failed to read BMI270: %d", ret);
			return -1;
		}

		print_sensor_values(acc, gyr, &steps);

		LOG_INF("Hello from %s", CONFIG_BOARD_TARGET);
	}

	return 0;
}
