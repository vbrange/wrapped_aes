/*
 * SPDX-FileCopyrightText: 2020 Efabless Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * SPDX-License-Identifier: Apache-2.0
 */

// This include is relative to $CARAVEL_PATH (see Makefile)
#include "verilog/dv/caravel/defs.h"
#include "verilog/dv/caravel/stub.c"

/*
	MPRJ LA Test:
		- Sets counter clk through LA[64]
		- Sets counter rst through LA[65] 
		- Observes count value for five clk cycle through LA[31:0]
*/

int clk = 0;
int i;

int bufeq(const uint8_t* p, const uint8_t* q)
{
    uint32_t* pp = (uint32_t*)p;
    uint32_t* qq = (uint32_t*)q;
    int i;
    for(i=0; i<4; i++)
    {
        if (*pp++ != *qq++)
            return 0;
    }
    return 1;
}

void bufcpy(uint8_t* p, const uint8_t* q)
{
    uint32_t* pp = (uint32_t*)p;
    uint32_t* qq = (uint32_t*)q;
    int i;
    for(i=0; i<4; i++)
    {
        *pp++ = *qq++;
    }
}

void main()
{
    /* Set up the housekeeping SPI to be connected internally so	*/
    /* that external pin changes don't affect it.			*/

    reg_spimaster_config = 0xa002; // Enable, prescaler = 2,
                                   // connect to housekeeping SPI

    // Connect the housekeeping SPI to the SPI master
    // so that the CSB line is not left floating.  This allows
    // all of the GPIO pins to be used for user functions.

    // All GPIO pins are configured to be output
    // Used to flad the start/end of a test

    reg_mprj_io_31 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_30 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_29 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_28 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_27 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_26 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_25 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_24 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_23 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_22 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_21 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_20 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_19 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_18 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_17 = GPIO_MODE_MGMT_STD_OUTPUT;
    reg_mprj_io_16 = GPIO_MODE_MGMT_STD_OUTPUT;

    reg_mprj_io_15 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_14 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_13 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_12 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_11 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_10 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_9 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_8 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_7 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_5 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_4 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_3 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_2 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_1 = GPIO_MODE_USER_STD_OUTPUT;
    reg_mprj_io_0 = GPIO_MODE_USER_STD_OUTPUT;

    /* Apply configuration */
    reg_mprj_xfer = 1;
    while (reg_mprj_xfer == 1)
        ;

    // Configure All LA probes as inputs to the cpu
    reg_la0_oenb = reg_la0_iena = 0xFFFFFFFF; // [31:0]
    reg_la1_oenb = reg_la1_iena = 0xFFFFFFFF; // [63:32]
    reg_la2_oenb = reg_la2_iena = 0xFFFFFFFF; // [95:64]
    reg_la3_oenb = reg_la3_iena = 0xFFFFFFFF; // [127:96]

    // Flag start of the test
    reg_mprj_datal = 0x41410000;

    int tests_good = 0;

    *(int*) 0x30000004 = 1;
    *(int*) 0x30000008 = 1;
    while (!((*(int*) 0x30000000) & 1));

    static const uint8_t aes_128_vector0_ref[] = {
        0x66, 0xe9, 0x4b, 0xd4, 0xef, 0x8a, 0x2c, 0x3b, 0x88, 0x4c, 0xfa, 0x59, 0xca, 0x34, 0x2b, 0x2e
    };

    if (bufeq((uint8_t*) 0x30000030, aes_128_vector0_ref))
        tests_good++;

    static const uint8_t aes_128_vector1_key[] = {
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f
    };
    static const uint8_t aes_128_vector1_in[] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
    };
    static const uint8_t aes_128_vector1_ref[] = {
        0x9c, 0x54, 0xd5, 0x71, 0x70, 0x2c, 0xfa, 0x0f, 0x03, 0xf3, 0x62, 0x15, 0x67, 0x6b, 0xab, 0x78
    };

    bufcpy((uint8_t*) 0x30000010, aes_128_vector1_key);
    bufcpy((uint8_t*) 0x30000020, aes_128_vector1_in);

    *(int*) 0x30000004 = 1;
    *(int*) 0x30000008 = 1;
    while (!((*(int*) 0x30000000) & 1));

    if (bufeq((uint8_t*) 0x30000030, aes_128_vector1_ref))
        tests_good++;

    reg_mprj_datal = 0x80000000 | (tests_good << 16);

    while (1)
        ;
}
