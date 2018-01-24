#include <p24Fxxxx.h>
#include "./../../airpic.h"
#include "./../airpic-i2c.h"

#include "airpic-gyro.h"

struct Gyroscope gyro_1;
struct Gyroscope gyro_2;

void gyro1_init(void)
{
    if( airpic_debugger_isenabled )
        airpic_debugger_println("airpic-gyro1     : init", 23);

    gyro_1.xVel     = 0;
    gyro_1.yVel     = 0;
    gyro_1.zVel     = 0;
    
    gyro_1.x        = 0.0;
    gyro_1.y        = 0.0;
    gyro_1.z        = 0.0;
    
    gyro_1.x     = 0;
    gyro_1.y     = 0;
    gyro_1.z     = 0;

    if( airpic_debugger_isenabled )
        airpic_debugger_println("airpic-gyro1     : begin transmission to device", 47);
    
    i2c_start_write(0b1101011);
    i2c_transmit(0x20);
    i2c_transmit(0x00);
    i2c_stop();
    
    i2c_start_write(0b1101011);
    i2c_transmit(0x20);
    i2c_transmit(0x0F);
    i2c_stop();
    
    if( airpic_debugger_isenabled )
        airpic_debugger_println("airpic-gyro1     : end of init", 30);
    
}

void gyro2_init(void)
{
    if( airpic_debugger_isenabled )
        airpic_debugger_println("airpic-gyro2     : init", 23);
    
    gyro_2.xVel     = 0;
    gyro_2.yVel     = 0;
    gyro_2.zVel     = 0;
    
    gyro_2.x        = 0.0;
    gyro_2.y        = 0.0;
    gyro_2.z        = 0.0;
    
    if( airpic_debugger_isenabled )
        airpic_debugger_println("airpic-gyro2     : begin transmission to device", 47);
    
    i2c_start_write(0b1101010);
    i2c_transmit(0x20);
    i2c_transmit(0x00);
    i2c_stop();
    
    i2c_start_write(0b1101010);
    i2c_transmit(0x20);
    i2c_transmit(0x0F);
    i2c_stop();
    
    if( airpic_debugger_isenabled )
        airpic_debugger_println("airpic-gyro2     : end of init", 30);
    
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
    
    gyro_1.xVel = ((rcv[1] << 8) | rcv[0]);
    gyro_1.yVel = ((rcv[3] << 8) | rcv[2]);
    gyro_1.zVel = ((rcv[5] << 8) | rcv[4]);
    
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
    
    gyro_2.xVel = ((rcv[1] << 8) | rcv[0]);
    gyro_2.yVel = ((rcv[3] << 8) | rcv[2]);
    gyro_2.zVel = ((rcv[5] << 8) | rcv[4]);
    
}

void gyro1_accumulate(void)
{
    // assume the period of the interrupt is 20MS
    gyro_1.x = gyro_1.x + 0.02*(DPS_RANGE)*(gyro_1.xVel / 32767.0);
    gyro_1.y = gyro_1.y + 0.02*(DPS_RANGE)*(gyro_1.yVel / 32767.0);
    gyro_1.z = gyro_1.z + 0.02*(DPS_RANGE)*(gyro_1.zVel / 32767.0);
    
    if(gyro_1.x > 360.0)
        gyro_1.x = gyro_1.x - 360.0;
    if(gyro_1.y > 360.0)
        gyro_1.y = gyro_1.y - 360.0;
    if(gyro_1.z > 360.0)
        gyro_1.z = gyro_1.z - 360.0;
    
    if(gyro_1.x < 0.0)
        gyro_1.x = gyro_1.x + 360.0;
    if(gyro_1.y < 0.0)
        gyro_1.y = gyro_1.y + 360.0;
    if(gyro_1.z < 0.0)
        gyro_1.z = gyro_1.z + 360.0;
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