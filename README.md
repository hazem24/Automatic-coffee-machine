# Coffee Vending Machine.
> Software system for coffee vending machine which let the user choose his/her favorite type of coffee and insert the coins inside it and take his/her coffee. 


## Inputs

*User Command: 
      *User can choose his/her favorite type of coffee by keypad.
*coins: 
      *User can input coins inside the machine to sell his/her favorite coffee type (coins represented as external interrupt).
*Admin Command: 
      *The owner can add some magic commend to open coffee slot to add more coffee or to take all collected coins inside the machine.

## Outputs

*For User:
    *Messages via (LCD LM044L).
    *Coffee.
    *Coins in case of refund.

*For Owner:
   *Coins.

## Static architecture.
![Static architecture](https://ibb.co/JChyCKY)


## Operating System.
* FreeRtos v10.2.0

## State Machine Diagram
![alt text](https://ibb.co/JChyCKY)


## Tasks.

* 0.2.1
    * CHANGE: Update docs (module code remains unchanged)
* 0.2.0
    * CHANGE: Remove `setDefaultXYZ()`  
    * ADD: Add `init()`
* 0.1.1
    * FIX: Crash when calling `baz()` (Thanks @GenerousContributorName!)
* 0.1.0
    * The first proper release
    * CHANGE: Rename `foo()` to `bar()`
* 0.0.1
    * Work in progress

## Meta

Hazem Khaled – [@YourTwitter](https://www.linkedin.com/in/hazem-khaled-407b1b17a/) – hazemkhaled1948@gmail.com
