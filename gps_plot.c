#include <stdlib.h>
#include "gps_plot.h"

/**
 * Creates a position plot structure
 * @param input_lng - Longitude
 * @param input_lat - Latitude
 * @return struct Position_co_ords for struct gps_plot
 */
Position *create_position_co_ords(float input_lng, float input_lat) {
    Position *current = malloc(sizeof(struct Position_co_ords));
    if (current == NULL) {
        printf("Could not allocate sufficient memory for co-ordinates\n");
        exit(0);
    } else {
        current->lng = input_lng;
        current->lat = input_lat;
        return current;
    }
}

/**
	 * Creates a gps_plot structure
	 * @param input_lon - Longitude
	 * @param input_lat - Latitude
	 * @param input_date_and_time - struct tm at which the plot was made
	 * @return
	 */
plot_pointer create_gps_plot(float input_lon, float input_lat, struct tm input_date_and_time) {
    plot_pointer new_plot;
    new_plot = malloc(sizeof(struct GPS_plot));
    //tm_year = years after 1900 so, Year 2018 = 1900 + 118
    if (input_date_and_time.tm_year > 118) { return NULL; } //Only plots taken in 2018 are valid as per the spec
    if (new_plot == NULL) {
        printf("Could not allocate sufficient memory for all nodes\n");
        exit(0); //Program needs to be ran again after more memory is made available
    } else {
        new_plot->position = create_position_co_ords(input_lon, input_lat);
        new_plot->date_and_time = input_date_and_time;
    }
    return new_plot;
}

/**
	 * Reads the data file and adds valid plots into a linked list, additionally at the end outputting total number of
	 * lines read, and how many of those were: valid, invalid or duplicates
	 * @param config_settings - Structure containing user defined plot configuration, plus data file
	 * @param head - Head of linked list the plots are being written into
	 */
void read_plots(config_pointer config_settings, plot_pointer *head) {
    plot_pointer new_plot;
    plot_pointer last;

    char input[50];
    config_settings->data_file = fopen(config_settings->data_file_name, "r");

    int total_lines = 0;
    int valid_lines = 0;
    int duplicate_lines = 0;
    int invalid_lines = 0;

    while (fgets(input, 50, config_settings->data_file) != NULL) {
        if (strlen(input) > 36) {
            char *value_ptr;
            char temp_date[10];
            char temp_time[10];

            value_ptr = strtok(input, ",");
            float lon = strtof(value_ptr, NULL);

            value_ptr = strtok(NULL, ",");
            float lat = strtof(value_ptr, NULL);

            value_ptr = strtok(NULL, ",");
            strcpy(temp_date, value_ptr);

            value_ptr = strtok(NULL, "\n");
            strcpy(temp_time, value_ptr);

            struct tm date_and_time;
            //read in "dd/mm/yyyy" from temp_date
            value_ptr = strtok(temp_date, "/");
            date_and_time.tm_mday = strtol(value_ptr, NULL, 10);

            value_ptr = strtok(NULL, "/");
            date_and_time.tm_mon = strtol(value_ptr, NULL, 10) - 1;

            value_ptr = strtok(NULL, "/");
            date_and_time.tm_year = strtol(value_ptr, NULL, 10) - 1900;

            //read in "hh:mm:ss" from temp_time
            value_ptr = strtok(temp_time, ":");
            date_and_time.tm_hour = strtol(value_ptr, NULL, 10);

            value_ptr = strtok(NULL, ":");
            date_and_time.tm_min = strtol(value_ptr, NULL, 10);

            value_ptr = strtok(NULL, ":");
            date_and_time.tm_sec = strtol(value_ptr, NULL, 10);

            new_plot = create_gps_plot(lon, lat, date_and_time);
            if (new_plot != NULL) { //If plot creation was invalid a null was returned
                new_plot->next = NULL;
                if (*head == NULL) { //Head has not been assigned a plot so the new plot is first in the list
                    *head = new_plot;
                    valid_lines = 1;
                } else {
                    last = *head;
                    while (last->next != NULL) { //Iterate through the list until reaching the final addition
                        last = last->next;
                    }
                    //Checks if location of plot is the same as previous plot
                    if (last->position->lat == new_plot->position->lat
                        && last->position->lng == new_plot->position->lng) {
                        duplicate_lines++; //Plot not accepted, duplicate
                    } else {
                        last->next = new_plot; //Plot accepted
                        valid_lines++;
                    }
                }
            } else {
                invalid_lines++;
            }
        } else { invalid_lines++; }
        total_lines++;
    }

    printf("\nTotal Lines: %d", total_lines);
    printf("\nValid Lines: %d", valid_lines);
    printf("\nDuplicate Lines: %d", duplicate_lines);
    printf("\nInvalid Lines: %d\n", invalid_lines);
    fclose(config_settings->data_file);

    if (valid_lines == 0) {
        printf("No valid data found in file\n");
        exit(0); //CSV file and graph cannot be created if no data exists for them, so program shuts down
    }
}

