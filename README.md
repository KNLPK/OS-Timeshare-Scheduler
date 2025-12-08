# 🕒 OS Time-Share Scheduler Project

**Topik:** Simulasi penjadwalan time-sharing *Round Robin pre-emptive* menggunakan Bahasa C.
**Konsep Utama:** Sinyal (`SIGALRM`, `SIGSTOP`, `SIGCONT`), Timer (`setitimer`), dan Isolasi Hardware (Virtualization).

---

## ⚙️ Persiapan Lingkungan (Oracle VirtualBox + Ubuntu)

Proyek ini **wajib** dijalankan di lingkungan Linux murni menggunakan Virtual Machine untuk simulasi isolasi hardware.

### Tahap 1: Membuat Mesin Virtual (VirtualBox)

Ikuti langkah detail ini agar konfigurasi sesuai standar tugas:

1.  **Install & Buka VirtualBox**, lalu klik tombol **New** (ikon gerigi biru).
2.  **Isi Konfigurasi Awal:**
    * **Name:** `Ubuntu_OS_Project` (atau nama lain tanpa spasi).
    * **ISO Image:** Pilih file `.iso` Ubuntu yang sudah diunduh.
    * **PENTING:** Centang opsi **"Skip Unattended Installation"**. (Ini wajib agar kita bisa mengatur username/password secara manual).
3.  **Alokasi Hardware (System):**
    * **Base Memory:** Geser ke **4096 MB (4 GB)**. Jangan kurang, agar tidak *lag*.
    * **Processors:** Geser ke **2 CPU**.
4.  **Virtual Hard Disk:** Biarkan default atau set ke **25 GB**.
5.  Klik **Finish**. Mesin virtual sudah terbuat.

### Tahap 2: Instalasi Ubuntu (Di dalam VM)

1.  Pilih mesin yang baru dibuat, lalu klik tombol **Start** (Panah Hijau).
2.  Pilih opsi **"Try or Install Ubuntu"** menggunakan keyboard.
3.  Ikuti panduan instalasi di layar:
    * Pilih Bahasa English > Next.
    * **Installation Type:** Pilih **"Erase disk and install Ubuntu"**.
    * *Catatan:* Jangan panik. Opsi ini **hanya menghapus hardisk virtual** 25GB yang dibuat tadi. Data Windows/Laptop asli Anda **100% AMAN**.
4.  Buat Username dan Password (pendek saja agar mudah diingat).
5.  Tunggu instalasi selesai, lalu **Restart** mesin virtual.

### Tahap 3: Persiapan Compiler (GCC)

Setelah berhasil masuk ke Desktop Ubuntu, kita perlu menginstal "otak" untuk menjalankan kode C.

1.  Buka **Terminal** (Tekan tombol `Ctrl` + `Alt` + `T` secara bersamaan).
2.  Ketik perintah ini satu per satu (tekan Enter setiap baris):

    ```bash
    # Update daftar aplikasi
    sudo apt update

    # Install paket lengkap untuk coding C (GCC, Make, dll)
    sudo apt install build-essential
    ```

3.  Cek apakah instalasi berhasil dengan mengetik:
    ```bash
    gcc --version
    ```
    *Jika muncul versi gcc (misal: gcc 11.4.0), lingkungan siap digunakan.*
    
---

## 💻 Implementasi dan Kompilasi

### Tahap 4: Menyiapkan Proyek (Coding)
Kita akan membuat folder khusus dan file kode C di dalam lingkungan Linux.

1.  **Buat Direktori Kerja:**
    Di dalam Terminal Ubuntu, jalankan perintah ini baris per baris:
    ```bash
    # Kembali ke folder awal (Home)
    cd ~

    # Buat folder baru bernama 'aol_timeshare_project'
    mkdir aol_timeshare_project

    # Masuk ke dalam folder tersebut
    cd aol_timeshare_project
    ```

