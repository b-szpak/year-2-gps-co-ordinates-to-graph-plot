#include <stdio.h>
#include <stdlib.h>

/**
	 * Exists to break apart the program on the console, into sections for the user making output easier to read
	 */
void spacing_bar() { //
    printf("================================\n");
}

/**
	 * Prints that the user input did not have a menu choice associated with it
	 */
void invalid_choice() {
    printf("That is not a valid choice\n");
}

/**
	* Scans for an integer and returns the result
	* @return integer input by the user, null if invalid
	*/
int get_input_int() {
    char input[14];
    scanf("%s", input);
    return strtol(input, 0, 10);
}

/**
	* Scans for a float and returns the result
	* @return float input by the user, null if invalid
	*/
float get_input_float() {
    char input[25];
    scanf("%s", input);
    return strtof(input, NULL);
}

/**
	* Prints the main menu and returns the option in it to run, as chosen by the user
	* @return integer user choice
	*/
int run_main_menu() { //Menu as presented in section 4.4 of the assignment PDF
    printf("Please select one of the following options:\n");
    printf("    1. Load a configuration file\n");
    printf("    2. Load the data and generate the CSV output file and Graph Plots\n");
    printf("    3. View and change the current configuration information\n");
    printf("    4. Update the configuration file\n");
    spacing_bar();
    return get_input_int();
}

/**
	* Prints the configuration settings menu and returns the option in it to change (or exit), as chosen by the user
	* @return integer user choice
	*/
int run_config_menu() { //Menu listing all the different configuration settings but additionally an exit option
    spacing_bar();
    printf("    Configuration settings:\n");
    printf("        1. Data File Name\n");
    printf("        2. Movement Duration\n");
    printf("        3. Maximum Speed\n");
    printf("        4. Number of Bars\n");
    printf("        5. Range Longitudinal\n");
    printf("        6. Range Latitudinal\n");
    printf("        7. Exit Configuration Settings\n");
    spacing_bar();
    return get_input_int();
}

/**
	* Asks the user if the program should update the configuration file under the same name or a new one
	* @return true (1) or false (2)
	*/
int new_config() {
    printf("Would you like to save the configuration file under a new name?\n");
    printf("    1. Yes\n");
    printf("    2. No\n");
    spacing_bar();
    return get_input_int();
}