#include "nrf_drv_pwm.h"
#include "app_util_platform.h"


static nrf_drv_pwm_t m_pwm0 = NRF_DRV_PWM_INSTANCE(0);
static nrf_drv_pwm_t m_pwm1 = NRF_DRV_PWM_INSTANCE(1);

static nrf_pwm_values_individual_t m_motor1_seq_values;
static nrf_pwm_sequence_t m_motor1_seq;
static nrf_pwm_values_individual_t m_motor2_seq_values;
static nrf_pwm_sequence_t m_motor2_seq;

void motor_driver_init(void)
{
    m_motor1_seq.values.p_individual = &m_motor1_seq_values;
    m_motor1_seq.length              = NRF_PWM_VALUES_LENGTH(m_motor1_seq_values);
    m_motor1_seq.repeats             = 0;
    m_motor1_seq.end_delay           = 0;

    nrf_drv_pwm_config_t const config0 =
    {
        .output_pins =
        {
            4 , // channel 0
            6 , // channel 1
            11 , // channel 2
            10   // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOW,
        .base_clock   = NRF_PWM_CLK_8MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 400,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    nrf_drv_pwm_init(&m_pwm0, &config0, NULL);

    m_motor1_seq_values.channel_0 = 0;
    m_motor1_seq_values.channel_1 = 0;
    m_motor1_seq_values.channel_2 = 0;
    m_motor1_seq_values.channel_3 = 0;

    nrf_drv_pwm_simple_playback(&m_pwm0, &m_motor1_seq, 1,
        NRF_DRV_PWM_FLAG_LOOP);

    m_motor2_seq.values.p_individual = &m_motor2_seq_values;
    m_motor2_seq.length              = NRF_PWM_VALUES_LENGTH(m_motor2_seq_values);
    m_motor2_seq.repeats             = 0;
    m_motor2_seq.end_delay           = 0;

    nrf_drv_pwm_config_t const config1 =
    {
        .output_pins =
        {
            2 , // channel 0
            3 , // channel 1
            12 , // channel 2
            9   // channel 3
        },
        .irq_priority = APP_IRQ_PRIORITY_LOW,
        .base_clock   = NRF_PWM_CLK_8MHz,
        .count_mode   = NRF_PWM_MODE_UP,
        .top_value    = 400,
        .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
        .step_mode    = NRF_PWM_STEP_AUTO
    };
    nrf_drv_pwm_init(&m_pwm1, &config1, NULL);

    m_motor2_seq_values.channel_0 = 0;
    m_motor2_seq_values.channel_1 = 0;
    m_motor2_seq_values.channel_2 = 0;
    m_motor2_seq_values.channel_3 = 0;

    nrf_drv_pwm_simple_playback(&m_pwm1, &m_motor2_seq, 1,
        NRF_DRV_PWM_FLAG_LOOP);
}

void motor_driver_set(int channel, int top) {
    switch (channel) {
        case 0:
            m_motor1_seq_values.channel_0 = top;
            break;
        case 1:
            m_motor1_seq_values.channel_1 = top;
            break;
        case 2:
            m_motor1_seq_values.channel_2 = top;
            break;
        case 3:
            m_motor1_seq_values.channel_3 = top;
            break;
        case 4:
            m_motor2_seq_values.channel_0 = top;
            break;
        case 5:
            m_motor2_seq_values.channel_1 = top;
            break;
        case 6:
            m_motor2_seq_values.channel_2 = top;
            break;
        case 7:
            m_motor2_seq_values.channel_3 = top;
            break;
    }
}
