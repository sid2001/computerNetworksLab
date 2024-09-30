#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>

#define TIMEOUT 3  // Timeout in seconds for retransmitting frames
#define PROB_FRAME_LOSS 20  // Probability of frame loss (percentage)
#define PROB_ACK_LOSS 10  // Probability of ACK loss (percentage)

int window_size;
int total_seq_numbers;
int total_frames = 10;  // Number of frames to send

typedef struct {
    int seq_num;  // Sequence number of the frame
    bool sent;    // Whether the frame was sent
    bool acked;   // Whether the frame was acknowledged
} Frame;

// Function to calculate window size based on sequence bits
int calculate_window_size(int seq_bits) {
    return (1 << seq_bits);  // Window size = 2^n
}

// Function to randomly drop frames or ACKs
bool middleman_decides_to_drop(int loss_probability) {
    return (rand() % 100) < loss_probability;
}

// Sender function to send frames within the window
void sender(Frame frames[], int *last_frame_sent, int last_ack_received) {
    for (int i = 0; i < window_size; i++) {
        int frame_to_send = (*last_frame_sent + i) % total_seq_numbers;
        if (!frames[frame_to_send].acked && !frames[frame_to_send].sent) {
            printf("Sender: Sending frame %d\n", frames[frame_to_send].seq_num);
            frames[frame_to_send].sent = true;
        }
    }
}

// Receiver function to process received frames
void receiver(Frame frames[], int *frame_expected, int *ack_num) {
    printf("Receiver: Expecting frame %d\n", *frame_expected);
    
    if (middleman_decides_to_drop(PROB_FRAME_LOSS)) {
        printf("Middleman: Dropped frame %d\n", *frame_expected);
    } else {
        printf("Receiver: Received frame %d\n", *frame_expected);
        frames[*frame_expected].acked = true;
        *ack_num = *frame_expected;
        *frame_expected = (*frame_expected + 1) % total_seq_numbers;
    }
}

// Function to simulate middleman behavior with ACK loss
void middleman_forward_ack(int ack_num, int *last_ack_received) {
    if (middleman_decides_to_drop(PROB_ACK_LOSS)) {
        printf("Middleman: Dropped ACK for frame %d\n", ack_num);
    } else {
        printf("Middleman: Forwarding ACK for frame %d\n", ack_num);
        *last_ack_received = ack_num;
    }
}

// Main Selective Repeat ARQ function
int main() {
    srand(time(0));

    int seq_bits;
    printf("Enter the number of sequence address bits: ");
    scanf("%d", &seq_bits);

    total_seq_numbers = 1 << seq_bits;  // Total sequence numbers (2^n)
    window_size = calculate_window_size(seq_bits);  // Calculate Selective Repeat window size
    printf("Window size: %d\n", window_size);

    Frame frames[total_frames];
    for (int i = 0; i < total_seq_numbers; i++) {
        frames[i].seq_num = i;
        frames[i].sent = false;
        frames[i].acked = false;
    }

    int last_frame_sent = 0;  // Next frame to send
    int last_ack_received = -1;  // Last ACK received (sequence number)
    int frame_expected = 0;  // Receiver's expected frame number
    int ack_num = -1;  // Received ACK from the receiver

    while (last_ack_received < total_frames - 1) {
        // Sender sends frames within the window
        sender(frames, &last_frame_sent, last_ack_received);

        // Middleman forwards or drops the frames
        receiver(frames, &frame_expected, &ack_num);

        // Simulate the middleman forwarding or dropping the ACK
        if (ack_num >= 0) {
            middleman_forward_ack(ack_num, &last_ack_received);
        }

        // Update the sender window based on the ACK
        if (last_ack_received >= last_frame_sent) {
            last_frame_sent = (last_ack_received + 1) % total_seq_numbers;
            printf("Sender: Sliding window, next frame to send is %d\n", last_frame_sent);
        }

        // Simulate timeout and retransmission after timeout
        sleep(1);
    }

    printf("All frames sent and acknowledged successfully!\n");
    return 0;
}
