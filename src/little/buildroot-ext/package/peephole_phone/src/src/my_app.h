/* Copyright (c) 2023, Canaan Bright Sight Co., Ltd
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef MY_APP_H_
#define MY_APP_H_

#include <unistd.h>
#include <mutex>
#include <string>
#include <atomic>
#include <thread>
#include "comm_rpc_client.h"

typedef enum
{
    INTERCOM_STATUS_INITED,
    INTERCOM_STATUS_SUSPEND,
    INTERCOM_STATUS_RUNNING,
    INTERCOM_STATUS_IDLE
} intercom_status_t;

typedef enum
{
    VOICE_STATUS_ORIGIN,
    VOICE_STATUS_CHANGED,
    VOICE_STATUS_BUTT
} voice_status_t;

class MyApp : public IRpcClientCallback {
  public:
    static MyApp *GetInstance() {
        static MyApp instance;
        return &instance;
    }

    int Init();
    void DeInit();
    int DevPowerOff() {
        if (!rpc_comm_created_) return -1;
        return rpc_client_.DevPowerOff();
    }

    void Run(volatile int &exit_flag, const char *dev_ip, unsigned short port);
    
  private:
    // IRpcClientCallback
    virtual void OnServerInfo(const ServerInfo &info) override;
    virtual void OnEvent(const UserEventData &event) override;

  private:  // emphasize the following members are private
    MyApp() {}
    MyApp(const MyApp &);
    const MyApp &operator=(const MyApp &);
  
  private:
    std::mutex mutex_;
    std::atomic<bool> rpc_comm_created_{false};
    RpcClient rpc_client_;
    std::string dev_ip_;
    unsigned short dev_port_{0};
    std::string url_;
};

//
int intercom_init();
int intercom_deinit();
int intercom_start(const char *url);
int intercom_suspend();
int intercom_resume(const char *url);
int intercom_enable_pitch_shift();
int intercom_disable_pitch_shift();

#endif //MY_APP_H_