2.  **Buat File Source Code (`scheduler.c`):**
    Anda memiliki dua opsi untuk membuat file ini. Pilih salah satu yang paling nyaman:

    * **Opsi A: Menggunakan Terminal (Nano Editor) - Disarankan**
        1.  Ketik perintah:
            ```bash
            nano scheduler.c
            ```
        2.  Layar akan berubah menjadi editor teks kosong.
        3.  **Paste** kode C Anda di sini.
            * *Tips:* Jika `Ctrl+V` tidak berfungsi, gunakan **Klik Kanan > Paste** atau tekan `Ctrl+Shift+V`.
        4.  **Simpan & Keluar:**
            * Tekan `Ctrl + O` lalu `Enter` (untuk Save).
            * Tekan `Ctrl + X` (untuk Keluar).

    * **Opsi B: Menggunakan Tampilan Grafis (GUI)**
        1.  Klik ikon **"Files"** (gambar laci) di menu Ubuntu.
        2.  Buka folder `aol_timeshare_project`.
        3.  Klik kanan di area kosong > **Open in Terminal**.
        4.  Ketik `gedit scheduler.c` atau buka aplikasi **Text Editor** dari menu aplikasi.
        5.  Paste kode Anda, lalu klik **Save** di pojok kanan atas.
    
### Note
* Untuk Panduan per masing masing baris code nya ada di file 'panduan.c'.

---


## 📊 Analisis Output & Validasi

Jika output yang berjalan terus-menerus seperti di bawah ini, **JANGAN PANIK**. Itu artinya program **BERHASIL** mensimulasikan penjadwalan sistem operasi.

### 1. Bukti Mekanisme Pre-emption Berfungsi
Output tersebut menunjukkan urutan logika penjadwalan yang sempurna:

1.  **Interupsi Timer:**
    > `[SCHEDULER] Time Slice Habis.`
    * Ini bukti bahwa `setitimer` bekerja. Sinyal `SIGALRM` dikirim ke proses induk tepat waktu.
2.  **Context Switch (STOP):**
    > `Menghentikan PID 2747`
    * Scheduler mengambil alih kendali dan "membekukan" proses yang sedang berjalan menggunakan `SIGSTOP`.
3.  **Context Switch (CONT):**
    > `Memberikan CPU ke PID 2748`
    * Scheduler memilih proses antrian berikutnya (Round Robin) dan melanjutkannya menggunakan `SIGCONT`.
4.  **Eksekusi Proses:**
    > `Process PID 2748 | Status: Running`
    * Proses anak kembali bekerja memakan resource CPU hingga timer habis lagi.

### 2. Mengapa Program Tidak Berhenti (Looping)?
Anda mungkin bertanya, *"Kenapa programnya tidak berhenti-berhenti?"*

* **Tujuan Tugas:** Kita mensimulasikan CPU Scheduler. Sebuah Sistem Operasi (OS) tidak boleh berhenti; ia harus selalu siap menjadwalkan proses selama komputer menyala.
* **Logika Kode:** Fungsi `dummy_task()` dirancang sebagai *infinite loop* (`while(1)`), dan scheduler juga menunggu sinyal dalam *infinite loop*.
* **Kesimpulan:** Loop tanpa henti ini adalah indikator bahwa **timer interval (setitimer) tersetting dengan benar** karena terus-menerus memicu perpindahan proses secara berkala.

Untuk menghentikan program, cukup tekan **`Ctrl + C`** pada terminal.

### Contoh Output yang Valid
```text
[SCHEDULER] Time Slice Habis. Menghentikan PID 2747
[SCHEDULER] Memberikan CPU ke PID 2748
Process PID 2748 | Status: Running
Process PID 2748 | Status: Running

[SCHEDULER] Time Slice Habis. Menghentikan PID 2748
[SCHEDULER] Memberikan CPU ke PID 2746
Process PID 2746 | Status: Running
Process PID 2746 | Status: Running

[SCHEDULER] Time Slice Habis. Menghentikan PID 2746
[SCHEDULER] Memberikan CPU ke PID 2747
Process PID 2747 | Status: Running
... (berulang seterusnya)
