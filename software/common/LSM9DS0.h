#ifndef LSM9DS0_H_
#define LSM9DS0_H_

#include <stdint.h>

// I2C addresses
#define LSM9DS0_I2C_READ    0x01 //  000000  0 [1]
#define LSM9DS0_I2C_WRITE   0x00 //  000000  0 [0]
#define LSM9DS0_I2C_G_BASE  0xD4 // [110101] 0  0
#define LSM9DS0_I2C_G_LSA0  0x00 //  000000 [0] 0  Assuming pin output is low
#define LSM9DS0_I2C_G_HSA0  0x02 //  000000 [1] 0  Assuming pin output is high
#define LSM9DS0_I2C_XM_BASE 0x38 // [00111] 00  0
#define LSM9DS0_I2C_XM_LSA0 0x04 //  00000 [10] 0  Assuming pin output is low
#define LSM9DS0_I2C_XM_HSA0 0x02 //  00000 [01] 0  Assuming pin output is high

// General registers
#define LSM9DS0_FIFO_CTRL_REG 0x2E
#define LSM9DS0_FIFO_SRC_REG  0x2F

// Gyroscope (G) registers
#define LSM9DS0_WHO_AM_I_G      0x0F
#define LSM9DS0_WHO_AM_I_G_VAL  0xD4 // 1101 0100
#define LSM9DS0_CTRL_REG1_G     0x20
#define LSM9DS0_CTRL_REG2_G     0x21
#define LSM9DS0_CTRL_REG3_G     0x22
#define LSM9DS0_CTRL_REG4_G     0x23
#define LSM9DS0_CTRL_REG5_G     0x24
#define LSM9DS0_REFERENCE_G     0x25
#define LSM9DS0_STATUS_REG_G    0x27
#define LSM9DS0_OUT_X_L_G       0x28
#define LSM9DS0_OUT_X_H_G       0x29
#define LSM9DS0_OUT_Y_L_G       0x2A
#define LSM9DS0_OUT_Y_H_G       0x2B
#define LSM9DS0_OUT_Z_L_G       0x2C
#define LSM9DS0_OUT_Z_H_G       0x2D
#define LSM9DS0_INT1_CFG_G      0x30
#define LSM9DS0_INT1_SRC_G      0x31
#define LSM9DS0_INT1_THS_XH_G   0x32
#define LSM9DS0_INT1_THS_XL_G   0x33
#define LSM9DS0_INT1_THS_YH_G   0x34
#define LSM9DS0_INT1_THS_YL_G   0x35
#define LSM9DS0_INT1_THS_ZH_G   0x36
#define LSM9DS0_INT1_THS_ZL_G   0x37
#define LSM9DS0_INT1_DURATION_G 0x38

