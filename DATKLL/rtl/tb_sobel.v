`timescale 1ns / 1ps

module tb_sobel;

    reg clk;
    reg rst_n;
    reg [7:0] pixel_in;
    reg pixel_valid; // Tín hiệu dây dẫn của Testbench
    
    wire [7:0] pixel_out;
    wire out_valid;  // Tín hiệu dây dẫn của Testbench

    // Bộ nhớ đệm để đọc file Hex
    reg [7:0] img_data [0:512*512-1]; 
    integer i;
    integer file_out;

    // --- PHẦN QUAN TRỌNG NHẤT: KẾT NỐI VỚI CORE ---
    sobel_core u_sobel (
        .clk(clk),
        .rst_n(rst_n),
        .pixel_in(pixel_in),
        
        // SỬA LỖI Ở ĐÂY:
        // .Tên_Cổng_Trong_Core (Tên_Dây_Ngoài_Testbench)
        .pixel_valid_in(pixel_valid),   // ĐÚNG: pixel_valid_in
        .pixel_out(pixel_out),
        .pixel_valid_out(out_valid)     // ĐÚNG: pixel_valid_out
    );

    // Tạo xung clock 10ns
    always #5 clk = ~clk;

    initial begin
        // 1. Khởi tạo tín hiệu ban đầu
        clk = 0;
        rst_n = 0;
        pixel_valid = 0;

        // 2. Đọc file Hex
        $display("Loading image_data.hex...");
        $readmemh("image_data.hex", img_data);
        
        // Mở file để ghi kết quả
        file_out = $fopen("edge_output.hex", "w");

        // 3. Reset chip (giữ reset trong 20ns)
        #20 rst_n = 1;

        $display("Streaming pixels to Sobel Core...");
        
        // 4. Bắt đầu gửi dữ liệu (Giả lập Camera)
        for (i = 0; i < 512*512; i = i + 1) begin
            @(posedge clk);      // Chờ cạnh lên clock
            pixel_in = img_data[i]; 
            pixel_valid = 1;     // Bật tín hiệu Valid
        end

        // 5. Kết thúc gửi
        @(posedge clk);
        pixel_valid = 0;         // Tắt tín hiệu Valid

        // 6. Chờ dữ liệu chảy hết khỏi ống (Pipeline)
        // Line Buffer có độ trễ, cần chờ thêm thời gian
        #50000;
        
        $fclose(file_out);
        $display("Done! Output saved to edge_output.hex");
        $finish; // Dừng mô phỏng
    end

    // Ghi file kết quả khi có tín hiệu Valid từ Core
    always @(posedge clk) begin
        if (out_valid) begin
            $fwrite(file_out, "%02x\n", pixel_out);
        end
    end

endmodule