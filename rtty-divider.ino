#define DTX 6

#define MARK 105 // 170Hz 3
#define SPACE 100 // 0
#define SPEED 22 // 45.45 bauds => 1/45.45 = 22 ms

#define SHIFT_LTRS 0x1f
#define SHIFT_FIGS 0x1b

const char FIGURES[] = " \r\n1234567890-\a@!&#'()\"/:;?,.";
const char LETTERS[] = " \r\nQWERTYUIOPASDFGHJKLZXCVBNM";
const byte baudot[] = { 0x4, 0x8, 0x2, 0x1d, 0x19, 0x10, 0xa, 0x1, 0x15, 0x1c, 0xc, 0x3, 0xd, 0x18, 0x14, 0x12, 0x16, 0xb, 0x5, 0x1a, 0x1e, 0x9, 0x11, 0x17, 0xe, 0xf, 0x13, 0x6, 0x7 };

void rtty_txBit(bool bit) {
    if(bit) {
        digitalWrite(DTX, HIGH);
    }
    else {
        digitalWrite(DTX, LOW);
    }
    
    delay(SPEED);
}

void rtty_txByte(byte b) {
    rtty_txBit(0); // Start bit

    // Send byte, bit by bit starting from left (MSB)
    for(byte i = 5; i > 0 ; i--) {
        if(b & (1 << (i-1)))
        {
            rtty_txBit(1);
        }
        else {
            rtty_txBit(0);
        }
    }

    rtty_txBit(1); // Stop bits
    rtty_txBit(1);
}

void rtty_send(char * szMessage) {
    char * p = szMessage;
    char * find = NULL;
    char c = *p;
    byte index = 0;
    short shift = 0;

    while(*p) {
        find = strchr(FIGURES, c);
        if(find != NULL) {
            index = find - FIGURES;
            if(index > 2 && shift != 1) {
                shift = 1;
                rtty_txByte(SHIFT_FIGS);
            }

            rtty_txByte(baudot[index]);
        }
        else {
            c = toupper(c);
            find = strchr(LETTERS, c);
            if(find != NULL) {
                index = find - LETTERS;
                if(index > 2 && shift != 2) {
                    shift = 2;
                    rtty_txByte(SHIFT_LTRS);
                }

                rtty_txByte(baudot[index]);
            }
        }

        p++;
        c = *p;
    }
}

void setup() {
    pinMode(DTX, OUTPUT);
}

void loop() {
    rtty_send("CQ CQ ON3DEX CQ CQ ON3DEX ");
}