// Accelerometer and magnetometer (XM) registers
#define LSM9DS0_OUT_TEMP_L_XM      0x05
#define LSM9DS0_OUT_TEMP_H_XM      0x06
#define LSM9DS0_STATUS_REG_M       0x07
#define LSM9DS0_OUT_X_L_M          0x08
#define LSM9DS0_OUT_X_H_M          0x09
#define LSM9DS0_OUT_Y_L_M          0x0A
#define LSM9DS0_OUT_Y_H_M          0x0B
#define LSM9DS0_OUT_Z_L_M          0x0C
#define LSM9DS0_OUT_Z_H_M          0x0D
#define LSM9DS0_WHO_AM_I_XM        0x0F
#define LSM9DS0_WHO_AM_I_XM_VAL    0x49 // 0100 1001
#define LSM9DS0_INT_CTRL_REG_M     0x12
#define LSM9DS0_INT_SRC_REG_M      0x13
#define LSM9DS0_INT_THS_L_M        0x14
#define LSM9DS0_INT_THS_H_M        0x15
#define LSM9DS0_OFFSET_X_L_M       0x16
#define LSM9DS0_OFFSET_X_H_M       0x17
#define LSM9DS0_OFFSET_Y_L_M       0x18
#define LSM9DS0_OFFSET_Y_H_M       0x19
#define LSM9DS0_OFFSET_Z_L_M       0x1A
#define LSM9DS0_OFFSET_Z_H_M       0x1B
#define LSM9DS0_REFERENCE_X        0x1C
#define LSM9DS0_REFERENCE_Y        0x1D
#define LSM9DS0_REFERENCE_Z        0x1E
#define LSM9DS0_CTRL_REG0_XM       0x1F
#define LSM9DS0_CTRL_REG1_XM       0x20
#define LSM9DS0_CTRL_REG2_XM       0x21
#define LSM9DS0_CTRL_REG3_XM       0x22
#define LSM9DS0_CTRL_REG4_XM       0x23
#define LSM9DS0_CTRL_REG5_XM       0x24
#define LSM9DS0_CTRL_REG6_XM       0x25
#define LSM9DS0_CTRL_REG7_XM       0x26
#define LSM9DS0_STATUS_REG_A       0x27
#define LSM9DS0_OUT_X_L_A          0x28
#define LSM9DS0_OUT_X_H_A          0x29
#define LSM9DS0_OUT_Y_L_A          0x2A
#define LSM9DS0_OUT_Y_H_A          0x2B
#define LSM9DS0_OUT_Z_L_A          0x2C
#define LSM9DS0_OUT_Z_H_A          0x2D
#define LSM9DS0_INT_GEN_1_REG      0x30
#define LSM9DS0_INT_GEN_1_SRC      0x31
#define LSM9DS0_INT_GEN_1_THS      0x32
#define LSM9DS0_INT_GEN_1_DURATION 0x33
#define LSM9DS0_INT_GEN_2_REG      0x34
#define LSM9DS0_INT_GEN_2_SRC      0x35
#define LSM9DS0_INT_GEN_2_THS      0x36
#define LSM9DS0_INT_GEN_2_DURATION 0x37
#define LSM9DS0_CLICK_CFG          0x38
#define LSM9DS0_CLICK_SRC          0x39
#define LSM9DS0_CLICK_THS          0x3A
#define LSM9DS0_TIME_LIMIT         0x3B
#define LSM9DS0_TIME_LATENCY       0x3C
#define LSM9DS0_TIME_WINDOW        0x3D
#define LSM9DS0_ACT_THS            0x3E
#define LSM9DS0_ACT_DUR            0x3F

// Temperature drift
#define LSM9DS0_TEMPERATURE_LSB_PER_C     8
#define LSM9DS0_TEMPERATURE_SENSITIVITY_G 2   // % over -40..85°C
#define LSM9DS0_TEMPERATURE_SENSITIVITY_A 1.5 // % over -40..85°C
#define LSM9DS0_TEMPERATURE_SENSITIVITY_M 3   // % over -40..85°C
#define LSM9DS0_OFFSET_PER_CELSIUS_G      50  // +/-mdps/°C
#define LSM9DS0_OFFSET_PER_CELSIUS_A      0.5 // +/-mg/°C

/* Due to the lack of enum class on all platforms to guarantee equal data
 * sizes, use static bytes for each struct.
 */
struct LSM9DS0 {
    struct GScale {
        static const uint8_t G_SCALE_245DPS  = 0x0; // 00
        static const uint8_t G_SCALE_500DPS  = 0x1; // 01
        static const uint8_t G_SCALE_2000DPS = 0x2; // 10
    };

    struct AScale {
    	static const uint8_t A_SCALE_2G  = 0x0; // 000
    	static const uint8_t A_SCALE_4G  = 0x1; // 001
    	static const uint8_t A_SCALE_6G  = 0x2; // 010
    	static const uint8_t A_SCALE_8G  = 0x3; // 011
    	static const uint8_t A_SCALE_16G = 0x4; // 100
    };

    struct MScale {
    	static const uint8_t M_SCALE_2GS  = 0x0; // 00
    	static const uint8_t M_SCALE_4GS  = 0x1; // 01
    	static const uint8_t M_SCALE_8GS  = 0x2; // 10
    	static const uint8_t M_SCALE_12GS = 0x3; // 11
    };

