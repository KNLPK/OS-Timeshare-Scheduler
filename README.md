# 🕒 OS Time-Share Scheduler Project

**Topik:** Simulasi penjadwalan time-sharing *Round Robin pre-emptive* menggunakan Bahasa C.
**Konsep Utama:** Sinyal (`SIGALRM`, `SIGSTOP`, `SIGCONT`), Timer (`setitimer`), dan Isolasi Hardware (Virtualization).

---

## ⚙️ Persiapan Lingkungan (Oracle VirtualBox + Ubuntu)

Proyek ini **wajib** dijalankan di lingkungan Linux murni menggunakan Virtual Machine untuk simulasi isolasi hardware yang akurat (sesuai standar mata kuliah OS).

### Tahap 1: Persiapan Bahan & Instalasi VirtualBox
Sebelum memulai, unduh dua file berikut di OS utama (Windows/Mac):
1.  **Oracle VM VirtualBox:** [Download di sini](https://www.virtualbox.org/).
2.  **Ubuntu Desktop ISO (24.04 LTS):** [Download di sini](https://ubuntu.com/download/desktop).

**Langkah Instalasi:**
1.  Install **Oracle VirtualBox** seperti biasa.
2.  Buka VirtualBox, klik **New**.
3.  **Name:** `Ubuntu_OS_Project`.
4.  **ISO Image:** Pilih file `.iso` Ubuntu yang sudah diunduh.
5.  **Hardware:** Alokasikan minimal **4096 MB (4GB) RAM** dan **2 CPU**.
6.  **Hard Disk:** Alokasikan **25 GB**.
7.  Selesaikan wizard, lalu klik **Start** untuk memulai instalasi Ubuntu (pilih *Erase disk and install Ubuntu*).

### Tahap 2: Instalasi Compiler C (GCC)
Setelah berhasil login ke dalam Desktop Ubuntu (di dalam VirtualBox), buka **Terminal** (`Ctrl + Alt + T`) dan jalankan perintah berikut untuk menyiapkan "bengkel" koding:

1.  **Perbarui Daftar Paket:**
    ```bash
    sudo apt update
    ```
2.  **Instal Tools Development Esensial:**
    Ini akan menginstal GCC, G++, dan Make sekaligus.
    ```bash
    sudo apt install build-essential
    ```
3.  **Verifikasi Instalasi:**
    Pastikan compiler siap digunakan:
    ```bash
    gcc --version
    ```

---

## 💻 Implementasi dan Kompilasi

### Tahap 3: Menyiapkan Proyek
Lakukan langkah ini di dalam Terminal Ubuntu:

1.  **Buat Folder Kerja:**
    ```bash
    cd ~
    mkdir aol_timeshare_project
    cd aol_timeshare_project
    ```

2.  **Buat File Source Code:**
    Karena VS Code Windows tidak terhubung langsung ke VirtualBox, gunakan Text Editor bawaan Ubuntu atau `nano`.
    * **Opsi GUI:** Buka aplikasi "Text Editor" di Ubuntu, paste kode, simpan sebagai `scheduler.c` di folder tadi.
    * **Opsi Terminal:**
        ```bash
        nano scheduler.c
        ```
        (Paste kode di sini, lalu tekan `Ctrl+X`, `Y`, `Enter` untuk save).

### Tahap 4: Kompilasi dan Eksekusi

1.  **Kompilasi Kode:**
    Ubah kode mentah C menjadi program *executable* (biner Linux):
    ```bash
    gcc scheduler.c -o scheduler
    ```
    *Jika tidak muncul pesan error, berarti kompilasi sukses.*

2.  **Jalankan Program:**
    Mulai simulasi penjadwalan:
    ```bash
    ./scheduler
    ```

---

## ❓ Mengapa VirtualBox (VM)?
Berbeda dengan WSL2 yang menggunakan kernel translasi, VirtualBox menyediakan:
1.  **Isolasi Total:** Crash pada program OS ini tidak akan memengaruhi Windows utama.
2.  **Simulasi Hardware Nyata:** Manajemen memori dan CPU scheduling berjalan di atas hardware virtual yang terdedikasi, bukan "meminjam" kernel Windows.

### Note
* Untuk Panduan per masing masing baris code nya ada di file 'panduan.c'.


## 📊 Analisis Output & Validasi

Jika Anda melihat output yang berjalan terus-menerus seperti di bawah ini, **JANGAN PANIK**. Itu artinya program **BERHASIL** mensimulasikan penjadwalan sistem operasi.

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