/**
	 * PROVIDED IN "distance.c" BY ASSIGNMENT SPEC
	 * The great circle distance or the orthodromic distance is the shortest distance
	 * between two points on a sphere
	 * @param other
	 * @return distance in m
	 */
double distance(Position *first, Position *second) {

    double lon1 = degToRad(first->lng);
    double lon2 = degToRad(second->lng);
    double lat1 = degToRad(first->lat);
    double lat2 = degToRad(second->lat);

    // Haversine formula (alt. vincenties formula)
    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;
    double a = pow(sin(dlat / 2), 2)
               + cos(lat1) * cos(lat2)
                 * pow(sin(dlon / 2), 2);

    double c = 2 * asin(sqrt(a));

    // Radius of earth in kilometers. Use 3956
    // for miles
    double r = 6371;

    // calculate the result (in m)
    return ((c * r) * 1000);
}


/**
	 * Returns the time in seconds (s) between two dates (of the same year)
	 * @param first - beginning date
	 * @param second - ending date
	 * @return time in s
	 */
long calculate_total_time(struct tm first, struct tm second) {
    // This function does not check for changes in the year as the collar information exists only for the
    // year 2018, so all the differences except the difference in year values needs to be checked for
    // functionality.
    // Seconds in a Day = 60 * 60 * 24 = 86,400
    // Seconds in a Hour = 60 * 60 = 3,600
    // Seconds in a Minute = 60
    long result = 0;
    int months_between = second.tm_mon - first.tm_mon;
    int days_between = 0;

    if (months_between > 0) {
        int i = 0;
        //struct tm stores tm_mon as "months since January" so the months are one back from their calendar positions
        while (i < months_between) {
            if (first.tm_mon == 1) {
                //Checks if the year is a leap year
                if (first.tm_year + 1900 % 4 == 0) { days_between += 29; }
                else { days_between += 28; }
            } else if (first.tm_mon == 0 || first.tm_mon == 2 || first.tm_mon == 4 || first.tm_mon == 6 ||
                       first.tm_mon == 7 || first.tm_mon == 9 || first.tm_mon == 11) {
                days_between += 31;
            } else {
                days_between += 30;
            }
            i++;
        }
        //Removes the days between the start of the month and the plot
        days_between -= first.tm_mday;
    }

    //If the date days are not matching the program takes account of if there is a difference in months when
    //calculating seconds between the days
    if (second.tm_mday != first.tm_mday) {
        if (months_between > 0) {
            days_between += second.tm_mday;
        } else {
            days_between += second.tm_mday - first.tm_mday;
        }
    }

    if (days_between > 0) {
        //Adds the days between in seconds (s) to result
        result += days_between * 86400;
    }

    result += ((second.tm_hour - first.tm_hour) * 3600) + ((second.tm_min - first.tm_min) * 60) +
              (second.tm_sec - first.tm_sec);
    return result;
}

/**
	 * Create a '.csv' file named after the data file inside of a linked list and data files to be used in graph plots
	 * @param results - Head of linked list containing valid plots
	 * @param settings - Structure containing data file name that the CSV file is being produced for
	 */
