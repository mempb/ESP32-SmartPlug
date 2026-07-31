#ifndef _HOMEKIT_H_
#define _HOMEKIT_H_

void homekit_start(void);
void plug_init(void);
void plug_deinit(void);
int plug_set_on(bool value);

// Hardware control — defined in main.c
void setLED(int led);
void setRelay(int relay);

#endif /* _HOMEKIT_H_ */