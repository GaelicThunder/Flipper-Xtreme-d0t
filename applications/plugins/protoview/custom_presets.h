#include <cc1101.h>

/* This is how to configure registers MDMCFG3 and MDMCFG4.
 *
 * Data rate kBaud setting:
 *
 * MDMCFG3 is the data rate mantissa, the exponent is in MDMCFG4,
 * last 4 bits of the register.
 *
 * The rate (assuming 26Mhz crystal) is calculated as follows:
 *
 * ((256+MDMCFG3)*(2^MDMCFG4:0..3bits)) / 2^28 * 26000000.
 *
 * For instance for the default values of MDMCFG3 (34) and MDMCFG4 (12):
 *
 * ((256+34)*(2^12))/(2^28)*26000000 = 115051.2688000000, that is 115KBaud
 *
 * Bandwidth filter setting:
 *
 * BW filter as just 16 possibilities depending on how the first nibble
 * (first 4 bits) of the MDMCFG4 bits are set. Instead of providing the
 * formula, it is simpler to show all the values of the nibble and the
 * corresponding bandwidth filter.
 *
 * 0 812khz
 * 1 650khz
 * 2 541khz
 * 3 464khz
 * 4 406khz
 * 5 325khz
 * 6 270khz
 * 7 232khz
 * 8 203khz
 * 9 162khz
 * a 135khz
 * b 116khz
 * c 102khz
 * d 82 khz
 * e 68 khz
 * f 58 khz
 */

/* 20 KBaud, 2FSK, 28.56 kHz deviation, 325 Khz bandwidth filter. */
static uint8_t protoview_subghz_tpms1_async_regs[][2] = {
    /* GPIO GD0 */
    {CC1101_IOCFG0, 0x0D}, // GD0 as async serial data output/input

    /* Frequency Synthesizer Control */
    {CC1101_FSCTRL1, 0x06}, // IF = (26*10^6) / (2^10) * 0x06 = 152343.75Hz

    /* Packet engine */
    {CC1101_PKTCTRL0, 0x32}, // Async, continious, no whitening
    {CC1101_PKTCTRL1, 0x04},

    // // Modem Configuration
    {CC1101_MDMCFG0, 0x00},
    {CC1101_MDMCFG1, 0x02},
    {CC1101_MDMCFG2, 0x04}, // Format 2-FSK/FM, No preamble/sync, Disable (current optimized). Other code reading TPMS uses GFSK, but should be the same when in RX mode.
    {CC1101_MDMCFG3, 0x93}, // Data rate is 20kBaud
    {CC1101_MDMCFG4, 0x59}, // Rx bandwidth filter is 325 kHz
    {CC1101_DEVIATN, 0x41}, // Deviation 28.56 kHz

    /* Main Radio Control State Machine */
    {CC1101_MCSM0, 0x18}, // Autocalibrate on idle-to-rx/tx, PO_TIMEOUT is 64 cycles(149-155us)

    /* Frequency Offset Compensation Configuration */
    {CC1101_FOCCFG,
     0x16}, // no frequency offset compensation, POST_K same as PRE_K, PRE_K is 4K, GATE is off

    /* Automatic Gain Control */
    {CC1101_AGCCTRL0,
     0x91}, //10 - Medium hysteresis, medium asymmetric dead zone, medium gain ; 01 - 16 samples agc; 00 - Normal AGC, 01 - 8dB boundary
    {CC1101_AGCCTRL1,
     0x00}, // 0; 0 - LNA 2 gain is decreased to minimum before decreasing LNA gain; 00 - Relative carrier sense threshold disabled; 0000 - RSSI to MAIN_TARGET
    {CC1101_AGCCTRL2, 0x07}, // 00 - DVGA all; 000 - MAX LNA+LNA2; 111 - MAIN_TARGET 42 dB

    /* Wake on radio and timeouts control */
    {CC1101_WORCTRL, 0xFB}, // WOR_RES is 2^15 periods (0.91 - 0.94 s) 16.5 - 17.2 hours

    /* Frontend configuration */
    {CC1101_FREND0, 0x10}, // Adjusts current TX LO buffer
    {CC1101_FREND1, 0x56},

    /* End  */
    {0, 0},
};

/* 40 KBaud, 2FSK, 19 kHz deviation, 102 Khz bandwidth filter. */
static uint8_t protoview_subghz_tpms2_async_regs[][2] = {
    /* GPIO GD0 */
    {CC1101_IOCFG0, 0x0D}, // GD0 as async serial data output/input

    /* Frequency Synthesizer Control */
    {CC1101_FSCTRL1, 0x06}, // IF = (26*10^6) / (2^10) * 0x06 = 152343.75Hz

    /* Packet engine */
    {CC1101_PKTCTRL0, 0x32}, // Async, continious, no whitening
    {CC1101_PKTCTRL1, 0x04},

    // // Modem Configuration
    {CC1101_MDMCFG0, 0x00},
    {CC1101_MDMCFG1, 0x02},
    {CC1101_MDMCFG2, 0x04}, // Format 2-FSK/FM, No preamble/sync, Disable (current optimized). Other code reading TPMS uses GFSK, but should be the same when in RX mode.
    {CC1101_MDMCFG3, 0x93}, // Data rate is 40kBaud
    {CC1101_MDMCFG4, 0x6A}, // 6 = BW filter 270kHz, A = Data rate exp
    {CC1101_DEVIATN, 0x41}, // Deviation 19.042 kHz

    /* Main Radio Control State Machine */
    {CC1101_MCSM0, 0x18}, // Autocalibrate on idle-to-rx/tx, PO_TIMEOUT is 64 cycles(149-155us)

    /* Frequency Offset Compensation Configuration */
    {CC1101_FOCCFG,
     0x16}, // no frequency offset compensation, POST_K same as PRE_K, PRE_K is 4K, GATE is off

    /* Automatic Gain Control */
    {CC1101_AGCCTRL0,
     0x91}, //10 - Medium hysteresis, medium asymmetric dead zone, medium gain ; 01 - 16 samples agc; 00 - Normal AGC, 01 - 8dB boundary
    {CC1101_AGCCTRL1,
     0x00}, // 0; 0 - LNA 2 gain is decreased to minimum before decreasing LNA gain; 00 - Relative carrier sense threshold disabled; 0000 - RSSI to MAIN_TARGET
    {CC1101_AGCCTRL2, 0x07}, // 00 - DVGA all; 000 - MAX LNA+LNA2; 111 - MAIN_TARGET 42 dB

    /* Wake on radio and timeouts control */
    {CC1101_WORCTRL, 0xFB}, // WOR_RES is 2^15 periods (0.91 - 0.94 s) 16.5 - 17.2 hours

    /* Frontend configuration */
    {CC1101_FREND0, 0x10}, // Adjusts current TX LO buffer
    {CC1101_FREND1, 0x56},

    /* End  */
    {0, 0},
};

