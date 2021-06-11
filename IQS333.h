#ifndef IQS333_H
#define IQS333_H

#include <Arduino.h>
#include <Wire.h>

#define RDY                               2             //PIN2 on Arduino used as RDY pin

#define IQS333_I2C_Address                0x64          //IQS333A I2C Address

#define STOP                              true    
#define RESTART                           false

// Communication Command / Address Structure on IQS333 - ie. Memory Map
#define VERSION_INFO                      0x00  // Product number can be read      :  2 bytes
#define FLAGS                             0x01  // System flags and events         :  1 byte
#define WHEEL_COORDS                      0x02  // Wheel coordinates - 2 wheels    :  4 bytes
#define TOUCH_BYTES                       0x03  // Touch channels                  :  2 bytes
#define COUNTS                            0x04  // Count Values                    : 20 bytes
#define LTA                               0x05  // LTA Values                      : 20 bytes
#define MULTIPLIERS                       0x06  // Multipliers Values              : 10 bytes
#define COMPENSATION                      0x07  // Compensation Values (PCC)       : 10 bytes
#define PROXSETTINGS                      0x08  // Prox Settings - Various         :  6 bytes
#define THRESHOLDS                        0x09  // Threshold Values                : 10 bytes
#define TIMINGS                           0x0A  // Timings                         :  5 bytes
#define ATI_TARGETS                       0x0B  // Targets for ATI                 :  2 bytes
#define PWM                               0x0C  // PWM Settings                    :  4 bytes
#define PWM_LIMIT                         0x0D  // PWM Limits and speed            :  2 bytes
#define ACTIVE_CHANNELS                   0x0E  // Active channels                 :  2 bytes
#define BUZZER                            0x0F  // Buzzer Output                   :  1 byte

void readRandomBytes(uint8_t I2C_Address, uint8_t memoryAddress, int numBytes, uint8_t bytesArray[], bool stopOrRestart);

void writeRandomBytes(uint8_t I2C_Address, uint8_t memoryAddress, int numBytes, uint8_t bytesArray[], bool stopOrRestart);

void IQS333_setup();

int *IQS333_read();

void IQS333_settings();

#endif
