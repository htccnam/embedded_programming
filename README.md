# embedded_programming
"16. Thiết kế Hệ thống đồng hồ báo giờ
1. Hiển thị thông tin đề tài/thành viên nhóm và các thông báo khác trên LCD
2. Sử dụng module thời gian thực để hiển thị thời gian thực ( 1 múi giờ )
3. Chức năng hẹn giờ
4. Chức năng bấm giờ (như đồng hồ thể thao)
5. Sử dụng nút bấm để lựa chọn / thay đổi các chức năng"
<CODE CHÍNH NẰM TRONG FILE "final_code" DEMO PROTEUS CHÍNH NẰM TRONG "final_demo"
 ## mô tả

# ⏰ HỆ THỐNG ĐỒNG HỒ – BÁO THỨC – BẤM GIỜ (ARDUINO)

## 📌 Giới thiệu

Dự án **Hệ thống Đồng hồ – Báo thức – Bấm giờ** là một ứng dụng **lập trình nhúng trên Arduino**, sử dụng **LCD 16x2**, **RTC DS1307**, các **nút nhấn vật lý** và **buzzer** để xây dựng một thiết bị đa chức năng hiển thị thời gian.

Hệ thống cho phép:

* Hiển thị **thời gian & ngày tháng thực**
* Cài đặt và kích hoạt **báo thức**
* **Bấm giờ (Stopwatch)** với chức năng Start / Stop / Reset
* Hiển thị **thông tin thành viên**
* Điều khiển hoàn toàn bằng **nút nhấn**

Phù hợp cho:

* Đồ án **Lập trình nhúng**
* Thực hành **Arduino – Embedded Systems**
* Tham khảo thiết kế **Finite State Machine (FSM)** đơn giản

---

## 🎯 Mục tiêu dự án

* Làm quen với:

  * Arduino & lập trình C/C++
  * Giao tiếp **I2C (RTC DS1307)**
  * Điều khiển **LCD 16x2**
* Xử lý:

  * Nút nhấn (debounce logic)
  * Đa chế độ hoạt động (Mode)
* Áp dụng tư duy **state-based programming**

---

## 🛠 Phần cứng sử dụng

* **Arduino Uno**
* **LCD 16x2** (chế độ 4-bit)
* **RTC DS1307**
* **Buzzer**
* **Nút nhấn** (MODE, OK, UP)
* Điện trở, breadboard, dây nối

---

## 💻 Phần mềm & thư viện

* **Arduino IDE**
* Thư viện:

  * `Wire.h`
  * `LiquidCrystal.h`
  * `RTClib.h`

---

## 🔌 Sơ đồ chân kết nối

### LCD 16x2

| LCD | Arduino |
| --- | ------- |
| RS  | D2      |
| E   | D3      |
| D4  | D4      |
| D5  | D5      |
| D6  | D6      |
| D7  | D7      |

### Nút nhấn & Buzzer

| Thiết bị    | Arduino |
| ----------- | ------- |
| MODE Button | D12     |
| OK Button   | D8      |
| UP Button   | D10     |
| Buzzer      | D13     |

> Các nút sử dụng **INPUT_PULLUP**

---

## 🧩 Các chế độ hoạt động (Mode)

| Mode                  | Chức năng                               |
| --------------------- | --------------------------------------- |
| `CLOCK_MODE`          | Hiển thị giờ – phút – giây & ngày tháng |
| `DISPLAY_MEMBER_MODE` | Hiển thị thông tin thành viên           |
| `ALARM_MODE`          | Cài đặt & bật/tắt báo thức              |
| `STOPWATCH_MODE`      | Bấm giờ (Start / Stop / Reset)          |

Chuyển mode bằng **nút MODE**

---

## ⏱️ Chức năng nổi bật

### 🔔 Báo thức

* Cài đặt **giờ & phút**
* Bật / tắt báo thức
* Khi đến giờ:

  * LCD nhấp nháy
  * Buzzer kêu liên tục
* Nhấn **MODE** để tắt báo thức

---

### ⏲️ Bấm giờ (Stopwatch)

* **OK:** Start / Stop
* **UP:** Reset (chỉ khi đang dừng)
* Hiển thị:

  * Phút : Giây : Mili-giây (100ms)
* Ký hiệu:

  * `>` đang chạy
  * `#` đang dừng

---

## 🚀 Cách clone & chạy dự án

### 1️⃣ Clone project

```bash
git clone https://github.com/htccnam/embedded_programming.git
```

Hoặc tải **Download ZIP** từ GitHub.

---

### 2️⃣ Mở project

* Mở **Arduino IDE**
* Mở file `.ino` trong thư mục dự án

---

### 3️⃣ Cài thư viện (nếu thiếu)

Vào:

```
Sketch → Include Library → Manage Libraries
```

Tìm và cài:

* RTClib
* LiquidCrystal

---

### 4️⃣ Nạp chương trình

* Chọn **Board:** Arduino Uno
* Chọn **Port**
* Nhấn **Upload**

---

## 📂 Cấu trúc chương trình

* `setup()` – Khởi tạo LCD, RTC, chân I/O
* `loop()` – Vòng lặp chính
* Xử lý theo **Mode**
* Mỗi mode có **hàm hiển thị riêng**
* Sử dụng `enum` để quản lý trạng thái

---

## 📌 Ghi chú

* Thời gian RTC được đồng bộ theo **thời điểm biên dịch**
* Có thể mở rộng:

  * Thêm EEPROM lưu báo thức
  * Thêm chỉnh giờ bằng nút
  * Thêm cảm biến nhiệt độ
  * Thêm menu đa cấp

---

## 📄 License

This project is licensed under the **MIT License** – free to use for learning and educational purposes.
