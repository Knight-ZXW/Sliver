> APM 相关技术交流，欢迎加我微信 knight-ZXW
## 背景
通过StackVisitor实现堆栈抓取的实现DEMO， 方案细节见: https://blog.csdn.net/ByteDanceTech/article/details/119621240
## API说明

## 兼容性测试
目前只在Google模拟器Android 9~13 64位设备 上测试通过，未进行其他版本、机型测试

## 变更历史
- 修复在相同线程调用suspend导致卡死的问题 (2023-01-03)

## TODO
- 完善兼容性
- API 完善，数据写入文件
