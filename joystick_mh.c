
#include <joystick_mh.h>
#include <xglob.h>
#include <xlog.h>

#define AXIS 2

void joystick_mh_reader_thread(joystick_mh_t *joystick)
{
    X_WAIT_FOR(joystick->events);

    uint16_t buf, diff;
    size_t i, written;
    struct adc_sequence sequence = {
        .buffer = &buf,
        .buffer_size = sizeof(buf),
    };
    uint16_t joystick_state[AXIS] = {0, 0};

    while (true)
    {
        for (size_t i = 0; i < AXIS; i++)
        {
            SURE(adc_sequence_init_dt, joystick->axis[i], &sequence);
            SURE(adc_read_dt, joystick->axis[i], &sequence);
            diff = ABS(joystick_state[i], buf);
            if (diff >= JOYSTICK_SENS_THRESHOLD)
            {
                joystick_event_t evt = {
                    .event_type = JOYSTICK_MOVE,
                    .axis = i,
                    .axis_value = buf};

                SURE(k_pipe_put, joystick->events, &evt, sizeof(evt), &written, sizeof(evt), K_FOREVER);
                joystick_state[i] = buf;
            }
        }

        k_msleep(JOYSTICK_READ_DELAY_MS);
    }
}

int joystick_mh_get_event(joystick_mh_t *joystick, joystick_event_t *evt, k_timeout_t timeout)
{
    size_t read;
    return k_pipe_get(joystick->events, evt, sizeof(joystick_event_t), &read, sizeof(joystick_event_t), timeout);
}