# Pulsum-Plantae
This repository contains all the parts necesery to make pulsum plantae system run. It contains some hardware parts and software with various moduls from data receivers to sound interpreters. 

#Hardware
![Operation Amplifier](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/esquematicos/lm324n.png?raw=true)

The LM324N is the core of the circuit, consists of four independent, high-gain, internally frequency-compensated operational amplifiers designed specifically to operate from a single power supply over a wide range of voltages. for more information read the datashet: http://pdf.datasheetcatalog.com/datasheet/philips/LM324N.pdf

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


#Software 

#Instalation 

- 1 Load the pulsumSensorRead software in your arduino [find the code here.](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/arduino/pulsumSensorRead/pulsumSensorRead.ino)
- 2 conect your circuit power terminal to the arduino power 3.3V.
- 3 plug the output pin from the pulsum circuit to your analog input in arduino.
- 4 plug your electrodes to the terminal electrodes pin in your pulsum circuit.
- 5 Conect the electrodes to your plant 

**Some notes on electrodes**

Here is tree diferent types of electrodes to plug your plants, this depends on the physiological fetures of your plant.
The gel electrodes are specially good for long leaves:
![Gel electrodes for long leaves](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/gelelectrodes.jpg?raw=true)

The petridish system uses alumiun tape place in a T form. This type of electrodes work with moses or algea. 
[Petridish](https://github.com/Lessnullvoid/Pulsum-Plantae/blob/master/img/petridish.jpg?raw=true)



