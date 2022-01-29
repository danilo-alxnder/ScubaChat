# ScubaChat
Welcome to the README of the Scuba chat application, a fully distributed multi-hop ad-hoc chat application that enables communication between 4 devices.

## Functionalities
* Up to 4 users are allowed to talk
* The system allows messages up to 96 bytes using multi hop forwarding
* A forwarding table manages coallision of messages
* FIFO queing is used to deliver messages
* The application interface requires you to input `username`


### Running the application
1. To run ScubaChat you need an IDE. In this project, Visual Studio was used.
2. In your project include all folders of this repository
3. To test main.cpp, acces to [netsys.ewi.utwente](http://netsys.ewi.utwente.nl/integrationproject) and choose a preferable frequency
4. In main.cpp make sure you include this number in `FREQUENCY`



## Demo
When properly 
![demo](https://user-images.githubusercontent.com/70687643/151673074-73f4fbf2-9531-4a96-8fd8-9839f1d09e94.png)

In windows terminal, you can see a demostration when 3 users (Danilo, Hussain & Pepito) are talking.
![demo1](https://user-images.githubusercontent.com/70687643/151673076-8e338aa8-2f2e-460b-a8cf-0d0a60a238de.png)

