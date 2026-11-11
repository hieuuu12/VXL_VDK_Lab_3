# VXL - VĐK Lab 3: Buttons/Switches

Ứng dụng đèn giao thông ngã tư trên STM32F103C6, gồm 12 LED, 4 LED 7 đoạn và 3 nút nhấn.

- `Lab 3.ioc`, `Core/`, `Drivers/`: project STM32CubeIDE.
- `Lab 3.pdsprj`: mạch mô phỏng Proteus (nạp file `Debug/Lab 3.hex`).

## Chức năng
- **BUTTON1 (PB12):** đổi mode 1 → 2 → 3 → 4 → 1.
- **Mode 1:** đèn giao thông chạy bình thường, LED 7 đoạn hiển thị thời gian còn lại của mỗi đường.
- **Mode 2/3/4:** chỉnh thời gian đèn Đỏ/Vàng/Xanh. LED của màu đang chỉnh nháy 2 Hz.
  - **BUTTON2 (PB13):** tăng giá trị (1–99).
  - **BUTTON3 (PB14):** lưu giá trị. Luôn giữ ràng buộc Đỏ = Vàng + Xanh.
