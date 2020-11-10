# KNU EE - SW


* * *
## 1. JAVA

### PRINT DIGITS AS PRAMID
all digits are 1~9.

#### 1) JAVA_1_1.java
scan the integer and print pramid shaped numbers, each line' number is starting from 1 and increasing from right to left

#### 2) JAVA_1_2.java
simmilar to HW14, but each line is symmetric digit patterns

#### 3) JAVA_1_3.java
 simmilar to HW14, HW15. the digits should be connected even when the lines change. 
 odd line is left to right, even line is right to left.


#### 4) JAVA_2_1.java
Determines whether the received 9 by 9 matrix is Sudoku. If it is sudoku - valid / is not - invalid.
First variable s is 45(sum of 1 to 9). and compare with each column, row, square's sum. If it is same, it is sudoku.

#### 5) JAVA_2_2.java
Input the size of the matrix and print the sum of each column and row at the bottom and left.

#### 4) JAVA_2_3.java
 Randomly generate scores from 0 to 100, and output a histogram that goes up from the bottom, breaking into 10 units, such as 0≤x <10, 10≤x <20, ... , 90≤x≤100 .

* * *

## 2. MATLAB ( Numerical Analysis )

### 1) interp.m
select method 'lagrange' of 'newton'. this function performs interpolation of polynomial.

### 2) falespos.m 
 fin the root by using false position method, maximum iteration : 50(default)

### 3) goldmax.m 
find the root by using goldmax method

### 4) modsecant.m 
 find the root by using modified secant method.
 
 * * *
 
 
 ## 3. Data Structure
 ### 1) CvtDec2Bin.c
 Convert the Decimal number into Binary number. ( Example of using **stack** )
 
 ### 2) QueueApp.c
 Performs the four-order operation and the remaining operation by inputting 2 operators and 1 operator. ( Example of using **queue**)
 
 ### 3) ReverseAList.c
 Through dynamic memory allocation, infinite numbers can be entered(ideally). Outputs the number entered in reverse order. ( Example of using **stack** )
 
 ### 4) SingleServer.c
 It's an embodiment of the examples in the textbook('Single Server Program').
 
 ### 5) Starwars_movie.c
  This program was inspired by a program that prints Grammy Awards winners. The principles are the same, but the StarWars film's release year, series number, and director are printed in the year entered.
  
 **'Starwars.txt'** is is a file with pre-informed information.
 
 ### 6) ADT Header
 The header file used in the above examples is uploaded.
 
 * * *
 
 ## 4. SoC ( Zync, ARM Cortex 7 )
 ### 1) CLA
A verilog code that implements CLA.

I made cla_4bits.v using adder.v and clag.v. And made cla_16bits.v using cla_4bits.v.
 
 ### 2) TextLCD
 A verilog code for using Text LCD
 
 ### 3) Digital Clock
 
 Digital clock was made using Clock divider, FND, and Text LCD.
 
 ### 4) Term Project
 We made a Number Baseball Game. (3 students)
 
 |Used Device|
 |:---:|
 |Text LCD, LED, Push Button, TFT LCD, FND|
 
 * * *
 

## 5. Operating System ( LINUX, Ubuntu )
### 1) ThreadExample.c
Using the ten thread functions, the number from 0 to 99 in each function is generated 100,000 times as a random number to count each number.
**Problem** : When the clock on the CPU is performed in units [ns] and the 'time' operating in units [s] is given as 'seed', the same number of times will continue to occur. The number of random numbers caused by the Critical Section not being locked is transferred to other threads before counting, resulting in a smaller number than the intended number of 100,000.

### 2) ThreadWithLock.c
Most of them are the same as 1) but have **locked the Critical Section.**

The problem caused by 'time' will be solved by adding 1, each time random numbers are generated.

### 3) FileServer.c
Final Project of Operating System lecture.

The program manages metadata and stores data from previously processed files.

Determine commands for 'Save', 'Read', 'List', and 'End' to perform functions corresponding to each command. Continue to enter commands and files until 'End' is entered. (It was handled without regard to the case(ABC = abc).)

'Save' saves the file by dividing it by the number '_n_'. Save function is operating by thread function.

'Read' read previously stored files(divided) as a single file.

'List' reads and prints metadata file.

'End' ends the program.

* * *
