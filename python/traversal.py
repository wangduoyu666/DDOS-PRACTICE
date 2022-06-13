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
       print(os.path.join(root, root))
      for name in dirs:
       print(os.path.join(root, name))
       for name in files:
        print(os.path.join(root, name))
        
