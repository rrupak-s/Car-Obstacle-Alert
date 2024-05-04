#include <NewPing.h>
#include <MedianFilter.h>
#include <Wire.h>
#include <math.h>

unsigned char Buffer[9];

#define MAX_DISTANCE 250

#define TRIGGER_PIN_1 30
#define ECHO_PIN_1 28

#define TRIGGER_PIN_2 42
#define ECHO_PIN_2 40

#define TRIGGER_PIN_3 32
#define ECHO_PIN_3 34

#define TRIGGER_PIN_4 36
#define ECHO_PIN_4 38

#define display 50

NewPing sonar1(TRIGGER_PIN_1, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN_3, ECHO_PIN_3, MAX_DISTANCE);
NewPing sonar4(TRIGGER_PIN_4, ECHO_PIN_4, MAX_DISTANCE);

MedianFilter filter1(4, 0);
MedianFilter filter2(4, 0);
MedianFilter filter3(4, 0);
MedianFilter filter4(4, 0);

int filteredDistance1 = 0, filteredDistance2 = 0, filteredDistance3 = 0, filteredDistance4 = 0;
int thres_low = 50, thres_mid = 150, thres_high = 200;
// int disp_low = 05, disp_mid = 15, disp_high = 20;
volatile int flag = -1;
volatile int disp_flag = -1;

// IDs: 0x5100, 0x5200, 0x5300, and 0x5400
unsigned char data_b[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x51, 0x00, 0x00, 0x00 };
unsigned char data_a[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x52, 0x00, 0x00, 0x00 };
unsigned char data_c[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x53, 0x00, 0x00, 0x00 };
unsigned char data_d[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x54, 0x00, 0x00, 0x00 };
unsigned char buzz[8] = { 0x5A, 0xA5, 0x05, 0x82, 0x00, 0xA0, 0x00, 0x3E };

void setup() {
  Serial.begin(115200);
  Serial3.begin(115200);
  pinMode(display, OUTPUT);
  digitalWrite(display, HIGH);
  dwindisp_2(0);
  dwindisp(0);
  dwintext(00);
}

void dwinbuzz() {
  Serial3.write(buzz, sizeof(buzz));
}

void dwindisp(int data) {
  data_b[6] = highByte(data);
  data_b[7] = lowByte(data);

  Serial3.write(data_b, sizeof(data_b));
}

void dwintext(int data) {
  data_c[6] = highByte(data);
  data_c[7] = lowByte(data);
  Serial3.write(data_c, sizeof(data_c));
}

void dwindisp_2(int data) {
  data_a[6] = highByte(data);
  data_a[7] = lowByte(data);

  Serial3.write(data_a, sizeof(data_a));
}

void serialread() {
  while (Serial3.available() >= sizeof(Buffer)) {
    for (int i = 0; i < sizeof(Buffer); i++) {
      Buffer[i] = Serial3.read();
    }

    if (Buffer[0] == 0X5A) {
      switch (Buffer[4]) {
        case 0x54:  // for led
          if (Buffer[8] == 1) {
            flag = 1;
          } else {
            flag = 0;
          }
          break;
      }
    }
  }
}

