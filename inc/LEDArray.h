#ifndef LEDARRAY_H
#define LEDARRAY_H

#include "mbed.h"

#define IS31FL3731_ADDR (0x77<<1)

#define IS31FL3731_REG_CONFIG  0x00
#define IS31FL3731_REG_CONFIG_PICTUREMODE 0x00
#define IS31FL3731_REG_CONFIG_AUTOPLAYMODE 0x08
#define IS31FL3731_REG_CONFIG_AUDIOPLAYMODE 0x18

#define IS31FL3731_CONF_PICTUREMODE 0x00
#define IS31FL3731_CONF_AUTOFRAMEMODE 0x04
#define IS31FL3731_CONF_AUDIOMODE 0x08

#define IS31FL3731_REG_PICTUREFRAME  0x01

#define IS31FL3731_REG_SHUTDOWN 0x0A
#define IS31FL3731_REG_AUDIOSYNC 0x06

#define IS31FL3731_COMMANDREGISTER 0xFD
#define IS31FL3731_BANK_FUNCTIONREG 0x0B    // helpfully called 'page nine'

#define pi 3.1415

namespace codal
{
    /**
     * Class definition for LED Array.
     *
     */
    class LEDArray
    {
        mbed::I2C   &i2c;                // The I2C interface to use.
        NRF52Pin             &addressPin;              // Data ready interrupt.
        uint16_t        address;            // I2C address of this accelerometer.
        static constexpr uint8_t is_gamma[64] = {0, 1, 2, 3, 4, 5, 6, 7, 
                        8, 10, 12, 14, 16, 18, 20, 22,
                        24, 26, 29, 32, 35, 38, 41, 44,
                        47, 50, 53, 57, 61, 65, 69, 73,
                        77, 81, 85, 89, 94, 99, 104, 109,
                        114, 119, 124, 129, 134, 140, 146, 152,
                        158, 164, 170, 176, 182, 188, 195, 202,
                        209, 216, 223, 230, 237, 244, 251, 255};

        int selected_frame;
        bool resetRequest;
        int error_count;

        public:
        uint8_t is31_frame_buffer[145];

        /**
          * Constructor.
          *
          * @param _i2c an instance of I2C used to communicate with the onboard accelerometer.
          *
          * @param address the default I2C address of the accelerometer. Defaults to: MMA8653_DEFAULT_ADDR.
          *
          * @endcode
         */
        LEDArray(mbed::I2C &_i2c, NRF52Pin &_address_pin, uint16_t _address = IS31FL3731_ADDR);
        void selectFrame(uint8_t f);
        void displayFrame(uint8_t f);
        void renderPlasma(float sec);
        void sendImage(char* image_data);
        void pushFrame(void);
        void setPixel(int x, int y, char colour);

        char getPixel(int x, int y);

        private:

        void islWriteRegister(uint16_t address, uint8_t reg, uint8_t value);
        void islWriteRegister(uint16_t address, uint8_t reg, uint8_t *data, int len);

        void configure(void);
    };
}

#endif // LEDARRAY_H
