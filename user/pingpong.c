// pingpong.c for xv6 (assignment #1 for CSC.T371)
// name: Riku Yamagami
// id: 18B15744

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(1, "usage: %s N\n", argv[0]);
        exit(1);
    }

    // # of rounds
    int n = atoi(argv[1]);

    // tick value before starting rounds
    int start_tick = uptime();

    int c2p[2]; pipe(c2p); // child to parent
    int p2c[2]; pipe(p2c); // parent to child

    int pid;
    pid = fork ();

    if (pid == 0) {
        // child
        close(c2p[0]);
        close(p2c[1]);

        int num;
        for(int i=0;i<n;i++) {
            read(p2c[0],&num,sizeof(num));
            num += 1;
            write(c2p[1],&num,sizeof(num));
        }

        close(c2p[1]);
        close(p2c[0]);

        exit(0);
    } else {
        // parent
        close(p2c[0]);
        close(c2p[1]);

        int num = 0;
        for(int i=0;i<n;i++) {
            write(p2c[1],&num,sizeof(num));
            read(c2p[0],&num,sizeof(num));
            num += 1;
        }

        close(p2c[1]);
        close(c2p[0]);
        wait(0);
    }

    // print # of ticks in nrouds
    printf("# of ticks in %d rounds: %d\n", n, uptime() - start_tick);
    exit(0);
}
