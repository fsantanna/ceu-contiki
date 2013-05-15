#include <stdint.h>
typedef int64_t  s64;
typedef uint64_t u64;
typedef int32_t  s32;
typedef uint32_t u32;
typedef int16_t  s16;
typedef uint16_t u16;
typedef int8_t    s8;
typedef uint8_t   u8;

#include "contiki.h"
#include "_ceu_defs.h"

/****************************************************************************/

#ifdef CEU_FUN_printf
#include <stdio.h>
#endif

/****************************************************************************/

#ifdef CEU_WCLOCKS

#include "sys/ctimer.h"

struct ctimer ceu_contiki_timer;

clock_time_t ceu_contiki_timer_old;

static void ceu_contiki_timer_cb (void* ptr) {
    clock_time_t now = clock_time();
    s32 dt = now - ceu_contiki_timer_old;
    ceu_contiki_timer_old = now;
    ceu_go_wclock(dt*1000/CLOCK_SECOND*1000);
}
 
#define ceu_out_wclock(us)  {                       \
    if ((us) != CEU_WCLOCK_INACTIVE)                \
        ctimer_set(&ceu_contiki_timer, us/1000*CLOCK_SECOND/1000, \
                    ceu_contiki_timer_cb, NULL);    \
    }

#endif  /* CEU_WCLOCKS */

/****************************************************************************/

#ifdef CEU_FUN_leds_on

#include "dev/leds.h"

#endif  /* CEU_LEDS */

/****************************************************************************/

#ifdef CEU_IN_SIMPLE_UDP_RECEIVE

#include "net/uip.h"
#include "net/uip-ds6.h"
#include "simple-udp.h"

typedef struct simple_udp_connection simple_udp_connection;

typedef struct {
    struct simple_udp_connection*   c;
    const uip_ipaddr_t*             sender_addr;
    uint16_t                        sender_port;
    const uip_ipaddr_t*             receiver_addr;
    uint16_t                        receiver_port;
    const uint8_t*                  data;
    uint16_t                        datalen;
} simple_udp_receive_t;

static void simple_udp_receive_cb (
    struct simple_udp_connection*   c,
    const uip_ipaddr_t*             sender_addr,
    uint16_t                        sender_port,
    const uip_ipaddr_t*             receiver_addr,
    uint16_t                        receiver_port,
    const uint8_t*                  data,
    uint16_t                        datalen
) {
    simple_udp_receive_t p = { c, sender_addr, sender_port, receiver_addr,
                               receiver_port, data, datalen };
    ceu_go_event(CEU_IN_SIMPLE_UDP_RECEIVE, &p);
}

#define simple_udp_register_(a,b,c,d) \
        simple_udp_register(a,b,c,d, simple_udp_receive_cb)

#endif  /* CEU_IN_SIMPLE_UDP_RECEIVE */

/****************************************************************************/

#include "_ceu_code.cceu"

PROCESS(ceu_contiki, "MAIN");
AUTOSTART_PROCESSES(&ceu_contiki);

PROCESS_THREAD(ceu_contiki, ev, data)
{
    PROCESS_BEGIN();

#ifdef CEU_WCLOCKS
    ceu_contiki_timer_old = clock_time();
#endif

    ceu_go_init();

    PROCESS_END();
}