    struct GODRCutoff {                                   // DR BW   ODR (Hz)   Cutoff
    	static const uint8_t G_ODR_95_CUTOFF_12_5  = 0x0; // 00 00    95         12.5
    	static const uint8_t G_ODR_95_CUTOFF_25    = 0x1; // 00 01    95         25
    	static const uint8_t G_ODR_190_CUTOFF_12_5 = 0x4; // 01 00   190         12.5
    	static const uint8_t G_ODR_190_CUTOFF_25   = 0x5; // 01 01   190         25
    	static const uint8_t G_ODR_190_CUTOFF_50   = 0x6; // 01 10   190         50
    	static const uint8_t G_ODR_190_CUTOFF_70   = 0x7; // 01 11   190         70
    	static const uint8_t G_ODR_380_CUTOFF_20   = 0x8; // 10 00   380         20
    	static const uint8_t G_ODR_380_CUTOFF_25   = 0x9; // 10 01   380         25
    	static const uint8_t G_ODR_380_CUTOFF_50   = 0xA; // 10 10   380         50
        static const uint8_t G_ODR_380_CUTOFF_100  = 0xB; // 10 11   380        100
        static const uint8_t G_ODR_760_CUTOFF_30   = 0xC; // 11 00   760         30
        static const uint8_t G_ODR_760_CUTOFF_35   = 0xD; // 11 01   760         35
        static const uint8_t G_ODR_760_CUTOFF_50   = 0xE; // 11 10   760         50
        static const uint8_t G_ODR_760_CUTOFF_100  = 0xF; // 11 11   760        100
    };

    struct GPowerMode {                                 // PD Zen Yen Xen
    	static const uint8_t G_POWER_MODE_DOWN   = 0x0; //  0   0   0   0
    	static const uint8_t G_POWER_MODE_SLEEP  = 0x8; //  1   0   0   0
    	static const uint8_t G_POWER_MODE_NORMAL = 0xF; //  1   1   1   1
    };

    struct AODR {
    	static const uint8_t A_POWER_DOWN = 0x0; // Power-down mode
    	static const uint8_t A_ODR_3_125  = 0x1; //    3.125 Hz
    	static const uint8_t A_ODR_6_25   = 0x2; //    6.25  Hz
    	static const uint8_t A_ODR_12_5   = 0x3; //   12.5   Hz
    	static const uint8_t A_ODR_25     = 0x4; //   25     Hz
    	static const uint8_t A_ODR_50     = 0x5; //   50     Hz
        static const uint8_t A_ODR_100    = 0x6; //  100     Hz
        static const uint8_t A_ODR_200    = 0x7; //  200     Hz
        static const uint8_t A_ODR_400    = 0x8; //  400     Hz
        static const uint8_t A_ODR_800    = 0x9; //  800     Hz
        static const uint8_t A_ODR_1600   = 0xA; // 1600     Hz
    };

    struct AABF {
    	static const uint8_t A_ABF_773 = 0x0; // 773 Hz
    	static const uint8_t A_ABF_194 = 0x1; // 194 Hz
    	static const uint8_t A_ABF_362 = 0x2; // 362 Hz
    	static const uint8_t A_ABF_50  = 0x3; //  50 Hz
    };

    struct MODR {
    	static const uint8_t M_ODR_3_125 = 0x0; //   3.125 Hz
		static const uint8_t M_ODR_6_25  = 0x1; //   6.25  Hz
		static const uint8_t M_ODR_1_25  = 0x2; //  12.5   Hz
		static const uint8_t M_ODR_25    = 0x3; //  25     Hz
		static const uint8_t M_ODR_50    = 0x4; //  50     Hz
		static const uint8_t M_ODR_100   = 0x5; // 100     Hz
    };

    struct MRES {
    	static const uint8_t M_RES_LOW  = 0x0;
    	static const uint8_t M_RES_HIGH = 0x3;
    };
};

#endif /* LSM9DS0_H_ */
