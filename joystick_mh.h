#ifndef __joyrstick_mh__
#define __joyrstick_mh__

#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>
#include <xgpio.h>
#include <xglob.h>
#include <xdts.h>

#define JOYSTICK_MAX_EVENTS 16
#define JOYSTICK_READ_DELAY_MS 50
#define JOYSTICK_SENS_THRESHOLD 150

typedef enum
{
    push,
    move
} joystick_event_type_t;

typedef struct
{
    joystick_event_type_t event_type;
    int axis;
    uint16_t axis_value;
} joystick_event_t;

typedef struct
{
    gpio_dt_spec_t *button;
    adc_dt_spec_t **axis[2];
    k_pipe_t *events;
} joystick_mh_t;

#define DEFINE_JOYSTICK_MH(__dt_label)                                                         \
    DEFINE_GPIO_AS(__dt_label, _##__dt_label##_btn);                                           \
    adc_dt_spec_t _##__dt_label##_x = ADC_DT_SPEC_GET_BY_IDX(L(__dt_label), 0);                \
    adc_dt_spec_t _##__dt_label##_y = ADC_DT_SPEC_GET_BY_IDX(L(__dt_label), 1);                \
    K_PIPE_DEFINE(__dt_label##_events, JOYSTICK_MAX_EVENTS * sizeof(joystick_event_t), ALIGN); \
    DEFINE_GPIO_CALLBACK(_##__dt_label##_btn);                                                 \
    joystick_mh_t __dt_label = {.events = NULL};                                               \
    K_THREAD_DEFINE(__dt_label##joystick_mh_reader_thread_id, 512, joystick_mh_reader_thread, &__dt_label, NULL, NULL, 5, 0, 0)

#define JOYSTICK_MH_INSTALL(__dt_label)       \
    __dt_label.events = &__dt_label##_events; \
    __dt_label.button = &_##__dt_label##_btn; \
    __dt_label.axis[0] = &_##__dt_label##_x;  \
    __dt_label.axis[1] = &_##__dt_label##_y

int joystick_mh_get_event(joystick_mh_t *joystick, joystick_event_t *evt, k_timeout_t timeout);
void joystick_mh_reader_thread(joystick_mh_t *joystick);

#endif