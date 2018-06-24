简单维护过的代码。通过CANBUS及zigbee组成刷卡器网络。刷卡数据传输到中心服务器，应答后再发出响应传给刷卡器。

STC + MG2455[zigbee] + sja1000[canbus]
拓扑结构简述：
1. 服务器通过串口与PC通信【USART & CANBUS】
2. 协调器通过CANBUS与服务器通信，通过zigbee与刷卡器通信【CANBUS & Zigbee】
3. 刷卡器通过zigbee与协调器通信，通过***读卡【Zigbee & ***】

2012-4-18 17:54 目前为止，此版本的错误概率最低