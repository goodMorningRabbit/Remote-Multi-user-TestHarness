# Remote-Multi-user-TestHarness

# 1. Introduction
One focus area for this course is understanding how to structure and implement big software systems.
By big we mean systems that may consist of hundreds or even thousands of packages1 and perhaps several million lines of code. 
In order to successfully implement big systems we need to partition code into relatively small parts and thoroughly test each of the parts before inserting them into the software baseline. As new parts are added to the baseline and as we make changes to fix latent errors or performance problems we will re-run test sequences for those parts and, perhaps, for the entire baseline. Managing that process efficiently requires effective tools for code analysis as well as testing. 

# 2. Structure
The TestHarness must support:
## Loading dynamic link test libraries:
clients send one or more test libraries to the TestHarness Server
clients pass test request messages to define which libraries are to be executed
## Executing tests:
TestHarness executes each test request in a separate process, provided by a process pool.
The process pool is hosted by the TestHarness, which manages child test processes using message-passing communication.
When the test execution completes, the child test process sends, to the requesting client, a test report.
## Reporting results in a Graphical User Interface:
The client displays test information based on the return message and formatting parameters set by user actions in the client GUI.
## Selecting Test Libraries to send:
The Client displays all the dynamic link libraries in a specified directory. It allows the user to make multiple selections, and passes that information to a sender package.
## Sending the selected libraries to the TestHarness:
The client uses Comm's sendfile mechanism to transparently send each message.
## Displaying the results of testing:
The user has to be able to tell which results belong to which requests. With asynchronous message passing, it is possible for the results to arrive out-of-order, because each test is likely to take a different amount of time to complete and is executed concurrently with other testing. You can handle this by giving each TestRequest a unique identifier, and embed that identifier in the results message.
Each results message will have a status: pass or fail, and additional information that is likely to vary in contents and quantity from test to test. You should design a structure for reporting test results, and design the UI display to accomodate that structure, perhaps as a list of textual information items.
## the framework is shown on the Client-Channel-Server.jpg on the repo
# 3. Usage
It's simple:
1. git clone this repo
2. Run compile.bat and run.bat, then play with the GUI
3. test results are shown on the ./log_files seperately
