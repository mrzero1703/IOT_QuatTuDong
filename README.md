# Hệ thống quạt tự động dùng Arduino

Dự án này xây dựng một hệ thống quạt tự động sử dụng Arduino, cảm biến nhiệt độ DHT11, màn hình LCD I2C, driver động cơ DC và servo để điều khiển chức năng đảo gió. Hệ thống có thể hoạt động ở chế độ tự động theo nhiệt độ hoặc chế độ thủ công theo mức tốc độ người dùng chọn.
<img src="https://sf-static.upanhlaylink.com/img/image_20260520d357876415d942011facb93114e84217.jpg" alt="iot_map.png">   
## 1. Chức năng chính

- Bật/tắt toàn bộ hệ thống bằng nút nhấn.
- Đọc nhiệt độ môi trường bằng cảm biến DHT11.
- Điều khiển tốc độ quạt DC bằng PWM.
- Hỗ trợ 2 chế độ hoạt động:
  - **AUTO**: tự động thay đổi tốc độ quạt theo nhiệt độ.
  - **MANUAL**: người dùng chọn tốc độ quạt bằng nút nhấn.
- Điều khiển servo để mô phỏng chức năng đảo gió từ 0° đến 180°.
- Hiển thị trạng thái hệ thống trên LCD 16x2:
  - Nhiệt độ hiện tại.
  - Trạng thái OFF/AUTO/MANUAL.
  - Tốc độ quạt.
  - Trạng thái đảo gió.

## 2. Linh kiện sử dụng

| Linh kiện | Mô tả |
|---|---|
| Arduino Uno/Nano hoặc tương đương | Vi điều khiển chính |
| DHT11 | Cảm biến nhiệt độ |
| LCD 16x2 I2C | Hiển thị thông tin hệ thống |
| Động cơ DC | Làm quạt |
| Driver động cơ L298N/L293D hoặc module tương đương | Điều khiển chiều và tốc độ động cơ |
| Servo SG90 hoặc tương đương | Điều khiển đảo gió |
| 4 nút nhấn | Bật/tắt, đổi chế độ, đổi tốc độ, bật/tắt đảo gió |
| Điện trở/dây nối/breadboard | Kết nối mạch |
| Nguồn ngoài cho động cơ/servo | Khuyến nghị dùng nguồn riêng phù hợp tải |

## 3. Sơ đồ chân kết nối

| Chức năng | Chân Arduino |
|---|---|
| DHT11 Data | A0 |
| LCD I2C SDA | A4 trên Arduino Uno/Nano |
| LCD I2C SCL | A5 trên Arduino Uno/Nano |
| Driver motor IN1 | D8 |
| Driver motor IN2 | A2 |
| Driver motor EN/PWM | D11 |
| Servo signal | D10 |
| Nút bật/tắt | A1 |
| Nút đổi chế độ AUTO/MANUAL | D9 |
| Nút đổi tốc độ thủ công | D12 |
| Nút bật/tắt đảo gió | D13 |

> Lưu ý: Các nút nhấn được cấu hình `INPUT_PULLUP`, vì vậy một đầu nút nối vào chân tín hiệu, đầu còn lại nối GND. Khi nhấn nút, tín hiệu đọc được là `LOW`.

## 4. Nguyên lý hoạt động

Khi khởi động, LCD hiển thị thông báo:

```text
HE THONG QUAT TD
SAN SANG
```

Sau đó hệ thống đi vào vòng lặp chính:

1. Đọc trạng thái các nút nhấn.
2. Kiểm tra hệ thống đang bật hay tắt.
3. Nếu hệ thống bật:
   - Ở chế độ **AUTO**, Arduino đọc nhiệt độ từ DHT11 và tự chọn tốc độ quạt.
   - Ở chế độ **MANUAL**, Arduino dùng mức tốc độ do người dùng chọn.
4. Xuất PWM ra chân điều khiển động cơ.
5. Nếu bật đảo gió, servo quay qua lại từ 0° đến 180°.
6. LCD cập nhật trạng thái mỗi 300 ms.

## 5. Quy tắc điều khiển tốc độ quạt

### Chế độ AUTO

| Nhiệt độ | PWM | Tốc độ hiển thị |
|---|---:|---|
| Dưới 25°C | 0 | SPD:0 |
| 25°C đến 30°C | 100 | SPD:LOW |
| Trên 30°C và dưới 33°C | 200 | SPD:MED |
| Từ 33°C trở lên | 255 | SPD:MAX |

### Chế độ MANUAL

Mỗi lần nhấn nút tốc độ, hệ thống tăng `manualLevel` theo vòng lặp:

```text
0 -> 1 -> 2 -> 3 -> 0
```

| manualLevel | PWM | Tốc độ hiển thị |
|---:|---:|---|
| 0 | 0 | SPD:0 |
| 1 | 100 | SPD:LOW |
| 2 | 200 | SPD:MED |
| 3 | 255 | SPD:MAX |

