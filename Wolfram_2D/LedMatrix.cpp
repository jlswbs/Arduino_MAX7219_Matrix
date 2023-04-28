#include <string.h>
#include "LedMatrix.h"
#include "HardwareSerial.h"
#include "SPI.h"

const uint8_t NO_OP_CMD         = 0x00;
const uint8_t DIGIT_0_CMD       = 0x01;
const uint8_t DIGIT_1_CMD       = 0x02;
const uint8_t DIGIT_2_CMD       = 0x03;
const uint8_t DIGIT_3_CMD       = 0x04;
const uint8_t DIGIT_4_CMD       = 0x05;
const uint8_t DIGIT_5_CMD       = 0x06;
const uint8_t DIGIT_6_CMD       = 0x07;
const uint8_t DIGIT_7_CMD       = 0x08;
const uint8_t DECODE_MOED_CMD   = 0x09;
const uint8_t INTENSITY_CMD     = 0x0A;
const uint8_t SCAN_LIMIT_CMD    = 0x0B;
const uint8_t SHUTDOWN_CMD      = 0x0C;
const uint8_t DISPLAY_TEST_CMD  = 0x0F;

const uint8_t SHUTDOWN_OFF      = 0x01;
const uint8_t NO_DECODE         = 0x00;

LedMatrix::LedMatrix(uint8_t cs_pin, bool is_serial_print=false, uint8_t led_en_char='*', uint8_t led_dis_char='.')
{
    cs_pin_ = cs_pin;        
    led_en_char_ = led_en_char;
    led_dis_char_ = led_dis_char;
    is_serial_print_ = is_serial_print;
}

void LedMatrix::begin(void)
{
    clearBuffer();
    SPI.begin();
    pinMode(cs_pin_, OUTPUT);
    send_cmd_(SHUTDOWN_CMD, SHUTDOWN_OFF);
    send_cmd_(DECODE_MOED_CMD, NO_DECODE);
    send_cmd_(SCAN_LIMIT_CMD, 0xff);
    send_cmd_(INTENSITY_CMD, 0);
    send_cmd_(DISPLAY_TEST_CMD, 0);
}

void LedMatrix::send_cmd_(uint8_t address, uint8_t value)
{
    digitalWrite(cs_pin_, LOW);

     // delay(100);

    SPI.transfer(address);
    SPI.transfer(value);

    // delay(100);

    digitalWrite(cs_pin_, HIGH);
}

void LedMatrix::print_line_(uint8_t line_data)
{
    uint8_t led_char;
    for (uint8_t i = 0; i < LED_MATRIX_LINES_LEN; i++)
    {
        if (line_data & (1 << (LED_MATRIX_LINES_LEN - 1)))
        {
            led_char = led_en_char_;
        }
        else
        {
            led_char = led_dis_char_;
        }
        Serial.write(led_char);
        line_data <<= 1;
    }
}

void LedMatrix::setBuffer(uint8_t * buffer)
{
    memcpy(buffer_, buffer, LED_MATRIX_LINES_QTY);    
}

uint8_t * LedMatrix::getBuffer(void)
{
    return buffer_;
}

void LedMatrix::setIntensity(uint8_t intensity)
{
    send_cmd_(INTENSITY_CMD, intensity);
}

void LedMatrix::sendBuffer(void)
{
    for (uint8_t digit = DIGIT_0_CMD; digit <= DIGIT_7_CMD; digit++)
    {
       send_cmd_(digit, buffer_[digit - DIGIT_0_CMD]);
       if (is_serial_print_)
       {
            print_line_(buffer_[digit - DIGIT_0_CMD]);
            Serial.println("");
       }
    }
    if (is_serial_print_)
    {
        Serial.println("--------");
    }
}

void LedMatrix::clearBuffer(void)
{
    memset(buffer_, 0, LED_MATRIX_LINES_QTY);
}

void LedMatrix::setBufferPixel(uint8_t x, uint8_t y)
{
    if (x < LED_MATRIX_LINES_LEN && y < LED_MATRIX_LINES_QTY)
    {
        buffer_[y] |= (1 << x);
    }
}

void LedMatrix::clearBufferPixel(uint8_t x, uint8_t y)
{
    if (x < LED_MATRIX_LINES_LEN && y < LED_MATRIX_LINES_QTY)
    {
        buffer_[y] &= ~(1 << x);
    }
}
