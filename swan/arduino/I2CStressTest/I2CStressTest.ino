//
// Copyright 2019 Blues Inc.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.
//
// This example does the same function as the basic "Using Library" example, but demonstrates
// how best to use the Notecard API when your app uses either a) higher-frequency sampling, or
// b) binary samples.
//
// When using the standard Notecard "note.add" request for sending data to the service through
// outbound queues (notefiles ending in .qo), the Notecard limits the number of "pending notes"
// (the number of notes waiting to be sent to the service) to 100, because under these common
// circumstances the pending notes are all buffered in memory.
//
// When a customer application generates data at a higher frequency, or a longer sync period,
// such that more than 100 notes will be "waiting" to be uploaded, the developer should use
// a feature known as the "Note Template".  An application using a Note Template declares
// once, up-front, what the JSON object will 'look like' in terms of JSON fields, data types,
// and data length.  By doing this, the Notecard can store the data for each "note.add"
// directly into a region of Flash storage permitting more than 100 notes to be pending
// for subsequent upload.
//
// This example shows how a developer would declare and set a template, both for their own
// JSON object as well as for their own binary payload.  (If a binary payload is not needed,
// it can simply be eliminated from the example.)
//

#include <Notecard.h>
#include <Wire.h>

#include <stdlib.h>

// Note that both of these definitions are optional; just prefix either line with // to remove it.
//  Remove serialNotecard if you wired your Notecard using I2C SDA/SCL pins instead of serial RX/TX
//  Remove serialDebug if you don't want the Notecard library to output debug information
// #define serialNotecard Serial1
#define serialDebugOut Serial

// This is the unique Product Identifier for your device.  This Product ID tells the Notecard what
// type of device has embedded the Notecard, and by extension which vendor or customer is in charge
// of "managing" it.  In order to set this value, you must first register with notehub.io and
// "claim" a unique product ID for your device.  It could be something as simple as as your email
// address in reverse, such as "com.gmail.smith.lisa.test-device" or "com.outlook.gates.bill.demo"
#define myProductID "com.your-company.your-name:your_project"
Notecard notecard;

// A sample binary object, just for binary payload simulation
struct myBinaryPayload {
    double temp;
    double voltage;
};

// One-time Arduino initialization
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // Set up for debug output.
#ifdef serialDebugOut
    delay(2500);
    serialDebugOut.begin(115200);
    notecard.setDebugOutputStream(serialDebugOut);
#endif

    // Initialize the physical I/O channel to the Notecard
#ifdef serialNotecard
    notecard.begin(serialNotecard, 9600);
#else
    Wire.begin();

    notecard.begin();
#endif

    
    // Create a template note that we will register.  This template note will look "similar" to
    // the notes that will later be added with note.add, in that the data types are used to
    // intuit what the ultimate field data types will be, and their maximum length.
    J* req = notecard.newRequest("note.add");
    if (req != NULL) {

        // Create the body for a template that will be used to send notes below
        J *body = JCreateObject();
        if (body != NULL) {

            // Define the JSON template
            JAddStringToObject(body, "status", "AAAAAAAAAAAA");   // maximum string length
            JAddNumberToObject(body, "temp", 1.1);          // floating point (double)
            JAddNumberToObject(body, "voltage", 1.1);       // floating point (double)
            JAddNumberToObject(body, "count", 1);         // integer

            // Add the body to the request
            JAddItemToObject(req, "body", body);
        }

        // Create a template of the payload that will be used to send notes below
        JAddNumberToObject(req, "length", sizeof(myBinaryPayload));

        // Register the template in the output queue notefile
        JAddStringToObject(req, "file", "sensors.qo");
        JAddBoolToObject(req, "template", true);
        notecard.sendRequest(req);
    }


}

// In the Arduino main loop which is called repeatedly, add outbound data every 15 seconds
void loop()
{

    // Rather than simulating a temperature reading, use a Notecard request to read the temp
    // from the Notecard's built-in temperature sensor.  We use NoteRequestResponse() to indicate
    // that we would like to examine the response of the transaction.  This method takes a "request" JSON
    // data structure as input, then processes it and returns a "response" JSON data structure with
    // the response.  Note that because the Notecard library uses malloc(), developers must always
    // check for NULL to ensure that there was enough memory available on the microcontroller to
    // satisfy the allocation request.
    double temperature = 0;
    J *rsp = notecard.requestAndResponse(notecard.newRequest("card.temp"));
    if (rsp != NULL) {
        temperature = JGetNumber(rsp, "value");
        notecard.deleteResponse(rsp);
    }

    // Do the same to retrieve the voltage that is detected by the Notecard on its V+ pin.
    double voltage = 0;
    rsp = notecard.requestAndResponse(notecard.newRequest("card.voltage"));
    if (rsp != NULL) {
        voltage = JGetNumber(rsp, "value");
        notecard.deleteResponse(rsp);
    }

    static int eventCounter = 0;

       serialDebugOut.print("loop");
       serialDebugOut.print(eventCounter);
       serialDebugOut.println();

    if ((eventCounter++ % 10)!=0) {
        return;
    }

    // Add a binary data structure to the simulation
    struct myBinaryPayload binaryData;
    binaryData.temp = temperature;
    binaryData.voltage = voltage;

    // Enqueue the measurement to the Notecard for transmission to the Notehub
    J *req = notecard.newRequest("note.add");
    if (req != NULL) {
        JAddStringToObject(req, "file", "sensors.qo");
        J *body = JCreateObject();
        if (body != NULL) {
            JAddStringToObject(body, "status", temperature > 26.67 ? "hot" : "normal"); // 80F
            JAddNumberToObject(body, "temp", temperature);
            JAddNumberToObject(body, "voltage", voltage);
            JAddNumberToObject(body, "count", eventCounter);
            JAddItemToObject(req, "body", body);
        }
        JAddBinaryToObject(req, "payload", &binaryData, sizeof(binaryData));
        notecard.sendRequest(req);
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }


}
