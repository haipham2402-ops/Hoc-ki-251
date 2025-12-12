#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

int empty(struct queue_t *q)
{
    if (q == NULL)
        return 1;
    return (q->size == 0);
}

void enqueue(struct queue_t *q, struct pcb_t *proc)
{
    /* TODO: put a new process to queue [q] */
    if(q->size < MAX_QUEUE_SIZE){
        q->proc[q->size] = proc;
        q->size++;
    }
}

struct pcb_t *dequeue(struct queue_t *q)
{
    /* TODO: return a pcb whose prioprity is the highest
     * in the queue [q] and remember to remove it from q
     * */
    if (q == NULL || q->size == 0) {
        return NULL;
    }

    struct pcb_t *first_queue = q->proc[0];

    // Dồn hàng đợi lên
    for(int i = 0; i < q->size; i++){
        q->proc[i] = q->proc[i+1];
    }
    
    // Xóa phần tử cuối (optional, cho sạch)
    q->proc[q->size - 1] = NULL;
    q->size--;

    return first_queue;
}

/* Lưu ý: Hàm này trong đề bài ghi là "omitted" (bỏ qua), 
   nhưng nếu cần hiện thực để code chạy an toàn thì làm như sau */
struct pcb_t *purgequeue(struct queue_t *q, struct pcb_t *proc)
{
    /* TODO: remove a specific item from queue
     * */
    // Tìm vị trí của proc trong queue
    int idx = -1;
    for (int i = 0; i < q->size; i++) {
        if (q->proc[i] == proc) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {
        struct pcb_t * removed_proc = q->proc[idx];
        // Dồn hàng đợi từ vị trí idx
        for (int i = idx; i < q->size - 1; i++) {
            q->proc[i] = q->proc[i+1];
        }
        q->proc[q->size - 1] = NULL;
        q->size--;
        return removed_proc;
    }

    return NULL;
}