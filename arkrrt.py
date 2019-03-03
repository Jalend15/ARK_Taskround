import random
import math
import copy
import cv2
import numpy as np

class RRT():
  def __init__(self,start,end,r):
    self.start=Node(start[0],start[1])
    self.end=Node(end[0],end[1])
    self.r=r
    self.nlist=[self.start]
    print start

    
  def traj(self,frame):
     self.frame=frame
     ar=Node(random.randrange(463,1451), random.randrange(49,1027))
     nin1=self.indexofNearest(self.nlist,ar)
     theta1=math.atan((self.nlist[nin1].y-ar.y/self.nlist[nin1].x-ar.x))
     ar12=Node(self.nlist[nin1].x+self.r*math.cos(theta1),self.nlist[nin1].y+self.r*math.sin(theta1))
     ar12.parent=nin1
     print ar12.x,ar12.y
     reso=0.1
     p=copy.deepcopy(self.nlist[nin1].x)
     q=copy.deepcopy(self.nlist[nin1].y)
     k=0
     while k<=self.r:
       p=self.nlist[nin1].x+k*(0.1*math.cos(theta1))
       q=self.nlist[nin1].y+k*0.1*math.sin(theta1)
       if(self.obstaclecheck(theta1,p,q)==0):
        print 0
        return 0
       k=k+self.r/20
       
       if self.goal_reach(p,q) :
         print "goal!"
         self.nlist.append(ar12)
         goal_print(ar12)
         return 1
       for i in range(0,3):
         #print 1
         if p<1080 and q<1027:
          self.frame[(int)(p),(int)(q)][i]=127
     self.nlist.append(ar12) 
    
       
  
  def goal_reach(self,x,y):
   if x<1080and y<1027: 
    if self.frame[int(x),int(y)][0]==0 and self.frame[int(x),int(y)][1]>250 and self.frame[int(x),int(y)][2]==0:
      self.goal=[x,y]
      return 1
    else:
      return 0


     
  def indexofNearest(self,points,a):
    min=10000000
    l=0
    for point in points:
      d=math.sqrt((point.x-a.x)**2+(point.y-a.y)**2)
      if(d<min):
       min=d
       l=points.index(point)
    return l

  def obstaclecheck(self,theta,x,y):
   reso=10
   for v in np.arange(0, self.r+1,reso):
     #print 0
     #print v
     #print int(x+v*math.cos(theta)),int(y+v*math.sin(theta))
     if self.frame==None:
       self.video1()
     if int(x+v*math.cos(theta))<1080 and int(y+v*math.sin(theta))<1027 and x!=None and y!=None and v!=None:  
      if self.frame[int(x+v*math.cos(theta)),int(y+v*math.sin(theta))][0]==0 and self.frame[int(x+v*math.cos(theta)),int(y+v*math.sin(theta))][1]==0 and self.frame[int(x+v*math.cos(theta)),int(y+v*math.sin(theta))][2]==0:
        print x, y
        return 0
      if self.frame[int(x+v*math.cos(theta)),int(y+v*math.sin(theta))][0]>=225 and self.frame[int(x+v*math.cos(theta)),int(y+v*math.sin(theta))][1]==0 and self.frame[int(x+v*math.cos(theta)),int(y+v*math.sin(theta))][2]==0:
        print x,y
        return 0
      
   return 1
  
  def goal_print(self,ar):
    while ar.parent!=None:
      k=0
      p=copy.deepcopy(ar.x)
      q=copy.deepcopy(ar.y)
      theta1=math.atan(q-self.nlist[ar.parent].y/p-self.nlist[ar.parent].x)
      while k<=self.r:
       p=p+k*math.cos(theta1)
       q=q+k*math.sin(theta1)
       for i in range(0,3):
         self.frame[int(p),int(q)][i]=127
       k=k+self.r/50
      ar=self.nlist[ar.parent]
      
  def video1(self):

   cap = cv2.VideoCapture('path.mkv')
   while(cap.isOpened()):
    ret, frame = cap.read()
  
    q=self.traj(frame)
    if q==1:
      
      break
    
    
    if ret==True:
     cv2.imshow('frame',frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):  #press q to exit
        break
   cap.release()
 
# Closes all the frames
   cv2.destroyAllWindows()

     
    
  
        
class Node():
  def __init__(self,x,y):
    self.x=x
    self.y=y
    self.parent=None
    
  
  


def main():
 start=[529,964]
 goal=[1395,307]
 r=500
 rrt=RRT(start,goal,r)

 cap = cv2.VideoCapture('path.mkv')
 while(cap.isOpened()):
    ret, frame = cap.read()
  
    q=rrt.traj(frame)
    if q==1:
      
      break
    
    
    if ret==True:
     cv2.imshow('frame',frame)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):  #press q to exit
        break
 cap.release()
 
# Closes all the frames
 cv2.destroyAllWindows()


if __name__ == '__main__':
    main()
