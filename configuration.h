#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Configuration *config_pointer;

struct Configuration {
    FILE *config_file;
    char config_file_name[20];
    FILE *data_file;
    char data_file_name[20];
    float minimum_movement_duration;
    float maximum_movement_duration;
    float maximum_speed;
    int num_bars;
    float minimum_range_lon;
    float maximum_range_lon;
    float minimum_range_lat;
    float maximum_range_lat;
} config;

config_pointer create_default_config_settings();

void read_config_file(config_pointer config_ptr);

void save_config(config_pointer config_ptr);

void display_config(config_pointer config_ptr);