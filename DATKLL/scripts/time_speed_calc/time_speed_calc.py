import cv2
import time
import pandas as pd
import numpy as np

# --- CẤU HÌNH ---
# Hãy thay thế bằng tên file ảnh thật của bạn
danh_sach_anh = [
    'anh_1.jpg', 
    'anh_2.jpg',
    'anh_3.jpg',
    'anh_4.jpg',
    'anh_5.jpg'
]

SO_LAN_LAP = 50 # Số lần lặp để tính trung bình

def process_sobel_full_pipeline(img_color):
    # Bắt đầu tính giờ
    start = time.perf_counter()
    
    for _ in range(SO_LAN_LAP):
        # BƯỚC 1: Chuyển sang ảnh xám (Grayscale Conversion)
        gray = cv2.cvtColor(img_color, cv2.COLOR_BGR2GRAY)
        
        # BƯỚC 2: Thuật toán Sobel
        sobelx = cv2.Sobel(gray, cv2.CV_64F, 1, 0, ksize=3)
        sobely = cv2.Sobel(gray, cv2.CV_64F, 0, 1, ksize=3)
        
        # Hợp nhất biên (Tính Magnitude)
        # Đây là bước tốn kém nhất trong Sobel
        cv2.addWeighted(cv2.convertScaleAbs(sobelx), 0.5, cv2.convertScaleAbs(sobely), 0.5, 0)
        
    end = time.perf_counter()
    return ((end - start) / SO_LAN_LAP) * 1000 # Trả về ms

def process_canny_full_pipeline(img_color):
    # Bắt đầu tính giờ
    start = time.perf_counter()
    
    for _ in range(SO_LAN_LAP):
        # BƯỚC 1: Chuyển sang ảnh xám (Grayscale Conversion)
        # Lưu ý: Canny trong OpenCV bắt buộc input là ảnh 1 channel (xám)
        gray = cv2.cvtColor(img_color, cv2.COLOR_BGR2GRAY)
        
        # BƯỚC 2: Thuật toán Canny
        cv2.Canny(gray, 100, 200)
        
    end = time.perf_counter()
    return ((end - start) / SO_LAN_LAP) * 1000 # Trả về ms

# --- CHƯƠNG TRÌNH CHÍNH ---
ket_qua = []

print(f"Đang chạy thử nghiệm 'Full Pipeline' (RGB -> Gray -> Edge) trên {len(danh_sach_anh)} ảnh...")

for ten_file in danh_sach_anh:
    # LƯU Ý: Ở đây ta đọc ảnh MÀU (không có flag 0)
    img_color = cv2.imread(ten_file)
    
    if img_color is None:
        print(f"Lỗi: Không tìm thấy file '{ten_file}'")
        continue
        
    h, w, c = img_color.shape # Lấy thêm số kênh màu (thường là 3)
    pixel_count = h * w
    
    # Đo thời gian
    time_sobel = process_sobel_full_pipeline(img_color)
    time_canny = process_canny_full_pipeline(img_color)
    
    # Lưu kết quả
    ket_qua.append({
        "Tên ảnh": ten_file,
        "Độ phân giải": f"{w}x{h}",
        "Tổng Pixel": pixel_count,
        "Sobel Full (ms)": round(time_sobel, 3),
        "Canny Full (ms)": round(time_canny, 3),
        "Chênh lệch": f"{round(time_canny/time_sobel, 1)}x"
    })

# --- XUẤT BẢNG ---
print("\n=== BẢNG THỜI GIAN XỬ LÝ TOÀN HỆ THỐNG (BAO GỒM CHUYỂN ẢNH XÁM) ===")
df = pd.DataFrame(ket_qua)
print(df.to_string(index=False))
