# -*- coding: utf-8 -*-
"""【模板】keypatch 设备补丁模块骨架 —— 复制到 `keypatch/devices/<id>.py` 后填写。

复制方式（在仓库根运行）::

    Copy-Item devices\\_template\\device.py keypatch\\devices\\<id>.py
    # 然后把文件里的 <id> / <设备名> / 固件文件名 / sha256 / 基址全部替换成真实值，
    # 再把 <id> 加入 keypatch/devices/__init__.py 的 DEVICE_IDS。

本模板只依赖标准库 + `keypatch/common.py`；`capstone` / `unicorn` 只在 verify() 内部
延迟导入（这样 studio 后端只 import 标准库也能列出设备）。

接口约定（详见 `keypatch/INTERFACE.md`）：

* `DEVICE`：设备元数据（studio 卡片、CLI list/info、固件 sha256 校验都读它）；
* `apply(data, key)`：**原位**打补丁，输出长度必须与输入完全一致，返回 (patched, patch_log)；
* `verify(patched, key)`（可选但强烈建议）：离线验证，返回 [(ok, 说明), ...]。

写模块之前请先读 `docs/新增设备指南.md` 的「拿固件 → 定基址 → 找同步字函数」三节。
"""
from __future__ import annotations

import os
import sys

# 允许 `import common`（keypatch 目录），与 keypatch.py / 其它设备模块的加载方式一致。
_HERE = os.path.dirname(os.path.abspath(__file__))
_KEYPATCH_DIR = os.path.dirname(_HERE)          # 模板里假设本文件已复制到 keypatch/devices/
if _KEYPATCH_DIR not in sys.path:
    sys.path.insert(0, _KEYPATCH_DIR)

import common  # noqa: E402

# ===========================================================================
# 设备元数据
# ===========================================================================
DEVICE = {
    "id": "<id>",                        # 与文件名一致，如 "v1c"；必须加入 DEVICE_IDS
    "name": "<厂商> <型号>",              # 如 "Godox V1C"
    "kind": "flash",                     # trigger=引闪器 / flash=闪光灯（studio 卡片用）
    "model": "<型号>",                    # 如 "V1C"（studio 显示；缺省时 studio 用 name 兜底）
    "firmware_version": "<版本>",         # 如 "V1.0"（studio 显示；缺省可留空）
    # 原厂固件的仓库相对路径（设备目录的 firmware/ 下；*.bin 被 .gitignore 忽略）
    "image": "devices/<设备目录>/firmware/<原厂固件>.bin",
    "base": 0x08000000,                  # 镜像加载/链接基址（必填，见指南第 3 节）
    "sha256": "<原厂固件 sha256>",        # 必填：防止拿错固件版本（keypatch 会校验）
    "out_name": "<原厂固件名>_key{key:04X}.bin",  # 输出文件名模板（{key} 会被格式化）
    "notes": "一句话说明这个补丁做了什么",
}

# ===========================================================================
# 需要逆向补齐的常量
# ===========================================================================
BASE = DEVICE["base"]                    # 镜像基址：文件偏移 = 虚拟地址 - BASE
IMAGE_SIZE = 0                           # 原厂固件长度（字节），apply 前校验

# 同步字设置函数（把「ID -> 50024/(ID+1)」改写成「同步字 = 私有 KEY」的那段代码）
SYNC_FN = 0x00000000                     # 函数入口虚拟地址
SYNC_FN_SIZE = 0                         # 该函数（或被改写区域）的字节长度
# 原厂该区域的原始字节（apply 前逐字节校验，固件版本不对就抛 ValueError）
ORIG_BYTES = bytes.fromhex("")

# 关键辅助函数地址（按需填；verify 里会替换成记录桩）
WRITE_REG = 0x00000000                   # CC2500_WriteReg(reg=r0, val=r1)
DIV_HELPER = 0x00000000                  # __aeabi_uidiv / 软件除法（视芯片而定）
ID_NUMERATOR = common.ID_NUMERATOR       # 50024，原厂同步字公式分子


def _off(addr: int) -> int:
    """虚拟地址 -> 文件偏移。"""
    return addr - BASE


# ===========================================================================
# 打补丁
# ===========================================================================
def apply(data: bytes, key: int) -> tuple[bytes, list[dict]]:
    """把原厂镜像打上「同步字 = key」补丁，返回 (patched_bytes, patch_log)。

    必须遵守：
    1. **原位补丁**：输出长度与输入完全一致（G3 升级软件最稳）；
    2. 校验长度 + 每处补丁点的原始字节，不匹配立即 `raise ValueError`（防固件版本不对）；
    3. `patch_log` 每项形如：
       `{"addr": 0x0800B75C, "offset": 0x875C, "before": "41 1c", "after": "01 21",
         "desc": "..."}`（before/after 为十六进制字符串，offset 为文件偏移）。
    """
    raise NotImplementedError(
        "模板：尚未实现 apply()。请按 docs/新增设备指南.md 第 4-5 节找到同步字函数后填写。")


# ===========================================================================
# 离线验证（强烈建议实现）
# ===========================================================================
def verify(patched: bytes, key: int) -> list[tuple[bool, str]]:
    """离线验证补丁，返回 [(ok, 说明), ...]（不要抛异常，失败用 (False, 原因) 表达）。

    推荐做法（与 ad100/ad200/x3c 相同）：
    * 静态检查：长度不变、补丁区字节与期望一致、与原厂差异只出现在补丁区；
    * Unicorn 调用级：直接执行补丁后的同步字设置函数，把 WriteReg 等辅助函数替换成
      「记录参数 + `bx lr` 返回」的桩，断言 SYNC1=key>>8、SYNC0=key&0xFF；
    * 原厂镜像自证：同一个 harness 跑原厂镜像应得到 `50024/(ID+1)`。
    """
    raise NotImplementedError(
        "模板：尚未实现 verify()。可先只做静态检查，之后补 Unicorn 调用级验证。")
