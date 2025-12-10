#include <Wire.h>
#include <LiquidCrystal.h>
#include "RTClib.h"

// --- KHỞI TẠO ---
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
RTC_DS1307 rtc;

// --- ĐỊNH NGHĨA CHÂN (PIN) ---
#define OK_BUTTON 8
#define UP_BUTTON 9
#define MODE_BUTTON 10
#define BUZZER_PIN 11 // Chân còi báo động

// --- CÁC MODE ---
enum Mode { 
  CLOCK_MODE,
  DISPLAY_MEMBER_MODE,
  ALARM_MODE, // Thêm chế độ hẹn giờ
  NUM_MODES 
};

// --- CÁC THÀNH VIÊN ---
enum members { 
  NAM, KHANG, HAI, HOANG, NUM_MEMBER 
};

// --- BIẾN TOÀN CỤC ---
Mode currentMode = CLOCK_MODE;
members curentMember = NAM;

// --- BIẾN TRẠNG THÁI NÚT (GIỮ NGUYÊN KIỂU CŨ) ---
bool buttonPress = false;    // Trạng thái nút MODE
bool up_buttonPress = false; // Trạng thái nút UP
bool ok_buttonPress = false; // Trạng thái nút OK (thêm mới cho đúng bộ)

// --- BIẾN CHO BÁO THỨC ---
int alarmHour;      // Khai báo biến
int alarmMinute;    // Khai báo biến
bool isAlarmOn = false; // Bật/Tắt báo thức
bool isRinging = false; // Đang reo hay không
int alarmCursor = 0;    // Con trỏ chỉnh: 0=Giờ, 1=Phút, 2=On/Off

