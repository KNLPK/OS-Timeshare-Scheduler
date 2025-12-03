OS-Timeshare-Scheduler

Simulasi penjadwalan time-sharing Round Robin pre-emptive menggunakan C, sinyal (SIGALRM, SIGSTOP, SIGCONT), dan timer (setitimer). Mata Kuliah: COMP6697 Operating System.

⚙️ Persiapan Lingkungan (Menggunakan Windows Subsystem for Linux - WSL2)

Karena kode ini memerlukan fitur OS bergaya UNIX (seperti fork(), signal(), dan setitimer()), kita harus bekerja di lingkungan Linux.

Tahap 1: Instalasi WSL2 (Hanya untuk Pengguna Windows)

Buka Command Prompt (CMD) atau PowerShell

Jalankan sebagai Administrator (klik kanan > Run as administrator).

Jalankan perintah instalasi otomatis:

wsl --install


Perintah ini akan mengaktifkan fitur yang diperlukan, mengunduh kernel Linux, dan menginstal distribusi Ubuntu secara default.

Jika diminta, Restart Komputer Anda.

Setelah restart, aplikasi Ubuntu akan terbuka. Ikuti petunjuk untuk membuat Username dan Password Linux Anda.

Tahap 2: Instalasi Compiler C (GCC) di WSL

Setelah Anda berhasil masuk ke terminal Ubuntu (WSL):

Perbarui Daftar Paket:

sudo apt update


(Masukkan password Linux Anda saat diminta.)

Instal Tools Development Esensial (GCC):

sudo apt install build-essential


Paket ini mencakup GNU Compiler Collection (GCC) yang akan kita gunakan untuk mengkompilasi kode C.

Verifikasi Instalasi:

gcc --version


Pastikan versi GCC muncul. Lingkungan Anda sekarang siap.

💻 Implementasi dan Kompilasi

Tahap 3: Menyiapkan Proyek dan Kode

Pindah ke Home Directory dan Buat Folder Proyek:

cd ~
mkdir aol_timeshare_project
cd aol_timeshare_project


Buat File Kode:
Buat file scheduler.c dan masukkan semua kode yang telah disediakan (kode C yang sudah disiapkan).

nano scheduler.c
# (Copy-Paste kode C ke editor nano, lalu simpan dengan Ctrl+O, Enter, Ctrl+X)


Tahap 4: Kompilasi dan Eksekusi

Kompilasi Kode:

gcc scheduler.c -o scheduler


Perintah ini membuat file executable bernama scheduler.

Jalankan Program:

./scheduler
