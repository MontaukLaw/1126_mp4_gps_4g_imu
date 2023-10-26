#include "app_comm.h"

// #define DEVICE_FILE "/sys/bus/iio/devices/iio:device1/in_accel_x_raw"
// #define DEVICE_FILE "/sys/bus/iio/devices/iio:device1/in_accel_y_raw"
// #define DEVICE_FILE "/sys/bus/iio/devices/iio:device1/in_accel_z_raw"

// #define DEVICE_FILE "/sys/bus/iio/devices/iio:device1/in_anglvel_y_raw"
// #define DEVICE_FILE "/sys/bus/iio/devices/iio:device1/in_anglvel_y_raw"
// #define DEVICE_FILE "/sys/bus/iio/devices/iio:device1/in_anglvel_y_raw"

FILE *accel_x_file;
FILE *accel_y_file;
FILE *accel_z_file;
FILE *anglvel_x_raw_file;
FILE *anglvel_y_raw_file;
FILE *anglvel_z_raw_file;

int init_imu(void)
{

    // 打开设备文件
    accel_x_file = fopen("/sys/bus/iio/devices/iio:device1/in_accel_x_raw", "r");
    accel_y_file = fopen("/sys/bus/iio/devices/iio:device1/in_accel_y_raw", "r");
    accel_z_file = fopen("/sys/bus/iio/devices/iio:device1/in_accel_z_raw", "r");
    anglvel_x_raw_file = fopen("/sys/bus/iio/devices/iio:device1/in_anglvel_x_raw", "r");
    anglvel_y_raw_file = fopen("/sys/bus/iio/devices/iio:device1/in_anglvel_y_raw", "r");
    anglvel_z_raw_file = fopen("/sys/bus/iio/devices/iio:device1/in_anglvel_z_raw", "r");

    if (accel_x_file == NULL || accel_y_file == NULL || accel_z_file == NULL || anglvel_x_raw_file == NULL || anglvel_y_raw_file == NULL || anglvel_z_raw_file == NULL)
    {
        perror("cant access device file");
        return 1;
    }

    return 0;
}

void print_data(FILE *file, char *dataname)
{
    char data[10];
    // 读取设备文件内容
    if (fgets(data, sizeof(data), file) == NULL)
    {
        perror("read device file error");
    }
    printf("%s:%s", dataname, data);
}

void read_all_data(void)
{

    print_data(accel_x_file, "accel_x");
    print_data(accel_y_file, "accel_y");
    print_data(accel_z_file, "accel_z");
    print_data(anglvel_x_raw_file, "anglvel_x_raw");
    print_data(anglvel_y_raw_file, "anglvel_y_raw");
    print_data(anglvel_z_raw_file, "anglvel_z_raw");
}

void uinit_imu(void)
{
    fclose(accel_x_file);
    fclose(accel_y_file);
    fclose(accel_z_file);
    fclose(anglvel_x_raw_file);
    fclose(anglvel_y_raw_file);
    fclose(anglvel_z_raw_file);
}

int main__(void)
{

    while (1)
    {
        init_imu();
        read_all_data();
        uinit_imu();
        sleep(1);
    }
}

void *imu_process(void *param)
{
    while (1)
    {
        init_imu();
        read_all_data();
        uinit_imu();
        sleep(1);
    }
}

