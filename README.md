# Attendance Management System

This project is an attendance management system implemented using a client-server model with computer networking. It allows instructors to check and update student attendance, while students can view their attendance records. The system provides a user-friendly interface and enables efficient tracking and management of attendance data.

## Project Structure

The project consists of two main components:

1. Server: The server component handles client connections, manages attendance data, and processes client requests. It uses file handling to store and retrieve data.

2. Client: The client component provides a user interface for instructors and students to interact with the attendance system.

## File Structure

The project uses the following files:

1. `server.c`: Contains the server-side code for handling client connections and managing attendance data.

2. `client.c`: Contains the client-side code for providing a user interface and interacting with the server.

3. `user_pass.txt`: A text file used for storing usernames and passwords. Each line contains a username and its corresponding password, separated by a space.

4. `student_attendance.txt`: A text file used for storing attendance data. Each line contains a student's attendance record in the following format: PRN Name Subject1 Subject2 Subject3 Subject4 Subject5.

5. `temp.txt`: A temporary text file used for performing intermediate operations during the update of attendance.

## Prerequisites

To run the Attendance Management System, ensure that the following dependencies are installed:

- C Compiler (e.g., GCC)
- Standard C Libraries (stdio.h, stdlib.h, unistd.h, string.h, sys/types.h, sys/socket.h, netinet/in.h, netdb.h, stdbool.h)

## Running the Project

Follow these steps to run the Attendance Management System:

1. Clone the project repository:

   ```
   $ git clone <repository-url>
   $ cd attendance-management-system
   ```

2. Compile the server and client code:

   ```
   $ gcc server.c -o server
   $ gcc client.c -o client
   ```

3. Start the server:

   ```
   $ ./server <port-number>
   ```

   Replace `<port-number>` with the desired port to run the server (e.g., 8080).

4. Start the client:

   ```
   $ ./client <server-ip> <port-number>
   ```

   Replace `<server-ip>` with the IP address of the server running the attendance system and `<port-number>` with the same port number used to start the server.

5. The client will prompt you for login credentials. Enter the username, PRN (Personal Registration Number), and password to log in.

6. Once logged in, the client will display a menu with different options based on the user type (instructor or student). Select the desired option by entering the corresponding number.

7. Follow the prompts to perform various operations such as checking attendance, updating attendance, etc.

8. The attendance records and user passwords will be stored and retrieved from the respective text files (`student_attendance.txt` and `user_pass.txt`).

9. To exit the client, select the "Quit" option from the menu.

## Conclusion

The Attendance Management System provides an efficient and convenient way to manage attendance records using a client-server architecture. File handling is used to store and retrieve user passwords as well as student attendance data. By following the steps mentioned above, you can set up and run the attendance system on your local machine.