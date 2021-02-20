#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <logging/log.h>
LOG_MODULE_REGISTER(blinky, LOG_LEVEL_DBG);

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   500

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0	DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN	DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS	DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0	""
#define PIN	0
#define FLAGS	0
#endif

static const struct device *dev = NULL;

static void timer_callback(struct k_timer *timer_id)
{
    static bool is_led_on = true; 
    gpio_pin_set(dev, PIN, (int)is_led_on);
    is_led_on = !is_led_on;
}

void main(void)
{    
    int ret = 0;
    struct k_timer my_timer;

    LOG_MODULE_DECLARE(blinky, LOG_LEVEL_DBG);

    dev = device_get_binding(LED0);
    if (dev == NULL) 
    {
        LOG_ERR("Unable to get device binding for LED0");
    }

    ret = gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
    if (ret != 0)
    {
        LOG_ERR("Unable to configure PIN0 as output");
    }


    k_timer_init(&my_timer, timer_callback, NULL);
    k_timer_start(&my_timer, K_MSEC(0), K_MSEC(SLEEP_TIME_MS));

    while (1)
    {
        k_msleep(SLEEP_TIME_MS);
    }
}
