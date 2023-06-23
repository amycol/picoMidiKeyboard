#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/sync.h"

#include <string.h>
#include "bsp/board.h"
#include "tusb.h"
#include "tusb_config.h"

void main2();
void notes_task();

int values[4][8] = {{0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0}};
int notes[32];
int lastNotes[32];

void initBlink() {
    gpio_put(25, 0);
    sleep_ms(100);
    gpio_put(25, 1);
    sleep_ms(100);
}

int main() {
    //stdio_init_all();
    
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);
    sleep_ms(500);
    void midi_task();
    void led_blinking_task(void);
    int i;
    
    board_init();
    tud_init(BOARD_TUD_RHPORT); // initialize tinyusb stack
    printf("Core 0: Init complete.\n");
    initBlink();
    printf("Core 0: Starting Core 1...\n");
    multicore_launch_core1(main2);
    sleep_ms(1000); //Replace with wait for Core 1 init

    while(1) // the mainloop
    {
        tud_task(); // device task
        led_blinking_task();
        notes_task();
        midi_task();
    }
}

void main2() {
    printf("Core 1: Core 1 starting...\n");
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    gpio_put(25, 1);
    int rows[] = {2, 3, 4, 5};
    int columns[] = {6, 7, 8, 9, 10, 11, 12, 13};
    //int values[4][8] = {{0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0},{0, 0, 0, 0, 0, 0, 0, 0}};
    int i, j;
    for (i = 0; i < 8; i = i + 1) {
        gpio_init(columns[i]);
        gpio_set_dir(columns[i], GPIO_OUT);
        for (j = 0; j < 4; j = j + 1) {
            gpio_init(rows[j]);
            gpio_set_dir(rows[j], GPIO_IN);
        }
    }
    printf("Core 1: Init complete.\n");
    sleep_ms(250);
    initBlink();
    initBlink();
    while (true) {
        for (i = 0; i < 8; i = i + 1) {
            gpio_put(columns[i], 1);
            for (j = 0; j < 4; j = j + 1) {
                sleep_ms(1);
                printf("%d\n", values[0][0]);
                if (gpio_get(rows[j])) {
                    /**printf("%d%d\n", j, i);*/
                    values[j][i] = 1;
                } else {
                    values[j][i] = 0;
                }
            }
            gpio_put(columns[i], 0);
        }
    }
    
}

void notes_task() {
    //memcpy(lastNotes, notes, 32);
    for (int i = 0; i < 8; i = i + 1) {

        for (int j = 0; j < 4; j = j + 1) {
            if (values[j][i]) {
                notes[(((j*8)+i)*-1)+31] = 1;
            }else {
                notes[(((j*8)+i)*-1)+31] = 0;
            }
        }
    }
}

void midi_task() {
    static uint32_t start_ms = 0;

    uint8_t const cable_num = 0; // MIDI jack associated with USB endpoint
    uint8_t const channel   = 0; // 0 for channel 1

    // The MIDI interface always creates input and output port/jack descriptors
    // regardless of these being used or not. Therefore incoming traffic should be read
    // (possibly just discarded) to avoid the sender blocking in IO
    uint8_t packet[4];
    while ( tud_midi_available() ) tud_midi_packet_read(packet);

    // send note periodically
    if (board_millis() - start_ms < 1) return; // not enough time
    start_ms += 1;

    int i;
    int j;
    for (i = 0; i < 32; i = i + 1) {
        if (notes[i]) {
            if (lastNotes[i] == 0) {
                uint8_t note_on[3] = { 0x90 | channel, i + 53, 95};
                tud_midi_stream_write(cable_num, note_on, 3);
                lastNotes[i] = 1;
            }
        } else if (lastNotes[i]) {
            uint8_t note_off[3] = { 0x80 | channel, i + 53, 0};
            tud_midi_stream_write(cable_num, note_off, 3);
            lastNotes[i] = 0;
        }
    }
}



/* Blink pattern
 * - 250 ms  : device not mounted
 * - 1000 ms : device mounted
 * - 2500 ms : device is suspended
 */
enum  {
  BLINK_NOT_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_NOT_MOUNTED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;

  board_led_write(led_state);
  led_state = 1 - led_state; // toggle
}