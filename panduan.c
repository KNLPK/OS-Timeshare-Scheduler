#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>

// --- KONFIGURASI SIMULASI ---
#define NUM_CHILDREN 3      // Jumlah proses (tugas) yang akan dijadwalkan.
#define TIME_SLICE_MS 200   // Kuanta Waktu (Time Slice) dalam milidetik (ms).

// Array global untuk menyimpan ID Proses Anak (PID).
// PID ini akan dikendalikan oleh Penjadwal (Proses Induk).
pid_t child_pids[NUM_CHILDREN];
int current_process_index = 0; // Indeks proses yang sedang berjalan saat ini.

// --- FUNGSI PROSES ANAK (Tugas yang Diperlukan CPU) ---
void dummy_task() {
    // Ini adalah 'tugas' sederhana yang harus dijalankan oleh setiap proses anak.
    // Dalam OS nyata, ini bisa berupa perhitungan, I/O, dll.
    while (1) {
        printf("Process PID %d | Status: Running\n", getpid());
        // usleep() digunakan agar output tidak terlalu cepat dan bisa dibaca, 
        // tetapi tidak memengaruhi logika penjadwalan.
        usleep(50000); 
    }
}

// --- HANDLER SINYAL (Inti dari Penjadwal) ---
// Fungsi ini dipanggil setiap kali timer (setitimer) mengirim SIGALRM.
// Ini mensimulasikan Interrupt Hardware Clock.
void scheduler_handler(int signum) {
    if (signum != SIGALRM) return;

    // 1. PRE-EMPTION: Hentikan proses yang saat ini memegang CPU.
    pid_t pid_to_stop = child_pids[current_process_index];
    if (pid_to_stop > 0) {
        // Kirim SIGSTOP: Sinyal untuk menangguhkan (pause) eksekusi proses.
        kill(pid_to_stop, SIGSTOP); 
        printf("\n[SCHEDULER] Time Slice Habis. Menghentikan PID %d\n", pid_to_stop);
    }
    
    // 2. LOGIKA ROUND ROBIN: Pindah ke proses berikutnya dalam antrean.
    current_process_index = (current_process_index + 1) % NUM_CHILDREN;
    
    // 3. CONTEXT SWITCH: Mulai proses yang baru terpilih.
    pid_t pid_to_start = child_pids[current_process_index];
    printf("[SCHEDULER] Memberikan CPU ke PID %d\n", pid_to_start);
    // Kirim SIGCONT: Sinyal untuk melanjutkan (resume) eksekusi proses.
    kill(pid_to_start, SIGCONT); 
    
    // Catatan: setitimer otomatis akan mengatur alarm berikutnya, 
    // jadi tidak perlu memanggil setitimer lagi di dalam handler.
}

int main() {
    // Proses Induk bertindak sebagai Penjadwal.

    // 1. SETUP SINYAL: Mendaftarkan fungsi scheduler_handler untuk sinyal SIGALRM.
    // Kita menggunakan sigaction karena ini lebih modern dan aman daripada signal().
    struct sigaction sa;
    sa.sa_handler = scheduler_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("Gagal mendaftarkan sigaction");
        exit(EXIT_FAILURE);
    }

    // 2. FORK: Membuat Proses Anak (Tugas)
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("Gagal membuat proses anak (fork)");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // -- BAGIAN KODE PROSES ANAK --
            printf("Child %d created with PID %d\n", i + 1, getpid());
            // Proses anak harus segera berhenti (SIGSTOP) setelah dibuat, 
            // menunggu giliran pertamanya dari Penjadwal (Proses Induk).
            raise(SIGSTOP); 
            dummy_task(); // Tugas dijalankan hanya setelah menerima SIGCONT
            exit(EXIT_SUCCESS);
        } else {
            // -- BAGIAN KODE PROSES INDUK --
            child_pids[i] = pid; // Simpan PID anak
        }
    }
    
    printf("Semua %d proses anak telah dibuat dan berada dalam status 'Stopped'.\n", NUM_CHILDREN);

    // 3. SETUP TIMER (setitimer): Mengatur Kuanta Waktu
    struct itimerval it;
    
    // Nilai awal timer
    it.it_value.tv_sec = 0; 
    it.it_value.tv_usec = TIME_SLICE_MS * 1000; // Konversi ms ke mikrosekon
    
    // Nilai interval berulang (PENTING: agar timer terus mengirim SIGALRM)
    it.it_interval = it.it_value; 

    // setitimer(ITIMER_REAL): Mengatur timer real-time yang mengirim SIGALRM
    if (setitimer(ITIMER_REAL, &it, NULL) == -1) {
        perror("Gagal menyetel timer");
        exit(EXIT_FAILURE);
    }
    printf("Timer disetel ke %dms. Penjadwalan Round Robin dimulai...\n\n", TIME_SLICE_MS);

    // 4. MULAI PROSES PERTAMA
    // Proses Induk harus memulai proses pertama secara manual
    kill(child_pids[current_process_index], SIGCONT);

    // 5. PENJADWAL BERHENTI MENUNGGU SINYAL
    // Penjadwal (Proses Induk) hanya menunggu interupsi (SIGALRM) untuk bekerja.
    while (1) {
        pause(); // Menghentikan Proses Induk sampai sinyal diterima.
    }

    return 0; // Seharusnya tidak pernah tercapai
}
