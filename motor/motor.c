#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include <sys/stat.h>
#include <fcntl.h>
/* ioctl cmd */
#define MOTOR_STOP        0x1
#define MOTOR_RESET        0x2
#define MOTOR_MOVE        0x3
#define MOTOR_GET_STATUS    0x4
#define MOTOR_SPEED        0x5
/* directional_attr */
#define MOTOR_DIRECTIONAL_UP    0x0
#define MOTOR_DIRECTIONAL_DOWN    0x1
#define MOTOR_DIRECTIONAL_LEFT    0x2
#define MOTOR_DIRECTIONAL_RIGHT    0x3
#define MOTOR_INIT_SPEED    1000    /* unit :ns */

#include <arpa/inet.h>
#include <string.h>

typedef struct {
    int motor_directional;
    int motor_move_steps;
    int motor_move_speed;
} motor_move_st;

typedef struct  {
    int y_max;
    int y_min;
    int x_max;
    int x_min;
    int x_steps;
    int y_steps;
} motor_status_st;


void sendCommand(int cmd, void *buffer) {
    int fd = open("/dev/motor", O_WRONLY);
    ioctl(fd, cmd, buffer);
    close(fd);
}

void setSpeed(int speed) {
    int motorspeed = speed;
    sendCommand(MOTOR_SPEED, &motorspeed);
}

void setMovement(int direction, int steps) {


    motor_move_st motor_move;
    motor_move.motor_directional = direction;
    motor_move.motor_move_speed = 1000;
    motor_move.motor_move_steps = steps;
    sendCommand(MOTOR_MOVE, &motor_move);


}

void setStop() {
    sendCommand(MOTOR_STOP, 0);
}

void reset() {
    sendCommand(MOTOR_RESET, 0);
}


void getStatus() {
    motor_status_st status;
    sendCommand(MOTOR_GET_STATUS, &status);


    printf("y_max: %d\n", status.y_max);
    printf("y_min: %d\n", status.y_min);
    printf("x_max: %d\n", status.x_max);
    printf("x_min: %d\n", status.x_min);
    printf("x_steps: %d\n", status.x_steps);
    printf("y_steps: %d\n", status.y_steps);


}


int main(int argc, char *argv[]) {

    char direction = 0;
    int stepsize = 100;

    // Parse Arguments:
    int c;
    while ((c = getopt(argc, argv, "d:s:")) != -1) {
        switch (c) {
            case 'd':
                direction = optarg[0];
                break;
            case 's':
                stepsize = atoi(optarg);
                break;
            default:
                printf("Invalid Argument %c\n", c);
                exit(EXIT_FAILURE);
        }
    }


    switch (direction) {
        case 'u':
            setMovement(MOTOR_DIRECTIONAL_UP, stepsize);
            break;
        case 'd':
            setMovement(MOTOR_DIRECTIONAL_DOWN, stepsize);
            break;
        case 'l':
            setMovement(MOTOR_DIRECTIONAL_LEFT, stepsize);
            break;
        case 'r':
            setMovement(MOTOR_DIRECTIONAL_RIGHT, stepsize);
            break;
        default:
            printf("Invalid Direction Argument %c\n", c);
            exit(EXIT_FAILURE);
    }
    getStatus();

    return 0;


}
