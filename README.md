# AIBill

基于 **Qt Widgets + SQLite + DeepSeek API** 开发的智能记账分析桌面应用，支持微信账单导入、消费统计、AI 财务分析以及 PDF 报告自动生成，帮助用户快速了解个人消费结构与财务状况。

---

## ✨ 功能特性

* 📥 微信/支付宝 CSV 账单导入
* 🏷️ 消费记录自动分类
* ➕ 手动新增账单
* ✏️ 编辑账单信息
* 🗑️ 删除账单
* 🔍 多条件组合搜索
* 🚫 基于交易单号的防重复导入
* 📊 消费占比饼图分析（QtCharts）
* 🏆 消费 TOP 分类排行
* 🤖 DeepSeek AI 消费行为分析
* 📄 AI 财务分析报告自动导出 PDF
* 📈 收支统计与月度消费分析

---

## 🖥️ 技术栈

| 技术                    | 说明         |
| --------------------- | ---------- |
| C++                   | 核心业务逻辑开发   |
| Qt 5.9                | GUI 桌面应用开发 |
| Qt Widgets            | 界面构建       |
| SQLite                | 本地数据存储     |
| QtCharts              | 数据可视化      |
| QNetworkAccessManager | HTTP 网络请求  |
| DeepSeek API          | AI 消费分析能力  |
| Git / GitHub          | 版本管理       |

---

## 🏗️ 项目架构

```text
AIBill
├── UI                # 界面层
├── Service           # 业务逻辑层
├── Database          # 数据访问层
├── Import            # 微信/支付宝账单导入模块
├── AI                # DeepSeek 接入
├── Analysis          # 消费统计分析
└── PdfExporter       # PDF 报告导出
```

---

## 🚀 项目亮点

### 1. 独立完成完整项目开发

从需求分析、数据库设计、UI 开发到功能实现均独立完成。

### 2. 引入 AI 能力提升传统记账体验

通过接入 DeepSeek API，实现消费结构分析、储蓄能力评价、财务风险提示及个性化理财建议。

### 3. 实现账单防重复导入机制

利用微信交易单号建立唯一约束，避免重复导入导致的数据污染。

### 4. 分层架构设计

采用 **UI + Service + Database** 分层设计，降低模块耦合度，提高可维护性与扩展性。

### 5. 自动生成 AI 财务报告

在 AI 分析完成后自动导出 PDF 财务报告，实现数据统计、消费排行与 AI 建议的一体化输出。

---

## 📷 功能展示
<img width="855" height="944" alt="image" src="https://github.com/user-attachments/assets/68e20cd5-92d2-44a5-b8d0-530bf3842c8d" />
<img width="764" height="912" alt="image" src="https://github.com/user-attachments/assets/708db229-1c6f-44d7-b762-ac89795ff18b" />



## 📌 后续优化方向

* [ ] 支持按月/按年统计分析
* [ ] 增加预算管理功能
* [ ] 支持多用户数据隔离
* [ ] 支持云端同步与备份

---

## 📄 License

This project is licensed under the MIT License.



