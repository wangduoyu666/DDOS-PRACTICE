import requests
r = requests.get('http://www.zomato.com/')
print("文本编码：", r.encoding)
print("响应状态码：", r.status_code)
print("字符串方式的响应体：", r.text)
print(r.headers)

#同样的，用处不大
#还是自已慢慢想吧
#2022.6.21
