# ScubaChat 🐬
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
When properly installed, run the code and type the name of the user. You will see per each nickname a circle would be created in the server.
When sending messages, a sphere around the user will light up and  show its recheability to other users:

<p align="center">
<img src="https://user-images.githubusercontent.com/70687643/151673074-73f4fbf2-9531-4a96-8fd8-9839f1d09e94.png" width ="500">
</p>

In windows terminal, you can see a demostration when 3 users (Danilo, Hussain & Pepito) are talking.
- From left to righ, the sms send by each user is shown in each window.
- While sending messages, the forwarding table fills up and shows the code use to refer each username.
- When coallision is encounter first come first serve.

<p align="center">
<img src="https://user-images.githubusercontent.com/70687643/151673076-8e338aa8-2f2e-460b-a8cf-0d0a60a238de.png" width ="950">
</p>


