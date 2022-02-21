#!/bin/bash
curl --include --no-buffer --header "Host: 192.168.0.224:4444" --header "Upgrade: websocket" --header "Connection: Upgrade" --header "Sec-WebSocket-key:dGhlIHNhbXBsZSBub25jZQ==" --header "Origin:192.168.0.224:4444" --header "Sec-WebSocket-Protocol: chat, superchat" --header "Sec-WebSocket-Version: 13" 192.168.0.224:4444
