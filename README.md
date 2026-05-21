# Hệ thống quạt từ động cảm biến nhiệt độ để điều chỉnh tốc độ quạt 

## Mô tả

<img src="https://sf-static.upanhlaylink.com/img/image_20260520d357876415d942011facb93114e84217.jpg" alt="iot_map.png">

Hệ thống quạt thông minh sử dụng Arduino Uno để điều khiển tốc độ quạt tự động hoặc thủ công dựa trên nhiệt độ môi trường đo được từ cảm biến LM35. Hệ thống có hai chế độ hoạt động gồm AUTO và MANUAL, được chuyển đổi bằng nút nhấn MODE.

## Ở chế độ AUTO, Arduino đọc giá trị nhiệt độ từ cảm biến LM35, sau đó tự động điều chỉnh tốc độ quạt theo các mức nhiệt độ đã được cài đặt. Khi nhiệt độ thấp, quạt sẽ tắt hoặc quay chậm; khi nhiệt độ tăng cao, tốc độ quạt sẽ tăng tương ứng để làm mát hiệu quả hơn. <br>
## Nguyên lý hoạt động:<br>

Khi nhiệt độ thấp hơn 25°C, hệ thống sẽ tắt quạt để tiết kiệm điện năng. <br>
Khi nhiệt độ tăng từ 25°C đến 30°C, quạt chạy ở mức gió nhẹ để làm mát nhẹ nhàng. <br>
Khi nhiệt độ đạt từ 30°C đến dưới 33°C, quạt chuyển sang tốc độ trung bình. <br>
Khi nhiệt độ vượt quá 33°C, quạt chạy ở tốc độ tối đa nhằm làm giảm nhiệt nhanh chóng. <br>

## Ở chế độ MANUAL, người dùng có thể điều chỉnh tốc độ quạt bằng nút SPEED. Mỗi lần nhấn nút SPEED, tốc độ quạt sẽ thay đổi theo các mức:

Mức 1: tốc độ thấp, LED xanh sáng <br>
Mức 2: tốc độ trung bình, LED vàng sáng <br>
Mức 3: tốc độ cao nhất, LED đỏ sáng <br>
Lần nhấn tiếp theo: quạt tắt và tất cả LED tắt

Hệ thống sử dụng driver động cơ L298N để điều khiển quạt DC bằng tín hiệu PWM từ Arduino. Ngoài ra, hệ thống còn tích hợp servo điều khiển chức năng đảo gió (SWING). Khi bật chế độ SWING, servo sẽ quay qua lại từ 0° đến 180° giúp luồng gió phân bố đều hơn trong không gian.

Thông tin hoạt động của hệ thống như nhiệt độ, chế độ hoạt động, tốc độ quạt và trạng thái đảo gió được hiển thị trực tiếp trên màn hình LCD 16x2, giúp người dùng dễ dàng theo dõi và điều khiển hệ thống.
IOT_QuatTuDong