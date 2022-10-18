#ifndef _CMD_OP_CODE_H_
#define _CMD_OP_CODE_H_

typedef enum _CommandCode {
	//-- camera CMD define --//
    CMD_CAM_START       					= 0x0100,
	CMD_CAM_OPEN        					= 0x0180, 
    CMD_CAM_OPEN_RESP   					= 0x0181,
	CMD_CAM_CLOSE       					= 0x0182,
    CMD_CAM_CLOSE_RESP  					= 0x0183,
	CMD_CAM_SET_CONFIG  					= 0x0184, // 摄像头设置参数事件
    CMD_CAM_SET_CONFIG_RESP  				= 0x0185,
	CMD_CAM_GET_CONFIG  					= 0x0186,
    CMD_CAM_GET_CONFIG_RESP  				= 0x0187,
    CMD_CAM_STREAM_ON       				= 0x0188, 
    CMD_CAM_STREAM_ON_RESP  				= 0x0189,
    CMD_CAM_STREAM_OFF       				= 0x018a, 
    CMD_CAM_STREAM_OFF_RESP  				= 0x018b,
    CMD_CAM_END         					= 0x01ff,
    
    //-- stream pusher CMD define --//
	CMD_STREAM_PUSHER_START       			= 0x0200,
	//-- encoder parameter define --//
    CMD_ENCODER_SET_VIDEO_QUALITY   		= 0x0280, // 设置视频质量事件
    CMD_ENCODER_SET_VIDEO_QUALITY_RESP		= 0x0281, // 应答设置视频质量事件
	
    CMD_STREAM_PUSHER_OPEN              	= 0x0283,
    CMD_STREAM_PUSHER_OPEN_RESP         	= 0x0284,
    CMD_STREAM_PUSHER_CLOSE             	= 0x0285,
    CMD_STREAM_PUSHER_CLOSE_RESP        	= 0x0286,
	CMD_STREAM_PUSHER_START_PUSHING			= 0x0287, // 开始推流事件
    CMD_STREAM_PUSHER_START_PUSHING_RESP	= 0x0288, // 应答开始推流事件
	CMD_STREAM_PUSHER_STOP_PUSHING 			= 0x0289, // 停止推流事件
    CMD_STREAM_PUSHER_STOP_PUSHING_RESP 	= 0x028a, // 应答停止推流事件
	CMD_STREAM_PUSHER_SWITCH_CHANNEL 		= 0x028b, // 推流模块切换通道事件
    CMD_STREAM_PUSHER_SWITCH_CHANNEL_RESP 	= 0x028c, // 推流模块切换通道事件
	CMD_STREAM_PUSHER_END         			= 0x02ff,
    
    //-- recorder cmd define --//
	CMD_RECORDER_START       				= 0x0300,
	CMD_EVENT_RECORDING 	 				= 0x0380, // 事件录像命令
	CMD_EVENT_RECORDING_RESP 				= 0x0381,
	CMD_STOP_RECORDING  	 				= 0x0382, // 停止录像命令
	CMD_STOP_RECORDING_RESP  				= 0x0383,
	CMD_TIMING_RECORING 	 				= 0x0384, // 定时录像命令
	CMD_TIMING_RECORING_RESP 				= 0x0385,
	CMD_QUERY_RECORD_LIST    				= 0x0386, // 条件查询录像文件信息命令
	CMD_QUERY_RECORD_LIST_RESP 				= 0x0387, 
	CMD_RECORDER_END           				= 0x03ff,
	
	//-- lock service cmd define --//
	CMD_LOCK_SERVICE_START      			= 0x0500,
	CMD_LOCK_SERVICE_SET_CONFIG 			= 0x0580, // 设置关锁服务配置事件
	CMD_LOCK_SERVICE_SET_CONFIG_RESP 		= 0x0581,
	CMD_SEAL                            	= 0x0582, // 施封命令
	CMD_SEAL_RESP							= 0x0583,	
	CMD_UNSEAL                          	= 0x0584, // 解封事件
	CMD_UNSEAL_RESP							= 0x0585,
	CMD_CALL                            	= 0x0586, // 点名事件
	CMD_CALL_RESP							= 0x0587,
	CMD_SET_GPS_UPDATE_FREQUENCY 			= 0x0588, // 设置GPS上传频率命令
	CMD_SET_GPS_UPDATE_FREQUENCY_RESP		= 0x0589,
	CMD_LOCK_SERVICE_END   					= 0x05ff,
	
	//-- update service cmd define --//
	CMD_UPDATE_SERVICE_START 				= 0x0600,
	CMD_UPDATE_SERVICE_SET_CONFIG 			= 0x0680,
	CMD_UPDATE_SERVICE_SET_CONFIG_RESP		= 0x0681, 
	CMD_BACKUP_CURR_VERSION 	  			= 0x0682, // 备份当前版本命令
	CMD_BACKUP_CURR_VERSION_RESP			= 0x0683,
	CMD_RESTORE_PREV_VERSION 	  			= 0x0684, // 恢复前向版本命令
	CMD_RESTORE_PREV_VERSION_RESP			= 0x0685,
	CMD_UPDATE_DOWNLOAD_PACKAGE   			= 0x0686, // 下载安装包命令
	CMD_UPDATE_DOWNLOAD_PACKAGE_RESP		= 0x0687,
	CMD_UPDATE_UNPACK_PACKAGE 	  			= 0x0688, // 解压安装包命令
	CMD_UPDATE_UNPACK_PACKAGE_RESP			= 0x0689,
	CMD_UPDATE_SERVICE_END   	  			= 0x06ff,
	
	//-- log service cmd define --//
	CMD_LOG_SERVICE_START 	   				= 0x0700,
	CMD_LOG_SERVICE_SET_CONFIG 				= 0x0780,
	CMD_LOG_SERVICE_SET_CONFIG_RESP			= 0x0781,
	CMD_UPDATE_LOG_FILE        				= 0x0782, // 上传日志文件命令
	CMD_UPDATE_LOG_FILE_RESP				= 0x0783,
	CMD_LOG_SERVICE_END 	   				= 0x07ff,
	
	//-- service manager cmd define --//
	CMD_SERVICE_MANAGER_START 				= 0x0900,
	CMD_BIND_SERVICE 						= 0x0980,
	CMD_BIND_SERVICE_RESP					= 0x0981,
	CMD_UNBIND_SERVICE 						= 0x0982,
	CMD_UNBIND_SERVICE_RESP					= 0x0983,
	CMD_SERVICE_MANAGER_END 				= 0x09ff
} COMMANDCODE;

typedef struct cmd_info {
    COMMANDCODE   cmd;
}cmd_info_t;

#endif // _CMD_OP_CODE_H_
