#!/usr/bin/env python3

import http.client
import json

data = { "appTitle": "Remote UI Test", "description": "Contents", "appId": 2 }
jsonData = json.dumps(data)

headers = {"Accept": "*/*", "Accept-Encoding": "gzip, deflate", "Content-type": "text/plain"}

conn = http.client.HTTPConnection("192.168.0.34:5000")
conn.request("POST", "/", jsonData, headers)
res = conn.getresponse()

print(res.read().decode())