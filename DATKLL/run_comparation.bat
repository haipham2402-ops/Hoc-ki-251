@echo off
TITLE Simulation Run

REM Thiet lap duong dan
set PROJECT_ROOT=%~dp0
set SCRIPTS_DIR=%PROJECT_ROOT%scripts
set RTL_DIR=%PROJECT_ROOT%rtl

cd /d %PROJECT_ROOT%

echo [1/4] Chuyen doi anh sang Hex...
python "%SCRIPTS_DIR%\convert_to_hex.py"

echo [2/4] Chay mo phong Verilog (Line Buffer Mode)...
REM Biên dịch file Core và Testbench cùng lúc
iverilog -o sobel_sim.vvp "%RTL_DIR%\sobel_core.v" "%RTL_DIR%\tb_sobel.v"

REM Chạy mô phỏng
vvp sobel_sim.vvp

echo [3/4] Tai tao anh tu ket qua Verilog...
python "%SCRIPTS_DIR%\view_edge_output.py"

echo [4/4] Chay mo phong Python va Ghep anh...
python "%SCRIPTS_DIR%\sobel_software.py"

echo Hoan thanh. Dang mo file ket qua...
start comparison_result.png

pause