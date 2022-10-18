
// #define ENABLE_MQTT_TLS
#include <wolfmqtt/mqtt_client.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>


// #include <wolfssl/wolfcrypt/settings.h>
// #include <wolfssl/version.h>

// #include <wolfssl/ssl.h>
// #include <wolfssl/wolfcrypt/asn_public.h>
// #include <wolfssl/wolfcrypt/coding.h>
// #include <wolfssl/wolfcrypt/hmac.h>

#define HAVE_SOCKET

#define INVALID_SOCKET_FD    -1
#define MQTT_MAX_PACKET_SZ   1024
#define MQTT_CON_TIMEOUT_MS  5000

/* Configuration */
#define MQTT_HOST        "119.23.229.112" /* broker.hivemq.com */
#define MQTT_PORT        1883
#define MQTT_USE_TLS     0

#define MQTT_KEEP_ALIVE_SEC  60  //代表客户端发送两次 MQTT 协议包之间的最大间隔时间。
#define MQTT_CLIENT_ID       "WolfMQTTClientSimple"
#define MQTT_USERNAME        "laitao"
#define MQTT_PASSWORD        "123"

#define MQTT_TOPIC_NAME      "testTopic"
#define MQTT_QOS             MQTT_QOS_0

#define MQTT_PUBLISH_MSG     "Test Publish"

#define PRINT_BUFFER_SIZE    80
#define MQTT_CMD_TIMEOUT_MS  30000

// static MqttClient mClient;


// static MqttNet mNetwork;

static int mSockFd = INVALID_SOCKET_FD;


//编码期间使用的传输缓冲区
static byte mSendBuf[MQTT_MAX_PACKET_SZ];
//接收解码期间使用的缓冲区
static byte mReadBuf[MQTT_MAX_PACKET_SZ];

static volatile word16 mPacketIdLast;


static void setup_timeout(struct timeval* tv, int timeout_ms)
{
    tv->tv_sec = timeout_ms / 1000;
    tv->tv_usec = (timeout_ms % 1000) * 1000;

    /* Make sure there is a minimum value specified */
    if (tv->tv_sec < 0 || (tv->tv_sec == 0 && tv->tv_usec <= 0)) {
        tv->tv_sec = 0;
        tv->tv_usec = 100;
    }
}

static int socket_get_error(int sockFd)
{
    int so_error = 0;
    socklen_t len = sizeof(so_error);
    getsockopt(sockFd, SOL_SOCKET, SO_ERROR, &so_error, &len);
    return so_error;
}

