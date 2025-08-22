# ESP32

> idf.py set-target esp32
> idf.py menuconfig

copy file .gitignore

> idf.py build
> idf.py -p PORT flash monitor

> idf.py size → Hiển thị tóm tắt dung lượng flash đang dùng:

Used static DRAM:   10996 bytes ( 169740 remain, 6.1% used)
      .data size:    8732 bytes
      .bss  size:    2264 bytes
Used static IRAM:   50790 bytes (  80282 remain, 38.7% used)
      .text size:   49763 bytes
   .vectors size:    1027 bytes
Used Flash size :  117339 bytes
           .text:   79231 bytes
         .rodata:   37852 bytes
Total image size:  176861 bytes (.bin may be padded larger)

DRAM tĩnh mới dùng ~11 KB/≈180 KB vùng tĩnh → còn rất thoải mái cho biến global.
IRAM dùng ~50 KB/128 KB → còn ~80 KB, ổn; đừng rải IRAM_ATTR bừa bãi.
Flash cho app ~117 KB; tổng ảnh app ~177 KB → rất nhỏ so với 4 MB flash của ESP32‑WROOM‑32D.

> idf.py size-components → Liệt kê dung lượng bộ nhớ mà mỗi component (module) sử dụng.

> idf.py size-files → Liệt kê dung lượng theo từng file .o (object file).

> idf.py size-symbols (theo symbol/hàm/biến)
> echo $env:IDF_PATH

> idf.py partition_table
Xuất ra file CSV để xem chi tiết:
> idf.py partition_table -o build/partition_table.csv

Bảng phân vùng (4 MB flash, 2 OTA + 1 FS)

| Offset   | Kích thước | ≈ Dung lượng | Phân vùng           | Ghi chú |
|---------:|-----------:|-------------:|---------------------|--------|
| 0x1000   | biến đổi   | —            | Bootloader          | Được flash ở 0x1000 (không nằm trong partition table) |
| 0x8000   | 0x1000     | 4 KiB        | Partition table     | Danh sách phân vùng |
| 0x9000   | 0x5000     | 20 KiB       | NVS                 | Lưu config/key (NVS) |
| 0xE000   | 0x2000     | 8 KiB        | OTA Data (otadata)  | Lưu trạng thái OTA |
| 0x10000  | 0x140000   | ≈ 1.25 MiB   | OTA App Slot 0      | Phân vùng ứng dụng |
| 0x150000 | 0x140000   | ≈ 1.25 MiB   | OTA App Slot 1      | Phân vùng ứng dụng |
| 0x290000 | 0x170000   | ≈ 1.44 MiB   | SPIFFS/LittleFS     | Phân vùng hệ thống file |

Ghi chú: 0x290000 + 0x170000 = 0x400000 → tổng đúng 4 MB.

> esptool.exe --port COM7 flash_id

Chip is ESP32-D0WD (revision v1.0)
Features: WiFi, BT, Dual Core, 240MHz, VRef calibration in efuse, Coding Scheme None
Crystal is 40MHz
MAC: f0:08:d1:62:ec:a8
Uploading stub...
Running stub...
Stub running...
Manufacturer: 20
Device: 4016
Detected flash size: 4MB
Flash voltage set by a strapping pin to 3.3V

Bảng tóm tắt SRAM trên ESP32‑D0WD (ESP32‑WROOM‑32D)

| Vùng bộ nhớ | Dung lượng điển hình | Công dụng | Ghi chú/Quan sát |
|---|---:|---|---|
| DRAM (Data RAM, 8‑bit) | ≈ 320 KB | .data/.bss, heap (malloc/new), stack của task, buffer | Free heap sau boot thường ~280–320 KB khi chưa bật Wi‑Fi/BT; ví dụ hello_world của bạn ~300 KB |
| IRAM (Instruction RAM) | ≈ 128 KB | Mã chạy từ RAM: vector ngắt, ISR (IRAM_ATTR), code chạy khi flash cache tắt | idf.py size: Used static IRAM + remain ≈ 131072 B |
| RTC Fast memory | 16 KB | Giữ qua deep sleep; có thể chứa code/data nhỏ cần retention | Dùng các thuộc tính như RTC_IRAM_ATTR/RTC_DATA_ATTR tùy mục đích |
| RTC Slow memory | 8 KB | Giữ qua deep sleep; ULP, cấu hình/trạng thái cần retention | Tốc độ chậm hơn RTC Fast; ULP thường dùng vùng này |
| Phần SRAM nội bộ khác | ≈ 48 KB | Cơ chế nội bộ (cache/ánh xạ/bus…), không dùng trực tiếp | 520 KB tổng − (320+128+16+8) ≈ 48 KB (xấp xỉ) |

Lưu ý
- “Tổng SRAM on‑chip” ≈ 520 KB là tổng vật lý; phần ứng dụng thực tế dùng chủ yếu là ~320 KB DRAM và ~128 KB IRAM + 24 KB RTC. 
- PSRAM không có trên WROOM‑32D (có trên các module WROVER).
