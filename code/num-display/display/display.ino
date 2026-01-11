/**
 * Copyright © 2025 Karol Zmijewski

 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the “Software”), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,

 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <eeprom.h>

EEPROM eeprom;

void program_display() {
    const unsigned int MAX_BYTE_SIZE = 255;
    // Common Cathode decoding array for 7-segment display
    const byte DISPLAY_CC_DECODE_ARRAY[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
    // program decode array for 7-segment display
    // iterate over whole range of possible numbers (range from 0 to max size of 8bit number:(2^8)-1 which is 255)a
    int prog_process_str_last_byte_addr = 0;
    byte prog_prev_percent = 0;
    Serial.println("Programming display: ");
    for(unsigned int val = 0x00; val <= MAX_BYTE_SIZE; val++ ) {
        // notify about progress
        byte prog_progress_val = (byte)((val * 100) / MAX_BYTE_SIZE);
        if(prog_progress_val != prog_prev_percent) {
            Serial.print(".");
        }
        prog_prev_percent = prog_progress_val;
        // unities
        byte unity_val = (byte)(val % 10);
        byte unity_val_decode_sign = DISPLAY_CC_DECODE_ARRAY[unity_val];
        unsigned int unity_val_decode_sign_addr = val;
        eeprom.write(unity_val_decode_sign_addr, unity_val_decode_sign);
        // dozens
        byte dozen_val = (byte)((val / 10) % 10);
        byte dozen_val_decode_sign = DISPLAY_CC_DECODE_ARRAY[dozen_val];
        //   - shift by size of byte as a first byte is reserved for unity signs
        unsigned int dozen_val_decode_sign_addr = unity_val_decode_sign_addr + MAX_BYTE_SIZE + 1;
        eeprom.write(dozen_val_decode_sign_addr, dozen_val_decode_sign); 
        // hundreds
        byte hundred_val = (byte)((val / 100) % 10);
        byte hundred_val_decode_sign = DISPLAY_CC_DECODE_ARRAY[hundred_val];
        //   - shift by size of two byte as a first byte is reserved for unity signs and second byte for dozen signs
        unsigned int hundred_val_decode_sign_addr = dozen_val_decode_sign_addr + MAX_BYTE_SIZE + 1;
        eeprom.write(hundred_val_decode_sign_addr, hundred_val_decode_sign);
        // negative numbers
        if(val <= (unsigned int)(MAX_BYTE_SIZE / 2)) {
            // unities
            byte unity_neg_val = (byte)(val % 10);
            byte unity_neg_val_decode_sign = DISPLAY_CC_DECODE_ARRAY[unity_neg_val];
            // shift right by 6 * 256bit (first 3 are occupied by positive numbers (0, 255), next 3 by negative numbers (-127, 0)
            unsigned int unity_neg_val_decode_sign_addr = (unsigned int)(5 * (MAX_BYTE_SIZE + 1)) - (unsigned int)val;
            eeprom.write(unity_neg_val_decode_sign_addr, unity_neg_val_decode_sign);
            // dozens
            byte dozen_neg_val = (byte)((val / 10) % 10);
            byte dozen_neg_val_decode_sign = DISPLAY_CC_DECODE_ARRAY[dozen_neg_val];
            unsigned int dozen_neg_val_decode_sign_addr = unity_neg_val_decode_sign_addr + (unsigned int)(MAX_BYTE_SIZE + 1);
            eeprom.write(dozen_neg_val_decode_sign_addr, dozen_neg_val_decode_sign);
            // hundreds
            byte hundred_neg_val = (byte)((val / 100) % 10);
            byte hundred_neg_val_decode_sign = DISPLAY_CC_DECODE_ARRAY[hundred_neg_val];
            unsigned int hundred_neg_val_decode_sign_addr = dozen_neg_val_decode_sign_addr + (unsigned int)(MAX_BYTE_SIZE + 1);
            eeprom.write(hundred_neg_val_decode_sign_addr, hundred_neg_val_decode_sign);
        }
        // minus sign
        const byte MINUS_SIGN = 0x40;
        unsigned int minus_decode_sign_addr = (unsigned int)(7 * (MAX_BYTE_SIZE + 1)) + (unsigned int)val;
        eeprom.write(minus_decode_sign_addr, MINUS_SIGN);
    }
    Serial.println("\nProgramming done.");
}

void setup() {
    eeprom.begin();
    eeprom.writeAll(0x3F);
    program_display();
    eeprom.print(32, 2048);
}

void loop() {
}
