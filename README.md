# Lab3_816022581
ECNG3006 Lab 3 Repository. Testing is the greatest thing that a human can do to ensure that their ESP stays happy. A wise man once said "A person can only truly be happy if their freertos tasks don't cause their esp to spontaneously combust". 

# Branch Usage 


# Background Lore from lab 2 discussion and testing

 Recall 
 
 https://github.com/StephenJogie/Lab2_Q2_816022581/blob/q2_discuss/README.md 

The full system for Lab2 Q2 has been verified to be working as intended using the vTaskGetRunTimeStats for the task utilization percentages and the oscilloscope to verify that the desired output was achieved.

# Plan for power consumption test 

Initially, I had a plan to test the power consumption of the system with and without sleeping while IDLE. 
However, due to some circumstance such as the steady flow of time catching up to me, I was unable to complete lab2 question 3 in time. 
The plan is as follows and will be tested and added to the readme in an update at a further date (this might be utilized in the project however). 

![image](https://user-images.githubusercontent.com/91706020/202999876-de1bcf73-e0a8-4679-b0b8-7daa62ce8c3c.png)

4 1 ohm 1/4 W rated resistors can be used for the 0.25 Ohm 1W rated sense resistor (this design choice is solely based on convenience as I happen to have some 1 ohm 1/4 W resistors). 

# Last minute plan for ratching the testing (which was actually the process done to verify lab 2 was working)

My intention is to utilize the extra time that may or may not have been granted to get the unity testing framework exmaple working (there is a project example in the esp8266 sdk which I am trying to get to work. Currently I have it displaying the unity menu with none of the test cases added successfully yet for unit, integration, and verficiation testing. The first thing that I needed to do was alter the partition table csv to account for my 1MB flash size.)

## Step 1 (Manual Unit Testing): 

Test Task 1, 2, and 3 on their own in their own program. 

This would include under various conditions as seen below. 
### Task 1:
First unit test (alter task 1 to delete itself after its task delay to better test):

  1. Set GPIO2 to LOW manually
  
  2. Make mutex available
  
  3. Create Task 1 ONLY
  
  4. Check time at which task 1 took the mutex
  
  5. Check the state of the pin and mutex around 500 ms after 
  
  6. Make sure that the mutex is available again and the gpio2 pin state is now HIGH. 
  
  7. Wait until task 1 has deleted itself. 
  
  8. Verify that the time at which task 1 has been deleted is approximately 1.5 seconds after it was created. 
  
 Second Unit test (Add a small task delay (approximately 100ms) when task1 CANNOT take the mutex): 

  1. Set GPIO2 to LOW manually
  
  2. Make mutex UNAVAILABLE
  
  3. Create Task 1 ONLY
  
  4. Check the state of the pin and mutex around 1000 ms after 
  
  5. Verify that GPIO2 is still LOW. 
  
### Task 2:
First unit test (alter task 2 to delete itself after its task delay to better test):

  1. Set GPIO2 to HIGH manually
  
  2. Make mutex available
  
  3. Create Task 2 ONLY
  
  4. Check time at which task 2 took the mutex
  
  5. Check the state of the pin and mutex around 500 ms after 
  
  6. Make sure that the mutex is available again and the gpio2 pin state is now LOW. 
  
  7. Wait until task 2 has deleted itself. 
  
  8. Verify that the time at which task 2 has been deleted is approximately 1.5 seconds after it was created. 
  
 Second Unit test (Add a small task delay (approximately 100ms) when task2 CANNOT take the mutex): 

  1. Set GPIO2 to HIGH manually
  
  2. Make mutex UNAVAILABLE
  
  3. Create Task 2 ONLY
  
  4. Check the state of the pin and mutex around 1000 ms after 
  
  5. Verify that GPIO2 is still HIGH. 
  
 ### Task 3
 
 For task 3, in the lab 2 repository it can be seen that there was several variations of task 3 with different task delays to ensure that it behaved properly when INTEGRATED with the other tasks. For the unit test there only needs to be a simple unit test to verify that task 3 works on its own.
 
 First Unit test (have task 3 delete itself after it task delays to better test): 
  
  1. Set GPIO2 to LOW manually
  
  2. Make mutex UNAVAILABLE
  
  3. Create task 3
  
  4. Wait for 500 ms then verify that task 3 still exists as it has not been able to take the mutex and thus not been able to print the pinstate and delete itself. 
  
  5. Release the mutex
  
  6. Verify that task 3 has printed LOW as the pinstate and then deleted itself after releasing the mutex. 
  
  7. Set GPIO2 to HIGH manually
  
  8. Create task 3 again
  
  9. Verify that task 3 has printed HIGH as the pinstate and then deleted itself after releasing the mutex. 
  
