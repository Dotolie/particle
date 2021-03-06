/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sgp30.h"
#include "cm1109.h"
#include "pm5000.h"
#include "htu31d.h"
#include "debug.h"

#include <inttypes.h>  // PRIu64
#include <stdio.h>     // printf
#include <unistd.h>    // sleep
#include <zmq.h>
#include <string>
#include <stdlib.h>

/* TO USE CONSOLE OUTPUT (printf) AND WAIT (sleep) YOU MAY NEED TO ADAPT THE
 * INCLUDES ABOVE OR DEFINE THEM ACCORDING TO YOUR PLATFORM.
 * #define printf(...)
 * #define sleep(...)
 */
 
using namespace std;

tSGP30 g_SGP30 = {0,};
tHTU31D g_HTU31D = {0,};
tCM1109 g_CM1109 = {0,};
tPM5000 g_PM5000 = {0,};




int main(void) {
    uint16_t i = 0;
    int16_t err;
    uint16_t tvoc_ppb, co2_eq_ppm;
    uint32_t iaq_baseline;
    uint16_t ethanol_raw_signal, h2_raw_signal;
    uint16_t feature_set_version;
    uint8_t product_type;
    uint64_t serial_id;
	
	uint8_t szBuf[16];
	char zBuff[1024];
	uint16_t sSerial[6];
	uint16_t sCo2;
	uint8_t cStatus;
	float fTemp;
	float fHum;
	int nP03,nP05,nP10,nP25,nP50,nP100;
		
	//  Prepare our context and publisher
    void *context = zmq_ctx_new();
    void *publisher = zmq_socket (context, ZMQ_PUB);
    zmq_bind (publisher, "tcp://*:3333");
	
#if 0
    const char* driver_version = sgp30_get_driver_version();
    if (driver_version) {
        printf("SGP30 driver version %s\n", driver_version);
    } else {
        printf("fatal: Getting driver version failed\n");
        return -1;
    }
#endif

    /* Initialize I2C bus */
    sensirion_i2c_init();

#if 1
    /* Busy loop for initialization. The main loop does not work without
     * a sensor. */
    int16_t probe;
    while (1) {
        probe = sgp30_probe();

        if (probe == STATUS_OK)
            break;

        if (probe == SGP30_ERR_UNSUPPORTED_FEATURE_SET)
            DBG("Your sensor needs at least feature set version 1.0 (0x20)\n");

        DBG_E_R("SGP sensor probing failed\n");
        sensirion_sleep_usec(1000000);
    }

    printf("SGP sensor probing successful\n");


    err = sgp30_get_feature_set_version(&feature_set_version, &product_type);
    if (err == STATUS_OK) {
        printf("Feature set version: %u\n", feature_set_version);
        printf("Product type: %u\n", product_type);
    } else {
        printf("sgp30_get_feature_set_version failed!\n");
    }

    err = sgp30_get_serial_id(&serial_id);
    if (err == STATUS_OK) {
        printf("SerialID: %" PRIu64 "\n", serial_id);
    } else {
        printf("sgp30_get_serial_id failed!\n");
    }

    /* Read gas raw signals */
    err = sgp30_measure_raw_blocking_read(&ethanol_raw_signal, &h2_raw_signal);
    if (err == STATUS_OK) {
        /* Print ethanol raw signal and h2 raw signal */
        printf("Ethanol raw signal: %u\n", ethanol_raw_signal);
        printf("H2 raw signal: %u\n", h2_raw_signal);
    } else {
        printf("error reading raw signals\n");
    }

    /* Consider the two cases (A) and (B):
     * (A) If no baseline is available or the most recent baseline is more than
     *     one week old, it must discarded. A new baseline is found with
     *     sgp30_iaq_init() */
    err = sgp30_iaq_init();
    if (err == STATUS_OK) {
        printf("sgp30_iaq_init done\n");
    } else {
        printf("sgp30_iaq_init failed!\n");
    }
    /* (B) If a recent baseline is available, set it after sgp30_iaq_init() for
     * faster start-up */
    /* IMPLEMENT: retrieve iaq_baseline from presistent storage;
     * err = sgp30_set_iaq_baseline(iaq_baseline);
     */

	g_SGP30.m_sFeature_version = feature_set_version;
	g_SGP30.m_cProduct_type = product_type;
	g_SGP30.m_llSerial_id = serial_id;
	g_SGP30.m_sEthanol_raw_signal = ethanol_raw_signal;
	g_SGP30.m_sH2_raw_signal = h2_raw_signal;


#endif

#if 1
	err = cm1109_read_serial_number(sSerial);
	printf("ser err=%d, %d-%d-%d-%d-%d\r\n", err, sSerial[0],sSerial[1],sSerial[2],sSerial[3],sSerial[4]);

	err= cm1109_get_software_version(szBuf);
	printf("sw err=%d, %s\r\n",err,szBuf);

	err= cm1109_auto_zero_setting(szBuf);

//	err= cm1109_calibration(szBuf);

	err= cm1109_measuring_result( &sCo2, &cStatus);
	sleep(1);
#endif
#if 1
	pm5000_send(2);
	sleep(1);
#endif
	err = htu31d_reset();
	sleep(1);

	err = htu31d_readSerialNum(szBuf);
	err = htu31d_heaterOff();
	err = htu31d_readDiagnostic();
	err = htu31d_Convertion(3,3);
	sleep(1);


    /* Run periodic IAQ measurements at defined intervals */
    while (1) {
        /*
         * IMPLEMENT: get absolute humidity to enable humidity compensation
         * uint32_t ah = get_absolute_humidity(); // absolute humidity in mg/m^3
         * sgp30_set_absolute_humidity(ah);
         */
#if 1
        err = sgp30_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
        if (err == STATUS_OK) {
			g_SGP30.m_sTvoc_ppb = tvoc_ppb;
			g_SGP30.m_sCo2_eq_ppm = co2_eq_ppm;
		
            printf("tVOC : %dppb, CO2eq : %dppm\n", tvoc_ppb, co2_eq_ppm);

        } else {
            printf("error reading IAQ values\n");
        }

        /* Persist the current baseline every hour */
        if (++i % 3600 == 3599) {
            err = sgp30_get_iaq_baseline(&iaq_baseline);
            if (err == STATUS_OK) {
                /* IMPLEMENT: store baseline to presistent storage */
            }
        }
#endif

#if 1
		cm1109_measuring_result( &sCo2, &cStatus);
		printf("co2=%d, status=%d\r\n", sCo2, cStatus);
#endif

#if 1
		pm5000_read(&nP03, &nP05, &nP10, &nP25, &nP50, &nP100);
		printf("pm5000: 0.3u=%d, 0.5u=%d, 1.0u=%d, 2.5u=%d, 5u=%d, 10u=%d\r\n", nP03, nP05, nP10, nP25, nP50, nP100);
#endif


		err = htu31d_readTnRH(&fTemp, &fHum);
		err = htu31d_Convertion(3,3);

		string msg;
		msg = "{\"sgp30\": {\"version\":";
		msg += to_string(g_SGP30.m_sFeature_version);
		msg += ", \"type\":";
		msg += to_string(g_SGP30.m_cProduct_type);
		msg += ", \"serial\":";
		msg += to_string(g_SGP30.m_llSerial_id);
		msg += ", \"ethanol\":";
		msg += to_string(g_SGP30.m_sEthanol_raw_signal);
		msg += ", \"h2\":";
		msg += to_string(g_SGP30.m_sH2_raw_signal);
		msg += ", \"tvoc\":";
		msg += to_string(g_SGP30.m_sTvoc_ppb);
		msg += ", \"co2eq\":";
		msg += to_string(g_SGP30.m_sCo2_eq_ppm);
		msg += "},";

		msg += "\"pm5000\": {\"p03\":";
		msg += to_string(nP03);
		msg += ", \"p05\":";
		msg += to_string(nP05);
		msg += ", \"p10\":";
		msg += to_string(nP10);
		msg += ", \"p25\":";
		msg += to_string(nP25);
		msg += ", \"p50\":";
		msg += to_string(nP50);
		msg += ", \"p100\":";
		msg += to_string(nP100);
		msg += "}";

	printf("%s\r\n", msg.c_str());

        int rc = zmq_send(publisher, msg.c_str(), msg.size(), 0);
		if( rc == -1 ) 
			DBG_E_R("zmq_send::error=%d\r\n", zmq_errno());

        /* The IAQ measurement must be triggered exactly once per second (SGP30)
         * to get accurate values.
         */
        sleep(1);  // SGP30

    }

	zmq_close (publisher);
    zmq_term (context);

	return 0;
}
