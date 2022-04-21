const helloModule = require('hello-world');

window.addEventListener('DOMContentLoaded', () => {
  const replaceText = (selector, text) => {
    const element = document.getElementById(selector)
    if (element) element.innerText = text
  }

  for (const type of ['chrome', 'node', 'electron']) {
    replaceText(`${type}-version`, process.versions[type])
  }
});

setInterval(() => {
  helloModule.helloAsync(message => {
    const output = document.getElementById('output');

    const div = document.createElement('div');
    div.textContent = message;

    output.appendChild(div);
  });
}, 3000);