## 6. Chức năng các nút nhấn

| Nút | Chức năng |
|---|---|
| `btnOnOff` | Bật hoặc tắt hệ thống |
| `btnMode` | Chuyển giữa AUTO và MANUAL khi hệ thống đang bật |
| `btnSpeed` | Tăng mức tốc độ khi đang ở MANUAL |
| `btnSwing` | Bật hoặc tắt đảo gió khi hệ thống đang bật |

Khi bật hệ thống, chương trình tự đặt về:

- Chế độ AUTO.
- Tốc độ thủ công bằng 0.
- Tắt đảo gió.
- Servo về góc 90°.

Khi tắt hệ thống:

- Quạt dừng.
- Tốc độ thủ công về 0.
- Đảo gió tắt.
- Servo quay về góc giữa 90°.

## 7. Thư viện cần cài đặt

Trong Arduino IDE, cài các thư viện sau:

- `LiquidCrystal_I2C`
- `Servo`
- `Wire`
- `DHT sensor library`

Có thể cài bằng cách:

1. Mở **Arduino IDE**.
2. Vào **Tools > Manage Libraries...**.
3. Tìm và cài:
   - `LiquidCrystal I2C`
   - `DHT sensor library`
4. Thư viện `Servo` và `Wire` thường đã có sẵn trong Arduino IDE.

## 8. Cách chạy dự án

### Bước 1: Chuẩn bị phần cứng

Kết nối các linh kiện theo bảng chân ở mục **Sơ đồ chân kết nối**.

Khuyến nghị:

- Dùng nguồn riêng cho động cơ DC và servo nếu tải lớn.
- Nối chung GND giữa Arduino, driver động cơ, nguồn servo và nguồn động cơ.
- Kiểm tra địa chỉ I2C của LCD. Code hiện đang dùng địa chỉ:

```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2);
```

Nếu LCD không hiển thị, có thể cần đổi `0x27` thành `0x3F`.

### Bước 2: Mở code

Mở file:

```text
codequattd.ino
```

bằng Arduino IDE.

### Bước 3: Chọn board và cổng nạp

Trong Arduino IDE:

1. Vào **Tools > Board** và chọn board đang dùng, ví dụ **Arduino Uno**.
2. Vào **Tools > Port** và chọn cổng COM tương ứng.

### Bước 4: Biên dịch và nạp chương trình

Nhấn:

- **Verify** để kiểm tra lỗi biên dịch.
- **Upload** để nạp chương trình vào Arduino.

### Bước 5: Vận hành

Sau khi nạp thành công:

1. LCD hiển thị thông báo khởi động.
2. Nhấn nút **bật/tắt** để bật hệ thống.
3. Ở chế độ AUTO, quạt tự chạy theo nhiệt độ.
4. Nhấn nút **Mode** để chuyển sang MANUAL.
5. Nhấn nút **Speed** để đổi tốc độ.
6. Nhấn nút **Swing** để bật/tắt đảo gió.

## 9. Cấu trúc chương trình

| Hàm | Chức năng |
|---|---|
| `setup()` | Cấu hình chân, khởi tạo LCD, DHT11, servo và trạng thái ban đầu |
| `loop()` | Vòng lặp chính: đọc nút, tính tốc độ, chạy quạt, chạy servo, cập nhật LCD |
| `readTemperature()` | Đọc nhiệt độ từ DHT11 |
| `runFan(int pwm)` | Điều khiển động cơ quạt theo giá trị PWM |
| `readButtons()` | Xử lý các nút nhấn và chống dội phím đơn giản |
| `autoFanSpeed(float temp)` | Tính tốc độ quạt tự động theo nhiệt độ |
| `manualFanSpeed()` | Tính tốc độ quạt theo mức thủ công |
| `runSwing()` | Điều khiển servo đảo gió từ 0° đến 180° |
| `stopServo()` | Dừng đảo gió và đưa servo về góc 90° |
| `displayLCD(float temp, int speedPWM)` | Hiển thị nhiệt độ, chế độ, tốc độ và trạng thái đảo gió lên LCD |

## 10. Ghi chú và hướng phát triển

Một số cải tiến có thể thêm trong tương lai:

- Kiểm tra lỗi khi DHT11 trả về `NaN`.
- Thêm cảm biến độ ẩm và hiển thị độ ẩm lên LCD.
- Thêm điều khiển bằng remote hồng ngoại hoặc Bluetooth.
- Lưu chế độ cuối cùng vào EEPROM.
- Thiết kế vỏ quạt và cơ cấu đảo gió thực tế.
- Thêm sơ đồ mạch bằng Fritzing hoặc Proteus.

## 11. Tên dự án gợi ý

```text
He thong quat tu dong dieu khien theo nhiet do
```
