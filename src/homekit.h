#ifndef _HOMEKIT_H_
#define _HOMEKIT_H_

/**
 * @brief Initializes and starts the HomeKit accessory task
 *        (WiFi, HAP core, outlet service, reset button).
 */
void homekit_start(void);

#endif /* _HOMEKIT_H_ */