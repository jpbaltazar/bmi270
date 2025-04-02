#ifndef BMI_WRAPPER_H_
#define BMI_WRAPPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

int configure_bmi270(void);
int read_data_bmi270(struct sensor_value acc[3], struct sensor_value gyr[3],
		     struct sensor_value *steps);

#ifdef __cplusplus
}
#endif

#endif /* BMI_WRAPPER_H_ */
