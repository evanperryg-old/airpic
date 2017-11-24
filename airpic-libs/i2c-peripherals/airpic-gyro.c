#include <p24Fxxxx.h>
#include "./../airpic-i2c.h"

#include "airpic-gyro.h"

struct Gyroscope gyro_1;
struct Gyroscope gyro_2;

void gyro1_init(void)
{
    gyro_1.x     = 0;
    gyro_1.y     = 0;
    gyro_1.z     = 0;
    gyro_1.cal_x = 0;
    gyro_1.cal_y = 0;
    gyro_1.cal_z = 0;
    
    i2c_start_write(0b1101011);
    i2c_transmit(0x20);
    i2c_transmit(0x00);
    i2c_stop();
    
    i2c_start_write(0b1101011);
    i2c_transmit(0x20);
    i2c_transmit(0x0F);
    i2c_stop();
    
}

void gyro2_init(void)
{
    gyro_2.x     = 0;
    gyro_2.y     = 0;
    gyro_2.z     = 0;
    gyro_2.cal_x = 0;
    gyro_2.cal_y = 0;
    gyro_2.cal_z = 0;
    
    i2c_start_write(0b1101010);
    i2c_transmit(0x20);
    i2c_transmit(0x00);
    i2c_stop();
    
    i2c_start_write(0b1101010);
    i2c_transmit(0x20);
    i2c_transmit(0x0F);
    i2c_stop();
    
}

void gyro1_refresh(void)
{
    unsigned short rcv[6] = {0,0,0,0,0,0};
    
    i2c_start_write(0b1101011);
    i2c_transmit(0x28 | 0x80);
    i2c_stop();

    i2c_start_read(0b1101011);
    rcv[0] = i2c_receive();
    rcv[1] = i2c_receive();
    rcv[2] = i2c_receive();
    rcv[3] = i2c_receive();
    rcv[4] = i2c_receive();
    rcv[5] = i2c_lastReceive();
    i2c_stop();
    
    gyro_1.x = ((rcv[1] << 8) | rcv[0]) - gyro_1.cal_x;
    gyro_1.y = ((rcv[3] << 8) | rcv[2]) - gyro_1.cal_y;
    gyro_1.z = ((rcv[5] << 8) | rcv[4]) - gyro_1.cal_z;
    
}

void gyro2_refresh(void)
{
    unsigned short rcv[6] = {0,0,0,0,0,0};
    
    i2c_start_write(0b1101010);
    i2c_transmit(0x28 | 0x80);
    i2c_stop();

    i2c_start_read(0b1101010);
    rcv[0] = i2c_receive();
    rcv[1] = i2c_receive();
    rcv[2] = i2c_receive();
    rcv[3] = i2c_receive();
    rcv[4] = i2c_receive();
    rcv[5] = i2c_lastReceive();
    i2c_stop();
    
    gyro_2.x = ((rcv[1] << 8) | rcv[0]) - gyro_2.cal_x;
    gyro_2.y = ((rcv[3] << 8) | rcv[2]) - gyro_2.cal_y;
    gyro_2.z = ((rcv[5] << 8) | rcv[4]) - gyro_2.cal_z;
    
}

void gyro1_calibrate(void)
{
    unsigned short rcv[6] = {0,0,0,0,0,0};
    
    i2c_start_write(0b1101011);
    i2c_transmit(0x28 | 0x80);
    i2c_stop();

    i2c_start_read(0b1101011);
    rcv[0] = i2c_receive();
    rcv[1] = i2c_receive();
    rcv[2] = i2c_receive();
    rcv[3] = i2c_receive();
    rcv[4] = i2c_receive();
    rcv[5] = i2c_lastReceive();
    i2c_stop();
    
    gyro_1.cal_x = ((rcv[1] << 8) | rcv[0]);
    gyro_1.cal_y = ((rcv[3] << 8) | rcv[2]);
    gyro_1.cal_z = ((rcv[5] << 8) | rcv[4]);
    
    gyro_1.x = 0;
    gyro_1.y = 0;
    gyro_1.z = 0;
    
}

void gyro2_calibrate(void)
{
    unsigned short rcv[6] = {0,0,0,0,0,0};
    
    i2c_start_write(0b1101010);
    i2c_transmit(0x28 | 0x80);
    i2c_stop();

    i2c_start_read(0b1101010);
    rcv[0] = i2c_receive();
    rcv[1] = i2c_receive();
    rcv[2] = i2c_receive();
    rcv[3] = i2c_receive();
    rcv[4] = i2c_receive();
    rcv[5] = i2c_lastReceive();
    i2c_stop();
    
    gyro_2.cal_x = ((rcv[1] << 8) | rcv[0]);
    gyro_2.cal_y = ((rcv[3] << 8) | rcv[2]);
    gyro_2.cal_z = ((rcv[5] << 8) | rcv[4]);
    
    gyro_1.x = 0;
    gyro_1.y = 0;
    gyro_1.z = 0;
    
}

int gyro1_getX(void)
{
    return gyro_1.x;
}

int gyro1_getY(void)
{
    return gyro_1.y;
}

int gyro1_getZ(void)
{
    return gyro_1.z;
}

int gyro2_getX(void)
{
    return gyro_2.x;
}

int gyro2_getY(void)
{
    return gyro_2.y;
}

int gyro2_getZ(void)
{
    return gyro_2.z;
}