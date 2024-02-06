/*
 * Blinky Sample
 */

/* 1. Include modules

	The blinky sample uses the following modules in the nRF Connect SDK:

	Kernel services <zephyr/kernel.h> for the sleep function k_msleep(). This header is called the Public kernel 
	API header.
	The generic GPIO interface <zephyr/drivers/gpio.h> for the structure gpio_dt_spec, the macros GPIO_DT_SPEC_GET(), 
	and the functions, gpio_is_ready_dt(), gpio_pin_configure_dt() and gpio_pin_toggle_dt().
*/
#include <zephyr/kernel.h> 			
#include <zephyr/drivers/gpio.h>	

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   500

/* 2. Define the node identifier

	The line below uses the devicetree macro DT_ALIAS() to get the node identifier symbol LED0_NODE, which will 
	represent LED1 (node led_0). Remember from the Devicetree section that led_0 node is defined in the devicetree 
	of the DK. LED0_NODE is now the source code symbol that represents the hardware for LED1.

	The DT_ALIAS() macro gets the node identifier from the node’s alias, which as we saw in the Devicetree section, 
	is led0.

	leds {
			compatible = "gpio-leds";
			led0: led_0 {
				gpios = <&gpio0 2 0>;
				label = "Green LED 1";
			};

The devicetree node identifier for the "led0" alias. 
*/

#define LED0_NODE DT_ALIAS(led0)

/* Note
	There are many ways to retrieve the node identifier. The macros DT_PATH(), DT_NODELABEL(), DT_ALIAS(), 
	and DT_INST() all return the node identifier, based on various parameters.
*/


/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */

/* 3. Retrieve the device pointer, pin number, and configuration flags.

	The macro call GPIO_DT_SPEC_GET() returns the structure gpio_dt_spec led, which contains the device pointer 
	for node led_0 as well as the pin number and associated configuration flags. The node identifier LED0_NODE,
	defined in the previous step, has this information embedded inside its gpios property. Note the second 
	parameter gpios, the name of the property containing all this information.
*/
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	int ret;

	/* 4. Verify that the device is ready for use

		As we mentioned before, we must pass the device pointer of the gpio_dt_spec , in this case led, 
		to gpio_is_ready_dt(), to verify that the device is ready for use.
	*/
	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	/* 5. Configure the GPIO pin

		The generic GPIO API function gpio_pin_configure_dt() is used to configure the GPIO pin associated 
		with led as an output (active low) and initializes it to a logic 1, as explained in the GPIO Generic 
		API section.
	*/
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	/* 6. Continuously toggle the GPIO pin

		Finally, the blinky main function will enter an infinite loop where we continuously toggle the GPIO pin 
		using gpio_pin_toggle_dt(). Note that in every iteration, we are also calling the kernel service function 
		k_msleep(), which puts the main function to sleep for 1 second, resulting in the blinking behavior 
		at 1-second intervals.
	*/
	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
