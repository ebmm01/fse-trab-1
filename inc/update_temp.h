#ifndef UPDATE_TEMP_H_
#define UPDATE_TEMP_H_

extern volatile int fan_value , resistor_value ;

extern volatile float internal_temp , ref_temp , external_temp , signal_control, pot_temp ;

void update_temp(int type, int manual_ref_value);

#endif /* UPDATE_TEMP_H_ */
