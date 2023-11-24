#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include "confuse.h"
#include <errno.h>

#define _XOPEN_SOURCE 700
#define _POSIX_C_SOURCE 199309L

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

  if (chdir(".") < 0) {
    syslog(LOG_CRIT, "невозможно сделать текущим рабочим катологом текущий рабочий каталог");
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

static size_t readSizeFile() {
  char* filePath = NULL;
  cfg_opt_t opts[] = {
    CFG_SIMPLE_STR("filePath", &filePath),
    CFG_END()
  };
  cfg_t* cfg = cfg_init(opts, 0);
  cfg_parse(cfg, "daemon.conf");
  cfg_free(cfg);

  FILE* file = fopen(filePath, "r");
  if (file == NULL) {
    syslog(LOG_INFO, "Не удалось прочитать файл %s", filePath);
    return 0;
  }

  struct stat buffer;
  stat(filePath, &buffer);
  const size_t size = buffer.st_size;

  fclose(file);

  return size;
}

int main(int num, char** args) {
  if (num == 2 && memcmp(args[1], "toggleDaemon", 12) == 0) {
    daemonize("my_daemon");
  }

  int server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (server_socket < 0) {
    syslog(LOG_CRIT, " Ошибка открытия сокета");
    exit(0);
  }

  int one = 1;
  setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)); 

  struct sockaddr_un server_addr;
  server_addr.sun_family = AF_UNIX;
  strcpy(server_addr.sun_path, "daemon.socket");
  
  if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(struct sockaddr_un)) < 0) {
    syslog(LOG_ERR, "bind error socket: %s", strerror(errno));
    goto clear_sock;
  }

  listen(server_socket, 5);
  
  int msgsock = accept(server_socket, 0, 0);
  if (msgsock == -1) {
    syslog(LOG_CRIT, "error accept socket");
    goto clear_all;
  }
    
  const size_t size = readSizeFile();
  char buf[256] = "";
  snprintf(buf,  sizeof buf, "%zu", size);
  send(msgsock, buf, strlen(buf), 0);
  close(msgsock);
  goto clear_all;
clear_all:
  close(server_socket);
  unlink("daemon.socket");
  return 0;
clear_sock:
  close(server_socket);
  return 0;
}
