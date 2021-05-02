/*
 / I'd rather add too many comments than too few.
 / Code is easier to write than it is to read, so let's not risk it. 
 / Made by Slushee (Pol Fernàndez)
 / Alpha 1.0.1 (02/05/2021)
 */

HardwareSerial Reciever(1);                            // Initialize the harware's serial port 1 with the name Reciever (0 is for programming)

String RecievedString;                                 // Create a string to hold all the incoming chars

void setup() 
{
  Reciever.begin(115200, SERIAL_8N1, 17, 16);          // Start the sender serial port (Baud Rate, Protocol, TXd pin, RXd pin)
}

void loop()                                            // Run indefinitely
{
  while(Reciever.available())                          // While there is something being recieved,
  {
    char RecievedChar = Reciever.read();               // Add it to the char variable
    RecievedString += RecievedChar;                    // Concavinate all the incoming chars to the string
    
    if (RecievedChar == '\n')                          // If the recieved character is a newline character that means that it is the end of a message, so
    {                                                 
      if(RecievedString.indexOf("EXAMPLE!") >= 0)      // Check if the message is this one, and if it is,
      {
        /*ACTION*/                                     // Execute this action
        RecievedString = "";                           // And clear the message.
      }

      else if(RecievedString.indexOf("EXAMPLE") >= 0)  // Check if the message is this other one, and if it is,
      {
        /*ACTION*/                                     // Execute this action
        RecievedString = "";                           // And clear the message.
      }

      break;                                           // Break out of the loop
    }

    else if(RecievedString.length() >= 50)             // If the message being recieved is really long 
    {                                                  // it means that it is gibberish or that there has been an error recieving it, so
      RecievedString = "";                             // empty the message.
    }

  }

  
}
