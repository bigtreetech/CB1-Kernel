v1.3    2021.09.08
* Release Note  ：
        1. 文档包指引： 增加changelog、readme文件。
* Hardware      ：
        1. 芯片手册： 更新Datasheet。
        2. 硬件物料清单： 交付外围器件指南。
* Software      :
        1. 软件开发包： 更新XR829 SDK v1.2.8，具体更新查看SDK包的ChangeLog.md。
* Tool          ：
        1. 工具软件指南： 更新SDD工具指南
        2. 工具软件包： 更新BT ETF工具、WLAN ETF工具、SDD工具

V1.2.6

* All           ：
        1. 调整目录结构
* Software      :
        1. 软件开发包： 更新XR829 SDK v1.2.6，具体更新查看SDK包的ChangeLog.md。
* Tool          ：
        1. 工具软件包： 更新BT ETF、WLAN ETF、SDD工具
* Product       ：
        1. 认证指南： 更新BT BQB认证、WLAN WFA认证。

V1.2.5
        1. bt_firmware_v7.12.29 for R-linux no mesh
        A) 修复BLE反复被配对后概率出现连续配对失败问题, 修复iphone11配对兼容性问题。

V1.2.4
        1. HDK docs update
        2. XR829_BT_FW_V7.10.120 android tablet固件更新，提升蓝牙音箱兼容性
        3. wlan etf_v1.4.1
        A) 支持带小数TX功率，不依赖ETF FW版本;
        B) 提供了频偏校准值写入efuse支持，但需要以下版本及以上的软件支持：
                a) wlan_driver_v02.16.91_HT40_01.35
                b) wlan_etf_firmware_vA09.01.0104
                c) wlan_firmware_vC09.08.52.64_DBG_02.103

V1.2.3
        1. 调整目录结构

V1.2.2
        1. wlan etf_v1.4.0
        A) 支持HT40 WLAN ETF发包占空比>99%

V1.2.1
        1. bt_firmware_v7.12.25  TX/RX RF performance Enhanced
        2. wlan_firmware_vC09.08.52.64_DBG_02.100   TX/RX RF performance Enhanced
        3. wlan_etf_firmware_vA09.01.0102 发射功率校准写入efuse
        4. wlan_driver_v02.16.91_HT40_01.35   配合wlan etf_v1.3.9, etf.c/.h修改
        5. wlan etf_v1.3.9  提高WLAN ETF发包占空比，支持发射功率校准
        6. btetf_v1.0.4  支持单载波发送

V1.2.0
        1. XR829 Enhanced support
        2. WLAN ETF:
        A) 支持ETF_CLI(>=v1.3.7)通过命令配置频偏,支持校准发射功率(写efuse,cli待添加)
        B) 支持单载波发送

V1.1.8
        1. BT ETF工具更新，bt testmode更新

V1.1.7
        1. WLAN/BT ETF工具更新

V1.1.3 
        1. 增加CE认证说明

V1.1.2 
        1. BT ETF TOOL说明文档更新

V1.1
        1. BT更新支持4.2

V1.02 
        1. 局部微调
        A) HDK/XR829_SCH_PCB_Checklist_V1.0-CN.pdf
           有源晶振时，XTAL1由悬空改为接GND
        B) TOOLS/WLAN RF TEST 
           修改40M Tx测试channel的配置