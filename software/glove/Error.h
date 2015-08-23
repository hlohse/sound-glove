#ifndef ERROR_H_
#define ERROR_H_

/* Provides a global field to signal an occured error.
 * Exceptions are undesired due to increased code size. As this code conforms
 * to RAII principles, it is necessary to signal errors within constructors
 * where no return value is available.
 * The info fields provides a more detailed information code to the error type.
 */

class Error {
public:
    enum Type {
        TYPE_NONE = 0x00,

        TYPE_BT_STACK_BSC_INIT   = 0x10,
		TYPE_BT_STACK_L2CAP_ROLE = 0x11,
		TYPE_BT_STACK_HCI_POLICY = 0x12,
        TYPE_BT_STACK_HCI_REG    = 0x13,
        TYPE_BT_STACK_HCI_VER    = 0x14,
		TYPE_BT_STACK_MBAUD      = 0x15,
		TYPE_BT_STACK_OUT_POWER  = 0x16,

        TYPE_BT_DEVICE_QUERY_BD_ADDR = 0x20,
        TYPE_BT_DEVICE_SET_CONN      = 0x21,
        TYPE_BT_DEVICE_SET_DISC      = 0x22,
        TYPE_BT_DEVICE_SET_PAIR      = 0x23,
        TYPE_BT_DEVICE_QUERY_CONN    = 0x24,
        TYPE_BT_DEVICE_QUERY_DISC    = 0x25,
        TYPE_BT_DEVICE_QUERY_PAIR    = 0x26,

        TYPE_BT_GAP_REG_AUTH  = 0x30,
		TYPE_BT_GAP_REG_NAME  = 0x31,
        TYPE_BT_GAP_IO_CAP    = 0x32,
        TYPE_BT_GAP_LINK_SEND = 0x33,
        TYPE_BT_GAP_USER_CONF = 0x34,

        TYPE_BT_SPP_OPEN        = 0x40,
        TYPE_BT_SPP_CONN_MODE   = 0x41,
        TYPE_BT_SPP_SDP_REG     = 0x42,
        TYPE_BT_SPP_OPEN_REQ    = 0x43,
        TYPE_BT_SPP_CONN_HANDLE = 0x44,
        TYPE_BT_SPP_INFO        = 0x45,
        TYPE_BT_SPP_CLEAR       = 0x46,
        TYPE_BT_SPP_CONFIG      = 0x47,
		TYPE_BT_SPP_SNIFF_ENTER = 0x48,
		TYPE_BT_SPP_SNIFF_EXIT  = 0x49,

        TYPE_BT_SERIAL_PORT_READ_EXCEED    = 0x50,
        TYPE_BT_SERIAL_PORT_DATA_READ      = 0x51,
        TYPE_BT_SERIAL_PORT_WRITE_EXCEED   = 0x52,
        TYPE_BT_SERIAL_PORT_WRITELN_EXCEED = 0x53,
        TYPE_BT_SERIAL_PORT_DATA_WRITE     = 0x54,
		TYPE_BT_SERIAL_PORT_PEEK_EXCEED    = 0x55,
		TYPE_BT_SERIAL_PORT_READ_NULL      = 0x56,
		TYPE_BT_SERIAL_PORT_WRITE_NULL     = 0x57,
        TYPE_BT_SERIAL_PORT_WRITE_RAW      = 0x58,
        TYPE_BT_SERIAL_PORT_WRITE_LEN      = 0x59,
        TYPE_BT_SERIAL_PORT_WRITE_FITS     = 0x5A,

        TYPE_SCHEDULER_SCHEDULE = 0x60,

		TYPE_SERIALIZER_EXCEED     = 0x70,
		TYPE_SERIALIZER_BIT_EXCEED = 0x71,

		TYPE_DESERIALIZER_EXCEED     = 0x80,
		TYPE_DESERIALIZER_BIT_EXCEED = 0x81,

		TYPE_MARG_INIT        = 0x90,
		TYPE_MARG_WHO_AM_I_G  = 0x91,
		TYPE_MARG_WHO_AM_I_AM = 0x92,
		TYPE_MARG_MEAN        = 0x93,
		TYPE_MARG_BLOCK_ASYNC = 0x94,

		TYPE_FLASH_READ_EXCEED  = 0xA0,
		TYPE_FLASH_WRITE_EXCEED = 0xA1,

		TYPE_ADC_INIT = 0xB0,

		TYPE_RING_BUFFER_PUSH_FULL  = 0xC0,
		TYPE_RING_BUFFER_POP_EMPTY  = 0xC1,
		TYPE_RING_BUFFER_PEEK_EMPTY = 0xC2,

		TYPE_SERIALIZATION_SIZE_EXCEED = 0xD0
    };

    static const int no_info = 0;

    static Error& instance();

    bool isNone() const;

    void set(const Type type, const int info);

    Type type() const;
    int info() const;

private:
    Type type_;
    int info_;

    Error();

    // Disallow copies; (do not implement!)
    Error(const Error&);
    void operator=(const Error&);
};

#endif /* ERROR_H_ */
