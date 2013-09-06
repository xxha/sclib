#include <stdio.h>
#include <memory.h>
#include <fcntl.h>
#include <sys/mman.h>

enum ALL_PINS {
        OTDR_PIN1,
        OTDR_PIN2,
        OTDR_PIN3,
        OTDR_PIN4,
        OTDR_PIN5,
        ETH_PIN1,
        ETH_PIN2,
        ETH_PIN3
};

enum ETH_GPIO {
        ETH_GPIO1 = 1,
        ETH_GPIO2,
        ETH_GPIO3
};

enum OTDR_GPIO {
        OTDR_GPIO1 = 1,
        OTDR_GPIO2,
        OTDR_GPIO3,
        OTDR_GPIO4,
        OTDR_GPIO5
};

extern int gpio_init();
extern int gpio_exit();
extern int set_eth_gpio(unsigned int gpio, unsigned char data);
extern int get_eth_gpio (unsigned int gpio, unsigned char *data);
extern int set_otdr_gpio(unsigned int gpio, unsigned char data);
extern int get_otdr_gpio (unsigned int gpio, unsigned char *data);
