let port, reader;
const connectBtn = document.getElementById('connectBtn');
const disconnectBtn = document.getElementById('disconnectBtn');
const downloadBtn = document.getElementById('downloadBtn');
const darkToggle = document.getElementById('darkToggle');
const statLog = document.getElementById('log');
const ctxDiv = document.getElementById('context');
const gasDiv = document.getElementById('gas');
const micDiv = document.getElementById('mic');
const luxDiv = document.getElementById('lux');
const tempDiv = document.getElementById('temp');
const humDiv = document.getElementById('hum');
const bmptDiv = document.getElementById('bmpt');

let csv = [['timestamp','context','gas','mic','lux','temp','hum','bmpTemp']];

connectBtn.addEventListener('click', async () => {
  try {
    port = await navigator.serial.requestPort();
    await port.open({ baudRate: 9600 });
    connectBtn.disabled = true; disconnectBtn.disabled = false;
    const textDecoder = new TextDecoderStream();
    port.readable.pipeTo(textDecoder.writable);
    reader = textDecoder.readable.getReader();
    while (true) {
      const { value, done } = await reader.read();
      if (done) break;
      if (value) handleSerial(value);
    }
  } catch (err) {
    log('Error: ' + err);
  }
});

disconnectBtn.addEventListener('click', async () => {
  if (reader) { await reader.cancel(); reader = null; }
  if (port) { await port.close(); port = null; }
  connectBtn.disabled = false; disconnectBtn.disabled = true;
  log('Disconnected');
});

downloadBtn.addEventListener('click', () => {
  const csvContent = csv.map(r => r.join(',')).join('\n');
  const blob = new Blob([csvContent], {type: 'text/csv'});
  const url = URL.createObjectURL(blob);
  const a = document.createElement('a');
  a.href = url; a.download = 'ambientdata.csv'; a.click();
  URL.revokeObjectURL(url);
});

darkToggle.addEventListener('change', () => {
  document.body.classList.toggle('dark', darkToggle.checked);
});

function log(s) {
  statLog.textContent += s + '\n';
  statLog.scrollTop = statLog.scrollHeight;
}

function handleSerial(chunk) {
  log(chunk.trim());
  const lines = chunk.split(/\r?\n/);
  let state = {};
  lines.forEach(line => {
    if (!line) return;
    if (line.startsWith('Context:')) {
      const val = line.split(':').slice(1).join(':').trim();
      ctxDiv.textContent = val; state.context = val;
    } else if (line.startsWith('Gas:')) {
      const v = line.split(':')[1].trim(); gasDiv.textContent = v; state.gas = v;
    } else if (line.startsWith('Mic:')) {
      const v = line.split(':')[1].trim(); micDiv.textContent = v; state.mic = v;
    } else if (line.startsWith('Lux:')) {
      const v = line.split(':')[1].trim(); luxDiv.textContent = v; state.lux = v;
    } else if (line.startsWith('Temp:')) {
      const v = line.split(':')[1].trim(); tempDiv.textContent = v; state.temp = v;
    } else if (line.startsWith('Hum:')) {
      const v = line.split(':')[1].trim(); humDiv.textContent = v; state.hum = v;
    } else if (line.startsWith('BMP Temp:')) {
      const v = line.split(':')[1].trim(); bmptDiv.textContent = v; state.bmpTemp = v;
    }
  });
  // Append CSV row if context present
  if (state.context) {
    const now = new Date().toISOString();
    csv.push([now, state.context || '', state.gas || '', state.mic || '', state.lux || '', state.temp || '', state.hum || '', state.bmpTemp || '']);
  }
}
