#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

extern void kernel (unsigned n, float a[n][n], float b[n][n], float x);

// Fonction pour ouvrir un événement de performance
static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
			    int cpu, int group_fd, unsigned long flags)
{
  int ret;

  ret = syscall(__NR_perf_event_open, hw_event, pid, cpu,
		group_fd, flags);
  return ret;
}

// Fonction pour mesurer les cycles de CPU
uint64_t perf_event_count(uint64_t n, float a[n][n], float b[n][n], float x) {
  struct perf_event_attr pe;
  int fd;
  uint64_t count;

  // Initialiser les attributs de l'événement de performance
  memset(&pe, 0, sizeof(struct perf_event_attr));
  pe.type = PERF_TYPE_HARDWARE;
  pe.size = sizeof(struct perf_event_attr);
  pe.config = PERF_COUNT_HW_CPU_CYCLES;
  pe.disabled = 1;
  pe.exclude_kernel = 1;
  pe.exclude_hv = 1;

  // Ouvrir l'événement de performance
  fd = perf_event_open(&pe, 0, -1, -1, 0);
  if (fd == -1) {
    fprintf(stderr, "Error opening leader %llx\n", pe.config);
    exit(EXIT_FAILURE);
  }

  // Réinitialiser et activer l'événement
  ioctl(fd, PERF_EVENT_IOC_RESET, 0);
  ioctl(fd, PERF_EVENT_IOC_ENABLE, 0);

  // Exécuter la fonction kernel
  kernel(n, a, b, x);

  // Désactiver l'événement
  ioctl(fd, PERF_EVENT_IOC_DISABLE, 0);

  // Lire le nombre de cycles
  if (read(fd, &count, sizeof(uint64_t)) == -1) {
    perror("read");
    exit(EXIT_FAILURE);
  }

  close(fd);
  return count;
}
