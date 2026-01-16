#include <Wire.h>
#include <LiquidCrystal.h>
#include <RTClib.h>

// --- KHỞI TẠO ---
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);
RTC_DS1307 rtc;

// --- ĐỊNH NGHĨA CHÂN (PIN) ---
#define OK_BUTTON 8
#define UP_BUTTON 10
#define MODE_BUTTON 12
#define BUZZER_PIN 13

// --- CÁC MODE ---
enum Mode {
  CLOCK_MODE,
  DISPLAY_MEMBER_MODE,
  ALARM_MODE,
  STOPWATCH_MODE,  // <--- THÊM MỚI: Chế độ bấm giờ
  NUM_MODES
};

// --- CÁC THÀNH VIÊN ---
enum members {
  NAM,
  KHANG,
  HAI,
  HOANG,
  NUM_MEMBER
};

// --- BIẾN TOÀN CỤC ---
Mode currentMode = CLOCK_MODE;
members curentMember = NAM;

// --- BIẾN TRẠNG THÁI NÚT ---
bool mode_buttonPress = false;     // Nút MODE
bool up_buttonPress = false;  // Nút UP
bool ok_buttonPress = false;  // Nút OK

// --- BIẾN CHO BÁO THỨC ---
int alarmHour;
int alarmMinute;
bool isAlarmOn = false;
bool isRinging = false;
int alarmCursor = 0;

// --- BIẾN CHO BẤM GIỜ (STOPWATCH) - MỚI ---
unsigned long stopwatchStartMillis = 0;  // Thời điểm bắt đầu bấm
unsigned long stopwatchElapsed = 0;      // Thời gian đã trôi qua (lưu khi tạm dừng)
bool isStopwatchRunning = false;         // Trạng thái đang chạy hay dừng

