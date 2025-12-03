# 🕒 OS Time-Share Scheduler Project

**Topik:** Simulasi penjadwalan time-sharing *Round Robin pre-emptive* menggunakan Bahasa C.  
**Konsep Utama:** Sinyal (`SIGALRM`, `SIGSTOP`, `SIGCONT`) dan Timer (`setitimer`).

---

## ⚙️ Persiapan Lingkungan (Windows Subsystem for Linux - WSL2)

Kode ini menggunakan fitur spesifik OS bergaya UNIX (Linux). Pengguna Windows **wajib** menggunakan WSL2 (Ubuntu).

### Tahap 1: Instalasi WSL2
*(Lewati jika sudah memiliki terminal Ubuntu/Linux)*

1.  Buka **Command Prompt (CMD)** atau **PowerShell**.
2.  Jalankan sebagai Administrator (Klik kanan > *Run as administrator*).
3.  Ketik perintah berikut dan tekan Enter:
    ```powershell
    wsl --install
    ```
4.  **Restart Komputer Anda**.
5.  Setelah restart, terminal akan terbuka otomatis. Buat **Username** dan **Password** Linux Anda.

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

2.  **Buat File Source Code:**
    Buat file bernama `scheduler.c` dan buka text editor (misalnya `nano`):
    ```bash
    nano scheduler.c
    ```
    *(Paste kode C Anda di sini, lalu simpan dengan `Ctrl+O`, Enter, dan keluar dengan `Ctrl+X`)*.

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

## 🎯 Panduan Penjelasan Video (Bobot Nilai: 30%)

Rekan tim yang bertugas membuat video harus menjelaskan poin-poin teknis berikut dalam durasi **5-10 menit**:

### 1. Mekanisme Interupsi Timer ⏱️
* Jelaskan fungsi `setitimer(ITIMER_REAL, ...)` yang ada di fungsi `main`.
* Jelaskan bahwa fungsi ini mengatur interval waktu (misalnya **200ms**).
* Fungsi ini secara otomatis mengirimkan sinyal **`SIGALRM`** ke proses induk. Ini adalah simulasi dari *Hardware Clock Interrupt* pada OS nyata.

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
