# Lab3_816022581
ECNG3006 Lab 3 Repository. Testing is the greatest thing that a human can do to ensure that their ESP stays happy. A wise man once said "A person can only truly be happy if their freertos tasks don't cause their esp to spontaneously combust". 

# Task 1 Unit Test Procedure

## Task 1 testing version

As discussed, in the comments of the code, a new version of task_1 was created for unit testing purposes. This was to somewhat replicate what I wanted to accomplish using the unity framework and test cases in that method, but I was unable to get it working properly in time for submission of this assignment so I chose to use this method instead. 

See the figure below which shows the key differences between the testing version of task 1 and the original version (NOTE: the functionality to be verified remains entirely UNCHANGED):

![image](https://user-images.githubusercontent.com/91706020/207546738-7e5ec8d8-bee8-4d7e-b610-110600380e49.png)

ONE MORE KEY DIFFERENCE TO NOTE: 

The task delay is removed from the testing version of task_1. After the active delay and giving back the mutex it deletes itself. This is to simplify testing.

## Task 1 Unit Test Procedure Scenarios (slightly different from what was specified in main branch readme file) and Verification Notes 

Note that the above testing version of task_1 is used for this. 

This unit test is created as a self deleting task, once it is complete it will delete itself. 

The task for the testing version of task_1 is created within this unit test task multiple times to test the desired scenarios. 

This is used for validation functional requirements of the units (tasks) of the system itself. 

For the verification of the system (q3 of this lab), the results from the scenarios of the unit test, alongside the pass/fail of the FULL SYSTEM integration test are used to verify the overall requirements of the system itself and if it functions as intended. The pass/fail scenarios of the unit test is intented to be used for verification of the requirements for the specific task itself (the requirements of which were given in the lab 2 q1 and q2 descriptions). 

The unit test task will be managing the mutex. It does this by taking the mutex and choosing to give it or continue holding it when creating the test version of task_1 (for scenarios where task 1 can and cannot take mutex to access shared resource which is the GPIO2 pin). 

### Unit Test initialization
The unit test task is created with a lower priority of 5, and WHEN it creates the test version of task_1 it does so with a priority higher, such that it can pre-empt the unit test task. 

The unit test task first takes the mutex the first time. Then it sets the GPIO2 level low, such that task 1 required functionality of setting GPIO2 HIGH can be tested. 

The unit test creates the task for the test version of task 1. 

### Task 1 TEST Scenario 1
INITIAL STIMULUS: 

-MUTEX IS Available for Task 1 to take

REQUIREMENT TESTED: Active Delay of 500MS

PASS REQUIREMENT: TICKS BETWEEN 49-51 (490ms and 510ms) for the active delay before test version of task_1 deletes itself.

Task 1 take the mutex. 

The first test scenario (seen in the snippet of code below) will verify that the active delay for the task is indeed the required 500ms within plus or minus 1 tick (10ms as the tick rate is 100Hz, see freertos config files and settings):

![image](https://user-images.githubusercontent.com/91706020/207549534-12aeacfc-cc52-4cca-a743-97c123a608ef.png)

At this point the test version of task 1 would have done its job (which will be the next test) and it would be deleted.

### Task 1 TEST Scenario 2
INITIAL STIMULUS: 
(Stimulus from the same snippet of code as above test, but checking a different parameter now for this TEST scenario)

-MUTEX IS Available for Task 1 to take

-GPIO2 Pin Set LOW Initially

REQUIREMENT TESTED: Task_1 set GPIO2 to HIGH

PASS REQUIREMENT: GPIO Level is HIGH after test version of task_1 active delay and deletion event

![image](https://user-images.githubusercontent.com/91706020/207553098-fb093fcd-1b21-47cf-b4a2-916857682822.png)

### Task 1 TEST Scenario 3
INITIAL STIMULUS:

-MUTEX IS NOT Available for Task 1 to take

-GPIO2 Pin Set LOW Initially

REQUIREMENT TESTED: Task_1 will NOT set GPIO2 to HIGH when it does not have the MUTEX even if it is ready to run

PASS REQUIREMENT: GPIO2 Level STILL LOW after 1.5 seconds and test version of task 1 deleted itself after

![image](https://user-images.githubusercontent.com/91706020/207553826-d8f25ee6-c3fd-4d50-9395-af8d3e5fe299.png)

The unit test task then deletes itself. 

#RESULTS

From the .out file generated using picocom. 

![image](https://user-images.githubusercontent.com/91706020/207555093-89c0d241-662e-4711-ae87-6abdeac46f50.png)

![image](https://user-images.githubusercontent.com/91706020/207555200-447624bf-1196-4273-8bb4-3d341d878da2.png)

![image](https://user-images.githubusercontent.com/91706020/207555271-8ad0d5ee-59b0-411d-a88e-401ae3f51c00.png)

The unit test for task 1 completed 3/3 successful tests.

Task 1 is verified to meet its requirements. 
