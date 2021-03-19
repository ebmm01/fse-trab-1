#include <stdio.h>
#include <time.h>

#define FILE_LOCATION "data.csv"

void write_csv_on_file(
    float ti,
    float te,
    float td,
    float resistor,
    float fan
) {
    FILE *fp = fopen(FILE_LOCATION, "a");
    char date_time[30];

    time_t t = time(NULL);
    struct tm dtm = *localtime(&t);
    sprintf(date_time, "%d-%02d-%02d %02d:%02d:%02d", dtm.tm_year + 1900, dtm.tm_mon + 1, dtm.tm_mday, dtm.tm_hour, dtm.tm_min, dtm.tm_sec);

    fprintf(fp, "\"%s\",\"%.2f\",\"%.2f\",\"%.2f\",\"%.2f\",\"%.2f\"\n", date_time, ti, te, td, resistor, fan);
    fclose(fp);
}

void handle_file_creation() {
    FILE *fp;
    fp = fopen(FILE_LOCATION, "r");
    
    if(fp == NULL)
    {
        fp = fopen(FILE_LOCATION, "w");
        fprintf(fp, "\"date_time\",\"ti\",\"te\",\"td\",\"resistor\",\"fan\"\n");
        fclose(fp);
    }
    else {
        fclose(fp);
    }
}