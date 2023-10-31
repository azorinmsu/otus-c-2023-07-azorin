#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>

static void daemonize(const char* cmd) {
  int               fd0, fd1, fd2;
  pid_t             pid;
  struct rlimit     rl;
  struct sigaction  sa;

  openlog(cmd, LOG_CONS, LOG_DAEMON);

  umask(0);

  if (getrlimit(RLIMIT_NOFILE, &rl) < 0) {
    perror("Невозможно получить максимальный номер дескриптора");
  }

  if ((pid = fork()) < 0) {
    perror("ошибка вызова функции fork");
  } else if (pid != 0) {
    exit(0);
  }
  setsid();

  sa.sa_handler = SIG_IGN;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  if (sigaction(SIGHUP, &sa, NULL) < 0) {
    syslog(LOG_CRIT, "невозможно игнорировать сигнал SIGHUP");
  }

  if ((pid = fork()) < 0) {
    perror("ошибка вызова функции fork");
  } else if (pid != 0) {
    exit(0);
  }

  if (chdir("/") < 0) {
    syslog(LOG_CRIT, "невозможно сделать текущим рабочим катологом /");
  }

  if (rl.rlim_max == RLIM_INFINITY) {
    rl.rlim_max = 1024;
  }
  for (rlim_t i = 0; i < rl.rlim_max; ++i) {
    close(i);
  }

  fd0 = open("/dev/null", O_RDWR);
  fd1 = dup(0);
  fd2 = dup(0);

  if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
    syslog(LOG_CRIT, "Неверные файловые дескрипторы %d %d %d", fd0, fd1, fd2); 
  }
}

int main() {
  daemonize("test_daemon");
}
