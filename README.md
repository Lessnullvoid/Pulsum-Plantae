# Pulsum-Plantae
Pulsum Plantae is a project focused on bioelectrical activity readings from different types of plants. Using their perceptual skills as biosensors, amplifying the signals to be sent to a microncontrolador and to work with them in a process of experimental sonification. This repository contains all the parts necessary to make pulsum plantae system run. Check our website for other examples http://lessnullvoid.cc/pulsum send us and email at info@interspecifics.cc 

The Plants, Leslie Garcia, Thiago Hersan, Paloma López and many other collaborators. 

#Hardware
The LM324N is the core of the circuit, consists of four independent, high-gain, internally frequency-compensated operational amplifiers designed specifically to operate from a single power supply over a wide range of voltages. for more information read the datashet: http://pdf.datasheetcatalog.com/datasheet/philips/LM324N.pdf
![Operation Amplifier](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/esquematicos/lm324n.png?raw=true)

**Biosensor circuit for bio-electrical readings**
![Gerber file](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/esquematicos/gerber.png?raw=true)

- [Download Fritzing file here:](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/pcb/GalvanicoLM324Final.fzz)
- [Download the pdf etchable file here:](https://github.com/Lessnullvoid/Pulsum-Plantae/tree/master/pcb/pdfLM324Final)
- [Download the gerber file here:](https://github.com/Lessnullvoid/Pulsum-Plantae/tree/master/pcb/GalvanicoLM324_Gerber_Final)
- [We have some circuits available to sell, just send us an email](info@interspecifics.cc)

**List of components for the circuit:**
- 1 LM324N
- 3 100K resistors
- 3 10K resistors
- 1 1M resistor
- 1 100K trimpot
- 1 1M trimpot
- 2 terminals
- 2 gel electrodes
- 2 acupunture needles
- 1 jumper wire


#Installation 

**Prepare your arduino and circuits**

- 1 Download the Pulsum-Plante master to your computer.
- 2 Load the pulsumSensorRead software in your arduino [find the code here.](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/arduino/pulsumSensorRead/pulsumSensorRead.ino)
- 3 connect your circuit power terminal to the arduino power 3.3V.
- 4 plug the output pin from the pulsum circuit to your analog input in arduino.
- 5 plug your electrodes to the terminal electrodes pin in your pulsum circuit.
- 6 connect the electrodes to your plant 

#Software

- [Download a zip file with the compile version of PulsumOSC here:](http://lessnullvoid.cc/download/PulsumOscDebug.zip)
- [Compile your own PulsumOSC, fing the code here:](https://github.com/Lessnullvoid/Pulsum-Plantae/tree/master/PulsumOsc)

After preparing the circuit and pluged to the arduino board, you are ready to test the system using the software we provided here. We have a tool called PulsumOSC is a OpenFrameWorks program that runs on Mac 0SX 10.7, 10.8, 10.9, 10.10. To run the software you need to select your serial port on the serial list.

![PulsumOsc](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/OSCsoftware.png?raw=true)

After running, the software starts serving OSC values in the following arrangement: 

 ```
OSC0/filtrado/data OSC0/crudo/data OSC0/min/data OSC0/max/data
OSC1/filtrado/data OSC1/crudo/data OSC1/min/data OSC1/max/data
OSC2/filtrado/data OSC2/crudo/data OSC2/min/data OSC2/max/data
OSC3/filtrado/data OSC3/crudo/data OSC3/min/data OSC3/max/data
OSC4/filtrado/data OSC4/crudo/data OSC4/min/data OSC4/max/data
```
The number after the OSC indicates the analog input where you pluged the sensor.

This values are processed in the sensor.h file as follows:
 ```
const unsigned short getMin() const;
const unsigned short getMax() const;
const unsigned short getRawValue() const;
const unsigned short getAverageValue() const;
const unsigned short getAverageValueNormalized() const;
 ```
 **Note for linux and windows users**
 
There is also a processing version of this software to perform the same transmision of data in OSC you can check the [code here.](https://github.com/Lessnullvoid/Pulsum-Plantae/tree/master/processing/PulsumOsc)

**DSP PureData and other programs**

After running any of this software's your ar good to run the puredata part in the system. This specific patch is a granular synth. Is important to preload a sound sample on every module. By default the outputs are setup in a quadraphonic arrangement, so every module send to ~DAC1 ~DAC2 ~DAC3 and so on. You can control this on the output panel. 
 ![pd granular](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/pdgranular.jpg?raw=true)

This is a first version of the software for sonification. Both versions are available. 
 ![pd plantSynth](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/pulsumv2.jpg?raw=true)


#Some notes on electrodes

Here is three different types of electrodes to plug your plants, this depends on the physiological features of the plant you are attempting to connect. For best results we recommend to test the three options and check the results with the monitor software.

- The gel electrodes are specially good for long leaves, the best are the circular ones use for facial treatment. They are small and reusable, so you can clean after using, just with a little of wather: ![Gel electrodes for long leaves](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/gelelectrodes.jpg?raw=true)

- The petri dish system uses aluminum tape place in a Y form. This type of electrodes work with mosses or algae or other type of small plants. You can also work with copper tape, but is important to consider the process of corrosion is faster on copper than aluminum. And this could poison your plants. ![Petridish](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/petridish.jpg?raw=true)

- The acupuncture  needles are specially good for cactacea or suculente type of plants, they are made out stainless steel, that means you can leave the needles inside the cactus for long time with no problems. ![Needles](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/needless.jpg?raw=true)





