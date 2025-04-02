#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

#include "bmi270.h"
#include "bmi_wrapper.h"

LOG_MODULE_REGISTER(bmi_wrapper, CONFIG_LOG_DEFAULT_LEVEL);

// Defines

#define BMI270_REG_SC_26 0x32

#define BMI270_REG_SC_26_EN_DETECTOR BIT(11)
#define BMI270_REG_SC_26_EN_COUNTER  BIT(12)
#define BMI270_REG_SC_26_EN_ACTIVITY BIT(13)

static const struct device *const bmi_dev = DEVICE_DT_GET(DT_NODELABEL(bmi_wrapper));

static int configure_accel(struct sensor_value *full_scale, struct sensor_value *sampling_freq,
			   struct sensor_value *oversampling)
{
	int ret;

	ret = sensor_attr_set(bmi_dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_FULL_SCALE, full_scale);
	if (ret < 0) {
		LOG_ERR("Failed to set full scale, error %d", ret);
		return ret;
	}

	ret = sensor_attr_set(bmi_dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_OVERSAMPLING,
			      oversampling);
	if (ret < 0) {
		LOG_ERR("Failed to set oversampling, error %d", ret);
		return ret;
	}

	ret = sensor_attr_set(bmi_dev, SENSOR_CHAN_ACCEL_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY,
			      sampling_freq);
	if (ret < 0) {
		LOG_ERR("Failed to set sampling frequency, error %d", ret);
		return ret;
	}

	return 0;
}

static int configure_gyro(struct sensor_value *full_scale, struct sensor_value *sampling_freq,
			  struct sensor_value *oversampling)
{
	int ret;

	ret = sensor_attr_set(bmi_dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_FULL_SCALE, full_scale);
	if (ret < 0) {
		LOG_ERR("Failed to set full scale, error %d", ret);
		return ret;
	}

	ret = sensor_attr_set(bmi_dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_OVERSAMPLING,
			      oversampling);
	if (ret < 0) {
		LOG_ERR("Failed to set oversampling, error %d", ret);
		return ret;
	}

	ret = sensor_attr_set(bmi_dev, SENSOR_CHAN_GYRO_XYZ, SENSOR_ATTR_SAMPLING_FREQUENCY,
			      sampling_freq);
	if (ret < 0) {
		LOG_ERR("Failed to set sampling frequency, error %d", ret);
		return ret;
	}

	return 0;
}

int configure_bmi270(void)
{
	int ret;

	if (!device_is_ready(bmi_dev)) {
		LOG_ERR("Device %s is not ready", bmi_dev->name);
		return -EINVAL;
	}

	// setup accelerometer

	struct sensor_value full_scale = {.val1 = 2, .val2 = 0};
	struct sensor_value sampling_freq = {.val1 = 100, .val2 = 0};
	struct sensor_value oversampling = {.val1 = 1, .val2 = 0};

	ret = configure_accel(&full_scale, &sampling_freq, &oversampling);
	if (ret < 0) {
		LOG_ERR("Failed to configure accelerometer, error %d", ret);
		return ret;
	}

	// setup gyroscope

	full_scale.val1 = 500;
	sampling_freq.val1 = 100;
	oversampling.val1 = 1;

	ret = configure_gyro(&full_scale, &sampling_freq, &oversampling);
	if (ret < 0) {
		LOG_ERR("Failed to configure gyroscope, error %d", ret);
		return ret;
	}

	// setup step counter
	// Since there is no step counter channel,
	// I'll read it directly from the registers

	// For a more serious purpose,
	// a custom driver could be written for this purpose

	// Enable counter
	uint8_t sc_26;
	ret = bmi270_reg_read(bmi_dev, BMI270_REG_SC_26, &sc_26, sizeof(sc_26));
	if (ret != 1) {
		LOG_ERR("Failed to read SC_26, error %d", ret);
		return ret;
	}

	sc_26 |= BMI270_REG_SC_26_EN_COUNTER;

	ret = bmi270_reg_write(bmi_dev, BMI270_REG_SC_26, &sc_26, sizeof(sc_26));
	if (ret != 1) {
		LOG_ERR("Failed to write SC_26, error %d", ret);
		return ret;
	}

	return 0;
}

int read_data_bmi270(struct sensor_value acc[3], struct sensor_value gyr[3],
		     struct sensor_value *steps)
{
	int ret;

	if (!device_is_ready(bmi_dev)) {
		LOG_ERR("Device %s is not ready", bmi_dev->name);
		return -EINVAL;
	}

	ret = sensor_sample_fetch(bmi_dev);
	if (ret < 0) {
		LOG_ERR("Failed to fetch sample, error %d", ret);
		return ret;
	}

	if (acc) {
		ret = sensor_channel_get(bmi_dev, SENSOR_CHAN_ACCEL_XYZ, acc);
		if (ret < 0) {
			LOG_ERR("Failed to get accel sample, error %d", ret);
			return ret;
		}
	}

	if (gyr) {
		ret = sensor_channel_get(bmi_dev, SENSOR_CHAN_GYRO_XYZ, gyr);
		if (ret < 0) {
			LOG_ERR("Failed to get gyr sample, error %d", ret);
			return ret;
		}
	}

	if (steps) {
		uint32_t steps_reg;
		ret = bmi270_reg_read(bmi_dev, BMI270_REG_SC_OUT_0, (uint8_t *)&steps_reg,
				      sizeof(steps_reg));
		if (ret < 0) {
			LOG_ERR("Failed to get steps sample, error %d", ret);
			return ret;
		}

		steps->val1 = steps_reg;
	}

	return 0;
}
