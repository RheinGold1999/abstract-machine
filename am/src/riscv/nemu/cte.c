#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

enum {
  SYS_exit,
  SYS_yield,
  SYS_open,
  SYS_read,
  SYS_write,
  SYS_kill,
  SYS_getpid,
  SYS_close,
  SYS_lseek,
  SYS_brk,
  SYS_fstat,
  SYS_time,
  SYS_signal,
  SYS_execve,
  SYS_fork,
  SYS_link,
  SYS_unlink,
  SYS_wait,
  SYS_times,
  SYS_gettimeofday
};

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case -1:
        ev.event = EVENT_YIELD;  // `-1` comes from yield(): "li a7, -1; ecall"
        c->mepc += 4; // MEPC should be added by 4 when it is YIELD
        break;
      default: 
        if ((SYS_exit <= c->mcause) && c->mcause <= SYS_gettimeofday) {
          ev.event = EVENT_SYSCALL;
          if (c->mcause != SYS_execve) {
            c->mepc += 4; // MEPC should be added by 4 when it is SYSCALL
          }
        } else {
          ev.event = EVENT_ERROR; 
        }
        break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context *ctx = (Context *)(kstack.end - sizeof(Context));
  ctx->mepc = (uintptr_t)entry;
  ctx->gpr[10] = (uintptr_t)arg;
  return ctx;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
