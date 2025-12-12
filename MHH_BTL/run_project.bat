@echo off
:: Chuyển thư mục làm việc về nơi chứa file .bat này
cd /d "%~dp0"

:: Thông báo đang chạy
echo Dang khoi dong file run.py...
echo --------------------------------

:: Lệnh chạy python
python run.py

:: Dừng màn hình lại sau khi chạy xong (hoặc khi lỗi) để xem kết quả
echo.
echo --------------------------------
echo Chuong trinh da ket thuc.
pause