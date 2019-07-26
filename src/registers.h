//Register Pointer Map
enum Qwiic_Button_Register: uint8_t {
    BUTTON_STATUS = 0x00,
    BUTTON_DEBOUNCE_TIME = 0x01,
    INTERRUPT_CONFIG = 0x03,
    PRESSED_QUEUE_STATUS = 0x04,
    PRESSED_QUEUE_FRONT = 0x05,
    PRESSED_QUEUE_BACK = 0x09,
    CLICKED_QUEUE_STATUS = 0x0D,
    CLICKED_QUEUE_FRONT = 0x0E,
    CLICKED_QUEUE_BACK = 0x12,
    LED_BRIGHTNESS = 0x16,
    LED_PULSE_GRANULARITY = 0x17,
    LED_PULSE_CYCLE_TIME = 0x18,
    LED_PULSE_OFF_TIME = 0x1A,
    I2C_ADDRESS = 0x1C,
    ID = 0x1D,
    FIRMWARE_MINOR = 0x1E,
    FIRMWARE_MAJOR = 0x1F,
};

typedef union {
    struct {
        bool : 6;
        bool isPressed : 1;  //not mutable by user, set to zero if button is not pushed, set to one if button is pushed
        bool hasBeenClicked : 1; //mutable by user, basically behaves like an interrupt. Defaults to zero on POR, but gets set to one every time the button gets clicked. Can be cleared by the user, and that happens regularly in the accompnaying arduino library
    };
    uint8_t byteWrapped;
} statusRegisterBitField;

typedef union {
    struct {
        bool: 4;
        bool pressedEnable : 1; //user mutable, set to 1 to enable an interrupt when the button is pressed. Defaults to 0.
        bool clickedEnable : 1; //user mutable, set to 1 to enable an interrupt when the button is clicked. Defaults to 0.
        bool logicLevel : 1; //user mutable, set to 0 for active low, set to 1 for active high. Defaults to 0.
        bool status : 1; //user mutable, gets set to 1 when the interrupt is triggered. User is expected to write 0 to clear the interrrupt.
    };
    uint8_t byteWrapped;
} interrruptConfigBitField;

typedef union {
    struct {
        bool: 5;
        bool isFull : 1; //user immutable, returns 1 or 0 depending on whether or not the queue is full
        bool isEmpty : 1; //user immutable, returns 1 or 0 depending on whether or not the queue is empty
        bool popRequest : 1; //user mutable, user sets to 1 to pop from queue, we pop from queue and set the bit back to zero.
    };
    uint8_t byteWrapped;
} queueStatusBitField;