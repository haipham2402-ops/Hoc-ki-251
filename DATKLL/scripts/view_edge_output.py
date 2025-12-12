from PIL import Image
import numpy as np
import os

# --- CẤU HÌNH ---
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
HEX_OUTPUT_PATH = os.path.join(BASE_DIR, "edge_output.hex")
IMG_OUTPUT_PATH = os.path.join(BASE_DIR, "edge_result.png")

# Kích thước chuẩn
FULL_SIZE = 512
# Kích thước thực tế Verilog xuất ra (Bỏ 2 dòng đầu, bỏ 2 cột đầu mỗi dòng)
VALID_SIZE = 510 
EXPECTED_PIXELS = VALID_SIZE * VALID_SIZE  # 260,100 pixels

def hex_to_image():
    print(f"Dang doc file: {HEX_OUTPUT_PATH}")
    
    if not os.path.exists(HEX_OUTPUT_PATH):
        print("LOI: Khong tim thay file edge_output.hex")
        return

    with open(HEX_OUTPUT_PATH, "r") as f:
        lines = [x.strip() for x in f if x.strip() != "" and not x.startswith("//")]
    
    data = []
    for x in lines:
        try:
            data.append(int(x, 16))
        except ValueError:
            data.append(0)

    print(f"So luong pixel tu Verilog: {len(data)} (Ky vong: {EXPECTED_PIXELS})")

    # --- XỬ LÝ SẮP XẾP LẠI ẢNH (QUAN TRỌNG) ---
    try:
        # 1. Cắt hoặc bù dữ liệu cho đúng kích thước phần lõi 510x510
        if len(data) < EXPECTED_PIXELS:
            data.extend([0] * (EXPECTED_PIXELS - len(data)))
        elif len(data) > EXPECTED_PIXELS:
            data = data[:EXPECTED_PIXELS]

        # 2. Tạo ma trận lõi 510x510 từ dữ liệu Verilog
        # Đây là bước sửa lỗi méo ảnh: Gom đúng 510 pixel mỗi dòng
        core_img = np.array(data, dtype=np.uint8).reshape((VALID_SIZE, VALID_SIZE))

        # 3. Tạo khung ảnh đen 512x512
        full_img = np.zeros((FULL_SIZE, FULL_SIZE), dtype=np.uint8)

        # 4. Dán phần lõi vào khung (Lùi vào 2 pixel để bù Latency)
        # Verilog bắt đầu tính khi row>=2 và col>=2
        full_img[2:, 2:] = core_img

        # 5. Lưu ảnh
        img = Image.fromarray(full_img, mode="L")
        img.save(IMG_OUTPUT_PATH)
        print(f"Đã tạo ảnh CHUẨN (Fix lệch dòng): {IMG_OUTPUT_PATH}")
        
    except Exception as e:
        print(f"LOI khi xu ly anh: {e}")

if __name__ == "__main__":
    hex_to_image()