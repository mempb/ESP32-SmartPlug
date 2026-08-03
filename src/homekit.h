#ifndef _HOMEKIT_H_
#define _HOMEKIT_H_

void homekit_start(void);
int plug_set(bool value);

// Hardware control — defined in main.c
void setIO(bool state);

#endif /* _HOMEKIT_H_ */