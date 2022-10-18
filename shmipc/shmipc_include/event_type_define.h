#ifndef EVENT_TYPE_DEFINE_H
#define EVENT_TYPE_DEFINE_H

typedef enum _EventCode {
	//-- system event define --//
	EVT_SYS_START                        	= 0x0000,
	EVT_BATTARY_LOW_POWER                	= 0x0001, 
	EVT_RUNNING_LOW_ON_MEMORY            	= 0x0002, // 运行内存不足事件
	EVT_INSUFFICIENT_DISK_SPACE          	= 0x0003, // 磁盘空间不足事件
	EVT_LOW_RESIDUAL_DATA                	= 0x0004, // 剩余流量低事件
	EVT_GPS_ANTENNA_MALFUNCTION          	= 0x0005, // GPS天线故障事件
	EVT_GPS_ANTENNA_RECEIVER_MALFUNCTION 	= 0x0006, // GPS天线接收器故障事件
	EVT_GPS_WEAK_SIGNAL                  	= 0x0007, // GPS信号弱事件
	EVT_HARDWARE_ERROR                   	= 0x0008, 
	EVT_SOFTWARE_ERROR                   	= 0x0009,
	EVT_SYS_WAKEUP                       	= 0x000a,
	EVT_SYS_SLEEP                        	= 0x000b,
	EVT_HARDWARE_CONTROL                 	= 0x000c, // 硬件控制
	EVT_SYS_END                          	= 0x00ff,
	
	//-- camera event define --//
    EVT_CAM_START       					= 0x0100,
	EVT_CAM_MALFUNCTION 					= 0x010E, // 摄像头故障码事件
    EVT_CAM_END         					= 0x01ff,
	
	//-- stream pusher event define --//
	EVT_STREAM_PUSHER_START     			= 0x0200,
	EVT_STREAM_PUSHER_ERROR     			= 0x0210, // 推流模块错误码事件
	EVT_STREAM_PUSHER_END       			= 0x02ff,
	
	//-- recorder event define --//
	EVT_RECORDER_START                      = 0x0300,
	EVT_RECORDER_ERROR          			= 0x0306, // 录像模块错误码事件
	EVT_RECORDER_END            			= 0x03ff,
	
	//-- algo recognize event define --//
	EVT_ALGO_SERVICE_START       			= 0x0400,
	EVT_ALGO_SERVICE_INIT         			= 0x0401, 
	EVT_ALGO_SERVICE_DEINIT       			= 0x0402, 
	EVT_ALGO_SERVICE_ERROR        			= 0x0403, // 算法服务错误码事件
	EVT_ANOMALOUS_DOOR            			= 0x0404, // 箱门异常报警事件
	EVT_ANOMALOUS_PEOPLE_CLOSE_UP 			= 0x0405, // 人员异常靠近报警事件
	EVT_ANOMALOUS_CAR_CLOSE_UP    			= 0x0406, // 车辆异常靠近报警事件
	EVT_CAMERA_OCCLUSION          			= 0x0407, // 摄像头遮挡报警事件
	EVT_DETECT_OBJ_UNMATCH        			= 0x0408, // 异常检测对象报警事件
	EVT_ALGO_RECOGNIZE_END        			= 0x04ff,
	
	//-- lock service event define --//
	EVT_LOCK_SERVICE_START      		   	= 0x0500,
	EVT_LOCK_SERVICE_INIT       		   	= 0x0501,
	EVT_LOCK_SERVICE_DEINIT     		   	= 0x0502,
	EVT_LOCK_SERVICE_CMD 		   			= 0x0503, // 关锁服务命令消息
	EVT_LOCK_SERVICE_CMD_RESP				= 0x0504, // 命令回复
	EVT_LOCK_SERVICE_ERROR      		   	= 0x0505, // 关锁服务错误码事件
	EVT_LOCK_LEVER_OPENED       		   	= 0x0506, // 锁杆打开报警事件
	EVT_LOCK_LEVER_CUTTED       		   	= 0x0507, // 锁杆剪断报警事件
	EVT_SHELL_TEARDOWN                     	= 0x0508, // 设备拆壳报警事件
	EVT_EXCEED_SPEED_LIMIT                 	= 0x0509, // 超速报警事件
	EVT_ABNORMAL_UNLOCKING_IN_SEALED_STATE 	= 0x050A, // 施封状态下的异常开锁事件
	EVT_LOCK_SERVICE_END   				   	= 0x05ff,
	
	//-- update service event define --//
	EVT_UPDATE_SERVICE_START 				= 0x0600,
	EVT_UPDATE_SERVICE_INIT 	  			= 0x0601,
	EVT_UPDATE_SERVICE_DEINIT 	  			= 0x0602,
	EVT_UPDATE_SERVICE_CMD 					= 0x0603,
	EVT_UPDATE_SERVICE_CMD_RESP				= 0x0604,
	EVT_UPDATE_SERVICE_END   	  			= 0x06ff,
	
	//-- log service event define --//
	EVT_LOG_SERVICE_START 	   				= 0x0700,
	EVT_LOG_SERVICE_INIT 	   				= 0x0701,
	EVT_LOG_SERVICE_DEINIT 	   				= 0x0702,
	EVT_LOG_SERVICE_SET_CONFIG 				= 0x0703,
	EVT_LOG_SERIVCE_ERROR 	   				= 0x0704,
	EVT_UPDATE_LOG_FILE        				= 0x0705, // 上传日志文件事件
	EVT_LOG_SERVICE_END 	   				= 0x07ff,
	
	//-- config manager event define --//
	EVT_CONFIG_MANAGER_START  				= 0x0800,
	EVT_CONFIG_MANAGER_INIT   				= 0x0801,
	EVT_CONFIG_MANAGER_DEINIT 				= 0x0802,
	EVT_CONFIG_MANAGER_ERROR  				= 0x0803, // 配置管理错误码事件
	EVT_CONFIG_MANAGER_END    				= 0x08ff,
	
	//-- service manager event define --//
	EVT_SERVICE_MANAGER_START 	 			= 0x0900,
	EVT_SERVICE_MANAGER_INIT  	 			= 0x0901,
	EVT_SERVICE_MANAGER_DEINIT 	 			= 0x0902,
	EVT_SERVICE_MANAGER_ERROR 	 			= 0x0903,
	EVT_SERVICE_MANAGER_CMD 	 			= 0x0904,
	EVT_SERVICE_MANAGER_CMD_RESP 			= 0x0905,
	EVT_SERVICE_MANAGER_END 	 			= 0x09ff,
	
	//-- watch dog event define --//
	EVT_WATCHDOG_SERVICE_START  			= 0x0a00,
	EVT_WATCHDOG_SERVICE_INIT   			= 0x0a01,
	EVT_WATCHDOG_SERVICE_DEINIT 			= 0x0a02,
	EVT_WATCHDOG_SERVICE_ERROR 				= 0x0a03,
	EVT_WATCHDOG_SERVICE_CMD				= 0x0a04, 
	EVT_WATCHDOG_SERVICE_CMD_RESP 			= 0x0a05,
	EVT_PROCESS_GONE           				= 0x0a06, // 某进程已退出
	EVT_WATCHDOG_SERVICE_END   				= 0x0aff
}EventCode;


typedef struct _EventInfo {
	EventCode event_code;
}EventInfo;



#endif