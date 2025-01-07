#include <stdio.h>
#include "message.h"

int main(void)
{
    Message testMessage = {   // create a dummy message for the test
        .msgId  = 1,
        .myflag = 2,
        .value  = 3.14,
        .szDescription = {
             .arr = "HelloWorld"
        }
    };

    // Create a buffer to hold the encoded data.
    // The (maximum) size is computed by the compiler and set
    // in a macro defined in sample.h
    unsigned char encodedBuffer[Message_REQUIRED_BYTES_FOR_ENCODING];

    // The encoder needs a data structure for the serialization
    BitStream encodedMessage;

    // The Encoder may fail and update an error code
    int errCode;

    // Initialization associates the buffer to the bit stream
    BitStream_Init (&encodedMessage,
                    encodedBuffer,
                    Message_REQUIRED_BYTES_FOR_ENCODING);
    
    // Encode the message using uPER encoding rule
    if (!Message_Encode(&testMessage,
                        &encodedMessage,
                        &errCode,
                        true))
    {
        // Error codes are defined as macros in sample.h
        printf("Encoding failed with error code %d\n", errCode);       
    }
    else  // Everything went fine, print the message as a suite of hex numbers
    {
        int encodedSize = BitStream_GetLength(&encodedMessage);
        for (int i=0; i<encodedSize; ++i)
        {
            printf("%02x ", encodedBuffer[i]);
        }
        printf("(%d bytes)\n", encodedSize);
    }
}