#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <logging/log.h>
LOG_MODULE_REGISTER(blinky, LOG_LEVEL_DBG);

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


class Blinky : public k_timer
{
    public:
        Blinky()
        {
            dev = device_get_binding(LED0);
            gpio_pin_configure(dev, PIN, GPIO_OUTPUT_ACTIVE | FLAGS);
            is_led_on = true;
            k_timer_init(this, timer_callback, NULL);
        }

        void Start(int sleepTimeMs)
        {
            k_timer_start(this, K_MSEC(0), K_MSEC(sleepTimeMs));
        }
    private:
        bool is_led_on;
        const struct device *dev;
        static void timer_callback(struct k_timer *timer_id)
        {
            Blinky *blinky = static_cast<Blinky*>(timer_id);
            gpio_pin_set(blinky->dev, PIN, blinky->is_led_on ? 1 : 0);
            blinky->is_led_on = !blinky->is_led_on;
        }
};

void main(void)
{
    LOG_MODULE_DECLARE(blinky, LOG_LEVEL_DBG);
    LOG_DBG("main");

    Blinky b;
    b.Start(1000);

    while (1)
    {
        k_msleep(500);
    }
}
