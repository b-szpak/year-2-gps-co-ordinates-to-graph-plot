#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "gps_plot.h"

int main(int argc, char *argv[]) {
    config_pointer configurationSettings = create_default_config_settings();
    configurationSettings->config_file = NULL;
    configurationSettings->data_file = NULL;

    if (argc == 1) { //argc[0] is the name of the program, if argc = 1 no file name was added as a parameter of the app
        printf("Program starting with default configuration values\n");
        spacing_bar();
        int loop_done = 0;
        while (loop_done != 1) {
            switch (run_main_menu()) {
                // 1. Load a configuration file
                case 1:
                    printf("Please enter the name of the configuration file you wish to load:\n");
                    char config_name[20];
                    scanf("%s", config_name);

                    strcpy(configurationSettings->config_file_name, config_name);
                    read_config_file(configurationSettings);

                    if (configurationSettings->config_file == NULL) {
                        printf("Configuration File '%s' not found\n", configurationSettings->config_file_name);
                    } else {
                        printf("Configuration File '%s' loaded\n", configurationSettings->config_file_name);
                    }
                    break;
                // 2. Load the data and generate the CSV output file and Graph Plots
                case 2:
                    if (configurationSettings->config_file == NULL) {
                        printf("Please first select or create a configuration file to run program through\n");
                        break;
                    }
                    if (configurationSettings->data_file == NULL) {
                        printf("Please first select a valid data file\n");
                        break;
                    }
                    loop_done = 1;
                    break;
                // 3. View and change the current configuration information
                case 3:
                    display_config(configurationSettings);

                    switch (run_config_menu()) {
                        case 1:
                            printf("Enter new 'Data File Name':\n");
                            scanf("%s", configurationSettings->data_file_name);

                            configurationSettings->data_file = fopen(configurationSettings->data_file_name, "r");
                            fclose(configurationSettings->data_file);

                            if (configurationSettings->data_file == NULL) {
                                printf("'%s' does not exist, please enter a valid filename\n",
                                       configurationSettings->data_file_name);
                            } else {
                                printf("'%s' exists and has been saved\n",
                                       configurationSettings->data_file_name);
                            }
                            break;
                        case 2:
                            printf("Enter new 'Minimum Movement Duration':\n");
                            configurationSettings->minimum_movement_duration = get_input_float();

                            printf("Enter new 'Maximum Movement Duration':\n");
                            configurationSettings->maximum_movement_duration = get_input_float();

                            printf("Movement Duration: <%f:%f>\n",
                                   configurationSettings->minimum_movement_duration,
                                   configurationSettings->maximum_movement_duration);
                            break;
                        case 3:
                            printf("Enter new 'Maximum Speed':\n");
                            configurationSettings->maximum_speed = get_input_float();

                            printf("Max Speed: %f\n", configurationSettings->maximum_speed);
                            break;
                        case 4:
                            printf("Enter new 'Number of Bars':\n");
                            configurationSettings->num_bars = get_input_int();

                            printf("Number of Bars: %d\n", configurationSettings->num_bars);
                            break;
                        case 5:
                            printf("Enter new 'Minimum Longitudinal Range':\n");
                            configurationSettings->minimum_range_lon = get_input_float();

                            printf("Enter new 'Maximum Longitudinal Range':\n");
                            configurationSettings->maximum_range_lon = get_input_float();

                            printf("Longitudinal Range: <%f:%f>\n", configurationSettings->minimum_range_lon,
                                   configurationSettings->maximum_range_lon);
                            break;
                        case 6:
                            printf("Enter new 'Minimum Latitudinal Range':\n");
                            configurationSettings->minimum_range_lat = get_input_float();

                            printf("Enter new 'Maximum Latitudinal Range':\n");
                            configurationSettings->maximum_range_lat = get_input_float();

                            printf("Latitudinal Range: <%f:%f>\n", configurationSettings->minimum_range_lat,
                                   configurationSettings->maximum_range_lat);
                            break;
                        case 7:
                            //Menu ends after 1 run so no instructions to break out needed
                            break;
                        default:
                            invalid_choice();
                            break;
                    }
                    break;
                // 4. Update the configuration file
                case 4:
                    if (configurationSettings->data_file == NULL) {
                        printf("Please first select a valid and existing data file to search\n");
                        break;
                    }
                    if (configurationSettings->config_file == NULL) {
                        printf("Please enter name to save configuration file under:\n");
                        char new_name[20];
                        scanf("%s", new_name);

                        strcpy(configurationSettings->config_file_name, new_name);
                        configurationSettings->config_file = fopen(configurationSettings->config_file_name, "r");
                        fclose(configurationSettings->config_file);
                        save_config(configurationSettings);
                    } else {
                        switch (new_config()) {
                            case 1:
                                printf("Please enter new configuration file name:\n");
                                char new_name[20];
                                scanf("%s", new_name);
                                strcpy(configurationSettings->config_file_name, new_name);
                                save_config(configurationSettings);
                                break;
                            case 2:
                                save_config(configurationSettings);
                                break;
                            default:
                                invalid_choice();
                                break;
                        }

                        save_config(configurationSettings);
                        printf("Configuration File '%s' has been saved\n",
                               configurationSettings->config_file_name);
                    }
                    break;
                default:
                    invalid_choice();
                    break;
            }

            spacing_bar();
        }
        printf("Program starting with configuration values in: '%s'\n",
               configurationSettings->config_file_name);
    }
    if (argc == 2) { //argc 2 is the configuration file that has been parsed - program runs without menu
        printf("Program starting with configuration values in: '%s'\n", argv[1]);
        strcpy(configurationSettings->config_file_name, argv[1]);
        read_config_file(configurationSettings);

        //If data file name doesn't exist quit
        if (configurationSettings->config_file == NULL) {
            printf("No Configuration file found in '%s'\n", configurationSettings->config_file_name);
            exit(0);
        }
        if (configurationSettings->data_file == NULL) {
            printf("No Data File found in '%s'\n", argv[1]);
            exit(0);
        }
    }
    //Create CSV Files and Graph Plots
    //Both if statements end when user selects the menu option to proceed or the given configuration file has valid
    //settings so both ways of starting-up the program now make use of the following code

    plot_pointer head = NULL;
    read_plots(configurationSettings, &head);

    spacing_bar();

    create_csv_file(head, configurationSettings);
    create_graphs(configurationSettings);
    return 0;
}