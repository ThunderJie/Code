import requests
from bs4 import BeautifulSoup
import datetime

#玄武推送
url = "https://sec.today/pulses/#"
#安全客
url1 = "https://www.anquanke.com/knowledge"
#先知社区
url2 = "https://xz.aliyun.com/"

li = []
li2 = []
li3 = []
li4 = []
li5 = []

def get_content(title,s,mode):
    data = soup.find_all(title,class_ = s) # 获取需要的数据
    for j in data:
        if(j.text == "\n安全知识\n"):
            continue
        else:
            if(mode == 1):
                li.append("标题:  " + j.text)  # 玄武
            elif(mode == 2):
                li.append("标题:  " + j.text[2:].strip('\n')) # 安全客
            elif(mode == 3):
                li.append("标题:  " + j.text[9:].strip('\n'))  # 安全客

def get_content1(title,s):
    data = soup.find_all(title,class_ = s) # 获取需要的数据
    for j in data:
        li2.append("描述:  " + j.text) # 添加进列表

def get_content2(title,s):
    data = soup.find_all(title,class_ = s) # 获取需要的数据
    for j in data:
        li5.append("描述:  " + j.text) # 添加进列表

def get_web_xuanwu():
    for k in soup.find_all('a'):
        if(k['href'] == '/login/'):
            continue
        elif (k['href'] == '#'):
            continue
        elif (k['href'] == '/pulses/'):
            continue
        elif (k['href'] == '/apps/'):
            continue
        elif (k['href'] == '/humans/'):
            continue
        elif (k['href'] == '/'):
            continue
        elif (k['href'] == '/about/site'):
            continue
        elif (k['href'] == 'https://xlab.tencent.com'):
            continue
        elif (k['href'] == '/events/'):
            continue
        elif (k['href'][:6] == '/user/'):
            continue
        else:
            li3.append('网址:  https://sec.today' + k['href'] + '\n')

def get_web_anquanke():
    for k in soup.find_all('a',attrs={"target": "_blank"}):
        if (k['href'][:9] == '/post/id/'):
                li2.append('网址:  https://www.anquanke.com' + k['href'][:15] + '\n')

def get_web_xianzhi():
    for k in soup.find_all('a', attrs={"class": "topic-title"}):
        li2.append('网址:  https://xz.aliyun.com' + k['href'] + '\n')

try:
    # 玄武每日推送
    contents = requests.get(url)
    contents.encoding = "utf-8"
    text = contents.text  # 获取源码
    soup = BeautifulSoup(text, "html.parser")
    get_content('a',"text-dark",1) # 文章标题
    get_content1('q',"")
    get_web_xuanwu()
    li4.append("{}年{}月{}日安全推送      --By Thunder_J".format(datetime.datetime.now().year,datetime.datetime.now().month,datetime.datetime.now().day))
    li4.append("玄武每日推送:" + '\n')
    for i in range(0,20):
        li4.append(li[i])
        li4.append(li2[i])
        li4.append(li3[i])

    li = []
    li2 = []
    li3 = []

    # 安全客每日推送
    li4.append("安全客每日推送:" + '\n')
    contents = requests.get(url1)
    contents.encoding = "utf-8"
    text = contents.text  # 获取源码
    soup = BeautifulSoup(text, "html.parser")

    get_content('div',"title",2) # 文章标题
    get_content2('div','desc hide-in-mobile-device')

    get_web_anquanke()
    [li3.append(i) for i in li2 if not i in li3]

    for i in range(0,10):
        li4.append(li[i])
        li4.append(li5[i])
        li4.append(li3[i])

    # 先知社区每日推送
    li = []
    li2 = []
    li3 = []
    li5 = []

    li4.append("先知社区每日推送:" + '\n')
    contents = requests.get(url2)
    contents.encoding = "utf-8"
    text = contents.text  # 获取源码
    soup = BeautifulSoup(text, "html.parser")
    get_content('a', "topic-title", 3)  # 文章标题
    get_web_xianzhi()

    for i in range(0, 10):
        li4.append(li[i])
        li4.append(li2[i])

    with open('news.txt','a',encoding='gbk') as f:
        for i in li4:
            f.write(i + '\n')

except:
    print("爬取失败")