const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="cache-control" content="no-cache">
    <title>INTERCOM CTRL // ESP8266</title>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;500;700&family=Space+Grotesk:wght@400;500;700&display=swap" rel="stylesheet">
    <style>
        :root {
            --bg-primary: #0a0a0b;
            --bg-secondary: #111113;
            --bg-tertiary: #1a1a1d;
            --border-color: #2a2a2d;
            --border-highlight: #3a3a3d;
            --text-primary: #e8e8e8;
            --text-secondary: #888;
            --text-muted: #555;
            --accent-cyan: #00d4ff;
            --accent-green: #00ff88;
            --accent-yellow: #ffcc00;
            --accent-red: #ff4444;
            --accent-orange: #ff8800;
            --font-mono: 'JetBrains Mono', monospace;
            --font-display: 'Space Grotesk', sans-serif;
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }

        body {
            font-family: var(--font-mono);
            background: var(--bg-primary);
            color: var(--text-primary);
            min-height: 100vh;
            overflow-x: hidden;
        }

        /* Subtle grid background */
        body::before {
            content: '';
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background-image: 
                linear-gradient(rgba(255,255,255,0.02) 1px, transparent 1px),
                linear-gradient(90deg, rgba(255,255,255,0.02) 1px, transparent 1px);
            background-size: 50px 50px;
            pointer-events: none;
            z-index: 0;
        }

        .container {
            position: relative;
            z-index: 1;
            max-width: 1200px;
            margin: 0 auto;
            padding: 20px;
        }

        /* Header */
        header {
            border: 1px solid var(--border-color);
            background: var(--bg-secondary);
            padding: 20px 30px;
            margin-bottom: 20px;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }

        .logo {
            display: flex;
            align-items: center;
            gap: 15px;
        }

        .logo-icon {
            width: 40px;
            height: 40px;
            border: 2px solid var(--accent-cyan);
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 20px;
        }

        .logo h1 {
            font-family: var(--font-display);
            font-size: 1.5rem;
            font-weight: 700;
            letter-spacing: 2px;
            color: var(--text-primary);
        }

        .logo span {
            font-size: 0.75rem;
            color: var(--text-muted);
            letter-spacing: 1px;
        }

        .header-info {
            text-align: right;
            font-size: 0.75rem;
            color: var(--text-secondary);
        }

        .header-info .ip {
            color: var(--accent-cyan);
            font-weight: 500;
        }

        .header-info .mac {
            color: var(--text-muted);
        }

        /* Main Grid Layout */
        .main-grid {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 20px;
        }

        @media (max-width: 900px) {
            .main-grid {
                grid-template-columns: 1fr;
            }
        }

        /* Panel Styles */
        .panel {
            border: 1px solid var(--border-color);
            background: var(--bg-secondary);
        }

        .panel-header {
            padding: 15px 20px;
            border-bottom: 1px solid var(--border-color);
            display: flex;
            align-items: center;
            gap: 10px;
            font-size: 0.8rem;
            text-transform: uppercase;
            letter-spacing: 2px;
            color: var(--text-secondary);
        }

        .panel-header::before {
            content: '';
            width: 8px;
            height: 8px;
            background: var(--accent-cyan);
        }

        .panel-content {
            padding: 20px;
        }

        /* Status Panel */
        .status-grid {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 15px;
        }

        .status-item {
            background: var(--bg-tertiary);
            border: 1px solid var(--border-color);
            padding: 15px;
            position: relative;
            overflow: hidden;
        }

        .status-item::before {
            content: '';
            position: absolute;
            top: 0;
            left: 0;
            width: 3px;
            height: 100%;
            background: var(--accent-cyan);
            opacity: 0.5;
        }

        .status-label {
            font-size: 0.65rem;
            text-transform: uppercase;
            letter-spacing: 2px;
            color: var(--text-muted);
            margin-bottom: 8px;
        }

        .status-value {
            font-size: 1.1rem;
            font-weight: 700;
            color: var(--text-primary);
        }

        .status-item.active .status-value {
            color: var(--accent-green);
        }

        .status-item.warning .status-value {
            color: var(--accent-yellow);
        }

        .status-item.alert .status-value {
            color: var(--accent-red);
            animation: pulse 1s infinite;
        }

        .status-item.state-idle::before { background: var(--accent-green); }
        .status-item.state-operating::before { background: var(--accent-yellow); }
        .status-item.state-terminating::before { background: var(--accent-orange); }

        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }

        /* Control Panel */
        .control-grid {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 12px;
        }

        .btn {
            font-family: var(--font-mono);
            font-size: 0.8rem;
            font-weight: 500;
            text-transform: uppercase;
            letter-spacing: 1px;
            padding: 18px 20px;
            border: 1px solid var(--border-color);
            background: var(--bg-tertiary);
            color: var(--text-primary);
            cursor: pointer;
            transition: all 0.15s ease;
            position: relative;
            overflow: hidden;
        }

        .btn::before {
            content: '';
            position: absolute;
            top: 0;
            left: -100%;
            width: 100%;
            height: 100%;
            background: linear-gradient(90deg, transparent, rgba(255,255,255,0.1), transparent);
            transition: left 0.4s ease;
        }

        .btn:hover {
            border-color: var(--accent-cyan);
            background: var(--bg-secondary);
        }

        .btn:hover::before {
            left: 100%;
        }

        .btn:active {
            transform: scale(0.98);
        }

        .btn.primary {
            background: var(--accent-cyan);
            color: var(--bg-primary);
            border-color: var(--accent-cyan);
        }

        .btn.primary:hover {
            background: #00b8e0;
            border-color: #00b8e0;
        }

        .btn.danger {
            border-color: var(--accent-red);
            color: var(--accent-red);
        }

        .btn.danger:hover {
            background: var(--accent-red);
            color: var(--bg-primary);
        }

        .btn.warning {
            border-color: var(--accent-orange);
            color: var(--accent-orange);
        }

        .btn.warning:hover {
            background: var(--accent-orange);
            color: var(--bg-primary);
        }

        .btn.full-width {
            grid-column: span 2;
        }

        .btn:disabled {
            opacity: 0.4;
            cursor: not-allowed;
        }

        /* System Panel */
        .system-grid {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 12px;
        }

        @media (max-width: 600px) {
            .system-grid {
                grid-template-columns: repeat(2, 1fr);
            }
        }

        /* Terminal Panel */
        .terminal-panel {
            grid-column: span 2;
        }

        @media (max-width: 900px) {
            .terminal-panel {
                grid-column: span 1;
            }
        }

        .terminal-toggle {
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 15px 20px;
            background: var(--bg-tertiary);
            border: 1px solid var(--border-color);
            cursor: pointer;
            transition: all 0.15s ease;
        }

        .terminal-toggle:hover {
            border-color: var(--accent-cyan);
        }

        .terminal-toggle-text {
            display: flex;
            align-items: center;
            gap: 10px;
            font-size: 0.8rem;
            text-transform: uppercase;
            letter-spacing: 1px;
        }

        .terminal-toggle-icon {
            width: 10px;
            height: 10px;
            border-right: 2px solid var(--text-secondary);
            border-bottom: 2px solid var(--text-secondary);
            transform: rotate(45deg);
            transition: transform 0.2s ease;
        }

        .terminal-toggle.open .terminal-toggle-icon {
            transform: rotate(-135deg);
        }

        .terminal-container {
            display: none;
            border: 1px solid var(--border-color);
            border-top: none;
        }

        .terminal-container.visible {
            display: block;
        }

        .terminal-container iframe {
            width: 100%;
            height: 400px;
            border: none;
            background: #000;
        }

        /* Modal */
        .modal-overlay {
            display: none;
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
            background: rgba(0, 0, 0, 0.8);
            z-index: 1000;
            align-items: center;
            justify-content: center;
        }

        .modal-overlay.visible {
            display: flex;
        }

        .modal {
            background: var(--bg-secondary);
            border: 1px solid var(--border-color);
            max-width: 450px;
            width: 90%;
        }

        .modal-header {
            padding: 20px;
            border-bottom: 1px solid var(--border-color);
            display: flex;
            align-items: center;
            gap: 12px;
        }

        .modal-header-icon {
            width: 40px;
            height: 40px;
            background: rgba(255, 68, 68, 0.1);
            border: 1px solid var(--accent-red);
            display: flex;
            align-items: center;
            justify-content: center;
            font-size: 1.2rem;
        }

        .modal-header h3 {
            font-family: var(--font-display);
            font-size: 1.1rem;
            font-weight: 600;
            color: var(--accent-red);
        }

        .modal-body {
            padding: 20px;
            font-size: 0.85rem;
            line-height: 1.6;
            color: var(--text-secondary);
        }

        .modal-body strong {
            color: var(--text-primary);
        }

        .modal-footer {
            padding: 20px;
            border-top: 1px solid var(--border-color);
            display: flex;
            gap: 12px;
            justify-content: flex-end;
        }

        .modal-footer .btn {
            padding: 12px 24px;
        }

        /* Footer */
        footer {
            margin-top: 20px;
            padding: 15px 20px;
            border: 1px solid var(--border-color);
            background: var(--bg-secondary);
            display: flex;
            justify-content: space-between;
            align-items: center;
            font-size: 0.7rem;
            color: var(--text-muted);
        }

        footer a {
            color: var(--accent-cyan);
            text-decoration: none;
        }

        footer a:hover {
            text-decoration: underline;
        }

        /* Loading indicator */
        .loading {
            display: inline-block;
            width: 12px;
            height: 12px;
            border: 2px solid var(--border-color);
            border-top-color: var(--accent-cyan);
            border-radius: 50%;
            animation: spin 0.8s linear infinite;
            margin-left: 8px;
            vertical-align: middle;
        }

        @keyframes spin {
            to { transform: rotate(360deg); }
        }

        /* Notification Toast */
        .toast {
            position: fixed;
            bottom: 20px;
            right: 20px;
            background: var(--bg-secondary);
            border: 1px solid var(--border-color);
            padding: 15px 20px;
            font-size: 0.8rem;
            z-index: 999;
            transform: translateY(100px);
            opacity: 0;
            transition: all 0.3s ease;
        }

        .toast.visible {
            transform: translateY(0);
            opacity: 1;
        }

        .toast.success {
            border-color: var(--accent-green);
        }

        .toast.error {
            border-color: var(--accent-red);
        }
    </style>
