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
    ManchesterEncoder(PinName pin, int time_us, bool idle_state = 1) : out(pin) {
        out = idle_state;
        wait_time = time_us;
    }
    
    void send(uint16_t data_out) {
        core_util_critical_section_enter();
        // Send start condition
        out = 0;
        wait_us(wait_time);
        out = 1;
        wait_us(wait_time);
        // Send the data
        for(int i = 0; i < 16; i++) {
            // Send the inverted MSb
            out = !((bool)(data_out & 0x8000));
            wait_us(wait_time);
            // Send the actual MSb
            out = (bool)(data_out & 0x8000);
            wait_us(wait_time);
            // Shift to next bit
            data_out = data_out << 1;
        }
        // Send the stop condition
        out = 1;
        wait_us(wait_time*4);
        core_util_critical_section_exit();
    } 

private:
    DigitalOut out;
    int wait_time;
};

#endif
 
