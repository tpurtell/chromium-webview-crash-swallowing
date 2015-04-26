#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <android/log.h>
#include <linux/user.h>
#include <signal.h>
#include <errno.h>
#include "ucontext.h"

#define NUM_SIG 64
void     (*actions[NUM_SIG])(int, siginfo_t *, void *);
struct sigaction* base_actions[NUM_SIG];
bool hooked[NUM_SIG];

#if defined(__arm__)
  #define PC(c) (void*)(c ? ((ucontext_t*)(c))->uc_mcontext.arm_pc : 0xffffffff)
#elif defined(__i386__)
  #define PC(c) (void*)(c ? ((ucontext_t*)(c))->uc_mcontext.gregs[REG_EIP] : 0xffffffff)
#else
  #define PC(c) (void*)0xffffffff
#endif


#define SIGNAL_WRAPPER(s) \
	struct sigaction base_sigaction_##s; \
	static void propagate_sigaction_##s (int a, siginfo_t * b, void * c) { \
		__android_log_print(ANDROID_LOG_DEBUG, "WRAPSIG", "received %d, code %p, addr %p, pc %p\n", s, (void*)b->si_code, (void*)b->si_addr, PC(c)); \
		if( base_sigaction_##s.sa_flags & SA_SIGINFO ) { \
			base_sigaction_##s.sa_sigaction(a, b, c); \
		} else { \
			base_sigaction_##s.sa_handler(a); \
		} \
	} \
	static bool setup_signal_##s () { \
		actions[s] = propagate_sigaction_##s; \
		base_actions[s] = & base_sigaction_##s; \
		return true; \
	} \
	static bool setup_##s = setup_signal_##s();

SIGNAL_WRAPPER(1)
SIGNAL_WRAPPER(2)
SIGNAL_WRAPPER(3)
SIGNAL_WRAPPER(4)
SIGNAL_WRAPPER(5)
SIGNAL_WRAPPER(6)
SIGNAL_WRAPPER(7)
SIGNAL_WRAPPER(8)
SIGNAL_WRAPPER(9)
SIGNAL_WRAPPER(10)
SIGNAL_WRAPPER(11)
SIGNAL_WRAPPER(12)
SIGNAL_WRAPPER(13)
SIGNAL_WRAPPER(14)
SIGNAL_WRAPPER(15)
SIGNAL_WRAPPER(16)
SIGNAL_WRAPPER(17)
SIGNAL_WRAPPER(18)
SIGNAL_WRAPPER(19)
SIGNAL_WRAPPER(20)
SIGNAL_WRAPPER(21)
SIGNAL_WRAPPER(22)
SIGNAL_WRAPPER(23)
SIGNAL_WRAPPER(24)
SIGNAL_WRAPPER(25)
SIGNAL_WRAPPER(26)
SIGNAL_WRAPPER(27)
SIGNAL_WRAPPER(28)
SIGNAL_WRAPPER(29)
SIGNAL_WRAPPER(30)
SIGNAL_WRAPPER(31)
SIGNAL_WRAPPER(32)
SIGNAL_WRAPPER(33)
SIGNAL_WRAPPER(34)
SIGNAL_WRAPPER(35)
SIGNAL_WRAPPER(36)
SIGNAL_WRAPPER(37)
SIGNAL_WRAPPER(38)
SIGNAL_WRAPPER(39)
SIGNAL_WRAPPER(40)
SIGNAL_WRAPPER(41)
SIGNAL_WRAPPER(42)
SIGNAL_WRAPPER(43)
SIGNAL_WRAPPER(44)
SIGNAL_WRAPPER(45)
SIGNAL_WRAPPER(46)
SIGNAL_WRAPPER(47)
SIGNAL_WRAPPER(48)
SIGNAL_WRAPPER(49)
SIGNAL_WRAPPER(50)
SIGNAL_WRAPPER(51)
SIGNAL_WRAPPER(52)
SIGNAL_WRAPPER(53)
SIGNAL_WRAPPER(54)
SIGNAL_WRAPPER(55)
SIGNAL_WRAPPER(56)
SIGNAL_WRAPPER(57)
SIGNAL_WRAPPER(58)
SIGNAL_WRAPPER(59)
SIGNAL_WRAPPER(60)
SIGNAL_WRAPPER(61)
SIGNAL_WRAPPER(62)
SIGNAL_WRAPPER(63)

extern "C" int hook_signal(int i) {
	if(hooked[i]) {
		__android_log_print(ANDROID_LOG_DEBUG, "WRAPSIG", "already hooked %d\n", i);
		return -1;
	}
	struct sigaction* dest = base_actions[i];
	void     (*handler)(int, siginfo_t *, void *) = actions[i];

	int result = sigaction(i, NULL, dest);

	//no signal handler
	if(result < 0 || dest->sa_handler == NULL && dest->sa_sigaction == NULL) {
		__android_log_print(ANDROID_LOG_DEBUG, "WRAPSIG", "no existing handler %d\n", i);
		return -1;
	}

	//create wrapper descriptor
	struct sigaction repl = *dest;
	if(dest->sa_handler)
		repl.sa_handler = (void(*)(int))handler;
	if(dest->sa_sigaction)
		repl.sa_sigaction = handler;
		
	//replace handler
	result = sigaction(i, &repl, NULL);
	if(result < 0) 
	{
		__android_log_print(ANDROID_LOG_DEBUG, "WRAPSIG", "unable to set signal handler %d, %d, %d\n", i, result, errno);
		return -1;
	}
	__android_log_print(ANDROID_LOG_DEBUG, "WRAPSIG", "wrapped signal %d\n", i);
	hooked[i] = true;
	return 0;
}
