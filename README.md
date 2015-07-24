# Pulsum-Plantae
This repository contains all the parts necesery to make pulsum plantae system run. It contains some hardware parts and software with various moduls from data receivers to sound interpreters. 

#Hardware
The LM324N is the core of the circuit, consists of four independent, high-gain, internally frequency-compensated operational amplifiers designed specifically to operate from a single power supply over a wide range of voltages. for more information read the datashet: http://pdf.datasheetcatalog.com/datasheet/philips/LM324N.pdf
![Operation Amplifier](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/esquematicos/lm324n.png?raw=true)

**Circuit for bio-electrical readings**
![Gerber file](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/esquematicos/gerber.png?raw=true)

- [Download Fritzing file here:](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/pcb/GalvanicoLM324Final.fzz)
- [Download the pdf etchable file here:](https://github.com/Lessnullvoid/Pulsum-Plantae/tree/master/pcb/pdfLM324Final)
- [Download the gerber file here:](https://github.com/Lessnullvoid/Pulsum-Plantae/tree/master/pcb/GalvanicoLM324_Gerber_Final)

List of components for the circuit:
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


- 1 Load the pulsumSensorRead software in your arduino [find the code here.](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/arduino/pulsumSensorRead/pulsumSensorRead.ino)
- 2 conect your circuit power terminal to the arduino power 3.3V.
- 3 plug the output pin from the pulsum circuit to your analog input in arduino.
- 4 plug your electrodes to the terminal electrodes pin in your pulsum circuit.
- 5 Conect the electrodes to your plant 

#Software

After prepare the circuit and pluged to the arduino board, you are ready to test the system using the software we provided here.
We have a tool call PulsumOSC is a OpenFrameWorks program that runs on Mac 0SX 10.7, 10.8, 10.9, 10.10. To run the software you need to select your serial port on the serial list.

![PulsumOsc](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/OSCsoftware.png?raw=true)

After runing the software is going to start serving OSC values in the following arrangement: 

 ```
OSC0/filtrado OSC0/crudo OSC0/min OSC0/max
OSC1/filtrado OSC1/crudo OSC1/min OSC1/max
OSC2/filtrado OSC2/crudo OSC2/min OSC2/max
OSC3/filtrado OSC3/crudo OSC3/min OSC3/max
OSC4/filtrado OSC4/crudo OSC4/min OSC4/max
```

This values are processed in the sensor.h as it:

 ```
  const unsigned short getMin() const;
	const unsigned short getMax() const;
	const unsigned short getRawValue() const;
	const unsigned short getAverageValue() const;
	const unsigned short getAverageValueNormalized() const;
 ```
 Then your ar good to run the puredata part in the system:
 ![pd granular](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/pdgranular.jpg?raw=true)

#Some notes on electrodes

Here is tree diferent types of electrodes to plug your plants, this depends on the physiological fetures of the plant you are intending to connect. For best results i will recomend to test the tree options and check the results with the monitor software.

- The gel electrodes are specially good for long leaves, i specially recomend the circular one use for facial treatment. They are small and reusable, so you can clean after using, just with a little of wather: ![Gel electrodes for long leaves](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/gelelectrodes.jpg?raw=true)

- The petridish system uses alumiun tape place in a T form. This type of electrodes work with moses or algea or other type of small plants. You can also work with cooper tape, but is important to consider the process of corrosion is faster on cooper than aluminion. And this could poison your plants. ![Petridish](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/petridish.jpg?raw=true)

- The acupunture needles are specially good for cactacea or suculente type of plants, they are made out stainless steal, that means you can leave the needles inside the cactus for long time with no problems. ![Needles](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/needless.jpg?raw=true)





