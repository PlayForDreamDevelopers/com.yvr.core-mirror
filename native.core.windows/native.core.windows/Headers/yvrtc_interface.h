#include <string>
namespace yvrtc
{
     enum VideoFrameType
    {
        VideoFrameTypeP = 0,
        VideoFrameTypeI,
        VideoFrameTypeB,
        VideoFrameUnknow
    };

    enum YvrError
    {
        YvrFrameErr = -3,
        YvrPutUdp = -2,
        YvrFail = -1,
        YvrSuccess = 0,
        YvrTcpConnFail= 1,
        YvrTcpDISCONN = 2,
    };

    #pragma pack(1)
    typedef struct YvrTcpHeadInfo
    {
        int32_t uid;
        int32_t frameType;
        int16_t mediaType;
        int16_t codecType;
        int32_t size;
        int64_t timestamp;
        int32_t extraSize;
    }TcpHeadInfo;
#pragma pack()

     //asio tcp
    class YVRTcpServerEngine
    {
    public:
        YVRTcpServerEngine();
        ~YVRTcpServerEngine();
        /**
         * 开启服务
         * @param recever 接收回调函数指针
         * @param connStatus 连接状态回调
         * @param pUser 上层透传指针
         * @param port  绑定端口
         * @return 无
         */
        void StartTcpServer(void (*recever)(const unsigned char *data, int length, void *pUser),void(*connStatus)(int conn, void* pUser),void *pUser, int port);
        /**
         * 停止服务
         * @param 无
         * @return 无
         */
        void StopTcpServer();
        /**
         * 发送数据
         * @param message 发送buffer
         * @param length 发送长度
         * @param type 暂时无用
         * @return <0 失败
         */
        int PutTcpData(const unsigned char *message, int length, unsigned char type = 0);

        bool CheckAdb(bool bFlag,int port);

    private:
        void InitLogger();
        bool AdbDevice(const std::string&cmd);
        void StartAdbReverse();
        void StopAdbReverse();
        void WriteLog(const std::string log);

    private:
        void *m_asio_server;
        bool m_bLog = false;
    };

    class  YVRTcpClientEngine
    {
    public:
        YVRTcpClientEngine();
        ~YVRTcpClientEngine();
        /**
         * 开启服务
         * @param recever 接收回调函数指针
         * @param connStatus 连接状态回调
         * @param pUser 上层透传指针
         * @param ip  连接对端ip
         * @param port 绑定端口
         * @return 无
         */
        void StartTcpClient(void (*recever)(const unsigned char *data, int length, void *pUser),void(*connStatus)(int conn, void* pUser), void *pUser, const std::string &ip, int port);
        /**
         * 停止服务
         * @param 无
         * @return 无
         */
        void StopTcpClient();
        /**
         * 发送数据
         * @param message 发送buffer
         * @param length 发送长度
         * @param type 暂时无用
         * @return <0 失败
         */
        int PutTcpData(const unsigned char *message, int length, unsigned char type = 0);



    private:
        void *m_asio_client;
    };

   
}
