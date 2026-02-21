#ifndef INDEX_HTML_H
#define INDEX_HTML_H

// Embedded Floor Lamp Control UI (same as index.html), served at GET / and GET /index.html
static const char INDEX_HTML[] PROGMEM = R"LAMPIDX(
<!DOCTYPE html>
<html lang="en" class="dark">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Floor Lamp Control</title>
  <script src="https://cdn.tailwindcss.com"></script>
  <script>
    tailwind.config = {
      darkMode: 'class',
      theme: {
        extend: {
          colors: {
            frame: '#0a0a0a',
            glow: '#fbbf24',
            'glow-dim': '#fcd34d',
          },
          fontFamily: {
            display: ['DM Sans', 'system-ui', 'sans-serif'],
          },
          boxShadow: {
            'glow': '0 0 20px rgba(251, 191, 36, 0.4)',
            'glow-strong': '0 0 30px rgba(251, 191, 36, 0.6)',
          },
        },
      },
    }
  </script>
  <link rel="preconnect" href="https://fonts.googleapis.com">
  <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
  <link href="https://fonts.googleapis.com/css2?family=DM+Sans:ital,opsz,wght@0,9..40,400;0,9..40,500;0,9..40,600;0,9..40,700&display=swap" rel="stylesheet">
  <style>
    .bulb-on { box-shadow: 0 0 20px rgba(251, 191, 36, 0.5); }
    .bulb-off { filter: grayscale(0.8); opacity: 0.5; }
    .toggle-track { transition: background 0.3s ease; }
    .toggle-thumb { transition: transform 0.3s ease; }
  </style>
