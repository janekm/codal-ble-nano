#include "NRF52Pin.h"
#include "LEDArray.h"
#include "mbed.h"
#include "CodalFiber.h"
#include "math.h"

using namespace codal;

void codal::LEDArray::configure(void) {
    
    this->resetRequest = false;
    this->error_count = 0;
    //islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, IS31FL3731_BANK_FUNCTIONREG);
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, IS31FL3731_BANK_FUNCTIONREG);
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_REG_SHUTDOWN, 0x00);
    //addressPin.setDigitalValue(0);
    //fiber_sleep(10);
    //addressPin.setDigitalValue(1);
    //fiber_sleep(10);
    fiber_sleep(200);
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, IS31FL3731_BANK_FUNCTIONREG);
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_REG_SHUTDOWN, 0x01);
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, IS31FL3731_BANK_FUNCTIONREG);
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_REG_CONFIG, IS31FL3731_REG_CONFIG_PICTUREMODE);
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, IS31FL3731_BANK_FUNCTIONREG);
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_REG_PICTUREFRAME, 0);
    for (uint8_t f=0; f<8; f++) {  
        islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, f);
        for (uint8_t i=0; i<=0x11; i++) {
            islWriteRegister(IS31FL3731_ADDR, i, 0xff);     // each 8 LEDs on
        }
    }
    //is31_frame_buffer[0] = 0x24; // load register address of frame into frist element for faster I2C write with mbed
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, this->selected_frame);
    //for (int i = 0; i<144; i++) {
    //    is31_frame_buffer[i] = 0x40;
    //}
    //is31_frame_buffer[4] = 0xFF; 
    //i2c_driver_write_at_once(IS31FL3731_ADDR, 0x24, is31_frame_buffer, 144);
    //for(uint8_t i = 0; i < 144; i++) {
    //    islWriteRegister(IS31FL3731_ADDR, 0x24+i, is31_frame_buffer[i]);
    //}
    //for(uint8_t i = 0; i < 6; i++) {
    //    islWriteRegister(address, 0x24+i*24, &is31_frame_buffer[i*24], 24);
    //}
    //pushFrame();

}

void codal::LEDArray::islWriteRegister(uint16_t address, uint8_t reg, uint8_t value) {
    uint8_t command[2];
    command[0] = reg;
    command[1] = value;
    addressPin.setDigitalValue(1);

    if (i2c.write(address, (const char *)command, 2) == DEVICE_OK) {
        //pc.printf("I2C OK\r\n");
    } else {
        resetRequest = true;
        //error_count++;
        //addressPin.setDigitalValue(0);
        //fiber_sleep(10);
        //addressPin.setDigitalValue(1);
        //fiber_sleep(10);

        //configure();
        //pc.printf("I2C Error\r\n");
    }
    addressPin.setDigitalValue(0);
}

void codal::LEDArray::islWriteRegister(uint16_t address, uint8_t reg, uint8_t *data, int len) {
    uint8_t command[32];
    command[0] = reg;
    addressPin.setDigitalValue(1);
    for(int i = 0; i < len; i++) {
        command[i+1] = data[i];
    }
    if (i2c.write(address, (const char *)command, len+1) == DEVICE_OK) {
        //pc.printf("I2C OK\r\n");
        error_count = 0;
    } else {
        resetRequest = true;
        error_count++;
        //addressPin.setDigitalValue(0);
        //fiber_sleep(10);
        //addressPin.setDigitalValue(1);
        //fiber_sleep(10);
    }
    addressPin.setDigitalValue(0);
}

void codal::LEDArray::pushFrame(void) {
    for(uint8_t i = 0; i < 6; i++) {
        islWriteRegister(address, 0x24+i*24, &is31_frame_buffer[i*24], 24);
    }
}

void codal::LEDArray::selectFrame(uint8_t f) {
    this->selected_frame =f;
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, IS31FL3731_BANK_FUNCTIONREG);
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, f);
}

void codal::LEDArray::displayFrame(uint8_t f) {
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, IS31FL3731_BANK_FUNCTIONREG);
    islWriteRegister(IS31FL3731_ADDR, IS31FL3731_REG_PICTUREFRAME, f);
}

constexpr uint8_t codal::LEDArray::is_gamma[64];

void codal::LEDArray::renderPlasma(float sec) {
    float dx, dy, dv;
    if(resetRequest) {
        resetRequest = false;
        //configure();
        
        addressPin.setDigitalValue(0);
        fiber_sleep(10);
        addressPin.setDigitalValue(1);
        fiber_sleep(10);
        islWriteRegister(IS31FL3731_ADDR, IS31FL3731_REG_PICTUREFRAME, 0);
        for (uint8_t f=0; f<2; f++) {  
            islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, f);
            for (uint8_t i=0; i<=2; i++) {
                islWriteRegister(IS31FL3731_ADDR, i, 0xff);     // each 8 LEDs on
            }
        }
        //is31_frame_buffer[0] = 0x24; // load register address of frame into frist element for faster I2C write with mbed
        islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, this->selected_frame);
        
    }
    if (error_count > 2) {
        //DMESG("I2C Reset\r\n");
        NVIC_SystemReset();
        //NRF_TWIM1->ENABLE = 0;
        //fiber_sleep(200);
        //NRF_TWIM1->ENABLE = 1;
        //i2c.stop();
        addressPin.setDigitalValue(1);
        fiber_sleep(2000);
        addressPin.setDigitalValue(0);
        fiber_sleep(2000);
        //fiber_sleep(100);
        configure();
    }
    for(int x = 0; x < 16; x++) {
        for (int y = 0; y < 9; y++) {
            dx = ((float)x)/16.0 + .5 * sin(sec/5.0);
			dy = ((float)y)/16.0 + .5 * cos(sec/3.0);
            dv = sin(sqrt(50.0 * (dx*dx + dy*dy)+1.0)+ sec);
 
			//dv = sin(x*10 + sec) + sin(10*(x*sin(sec/2.0) + y*cos(sec/3.0)) + sec) + sin(sqrt(100*(dx*dx + dy*dy)+1) + sec);
            is31_frame_buffer[x+y*16] = is_gamma[(int)(63*(sin(dv*pi)*0.5+0.5))];
        }
    }
    //for(uint8_t i = 0; i < 144; i++) {
    //    islWriteRegister(IS31FL3731_ADDR, 0x24+i, is31_frame_buffer[i]);
    //}
    //i2c.write(address, (const char *)is31_frame_buffer, 145);
    //islWriteRegister(IS31FL3731_ADDR, IS31FL3731_COMMANDREGISTER, selected_frame);
    pushFrame();
}

void codal::LEDArray::sendImage(char* image_data) {
    for(int i = 0; i < 16*9; i++) {
        is31_frame_buffer[i] = image_data[i];
    }
    pushFrame();
}
/**
  * Constructor.
  *
 */
codal::LEDArray::LEDArray(mbed::I2C &_i2c, codal::NRF52Pin &_address_pin, uint16_t _address) : i2c(_i2c), addressPin(_address_pin) 
{
    this->address = _address;
    this->selected_frame = 0;
    configure();
}
