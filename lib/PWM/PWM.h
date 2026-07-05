#ifndef PWM_H
#define PWM_H

#include <Arduino.h>

/**
 * @brief PWM Status Register
 */
typedef struct
{
    uint8_t configured : 1;
    uint8_t enabled    : 1;
    uint8_t running    : 1;
    uint8_t reserved   : 5;

} PWM_STATUS;

class PWM
{
private:

    uint8_t pin;
    uint8_t channel;

    uint32_t frequency;
    uint8_t resolution;

    uint32_t maxDuty;
    uint32_t duty;

    PWM_STATUS status;

    static uint8_t nextChannel;

public:

    /**
     * @brief Construct PWM object
     *
     * @param gpio GPIO Number
     * @param freq PWM Frequency
     * @param res Resolution (1-16 bits)
     */
    PWM(uint8_t gpio,
        uint32_t freq = 5000,
        uint8_t res = 8);

    /**
     * @brief Initialize PWM
     */
    void begin();

    /**
     * @brief Enable PWM output
     */
    void enable();

    /**
     * @brief Disable PWM output
     */
    void disable();

    /**
     * @brief Turn output ON (100%)
     */
    void on();

    /**
     * @brief Turn output OFF
     */
    void off();

    /**
     * @brief Set duty percentage
     *
     * @param percent 0-100
     */
    void setDuty(uint8_t percent);

    /**
     * @brief Set raw duty
     *
     * @param raw Raw duty value
     */
    void setDutyRaw(uint32_t raw);

    /**
     * @brief Get duty percentage
     */
    uint8_t getDuty();

    /**
     * @brief Get raw duty
     */
    uint32_t getDutyRaw();

    /**
     * @brief Change frequency
     */
    void setFrequency(uint32_t freq);

    /**
     * @brief Get frequency
     */
    uint32_t getFrequency();

    /**
     * @brief Change resolution
     */
    void setResolution(uint8_t bits);

    /**
     * @brief Get GPIO
     */
    uint8_t getPin();

    /**
     * @brief Get channel
     */
    uint8_t getChannel();

    /**
     * @brief Get status
     */
    PWM_STATUS getStatus();

    /**
     * @brief Test PWM
     */
    void test();

};

#endif
