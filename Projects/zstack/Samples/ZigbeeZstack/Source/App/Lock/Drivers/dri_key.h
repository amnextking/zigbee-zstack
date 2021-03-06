#ifndef DRI_KEY_H_
#define DRI_KEY_H_

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */
#include<ioCC2530.h>


/* ------------------------------------------------------------------------------------------------
 *                                          Define
 * ------------------------------------------------------------------------------------------------
 */

/*门锁反锁状态标识*/
#define LOCK_YES  0     //门反锁
#define LOCK_NO   1     //门未反锁


/*门反锁口*/
#define LOCK_PORT      P0_5  

/* ------------------------------------------------------------------------------------------------
 *                                          Prototypes
 * ------------------------------------------------------------------------------------------------
 */
void Key_Init(void);


#endif