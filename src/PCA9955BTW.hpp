#ifndef _PCA9955BTW_H_
#define _PCA9955BTW_H_

#include <Arduino.h>
#include <Wire.h>

#define PCA9955BTW_ADDR 0x65

#define ON 0xFF
#define OFF 0x00

#define LEDOFF 0b00
#define LEDON 0b01
#define LEDPWM 0b10
#define LEDGRP 0b11

//  Control registers
#define MODE1 0x00
#define MODE2 0x01
#define LEDOUT0 0x02
#define LEDOUT1 0x03
#define LEDOUT2 0x04
#define LEDOUT3 0x05
#define GRPPWM 0x06
#define GRPFREQ 0x07
#define PPWM0 0x08
#define PPWM1 0x09
#define PPWM2 0x0A
#define PPWM3 0x0B
#define PPWM4 0x0C
#define PPWM5 0x0D
#define PPWM6 0x0E
#define PPWM7 0x0F
#define PPWM8 0x10
#define PPWM9 0x11
#define PPWM10 0x12
#define PPWM11 0x13
#define PPWM12 0x14
#define PPWM13 0x15
#define PPWM14 0x16
#define PPWM15 0x17
#define IREF0 0x18
#define IREF1 0x19
#define IREF2 0x1A
#define IREF3 0x1B
#define IREF4 0x1C
#define IREF5 0x1D
#define IREF6 0x1E
#define IREF7 0x1F
#define IREF8 0x20
#define IREF9 0x21
#define IREF10 0x22
#define IREF11 0x23
#define IREF12 0x24
#define IREF13 0x25
#define IREF14 0x26
#define IREF15 0x27
#define RAMP_RATE_GRP0 0x28
#define STEP_TIME_GRP0 0x29
#define HOLD_CNTL_GRP0 0x2A
#define IREF_GRP0 0x2B
#define RAMP_RATE_GRP1 0x2C
#define STEP_TIME_GRP1 0x2D
#define HOLD_CNTL_GRP1 0x2E
#define IREF_GRP1 0x2F
#define RAMP_RATE_GRP2 0x30
#define STEP_TIME_GRP2 0x31
#define HOLD_CNTL_GRP2 0x32
#define IREF_GRP2 0x33
#define RAMP_RATE_GRP3 0x34
#define STEP_TIME_GRP3 0x35
#define HOLD_CNTL_GRP3 0x36
#define IREF_GRP3 0x37
#define GRAD_MODE_SEL0 0x38
#define GRAD_MODE_SEL1 0x39
#define GRAD_GRP_SEL0 0x3A
#define GRAD_GRP_SEL1 0x3B
#define GRAD_GRP_SEL2 0x3C
#define GRAD_GRP_SEL3 0x3D
#define GRAD_CNTL 0x3E
#define OFFSET 0X3F
#define SUBADR1 0x40
#define SUBADR2 0x41
#define SUBADR3 0x42
#define ALLCALLADR 0x43
#define PWMALL 0x44
#define IREFALL 0x45
#define EFLAG0 0x46
#define EFLAG1 0x47
#define EFLAG2 0x48
#define EFLAG3 0x49

class PCA9955BTW
{
public:
    PCA9955BTW(TwoWire *wire, uint8_t address);
    void initialize();

    void setMode(uint8_t mode1, uint8_t mode2);
    void setSleepOn();
    void setSleepOff(); // Default -- Off
    void setExpOn();
    void setExpOff(); // Default -- Off
    // Brightness adjustment for gradation control is either linear or exponential by setting the EXP_EN bit as shown in Figure 5.
    // When EXP_EN = 0, linear adjustment scale is used. When EXP_EN = 1, exponential scale is used.

    void setLedOutState(uint8_t state);
    void setLedOutState(uint8_t ledNum, uint8_t state);
    void setLedOutState(uint8_t *state);

    void setCurrent(uint8_t iRef);
    void setCurrent(uint8_t ledNum, uint8_t iRef);
    void setCurrent(uint8_t *iRef);

    void setPwm(uint8_t pwm);
    void setPwm(uint8_t ledNum, uint8_t pwm);
    void setPwm(uint8_t *pwm);

private:
    TwoWire *wire;
    uint8_t address;
    uint8_t mode[2] = {0x00, 0x01};
    uint8_t ledOutState[16] = {LEDPWM, LEDPWM, LEDPWM, LEDPWM,
                               LEDPWM, LEDPWM, LEDPWM, LEDPWM,
                               LEDPWM, LEDPWM, LEDPWM, LEDPWM,
                               LEDPWM, LEDPWM, LEDPWM, LEDPWM};
    uint8_t ledOut[4] = {LEDPWM << 6 + LEDPWM << 4 + LEDPWM << 2 + LEDPWM,
                         LEDPWM << 6 + LEDPWM << 4 + LEDPWM << 2 + LEDPWM,
                         LEDPWM << 6 + LEDPWM << 4 + LEDPWM << 2 + LEDPWM,
                         LEDPWM << 6 + LEDPWM << 4 + LEDPWM << 2 + LEDPWM};
    //  ledOutのLEDの対応は
    //  3,  2,  1,  0,
    //  7,  6,  5,  4,
    // 11, 10,  9,  8,
    // 15, 14, 13, 12
    // 00 -- LED driver x is off.
    // 01 -- LED driver x is fully on.
    // 10 -- LED driver x individual brightness can be controlled through its PWMx register (default power-up state) or PWMALL register for all LEDn outputs.
    // 11 -- LED driver x individual brightness and group dimming/blinking can be controlled through its PWMx register and the GRPPWM registers.
    uint8_t iRef[16] = {OFF, OFF, OFF, OFF,
                        OFF, OFF, OFF, OFF,
                        OFF, OFF, OFF, OFF,
                        OFF, OFF, OFF, OFF};
    // iRef -- 0x00 ~ 0xFF
    // LED current = iRef * 900mV / (4 * R_ext)
    uint8_t pwm[16] = {ON, ON, ON, ON,
                       ON, ON, ON, ON,
                       ON, ON, ON, ON,
                       ON, ON, ON, ON};
    void setLedOutFromLedOutState();
    void i2cWrite(uint8_t resisterAddress, uint8_t data);
    void i2cWrite(uint8_t registerAddress, uint8_t *data, uint8_t dataLength);
};

#endif