void setup() {
  lcd.begin(16, 2);

  if (!rtc.begin()) {
    lcd.print("Loi RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Cấu hình nút
  pinMode(OK_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(MODE_BUTTON, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // --- LẤY GIỜ HIỆN TẠI LÀM MẶC ĐỊNH ---
  DateTime now = rtc.now();
  alarmHour = now.hour();
  alarmMinute = now.minute();
}

void loop() {
  // 1. Kiểm tra báo thức
  checkAlarm();

  // 2. Xử lý nút MODE (Dùng biến buttonPress cũ của bạn)
  int mode_buttonState = digitalRead(MODE_BUTTON);
  if (mode_buttonState == LOW && !buttonPress) { 
    buttonPress = true;
    
    if (isRinging) {
      stopRinging(); // Nếu đang reo thì tắt
    } else {
      change_mode(); // Nếu không thì chuyển chế độ
    }
    delay(50);
  } else if (mode_buttonState == HIGH) {
    buttonPress = false;
  }

  // 3. Hiển thị
  if (isRinging) {
    displayRinging(); // Hiển thị chữ khi báo thức
  } else {
    switch (currentMode) {
      case CLOCK_MODE:
        displayClock();
        break;
      case DISPLAY_MEMBER_MODE:
        displayMember();
        break;
      case ALARM_MODE:
        displayAlarm(); // Hàm hiển thị chỉnh báo thức
        break;
    }
  }
}

void change_mode() {
  // Sửa % 2 thành % NUM_MODES để chạy được 3 chế độ
  currentMode = (Mode)((currentMode + 1) % NUM_MODES);
  lcd.clear();
}

// --- HÀM KIỂM TRA BÁO THỨC ---
void checkAlarm() {
  if (!isRinging && isAlarmOn) {
    DateTime now = rtc.now();
    // So sánh giờ phút giây
    if (now.hour() == alarmHour && now.minute() == alarmMinute && now.second() == 0) {
      isRinging = true;
      lcd.clear();
    }
  }
}

// --- HÀM HIỂN THỊ KHI ĐANG REO ---
void displayRinging() {
  // Nhấp nháy chữ mỗi 0.5 giây
  if ((millis() / 500) % 2 == 0) {
    lcd.setCursor(0, 0);
    lcd.print("!!! BAO THUC !!!");
    lcd.setCursor(0, 1);
    lcd.print(" DAY DI BAN OI  "); // Bạn có thể sửa nội dung này
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    lcd.clear(); // Tắt chữ tạo hiệu ứng nhấp nháy
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void stopRinging() {
  isRinging = false;
  isAlarmOn = false; // Tắt báo thức sau khi đã reo
  digitalWrite(BUZZER_PIN, LOW);
  lcd.clear();
}

// --- MODE 1: ĐỒNG HỒ ---
void displayClock() {
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  if (now.hour() < 10) lcd.print("0"); lcd.print(now.hour());
  lcd.print(':');
  if (now.minute() < 10) lcd.print("0"); lcd.print(now.minute());
  lcd.print(':');
  if (now.second() < 10) lcd.print("0"); lcd.print(now.second());

  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day()); lcd.print('/');
  lcd.print(now.month()); lcd.print('/');
  lcd.print(now.year());
}

// --- MODE 2: THÀNH VIÊN ---
void displayMember() {
  // Dùng biến up_buttonPress cũ của bạn
  int up_buttonState = digitalRead(UP_BUTTON);
  if (up_buttonState == LOW && !up_buttonPress) {
    up_buttonPress = true;
    curentMember = (members)((curentMember + 1) % NUM_MEMBER);
    lcd.clear();
    delay(50);
  } else if (up_buttonState == HIGH) {
    up_buttonPress = false;
  }

  switch (curentMember) {
    case NAM:
      lcd.setCursor(0, 0); lcd.print("HOANG HAI NAM");
      lcd.setCursor(0, 1); lcd.print("74DCTT28"); break;
    case KHANG:
      lcd.setCursor(0, 0); lcd.print("LE TAN KHANG");
      lcd.setCursor(0, 1); lcd.print("74DCTT28"); break;
    case HAI:
      lcd.setCursor(0, 0); lcd.print("VU HUNG HAI");
      lcd.setCursor(0, 1); lcd.print("74DCTT28"); break;
    case HOANG:
      lcd.setCursor(0, 0); lcd.print("NGUYEN VIET HOANG");
      lcd.setCursor(0, 1); lcd.print("74DCTT28"); break;
  }
}

// --- MODE 3: CHỈNH BÁO THỨC ---
void displayAlarm() {
  // 1. Xử lý nút OK (Dùng biến ok_buttonPress mới tạo theo kiểu cũ)
  int ok_buttonState = digitalRead(OK_BUTTON);
  if (ok_buttonState == LOW && !ok_buttonPress) {
    ok_buttonPress = true;
    alarmCursor = (alarmCursor + 1) % 3; // Chuyển vị trí: Giờ -> Phút -> Bật/Tắt
    delay(50);
  } else if (ok_buttonState == HIGH) {
    ok_buttonPress = false;
  }

  // 2. Xử lý nút UP (Dùng lại biến up_buttonPress)
  int up_buttonState = digitalRead(UP_BUTTON);
  if (up_buttonState == LOW && !up_buttonPress) {
    up_buttonPress = true;
    if (alarmCursor == 0) alarmHour = (alarmHour + 1) % 24;
    else if (alarmCursor == 1) alarmMinute = (alarmMinute + 1) % 60;
    else if (alarmCursor == 2) isAlarmOn = !isAlarmOn;
    delay(50);
  } else if (up_buttonState == HIGH) {
    up_buttonPress = false;
  }

  // 3. Hiển thị
  lcd.setCursor(0, 0);
  lcd.print("SET ALARM: ");
  if (isAlarmOn) lcd.print("ON "); else lcd.print("OFF");

  lcd.setCursor(0, 1);
  
  // Vẽ dấu ngoặc [] quanh số đang chỉnh
  if (alarmCursor == 0) lcd.print("["); 
  if (alarmHour < 10) lcd.print("0"); lcd.print(alarmHour);
  if (alarmCursor == 0) lcd.print("]"); else lcd.print(" ");
  
  lcd.print(":");

  if (alarmCursor == 1) lcd.print("[");
  if (alarmMinute < 10) lcd.print("0"); lcd.print(alarmMinute);
  if (alarmCursor == 1) lcd.print("]"); else lcd.print(" ");
  
  // Dấu mũi tên chỉ vào ON/OFF
  if (alarmCursor == 2) {
    lcd.setCursor(15, 0); lcd.print("<");
  } else {
    lcd.setCursor(15, 0); lcd.print(" ");
  }
}