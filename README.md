# Coffee Vending Machine.
> Software system for coffee vending machine which let the user choose his/her favorite type of coffee and insert the coins inside it and take his/her coffee. 

## Watch The video to know more about vending machine.
[![Vending Machine](https://img.youtube.com/vi/2YR-EYTD62M/0.jpg)](https://www.youtube.com/watch?v=2YR-EYTD62M)


## Inputs

* User Command:
    * User can choose his/her favorite type of coffee by keypad.
* coins:
    * User can input coins inside the machine to buy his/her favorite coffee type (coins represented as external interrupt).
* Admin Command:
    * The owner can add some magic commend to open coffee slot to add more coffee or to take all collected coins inside the machine.
## Outputs

* For User:
    * Messages via (LCD LM044L).
    * Coffee.
    * Coins in case of refund.

* For Owner:
   * Coins.
## Target:
   * MCAL Layer for atmega32.
   * Just change the Infrastructure folder and MCAL Folder with your own target.
## Static architecture.
![Static architecture](https://i.ibb.co/cvPfqhX/Static-Arch.png)


## Operating System.
* FreeRtos v10.2.0

## State Machine Diagram
![FSM](https://i.ibb.co/TrCbrkQ/State-Machine-Diagram.png)


## Tasks.

* System Init Task.
    * Priority: 6 (Highest priority in the system).
* Collect Coins.
    * priority: 5  
    * Description: Collect coins from the user.
* Read Command.
    * Priority: 4
    * Description: Read Command via Keypad.
* Refund:
    * Priority: 3
    * Description: Give Refund for user (if inserted coins < coffee_price || inserted_coins > coffee_price). 
* Deliver Coffee:
    * Priority: 2
    * Description: Deliver choosen coffee to user.
* Display message:
    * Priority: 1
    * Description: Display Message via LCD.

## Meta

* Hazem Khaled – [@Linkedin](https://www.linkedin.com/in/hazem-khaled-407b1b17a/) – hazemkhaled1948@gmail.com
* Ahmed Kamal– [@Linkedin](https://www.linkedin.com/in/ahmedmkamal60/)
