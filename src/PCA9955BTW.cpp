#include "PCA9955BTW.hpp"

PCA9955BTW::PCA9955BTW(TwoWire *wire, uint8_t address) : wire(wire), address(address)
{
    initialize();
}

void PCA9955BTW::initialize()
{
    setMode(mode[0], mode[1]);
    setCurrent((uint8_t)0x00);
    setLedOutState((uint8_t)0b10);
}

void PCA9955BTW::setMode(uint8_t mode1, uint8_t mode2)
{
    mode[0] = mode1;
    mode[1] = mode2;
    i2cWrite(MODE1 | 0x80, mode, 2);
}

void PCA9955BTW::setSleepOn()
{
    mode[0] |= 0b00010000;
    i2cWrite(MODE1, mode[0]);
}

void PCA9955BTW::setSleepOff()
{
    mode[0] &= 0b11101111;
    i2cWrite(MODE1, mode[0]);
}

void PCA9955BTW::setExpOn()
{
    mode[1] |= 0b00000100;
    i2cWrite(MODE2, mode[1]);
}

void PCA9955BTW::setExpOff()
{
    mode[1] = mode[1] & 0b11111011;
    i2cWrite(MODE2, mode[1]);
}

void PCA9955BTW::setLedOutState(uint8_t state)
{
    state &= 0b11;
    for (int i = 0; i < 16; i++)
    {
        ledOutState[i] = state;
    }
    for (int i = 0; i < 4; i++)
    {
        ledOut[i] = state << 6 + state << 4 + state << 2 + state;
    }
    i2cWrite(LEDOUT0 | 0x80, ledOut, 4);
}

void PCA9955BTW::setLedOutState(uint8_t ledNum, uint8_t state)
{
    state &= 0b11;
    ledNum &= 0xF;
    ledOutState[ledNum] = state;
    setLedOutFromLedOutState();
    i2cWrite(LEDOUT0 + ledNum / 4, ledOut[ledNum / 4]);
}

void PCA9955BTW::setLedOutState(uint8_t *state)
{
    for (int i = 0; i < 16; i++)
    {
        ledOutState[i] = state[i] & 0b11;
    }
    setLedOutFromLedOutState();
    i2cWrite(LEDOUT0 | 0x80, ledOut, 4);
}

void PCA9955BTW::setLedOutFromLedOutState()
{
    ledOut[0] = ledOutState[3] << 6 + ledOutState[2] << 4 + ledOutState[1] << 2 + ledOutState[0];
    ledOut[1] = ledOutState[7] << 6 + ledOutState[6] << 4 + ledOutState[5] << 2 + ledOutState[4];
    ledOut[2] = ledOutState[11] << 6 + ledOutState[10] << 4 + ledOutState[9] << 2 + ledOutState[8];
    ledOut[3] = ledOutState[15] << 6 + ledOutState[14] << 4 + ledOutState[13] << 2 + ledOutState[12];
}

void PCA9955BTW::setCurrent(uint8_t iRef)
{
    for (int i = 0; i < 16; i++)
    {
        this->iRef[i] = iRef;
    }
    i2cWrite(IREF0 | 0x80, this->iRef, 16);
}

void PCA9955BTW::setCurrent(uint8_t ledNum, uint8_t iRef)
{
    ledNum &= 0xF;
    this->iRef[ledNum] = iRef;
    i2cWrite(IREF0 + ledNum, this->iRef[ledNum]);
}

void PCA9955BTW::setCurrent(uint8_t *iRef)
{
    for (int i = 0; i < 16; i++)
    {
        this->iRef[i] = iRef[i];
    }
    i2cWrite(IREF0 | 0x80, this->iRef, 16);
}

void PCA9955BTW::setPwm(uint8_t pwm)
{
    for (int i = 0; i < 16; i++)
    {
        this->pwm[i] = pwm;
    }
    i2cWrite(PPWM0 | 0x80, this->pwm, 16);
}

void PCA9955BTW::setPwm(uint8_t ledNum, uint8_t pwm)
{
    ledNum &= 0xF;
    this->pwm[ledNum] = pwm;
    i2cWrite(PPWM0 + ledNum, this->pwm[ledNum]);
}

void PCA9955BTW::setPwm(uint8_t *pwm)
{
    for (int i = 0; i < 16; i++)
    {
        this->pwm[i] = pwm[i];
    }
    i2cWrite(PPWM0 | 0x80, this->pwm, 16);
}

void PCA9955BTW::i2cWrite(uint8_t resisterAddress, uint8_t data)
{
    Wire.beginTransmission(address);
    Wire.write(resisterAddress);
    Wire.write(data);
    Wire.endTransmission();
}

void PCA9955BTW::i2cWrite(uint8_t registerAddress, uint8_t *data, uint8_t dataLength)
{
    Wire.beginTransmission(address);
    Wire.write(registerAddress);
    for (int i = 0; i < dataLength; i++)
    {
        Wire.write(data[i]);
    }
    Wire.endTransmission();
}
