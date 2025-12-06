#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"
//hoanghainam
//vuhunghai
//LeTanKhang
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

RTC_DS1307 rtc;

//khai báo biến cố định (nút)
#define OK_BUTTON 8
#define UP_BUTTON 9
#define MODE_BUTTON 10

//khai báo biến để hiển thị các mode
enum Mode { CLOCK_MODE,
            DISPLAY_MEMBER_MODE,
            NUM_MODES };

//biến toàn cục
Mode currentMode = CLOCK_MODE;

//biến trạng thái nút
bool buttonPress = false;

//khai báo biến để hiển thị thành viên nhóm:
enum members { NAM,
                 KHANG,
                 HAI,
                 HOANG,
                 NUM_MEMBER };
//Trạng thái nút up
bool up_buttonPress=false;
members curentMember = NAM;

void setup() {
  lcd.begin(16, 2);  // Khởi tạo màn hình

  if (!rtc.begin()) {
    lcd.print("Loi RTC");
    while (1)
      ;
  }

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  //cấu hình nút với điện trở ở bên ngoài
  pinMode(OK_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(MODE_BUTTON, INPUT_PULLUP);
}

void loop() {
  //tiến hành đọc nút
  int mode_buttonState = digitalRead(MODE_BUTTON);
  if (mode_buttonState == LOW && !buttonPress) {  //Nút đang được nhấn (vì dùng INPUT_PULLUP, nhấn = LOW)
    buttonPress = true;
    change_mode();
    delay(50);
  } else if (mode_buttonState == HIGH) {  //nếu nút không được nhấn
    buttonPress = false;
  }

  switch (currentMode) {
    case CLOCK_MODE:
      displayClock();
      break;
    case DISPLAY_MEMBER_MODE:
      displayMember();
      break;
  }
}
void change_mode() {
  currentMode = (Mode)(currentMode + 1) % 2;
  lcd.clear();
}
void displayClock() {
  DateTime now = rtc.now();

  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  if (now.hour() < 10) lcd.print("0");
  lcd.print(now.hour());
  lcd.print(':');
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());
  lcd.print(':');
  if (now.second() < 10) lcd.print("0");
  lcd.print(now.second());

  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());
}
void displayMember() {
  int up_buttonState = digitalRead(UP_BUTTON);
  if (up_buttonState == LOW && !up_buttonPress) {
    up_buttonPress = true;
    curentMember = (members)(curentMember + 1) % NUM_MEMBER;
    lcd.clear();
    delay(50);
  } else if (up_buttonState == HIGH){
    up_buttonPress=false;
  }

  switch (curentMember) {
    case NAM:
      lcd.setCursor(0, 0);
      lcd.print("HOANG HAI NAM");
      lcd.setCursor(0, 1);
      lcd.print("74DCTT28");
      break;
    case KHANG:
      lcd.setCursor(0, 0);
      lcd.print("LE TAN KHANG");
      lcd.setCursor(0, 1);
      lcd.print("74DCTT28");
      break;
    case HAI:
      lcd.setCursor(0, 0);
      lcd.print("VU HUNG HAI");
      lcd.setCursor(0, 1);
      lcd.print("74DCTT28");
      break;
    case HOANG:
      lcd.setCursor(0, 0);
      lcd.print("NGUYEN VIET HOANG");
      lcd.setCursor(0, 1);
      lcd.print("74DCTT28");
      break;
  }
}