void create_csv_file(plot_pointer results, config_pointer settings) {
    FILE *csv;
    FILE *graph_vector;
    FILE *graph_box;
    char *value_ptr;
    double stationary_tally = 0;
    double foraging_tally = 0;
    double moving_tally = 0;
    double implausible_tally = 0;

    value_ptr = strtok(settings->data_file_name, ".");
    char name[20];
    //name used a filename placeholder for .csv and .data
    strcpy(name, value_ptr);
    strcat(name, ".csv");
    csv = fopen(name, "w");
    printf("Writing to file: '%s'\n", name);
    strcpy(name, value_ptr);
    strcat(name, "_vector.data");
    graph_vector = fopen(name, "w");

    if (graph_vector == NULL) {
        printf("Could not create DATA file\n");
        exit(0);
    }
    if (csv == NULL) {
        printf("Could not create CSV file\n");
        exit(0);
    } else {
        int movements = 0;
        plot_pointer prior = results;
        plot_pointer current = results->next;

        while (current != NULL) {
            double distance_value = distance(prior->position, current->position);

            long total_time = calculate_total_time(prior->date_and_time, current->date_and_time);

            double speed = distance_value / total_time;

            char behaviour_category[12];
            if (speed < 0.02) { strcpy(behaviour_category, "Stationary"); }
            if (speed >= 0.02 && speed < 0.33) { strcpy(behaviour_category, "Foraging"); }
            if (speed >= 0.33 && speed < 12.00) { strcpy(behaviour_category, "Moving"); }
            if (speed >= 12) { strcpy(behaviour_category, "Implausible"); }
            if (speed < 50) { //As per spec: Movements over 50m/s are omitted from the CSV file
                int configuration_check = 0;

                //Configuration Checks
                if (speed > settings->maximum_speed) { configuration_check = 1; }
                if (total_time < (long) settings->minimum_movement_duration) { configuration_check = 1; }
                if (total_time > (long) settings->maximum_movement_duration) { configuration_check = 1; }
                if (prior->position->lat > settings->maximum_range_lat ||
                    current->position->lat > settings->maximum_range_lat) { configuration_check = 1; }
                if (prior->position->lng < settings->minimum_range_lon ||
                    current->position->lng > settings->maximum_speed) { configuration_check = 1; }

                if (configuration_check == 0) {
                    movements++;
                    //Adds time in seconds to movement category for histograph
                    if (strcmp(behaviour_category, "Stationary") == 0) { stationary_tally += total_time; }
                    if (strcmp(behaviour_category, "Foraging") == 0) { foraging_tally += total_time; }
                    if (strcmp(behaviour_category, "Moving") == 0) { moving_tally += total_time; }
                    if (strcmp(behaviour_category, "Implausible") == 0) { implausible_tally += total_time; }
                    //Writing to CSV
                    fprintf(csv, "Movement:%d, %d/%d/%d, ", movements, current->date_and_time.tm_mday,
                            current->date_and_time.tm_mon + 1, current->date_and_time.tm_year + 1900);
                    fprintf(csv, "%d:%d:%d, ", current->date_and_time.tm_hour, current->date_and_time.tm_min,
                            current->date_and_time.tm_sec);
                    fprintf(csv, "%f, %f, %f, %f, ", prior->position->lng, prior->position->lat,
                            current->position->lng, current->position->lat);
                    fprintf(csv, "%fm, %lds, %fm/s, ", distance_value, total_time, speed);
                    fprintf(csv, "%s\n", behaviour_category);
                    //Writing to DATA
                    fprintf(graph_vector, "%f, %f, %f, %f, %f\n", prior->position->lng, prior->position->lat,
                            current->position->lng - prior->position->lng,
                            current->position->lat - prior->position->lat,
                            speed);
                }
            }

            prior = current;
            current = current->next;
        }

        strcpy(name, value_ptr);
        strcat(name, "_histograph.data");
        graph_box = fopen(name, "w");
        if (graph_box == NULL) {
            printf("Could not create DATA 2 file\n");
            exit(0);
        } else {
            //Prints duration of each category to .data file
            if (settings->num_bars >= 1) { fprintf(graph_box, "0.02, %f\n", stationary_tally); }
            if (settings->num_bars >= 2) { fprintf(graph_box, "0.33, %f\n", foraging_tally); }
            if (settings->num_bars >= 3) { fprintf(graph_box, "12, %f\n", moving_tally); }
            if (settings->num_bars >= 4) { fprintf(graph_box, "50, %f\n", implausible_tally); }
            fclose(graph_box);
            //Prints total duration of each category to console as well
            printf("Stationary seconds: %f\n", stationary_tally);
            printf("Foraging seconds: %f\n", foraging_tally);
            printf("Moving seconds: %f\n", moving_tally);
            printf("Implausible seconds: %f\n", implausible_tally);
        }
        fclose(csv);
        fclose(graph_vector);
    }
}

