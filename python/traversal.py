import os
import os.path

"""
fullpath = os.listdir()
fp = input("enter your fullpath:")
pathname = os.open(fp)
if os.path.isdir(pathname):
    for pathname in fp:
        pn = os.scandir(pathname)
        print(os.listdir(pn))
else:
    print(os.path.basename(pathname))
 def tra()
 """

for root, dirs, files in os.walk("c:/Microsoft", topdown=True):
      for name in root:
       print(os.path.join(root, name))
      for name in dirs:
       print(os.path.join(root, name))
       for name in files:
        print(os.path.join(root, name))
        f = open("test.txt", "a")
        f.write(os.path.join(root, name))
        f.close()

 #python确实不错，除了运行速度上比起c稍微慢了点，其他的不错
 #2022.6.13
    

