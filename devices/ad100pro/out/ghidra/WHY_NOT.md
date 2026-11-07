# 为什么 out/ghidra/ 没有真实 Ghidra 反编译产物（WHY_NOT.md）

## 结论（2026-11-07 00:35）

Ghidra **没能跑起来**，而且不是"没下载完"——zip 最终下载完成且大小精确等于期望值，
但 **zip 内容有 42 个成员损坏（含 Ghidra 核心 jar）**；此外这套 Ghidra 12.2 构建
**要求 JDK 25+**（本机只有 JDK 21），且 `analyzeHeadless.bat` 无法处理含中文的
`JAVA_HOME` 路径。三条原因叠加，`out/ghidra/decompile_all.c` 无法生成。
已改用 capstone 伪反编译交付到 `out/decompiled/`（1199 个 .c 文件，见
`out/decompiled/` 与 `ghidra/README.md`）。

## 证据

### 1) 下载过程（570MB，多次失败后最终到齐）

| 时间（本地） | 事件 |
|---|---|
| 22:17–23:26 | 直连 GitHub 8–40KB/s、频繁断连，只到 39MB |
| 23:26–00:13 | 并行分块（gh-proxy.com，36 段）多次中断；merge 出 588,325,846 字节（期望 569,712,708），sha256 `a6c90ed0…` ≠ 期望 `2c2cd776…` |
| 00:15–00:22 | Lead 改用 1MB 分片 + 多镜像（gh-proxy.com / gh.monlor.com）重新下载 |
| 00:22 | 得到 `tools/ghidra_12.2_PUBLIC.zip`，**大小 = 569,712,708（精确）**，但 sha256 = `10887e0b…` 仍与期望不符 |
| 00:27 | `tar -xf` 解压 1.1s 后报错 `ZIP decompression failed (-3)`，只解出约 551MB、缺 `support/` 等目录 |
| 00:29 | 用 `ghidra/extract_salvage.py` 按成员逐个解压：**6781/6823 个成员成功**，42 个失败（损坏区连续），失败的包括 `Ghidra/Framework/Docking/lib/Docking.jar`、`Ghidra/Framework/Generic/lib/Generic.jar`、`Ghidra/Framework/Application/lib/Application.jar`、`Ghidra/Application/lib/Ghidra.jar` 等核心 jar |
| 00:32 | `tools/ghidra_12.2_PUBLIC` 共 5293 个文件 / 858MB（完整发行解压约 1.3GB+），核心 jar 缺失，`analyzeHeadless.bat` 无法工作 |

修复尝试：曾按 chunk 截断（chunk_031/033/035 被重复追加），但该 zip 随后被删除，
且损坏区跨多个成员，无法可靠拼接；`out/ghidra/` 里保留了 `repair_ghidra.ps1` 作为记录。

### 2) JDK 版本也不满足这套 Ghidra

`analyzeHeadless.bat` 实测输出：

```
WARNING: JAVA_HOME environment specifies unsupported java version: ...jdk-21.0.13+9
JDK 25+ (64-bit) could not be found and must be manually chosen!
ERROR: Failed to find a supported JDK.
```

- 这套 Ghidra 12.2（release build `20261016`）要求 **JDK 25+**，而 `tools/jdk` 里是
  Temurin **21.0.13+9**（Ghidra 12.0/12.1 时代要求 21，12.2 此构建已升到 25）。
- 同时 JDK 放在 `tools\jdk\...`（中文路径），`analyzeHeadless.bat` 把
  `JAVA_HOME` 以 ANSI 传给 Java 时出现乱码（日志里 `D:\��ţ\...`），即使版本满足也会失败。
  修复方式：装 JDK 25 到纯 ASCII 路径（如 `C:\jdk25`）。

### 3) 其它已就绪、只差运行环境的东西

- `ghidra/scripts/InitFirmware.java`（preScript：向量表→命名 Thumb 函数+入口点）；
- `ghidra/scripts/ExportAll.java`（postScript：DecompInterface 反编译全部函数→
  functions.tsv / decompiled/<addr>_<name>.c / decompile_all.c / strings.tsv / refs.tsv）；
- `ghidra/run_analysis.ps1`（一键复现，基址已写死 **0x08003000**、入口 0x080030D4）；
- 固件事实已核实：向量表 0x08003000（48 项）、Reset 0x080030D4、main 0x0800CF74。

## 如果之后要补跑（前置条件）

1. 拿到**完整且 sha256 正确**的 Ghidra 12.2 zip（期望
   `2c2cd776db3c1f6a097ba418922378ac3649cadd5e844a6f5f088ae628a7be67`），
   或换成 Ghidra 12.0/12.1（可用 JDK 21）；
2. JDK 放到**纯 ASCII 路径**（如 `C:\jdk25`，Ghidra 12.2 要求 25+），
   在 `run_analysis.ps1` 里改 `-JdkHome`；
3. 运行：`pwsh -File devices\ad100pro\ghidra\run_analysis.ps1`；
4. 产物：`out/ghidra/functions.tsv`、`out/ghidra/decompiled/*.c`、`out/ghidra/decompile_all.c`。

## 备选方案（已交付）

`out/decompiled/`：capstone 反汇编 + 机械翻译的寄存器级伪 C，1199 个文件，
带调用者/被调用者/MMIO 头注释与逐条原始指令注释；附 `index.tsv`、`CALLGRAPH.md`、
`SELFCHECK.md`、`ANNOTATED_KEY_FUNCS.md`、`ISSUES.md`、`FINDINGS.md`。
它**不是** Ghidra 产物，没有表达式重建/类型恢复，但可读、可检索、地址可逐条核对。
