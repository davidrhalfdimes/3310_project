# 3310 project

3/22: david & jeswin met. we decided it would be best to keep .cpp files seperate (like the DBMS,role,users.cpp etc. files) before copy and pasting them into the appropriate file (client or server or both)
-David

3/24:
First goal: "Skeleton-ify" all classes from class diagram and get them to compile by linking them to the makefile.

4/5:
still clueless

After that, we'll tackle client functionality first:
multiple clients (Example code already supports infinite # of group chats)
username
setting default name
GUI 

and then server functionality last.



Go from working system to working system. - our wise professor

--
``this readme will be changed. just a temp thing so we don't forget it``
Superchat Program:

This program allows users to log into a multi-client chat server program where they can communicate with all other users. Chat rooms can be created and deleted, which contain their respective admins.

# Compilation instructions:
#
# Server: g++ -o server server.cpp -pthread
# Client: g++ -o client client.cpp -pthread
# make
# ./server 9000
# ./client 127.0.0.1
# Client can be run multiple times to create multiple users (up to 50).
