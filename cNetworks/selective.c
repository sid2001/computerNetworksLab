#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

typedef struct {
    int window_size;
    int num_frames;
    int window_start;
    bool* frames;
    int total_transmissions;
    int total_ack_losses;
    int total_packet_losses;
} SelectiveRepeatARQ;

void receive_ack(SelectiveRepeatARQ* arq, int frame_num);

void init_arq(SelectiveRepeatARQ* arq, int window_size, int num_frames) {
    arq->window_size = window_size;
    arq->num_frames = num_frames;
    arq->window_start = 0;
    arq->frames = (bool*)calloc(num_frames, sizeof(bool));
    arq->total_transmissions = 0;
    arq->total_ack_losses = 0;
    arq->total_packet_losses = 0;
}

void send_frame(SelectiveRepeatARQ* arq, int frame_num) {
    bool success = rand() % 2;
    printf("[INFO] Sending frame %d... ", frame_num);
    arq->total_transmissions++;
    if (success) {
        printf("Transmission successful!\n");
        receive_ack(arq, frame_num);
    } else {
        printf("Frame lost!\n");
        arq->total_packet_losses++;
    }
}

void receive_ack(SelectiveRepeatARQ* arq, int frame_num) {
    printf("[INFO] Waiting for ACK for frame %d...\n", frame_num);
    arq->frames[frame_num] = true;
    bool ack_loss = rand() % 2;
    if (ack_loss) {
        printf("[ERROR] ACK for frame %d lost!\n", frame_num);
        arq->total_ack_losses++;
        arq->frames[frame_num] = false;
    } else {
        printf("[INFO] ACK received for frame %d.\n", frame_num);
    }
}

void run_protocol(SelectiveRepeatARQ* arq) {
    while (arq->window_start < arq->num_frames) {
        for (int i = arq->window_start; i < (arq->window_start + arq->window_size) && i < arq->num_frames; ++i) {
            if (!arq->frames[i]) {
                send_frame(arq, i);
            }
        }
        while (arq->window_start < arq->num_frames && arq->frames[arq->window_start]) {
            ++arq->window_start;
            printf("[INFO] Sliding window to start at frame %d.\n", arq->window_start);
        }
        sleep(1);
    }
}

void print_statistics(SelectiveRepeatARQ* arq) {
    printf("\n________________________\n");
    printf("Total Transmissions: %d\n", arq->total_transmissions);
    printf("Total ACK Losses: %d\n", arq->total_ack_losses);
    printf("Total Packet Losses: %d\n", arq->total_packet_losses);
}

int main() {
    srand(time(0)); 

    int window_size = 4;
    int num_frames = 10;

    SelectiveRepeatARQ arq;
    init_arq(&arq, window_size, num_frames);
    run_protocol(&arq);
    print_statistics(&arq);

    free(arq.frames);
    return 0;
}
