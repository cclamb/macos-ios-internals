#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/event.h>

#define ERROR_QUEUE 2
#define ERROR_EVENT 3
#define ERROR_ARGS  4

#define EXIT_OK     0

int main(int argc, char* argv[]) {
  pid_t pid;
  int kq;
  int rc;
  int done;
  struct kevent ke;

  if (argc != 2) {
    perror("args");
    exit(ERROR_ARGS);
  }

  pid = atoi(argv[1]);

  kq = kqueue();
  if (kq == -1) {
    perror("kqueue");
    exit(ERROR_QUEUE);
  }

  EV_SET(&ke, pid, EVFILT_PROC, EV_ADD, NOTE_EXIT | NOTE_FORK | NOTE_EXEC, 0, NULL);

  // registering the event
  rc = kevent(kq, &ke, 1, NULL, 0, NULL);
  if (rc < 0) {
    perror("kevent creation");
    exit(ERROR_EVENT);
  }

  done = 0;
  while (!done) {
    memset(&ke, '\0', sizeof(struct kevent));
    rc = kevent(kq, NULL, 0, &ke, 1, NULL );

    if (rc < 0) {
      perror("event monitoring loop");
      exit(ERROR_EVENT);
    }

    if (ke.fflags & NOTE_FORK) {
      printf("PID %ld fork()ed\n", ke.ident);
    }

    if (ke.fflags & NOTE_EXEC) {
      printf("pid %ld has exec()ed\n", ke.ident);
    }

    if (ke.fflags & NOTE_EXIT) {
        printf("pid %ld has exited\n", ke.ident);
        done++;
    }
  }

  exit(EXIT_OK);
}
