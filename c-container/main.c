#define _GNU_SOURCE
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define STACK_SIZE (1024 * 1024)  // スタックサイズ
#define CGROUP_MEMORY_FOLDER "/sys/fs/cgroup/memory/my_container/"
#define CGROUP_CPU_FOLDER "/sys/fs/cgroup/cpu/my_container/"
#define CGROUP_PROCS_FILE_MEMORY "/sys/fs/cgroup/memory/my_container/cgroup.procs"
#define CGROUP_PROCS_FILE_CPU "/sys/fs/cgroup/cpu/my_container/cgroup.procs"
#define CGROUP_MEMORY_LIMIT_FILE "/sys/fs/cgroup/memory/my_container/memory.limit_in_bytes"
#define CGROUP_CPU_SHARES_FILE "/sys/fs/cgroup/cpu/my_container/cpu.shares"

// コンテナ内で実行される関数
int container_main(void *arg) {
    printf("Container - inside the container!\n");

    // /procをマウントする
    if (mount("proc", "/proc", "proc", 0, NULL) != 0) {
        perror("mount");
        return 1;
    }

    // シェルを起動する
    char *const args[] = {"/bin/bash", NULL};
    execv(args[0], args);

    // execvが失敗した場合
    perror("execv");
    return 1;
}

// cgroupsの設定
void setup_cgroup(pid_t pid) {
    char pid_str[16];
    snprintf(pid_str, sizeof(pid_str), "%d", pid);

    // メモリcgroupディレクトリを作成
    mkdir(CGROUP_MEMORY_FOLDER, 0755);

    // メモリ制限の設定
    int memory_fd = open(CGROUP_MEMORY_LIMIT_FILE, O_WRONLY);
    if (memory_fd == -1) {
        perror("open memory.limit_in_bytes");
        exit(EXIT_FAILURE);
    }
    if (write(memory_fd, "536870912", 9) == -1) { // 512MB
        perror("write memory.limit_in_bytes");
        exit(EXIT_FAILURE);
    }
    close(memory_fd);

    // CPUcgroupディレクトリを作成
    mkdir(CGROUP_CPU_FOLDER, 0755);

    // CPU制限の設定
    int cpu_fd = open(CGROUP_CPU_SHARES_FILE, O_WRONLY);
    if (cpu_fd == -1) {
        perror("open cpu.shares");
        exit(EXIT_FAILURE);
    }
    if (write(cpu_fd, "512", 3) == -1) {
        perror("write cpu.shares");
        exit(EXIT_FAILURE);
    }
    close(cpu_fd);

    // プロセスをメモリcgroupに追加
    int procs_fd_memory = open(CGROUP_PROCS_FILE_MEMORY, O_WRONLY);
    if (procs_fd_memory == -1) {
        perror("open cgroup.procs (memory)");
        exit(EXIT_FAILURE);
    }
    if (write(procs_fd_memory, pid_str, strlen(pid_str)) == -1) {
        perror("write cgroup.procs (memory)");
        exit(EXIT_FAILURE);
    }
    close(procs_fd_memory);

    // プロセスをCPUcgroupに追加
    int procs_fd_cpu = open(CGROUP_PROCS_FILE_CPU, O_WRONLY);
    if (procs_fd_cpu == -1) {
        perror("open cgroup.procs (cpu)");
        exit(EXIT_FAILURE);
    }
    if (write(procs_fd_cpu, pid_str, strlen(pid_str)) == -1) {
        perror("write cgroup.procs (cpu)");
        exit(EXIT_FAILURE);
    }
    close(procs_fd_cpu);
}

int main() {
    char *stack;
    char *stack_top;
    pid_t pid;

    // スタックメモリを確保
    stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    stack_top = stack + STACK_SIZE;  // スタックのトップを設定

    // 新しいプロセスを作成し、名前空間を分離
    pid = clone(container_main, stack_top, CLONE_NEWPID | CLONE_NEWUTS | CLONE_NEWNS | SIGCHLD, NULL);
    if (pid == -1) {
        perror("clone");
        free(stack);
        exit(EXIT_FAILURE);
    }

    // cgroupsの設定
    // setup_cgroup(pid);

    // 子プロセスの終了を待つ
    if (waitpid(pid, NULL, 0) == -1) {
        perror("waitpid");
        free(stack);
        exit(EXIT_FAILURE);
    }

    // スタックメモリを解放
    free(stack);

    printf("Container - outside the container!\n");

    return 0;
}
