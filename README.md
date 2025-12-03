# 🕒 OS Time-Share Scheduler Project

**Topik:** Simulasi penjadwalan time-sharing *Round Robin pre-emptive* menggunakan Bahasa C.  
**Konsep Utama:** Sinyal (`SIGALRM`, `SIGSTOP`, `SIGCONT`) dan Timer (`setitimer`).

---

## ⚙️ Persiapan Lingkungan (Windows Subsystem for Linux - WSL2)

Kode ini menggunakan fitur spesifik OS bergaya UNIX (Linux). Pengguna Windows **wajib** menggunakan WSL2 (Ubuntu).

### Tahap 1: Instalasi WSL2

1.  Buka **Command Prompt (CMD)** atau **PowerShell**.
2.  Jalankan sebagai Administrator (Klik kanan > *Run as administrator*).
3.  Ketik perintah berikut dan tekan Enter:
    ```powershell
    wsl --install
    ```
4.  **Restart Komputer Anda**.
5.  Setelah restart, terminal akan terbuka otomatis. Buat **Username** dan **Password** Linux Anda(Diingat).

### Tahap 2: Instalasi Compiler C (GCC)
Setelah berhasil masuk ke terminal Ubuntu (WSL), lakukan instalasi *tools* yang dibutuhkan:

1.  **Perbarui Daftar Paket:**
    ```bash
    sudo apt update
    ```
2.  **Instal Tools Development Esensial (termasuk GCC):**
    ```bash
    sudo apt install build-essential
    ```
3.  **Verifikasi Instalasi:**
    Pastikan GCC sudah terinstal dengan benar:
    ```bash
    gcc --version
    ```

---

## 💻 Implementasi dan Kompilasi

### Tahap 3: Menyiapkan Proyek
Lakukan perintah berikut di terminal Anda untuk membuat folder kerja:

1.  **Pindah ke Home Directory dan Buat Folder:**
    ```bash
    cd ~
    mkdir aol_timeshare_project
    cd aol_timeshare_project
    ```
    Nama Project nya bebas apa saja, ini nama untuk project os nanti.
    
3.  **Buat File Source Code:**
    Buat file bernama `scheduler.c` dan buka text editor (VSC):
    ```bash
    code scheduler.c
    ```
    *(Paste kode C scheduler.c nya)*.

### Tahap 4: Kompilasi dan Eksekusi

1.  **Kompilasi Kode:**
    Ubah kode C menjadi program yang bisa dijalankan (`executable` bernama `scheduler`):
    ```bash
    gcc scheduler.c -o scheduler
    ```

2.  **Jalankan Program:**
    Mulai simulasi penjadwalan:
    ```bash
    ./scheduler
    ```

---

## 🎯 Panduan Penjelasan Video 

Video harus menjelaskan poin-poin teknis berikut dalam durasi **5-10 menit**:

### 1. Mekanisme Interupsi Timer ⏱️
* Jelaskan fungsi `setitimer(ITIMER_REAL, ...)` yang ada di fungsi `main`.
* Jelaskan bahwa fungsi ini mengatur interval waktu (misalnya **200ms**).
* Fungsi ini secara otomatis mengirimkan sinyal **`SIGALRM`** ke proses induk. Ini adalah simulasi dari *Hardware Clock Interrupt* pada OS.

### 2. Peran Penjadwal (Scheduler) 👮
* Jelaskan bahwa **Proses Induk** bertindak sebagai *Scheduler*.
* Scheduler hanya aktif "bangun" ketika menerima sinyal `SIGALRM`.
* Tunjukkan bagian kode: `while(1) { pause(); }` yang menunggu sinyal tersebut.

### 3. Logika Context Switch 🔄
Bedah langkah-langkah di dalam fungsi `scheduler_handler`:
* **`kill(pid, SIGSTOP)`**: Menghentikan sementara proses anak yang sedang berjalan. Ini disebut *Pre-emption*.
* **`current_process_index = (index + 1) % NUM_CHILDREN;`**: Rumus matematika untuk algoritma *Round Robin* (berputar kembali ke 0 setelah mencapai batas).
* **`kill(pid, SIGCONT)`**: Melanjutkan eksekusi proses anak berikutnya dalam antrian.

### 4. Demonstrasi Output 📺
* Tunjukkan terminal saat program berjalan.
* Buktikan bahwa output PID berubah-ubah secara berurutan.
    * *Contoh:* Proses 2746 berjalan -> Proses 2747 berjalan -> Proses 2748 berjalan -> Kembali ke 2746.
* Ini adalah bukti bahwa simulasi CPU Time-Sharing berhasil dilakukan.

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
