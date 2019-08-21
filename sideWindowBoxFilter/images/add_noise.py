import numpy as np
import random
import cv2

# reference: https://blog.csdn.net/KimLK/article/details/78261809
def saltpepper(img,n):
    m=int((img.shape[0]*img.shape[1])*n)
    for a in range(m):
        i=int(np.random.random()*img.shape[1])
        j=int(np.random.random()*img.shape[0])
        if img.ndim==2:
            img[j,i]=255
        elif img.ndim==3:
            img[j,i,0]=255
            img[j,i,1]=255
            img[j,i,2]=255
    for b in range(m):
        i=int(np.random.random()*img.shape[1])
        j=int(np.random.random()*img.shape[0])
        if img.ndim==2:
            img[j,i]=0
        elif img.ndim==3:
            img[j,i,0]=0
            img[j,i,1]=0
            img[j,i,2]=0
    return img

if __name__=="__main__": 
    img = 'dragonball_small.jpg'
    sp_noise = 'dragonball_small_salt_pepper_noise.jpg'

    image = cv2.imread(img)
    result = saltpepper(image, 0.05)

    cv2.imwrite(sp_noise, result)
    cv2.imshow('fake', result)
    cv2.waitKey()