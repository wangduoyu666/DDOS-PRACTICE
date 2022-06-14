
import requests
from bs4 import BeautifulSoup

url = 'https://xxxxxxxxxxx/'
reqs = requests.get(url)
soup = BeautifulSoup(reqs.text, 'html.parser')

urls = []
for link in soup.find_all('a'):
    print(link.get('href'))
    
#有用但是用处不大，个人感觉，如果你要扫后台文件，建议还是自已写
#或者用其他人的工具
