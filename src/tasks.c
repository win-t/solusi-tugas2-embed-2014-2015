// Pada tugas ini, multitasking dilakukan dengan metode
// non Nonpreemptive multitasking, atau cooperative thread,
// http://en.wikipedia.org/wiki/Nonpreemptive_multitasking

#include <avr/io.h>

#define LED_DDR              DDRB
#define LED_PORT             PORTB

#define BTN_DDR              DDRA
#define BTN_PORT             PORTA
#define BTN_PIN              PINA
#define BTN_SLOW             1
#define BTN_FAST             0

static int16_t led_task();
static int16_t tombol_task();

struct {
    int16_t counting;
    int16_t (*function)();
} all_task[] = {
    { 0, led_task },        // 0
    { 0, tombol_task },     // 1
};

int task_count() {
    return (sizeof(all_task)/sizeof(all_task[0]));
}


static int16_t led_delay;   // delay dari shitf led
static uint8_t slow_btn;    // indikasi jika button slow ditekan
static uint8_t fast_btn;    // indikasi jika button fast ditekan

void task_setup() {

    LED_DDR = 0xff;
    LED_PORT = 0xfe;

    BTN_DDR &= ~(_BV(BTN_SLOW) | _BV(BTN_FAST));
    BTN_PORT |= (_BV(BTN_SLOW) | _BV(BTN_FAST));

    led_delay = 300; // defaultnya adalah 300ms
    slow_btn = fast_btn = 0; // defaultnnya belum ditekan
}

static int16_t tombol_task() {
    // dapatkan kondisi button sekarang
    uint8_t current_slow_btn = bit_is_clear(BTN_PIN, BTN_SLOW);
    uint8_t current_fast_btn = bit_is_clear(BTN_PIN, BTN_FAST);

    // hanya proses jika berubah dari tidak-ditekan ke ditekan
    if(!slow_btn && current_slow_btn) {
        // kurangi kecepatan dengan menambah delay
        led_delay += 100;
        if(led_delay > 500) led_delay = 500;

        // update counting dari led_task
        if(all_task[0].counting > led_delay) {
            all_task[0].counting = led_delay;
        }
    }

    // hanya proses jika berubah dari tidak-ditekan ke ditekan
    if(!fast_btn && current_fast_btn) {
        // tambah kecepatan dengan mengurangi delay
        led_delay -= 100;
        if(led_delay < 10) led_delay = 100;

        // update counting dari led_task
        if(all_task[0].counting > led_delay) {
            all_task[0].counting = led_delay;
        }
    }

    // update kondisi button untuk iterasi selanjutnya
    slow_btn = current_slow_btn;
    fast_btn = current_fast_btn;

    // delay 10ms untuk mengatasi contac bounce,
    // http://en.wikipedia.org/wiki/Switch#Contact_bounce
    return 10;
}

static int16_t led_task() {
    static uint8_t is_left = 1;
    if(is_left) {
        // shift left
        LED_PORT = (LED_PORT << 1) | _BV(0);
        if(LED_PORT == 0x7f) is_left = 0;
    } else {
        // shift right
        LED_PORT = (LED_PORT >> 1) | _BV(7);
        if(LED_PORT == 0xfe) is_left = 1;
    }

    // delay sesuai dengan speed sekarang
    return led_delay;
}
