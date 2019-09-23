#ifndef SMARTMESH_IP_MNGR_H
#define SMARTMESH_IP_MNGR_H

#ifdef __cplusplus
extern "C" {
#endif

#include "smartmesh_ip.h"


/**
 * @brief 初始化 mngr 模块
 * 
 * @param mngr mngr 句柄
 * @param type mngr 类型
 * 
 * @return 错误码
 */
int smip_mngr_init(sSmartMeshIP *mngr);

/**
 * @brief 销毁 mngr 模块
 * 
 * @param mngr mngr 句柄
 * 
 * @return 错误码
 */
int smip_mngr_deinit(sSmartMeshIP *mngr);

void smip_mngr_loop(sSmartMeshIP *mngr);

#ifdef __cplusplus
}
#endif

#endif // SMARTMESH_IP_MNGR_H

// EOF
