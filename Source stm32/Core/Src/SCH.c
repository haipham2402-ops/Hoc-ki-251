#include "SCH.h"

sTask SCH_tasks_G[MAX_TASKS];
// =========================================
// Hàm khởi tạo Scheduler
// =========================================
void SCH_Init(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        SCH_tasks_G[i].pTask = 0;
        SCH_tasks_G[i].Delay = 0;
        SCH_tasks_G[i].Period = 0;
        SCH_tasks_G[i].RunMe = 0;
        SCH_tasks_G[i].TaskID = i;
    }
}

// =========================================
// Hàm cập nhật Scheduler (gọi trong ngắt timer định kỳ)
// =========================================
void SCH_Update(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (SCH_tasks_G[i].pTask) {
            if (SCH_tasks_G[i].Delay == 0) {
                SCH_tasks_G[i].RunMe += 1; // Đặt cờ thực thi
                if (SCH_tasks_G[i].Period) {
                    SCH_tasks_G[i].Delay = SCH_tasks_G[i].Period; // Reset delay
                }
            } else {
                SCH_tasks_G[i].Delay -= 1;
            }
        }
    }
}

// =========================================
// Hàm thực thi các task đã sẵn sàng
// =========================================
void SCH_Dispatch_Tasks(void) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (SCH_tasks_G[i].pTask && SCH_tasks_G[i].RunMe > 0) {
            (*SCH_tasks_G[i].pTask)(); // Gọi hàm task
            SCH_tasks_G[i].RunMe -= 1;
        }
    }
}

// =========================================
// Thêm task mới
// =========================================
uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (SCH_tasks_G[i].pTask == 0) {
            SCH_tasks_G[i].pTask = pFunction;
            SCH_tasks_G[i].Delay = DELAY;
            SCH_tasks_G[i].Period = PERIOD;
            SCH_tasks_G[i].RunMe = 0;
            SCH_tasks_G[i].TaskID = i;
            return i;
        }
    }
    return MAX_TASKS; // Không còn slot trống
}

// =========================================
// Xóa task theo chỉ số
// =========================================
uint8_t SCH_Delete_Task(uint32_t TASK_INDEX) {
    if (TASK_INDEX >= MAX_TASKS || SCH_tasks_G[TASK_INDEX].pTask == 0) return 0;

    SCH_tasks_G[TASK_INDEX].pTask = 0;
    SCH_tasks_G[TASK_INDEX].Delay = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe = 0;
    return 1;
}