/**
 * Plots a vector and histograph graph using GNU Plot by executing on the command line, afterwards deleting data files
 * as they do not need to be stored
 * @param settings - Structure containing configuration settings for graph plot
 */
void create_graphs(config_pointer settings) {
    FILE *GNU_plot;
    char *value_ptr;
    value_ptr = strtok(settings->data_file_name, ".");
    char name[20];

    //Write vector gnuplot file and run it
    strcpy(name, value_ptr);
    strcat(name, ".gnuplot");
    GNU_plot = fopen(name, "w");
    if (GNU_plot == NULL) {
        printf("Could not generate GNU Plot Command File\n");
        exit(0);
    }
    fprintf(GNU_plot, "set terminal svg noenhanced linewidth 0.5 size 768,768\n");
    fprintf(GNU_plot, "set output \"%s_graph_vector.svg\"\n", value_ptr);
    fprintf(GNU_plot, "set title \"GPS sheep movement plot\"\n");
    fprintf(GNU_plot, "set xlabel \"Longitude\"\n");
    fprintf(GNU_plot, "show xlabel\n");
    fprintf(GNU_plot, "set ylabel \"Latitude\"\n");
    fprintf(GNU_plot, "show ylabel\n");
    fprintf(GNU_plot, "set cblabel \"Movement (m/s)\"\n");
    fprintf(GNU_plot, "set palette defined (0 \"black\", 0.02 \"blue\", 0.33 \"red\", 12 \"orange-red\", "
                      "12.1 \"yellow\")\n");
    fprintf(GNU_plot, "plot '%s_vector.data' using 1:2:3:4:5 with vectors nohead linecolor palette z title "
                      "\"%s\"\n", value_ptr, value_ptr);
    fclose(GNU_plot);

    //Write cmd command and call it
    char command[20] = "gnuplot ";
    strcat(command, value_ptr);
    strcat(command, ".gnuplot");

    system(command);

    //Remove vector data file
    strcpy(name, value_ptr);
    strcat(name, "_vector.data");
    remove(name);
    strcpy(name, value_ptr);
    strcat(name, ".gnuplot");
    remove(name);

    //Write histograph gnuplot file and run it
    GNU_plot = fopen(name, "w");
    if (GNU_plot == NULL) {
        printf("Could not generate GNU Plot Command File\n");
        exit(0);
    }
    fprintf(GNU_plot, "set terminal svg noenhanced linewidth 0.5 size 768,768\n");
    fprintf(GNU_plot, "set output \"%s_graph_histogram.svg\"\n", value_ptr);
    fprintf(GNU_plot, "set title \"GPS sheep movement histogram\"\n");
    fprintf(GNU_plot, "set xlabel  \"Movement Category\"\n");
    fprintf(GNU_plot, "show xlabel\n");
    fprintf(GNU_plot, "set ylabel \"Duration (s)\"\n");
    fprintf(GNU_plot, "show ylabel\n");
    fprintf(GNU_plot, "plot '%s_histograph.data' with boxes", value_ptr);
    fclose(GNU_plot);
    //Write cmd command and call it
    strcpy(command, "gnuplot ");
    strcat(command, value_ptr);
    strcat(command, ".gnuplot");
    //Call on cmd line
    system(command);
    //Removes .gnuplot file
    remove(name);

    strcpy(name, value_ptr);
    strcat(name, "_histograph.data");
    remove(name);
}