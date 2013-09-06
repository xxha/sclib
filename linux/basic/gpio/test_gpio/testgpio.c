/* By Yanjun Luo, for MX27 gpio test with application software. */

#include <unistd.h>
#include <stdlib.h>
#include "gpio.h"
 
#define	LED1	16
#define	LED2	17
#define	LED3	18
#define	LED4	19

#define MX120_GPIO1	16

#define TENGG_GPIO1     11
#define	TENGG_GPIO3	19
#define TENGG_GPIO2	30

#define	TEST_DELAY	30

#define	CHARGE_IN	15

#define	I2C1_SDA	17	//d
#define	I2C1_SCL	18
#define	I2C2_SDA	5	//c
#define	I2C2_SCL	6


int main()
{
	int ret = 0;
	int i = 0;
	unsigned int data = 0;
	
	ret = gpio_init();
	if(ret < 0){
		printf("GPIO init failed!\n");
		exit(1);
	}

#if 0
        setpullup(PORT_D, I2C1_SDA, 1);
        setgpiodir(PORT_D, I2C1_SCL, INPUT);

        setpullup(PORT_C, I2C2_SDA, 1);
        setgpiodir(PORT_C, I2C2_SCL, INPUT);

	for(;;){
		getgpiodata(PORT_D, I2C1_SDA, &data);
		printf("sda: %d\n", data);
		getgpiodata(PORT_D, I2C1_SCL, &data);
		printf("scl: %d\n", data);
		sleep(1);
	}
#endif

#if 0
	setpullup(PORT_F, TENGG_GPIO1, 1);
	setgpiodir(PORT_F, TENGG_GPIO1, OUTPUT);
	for(;;){
		setgpiodata(PORT_F, TENGG_GPIO1, 1);
		sleep(1);
		setgpiodata(PORT_F, TENGG_GPIO1, 0);
		sleep(1);
	}
#endif

#if 0
	setpullup(PORT_D, I2C1_SDA, 1);
	setgpiodir(PORT_D, I2C1_SCL, OUTPUT);

	setpullup(PORT_C, I2C2_SDA, 1);
	setgpiodir(PORT_C, I2C2_SCL, OUTPUT);

        for(;;){
		setgpiodata(PORT_D, I2C1_SDA, 0);
		setgpiodata(PORT_D, I2C1_SCL, 0);

                setgpiodata(PORT_C, I2C2_SDA, 0);
		setgpiodata(PORT_C, I2C2_SCL, 0);

		sleep(1);

                setgpiodata(PORT_D, I2C1_SDA, 1);
                setgpiodata(PORT_D, I2C1_SCL, 1);

		setgpiodata(PORT_C, I2C2_SDA, 1);
		setgpiodata(PORT_C, I2C2_SCL, 1);
	
		sleep(1);
	}
#endif

#if 0
	setpullup(PORT_F, TENGG_GPIO3, 1);
	setgpiodir(PORT_F, TENGG_GPIO3, INPUT);

	for(;;){
		getgpiodata(PORT_F, TENGG_GPIO3, &data);
		printf("10G GPIO3: %d\n", data);
		sleep(1);
	}
#endif

#if 0	
	setpullup(PORT_F, MX120_GPIO1, 1);
	setgpiodir(PORT_F, MX120_GPIO1, OUTPUT);

	for(;;){
		setgpiodata(PORT_F, MX120_GPIO1, 1);
		printf("MX120 running!\n");
		sleep(TEST_DELAY);
		setgpiodata(PORT_F, MX120_GPIO1, 0);
		printf("MX120 stop!\n");
		sleep(TEST_DELAY);
	}
#endif

#if 1
	setpullup(PORT_C, TENGG_GPIO2, 1);
	setgpiodir(PORT_C, TENGG_GPIO2, OUTPUT);
	setgpiodata(PORT_C, TENGG_GPIO2, 0);
#endif

#if 0
	sleep(1);
	
for(i = 0; i < 100; i ++){
	setgpiodata(PORT_F, TENGG_GPIO2, 0);
	
	sleep(1);

	setgpiodata(PORT_F, TENGG_GPIO2, 1);

	sleep(1);
}
#endif

#if 0
	setpullup(PORT_C, LED1, 1);
	setpullup(PORT_C, LED2, 1);
	setpullup(PORT_C, LED3, 1);
	setpullup(PORT_C, LED4, 1);

	setgpiodir(PORT_C, LED1, INPUT);
	setgpiodir(PORT_C, LED2, OUTPUT);
	setgpiodir(PORT_C, LED3, OUTPUT);
	setgpiodir(PORT_C, LED4, OUTPUT);

	for(i = 0; i < 10; i ++){
		getgpiodata(PORT_C, LED1, &data);
		printf("%d\n", data);
		setgpiodata(PORT_C, LED2, 1);
		setgpiodata(PORT_C, LED3, 1);
		setgpiodata(PORT_C, LED4, 1);
		sleep(1);
	
		getgpiodata(PORT_C, LED1, &data);
		printf("%d\n", data);
		setgpiodata(PORT_C, LED2, 0);
		setgpiodata(PORT_C, LED3, 0);
		setgpiodata(PORT_C, LED4, 0);
		sleep(1);
	}
#endif
	
	return 0;
}

