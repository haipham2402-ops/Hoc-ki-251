#ifndef INC_SCH_H_
#define INC_SCH_H_

#include "main.h"

#define MAX_TASKS 40

// ===== Cấu trúc mô tả 1 task =====
typedef struct {
    void (*pTask)(void);     // Con trỏ hàm trỏ tới task (hàm người dùng)
    uint32_t Delay;          // Thời gian chờ trước khi chạy (ms)
    uint32_t Period;         // Chu kỳ lặp lại (ms)
    uint8_t RunMe;           // Cờ báo cần thực thi
    uint32_t TaskID;         // ID để quản lý task (tuỳ chọn)
} sTask;

// ===== Prototype các hàm của Scheduler =====
void SCH_Init(void);
void SCH_Update(void);
void SCH_Dispatch_Tasks(void);

uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD);
uint8_t SCH_Delete_Task(uint32_t TASK_INDEX);

// ===== Các biến dùng chung (nếu cần) =====
extern sTask SCH_tasks_G[MAX_TASKS];

#endif /* INC_SCH_H_ */
