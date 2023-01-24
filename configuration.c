#include "configuration.h"

/**
	 * Create a struct Configuration and assign it default values
	 * @return config_pointer to the newly generate struct Configuration
	 */
config_pointer create_default_config_settings() {
    config_pointer default_config_ptr;
    default_config_ptr = malloc(sizeof(config));
    if (default_config_ptr == NULL) {
        return NULL;
    } else {
        //Assigns default values
        default_config_ptr->minimum_movement_duration = 0;
        default_config_ptr->maximum_movement_duration = 604800;
        default_config_ptr->maximum_speed = 50;
        default_config_ptr->num_bars = 20;
        default_config_ptr->minimum_range_lon = 0;
        default_config_ptr->maximum_range_lon = 0;
        default_config_ptr->minimum_range_lat = 0;
        default_config_ptr->maximum_range_lat = 0;
    }
    return default_config_ptr;
}

/**
	 * Read the configuration file given as a parameter and scans it for the settings being demanded
	 * Settings may appear multiple times but last instance will always be the one used
	 * @param config_ptr - struct Configuration containing the config_file_name used to assign the file
	 */
void read_config_file(config_pointer config_ptr) {
    char input[50];
    config_ptr->config_file = fopen(config_ptr->config_file_name, "r");
    while (fgets(input, 50, config_ptr->config_file) != NULL) {
        char config_item[17];
        char config_value[26];
        sscanf(input, "%s %s", config_item, config_value);

        char *value_begin = strstr(input, "<");
        if (value_begin != NULL) {
            strcpy(config_value, value_begin);
            strcpy(config_value, strtok(config_value, "\n"));
        }

        if (strcmp(config_item, "data_file_name") == 0) {
            char *double_quote = strtok(config_value, "\"");
            if (double_quote != NULL) { strcpy(config_ptr->data_file_name, double_quote); }
            config_ptr->data_file = fopen(config_ptr->data_file_name, "r");
            fclose(config_ptr->data_file);
        }

        if (strcmp(config_item, "movement_duration") == 0) {
            char *value_ptr;
            char temp[25];
            value_ptr = strtok(config_value + 1, ":");
            strcpy(temp, value_ptr);
            config_ptr->minimum_movement_duration = strtof(temp, NULL);
            value_ptr = strtok(NULL, ">");
            strcpy(temp, value_ptr);
            config_ptr->maximum_movement_duration = strtof(temp, NULL);
        }

        if (strcmp(config_item, "maximum_speed") == 0) {
            config_ptr->maximum_speed = strtof(config_value, NULL);
        }

        if (strcmp(config_item, "num_bars") == 0) {
            config_ptr->num_bars = strtol(config_value, NULL, 10);
        }

        if (strcmp(config_item, "range_lon") == 0) {
            char *value_ptr;
            char temp[25];
            value_ptr = strtok(config_value + 1, ":");
            strcpy(temp, value_ptr);
            config_ptr->minimum_range_lon = strtof(temp, NULL);
            value_ptr = strtok(NULL, ">");
            strcpy(temp, value_ptr);
            config_ptr->maximum_range_lon = strtof(temp, NULL);
        }

        if (strcmp(config_item, "range_lat") == 0) {
            char *value_ptr;
            char temp[25];
            value_ptr = strtok(config_value + 1, ":");
            strcpy(temp, value_ptr);
            config_ptr->minimum_range_lat = strtof(temp, NULL);
            value_ptr = strtok(NULL, ">");
            strcpy(temp, value_ptr);
            config_ptr->maximum_range_lat = strtof(temp, NULL);
        }
    }
    fclose(config_ptr->config_file);
}

/**
	 * Save struct Configuration values to the config file name within the structure
	 * @param config_ptr - struct Configuration containing settings and name to be saved under
	 */
void save_config(config_pointer config_ptr) {
    config_ptr->config_file = fopen(config_ptr->config_file_name, "w");

    fprintf(config_ptr->config_file, "data_file_name \"%s\"\n", config_ptr->data_file_name);

    fprintf(config_ptr->config_file, "movement_duration <%f:%f>\n", config_ptr->minimum_movement_duration,
            config_ptr->maximum_movement_duration);

    fprintf(config_ptr->config_file, "maximum_speed %f\n", config_ptr->maximum_speed);

    fprintf(config_ptr->config_file, "num_bars %d\n", config_ptr->num_bars);

    fprintf(config_ptr->config_file, "range_lon <%f:%f>\n", config_ptr->minimum_range_lon,
            config_ptr->maximum_range_lon);

    fprintf(config_ptr->config_file, "range_lat <%f:%f>\n", config_ptr->minimum_range_lat,
            config_ptr->maximum_range_lat);

    fclose(config_ptr->config_file);
}

/**
	 * Prints to the console all the configuration settings currently loaded
	 * @param config_ptr - struct Configuration currently running in the program
	 */
void display_config(config_pointer config_ptr) {
    if (config_ptr->config_file == NULL) {
        printf("Configuration File: 'Not Set'\n");
    } else {
        printf("Configuration File: '%s'\n", config_ptr->config_file_name);
    }

    if (config_ptr->data_file == NULL) {
        printf("Data File Name: 'Not Set'\n");
    } else {
        printf("Data File Name: '%s'\n", config_ptr->data_file_name);
    }

    printf("Movement Duration: <%f:%f>\n", config_ptr->minimum_movement_duration,
           config_ptr->maximum_movement_duration);
    printf("Maximum Speed: %f\n", config_ptr->maximum_speed);
    printf("Number of Bars: %d\n", config_ptr->num_bars);
    printf("Range Longitudinal: <%f:%f>\n", config_ptr->minimum_range_lon, config_ptr->maximum_range_lon);
    printf("Range Latitudinal: <%f:%f>\n", config_ptr->minimum_range_lat, config_ptr->maximum_range_lat);
}