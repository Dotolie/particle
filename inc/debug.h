#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <assert.h>

#ifdef DEBUG
#define DBG(fmt,...) 		printf(fmt, ##__VA_ARGS__);

#define DBG_I_N(fmt,...) 	printf("I:[%s:%s]" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_W(fmt,...) 	printf("I:[%s:%s]" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_R(fmt,...) 	printf("\033[7;31mI:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_E_N(fmt,...) 	printf("E:[%s:%s]" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_E_R(fmt,...) 	printf("\033[7;31mE:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_G(fmt,...)	printf("\033[7;32mI:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_Y(fmt,...)	printf("\033[7;33mW:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_P(fmt,...)	printf("\033[7;35mI:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_C(fmt,...)	printf("\033[7;36mI:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);

#define ASSERT(x)		assert(x);
#else
#define DBG(fmt,...) 

#define DBG_I_N(fmt,...)
#define DBG_I_W(fmt,...) 	printf("I:[%s:%s]" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_R(fmt...)
#define DBG_E_N(fmt...)
#define DBG_E_R(fmt,...) 	printf("\033[7;31mE:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_G(fmt,...)
#define DBG_I_Y(fmt,...)
#define DBG_I_P(fmt,...)
#define DBG_I_C(fmt,...)

#define ASSERT(x)
#endif



#endif /* __DEBUG_H__ */

