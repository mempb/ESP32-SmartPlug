#ifndef _PLUG_H_
#define _PLUG_H_

/**
 * @brief initialize the plug lowlevel module
 *
 * @param none
 *
 * @return none
 */
void plug_init(void);

/**
 * @brief deinitialize the plug lowlevel module
 *
 * @param none
 *
 * @return none
 */
void plug_deinit(void);

/**
 * @brief turn on/off the lowlevel plug
 *
 * @param value The "On" value
 *
 * @return none
 */
int plug_set_on(bool value);

#endif /* _PLUG_H_ */