void loop() {
  int cm1 = sonar1.ping_cm();
  int cm2 = sonar2.ping_cm();
  int cm3 = sonar3.ping_cm();
  int cm4 = sonar4.ping_cm();


  // Sensor 1

  filter1.in(cm1);
  filteredDistance1 = filter1.out();
  Serial.print(filteredDistance1);  // Send ping, get filtered distance in cm and print result
  Serial.print("   ");



  // Sensor 2

  filter2.in(cm2);
  filteredDistance2 = filter2.out();
  Serial.print(filteredDistance2);  // Send ping, get filtered distance in cm and print result
  Serial.print("   ");

  // Sensor 3

  filter3.in(cm3);
  filteredDistance3 = filter3.out();
  Serial.print(filteredDistance3);  // Send ping, get filtered distance in cm and print result
  Serial.print("   ");

  // Sensor 4

  filter4.in(cm4);
  filteredDistance4 = filter4.out();
  Serial.print(filteredDistance4);  // Send ping, get filtered distance in cm and print result
  Serial.println("   ");




  // Add your logic here based on sensor readings

  if ((thres_high >= filteredDistance1) && (filteredDistance1 > thres_mid) && (filteredDistance2 > thres_high || filteredDistance2 == 0)) {
    dwindisp(1);

    int disp = ceil(filteredDistance1 / 10);
    // char formattedNumber[3];
    // sprintf(formattedNumber, "%02d", disp);

    dwintext(disp);
    disp_falg = 0;
    digitalWrite(display, HIGH);

    // dwinbuzz();

    //
  } else if ((thres_mid >= filteredDistance1) && (filteredDistance1 > thres_low) && (filteredDistance2 > thres_mid || filteredDistance2 == 0)) {
    dwindisp(2);
    int disp = ceil(filteredDistance1 / 10);
    //  char formattedNumber[3];
    // sprintf(formattedNumber, "%02d", disp);

    dwintext(disp);
    disp_falg = 0;
    digitalWrite(display, LOW);
    dwinbuzz();


  } else if ((thres_low >= filteredDistance1 && filteredDistance1 > 0) && (filteredDistance2 > thres_mid || filteredDistance2 == 0)) {
    dwindisp(3);
    int disp = ceil(filteredDistance1 / 10);
    //  char formattedNumber[3];
    // sprintf(formattedNumber, "%02d", disp);

    dwintext(disp);
    disp_falg = 0;
    digitalWrite(display, LOW);


    dwinbuzz();


  } else if ((thres_high >= filteredDistance2) && (filteredDistance2 > thres_mid) && (filteredDistance1 > thres_high || filteredDistance1 == 0)) {
    dwindisp(4);
    int disp = ceil(filteredDistance2 / 10);
    // char formattedNumber[3];
    // sprintf(formattedNumber, "%02d", disp);

    dwintext(disp);
    disp_falg = 0;
    digitalWrite(display, HIGH);

    //  dwinbuzz();


  } else if ((thres_mid >= filteredDistance2) && (filteredDistance2 > thres_low) && (filteredDistance1 > thres_mid || filteredDistance1 == 0)) {
    dwindisp(5);
    int disp = ceil(filteredDistance2 / 10);
    // char formattedNumber[3];
    // sprintf(formattedNumber, "%02d", disp);

    dwintext(disp);
    disp_falg = 0;
    digitalWrite(display, LOW);


    dwinbuzz();


  } else if ((thres_low >= filteredDistance2 && filteredDistance2 > 0) && (filteredDistance1 > thres_mid || filteredDistance1 == 0)) {
    dwindisp(6);
    int disp = ceil(filteredDistance2 / 10);
    // char formattedNumber[3];
    // sprintf(formattedNumber, "%02d", disp);

    dwintext(disp);
    disp_falg = 0;
    digitalWrite(display, LOW);

    dwinbuzz();


  } else if ((thres_high >= filteredDistance1 && filteredDistance1 > thres_mid) && (thres_high >= filteredDistance2 && filteredDistance2 > thres_mid)) {
    dwindisp(7);

    // char formattedNumber[3];
    // sprintf(formattedNumber, "%02d", disp);

    if (filteredDistance1 < filteredDistance2) {
      int disp = ceil(filteredDistance1 / 10);
      dwintext(disp);
    } else {
      int disp = ceil(filteredDistance2 / 10);
      dwintext(disp);
    }
    disp_falg = 0;
    digitalWrite(display, HIGH);



  } else if ((thres_mid >= filteredDistance1 && filteredDistance1 > thres_low) && (thres_mid >= filteredDistance2 && filteredDistance2 > thres_low)) {
    dwindisp(8);
    int disp = ceil(filteredDistance1 / 10);
    if (filteredDistance1 < filteredDistance2) {
      int disp = ceil(filteredDistance1 / 10);
      dwintext(disp);

    } else {
      int disp = ceil(filteredDistance2 / 10);
      dwintext(disp);
    }
    disp_falg = 0;
    digitalWrite(display, LOW);
    dwinbuzz();

  } else if ((thres_low >= filteredDistance1 && filteredDistance1 > 0) && (thres_low >= filteredDistance2 && filteredDistance2 > 0)) {
    dwindisp(9);
    if (filteredDistance1 < filteredDistance2) {
      int disp = ceil(filteredDistance1 / 10);
      dwintext(disp);

    } else {
      int disp = ceil(filteredDistance2 / 10);
      dwintext(disp);
    }
    disp_falg = 0;
    digitalWrite(display, LOW);
    dwinbuzz();

  }

  else {
    dwindisp(0);
    dwintext(00);
    disp_flag = 1;
    digitalWrite(display, HIGH);
  }

  if ((filteredDistance3 <= 100 && filteredDistance3 > 0) || (filteredDistance4 <= 100 && filteredDistance4 > 0)) {
    dwindisp_2(1);
    if (disp_flag == 1) {
      if (filteredDistance3 < filteredDistance4) {
        int disp = ceil(filteredDistance3 / 10);
        dwintext(disp);

      } else {
        int disp = ceil(filteredDistance4 / 10);
        dwintext(disp);
      }
    }
    dwinbuzz();
  } else {
    dwindisp_2(0);
  }

  delay(50);

  // Continue with the rest of your code
}