</head>
<body class="bg-frame min-h-screen font-display antialiased text-slate-100">
  <div class="max-w-md mx-auto px-6 py-10">
    <header class="text-center mb-10">
      <div class="inline-flex items-center justify-center w-16 h-16 rounded-2xl bg-slate-800/80 border border-slate-700/50 mb-4">
        <svg class="w-8 h-8 text-amber-400 rotate-180" fill="currentColor" viewBox="0 0 24 24">
          <path d="M9 21c0 .5.4 1 1 1h4c.6 0 1-.5 1-1v-1H9v1zm3-19C8.1 2 5 5.1 5 9c0 2.4 1.2 4.5 3 5.7V17c0 .5.4 1 1 1h6c.6 0 1-.5 1-1v-2.3c1.8-1.3 3-3.4 3-5.7 0-3.9-3.1-7-7-7z"/>
        </svg>
      </div>
      <h1 class="text-2xl font-semibold tracking-tight text-white">Floor Lamp</h1>
      <p class="text-slate-500 text-sm mt-1">Control all 5 bulbs</p>
    </header>
    <div class="mb-6 rounded-xl bg-slate-900/90 border border-slate-700/50 p-3">
      <label for="api-host" class="block text-xs text-slate-400 mb-2">ESP32 API Host</label>
      <div class="flex gap-2">
        <input id="api-host" type="text" value="192.168.1.11" placeholder="192.168.1.11" class="flex-1 bg-slate-800 border border-slate-700 rounded-lg px-3 py-2 text-sm text-slate-100 placeholder:text-slate-500 focus:outline-none focus:ring-1 focus:ring-amber-400/60">
        <button id="save-host" type="button" class="px-3 py-2 rounded-lg text-sm font-medium bg-slate-700 text-slate-200 hover:bg-slate-600">Save</button>
      </div>
      <p id="host-hint" class="text-[11px] text-slate-500 mt-2">Served from this device. Change if using a different host.</p>
    </div>
    <div class="space-y-3">
      <template id="bulb-template">
        <div class="bulb-card bg-slate-900/90 rounded-xl border border-slate-700/50 p-4 flex items-center justify-between gap-4 transition-all duration-300 hover:border-slate-600/50" data-bulb="">
          <div class="flex items-center gap-4 flex-1 min-w-0">
            <div class="bulb-icon w-12 h-12 rounded-full bg-slate-800 flex items-center justify-center flex-shrink-0 transition-all duration-300" data-icon="">
              <svg class="w-6 h-6 text-amber-400/60 rotate-180" fill="currentColor" viewBox="0 0 24 24">
                <path d="M9 21c0 .5.4 1 1 1h4c.6 0 1-.5 1-1v-1H9v1zm3-19C8.1 2 5 5.1 5 9c0 2.4 1.2 4.5 3 5.7V17c0 .5.4 1 1 1h6c.6 0 1-.5 1-1v-2.3c1.8-1.3 3-3.4 3-5.7 0-3.9-3.1-7-7-7z"/>
              </svg>
            </div>
            <span class="font-medium text-slate-200 truncate" data-label="">Bulb 1</span>
          </div>
          <div class="flex items-center gap-2 flex-shrink-0">
            <button type="button" class="toggle-btn px-4 py-2 rounded-lg text-sm font-medium transition-all duration-200" data-action="off">OFF</button>
            <button type="button" class="toggle-btn px-4 py-2 rounded-lg text-sm font-medium transition-all duration-200" data-action="on">ON</button>
          </div>
        </div>
      </template>
      <div id="bulbs-container"></div>
    </div>
    <div class="mt-8 flex gap-3">
      <button id="all-on" type="button" class="flex-1 py-3 px-4 rounded-xl bg-amber-500/20 hover:bg-amber-500/30 text-amber-400 font-medium transition-colors border border-amber-500/30">All On</button>
      <button id="all-off" type="button" class="flex-1 py-3 px-4 rounded-xl bg-slate-800 hover:bg-slate-700 text-slate-400 font-medium transition-colors border border-slate-600/50">All Off</button>
    </div>
    <div class="mt-8 flex flex-col items-center gap-2">
      <div id="connection-status" class="inline-flex items-center gap-2 px-3 py-1.5 rounded-full bg-slate-800/80 border border-slate-700/50">
        <span id="status-dot" class="w-2 h-2 rounded-full bg-slate-500 animate-pulse"></span>
        <span id="status-text" class="text-slate-500 text-xs">Checking...</span>
      </div>
      <p id="status" class="text-center text-slate-600 text-xs">Ready</p>
    </div>
    <p class="text-center text-slate-600 text-xs mt-6">made for <span class="text-red-400">Malu</span> <span class="text-red-400">❤</span></p>
  </div>
  <script>
    const BULB_COUNT = 5;
    const DEFAULT_API_HOST = '192.168.1.11';
    const API_HOST_STORAGE_KEY = 'esp32ApiHost';
    const BULB_UI_TO_API = { 1: 1, 2: 2, 3: 3, 4: 4, 5: 5 };
    const bulbCards = new Array(BULB_COUNT + 1);
    function normalizeBaseUrl(value) {
      let host = (value || '').trim();
      if (!host) host = DEFAULT_API_HOST;
      if (!/^https?:\/\//i.test(host)) host = 'http://' + host;
      return host.replace(/\/+$/, '');
    }
    let baseUrl = normalizeBaseUrl(localStorage.getItem(API_HOST_STORAGE_KEY) || DEFAULT_API_HOST);
    function syncHostInput() {
      var input = document.getElementById('api-host');
      var hint = document.getElementById('host-hint');
      input.value = baseUrl.replace(/^https?:\/\//i, '');
      hint.textContent = 'Using: ' + baseUrl;
    }
    function saveApiHost() {
      var input = document.getElementById('api-host');
      baseUrl = normalizeBaseUrl(input.value);
      localStorage.setItem(API_HOST_STORAGE_KEY, baseUrl);
      syncHostInput();
      loadInitialState();
      checkConnection();
    }
    function getApiUrl(uiIndex, desiredOn) {
      var apiLed = BULB_UI_TO_API[uiIndex] !== undefined ? BULB_UI_TO_API[uiIndex] : uiIndex;
      var apiState = desiredOn ? 'off' : 'on';
      return baseUrl + '/led' + apiLed + '/' + apiState;
    }
    function createBulbCard(index) {
      var template = document.getElementById('bulb-template');
      var clone = template.content.cloneNode(true);
      var card = clone.querySelector('.bulb-card');
      var label = clone.querySelector('[data-label]');
      card.dataset.bulb = index;
      label.textContent = 'Bulb ' + index;
      bulbCards[index] = card;
      var offBtn = clone.querySelector('[data-action="off"]');
      var onBtn = clone.querySelector('[data-action="on"]');
      offBtn.classList.add('bg-slate-700', 'text-slate-400', 'hover:bg-slate-600');
      onBtn.classList.add('bg-amber-500/20', 'text-amber-400', 'hover:bg-amber-500/30', 'border', 'border-amber-500/30');
      offBtn.onclick = function() { toggleBulb(index, false); };
      onBtn.onclick = function() { toggleBulb(index, true); };
      return clone;
    }
    function setBulbState(card, on) {
      var icon = card.querySelector('[data-icon]');
      var offBtn = card.querySelector('[data-action="off"]');
      var onBtn = card.querySelector('[data-action="on"]');
      if (on) {
        icon.classList.remove('bg-slate-800');
        icon.classList.add('bulb-on', 'bg-amber-500/20', 'border', 'border-amber-500/40');
        icon.querySelector('svg').classList.remove('text-amber-400/60');
        icon.querySelector('svg').classList.add('text-amber-400');
        offBtn.className = 'toggle-btn px-4 py-2 rounded-lg text-sm font-medium transition-all duration-200 bg-slate-800 text-slate-500 border border-slate-700 hover:bg-slate-700';
        onBtn.className = 'toggle-btn px-4 py-2 rounded-lg text-sm font-medium transition-all duration-200 bg-amber-500 text-slate-900 border border-amber-500 hover:bg-amber-400';
      } else {
        icon.classList.add('bg-slate-800');
        icon.classList.remove('bulb-on', 'bg-amber-500/20', 'border', 'border-amber-500/40');
        icon.querySelector('svg').classList.add('text-amber-400/60');
        icon.querySelector('svg').classList.remove('text-amber-400');
        offBtn.className = 'toggle-btn px-4 py-2 rounded-lg text-sm font-medium transition-all duration-200 bg-slate-700 text-slate-200 border border-slate-600 hover:bg-slate-600';
        onBtn.className = 'toggle-btn px-4 py-2 rounded-lg text-sm font-medium transition-all duration-200 bg-amber-500/20 text-amber-400 border border-amber-500/30 hover:bg-amber-500/30';
      }
    }
    function toggleBulb(bulb, on) {
      var statusEl = document.getElementById('status');
      statusEl.textContent = 'Bulb ' + bulb + ' ' + (on ? 'on' : 'off') + '...';
      fetch(getApiUrl(bulb, on), { cache: 'no-store' }).then(function(res) {
        if (res.ok) {
          var card = bulbCards[bulb];
          if (card) setBulbState(card, on);
          statusEl.textContent = 'Ready';
        } else { statusEl.textContent = 'Error: ' + res.status; }
      }).catch(function(err) { statusEl.textContent = 'Error: ' + err.message; });
    }
    function setAllBulbs(on) {
      var statusEl = document.getElementById('status');
      statusEl.textContent = on ? 'Turning all on...' : 'Turning all off...';
      var promises = [];
      for (var i = 1; i <= BULB_COUNT; i++) promises.push(fetch(getApiUrl(i, on), { cache: 'no-store' }));
      Promise.all(promises).then(function() {
        for (var i = 1; i <= BULB_COUNT; i++) if (bulbCards[i]) setBulbState(bulbCards[i], on);
        statusEl.textContent = 'Ready';
      }).catch(function(err) { statusEl.textContent = 'Error: ' + err.message; });
    }
    function applyBulbStates(states) {
      for (var i = 1; i <= BULB_COUNT; i++) {
        var card = bulbCards[i];
        if (!card) continue;
        setBulbState(card, !!states[i - 1]);
      }
    }
    function loadInitialState() {
      var statusEl = document.getElementById('status');
      statusEl.textContent = 'Syncing bulb state...';
      return fetch(baseUrl + '/state', { cache: 'no-store' }).then(function(res) {
        if (!res.ok) throw new Error('state ' + res.status);
        return res.json();
      }).then(function(data) {
        if (!data || !Array.isArray(data.bulbs)) throw new Error('invalid state');
        applyBulbStates(data.bulbs);
        statusEl.textContent = 'Ready';
      }).catch(function(err) {
        // Fallback keeps UI responsive even if state endpoint is unavailable.
        applyBulbStates([false, false, false, false, false]);
        statusEl.textContent = 'Ready';
      });
    }
    var container = document.getElementById('bulbs-container');
    var fragment = document.createDocumentFragment();
    for (var i = 1; i <= BULB_COUNT; i++) fragment.appendChild(createBulbCard(i));
    container.appendChild(fragment);
    document.getElementById('save-host').onclick = saveApiHost;
    document.getElementById('api-host').addEventListener('keydown', function(e) { if (e.key === 'Enter') { e.preventDefault(); saveApiHost(); } });
    syncHostInput();
    document.getElementById('all-on').onclick = function() { setAllBulbs(true); };
    document.getElementById('all-off').onclick = function() { setAllBulbs(false); };
    function checkConnection() {
      var dot = document.getElementById('status-dot');
      var text = document.getElementById('status-text');
      fetch(baseUrl + '/ping', { cache: 'no-store' }).then(function(res) {
        if (res.ok) {
          dot.classList.remove('bg-slate-500', 'animate-pulse');
          dot.classList.add('bg-emerald-500');
          text.textContent = 'API live • Device ready';
          text.classList.remove('text-slate-500');
          text.classList.add('text-slate-400');
        } else {
          dot.classList.remove('bg-emerald-500');
          dot.classList.add('bg-amber-500', 'animate-pulse');
          text.textContent = 'API error';
          text.classList.add('text-amber-400/80');
        }
      }).catch(function() {
        dot.classList.remove('bg-emerald-500');
        dot.classList.add('bg-red-500/80', 'animate-pulse');
        text.textContent = 'Offline';
        text.classList.remove('text-slate-400');
        text.classList.add('text-red-400/80');
      });
    }
    loadInitialState();
    checkConnection();
    setInterval(checkConnection, 10000);
  </script>
</body>
</html>
)LAMPIDX";

#endif
