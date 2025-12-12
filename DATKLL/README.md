# Image Edge Detection using Sobel Operator (Verilog HDL)

## Giới thiệu (Overview)
Dự án này là một mô hình thiết kế phần cứng (Hardware Design Model) thực hiện thuật toán phát hiện biên **Sobel** trên ảnh số. Hệ thống được viết bằng ngôn ngữ **Verilog HDL** và kiểm tra thông qua mô phỏng (Simulation).

Quy trình xử lý ảnh:
1.  **Tiền xử lý (Python):** Chuyển ảnh JPEG sang Grayscale và trích xuất dữ liệu Hex.
2.  **Xử lý chính (Verilog):** Thực hiện tích chập với toán tử Sobel (3x3 Kernel) để tính Gradient và biên.
3.  **Hậu xử lý (Python):** Dựng lại ảnh biên từ dữ liệu Hex xuất ra.

## 📂 Cấu trúc Dự án (Project Structure)
Dự án được tổ chức theo tiêu chuẩn thiết kế phần cứng:

```text
Sobel_Edge_Project/
│
├── 📂 rtl/                 # Mã nguồn Verilog (Register Transfer Level)
│   ├── sobel_core.v    # Module chính: Tính toán Sobel & Thresholding
│   └── tb_sobel.v
│
├── 📂 scripts/             # Các công cụ hỗ trợ (Python)
│   ├── convert_to_hex.py   # Chuyển ảnh -> image_data.hex
│   └── view_edge_output.py # Chuyển edge_output.hex -> ảnh PNG
│
├── 📂 sim/                 # Môi trường mô phỏng (Simulation Work)
│   ├── input.jpeg          # Ảnh đầu vào gốc
│   ├── run.bat     # Script tự động chạy toàn bộ quy trình
│   └── (Các file tạm như .hex, .out sẽ được sinh ra tại đây)
│
│── 📂 docs/                # Tài liệu báo cáo & tham khảo
│    ├── DATKLL_251.pdf
│    └── DATKLL_Sobel_Edge_Detection_Final.pdf
│
│── run_comparation.bat
|
└── input.jpeg
