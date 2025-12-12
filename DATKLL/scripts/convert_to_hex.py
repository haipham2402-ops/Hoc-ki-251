from PIL import Image
import numpy as np

# 1. Đọc ảnh gốc và chuyển sang grayscale
img = Image.open("input.jpeg").convert("L")  # "L" = grayscale
img = img.resize((512, 512))                 # resize về 512x512

# 2. Lưu ảnh mức xám để so sánh
img.save("grayscale_input.png")
img.show()
print("Đã lưu ảnh grayscale_input.png")

# 3. Chuyển thành mảng 1D để tạo file hex
data = np.array(img, dtype=np.uint8).flatten()

# 4. Ghi file hex (dùng cho Verilog Sobel)
with open("image_data.hex", "w") as f:
    for val in data:
        f.write(f"{val:02X}\n")

print("Đã tạo file image_data.hex (512x512)")
