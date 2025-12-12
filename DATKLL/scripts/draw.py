import matplotlib.pyplot as plt
import matplotlib.patches as patches
import os

def draw_architecture():
    # --- 1. CẤU HÌNH ĐƯỜNG DẪN CHUẨN ---
    # Lấy đường dẫn thư mục hiện tại của file script
    current_dir = os.path.dirname(os.path.abspath(__file__))
    # Lấy đường dẫn thư mục gốc dự án (cha của scripts)
    project_root = os.path.dirname(current_dir)
    # Đường dẫn thư mục images
    output_dir = os.path.join(project_root, "images")
    
    # TỰ ĐỘNG TẠO THƯ MỤC NẾU CHƯA CÓ
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
        print(f"Da tao thu muc moi: {output_dir}")

    # Đường dẫn file ảnh đầu ra
    save_path = os.path.join(output_dir, "line_buffer_diagram.png")

    # --- 2. VẼ HÌNH ---
    fig, ax = plt.subplots(figsize=(12, 6))
    ax.set_xlim(0, 20)
    ax.set_ylim(0, 10)
    ax.axis('off')

    # Style
    box_style = dict(boxstyle="round,pad=0.5", fc="#fff4c7", ec="black", lw=2)
    win_style = dict(boxstyle="square,pad=0.1", fc="#e1f7d5", ec="green", lw=2)
    logic_style = dict(boxstyle="round,pad=0.5", fc="#d6eaff", ec="blue", lw=2)
    
    # Line Buffers
    ax.text(5, 7, "Line Buffer 1 (FIFO)\n(Row N-1)", ha="center", va="center", size=12, bbox=box_style)
    ax.text(5, 3, "Line Buffer 0 (FIFO)\n(Row N-2)", ha="center", va="center", size=12, bbox=box_style)

    # Window Grid
    start_x, start_y = 10, 2.5
    size = 1.5
    gap = 0.2
    
    for col in range(3):
        for row in range(3):
            x = start_x + (2-col)*(size+gap)
            y = start_y + (2-row)*(size+gap)
            label = f"$P_{{{row}{col}}}$"
            color = "#ffcccc" if col == 2 else "white"
            rect = patches.Rectangle((x, y), size, size, linewidth=1, edgecolor='black', facecolor=color)
            ax.add_patch(rect)
            ax.text(x + size/2, y + size/2, label, ha="center", va="center", fontsize=12, weight='bold')

    # Window Frame
    rect_win = patches.Rectangle((start_x - 0.2, start_y - 0.2), 3*(size+gap)+0.2, 3*(size+gap)+0.2, 
                                 linewidth=2, edgecolor='green', facecolor='none', linestyle='--')
    ax.add_patch(rect_win)
    ax.text(start_x + 2.5, start_y + 5.5, "3x3 Sliding Window", ha="center", fontsize=14, weight='bold', color='green')

    # IO Nodes
    ax.text(1, 5, "Pixel In\n(Stream)", ha="center", va="center", size=12, bbox=dict(boxstyle="circle", fc="#ffaaaa", ec="red"))
    ax.text(18, 5, "Pixel Out", ha="center", va="center", size=12, weight='bold')

    # Logic Block
    ax.text(15.5, 5, "Sobel Core\nCalculation\n$|G_x| + |G_y|$", ha="center", va="center", size=12, bbox=logic_style)

    # Arrows
    props = dict(arrowstyle="-|>,head_width=0.4,head_length=0.8", lw=2, color='black')
    
    # Routing Arrows
    ax.annotate("", xy=(2.5, 7), xytext=(1.5, 5.5), arrowprops=dict(connectionstyle="bar,angle=180,fraction=-0.2", **props))
    ax.annotate("", xy=(13.5+size/2, 2.5), xytext=(1.5, 4.5), arrowprops=dict(connectionstyle="bar,angle=180,fraction=0.2", **props))
    ax.annotate("", xy=(2.5, 3), xytext=(7.5, 7), arrowprops=dict(connectionstyle="bar,angle=180,fraction=-0.2", **props))
    ax.annotate("", xy=(13.5+size/2, 2.5+2*(size+gap)+size), xytext=(7.5, 7), arrowprops=dict(connectionstyle="bar,angle=180,fraction=0.2", **props))
    ax.annotate("", xy=(13.5+size/2, 2.5+(size+gap)+size/2), xytext=(7.5, 3), arrowprops=dict(connectionstyle="bar,angle=180,fraction=0.2", **props))
    ax.annotate("", xy=(14.2, 5), xytext=(13.5+size+gap, 5), arrowprops=dict(arrowstyle="-|>", lw=2))
    ax.annotate("", xy=(17.2, 5), xytext=(16.8, 5), arrowprops=dict(arrowstyle="-|>", lw=2))

    # --- 3. LƯU ẢNH ---
    plt.tight_layout()
    plt.savefig(save_path, dpi=300, bbox_inches='tight')
    print(f"✅ Da tao anh thanh cong tai: {save_path}")

if __name__ == "__main__":
    draw_architecture()