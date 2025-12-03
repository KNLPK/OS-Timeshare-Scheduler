#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>

#define NUM_CHILDREN 3
#define TIME_SLICE_MS 200

pid_t child_pids[NUM_CHILDREN];
int current_process_index = 0;

void dummy_task() {
    while (1) {
        printf("Process PID %d | Status: Running\n", getpid());
        usleep(50000); 
    }
}

void scheduler_handler(int signum) {
    if (signum != SIGALRM) return;

    // A. HENTIKAN proses yang sedang berjalan
    pid_t pid_to_stop = child_pids[current_process_index];
    if (pid_to_stop > 0) {
        kill(pid_to_stop, SIGSTOP); 
        printf("\n[SCHEDULER] Time Slice Habis. Menghentikan PID %d\n", pid_to_stop);
    }
    
    // B. Pindah ke proses berikutnya (Round Robin)
    current_process_index = (current_process_index + 1) % NUM_CHILDREN;
    
    // C. MULAI proses berikutnya
    pid_t pid_to_start = child_pids[current_process_index];
    printf("[SCHEDULER] Memberikan CPU ke PID %d\n", pid_to_start);
    kill(pid_to_start, SIGCONT); 

}

int main() {
    // 1. SETUP SINYAL: Hubungkan SIGALRM ke scheduler_handler
    struct sigaction sa;
    sa.sa_handler = scheduler_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, NULL) == -1) {
        perror("sigaction error");
        exit(EXIT_FAILURE);
    }

    // 2. FORK: Membuat Proses Anak
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        
        if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            printf("Child %d created with PID %d\n", i + 1, getpid());
            raise(SIGSTOP); 
            dummy_task(); 
            exit(EXIT_SUCCESS);
        } else {
            child_pids[i] = pid;
        }
    }
    
    printf("Semua %d proses anak telah dibuat dan dihentikan (SIGSTOP).\n", NUM_CHILDREN);

    // 3. SETUP TIMER (setitimer)
    struct itimerval it;
    it.it_value.tv_sec = 0; 
    it.it_value.tv_usec = TIME_SLICE_MS * 1000; 

    it.it_interval = it.it_value; 

    if (setitimer(ITIMER_REAL, &it, NULL) == -1) {
        perror("setitimer error");
        exit(EXIT_FAILURE);
    }
    printf("Timer disetel ke %dms. Penjadwalan dimulai...\n\n", TIME_SLICE_MS);

    // 4. MULAI PROSES PERTAMA
    kill(child_pids[current_process_index], SIGCONT);

    // 5. PENJADWAL BERHENTI MENUNGGU SINYAL (Main Loop)
    while (1) {
        pause(); 
    }

    return 0;
}