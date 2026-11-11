/* ============================================================================
 * 神牛密钥工坊 · Godox Key Studio — 前端应用（零依赖、无构建）
 * 对接 studio/server.py 的 HTTP API，见 studio/API.md。
 * ========================================================================== */
(function () {
  'use strict';

  /* ------------------------------------------------------------------ 工具 */
  var $ = function (id) { return document.getElementById(id); };

  function esc(value) {
    return String(value === undefined || value === null ? '' : value)
      .replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
      .replace(/"/g, '&quot;').replace(/'/g, '&#39;');
  }

  function fmtSize(bytes) {
    if (typeof bytes !== 'number' || !isFinite(bytes)) return '—';
    if (bytes < 1024) return bytes + ' B';
    if (bytes < 1024 * 1024) return (bytes / 1024).toFixed(1) + ' KB';
    return (bytes / 1024 / 1024).toFixed(2) + ' MB';
  }

  function shortSha(sha, head, tail) {
    if (!sha) return '—';
    var h = head === undefined ? 8 : head;
    var t = tail === undefined ? 8 : tail;
    if (sha.length <= h + t + 1) return sha;
    return sha.slice(0, h) + '…' + sha.slice(-t);
  }

  function shortTime(iso) {
    if (!iso) return '—';
    var d = new Date(iso);
    if (isNaN(d.getTime())) return String(iso);
    var p = function (n) { return (n < 10 ? '0' : '') + n; };
    return (d.getMonth() + 1) + '-' + p(d.getDate()) + ' ' + p(d.getHours()) + ':' + p(d.getMinutes());
  }

  function shortDay(iso) {
    if (!iso) return '—';
    var d = new Date(iso);
    if (isNaN(d.getTime())) return String(iso);
    var p = function (n) { return (n < 10 ? '0' : '') + n; };
    return p(d.getMonth() + 1) + '-' + p(d.getDate());
  }

  function shortClock(iso) {
    if (!iso) return '—';
    var d = new Date(iso);
    if (isNaN(d.getTime())) return String(iso);
    var p = function (n) { return (n < 10 ? '0' : '') + n; };
    return p(d.getHours()) + ':' + p(d.getMinutes());
  }

  function fmtDateTime(iso) {
    if (!iso) return '—';
    var d = new Date(iso);
    if (isNaN(d.getTime())) return String(iso);
    var p = function (n) { return (n < 10 ? '0' : '') + n; };
    return d.getFullYear() + '-' + p(d.getMonth() + 1) + '-' + p(d.getDate()) +
      ' ' + p(d.getHours()) + ':' + p(d.getMinutes()) + ':' + p(d.getSeconds());
  }

  function svg(paths, cls) {
    return '<svg class="' + (cls || '') + '" viewBox="0 0 24 24" fill="none" stroke="currentColor" ' +
      'stroke-width="1.8" stroke-linecap="round" stroke-linejoin="round" aria-hidden="true">' + paths + '</svg>';
  }

  var ICON = {
    check: '<path d="m5 12.5 4.6 4.6L19 7.6"/>',
    alert: '<circle cx="12" cy="12" r="9"/><path d="M12 8v4.5M12 16h.01"/>',
    download: '<path d="M12 3.8v10.4"/><path d="m7.6 10.2 4.4 4.4 4.4-4.4"/><path d="M4.5 17.5v1.6A2 2 0 0 0 6.5 21h11a2 2 0 0 0 2-2v-1.6"/>',
    zip: '<path d="M6 3.5h9l3.5 3.5v13.5H6z"/><path d="M12 3.5v4M10 7.5h4M10 11h4M10 14.5h4"/>',
    refresh: '<path d="M20 11.2A8 8 0 1 0 17.8 17"/><path d="M20 4.5v6.7h-6.7"/>',
    upload: '<path d="M12 20.5V10.1"/><path d="m7.6 14.1 4.4-4.4 4.4 4.4"/><path d="M4.5 6.5V4.9a2 2 0 0 1 2-2h11a2 2 0 0 1 2 2v1.6"/>',
    copy: '<rect x="9" y="9" width="11" height="11" rx="2"/><path d="M15 9V6a2 2 0 0 0-2-2H6a2 2 0 0 0-2 2v7a2 2 0 0 0 2 2h3"/>',
    key: '<circle cx="8.4" cy="12" r="3.7"/><path d="M12.1 12h8.5M17.2 12v3.3M20.2 12v2.4"/>',
    device: '<rect x="6" y="6" width="12" height="12" rx="2.5"/><path d="M9 2.6V5M15 2.6V5M9 19v2.4M15 19v2.4M2.6 9H5M2.6 15H5M19 9h2.4M19 15h2.4"/>',
    clock: '<circle cx="12" cy="12" r="9"/><path d="M12 7v5.2l3.2 2"/>',
    flash: '<path d="M13 2.5 4.8 13.4h6.1l-1 8.1 8.3-11.1h-6.1l.9-7.9Z"/>',
    trigger: '<path d="M4.5 5.5a10 10 0 0 0 0 13M19.5 5.5a10 10 0 0 1 0 13"/><path d="M8 8.5a6 6 0 0 0 0 7M16 8.5a6 6 0 0 1 0 7"/><circle cx="12" cy="12" r="1.6"/>',
    book: '<path d="M4.5 5.2A2.7 2.7 0 0 1 7.2 2.5H20v16.2H7.2a2.7 2.7 0 0 0-2.7 2.7V5.2Z"/><path d="M4.5 18.6a2.7 2.7 0 0 1 2.7-2.7H20"/>'
  };

  /* --------------------------------------------------------------- 状态 */
  var state = {
    mock: /(?:^|[?&])mock=1(?:&|$)/.test(window.location.search),
    loading: false,
    devices: [],
    keyRules: null,
    selected: new Set(),
    key: { text: '', hex: '', decimal: null, sync1: '', sync0: '', valid: false, checking: false },
    keySeq: 0,
    health: null,
    generating: false,
    build: null,
    history: [],
    historyExpanded: false,
    mockBuilds: []
  };

  var els = {};

  /* -------------------------------------------------------------- API 层 */
  function apiFetch(path, init) {
    if (state.mock) return mockFetch(path, init);
    return fetch(path, init).then(function (res) {
      return res.text().then(function (text) {
        var data = null;
        try { data = text ? JSON.parse(text) : null; } catch (e) { data = null; }
        if (!res.ok || (data && data.ok === false)) {
          var err = new Error((data && data.error) || ('请求失败（HTTP ' + res.status + '）'));
          err.status = res.status;
          err.data = data;
          throw err;
        }
        return data;
      });
    });
  }

  function apiGet(path) { return apiFetch(path, { method: 'GET' }); }

  function apiPost(path, body) {
    return apiFetch(path, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(body || {})
    });
  }

  /* -------------------------------------------------------------- 键盘 */
  function isTypingTarget(el) {
    if (!el) return false;
    var tag = (el.tagName || '').toLowerCase();
    return tag === 'input' || tag === 'textarea' || tag === 'select' || el.isContentEditable;
  }

  /* ----------------------------------------------------------- 状态灯 */
  function setStatus(kind, text) {
    if (!els.statusChip) return;
    els.statusChip.className = 'status-chip ' + (
      kind === 'online' ? 'is-online' : kind === 'offline' ? 'is-offline' : 'is-loading');
    els.statusText.textContent = text;
  }

  /* ------------------------------------------------------------ Toast */
  function toast(kind, title, message, timeout) {
    var root = els.toastRoot;
    if (!root) return;
    var wrap = document.createElement('div');
    wrap.className = 'toast toast-' + (kind || 'info');
    wrap.setAttribute('data-testid', 'toast');
    var icon = kind === 'success' ? ICON.check : kind === 'error' ? ICON.alert : ICON.upload;
    wrap.innerHTML =
      '<span class="toast-icon">' + svg(icon) + '</span>' +
      '<div class="toast-body"><strong>' + esc(title) + '</strong>' +
      (message ? '<p>' + esc(message) + '</p>' : '') + '</div>' +
      '<button type="button" class="toast-close" aria-label="关闭提示">×</button>';
    wrap.querySelector('.toast-close').addEventListener('click', function () { remove(); });
    root.appendChild(wrap);
    var timer = setTimeout(remove, timeout || 6500);
    function remove() {
      clearTimeout(timer);
      if (!wrap.parentNode) return;
      wrap.classList.add('hide');
      setTimeout(function () { if (wrap.parentNode) wrap.parentNode.removeChild(wrap); }, 220);
    }
  }

  /* ------------------------------------------------------------ Modal */
  function openModal(html) {
    var root = els.modalRoot;
    root.innerHTML = '<div class="modal-backdrop" data-close="1"></div>' +
      '<div class="modal-card" role="dialog" aria-modal="true"><button type="button" class="modal-close" aria-label="关闭">×</button>' +
      html + '</div>';
    root.classList.add('open');
    root.classList.add('show');
    root.querySelector('.modal-close').addEventListener('click', closeModal);
    root.querySelector('.modal-backdrop').addEventListener('click', closeModal);
    var first = root.querySelector('input, button.modal-input, select');
    if (first) setTimeout(function () { first.focus(); }, 60);
  }

  function closeModal() {
    var root = els.modalRoot;
    root.classList.remove('open');
    root.classList.remove('show');
    root.innerHTML = '';
  }

  /* -------------------------------------------------------- 设备渲染 */
  function deviceKindLabel(kind) {
    return kind === 'trigger' ? '引闪器' : '闪光灯';
  }

  function firmwareBadge(dev) {
    var fw = dev.firmware || {};
    if (!fw.present) {
      return '<span class="device-status status-missing">' + svg(ICON.alert, 'dot') + '原厂固件缺失</span>';
    }
    if (!fw.matches) {
      return '<span class="device-status status-mismatch">' + svg(ICON.alert, 'dot') + 'sha256 不匹配</span>';
    }
    return '<span class="device-status status-ok">' + svg(ICON.check, 'dot') + '固件已就位 · sha256 OK</span>';
  }

  /* 设备分组：引闪器在前，闪光灯在后 */
  var DEVICE_GROUPS = [
    { key: 'trigger', label: '引闪器', icon: 'trigger', icoClass: 'ico-trigger', kindClass: 'kind-trigger' },
    { key: 'flash', label: '闪光灯', icon: 'flash', icoClass: 'ico-flash', kindClass: 'kind-flash' }
  ];

  /* 刷入后「密钥开关」对照：来自 keypatch/devices/*.py 的补丁语义 */
  var USAGE_SWITCH = {
    x3c: { on: 'ID 菜单设为 99 → 密钥模式', tip: '密钥帧 + ID99 帧双发；ID 0–98 为原厂单发' },
    x3s: { on: '长按拨轮 / M / C ≈1.5 秒 → 密钥模式', tip: 'RAM 开关：关机 / 换电池后需重新长按' },
    x3pro: { on: '长按 PA6 ≈1.5 秒 → 密钥模式（无屏幕提示）', tip: 'RAM 开关：重启后回到普通模式，需要重新长按' },
    x3pros: { on: 'ID 菜单设为 99 → 密钥模式', tip: '双发兼容普通灯；ID 0–98 为原厂单发' },
    x2ts: { on: 'C.Fn 第 3 项 BEEP → ON', tip: 'OFF / 原厂档会回到 50024/(ID+1)' },
    x2tc: { on: 'MENU → C.Fn → LIGHT（第 4 页）→ ON', tip: '改完约 2 秒自动保存；原厂文件即 G3 容器，直接用 G3 V2.0 刷' },
    ad100: { on: 'C.Fn 第 7 项 KEY → ON', tip: 'ID 菜单恢复原厂含义' },
    ad200: { on: 'C.Fn 第 8 项 F8 → ON', tip: '改值即时生效，无需重启' },
    ad300: { on: 'C.Fn 第 8 项 F8 → ON', tip: '改值即时生效，无需重启' },
    v1c: { on: 'C.Fn 第 3 页 LIGHT → ON（值 2）', tip: 'LIGHT 的 12sec / OFF 仍是原厂模式' },
    v1s: { on: 'C.Fn 第 3 页 LIGHT → ON（值 2）', tip: 'ON 会掉电保存；恢复出厂后回到普通模式' },
    v860iiic: { on: 'C.Fn 第 3 页 LIGHT → ON（值 2）', tip: 'LIGHT 的 12sec / OFF 仍是原厂模式' },
    v860iiis: { on: 'C.Fn 第 3 页 LIGHT → ON（值 2）', tip: 'ON 会掉电保存；恢复出厂后回到普通模式' }
  };
  var USAGE_DEFAULT = { on: '在设备菜单中找到「密钥 / 同步字」开关 → ON', tip: '菜单项名称以该设备实际固件为准' };

  function deviceInGroup(dev, groupKey) {
    return groupKey === 'trigger' ? dev.kind === 'trigger' : dev.kind !== 'trigger';
  }

  function deviceCardHtml(dev) {
    var fw = dev.firmware || {};
    var selected = state.selected.has(dev.id);
    var needUpload = !fw.present || !fw.matches;
    var kindClass = dev.kind === 'trigger' ? 'kind-trigger' : 'kind-flash';
    var icoClass = dev.kind === 'trigger' ? 'ico-trigger' : 'ico-flash';
    var ico = dev.kind === 'trigger' ? ICON.trigger : ICON.flash;
    return '' +
      '<article class="device-card' + (selected ? ' is-selected' : '') + (needUpload ? ' needs-firmware' : '') + '"' +
      ' data-testid="device-card" data-device-id="' + esc(dev.id) + '"' +
      ' role="checkbox" aria-checked="' + (selected ? 'true' : 'false') + '" tabindex="0">' +
        '<label class="device-check" title="选择此设备">' +
          '<input type="checkbox" class="sr-only" data-testid="device-select" data-device-id="' + esc(dev.id) + '"' +
            (selected ? ' checked' : '') + ' aria-label="选择 ' + esc(dev.name) + '">' +
          '<span class="check" aria-hidden="true">' + svg(ICON.check) + '</span>' +
        '</label>' +
        '<div class="device-head">' +
          '<span class="device-kind-ico ' + icoClass + '" aria-hidden="true">' + svg(ico) + '</span>' +
          '<div class="device-id">' +
            '<h3 class="device-model">' + esc(dev.model || dev.name) + '</h3>' +
            '<div class="device-sub">' +
              '<span class="kind-tag ' + kindClass + '">' + deviceKindLabel(dev.kind) + '</span>' +
              (dev.firmware_version ? '<span class="device-ver mono">' + esc(dev.firmware_version) + '</span>' : '') +
            '</div>' +
          '</div>' +
        '</div>' +
        '<p class="device-notes" title="' + esc(dev.notes || dev.name) + '">' + esc(dev.notes || dev.name) + '</p>' +
        '<div class="device-foot">' +
          firmwareBadge(dev) +
          '<span class="device-size">' + esc(fmtSize(fw.size)) + '</span>' +
        '</div>' +
        '<div class="device-meta">' +
          '<span class="sha" title="原厂固件 sha256">sha ' + esc(shortSha(fw.sha256 || dev.original_sha256, 10, 8)) + '</span>' +
          (dev.base ? '<span class="device-base">' + esc(dev.base) + '</span>' : '') +
        '</div>' +
        (needUpload
          ? '<button type="button" class="btn btn-ghost btn-xs device-upload" data-device-id="' + esc(dev.id) + '">' +
            svg(ICON.upload) + '<span>上传原厂固件</span></button>'
          : '') +
      '</article>';
  }

  function bindDeviceCards() {
    Array.prototype.forEach.call(els.devicesGrid.querySelectorAll('[data-testid="device-card"]'), function (card) {
      var id = card.getAttribute('data-device-id');
      card.addEventListener('click', function (ev) {
        if (ev.target.closest('button, label, input, a')) return;
        toggleDevice(id);
      });
      card.addEventListener('keydown', function (ev) {
        if (ev.key === ' ' || ev.key === 'Enter') {
          ev.preventDefault();
          toggleDevice(id);
        }
      });
      var box = card.querySelector('[data-testid="device-select"]');
      if (box) {
        box.addEventListener('change', function () {
          setDeviceSelected(id, box.checked);
        });
      }
      var up = card.querySelector('.device-upload');
      if (up) {
        up.addEventListener('click', function (ev) {
          ev.stopPropagation();
          openUploadModal(deviceById(id));
        });
      }
      // 拖拽上传（缺固件的卡片）
      card.addEventListener('dragover', function (ev) {
        if (!card.classList.contains('needs-firmware')) return;
        ev.preventDefault();
        card.classList.add('drop-target');
      });
      card.addEventListener('dragleave', function () { card.classList.remove('drop-target'); });
      card.addEventListener('drop', function (ev) {
        if (!card.classList.contains('needs-firmware')) return;
        ev.preventDefault();
        card.classList.remove('drop-target');
        var file = ev.dataTransfer && ev.dataTransfer.files && ev.dataTransfer.files[0];
        if (file) uploadFirmware(deviceById(id), file);
      });
    });
  }

  function renderDevices() {
    var grid = els.devicesGrid;
    var empty = els.devicesEmpty;
    if (!state.devices.length) {
      grid.innerHTML = '';
      empty.classList.remove('hidden');
      empty.innerHTML =
        '<span class="empty-icon">' + svg(ICON.device) + '</span>' +
        '<strong>未识别到设备</strong><p>请确认 <code>keypatch/devices/</code> 中有设备模块，然后刷新。</p>';
      updateSelectionUI();
      return;
    }
    empty.classList.add('hidden');
    grid.innerHTML = DEVICE_GROUPS.map(function (group) {
      var list = state.devices.filter(function (dev) { return deviceInGroup(dev, group.key); });
      if (!list.length) return '';
      return '' +
        '<section class="device-group" data-group="' + group.key + '" aria-label="' + group.label + '">' +
          '<header class="device-group-head">' +
            '<span class="device-group-icon ' + group.icoClass + '" aria-hidden="true">' + svg(ICON[group.icon]) + '</span>' +
            '<h3>' + group.label + '</h3>' +
            '<span class="pill">' + list.length + ' 台</span>' +
            '<span class="device-group-pick" data-group-pick="' + group.key + '"></span>' +
          '</header>' +
          '<div class="device-group-grid">' + list.map(deviceCardHtml).join('') + '</div>' +
        '</section>';
    }).join('');
    bindDeviceCards();
    updateSelectionUI();
  }

  function deviceById(id) {
    for (var i = 0; i < state.devices.length; i++) {
      if (state.devices[i].id === id) return state.devices[i];
    }
    return null;
  }

  function toggleDevice(id) { setDeviceSelected(id, !state.selected.has(id)); }

  function setDeviceSelected(id, on) {
    if (on) state.selected.add(id); else state.selected.delete(id);
    var card = document.querySelector('[data-testid="device-card"][data-device-id="' + id + '"]');
    if (card) {
      card.classList.toggle('is-selected', on);
      card.setAttribute('aria-checked', on ? 'true' : 'false');
      var box = card.querySelector('[data-testid="device-select"]');
      if (box && box.checked !== on) box.checked = on;
    }
    updateSelectionUI();
  }

  function updateSelectionUI() {
    var n = state.selected.size;
    if (els.selectedBadge) els.selectedBadge.textContent = '已选 ' + n + ' 台';
    if (els.sumDevices) {
      els.sumDevices.textContent = n ? (Array.from(state.selected).map(function (id) {
        var d = deviceById(id); return d ? (d.model || d.name) : id;
      }).join(' / ')) : '—';
    }
    if (els.btnGenerate) {
      els.btnGenerate.disabled = !(n > 0 && state.key.valid) || state.generating;
    }
    if (els.generateHint) {
      if (!n) els.generateHint.textContent = '请先勾选需要定制的设备。';
      else if (!state.key.valid) els.generateHint.textContent = '请输入合法的 16 位密钥（4 位十六进制）。';
      else els.generateHint.textContent = '将生成 ' + n + ' 个固件文件，可直接下载或打包。';
    }
    if (els.keyTarget) {
      els.keyTarget.textContent = n ? ('将写入 ' + n + ' 台设备') : '尚未选择设备';
      els.keyTarget.classList.toggle('is-ready', n > 0);
    }
    updateGroupPicks();
    updateFlow();
  }

  /* 设备分组头的「已选 x / y」计数 */
  function updateGroupPicks() {
    var nodes = els.devicesGrid ? els.devicesGrid.querySelectorAll('[data-group-pick]') : [];
    Array.prototype.forEach.call(nodes, function (el) {
      var key = el.getAttribute('data-group-pick');
      var total = 0;
      var picked = 0;
      state.devices.forEach(function (dev) {
        if (!deviceInGroup(dev, key)) return;
        total += 1;
        if (state.selected.has(dev.id)) picked += 1;
      });
      el.textContent = '已选 ' + picked + ' / ' + total;
      el.classList.toggle('is-picked', picked > 0);
    });
  }

  /* 顶部流程条：01 选择设备 / 02 设置密钥 / 03 生成固件 */
  function setFlowStep(el, done, active) {
    if (!el) return;
    el.classList.toggle('is-done', !!done);
    el.classList.toggle('is-active', !!active && !done);
  }

  function updateFlow() {
    var n = state.selected.size;
    var keyOk = !!(state.key && state.key.valid);
    var ready = n > 0 && keyOk;
    var built = !!state.build;
    setFlowStep(els.stepDevices, n > 0, n === 0);
    setFlowStep(els.stepKey, keyOk, n > 0 && !keyOk);
    setFlowStep(els.stepBuild, built, ready && !built);
    if (els.stepDevicesNote) els.stepDevicesNote.textContent = n ? ('已选 ' + n + ' 台') : '勾选灯 / 引闪器';
    if (els.stepKeyNote) {
      els.stepKeyNote.textContent = keyOk
        ? ('0x' + String(state.key.hex || '').replace(/^0x/i, '').toUpperCase())
        : '4 位十六进制';
    }
    if (els.stepBuildNote) {
      els.stepBuildNote.textContent = built ? '已生成，可下载' : (ready ? '可以生成了' : '下载并刷写');
    }
  }

  /* ------------------------------------------------------------ 密钥 */
  function normalizeKeyText(raw) {
    return String(raw || '').toUpperCase().replace(/[^0-9A-F]/g, '').slice(0, 4);
  }

  function colorKeyState(kind) {
    var field = els.keyField;
    if (!field) return;
    field.classList.remove('is-valid', 'is-invalid', 'is-checking', 'has-key');
    if (kind) field.classList.add(kind);
    if (state.key.text) field.classList.add('has-key');
  }

  function setKeyStatus(icon, text, kind) {
    if (els.keyStatusIcon) {
      els.keyStatusIcon.innerHTML = '<span class="status-dot"></span>';
    }
    if (els.keyStatusText) els.keyStatusText.textContent = text;
    colorKeyState(kind);
  }

  function setKeyStats(hex, decimal, sync1, sync0) {
    if (els.statSync1) els.statSync1.textContent = sync1 || '—';
    if (els.statSync0) els.statSync0.textContent = sync0 || '—';
    if (els.statDecimal) els.statDecimal.textContent = (decimal === null || decimal === undefined) ? '—' : String(decimal);
    if (els.sumKey) els.sumKey.textContent = hex || '—';
  }

  function clearKey() {
    state.key = { text: '', hex: '', decimal: null, sync1: '', sync0: '', valid: false, checking: false };
    setKeyStats('', null, '', '');
    showSuggest('');
    updateSelectionUI();
  }

  function showSuggest(suggest) {
    var box = els.keySuggest;
    if (!box) return;
    if (!suggest) { box.classList.add('hidden'); box.innerHTML = ''; return; }
    box.classList.remove('hidden');
    box.innerHTML = '建议改用 <b class="mono">' + esc(suggest) + '</b> ' +
      '<button type="button" class="btn btn-soft btn-xs" id="suggest-apply">应用</button>';
    var btn = box.querySelector('#suggest-apply');
    if (btn) btn.addEventListener('click', function () {
      setKeyInputValue(suggest.replace(/^0x/i, ''));
      validateKey();
    });
  }

  function setKeyInputValue(text) {
    if (!els.keyInput) return;
    els.keyInput.value = text;
    state.key.text = text;
    colorKeyState(state.key.text ? '' : null);
  }

  var keyTimer = null;

  function onKeyInput() {
    var text = normalizeKeyText(els.keyInput.value);
    if (els.keyInput.value !== text) els.keyInput.value = text;
    state.key.text = text;
    showSuggest('');
    if (!text) { clearKey(); setKeyStatus(null, '输入 4 位十六进制密钥（0-9、A-F）'); return; }
    if (text.length < 4) {
      state.key.valid = false;
      setKeyStats('', null, '', '');
      setKeyStatus(null, '还需 ' + (4 - text.length) + ' 位十六进制字符', 'is-checking');
      updateSelectionUI();
      return;
    }
    // 本地快速校验（规则来自 /api/devices.key_rules），再请求后端确认
    var local = localValidateKey(text);
    if (local.error) {
      state.key.valid = false;
      setKeyStats('', null, '', '');
      setKeyStatus('error', local.error, 'is-invalid');
      if (local.suggest) showSuggest(local.suggest);
      updateSelectionUI();
      return;
    }
    state.key.valid = false;
    setKeyStatus(null, '正在校验密钥…', 'is-checking');
    clearTimeout(keyTimer);
    keyTimer = setTimeout(function () { validateKey(); }, 220);
  }

  function localValidateKey(text) {
    if (!/^[0-9A-F]{4}$/.test(text)) return { error: '密钥必须是 4 位十六进制（0-9、A-F）' };
    var value = parseInt(text, 16);
    var rules = state.keyRules || {};
    var forbidden = (rules.forbidden || []).map(function (x) { return String(x).toUpperCase(); });
    if (forbidden.indexOf('0X' + text) >= 0) {
      return { error: '0x' + text + ' 是保留/危险同步字', suggest: suggestFor(text) };
    }
    var publicValues = (rules.public_sync_values || []).map(function (x) { return String(x).toUpperCase(); });
    if (publicValues.indexOf('0X' + text) >= 0) {
      return { error: '0x' + text + ' 与原厂 ID 同步字冲突，会破坏隔离性', suggest: suggestFor(text) };
    }
    return { value: value };
  }

  function suggestFor(text) {
    var value = parseInt(text, 16);
    if (!isFinite(value)) value = 0xD798;
    var rules = state.keyRules || {};
    var banned = {};
    (rules.forbidden || []).forEach(function (x) { banned[String(x).toUpperCase()] = 1; });
    (rules.public_sync_values || []).forEach(function (x) { banned[String(x).toUpperCase()] = 1; });
    for (var i = 1; i <= 8192; i++) {
      var cand = (value + i) & 0xFFFF;
      var hex = ('000' + cand.toString(16).toUpperCase()).slice(-4);
      if (!banned['0X' + hex]) return '0x' + hex;
    }
    return '0xD798';
  }

  function validateKey() {
    var text = state.key.text;
    if (text.length !== 4) return Promise.resolve();
    var seq = ++state.keySeq;
    state.key.checking = true;
    return apiPost('/api/key/validate', { text: text }).then(function (data) {
      if (seq !== state.keySeq) return;
      state.key.checking = false;
      state.key.valid = true;
      state.key.hex = data.key.hex;
      state.key.decimal = data.key.decimal;
      state.key.sync1 = data.key.sync1;
      state.key.sync0 = data.key.sync0;
      setKeyStats(data.key.hex, data.key.decimal, data.key.sync1, data.key.sync0);
      setKeyStatus('ok', '密钥合法 · 灯与引闪器必须刷入同一把', 'is-valid');
      showSuggest('');
      updateSelectionUI();
    }).catch(function (err) {
      if (seq !== state.keySeq) return;
      state.key.checking = false;
      state.key.valid = false;
      setKeyStats('', null, '', '');
      setKeyStatus('error', err.message || '密钥不合法', 'is-invalid');
      if (err.data && err.data.suggest) showSuggest(err.data.suggest);
      updateSelectionUI();
    });
  }

  function randomKey() {
    if (!els.btnRandom) return;
    els.btnRandom.disabled = true;
    apiPost('/api/key/random', {}).then(function (data) {
      setKeyInputValue(data.key.hex.replace(/^0x/i, ''));
      state.key.valid = true;
      state.key.hex = data.key.hex;
      state.key.decimal = data.key.decimal;
      state.key.sync1 = data.key.sync1;
      state.key.sync0 = data.key.sync0;
      setKeyStats(data.key.hex, data.key.decimal, data.key.sync1, data.key.sync0);
      setKeyStatus('ok', '已生成随机密钥 · 请自行保存', 'is-valid');
      updateSelectionUI();
      toast('success', '随机密钥已生成', data.key.hex + '（SYNC1=' + data.key.sync1 + '，SYNC0=' + data.key.sync0 + '）');
    }).catch(function (err) {
      toast('error', '生成随机密钥失败', err.message);
    }).then(function () { els.btnRandom.disabled = false; });
  }

  function openDeriveModal() {
    openModal(
      '<div class="modal-head"><h3>口令派生密钥</h3><p>同一口令总是派生出同一把密钥，方便在多台设备上复现。</p></div>' +
      '<div class="modal-body">' +
        '<label class="modal-label" for="derive-input">口令 / 短语</label>' +
        '<input class="modal-input" id="derive-input" type="text" placeholder="例如：我的引闪器 2026" autocomplete="off">' +
        '<p class="modal-error hidden" id="derive-error"></p>' +
      '</div>' +
      '<div class="modal-actions"><button type="button" class="btn btn-ghost" data-close-modal>取消</button>' +
        '<button type="button" class="btn btn-primary" id="derive-submit">派生密钥</button></div>'
    );
    var root = els.modalRoot;
    root.querySelector('[data-close-modal]').addEventListener('click', closeModal);
    var input = root.querySelector('#derive-input');
    var errorEl = root.querySelector('#derive-error');
    var submit = root.querySelector('#derive-submit');
    function run() {
      var pass = (input.value || '').trim();
      if (!pass) { errorEl.textContent = '口令不能为空'; errorEl.classList.remove('hidden'); return; }
      submit.disabled = true;
      apiPost('/api/key/derive', { passphrase: pass }).then(function (data) {
        closeModal();
        setKeyInputValue(data.key.hex.replace(/^0x/i, ''));
        state.key.valid = true;
        state.key.hex = data.key.hex;
        state.key.decimal = data.key.decimal;
        state.key.sync1 = data.key.sync1;
        state.key.sync0 = data.key.sync0;
        setKeyStats(data.key.hex, data.key.decimal, data.key.sync1, data.key.sync0);
        setKeyStatus('ok', '由口令派生 · ' + data.key.hex, 'is-valid');
        updateSelectionUI();
      }).catch(function (err) {
        errorEl.textContent = err.message || '派生失败';
        errorEl.classList.remove('hidden');
      }).then(function () { submit.disabled = false; });
    }
    submit.addEventListener('click', run);
    input.addEventListener('keydown', function (ev) { if (ev.key === 'Enter') run(); });
  }

  /* ------------------------------------------------------ 密钥规则面板 */
  function renderKeyRules(rules) {
    if (!rules) return;
    if (els.rulesNote && rules.note) els.rulesNote.textContent = rules.note;
    if (els.forbiddenChips) {
      els.forbiddenChips.innerHTML = (rules.forbidden || []).map(function (v) {
        return '<span class="chip chip-danger mono">' + esc(v) + '</span>';
      }).join('');
    }
    if (els.publicChips) {
      els.publicChips.innerHTML = (rules.public_sync_values || []).map(function (v) {
        return '<span class="chip mono">' + esc(v) + '</span>';
      }).join('');
    }
  }

  /* ----------------------------------------------------------- 固件上传 */
  function openUploadModal(dev) {
    if (!dev) return;
    openModal(
      '<div class="modal-head"><h3>上传 ' + esc(dev.model || dev.name) + ' 原厂固件</h3>' +
        '<p>文件会被校验 sha256（只接受原厂版本），保存在 <code>' + esc((dev.firmware || {}).path || '') + '</code>。</p></div>' +
      '<div class="modal-body">' +
        '<div class="drop-hint" id="upload-drop"><span>' + svg(ICON.upload) + '</span>' +
          '<strong>点击选择或拖拽 .bin 文件到这里</strong></div>' +
        '<input type="file" id="upload-file" accept=".bin,application/octet-stream" class="sr-only">' +
        '<p class="modal-error hidden" id="upload-error"></p>' +
      '</div>' +
      '<div class="modal-actions"><button type="button" class="btn btn-ghost" data-close-modal>取消</button></div>'
    );
    var root = els.modalRoot;
    root.querySelector('[data-close-modal]').addEventListener('click', closeModal);
    var fileInput = root.querySelector('#upload-file');
    var drop = root.querySelector('#upload-drop');
    var errorEl = root.querySelector('#upload-error');
    drop.addEventListener('click', function () { fileInput.click(); });
    fileInput.addEventListener('change', function () {
      if (fileInput.files && fileInput.files[0]) uploadFirmware(dev, fileInput.files[0]);
    });
    drop.addEventListener('dragover', function (ev) { ev.preventDefault(); drop.classList.add('drop-target'); });
    drop.addEventListener('dragleave', function () { drop.classList.remove('drop-target'); });
    drop.addEventListener('drop', function (ev) {
      ev.preventDefault();
      var file = ev.dataTransfer.files && ev.dataTransfer.files[0];
      if (file) uploadFirmware(dev, file);
    });
    function showError(msg) { errorEl.textContent = msg; errorEl.classList.remove('hidden'); }
    // 供 uploadFirmware 使用
    dev.__showUploadError = showError;
  }

  function uploadFirmware(dev, file) {
    if (!dev) return;
    if (!/\.bin$/i.test(file.name)) {
      if (dev.__showUploadError) dev.__showUploadError('请选择 .bin 原厂固件文件');
      else toast('error', '文件类型不正确', '请选择 .bin 原厂固件');
      return;
    }
    var reader = new FileReader();
    reader.onload = function () {
      apiFetch('/api/firmware/upload?device=' + encodeURIComponent(dev.id), {
        method: 'POST',
        headers: { 'Content-Type': 'application/octet-stream', 'X-Filename': file.name },
        body: reader.result
      }).then(function () {
        closeModal();
        toast('success', '固件上传成功', (dev.model || dev.id) + ' 已就位，可以生成固件了。');
        loadDevices();
      }).catch(function (err) {
        var msg = err.message || '上传失败';
        if (err.data && err.data.expected) {
          msg += '\n期望 sha256：' + err.data.expected + '\n实际 sha256：' + err.data.actual;
        }
        if (dev.__showUploadError) dev.__showUploadError(msg);
        else toast('error', '固件上传失败', msg);
      });
    };
    reader.onerror = function () {
      toast('error', '读取文件失败', file.name);
    };
    reader.readAsArrayBuffer(file);
  }

  /* ------------------------------------------------------------- 生成 */
  function setGenerating(on) {
    state.generating = on;
    if (els.generateSpinner) els.generateSpinner.classList.toggle('hidden', !on);
    if (els.btnGenerate) {
      els.btnGenerate.disabled = on || !(state.selected.size > 0 && state.key.valid);
      els.btnGenerate.classList.toggle('spinning', on);
    }
    if (els.generateLabel) els.generateLabel.textContent = on ? '正在生成…' : '生成并导出';
  }

  function renderProgress() {
    if (!els.panelResult) return;
    els.panelResult.classList.remove('hidden');
    if (els.resultPill) els.resultPill.textContent = '生成中';
    if (els.resultActions) els.resultActions.innerHTML = '';
    if (els.resultRoot) {
      els.resultRoot.innerHTML =
        '<div class="progress-card">' +
          '<div class="progress-head">' +
            '<span class="spinner-lg spinning" aria-hidden="true"></span>' +
            '<div><h3>正在生成固件…</h3><p class="progress-note">补丁、打包与离线验证在服务端完成，请稍候。</p></div>' +
          '</div>' +
          '<ul class="progress-steps" id="progress-steps">' +
            '<li class="progress-step is-done" data-step="0"><span class="step-dot"></span>校验密钥</li>' +
            '<li class="progress-step" data-step="1"><span class="step-dot"></span>读取原厂固件</li>' +
            '<li class="progress-step" data-step="2"><span class="step-dot"></span>原位打补丁</li>' +
            '<li class="progress-step" data-step="3"><span class="step-dot"></span>验证并导出</li>' +
          '</ul>' +
        '</div>';
      var steps = els.resultRoot.querySelectorAll('.progress-step');
      var i = 0;
      var timer = setInterval(function () {
        i += 1;
        if (i >= steps.length || !document.body.contains(steps[0])) { clearInterval(timer); return; }
        steps[i].classList.add('is-done');
      }, 900);
      state._progressTimer = timer;
    }
  }

  function generate() {
    if (state.generating) return;
    if (!state.selected.size) { toast('warning', '还没有选择设备', '请至少勾选一台设备。'); return; }
    if (!state.key.valid) { toast('warning', '密钥不合法', '请输入合法的 4 位十六进制密钥。'); return; }
    setGenerating(true);
    renderProgress();
    var body = {
      key: state.key.hex,
      devices: Array.from(state.selected),
      verify: els.optVerify ? els.optVerify.checked : true,
      container: els.optContainer ? els.optContainer.checked : false
    };
    apiPost('/api/generate', body).then(function (data) {
      if (state._progressTimer) clearInterval(state._progressTimer);
      state.build = data.build;
      renderBuild(data.build);
      loadHistory();
      toast('success', '固件生成完成', data.build.files.length + ' 个文件，点击卡片右侧下载。');
    }).catch(function (err) {
      if (state._progressTimer) clearInterval(state._progressTimer);
      renderGenerateError(err);
      toast('error', '生成失败', err.message);
    }).then(function () {
      setGenerating(false);
    });
  }

  function renderGenerateError(err) {
    if (!els.panelResult) return;
    els.panelResult.classList.remove('hidden');
    if (els.resultPill) els.resultPill.textContent = '失败';
    if (els.resultActions) els.resultActions.innerHTML = '';
    if (els.resultRoot) {
      els.resultRoot.innerHTML =
        '<div class="result-error">' +
          '<span class="result-error-icon">' + svg(ICON.alert) + '</span>' +
          '<div><h3>生成失败</h3><p>' + esc(err.message || '未知错误') + '</p>' +
            '<div class="result-error-actions"><button type="button" class="btn btn-soft btn-sm" id="retry-generate">重试</button></div>' +
          '</div>' +
        '</div>';
      var retry = els.resultRoot.querySelector('#retry-generate');
      if (retry) retry.addEventListener('click', generate);
    }
  }

  function patchLogTable(log) {
    if (!log || !log.length) return '<p class="detail-count">无补丁点记录</p>';
    return '<div class="patch-table-wrap"><table class="patch-table"><thead><tr>' +
      '<th>地址</th><th>偏移</th><th>原字节</th><th>新字节</th><th>说明</th></tr></thead><tbody>' +
      log.map(function (p) {
        return '<tr><td>' + esc(p.addr || '') + '</td>' +
          '<td>' + (typeof p.offset === 'number' ? '0x' + p.offset.toString(16).toUpperCase() : '') + '</td>' +
          '<td>' + esc(p.before || '') + '</td><td>' + esc(p.after || '') + '</td>' +
          '<td>' + esc(p.desc || '') + '</td></tr>';
      }).join('') + '</tbody></table></div>';
  }

  function verifyList(results) {
    if (!results || !results.length) return '<p class="detail-count">未执行离线验证</p>';
    return '<ul class="verify-list">' + results.map(function (r) {
      return '<li class="verify-item"><span class="verify-badge ' + (r.ok ? 'pass' : 'fail') + '">' +
        (r.ok ? 'PASS' : 'FAIL') + '</span><span>' + esc(r.msg || '') + '</span></li>';
    }).join('') + '</ul>';
  }

  /* 刷入后使用教程：通用步骤 + 按本次生成设备生成的密钥开关对照表 */
  function usageDeviceCard(file) {
    var dev = deviceById(file.device);
    var name = (dev && (dev.model || dev.name)) || file.name || file.device;
    var isTrigger = !!(dev && dev.kind === 'trigger');
    var usage = USAGE_SWITCH[String(file.device || '').toLowerCase()] || USAGE_DEFAULT;
    return '' +
      '<div class="usage-device">' +
        '<div class="usage-device-head">' +
          '<span class="usage-device-name">' + esc(name) + '</span>' +
          '<span class="kind-tag ' + (isTrigger ? 'kind-trigger' : 'kind-flash') + '">' +
            (isTrigger ? '引闪器' : '闪光灯') + '</span>' +
        '</div>' +
        '<span class="usage-device-switch">' + esc(usage.on) + '</span>' +
        (usage.tip ? '<span class="usage-device-tip">' + esc(usage.tip) + '</span>' : '') +
      '</div>';
  }

  function renderUsageGuide(build) {
    var files = (build.files || []).slice();
    if (!files.length) return '';
    // 引闪器在前、闪光灯在后，与设备选择区顺序一致
    var rank = function (file) {
      var dev = deviceById(file.device);
      return dev && dev.kind === 'trigger' ? 0 : 1;
    };
    files.sort(function (a, b) { return rank(a) - rank(b); });
    return '' +
      '<section class="usage-guide" aria-labelledby="usage-title">' +
        '<header class="usage-head">' +
          '<span class="usage-icon" aria-hidden="true">' + svg(ICON.book) + '</span>' +
          '<div class="usage-heading">' +
            '<h3 id="usage-title">刷入后怎么用</h3>' +
            '<p>本次生成的 ' + files.length + ' 台设备刷入后，按下面步骤开启密钥模式并验证。</p>' +
          '</div>' +
        '</header>' +
        '<ol class="usage-steps">' +
          '<li><span class="usage-no" aria-hidden="true">1</span><div>' +
            '<b>用官方 G3 升级软件刷入</b>' +
            '<p>先保留同版本原厂固件用于回滚；刷完确认设备能正常开机、版本号正确。</p></div></li>' +
          '<li><span class="usage-no" aria-hidden="true">2</span><div>' +
            '<b>逐台开启密钥模式</b>' +
            '<p>按下方对照表开启（C.Fn 项 / ID=99 / 长按按键）—— 灯与引闪器必须使用同一把密钥。</p></div></li>' +
          '<li><span class="usage-no" aria-hidden="true">3</span><div>' +
            '<b>对齐频道并实拍验证</b>' +
            '<p>引闪器与灯设为同一 CH，按快门确认闪光；未刷写的原厂灯不应被这把密钥引闪。</p></div></li>' +
          '<li><span class="usage-no" aria-hidden="true">4</span><div>' +
            '<b>不同步时的排查顺序</b>' +
            '<p>核对开关页数与值 → X3-S / X3 Pro 的开关是 RAM 版，重启后需重新长按 → 若 C.Fn 被「设置同步」覆盖，重新设为 ON。</p></div></li>' +
          '<li><span class="usage-no" aria-hidden="true">5</span><div>' +
            '<b>回滚到原厂</b>' +
            '<p>刷回原厂固件，并把掉电保存的 C.Fn 开关改回原值，即可恢复普通模式。</p></div></li>' +
        '</ol>' +
        '<div class="usage-switch">' +
          '<span class="usage-label">各设备密钥开关 · 刷入后开启</span>' +
          '<div class="usage-devices">' + files.map(usageDeviceCard).join('') + '</div>' +
        '</div>' +
        '<p class="usage-note">' + svg(ICON.alert, 'usage-note-icon') +
          '<span>本教程由设备模块的补丁语义自动生成；固件版本不同时菜单项位置可能略有差异，以设备实际菜单为准。</span></p>' +
      '</section>';
  }

  function renderBuild(build) {
    if (!els.panelResult) return;
    els.panelResult.classList.remove('hidden');
    var verifyAll = [];
    build.files.forEach(function (f) {
      (f.verify_results || []).forEach(function (r) { verifyAll.push(r.ok); });
    });
    var verifyOk = verifyAll.length > 0 && verifyAll.every(Boolean);
    if (els.resultPill) {
      els.resultPill.textContent = build.files.length + ' 个文件';
      els.resultPill.className = 'pill ' + (verifyOk ? 'pill-amber' : '');
    }
    if (els.resultActions) {
      els.resultActions.innerHTML = build.zip
        ? '<a class="btn btn-primary btn-sm" href="' + esc(build.zip) + '" download>' +
          svg(ICON.zip) + '<span>下载全部 ZIP</span></a>'
        : '';
    }
    var keyText = build.key && build.key.hex ? build.key.hex : '';
    var filesHtml = build.files.map(function (f) {
      var badge = f.verify_results && f.verify_results.length
        ? (f.verify_results.every(function (r) { return r.ok; })
          ? '<span class="verify-badge pass">PASS</span>'
          : '<span class="verify-badge fail">FAIL</span>')
        : '';
      return '' +
        '<div class="file-card" data-testid="result-file" data-device-id="' + esc(f.device) + '">' +
          '<div class="file-card-head">' +
            '<div class="file-name-wrap">' +
              '<div class="file-kind">' + esc(f.name || f.device) + '</div>' +
              '<div class="file-name mono">' + esc(f.filename) + '</div>' +
            '</div>' +
            '<div class="file-actions">' +
              '<a class="btn btn-soft btn-sm" data-testid="download-link" href="' + esc(f.download || '#') + '"' +
                ' download title="下载固件">' + svg(ICON.download) + '<span>下载固件</span></a>' +
            '</div>' +
          '</div>' +
          '<div class="file-meta">' +
            '<span class="file-meta-item">大小 <b class="mono">' + esc(fmtSize(f.size)) + '</b></span>' +
            '<span class="file-meta-item">补丁点 <b class="mono">' + esc(f.patch_count || 0) + '</b></span>' +
            '<span class="file-meta-item">验证 ' + badge + '</span>' +
          '</div>' +
          '<div class="file-sha">' +
            '<span class="sha-label">sha256</span><code class="mono">' + esc(f.sha256 || '') + '</code>' +
            '<button type="button" class="copy-btn" data-copy="' + esc(f.sha256 || '') + '">' +
              svg(ICON.copy) + '<span>复制</span></button>' +
          '</div>' +
          '<details class="detail"><summary>补丁点明细（' + esc(f.patch_count || 0) + '）</summary>' +
            patchLogTable(f.patch_log) + '</details>' +
          '<details class="detail"><summary>离线验证（' +
            ((f.verify_results || []).filter(function (r) { return r.ok; }).length) + '/' +
            (f.verify_results || []).length + '）</summary>' + verifyList(f.verify_results) + '</details>' +
          (f.warnings && f.warnings.length
            ? '<ul class="warn-list">' + f.warnings.map(function (w) {
              return '<li>' + esc(w) + '</li>';
            }).join('') + '</ul>'
            : '') +
          (f.extra_files && f.extra_files.length
            ? '<div class="file-meta"><span class="file-meta-item">额外文件：' +
              f.extra_files.map(function (n) {
                return '<a class="file-chip mono" href="/api/builds/' + encodeURIComponent(build.id) + '/' +
                  encodeURIComponent(n) + '" download>' + esc(n) + '</a>';
              }).join(' ') + '</span></div>'
            : '') +
        '</div>';
    }).join('');

    els.resultRoot.innerHTML =
      '<div class="build-head">' +
        '<div class="build-head-main">' +
          '<div class="build-title-row"><h3>生成完成</h3><span class="pill mono">' + esc(build.id) + '</span></div>' +
          '<p class="build-meta">密钥 <b class="mono">' + esc(keyText) + '</b> · ' +
            esc(build.files.length) + ' 个文件 · <span class="build-time">' + esc(fmtDateTime(build.created)) + '</span></p>' +
        '</div>' +
      '</div>' +
      '<div class="file-list">' + filesHtml + '</div>' +
      renderUsageGuide(build);

    Array.prototype.forEach.call(els.resultRoot.querySelectorAll('.copy-btn'), function (btn) {
      btn.addEventListener('click', function () {
        copyText(btn.getAttribute('data-copy') || '', btn);
      });
    });
    // 演示模式下拦截下载
    if (state.mock) {
      Array.prototype.forEach.call(els.resultRoot.querySelectorAll('a[download]'), function (a) {
        a.addEventListener('click', function (ev) {
          ev.preventDefault();
          toast('info', '演示模式', '启动后端服务后即可下载真实固件。');
        });
      });
    }
    updateFlow();
    els.panelResult.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
  }

  function copyText(text, btn) {
    function done() {
      var old = btn.innerHTML;
      btn.innerHTML = svg(ICON.check) + '<span>已复制</span>';
      setTimeout(function () { btn.innerHTML = old; }, 1500);
    }
    if (navigator.clipboard && navigator.clipboard.writeText) {
      navigator.clipboard.writeText(text).then(done).catch(function () { fallback(); });
    } else fallback();
    function fallback() {
      try {
        var ta = document.createElement('textarea');
        ta.value = text; ta.style.position = 'fixed'; ta.style.opacity = '0';
        document.body.appendChild(ta); ta.select();
        document.execCommand('copy');
        document.body.removeChild(ta);
        done();
      } catch (e) { toast('error', '复制失败', '请手动选择文本复制'); }
    }
  }

  /* ------------------------------------------------------------- 历史 */
  var HISTORY_PREVIEW = 6;

  function historyFileChip(build, file) {
    var href = build.id ? '/api/builds/' + encodeURIComponent(build.id) + '/' + encodeURIComponent(file.filename) : '#';
    return '<a class="file-chip" href="' + esc(href) + '" download data-mock-link="' + (state.mock ? '1' : '') + '">' +
      '<span>' + esc(file.device || '') + '</span><span class="mono">' + esc(file.filename || '') + '</span></a>';
  }

  function historyItemHtml(b) {
    var chips = (b.files || []).map(function (f) { return historyFileChip(b, f); }).join('');
    return '' +
      '<div class="history-item" data-testid="history-item">' +
        '<span class="history-dot" aria-hidden="true"></span>' +
        '<span class="history-time mono">' + esc(shortClock(b.created)) + '</span>' +
        '<div class="history-main">' +
          '<div class="history-top">' +
            '<span class="history-key mono">' + esc(b.key || '') + '</span>' +
            '<span class="history-id mono">' + esc(b.id || '') + '</span>' +
          '</div>' +
          '<div class="history-files">' + chips + '</div>' +
        '</div>' +
        '<div class="history-actions">' +
          (b.zip ? '<a class="btn btn-ghost btn-xs" href="' + esc(b.zip) + '" download>ZIP</a>' : '') +
        '</div>' +
      '</div>';
  }

  function renderHistory() {
    if (!els.historyRoot) return;
    var builds = state.history || [];
    if (els.historyCount) els.historyCount.textContent = builds.length + ' 条';
    if (!builds.length) {
      els.historyRoot.innerHTML =
        '<div class="empty"><span class="empty-icon">' + svg(ICON.clock) + '</span>' +
        '<strong>还没有生成记录</strong><p>生成一次固件后，历史会出现在这里。</p></div>';
      return;
    }

    var expanded = !!state.historyExpanded;
    var shown = expanded ? builds : builds.slice(0, HISTORY_PREVIEW);
    var fileTotal = builds.reduce(function (sum, b) { return sum + ((b.files || []).length); }, 0);
    var today = shortDay(new Date().toISOString());

    // 按日期分组，形成轻量时间线
    var days = [];
    var byDay = {};
    shown.forEach(function (b) {
      var key = shortDay(b.created);
      if (!byDay[key]) { byDay[key] = { key: key, items: [] }; days.push(byDay[key]); }
      byDay[key].items.push(b);
    });

    var html = '' +
      '<div class="history-overview">' +
        '<span>共 <b>' + builds.length + '</b> 次生成</span>' +
        '<span class="sep" aria-hidden="true"></span>' +
        '<span>涉及 <b>' + fileTotal + '</b> 个固件</span>' +
        '<span class="sep" aria-hidden="true"></span>' +
        '<span>最近一次 <b>' + esc(shortTime(builds[0].created)) + '</b></span>' +
      '</div>' +
      '<div class="history-list">' + days.map(function (day) {
        var label = day.key === today ? ('今天 · ' + day.key) : day.key;
        return '' +
          '<section class="history-day" aria-label="' + esc(label) + '">' +
            '<div class="history-day-head">' + esc(label) +
              '<span class="count">' + day.items.length + ' 条</span></div>' +
            '<div class="history-rows">' + day.items.map(historyItemHtml).join('') + '</div>' +
          '</section>';
      }).join('') + '</div>';

    if (builds.length > HISTORY_PREVIEW) {
      html += '<button type="button" class="history-more" id="history-more">' +
        (expanded ? '收起历史记录' : '显示全部 ' + builds.length + ' 条记录') + '</button>';
    }
    els.historyRoot.innerHTML = html;

    var more = els.historyRoot.querySelector('#history-more');
    if (more) more.addEventListener('click', function () {
      state.historyExpanded = !state.historyExpanded;
      renderHistory();
    });
    if (state.mock) {
      Array.prototype.forEach.call(els.historyRoot.querySelectorAll('a[download]'), function (a) {
        a.addEventListener('click', function (ev) {
          ev.preventDefault();
          toast('info', '演示模式', '启动后端服务后即可下载真实固件。');
        });
      });
    }
  }

  /* -------------------------------------------------------- 数据加载 */
  function loadDevices() {
    state.loading = true;
    if (els.devicesGrid) {
      els.devicesGrid.innerHTML =
        '<div class="device-group"><div class="device-group-grid">' +
        '<div class="skeleton-card"></div><div class="skeleton-card"></div><div class="skeleton-card"></div>' +
        '</div></div>';
    }
    return apiGet('/api/devices').then(function (data) {
      state.devices = data.devices || [];
      state.keyRules = data.key_rules || null;
      var ids = {};
      state.devices.forEach(function (d) { ids[d.id] = true; });
      state.selected.forEach(function (id) { if (!ids[id]) state.selected.delete(id); });
      renderKeyRules(state.keyRules);
      renderDevices();
      if (els.deviceCount) els.deviceCount.textContent = String(state.devices.length);
      setStatus('online', '服务已连接');
      setStatusMeta();
      state.loading = false;
      return state.devices;
    }).catch(function (err) {
      state.loading = false;
      state.devices = [];
      if (els.devicesGrid) els.devicesGrid.innerHTML = '';
      if (els.devicesEmpty) {
        els.devicesEmpty.classList.remove('hidden');
        els.devicesEmpty.innerHTML =
          '<span class="empty-icon">' + svg(ICON.alert) + '</span><strong>无法连接服务</strong>' +
          '<p>' + esc(err.message || '请确认后端服务已启动') + '</p>' +
          '<button type="button" class="btn btn-soft btn-sm" id="retry-load">重新连接</button>';
        var retry = els.devicesEmpty.querySelector('#retry-load');
        if (retry) retry.addEventListener('click', function () { bootData(); });
      }
      if (els.deviceCount) els.deviceCount.textContent = '—';
      setStatus('offline', '服务未连接');
      renderHistoryEmptyOffline();
      throw err;
    });
  }

  function renderHistoryEmptyOffline() {
    if (!els.historyRoot) return;
    if (els.historyCount) els.historyCount.textContent = '—';
    els.historyRoot.innerHTML =
      '<div class="empty"><span class="empty-icon">' + svg(ICON.alert) + '</span>' +
      '<strong>历史暂不可用</strong><p>连接服务后自动加载。</p></div>';
  }

  function setStatusMeta() {
    if (!els.statusChip) return;
    if (state.mock) {
      setStatus('online', '演示模式 · 内置数据');
    }
  }

  function loadHistory() {
    return apiGet('/api/builds').then(function (data) {
      state.history = (data.builds || []).slice(0, 20);
      renderHistory();
    }).catch(function () {
      if (!state.mock) renderHistoryEmptyOffline();
    });
  }

  function bootData() {
    loadDevices().then(function () { loadHistory(); }).catch(function () { /* handled */ });
  }

  /* ------------------------------------------------------------ 事件 */
  function bindEvents() {
    if (els.btnRefresh) els.btnRefresh.addEventListener('click', function () {
      els.btnRefresh.classList.add('spinning');
      bootData();
      setTimeout(function () { els.btnRefresh.classList.remove('spinning'); }, 800);
      toast('info', '已刷新', '重新检测服务连接与设备状态。');
    });
    if (els.btnSelectAll) els.btnSelectAll.addEventListener('click', function () {
      state.devices.forEach(function (d) {
        if (d.firmware && d.firmware.matches) state.selected.add(d.id);
      });
      renderDevices();
      toast('info', '已全选', '仅勾选固件已就位的设备。');
    });
    if (els.btnClearSelect) els.btnClearSelect.addEventListener('click', function () {
      state.selected.clear();
      renderDevices();
    });
    if (els.keyInput) {
      els.keyInput.addEventListener('input', onKeyInput);
      els.keyInput.addEventListener('keydown', function (ev) {
        if (ev.key === 'Enter') { ev.preventDefault(); generate(); }
      });
    }
    if (els.btnRandom) els.btnRandom.addEventListener('click', randomKey);
    if (els.btnDerive) els.btnDerive.addEventListener('click', openDeriveModal);
    if (els.btnGenerate) els.btnGenerate.addEventListener('click', generate);
    if (els.optVerify) els.optVerify.addEventListener('change', updateOptionSummary);
    if (els.optContainer) els.optContainer.addEventListener('change', updateOptionSummary);
    if (els.btnRefreshHistory) els.btnRefreshHistory.addEventListener('click', function () {
      loadHistory();
      toast('info', '历史已刷新', '');
    });
    document.addEventListener('keydown', function (ev) {
      if (ev.key === 'Escape') { closeModal(); return; }
      if (ev.key === 'Enter' && !isTypingTarget(document.activeElement) && !state.generating) {
        if (state.selected.size > 0 && state.key.valid) { ev.preventDefault(); generate(); }
      }
    });
    updateOptionSummary();
  }

  function updateOptionSummary() {
    if (els.sumVerify) els.sumVerify.textContent = els.optVerify && els.optVerify.checked ? '开' : '关';
    if (els.sumContainer) els.sumContainer.textContent = els.optContainer && els.optContainer.checked ? '开' : '关';
  }

  /* ------------------------------------------------------------ 演示数据 */
  var MOCK_MODELS = [
    {
      id: 'x3c', name: 'Godox X3-C', kind: 'trigger', model: 'X3-C', firmware_version: 'V1.38',
      base: '0x08004800', notes: 'ID=99 密钥模式；KEY 帧 + ID99 双发；快捷面板 KEY/BCAST 磁贴',
      out_name: 'X3_C_v1.38_app_key{key:04X}.bin',
      original_sha256: '45fdd9522c432f4be60a470741d3e5c12a7ee629eb2b277aed5187639f85ef62',
      firmware: { present: true, size: 1021468, matches: true, path: 'devices/x3c/firmware/X3_C_v1.38_app.bin' }
    },
    {
      id: 'x3pros', name: 'Godox X3 Pro (S)', kind: 'trigger', model: 'X3 Pro S', firmware_version: 'V1.11',
      base: '0x08020000', notes: 'ID=99 密钥模式：KEY 帧 + ID99(0x01F4) 帧双发；ID 0..98 原厂单发（同步字 10 B 改写 + TX4/TX2 入口注入）',
      out_name: 'X3pro_S_v1.11_app_key{key:04X}.bin',
      original_sha256: '4b3465cab7975749dd3d109aa1b6f60ee065402869aa88d7b007325def90e103',
      firmware: { present: true, size: 1931868, matches: true, path: 'devices/x3pros/firmware/X3pro_S_v1.11_app.bin' }
    },
    {
      id: 'x3s', name: 'Godox X3-S', kind: 'trigger', model: 'X3-S', firmware_version: 'V1.25',
      base: '0x08004800', notes: '机内开关（长按拨轮/M/C ≈1.5s）+ 双发：密钥模式先发 KEY 帧再发当前 ID 普通帧',
      out_name: 'X3_S_v1.25_app_key{key:04X}.bin',
      original_sha256: '3cd40ef3f91369d3147c538cf6745f9308d82e14ad94c9bad9c8f129e0753e4d',
      firmware: { present: true, size: 1012268, matches: true, path: 'devices/x3s/firmware/X3_S_v1.25_app.bin' }
    },
    {
      id: 'x2ts', name: 'Godox X2T-S', kind: 'trigger', model: 'X2T-S', firmware_version: 'V1.4',
      base: '0x08003000', notes: 'C.Fn 第 3 项 BEEP 密钥开关(C.Fn[2]=0x200203E2)：ON(1) -> SYNC=私有 KEY；OFF(0) -> 原厂 50024/(ID+1)。52 B 整函数改写(保留原厂 delay+SCAL 尾部) + C.Fn 加/减即时生效 hook（未用中断向量槽注入）',
      out_name: 'X2T-S_V1.4_key{key:04X}.bin',
      original_sha256: '478476447ff8d55496180facf578662dcbf9a1af0cf21f79df048b7df0a76efc',
      firmware: { present: true, size: 58628, matches: true, path: 'devices/x2ts/firmware/X2T-S_V1.4.bin' }
    },
    {
      id: 'x2tc', name: 'Godox X2T-C', kind: 'trigger', model: 'X2T-C', firmware_version: 'V1.01',
      base: '0x08005000', notes: 'C.Fn 菜单 LIGHT 密钥开关（ON=密钥模式 / 12sec·OFF 原厂公式）；56 B 整函数改写 + 背光读者屏蔽 + 保存前重设同步字 hook；原厂文件即 G3 容器（输出保持容器）',
      out_name: 'X2TC_Z02_V1.01_key{key:04X}.bin',
      original_sha256: 'd185e4101ad61422f2ac2bc9ee7ade1e6c245c6002361dfd50ed5f9e7b90210b',
      firmware: { present: true, size: 79748, matches: true, path: 'devices/x2tc/firmware/X2TC_Z02_V1.01.bin' }
    },
    {
      id: 'ad100', name: 'Godox AD100 Pro', kind: 'flash', model: 'AD100 Pro', firmware_version: 'V1.2',
      base: '0x08003000', notes: 'C.Fn 第 7 项 KEY 开关；ID 菜单恢复原厂含义',
      out_name: 'AD100pro_V1.2_key{key:04X}.bin',
      original_sha256: '3a3d3b917615bb20339f6c355ebe66d8d94e8199509c1e820007542c1ebf4e7c',
      firmware: { present: true, size: 50248, matches: true, path: 'devices/ad100pro/firmware/AD100pro_V1.2.bin' }
    },
    {
      id: 'ad200', name: 'Godox AD200 Pro', kind: 'flash', model: 'AD200 Pro', firmware_version: 'V1.60',
      base: '0x08003000', notes: 'C.Fn 第 8 项 F8 开关（原厂菜单项语义复用）；ID 菜单恢复原厂含义',
      out_name: 'AD200pro_V1.60_key{key:04X}.bin',
      original_sha256: 'fc0f77f1eb1c9f9d6f1f96d15daa5722ae33a57fceffe1da02739530dd798bd0',
      firmware: { present: true, size: 46380, matches: true, path: 'devices/ad200pro/firmware/AD200pro_V1.60.bin' }
    },
    {
      id: 'v1c', name: 'Godox V1C', kind: 'flash', model: 'V1C', firmware_version: 'V2.0',
      base: '0x08003000', notes: 'C.Fn 第 3 页 LIGHT 密钥开关（ON=密钥模式 / 12sec·OFF 原厂公式）；56 B 整函数改写 + 注入 hook + 读者屏蔽',
      out_name: 'V1C_V2.0_key{key:04X}.bin',
      original_sha256: '4435dda84903cb97cfaf34f7eeb15ecb6168771d60b57da1c346e6a9ca36edfb',
      firmware: { present: true, size: 88032, matches: true, path: 'devices/v1c/firmware/V1C_V2.0.bin' }
    },
    {
      id: 'v1s', name: 'Godox V1S', kind: 'flash', model: 'V1S', firmware_version: 'V1.8',
      base: '0x08003000', notes: 'C.Fn 第 3 页 LIGHT 密钥开关（ON=密钥模式 / 12sec·OFF 原厂公式）；56 B 整函数改写 + 两个 C.Fn 改值门改 b 同步块（即时生效）+ 读者屏蔽',
      out_name: 'V1S_V1.8_key{key:04X}.bin',
      original_sha256: '87a0600046e1b7c2198a7ac8fac15959bb3a5d582edf44d8e0df38706dcb917a',
      firmware: { present: true, size: 78500, matches: true, path: 'devices/v1s/firmware/V1S_V1.8.bin' }
    },
    {
      id: 'v860iiis', name: 'Godox V860III-S', kind: 'flash', model: 'V860III-S', firmware_version: 'V1.1',
      base: '0x08003000', notes: 'C.Fn 第 3 页 LIGHT 密钥开关（ON=密钥模式 / 12sec·OFF 原厂公式）；56 B 整函数改写 + 注入 hook + 读者屏蔽',
      out_name: 'V860IIIS_V1.1_key{key:04X}.bin',
      original_sha256: 'bdc8f3547808d121f15a6c58b9363c5792c957c2dce2a16b203276ab078d0b20',
      firmware: { present: true, size: 79300, matches: true, path: 'devices/v860iiis/firmware/V860IIIS_V1.1.bin' }
    },
    {
      id: 'v860iiic', name: 'Godox V860III-C', kind: 'flash', model: 'V860III-C', firmware_version: 'V1.3',
      base: '0x08003000', notes: 'C.Fn 第 3 页 LIGHT 密钥开关（ON=密钥模式 / 12sec·OFF 原厂公式）；56 B 整函数改写 + 注入 hook + 读者屏蔽',
      out_name: 'V860IIIC_V1.3_key{key:04X}.bin',
      original_sha256: '8530a7e85ce1098d64d72d7ea4a28f3fdfb489e3d39da49be1ffa166f6ba270d',
      firmware: { present: true, size: 87612, matches: true, path: 'devices/v860iiic/firmware/V860IIIC_V1.3.bin' }
    }
  ];

  function mockRules() {
    var forbidden = ['0x0000', '0xFFFF', '0xAAAA', '0x5555', '0x8000'];
    var seen = {};
    var publicValues = [];
    for (var i = 0; i < 100; i++) {
      var v = Math.floor(50024 / (i + 1));
      var hex = '0x' + ('000' + v.toString(16).toUpperCase()).slice(-4);
      if (!seen[hex]) { seen[hex] = 1; publicValues.push(hex); }
    }
    return {
      width: 4, forbidden: forbidden, public_sync_values: publicValues,
      note: '密钥必须是 16 位值（4 位十六进制），且不能等于原厂 ID 能产生的同步字或保留值。'
    };
  }

  function fakeSha(seed) {
    var h = 2166136261 >>> 0;
    var out = '';
    var s = String(seed);
    for (var i = 0; i < 64; i++) {
      h ^= s.charCodeAt(i % s.length);
      h = Math.imul(h, 16777619) >>> 0;
      out += ((h >>> 28) & 0xF).toString(16);
    }
    return out;
  }

  function mockKeyObj(hexText) {
    var v = parseInt(hexText.replace(/^0x/i, ''), 16);
    return {
      hex: '0x' + ('000' + v.toString(16).toUpperCase()).slice(-4),
      decimal: v,
      sync1: '0x' + ('0' + ((v >> 8) & 0xFF).toString(16).toUpperCase()).slice(-2),
      sync0: '0x' + ('0' + (v & 0xFF).toString(16).toUpperCase()).slice(-2)
    };
  }

  function mockValidate(text) {
    var t = String(text || '').toUpperCase().replace(/^0X/, '');
    if (!/^[0-9A-F]{4}$/.test(t)) return { ok: false, error: '密钥必须是 4 位十六进制（0-9、A-F）' };
    var keyObj = mockKeyObj(t);
    var rules = mockRules();
    if (rules.forbidden.indexOf('0x' + t) >= 0) {
      return { ok: false, error: '0x' + t + ' 是保留/危险同步字', suggest: suggestFor(t) };
    }
    if (rules.public_sync_values.indexOf('0x' + t) >= 0) {
      return { ok: false, error: '0x' + t + ' 与原厂 ID 同步字冲突', suggest: suggestFor(t) };
    }
    return { ok: true, key: keyObj };
  }

  function mockGenerate(body) {
    var keyObj = mockKeyObj(body.key || 'D798');
    var verify = body.verify !== false;
    var files = (body.devices || []).map(function (id) {
      var d = null;
      MOCK_MODELS.forEach(function (m) { if (m.id === id) d = m; });
      if (!d) return null;
      var filename = d.out_name.replace('{key:04X}', keyObj.hex.replace(/^0x/i, ''));
      return {
        device: d.id, name: d.name, filename: filename, size: d.firmware.size,
        sha256: fakeSha(d.id + keyObj.hex),
        original_sha256: d.original_sha256,
        patch_count: d.id === 'x3c' ? 15 : (d.id === 'x3pros' ? 4 : 6),
        patch_log: [
          { addr: d.base, offset: 0x100, before: '41 1c', after: '01 21', desc: '同步字设置：改为私有 KEY' },
          { addr: '0x' + (parseInt(d.base, 16) + 0x404).toString(16).toUpperCase(), offset: 0x404, before: '00 00', after: '96 02', desc: '补丁注入入口' },
          { addr: '0x' + (parseInt(d.base, 16) + 0x7A0).toString(16).toUpperCase(), offset: 0x7A0, before: '00 00 00 00', after: '70 47 00 00', desc: '菜单开关跳板' }
        ],
        verify_results: verify ? [
          { ok: true, msg: '固件长度 ' + d.firmware.size + ' 字节，原位补丁保持不变' },
          { ok: true, msg: '补丁点逐字节比对通过' },
          { ok: true, msg: 'Unicorn 调用级测试：SYNC1=' + keyObj.sync1 + ' SYNC0=' + keyObj.sync0 },
          { ok: true, msg: '整机回归：与原厂镜像运行路径一致' }
        ] : [],
        warnings: [],
        extra_files: body.container ? [d.id.toUpperCase().replace('X3C', 'X3C') + '_container.bin'] : [],
        download: '/api/builds/demo/' + filename
      };
    }).filter(Boolean);
    var build = {
      id: 'demo-' + keyObj.hex.replace(/^0x/i, ''),
      created: new Date().toISOString(),
      key: keyObj, verify: verify, container: !!body.container, files: files,
      zip: '/api/builds/demo/all.zip'
    };
    state.mockBuilds.unshift({
      id: build.id, created: build.created, key: keyObj.hex, verify: verify, container: !!body.container,
      files: files.map(function (f) {
        return { device: f.device, name: f.name, filename: f.filename, size: f.size, sha256: f.sha256, download: f.download };
      }),
      zip: build.zip
    });
    return { ok: true, build: build };
  }

  function mockFetch(path, init) {
    var body = {};
    try { body = init && init.body ? JSON.parse(init.body) : {}; } catch (e) { body = {}; }
    var p = path.split('?')[0];
    var delay = 350;
    return new Promise(function (resolve, reject) {
      setTimeout(function () {
        try {
          if (p === '/api/health') {
            resolve({ ok: true, version: 'mock', python: 'demo', devices: MOCK_MODELS.length });
          } else if (p === '/api/devices') {
            resolve({ ok: true, devices: MOCK_MODELS, key_rules: mockRules() });
          } else if (p === '/api/key/random') {
            var v = 0;
            do { v = Math.floor(Math.random() * 0xFFFE) + 1; } while (!mockValidate(('000' + v.toString(16)).slice(-4)).ok);
            resolve({ ok: true, key: mockKeyObj(v.toString(16)) });
          } else if (p === '/api/key/derive') {
            var pass = body.passphrase || '';
            if (!pass) {
              var e1 = new Error('口令不能为空');
              e1.data = { ok: false, error: '口令不能为空' };
              reject(e1); return;
            }
            var h = 0;
            for (var i = 0; i < pass.length; i++) h = (h * 31 + pass.charCodeAt(i)) & 0xFFFF;
            var tries = 0;
            while (!mockValidate(('000' + h.toString(16)).slice(-4)).ok && tries++ < 5000) h = (h + 1) & 0xFFFF;
            resolve({ ok: true, key: mockKeyObj(h.toString(16)) });
          } else if (p === '/api/key/validate') {
            var r = mockValidate(body.text);
            if (r.ok) resolve({ ok: true, key: r.key });
            else {
              var e2 = new Error(r.error || '密钥不合法');
              e2.data = { ok: false, error: r.error, suggest: r.suggest };
              reject(e2);
            }
          } else if (p === '/api/generate') {
            resolve(mockGenerate(body));
          } else if (p === '/api/builds') {
            resolve({ ok: true, builds: state.mockBuilds });
          } else if (p === '/api/firmware/upload') {
            var devId = (path.split('device=')[1] || '').split('&')[0];
            MOCK_MODELS.forEach(function (m) {
              if (m.id === devId) { m.firmware.present = true; m.firmware.matches = true; }
            });
            resolve({ ok: true, device: devId, matches: true, sha256: 'mock', size: 0 });
          } else {
            resolve({ ok: true });
          }
        } catch (err) {
          reject(err);
        }
      }, delay);
    });
  }

  /* --------------------------------------------------------------- 启动 */
  function boot() {
    els = {
      statusChip: $('status-chip'), statusText: $('status-text'),
      deviceCount: $('device-count'), deviceCountChip: $('device-count-chip'),
      devicesGrid: $('devices-grid'), devicesEmpty: $('devices-empty'),
      selectedBadge: $('selected-badge'), btnSelectAll: $('btn-select-all'),
      btnClearSelect: $('btn-clear-select'), btnRefresh: $('btn-refresh'),
      keyField: $('key-field'), keyInput: $('key-input'), keyTarget: $('key-target'),
      keyStatus: $('key-status'), keyStatusIcon: $('key-status-icon'), keyStatusText: $('key-status-text'),
      keySuggest: $('key-suggest'), statSync1: $('stat-sync1'), statSync0: $('stat-sync0'),
      statDecimal: $('stat-decimal'), btnRandom: $('btn-random'), btnDerive: $('btn-derive'),
      rulesNote: $('rules-note'), forbiddenChips: $('forbidden-chips'), publicChips: $('public-chips'),
      optVerify: $('opt-verify'), optContainer: $('opt-container'),
      sumDevices: $('sum-devices'), sumKey: $('sum-key'), sumVerify: $('sum-verify'), sumContainer: $('sum-container'),
      btnGenerate: $('btn-generate'), generateSpinner: $('generate-spinner'),
      generateLabel: $('generate-label'), generateHint: $('generate-hint'),
      panelResult: $('panel-result'), resultPill: $('result-pill'), resultActions: $('result-actions'),
      resultRoot: $('result-root'), historyRoot: $('history-root'), historyCount: $('history-count'),
      btnRefreshHistory: $('btn-refresh-history'), toastRoot: $('toast-root'), modalRoot: $('modal-root'),
      stepDevices: $('step-devices'), stepKey: $('step-key'), stepBuild: $('step-build'),
      stepDevicesNote: $('step-devices-note'), stepKeyNote: $('step-key-note'), stepBuildNote: $('step-build-note'),
      mockBadge: $('mock-badge'), mockBanner: $('mock-banner')
    };

    bindEvents();
    document.documentElement.classList.remove('booting');
    renderKeyRules(mockRules());

    if (state.mock) {
      if (els.mockBadge) els.mockBadge.classList.remove('hidden');
      if (els.mockBanner) els.mockBanner.classList.remove('hidden');
      setStatus('online', '演示模式 · 内置数据');
    } else {
      setStatus('loading', '连接服务…');
    }

    bootData();
    updateSelectionUI();
  }

  window.StudioApp = {
    boot: boot,
    __debug: function () {
      return {
        selected: Array.from(state.selected),
        key: state.key,
        generating: state.generating,
        devices: state.devices.length,
        buttonDisabled: els.btnGenerate ? els.btnGenerate.disabled : null,
        buttonId: els.btnGenerate ? els.btnGenerate.id : null
      };
    }
  };
})();