void setup() {
  lcd.begin(16, 2);

  if (!rtc.begin()) {
    lcd.print("Loi RTC");
    while (1)
      ;
  }


  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  pinMode(OK_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(MODE_BUTTON, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  DateTime now = rtc.now();
  alarmHour = now.hour();
  alarmMinute = now.minute();
}

void loop() {
  // 1. Kiểm tra báo thức ngầm
  checkAlarm();

  // 2. Xử lý nút MODE (Chuyển chế độ)
  int mode_buttonState = digitalRead(MODE_BUTTON);
  if (mode_buttonState == LOW && ! mode_buttonPress) {
     mode_buttonPress = true;

    if (isRinging) {
      stopRinging();
    } else {
      change_mode();
    }
    delay(50);
  } else if (mode_buttonState == HIGH) {
     mode_buttonPress = false;
  }

  // 3. Hiển thị theo chế độ
  if (isRinging) {
    displayRinging();
  } else {
    switch (currentMode) {
      case CLOCK_MODE:
        displayClock();
        break;
      case DISPLAY_MEMBER_MODE:
        displayMember();
        break;
      case ALARM_MODE:
        displayAlarm();
        break;
      case STOPWATCH_MODE:  // <--- GỌI HÀM MỚI
        displayStopwatch();
        break;
    }
  }
}

void change_mode() {
  currentMode = (Mode)((currentMode + 1) % NUM_MODES);
  lcd.clear();
}

// --- HÀM KIỂM TRA BÁO THỨC ---
void checkAlarm() {
  if (!isRinging && isAlarmOn) {
    DateTime now = rtc.now();
    if (now.hour() == alarmHour && now.minute() == alarmMinute && now.second() == 0) {
      isRinging = true;
      lcd.clear();
    }
  }
}

// --- HÀM HIỂN THỊ KHI ĐANG REO ---
void displayRinging() {
  if ((millis() / 500) % 2 == 0) {
    lcd.setCursor(0, 0);
    lcd.print("!!! BAO THUC !!!");
    lcd.setCursor(0, 1);
    lcd.print(" DAY DI BAN OI  ");
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    lcd.clear();
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void stopRinging() {
  isRinging = false;
  isAlarmOn = false;
  digitalWrite(BUZZER_PIN, LOW);
  lcd.clear();
}

// --- MODE 1: ĐỒNG HỒ ---
void displayClock() {
  DateTime now = rtc.now();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  if (now.hour() < 10) {
    lcd.print("0");
    lcd.print(now.hour());
  } else {
    lcd.print(now.hour());
  }
  lcd.print(':');
  if (now.minute() < 10) {
    lcd.print("0");
    lcd.print(now.minute());
  } else {
    lcd.print(now.minute());
  }

  lcd.print(':');
  if (now.second() < 10) {
    lcd.print("0");
    lcd.print(now.second());
  } else {
    lcd.print(now.second());
  }



  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(now.day());
  lcd.print('/');
  lcd.print(now.month());
  lcd.print('/');
  lcd.print(now.year());
}

// --- MODE 2: THÀNH VIÊN ---
void displayMember() {
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
      lcd.print("NG. VIET HOANG");
      lcd.setCursor(0, 1);
      lcd.print("74DCTT28");
      break;
  }
}

// --- MODE 3: CHỈNH BÁO THỨC ---
void displayAlarm() {
  int ok_buttonState = digitalRead(OK_BUTTON);
  if (ok_buttonState == LOW && !ok_buttonPress) {
    ok_buttonPress = true;
    alarmCursor = (alarmCursor + 1) % 3;
    delay(50);
  } else if (ok_buttonState == HIGH) {
    ok_buttonPress = false;
  }

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

  lcd.setCursor(0, 0);
  lcd.print("SET ALARM: ");
  if (isAlarmOn) lcd.print("ON ");
  else lcd.print("OFF");

  lcd.setCursor(0, 1);
  if (alarmCursor == 0) lcd.print("[");
  if (alarmHour < 10) lcd.print("0");
  lcd.print(alarmHour);
  if (alarmCursor == 0) lcd.print("]");
  else lcd.print(" ");

  lcd.print(":");

  if (alarmCursor == 1) lcd.print("[");
  if (alarmMinute < 10) lcd.print("0");
  lcd.print(alarmMinute);
  if (alarmCursor == 1) lcd.print("]");
  else lcd.print(" ");

  if (alarmCursor == 2) {
    lcd.setCursor(15, 0);
    lcd.print("<");
  } else {
    lcd.setCursor(15, 0);
    lcd.print(" ");
  }
}

// --- MODE 4: BẤM GIỜ (STOPWATCH) - MỚI ---
void displayStopwatch() {
  // 1. Xử lý nút OK (Start / Stop)
  int ok_buttonState = digitalRead(OK_BUTTON);
  if (ok_buttonState == LOW && !ok_buttonPress) {
    ok_buttonPress = true;
    if (isStopwatchRunning) {
      // Đang chạy -> Bấm thì Dừng: Cộng dồn thời gian chạy vào biến tổng
      stopwatchElapsed += millis() - stopwatchStartMillis;
      isStopwatchRunning = false;
    } else {
      // Đang dừng -> Bấm thì Chạy: Lưu mốc thời gian bắt đầu
      stopwatchStartMillis = millis();
      isStopwatchRunning = true;
    }
    delay(50);
  } else if (ok_buttonState == HIGH) {
    ok_buttonPress = false;
  }

  // 2. Xử lý nút UP (Reset - Chỉ khi đang dừng)
  int up_buttonState = digitalRead(UP_BUTTON);
  if (up_buttonState == LOW && !up_buttonPress) {
    up_buttonPress = true;
    if (!isStopwatchRunning) {
      stopwatchElapsed = 0;  // Reset về 0
      lcd.clear();
    }
  } else if (up_buttonState == HIGH) {
    up_buttonPress = false;
  }

  // 3. Tính toán thời gian hiển thị
  unsigned long currentTotalTime = stopwatchElapsed;
  if (isStopwatchRunning) {
    currentTotalTime += millis() - stopwatchStartMillis;
  }

  // Quy đổi ra Phút:Giây:MiliGiây
  // (MiliGiây lấy hàng trăm để hiển thị cho gọn: 100ms = 1 đơn vị)
  unsigned long minutes = (currentTotalTime / 60000);
  unsigned long seconds = (currentTotalTime % 60000) / 1000;
  unsigned long hundredths = (currentTotalTime % 1000) / 10;

  // 4. Hiển thị lên LCD
  lcd.setCursor(0, 0);
  lcd.print(" StopWatch :  ");

  lcd.setCursor(3, 1);
  if (minutes < 10) lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);
  lcd.print(":");
  if (hundredths < 10) lcd.print("0");
  lcd.print(hundredths);

  // Hiển thị trạng thái nhỏ ở góc
  lcd.setCursor(15, 1);
  if (isStopwatchRunning) lcd.print(">");  // Ký hiệu Play
  else lcd.print("#");                     // Ký hiệu Pause
}