/**
 * Pass message from parent to child via queue.
 */

#include <stdio.h>
#include <sys/msg.h>
#include <unistd.h>

typedef struct { 
	long mtype;
	int mtext;
} msg_t;

int main(int argc, char *argv[]) {
	int msgqid = msgget(IPC_PRIVATE, IPC_CREAT |IPC_EXCL | 0600); // new queue

	switch(fork()) {
		case -1:
			perror("fork failed");
			return 1;

		case 0: { // child
			msg_t msg;
			msgrcv(msgqid, &msg, sizeof(msg.mtext), 42, 0);
			printf("received %d\n", msg.mtext);
			msgctl(msgqid, IPC_RMID, NULL);
			break;
		}

		default: { // parent
			msg_t msg = { 42, 252 };
			msgsnd(msgqid, &msg, sizeof(msg.mtext), 0);
			break;
		}
	}

	return 0;
}

