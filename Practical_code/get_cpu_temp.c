#include <stdio.h>   
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define CPU_TEMP_FILE0 "/sys/devices/virtual/thermal/thermal_zone0/temp"

struct cpu_temperature
{
 int integer_part;
 int decimal_part;
};

typedef struct cpu_temperature cpu_temperature_t;

cpu_temperature_t get_cpu_temperature(const char *_cpu_temp_file)
{
 FILE *fp = NULL;
 cpu_temperature_t cpu_temperature = {0};
 int temp = 0;
 
 fp = fopen(_cpu_temp_file, "r");
 if (NULL == fp)
 {
  printf("fopen file error\n");
  return cpu_temperature;
 }

 fscanf(fp, "%d", &temp);
 cpu_temperature.integer_part = temp / 1000;
 cpu_temperature.decimal_part = temp % 1000 / 100;

 fclose(fp);

 return cpu_temperature;
}


int main(int arc, char *argv[])
{
 cpu_temperature_t cpu_temperature = {0};

 cpu_temperature = get_cpu_temperature(CPU_TEMP_FILE0);
 printf("cpu_temperature = %d.%d ℃\n", cpu_temperature.integer_part, cpu_temperature.decimal_part);
 return 0;
}