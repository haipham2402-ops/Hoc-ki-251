module sobel_core (
    input wire clk,
    input wire rst_n,
    input wire [7:0] pixel_in,   
    input wire pixel_valid_in,   
    output reg [7:0] pixel_out,  
    output reg pixel_valid_out   
);

    parameter SIZE = 512;
    parameter THRESHOLD = 100;

    // --- LINE BUFFERS ---
    reg [7:0] line_buff_0 [0:SIZE-1];
    reg [7:0] line_buff_1 [0:SIZE-1];
    integer wr_ptr;

    // --- WINDOW ---
    reg [7:0] w[0:2][0:2];

    // Biến tính toán (Integer 32-bit có dấu)
    integer gx, gy;
    integer abs_gx, abs_gy;
    integer mag;
    
    // BIẾN QUAN TRỌNG ĐỂ FIX LỖI MÉO ẢNH
    // Phải ép kiểu pixel 8-bit sang integer trước khi nhân với số âm
    integer p00, p01, p02;
    integer p10, p11, p12;
    integer p20, p21, p22;

    integer col_cnt;
    integer row_cnt;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            wr_ptr <= 0;
            col_cnt <= 0;
            row_cnt <= 0;
            pixel_valid_out <= 0;
            pixel_out <= 0;
        end else if (pixel_valid_in) begin
            // 1. Cập nhật Window & Buffer
            w[0][2] <= line_buff_0[wr_ptr]; 
            w[1][2] <= line_buff_1[wr_ptr]; 
            w[2][2] <= pixel_in; 

            line_buff_0[wr_ptr] <= line_buff_1[wr_ptr]; 
            line_buff_1[wr_ptr] <= pixel_in;            
            
            wr_ptr <= (wr_ptr == SIZE - 1) ? 0 : wr_ptr + 1;

            w[0][0] <= w[0][1]; w[1][0] <= w[1][1]; w[2][0] <= w[2][1];
            w[0][1] <= w[0][2]; w[1][1] <= w[1][2]; w[2][1] <= w[2][2];

            // 2. Quản lý tọa độ
            if (col_cnt == SIZE - 1) begin
                col_cnt <= 0;
                row_cnt <= row_cnt + 1;
            end else begin
                col_cnt <= col_cnt + 1;
            end

            // 3. TÍNH TOÁN (ĐÃ FIX LỖI SIGNED/UNSIGNED)
            if (row_cnt >= 2 && col_cnt >= 2) begin
                
                // Ép kiểu sang số nguyên có dấu để tránh lỗi tính toán
                p00 = w[0][0]; p01 = w[0][1]; p02 = w[0][2];
                p10 = w[1][0]; p11 = w[1][1]; p12 = w[1][2];
                p20 = w[2][0]; p21 = w[2][1]; p22 = w[2][2];

                // Tính Gx
                // -1 0 1
                // -2 0 2
                // -1 0 1
                gx = (-1*p00 + 1*p02) + 
                     (-2*p10 + 2*p12) + 
                     (-1*p20 + 1*p22);

                // Tính Gy
                // -1 -2 -1
                //  0  0  0
                //  1  2  1
                gy = (-1*p00 - 2*p01 - 1*p02) + 
                     ( 1*p20 + 2*p21 + 1*p22);

                // Trị tuyệt đối
                if (gx < 0) abs_gx = -gx; else abs_gx = gx;
                if (gy < 0) abs_gy = -gy; else abs_gy = gy;

                // Manhattan
                mag = abs_gx + abs_gy;

                // Phân ngưỡng
                if (mag > THRESHOLD) 
                    pixel_out <= 255;
                else 
                    pixel_out <= 0;

                pixel_valid_out <= 1;
            end else begin
                pixel_valid_out <= 0;
            end
        end else begin
            pixel_valid_out <= 0;
        end
    end
endmodule