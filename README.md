## Inspiration
The wireless gaming steering wheel is a joy-project that we decided to try out just for fun. 

This steering wheel allows us to play one of our favorite games - Asphalt 8 - with a more immersive experience our laptops. In reality, this steering wheel can be programmed to work with any game, as it emulated keypresses on the host laptop. It is also very easily scalable as we can increase the amount of wireless data being transmitted. Currently, we are using only 6 bits of the data being transmitted over the air:
* Bit 1: Left tilt
* Bit 2: Right tilt
* Bit 3: Nitro button
* Bit 4: Brake button(assuming auto-accelerate)
* Bit 5: Powerup 1
* Bit 6: Powerup 2

## What it does
### Steering Wheel(Transmitter)
This wireless steering wheel uses an accelerometer, 4 buttons, and a 433MHz transmitter to emulate the tilt of a steering wheel and 4 function buttons. 
### Receiver
There is another Arduino with a 433MHz receiver connected to the laptop running the game. The receiving end receives the data sent from the steering wheel, parses it and send it over UART to a python script running on the laptop. This python script accepts the Serial data and emulates pressing the appropriate keys. 

## How I built it
### Steering Wheel(Transmitter)
The steering wheel has an Arduino Nano hooked up to an ADXL345 accelerometer, 4 push buttons, and a 433MHz transmitter, all placed on a breadboard for portability. 

The accelerometer is placed in the centre of the breadboard to measure the tilt accurately. The Arduino is continuously polling the accelerometer for data on acceleration in X, Y, and Z axis using the I2C protocol. The tilt of the entire arrangement is calculated using the inverse tangent of the data on Y and X axis. Left and right tilt affect 2 bits of the byte being sent respectively. The accelerometer data is also passed through a software implemented low-pass filter to smooth the values and prevent noise. 

There are 4 large pushbuttons connected to the Arduino Nano and these are also being continuously polled and affect 4 separate bits.
* Button 1: Nitro button
* Button 2: Brake button(assuming auto-accelerate)
* Button 3: Powerup 1
* Button 4: Powerup 2

The byte of data is sent over to the receiver using the VirtualWire library for Arduino through the 433MHz transmitter via Amplitude Shift Keying(ASK).

![Steering Wheel Circuit](https://challengepost-s3-challengepost.netdna-ssl.com/photos/production/software_photos/001/268/748/datas/original.png "Steering Wheel Circuit")

Finally, I created a cover for the steering wheel to make it look good and drew symbols for each button and an 'On' indicator which lights up from the Arduino's power LED. 

![Steering Wheel Complete](https://challengepost-s3-challengepost.netdna-ssl.com/photos/production/software_photos/001/268/749/datas/original.jpg "Steering Wheel Complete")

Everything is powered using a 9V battery.

### Receiver
The receiver side has an Arduino Uno hooked up to a 433 MHz receiver. This receiver receives the byte and forwards it to the host computer over UART. The host computer has a python script running with receives this byte of data and emulates key presses according to the bits. 


![Receiver Circuit](https://challengepost-s3-challengepost.netdna-ssl.com/photos/production/software_photos/001/268/750/datas/original.png "Receiver Circuit")

## Challenges I ran into
* The antenna from both the transmitter and receiver broke off early on and got lost. We had to substitute them with bent pin headers and solder them back on.
* Even after we soldered the bent pin headers on the receiver and transmitter, there was a very low range of transmission. Hence, we connected extra wires in an attempt to increase it, which worked for our purpose.
* It was our first time using the I2C protocol with the Arduino Wire library. Hence, there was a lot of figuring out to do before we could use the accelerometer. 
* We had to create a low latency system, thus we decided to using bit shifting and were able to transmit all the data from the steering wheel in just 6 bits. We also had to test multiple libraries on Python to find the quickest working solution.

## Accomplishments that I'm proud of
* Fitting all the components for the steering wheel in a hand-help form factor. 
* We ran into many challenges getting the wireless transmission working, but we got it running in the end. 
* Creating a low latency system good enough to be used in a gaming environment. 
* Using the accelerometer for the first time. 

## What I learned
* Using I2C protocol for data collection. 
* How to pack a large amount of data in one byte.
* Smoothing data values using filters. 
* Emulating key-presses with Python. 

## What's next for Wireless Gaming Steering Wheel
* Expanding the functionality of the remote to include more buttons and functionality. 
* Currently it only presses keys(binary values), but we want to expand it to send analog values to make the car turn only the amount the steering is being tilted. 