</head>
<body>
    <div class="container">
        <header>
            <div class="logo">
                <div class="logo-icon">⚡</div>
                <div>
                    <h1>INTERCOM CTRL</h1>
                    <span>ESP8266 // DOOR AUTOMATION</span>
                </div>
            </div>
            <div class="header-info">
                <div>IP: <span class="ip" id="localip">---.---.---.---</span></div>
                <div class="mac" id="mac">MAC: --:--:--:--:--:--</div>
            </div>
        </header>

        <div class="main-grid">
            <!-- Status Panel -->
            <div class="panel">
                <div class="panel-header">System Status</div>
                <div class="panel-content">
                    <div class="status-grid">
                        <div class="status-item" id="state-container">
                            <div class="status-label">State</div>
                            <div class="status-value" id="state">IDLE</div>
                        </div>
                        <div class="status-item" id="bell-container">
                            <div class="status-label">Doorbell</div>
                            <div class="status-value" id="bell">INACTIVE</div>
                        </div>
                        <div class="status-item" id="door-container">
                            <div class="status-label">Door Relay</div>
                            <div class="status-value" id="door">CLOSED</div>
                        </div>
                        <div class="status-item" id="talk-container">
                            <div class="status-label">Talk Relay</div>
                            <div class="status-value" id="talk">IDLE</div>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Control Panel -->
            <div class="panel">
                <div class="panel-header">Manual Controls</div>
                <div class="panel-content">
                    <div class="control-grid">
                        <button class="btn primary" id="btn-door">
                            🔓 Unlock Door
                        </button>
                        <button class="btn" id="btn-talk">
                            🎤 Talk
                        </button>
                        <button class="btn warning full-width" id="btn-simulator">
                            ⚡ Run Test Sequence
                        </button>
                    </div>
                </div>
            </div>

            <!-- System Panel -->
            <div class="panel">
                <div class="panel-header">System</div>
                <div class="panel-content">
                    <div class="system-grid">
                        <button class="btn" id="btn-refresh">
                            Refresh
                        </button>
                        <button class="btn" id="btn-debug">
                            Debug
                        </button>
                        <button class="btn" id="btn-reset">
                            Soft Reset
                        </button>
                        <button class="btn danger" id="btn-hardreset">
                            Hard Reset
                        </button>
                        <button class="btn" id="btn-poll">
                            Auto-Poll: OFF
                        </button>
                    </div>
                </div>
            </div>

            <!-- Debug Info Panel -->
            <div class="panel">
                <div class="panel-header">Debug Info</div>
                <div class="panel-content">
                    <div class="status-grid">
                        <div class="status-item">
                            <div class="status-label">Debug Mode</div>
                            <div class="status-value" id="debug">OFF</div>
                        </div>
                        <div class="status-item">
                            <div class="status-label">Last Update</div>
                            <div class="status-value" id="lastUpdate">--:--:--</div>
                        </div>
                    </div>
                </div>
            </div>

            <!-- Terminal Panel -->
            <div class="panel terminal-panel">
                <div class="panel-header">Serial Terminal</div>
                <div class="panel-content" style="padding: 0;">
                    <div class="terminal-toggle" id="terminal-toggle">
                        <span class="terminal-toggle-text">
                            <span>WiFi Terminal</span>
                        </span>
                        <span class="terminal-toggle-icon"></span>
                    </div>
                    <div class="terminal-container" id="terminal-container">
                        <iframe id="terminal" src="about:blank"></iframe>
                    </div>
                </div>
            </div>
        </div>

        <footer>
            <div>INTERCOM CTRL v2.0 // Elfege Leylavergne // Smart Home Automation</div>
            <div>
                <a href="https://github.com/elfege" target="_blank">GitHub</a>
            </div>
        </footer>
    </div>

    <!-- Hard Reset Confirmation Modal -->
    <div class="modal-overlay" id="modal-hardreset">
        <div class="modal">
            <div class="modal-header">
                <div class="modal-header-icon">⚠</div>
                <h3>Confirm Hard Reset</h3>
            </div>
            <div class="modal-body">
                <p>This will perform a <strong>full hardware reset</strong> of the ESP8266, clearing all memory including RTC data.</p>
                <p style="margin-top: 12px;">Use this after OTA updates if the device is behaving unexpectedly, or if a soft reset doesn't resolve issues.</p>
            </div>
            <div class="modal-footer">
                <button class="btn" id="modal-cancel">Cancel</button>
                <button class="btn danger" id="modal-confirm">Hard Reset</button>
            </div>
        </div>
    </div>

    <!-- Toast Notification -->
    <div class="toast" id="toast"></div>

    <script src="https://cdn.jsdelivr.net/npm/axios/dist/axios.min.js"></script>
    <script>
        // State
        let localIp = null;
        let pollInterval = null;
        let isPollActive = false;

        // DOM Elements
        const $ = (sel) => document.querySelector(sel);
        const $$ = (sel) => document.querySelectorAll(sel);

        // Toast notification
        const showToast = (message, type = 'success') => {
            const toast = $('#toast');
            toast.textContent = message;
            toast.className = `toast ${type} visible`;
            setTimeout(() => toast.classList.remove('visible'), 3000);
        };

        // API call helper
        const apiCall = async (endpoint) => {
            try {
                const response = await axios.get(`/${endpoint}`);
                return response.data;
            } catch (err) {
                console.error(`API Error (${endpoint}):`, err.message);
                showToast(`Error: ${endpoint} failed`, 'error');
                throw err;
            }
        };

        // Update status displays
        const updateStatus = (key, value) => {
            const el = $(`#${key}`);
            if (!el) return;

            el.textContent = value.toUpperCase();

            // Update container styling based on state
            const container = $(`#${key}-container`);
            if (container) {
                container.classList.remove('active', 'warning', 'alert', 'state-idle', 'state-operating', 'state-terminating');

                if (key === 'state') {
                    if (value.toLowerCase() === 'idle') {
                        container.classList.add('state-idle');
                    } else if (value.toLowerCase().includes('operating')) {
                        container.classList.add('state-operating', 'warning');
                    } else if (value.toLowerCase().includes('terminating')) {
                        container.classList.add('state-terminating', 'warning');
                    } else {
                        container.classList.add('warning');
                    }
                } else if (key === 'bell') {
                    if (value.toLowerCase() === 'active') {
                        container.classList.add('alert');
                    }
                } else if (key === 'door' && value.toLowerCase() === 'opening') {
                    container.classList.add('active');
                } else if (key === 'talk' && value.toLowerCase() === 'talking') {
                    container.classList.add('active');
                }
            }
        };

        // Fetch and update all states
        const fetchStates = async () => {
            try {
                const data = await apiCall('getstates');
                const pairs = data.split(',').filter(s => s.trim());

                pairs.forEach(pair => {
                    const [key, ...valueParts] = pair.trim().split(' ');
                    const value = valueParts.join(' ');
                    if (key && value) {
                        updateStatus(key, value);
                    }
                });

                $('#lastUpdate').textContent = new Date().toLocaleTimeString();
            } catch (err) {
                // Error already shown via toast
            }
        };

        // Fetch device info
        const fetchDeviceInfo = async () => {
            try {
                const ip = await apiCall('getip');
                localIp = ip;
                $('#localip').textContent = ip;
            } catch (err) {}

            try {
                const mac = await apiCall('getmac');
                $('#mac').textContent = `MAC: ${mac}`;
            } catch (err) {}
        };

        // Toggle auto-polling
        const togglePoll = () => {
            isPollActive = !isPollActive;
            const btn = $('#btn-poll');

            if (isPollActive) {
                btn.textContent = 'Auto-Poll: ON';
                btn.classList.add('active');
                pollInterval = setInterval(fetchStates, 2000);
                showToast('Auto-polling enabled');
            } else {
                btn.textContent = 'Auto-Poll: OFF';
                btn.classList.remove('active');
                clearInterval(pollInterval);
                showToast('Auto-polling disabled');
            }
        };

        // Terminal toggle
        const toggleTerminal = () => {
            const toggle = $('#terminal-toggle');
            const container = $('#terminal-container');
            const iframe = $('#terminal');

            toggle.classList.toggle('open');
            container.classList.toggle('visible');

            if (container.classList.contains('visible') && localIp) {
                iframe.src = `http://${localIp}/term.html`;
            }
        };

        // Modal handling
        const showModal = (id) => {
            $(`#${id}`).classList.add('visible');
        };

        const hideModal = (id) => {
            $(`#${id}`).classList.remove('visible');
        };

        // Button handlers
        const handlers = {
            'btn-door': async () => {
                showToast('Sending door unlock command...');
                await apiCall('door');
                showToast('Door unlock triggered');
                fetchStates();
            },
            'btn-talk': async () => {
                showToast('Sending talk command...');
                await apiCall('talk');
                showToast('Talk triggered');
                fetchStates();
            },
            'btn-simulator': async () => {
                showToast('Starting test sequence...');
                await apiCall('simulator');
                showToast('Test sequence initiated');
                fetchStates();
            },
            'btn-refresh': async () => {
                showToast('Refreshing...');
                await apiCall('refresh');
                await fetchStates();
                showToast('Refresh complete');
            },
            'btn-debug': async () => {
                await apiCall('debug');
                await fetchStates();
                showToast('Debug mode toggled');
            },
            'btn-reset': async () => {
                showToast('Soft resetting...');
                await apiCall('reset');
                showToast('Soft reset sent - reconnecting...');
            },
            'btn-hardreset': () => {
                showModal('modal-hardreset');
            },
            'btn-poll': togglePoll,
            'terminal-toggle': toggleTerminal,
            'modal-cancel': () => hideModal('modal-hardreset'),
            'modal-confirm': async () => {
                hideModal('modal-hardreset');
                showToast('Hard resetting...');
                await apiCall('hardreset');
                showToast('Hard reset sent - device rebooting...');
            }
        };

        // Initialize
        document.addEventListener('DOMContentLoaded', () => {
            // Attach event handlers
            Object.keys(handlers).forEach(id => {
                const el = $(`#${id}`);
                if (el) {
                    el.addEventListener('click', handlers[id]);
                }
            });

            // Close modal on overlay click
            $('#modal-hardreset').addEventListener('click', (e) => {
                if (e.target.id === 'modal-hardreset') {
                    hideModal('modal-hardreset');
                }
            });

            // Initial data fetch
            fetchDeviceInfo();
            fetchStates();
        });
    </script>
</body>
</html>
)=====";