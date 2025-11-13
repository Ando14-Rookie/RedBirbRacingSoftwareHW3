/*
File: q1.cpp
Triggered when a specific CAN frame is received
    Analog read PIN_PC0,
    Send the reading over CAN.
While this happens, make an LED turn on to indicate activity.
    This LED is connected to PIN_PD2.
The CAN frames are received and sent via an MCP2515 CAN controller.
    It is connected via SPI with CS on PIN_PB2.
    The crystal is 20MHz and the CAN speed is 500kbps.
*/

// include the necessary libraries
#include <mcp2515.h>

// create MCP2515 object
/* todo 1 */

// Assign PB_12 the value 10 (digital pin 10)
const int CS_PIN = 10; 
// Assign PB_0 the value 8 (digital pin 8)
const int LED_PIN = 8; 
// Creating an MCP2515 object at PIN_PB2
MCP2515 canName(CS_PIN);

void setup() {
    // set the pinMode on the input pin
    pinMode(CS_PIN, INPUT);
    // set the pinMode on the LED pin
    pinMode(LED_PIN, INPUT);
    // reset command to the MCP2515 CAN controller
    canName.reset();
    // Set the MCP2515's can communication speed and crystal frequency
    // Initialize the CAN controller at 500kbps
    canName.setBitrate(CAN_500KBPS, MCP_16MHZ);
    // Allow CAN to send and receive messages on the physical CAN bus
    canName.setNormalMode();
}

void loop() {
    // create CAN frame objects
    // make one for receiving, another for transmitting
    struct can_frame txFrame;
    struct can_frame rxFrame;
    /* todo 2 */

    // check if a CAN frame is received
    // Pass the received message to rxFrame
    if (canName.readMessage(&rxFrame) == MCP2515::ERROR_OK) {
        // check if the received frame is of ID 0x420 and first data byte is 0x69
        if ((rxFrame.can_id == 0x420) && (rxFrame.data[0] == 0x69)) {
            // turn on the LED to indicate activity
            digitalWrite(LED_PIN, HIGH);

            // read the analog value from the input pin and store it as a variable
            // Reading the analog value from the PIN_PC0
            int analogValue = analogRead(A0); // assuming PC0 = analaog input A0

            // prepare the CAN frame to send
            txFrame.can_id = 0x690; // replace with desired send ID
            txFrame.can_dlc = 2; // data length code
            txFrame.data[0] = (analogValue >> 8) & 0xFF; // high byte
            txFrame.data[1] = analogValue & 0xFF; // low byte

            // send the CAN frame
            // Sending it over the CAN bus using the MCP2515 object
            canName.sendMessage(&txFrame);

            // turn off the LED after sending
            digitalWrite(LED_PIN, LOW);
        }
    }
}