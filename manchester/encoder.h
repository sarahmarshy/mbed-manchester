/* Manchester Encoder Library 
 * Copyright (c) 2018 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MAN_ENCODING_H
#define MAN_ENCODING_H

#include "mbed.h"

class ManchesterEncoder {
public:
    ManchesterEncoder(PinName pin, int baud, bool idle_state = 1) : _output_pin(pin) {
        _output_pin = idle_state;
        // Half bit time in seconds
        float time_s = 1.0/(2.0*(float)baud);
        // Half bit time in microseconds
        _half_bit_time =  (int)(time_s * 1000000.0);
    }
    
    void send(uint16_t data_out) {
        // We don't want to be preempted because this is time sensitive 
        core_util_critical_section_enter();
        // Send start condition
        _output_pin = 0;
        wait_us(_half_bit_time);
        _output_pin = 1;
        wait_us(_half_bit_time);
        // Send the data
        for(int i = 0; i < 16; i++) {
            // Send the inverted MSb
            _output_pin = !((bool)(data_out & 0x8000));
            wait_us(_half_bit_time);
            // Send the actual MSb
            _output_pin = (bool)(data_out & 0x8000);
            wait_us(_half_bit_time);
            // Shift to next bit
            data_out = data_out << 1;
        }
        // Send the stop condition
        _output_pin = 1;
        wait_us(_half_bit_time*4);
        core_util_critical_section_exit();
    } 

private:
    // Pin to output encoded data
    DigitalOut _output_pin;
    // Half the time for each bit (1/(2*baud)) 
    int _half_bit_time;
};

#endif
 
