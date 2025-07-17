MPU 60x0
========


Introduction
------------



Power Management
----------------

Initially, the sensor is in sleeping mode. To wake it up, follow the procedure below:

1 - Write 0100_0000 to register 106
2 - Write 1010_0101 to register 107
3 - Write 0100_0000 to register 108

That is:

1 - Register 106 - User Control

    DMP_EN       = 0        Disable DMP (?) features
    FIFO_EN      = 1        Enable FIFO operation
    I2C_MST_EN   = 0
    I2C_IF_DIS   = 0
    DMP_RST      = 0
    FIFO_RST     = 0
    I2C_MST_RST  = 0
    SIG_COND_RST = 0

2 - Register 107 - Power Management 1

    DEVICE_RESET = 1        Reset device
    SLEEP        = 0        Wake it up
    CYCLE        = 1        Cycle between sleep and a single sample a a rate specified by LP_WAKE_CTRL
    GYRO_STANDBY = 0
    TEMP_DIS     = 0        Enable temperature sensor
    CLKSEL       = 101      Auto select clock source

3 - Register 108 - Power Management 2

    LP_WAKE_CTRL = 01       Frequency of wake ups
    DISABLE_XA   = 0        Enable X accelerometer
    DISABLE_YA   = 0        Enable X accelerometer
    DISABLE_ZA   = 0        Enable X accelerometer
    DISABLE_XG   = 0        Enable X gyro
    DISABLE_YG   = 0        Enable Y gyro
    DISABLE_ZG   = 0        Enable Z gyro





References
----------
