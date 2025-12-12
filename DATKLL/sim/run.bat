@echo off
title Sobel Edge Detection Simulation Runner
echo ========================================================
echo  DU AN: IMAGE EDGE DETECTION (SOBEL)
echo  CAU TRUC: RTL - SIM - SCRIPTS
echo ========================================================

:: 1. Chuyen doi thu muc lam viec ve thu muc chua file .bat (thu muc sim)
cd /d "%~dp0"

echo.
echo [BUOC 1] DANG CHAY PYTHON TIEN XU LY...
echo --------------------------------------------------------
:: Goi script tu thu muc ../scripts/
:: Script se doc input.jpeg tai thu muc hien tai (sim/)
python ..\scripts\convert_to_hex.py

:: Kiem tra neu Python loi
if %errorlevel% neq 0 (
    echo [LOI] Khong chay duoc Python hoac thieu thu vien Pillow/Numpy!
    pause
    exit /b
)

echo.
echo [BUOC 2] DANG BIEN DICH VERILOG (RTL)...
echo --------------------------------------------------------
:: Goi iverilog bien dich file code nam trong ../rtl/
:: File sobel_sim se duoc tao ra ngay tai day (sim/)
iverilog -o sobel_sim ..\rtl\sobel_operator.v

:: Kiem tra loi bien dich
if %errorlevel% neq 0 (
    echo [LOI] Bien dich Verilog that bai! Kiem tra code trong ../rtl/
    pause
    exit /b
)

echo.
echo [BUOC 3] DANG CHAY MO PHONG (SIMULATION)...
echo --------------------------------------------------------
:: Chay file mo phong vua tao
vvp sobel_sim

echo.
echo [BUOC 4] DANG HIEN THI KET QUA...
echo --------------------------------------------------------
:: Goi script hien thi anh tu ../scripts/
python ..\scripts\view_edge_output.py

echo.
echo ========================================================
echo [HOAN TAT] Kiem tra file 'edge_result.png' trong thu muc nay.
echo ========================================================
pause