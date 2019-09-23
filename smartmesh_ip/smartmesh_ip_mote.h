#ifndef SMARTMESH_IP_MOTE_H
#define SMARTMESH_IP_MOTE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smartmesh_ip.h"


/**
 * @brief 初始化 mote 模块
 * 
 * @param mote mote 句柄
 * @param type mote 类型
 * 
 * @return 错误码
 */
int smip_mote_init(sSmartMeshIP *mote);

/**
 * @brief 销毁 mote 模块
 * 
 * @param mote mote 句柄
 * 
 * @return 错误码
 */
int smip_mote_deinit(sSmartMeshIP *mote);

void smip_mote_loop(sSmartMeshIP *mote);

#ifdef __cplusplus
}
#endif

#endif // SMARTMESH_IP_MOTE_H

// EOF
