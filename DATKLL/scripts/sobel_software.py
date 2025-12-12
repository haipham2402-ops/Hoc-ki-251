import cv2
import numpy as np
import os

# --- CẤU HÌNH ---
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HEX_INPUT_PATH = os.path.join(BASE_DIR, "image_data.hex")
VERILOG_RESULT_PATH = os.path.join(BASE_DIR, "edge_result.png")
FINAL_OUTPUT_PATH = os.path.join(BASE_DIR, "comparison_result.png")

# CẤU HÌNH THÔNG SỐ (PHẢI GIỐNG FILE VERILOG)
SIZE = 512
THRESHOLD = 100

def run_bit_true_simulation():
    print("LOG: Đang chạy mô phỏng Python (Chế độ Bit-True giống hệt Verilog)...")
    
    # 1. ĐỌC DỮ LIỆU TỪ FILE HEX (Để đảm bảo Input giống nhau)
    if not os.path.exists(HEX_INPUT_PATH):
        print("Lỗi: Không tìm thấy file image_data.hex")
        return

    with open(HEX_INPUT_PATH, 'r') as f:
        lines = f.readlines()
    
    # Chuyển hex sang mảng số nguyên (Integer)
    # Verilog: reg [7:0] img_in
    data = [int(line.strip(), 16) for line in lines if line.strip()]
    img_in = np.array(data, dtype=np.int32).reshape((SIZE, SIZE))
    
    # Tạo mảng kết quả (chứa toàn số 0)
    img_out = np.zeros((SIZE, SIZE), dtype=np.uint8)

    # 2. ĐỊNH NGHĨA KERNEL (SỐ NGUYÊN)
    # Verilog: integer Gx, Gy
    Gx = np.array([[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]], dtype=np.int32)
    Gy = np.array([[-1, -2, -1], [0, 0, 0], [1, 2, 1]], dtype=np.int32)

    # 3. CHẠY VÒNG LẶP MÔ PHỎNG PHẦN CỨNG
    # Verilog: for (i = 1; i < SIZE-1; ...) -> Bỏ qua viền 1 pixel
    print("LOG: Đang tính toán từng pixel (Vui lòng đợi vài giây)...")
    
    # Duyệt qua từng pixel (Trừ viền)
    for i in range(1, SIZE - 1):
        for j in range(1, SIZE - 1):
            
            # --- KHỐI TÍNH TÍCH CHẬP (CONVOLUTION) ---
            # Verilog: gx = 0; gy = 0;
            gx = 0
            gy = 0
            
            # Nhân chập vùng 3x3
            # Logic này tương đương 2 vòng lặp m, n trong Verilog của bạn
            # Hoặc viết tường minh để dễ so sánh:
            # P1 P2 P3
            # P4 P5 P6
            # P7 P8 P9
            
            # Hàng trên
            p1 = img_in[i-1, j-1]; p2 = img_in[i-1, j]; p3 = img_in[i-1, j+1]
            # Hàng giữa
            p4 = img_in[i,   j-1]; p5 = img_in[i,   j]; p6 = img_in[i,   j+1]
            # Hàng dưới
            p7 = img_in[i+1, j-1]; p8 = img_in[i+1, j]; p9 = img_in[i+1, j+1]

            # Tính Gx (Giống hệt ma trận Gx)
            # -1 0 1
            # -2 0 2
            # -1 0 1
            gx = (-1*p1 + 1*p3) + (-2*p4 + 2*p6) + (-1*p7 + 1*p9)

            # Tính Gy (Giống hệt ma trận Gy)
            # -1 -2 -1
            #  0  0  0
            #  1  2  1
            gy = (-1*p1 - 2*p2 - 1*p3) + (1*p7 + 2*p8 + 1*p9)

            # --- KHỐI TÍNH ĐỘ LỚN & TRỊ TUYỆT ĐỐI ---
            # Verilog logic: mag = abs(gx) + abs(gy)
            # Lưu ý: Verilog không có hàm abs sẵn, thường dùng: (gx < 0 ? -gx : gx)
            abs_gx = -gx if gx < 0 else gx
            abs_gy = -gy if gy < 0 else gy
            
            mag = abs_gx + abs_gy

            # --- KHỐI SO SÁNH NGƯỠNG ---
            # Verilog: if (mag > THRESHOLD)
            if mag > THRESHOLD:
                img_out[i, j] = 255
            else:
                img_out[i, j] = 0

    # 4. ĐỌC ẢNH VERILOG ĐỂ GHÉP
    if os.path.exists(VERILOG_RESULT_PATH):
        hardware_img = cv2.imread(VERILOG_RESULT_PATH, 0)
        hardware_img = cv2.resize(hardware_img, (SIZE, SIZE))
    else:
        hardware_img = np.zeros((SIZE, SIZE), dtype=np.uint8)

    # 5. GHÉP ẢNH
    separator = np.ones((SIZE, 5), dtype=np.uint8) * 128
    combined = np.hstack((img_out, separator, hardware_img))
    
    cv2.imwrite(FINAL_OUTPUT_PATH, combined)
    print(f"✅ XONG! Ảnh so sánh đã lưu tại: {FINAL_OUTPUT_PATH}")

if __name__ == "__main__":
    run_bit_true_simulation()