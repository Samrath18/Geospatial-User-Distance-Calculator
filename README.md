# 2023/11/11

# Geospatial-User-Distance-Calculator

INPUT:

Mode Selection:
    The program starts by prompting the user to choose a mode:
        Mode 1: Manual entry of other users and their data.
        Mode 2: Use a prewritten text file containing the entries of other users and their data.
    The user inputs a mode selection (1 or 2).

Mode 1 - Manual Entry:
    If the user selects mode 1:
        The program asks for the number of other users.
        The program then prompts the user to enter the reference user's data (name, latitude, longitude, and altitude).
        Next, the program prompts the user to enter data for each of the other users (name, latitude, longitude, and altitude).

Mode 2 - File Input:
    If the user selects mode 2:
        The program asks for the path of the input file containing user data.

OUTPUT:

Distances to Other Users:
    For both modes, the program calculates and prints the distances from the reference user to each of the other users.

Closest User:
    The program calculates and displays the closest user to the reference user based on geographical positions (latitude, longitude, and altitude).
    It prints the name, latitude, longitude, and altitude of the closest user.

Memory Management:
    The program dynamically allocates memory for an array of other users and frees this memory before the program exits.