static int mqtt_net_connect(void *context, const char* host, word16 port,int timeout_ms)
{
    int rc;
    int sockFd, *pSockFd = (int*)context;
    struct sockaddr_in addr;
    struct addrinfo *result = NULL;
    struct addrinfo hints;

    PRINTF("runing mqtt_net_connect");

    if (pSockFd == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    (void)timeout_ms;

    /* get address */
    XMEMSET(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    XMEMSET(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    rc = getaddrinfo(host, NULL, &hints, &result);
    if (rc >= 0 && result != NULL) {
        struct addrinfo* res = result;

        /* prefer ip4 addresses */
        while (res) {
            if (res->ai_family == AF_INET) {
                result = res;
                break;
            }
            res = res->ai_next;
        }
        if (result->ai_family == AF_INET) {
            addr.sin_port = htons(port);
            addr.sin_family = AF_INET;
            addr.sin_addr =
                ((struct sockaddr_in*)(result->ai_addr))->sin_addr;
        }
        else {
            rc = -1;
        }
        freeaddrinfo(result);
    }
    if (rc < 0) {
        return MQTT_CODE_ERROR_NETWORK;
    }

    sockFd = socket(addr.sin_family, SOCK_STREAM, 0);
    if (sockFd < 0) {
        return MQTT_CODE_ERROR_NETWORK;
    }

    /* Start connect */
    rc = connect(sockFd, (struct sockaddr*)&addr, sizeof(addr));
    if (rc < 0) {
        PRINTF("NetConnect: Error %d (Sock Err %d)",
            rc, socket_get_error(*pSockFd));
        close(sockFd);
        return MQTT_CODE_ERROR_NETWORK;
    }

    /* save socket number to context */
    *pSockFd = sockFd;

    return MQTT_CODE_SUCCESS;
}


static int mqtt_net_read(void *context, byte* buf, int buf_len, int timeout_ms)
{
    int rc;
    int *pSockFd = (int*)context;
    int bytes = 0;
    struct timeval tv;

	PRINTF("runing mqtt_net_read");

    if (pSockFd == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* Setup timeout */
    setup_timeout(&tv, timeout_ms);
    setsockopt(*pSockFd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));

    /* Loop until buf_len has been read, error or timeout */
    while (bytes < buf_len) {
        rc = (int)recv(*pSockFd, &buf[bytes], buf_len - bytes, 0);
        if (rc < 0) {
            rc = socket_get_error(*pSockFd);
            if (rc == 0)
                break; /* timeout */
            PRINTF("NetRead: Error %d", rc);
            return MQTT_CODE_ERROR_NETWORK;
        }
        bytes += rc; /* Data */
    }

    if (bytes == 0) {
        return MQTT_CODE_ERROR_TIMEOUT;
    }

    return bytes;
}


static int mqtt_net_write(void *context, const byte* buf, int buf_len,int timeout_ms)
{
    int rc;
    int *pSockFd = (int*)context;
    struct timeval tv;

	PRINTF("runing mqtt_net_write");

    if (pSockFd == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    /* Setup timeout */
    setup_timeout(&tv, timeout_ms);
    setsockopt(*pSockFd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(tv));

    rc = (int)send(*pSockFd, buf, buf_len, 0);
    if (rc < 0) {
        PRINTF("NetWrite: Error %d (Sock Err %d)",
            rc, socket_get_error(*pSockFd));
        return MQTT_CODE_ERROR_NETWORK;
    }

    return rc;
}


static int mqtt_net_disconnect(void *context)
{
    int *pSockFd = (int*)context;

	PRINTF("runing mqtt_net_disconnect");

    if (pSockFd == NULL) {
        return MQTT_CODE_ERROR_BAD_ARG;
    }

    close(*pSockFd);
    *pSockFd = INVALID_SOCKET_FD;

    return MQTT_CODE_SUCCESS;
}

//消息回调函数
int mqtt_message_cb(MqttClient *client, MqttMessage *msg,byte msg_new, byte msg_done)
{
    byte buf[PRINT_BUFFER_SIZE+1];
    word32 len;

	PRINTF("runing mqtt_message_cb");


    (void)client;

    if (msg_new) {
        /* Determine min size to dump */
        len = msg->topic_name_len;
        if (len > PRINT_BUFFER_SIZE) {
            len = PRINT_BUFFER_SIZE;
        }
        XMEMCPY(buf, msg->topic_name, len);
        buf[len] = '\0'; /* Make sure its null terminated */

        /* Print incoming message */
        PRINTF("MQTT Message: Topic %s, Qos %d, Len %u",
            buf, msg->qos, msg->total_len);
    }

    /* Print message payload */
    len = msg->buffer_len;
    if (len > PRINT_BUFFER_SIZE) {
        len = PRINT_BUFFER_SIZE;
    }
    XMEMCPY(buf, msg->buffer, len);
    buf[len] = '\0'; /* Make sure its null terminated */
    PRINTF("Payload (%d - %d) printing %d bytes:" LINE_END "%s",
        msg->buffer_pos, msg->buffer_pos + msg->buffer_len, len, buf);

    if (msg_done) {
        PRINTF("MQTT Message: Done");
    }

    /* Return negative to terminate publish processing */
    return MQTT_CODE_SUCCESS;
}

/* Use this callback to setup TLS certificates and verify callbacks */
//使用此回调设置TLS证书并验证回调

#ifdef ENABLE_MQTT_TLS
static int mqtt_tls_verify_cb(int preverify, WOLFSSL_X509_STORE_CTX* store)
{
    char buffer[WOLFSSL_MAX_ERROR_SZ];
    PRINTF("MQTT TLS Verify Callback: PreVerify %d, Error %d (%s)",
        preverify, store->error, store->error != 0 ?
            wolfSSL_ERR_error_string(store->error, buffer) : "none");
    PRINTF("  Subject's domain name is %s", store->domain);

    if (store->error != 0) {
        /* Allowing to continue */
        /* Should check certificate and return 0 if not okay */
        PRINTF("  Allowing cert anyways");
    }

    return 1;
}

/* Use this callback to setup TLS certificates and verify callbacks */
static int mqtt_tls_cb(MqttClient* client)
{
    int rc = WOLFSSL_FAILURE;

    /* Use highest available and allow downgrade. If wolfSSL is built with
     * old TLS support, it is possible for a server to force a downgrade to
     * an insecure version. */
    client->tls.ctx = wolfSSL_CTX_new(wolfSSLv23_client_method());
    if (client->tls.ctx) {
        wolfSSL_CTX_set_verify(client->tls.ctx, WOLFSSL_VERIFY_PEER,
                               mqtt_tls_verify_cb);

        /* default to success */
        rc = WOLFSSL_SUCCESS;

    #if !defined(NO_CERT)
    #if 0
        /* Load CA certificate buffer */
        rc = wolfSSL_CTX_load_verify_buffer(client->tls.ctx, caCertBuf,
                                          caCertSize, WOLFSSL_FILETYPE_PEM);
    #endif
    #if 0
        /* If using a client certificate it can be loaded using: */
        rc = wolfSSL_CTX_use_certificate_buffer(client->tls.ctx,
                           clientCertBuf, clientCertSize, WOLFSSL_FILETYPE_PEM);
    #endif
    #endif /* !NO_CERT */
    }

    PRINTF("MQTT TLS Setup (%d)", rc);

    return rc;
}
#else
static int mqtt_tls_cb(MqttClient* client)
{
    (void)client;
    return 0;
}
#endif /* ENABLE_MQTT_TLS */


static word16 mqtt_get_packetid(void)
{

    /* Check rollover */
    if (mPacketIdLast >= MAX_PACKET_ID) {
        mPacketIdLast = 0;
    }
    PRINTF("runing mqtt_get_packetid");
    return ++mPacketIdLast;
}

int mqttsimple_test(void){


MqttClient mClient;
MqttNet mNetwork;

	int rc = 0;
	MqttObject mqttObj;///* Generic MQTT struct for packet types */
	MqttTopic topics[1];
	
	
//	对MqttNet 的对象进行赋值
	XMEMSET(&mNetwork, 0, sizeof(mNetwork));
    mNetwork.connect = mqtt_net_connect;
    mNetwork.read = mqtt_net_read;
    mNetwork.write = mqtt_net_write;
    mNetwork.disconnect = mqtt_net_disconnect;
    mNetwork.context = &mSockFd;           
	
	PRINTF("MQTT Initing");
	//对MqttClient进行初始化
	// 参数：
	// 客户指向 MqttClient 结构的指针（未初始化是可以的）
	// 网指向已使用回调指针和上下文初始化的 MqttNet 结构的指针
	// msg_cb指向消息回调函数的指针
	// tx_buf指向编码期间使用的传输缓冲区的指针
	// tx_buf_len传输缓冲区的最大长度
	// rx_buf用于接收解码期间使用的缓冲区的指针
	// rx_buf_len接收缓冲区的最大长度
	// cmd_timeout_ms最大命令等待超时（以毫秒为单位）
	// 返回：MQTT_CODE_SUCCESS或MQTT_CODE_ERROR_BAD_ARG（参见枚举 MqttPacketResponseCodes）
    rc = MqttClient_Init(&mClient, &mNetwork, mqtt_message_cb,
		mSendBuf, MQTT_MAX_PACKET_SZ, mReadBuf, MQTT_MAX_PACKET_SZ,
		MQTT_CON_TIMEOUT_MS);
    if (rc != MQTT_CODE_SUCCESS) {
        goto exit;
	}
    PRINTF("MQTT Init Success");

	// 使用 TLS 执行网络连接（如果use_tls为非零值）如果use_tls值为非零值，则执行 MqttTlsCb 回调。

	// 参数：

	// 客户指向 MqttClient 结构的指针
	// 主机代理服务器的地址
	// 港口可选的自定义端口。如果为零将使用默认值
	// use_tls如果非零值将连接到并使用 TLS 对数据进行加密
	// 断续器用于配置 SSL 上下文证书检查的函数回调
	// timeout_ms读取超时前的毫秒
	// 返回：MQTT_CODE_SUCCESS或MQTT_CODE_ERROR_*（参见枚举 MqttPacketResponseCodes）
	
    rc = MqttClient_NetConnect(&mClient, MQTT_HOST, MQTT_PORT,
        MQTT_CON_TIMEOUT_MS, MQTT_USE_TLS, mqtt_tls_cb);
    if (rc != MQTT_CODE_SUCCESS) {
        goto exit;
    }
    PRINTF("MQTT Network Connect Success: Host %s, Port %d, UseTLS %d",MQTT_HOST, MQTT_PORT, MQTT_USE_TLS);
	
	/* Send Connect and wait for Ack */
	// 编码并发送 MQTT 连接数据包，并等待连接确认数据包。

	// 参数：

	// 客户指向 MqttClient 结构的指针
	// 连接指向使用连接参数初始化的 MqttConnect 结构的指针
	// 返回：MQTT_CODE_SUCCESS或MQTT_CODE_ERROR_*（参见枚举 MqttPacketResponseCodes）
	//注意：这是一个阻塞函数，将等待 MqttNet.read
    XMEMSET(&mqttObj, 0, sizeof(mqttObj));
    mqttObj.connect.keep_alive_sec = MQTT_KEEP_ALIVE_SEC;
    mqttObj.connect.client_id = MQTT_CLIENT_ID;
    mqttObj.connect.username = MQTT_USERNAME;
    mqttObj.connect.password = MQTT_PASSWORD;
    rc = MqttClient_Connect(&mClient, &mqttObj.connect);
    if (rc != MQTT_CODE_SUCCESS) {
        goto exit;
    }
    PRINTF("MQTT Broker Connect Success: ClientID %s, Username %s, Password %s",MQTT_CLIENT_ID,(MQTT_USERNAME == NULL) ? "Null" : MQTT_USERNAME,(MQTT_PASSWORD == NULL) ? "Null" : MQTT_PASSWORD);
	

    /* Subscribe and wait for Ack */
	// 编码并发送 MQTT-SN 订阅数据包，并等待包含已分配主题 ID 的订阅确认数据包。

	// 参数：

	// 客户指向 MqttClient 结构的指针
	// 订阅指向使用订阅主题列表和所需 QoS 初始化SN_Subscribe结构的指针。
	// 返回：MQTT_CODE_SUCCESS或MQTT_CODE_ERROR_*（参见枚举 MqttPacketResponseCodes）
	// 注意：这是一个阻塞函数，将等待 MqttNet.read
    XMEMSET(&mqttObj, 0, sizeof(mqttObj));
    topics[0].topic_filter = "testTopi";
    topics[0].qos = MQTT_QOS;
    mqttObj.subscribe.packet_id = mqtt_get_packetid();
    mqttObj.subscribe.topic_count = sizeof(topics) / sizeof(MqttTopic);
    mqttObj.subscribe.topics = topics;
    rc = MqttClient_Subscribe(&mClient, &mqttObj.subscribe);
    if (rc != MQTT_CODE_SUCCESS) {
        goto exit;
    }
    PRINTF("MQTT Subscribe Success: Topic %s, QoS %d",
        MQTT_TOPIC_NAME, MQTT_QOS);

	// 编码并发送 MQTT-SN 发布数据包并等待发布响应（如果 QoS > 0）。

	// 参数：

	// 客户指向 MqttClient 结构的指针
	// 发布指向使用消息数据初始化SN_Publish结构的指针 注意：SN_Publish和MqttMessage是相同的结构。
	// 返回：MQTT_CODE_SUCCESS或MQTT_CODE_ERROR_*（参见枚举 MqttPacketResponseCodes）

	// 注意：这是一个阻塞函数，它将等待MqttNet.read如果QoS级别= 1，
	// 则将等待PUBLISH_ACK。如果 QoS 级别 = 2，
	// 则将等待PUBLISH_REC然后发送PUBLISH_REL并等待PUBLISH_COMP。

	XMEMSET(&mqttObj, 0, sizeof(mqttObj));
    mqttObj.publish.qos = MQTT_QOS;
    mqttObj.publish.topic_name = MQTT_TOPIC_NAME;
    mqttObj.publish.packet_id = mqtt_get_packetid();
    mqttObj.publish.buffer = (byte*)MQTT_PUBLISH_MSG;
    mqttObj.publish.total_len = XSTRLEN(MQTT_PUBLISH_MSG);
    rc = MqttClient_Publish(&mClient, &mqttObj.publish);
    if (rc != MQTT_CODE_SUCCESS) {
        goto exit;
    }
    PRINTF("MQTT Publish: Topic %s, Qos %d, Message %s",mqttObj.publish.topic_name, mqttObj.publish.qos, mqttObj.publish.buffer);
	
	
	 // XMEMSET(&mqttObj, 0, sizeof(mqttObj));
    // topics[0].topic_filter = "testTop";
    // topics[0].qos = MQTT_QOS;
    // mqttObj.subscribe.packet_id = mqtt_get_packetid();
    // mqttObj.subscribe.topic_count = sizeof(topics) / sizeof(MqttTopic);
    // mqttObj.subscribe.topics = topics;
    // rc = MqttClient_Subscribe(&mClient, &mqttObj.subscribe);
    // if (rc != MQTT_CODE_SUCCESS) {
        // goto exit;
    // }
    // PRINTF("MQTT Subscribe Success: Topic %s, QoS %d",
        // MQTT_TOPIC_NAME, MQTT_QOS);
		
		
	
    while (1) {
		
		// 等待数据包到达。传入的发布消息将通过MqttClient_Init中提供的回调到达。

		// 参数：

		// 客户指向 MqttClient 结构的指针
		// 味精指向 MqttObject 结构的指针
		// timeout_ms读取超时前的毫秒
		// 返回：MQTT_CODE_SUCCESS或MQTT_CODE_ERROR_*（参见枚举 MqttPacketResponseCodes）

		// 注意：这是一个阻塞函数，将等待 MqttNet.read
//       rc = MqttClient_WaitMessage_ex(&mClient, &mqttObj, MQTT_CMD_TIMEOUT_MS);
		rc = MqttClient_Publish(&mClient, &mqttObj.publish);
		sleep(1);

        if (rc == MQTT_CODE_ERROR_TIMEOUT) {
            /* send keep-alive ping */
			//没有接到消息就发pingreq
            rc = MqttClient_Ping_ex(&mClient, &mqttObj.ping);
            if (rc != MQTT_CODE_SUCCESS) {
                break;
            }
            PRINTF("MQTT Keep-Alive Ping");
        }
        else if (rc != MQTT_CODE_SUCCESS) {
            break;
        }
    }

	
exit:
    if (rc != MQTT_CODE_SUCCESS) {
        PRINTF("MQTT Error %d: %s", rc, MqttClient_ReturnCodeToString(rc));
    }
    return rc;	
}

int main(int argc, char** argv)
{


    int rc = -1;
    (void)argc;
    (void)argv;

    rc = mqttsimple_test();

    return (rc == 0) ? 0 : EXIT_FAILURE;
}