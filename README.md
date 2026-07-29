# ClientForRoboticCar
A client for an ESP32 based robotic car for school project. The code uses TCP protocol and it uses Winsock2 library,  
therefore the client needs the Windows SDK present. The file `test_server.py` is a test server I wrote to test if the client works or not.
Another requirement is that the PC must support OpenGL 3.3+ for the app to open.

The client basically sends some tiny packets like '/s\n', '/w\n', etc. and the esp32 basically follows it and moves the car.
(The new line character '\n' is there so that the esp32 can read the packet until that character, so don't mind it)
