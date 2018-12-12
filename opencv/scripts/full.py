#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
import cv2 as cv
import sys

# Utility functions for dealing with contours
def get_cnt_details(cnt):
    M = cv.moments(cnt)
    details = None
    
    # if the moment is at point 00
    if (M['m00'] != 0):
        details = {}
        details['cx'] = cx = int(M['m10']/M['m00'])
        details['cy'] = cy = int(M['m01']/M['m00'])
        details['area'] = area = cv.contourArea(cnt)
        details['perimeter'] = perimeter = cv.arcLength(cnt,True)
        
        approx = cv.approxPolyDP(cnt,0.01*cv.arcLength(cnt,True),True)
        details['num_vertices'] = len(approx)
    else:
        return None

    if (details['area'] < 16):
        return None
    else:
        return details

def print_cnt_details(cnt):
    cnt_dict = get_cnt_details(cnt)
    
    if (cnt_dict is None):
        return
    
    cx = cnt_dict['cx']
    cy = cnt_dict['cy']
    area = cnt_dict['area']
    perimeter = cnt_dict['perimeter']
    num_vertices = cnt_dict['num_vertices']
    
    print('cx: ', cx, '\tcy: ', cy, '\tarea: ', area, '\tperimeter: ', perimeter, '\tvertices: ', num_vertices)

def cnt_is_circular(cnt):
    cnt_det = get_cnt_details(cnt)
    if (cnt_det is None):
        return False
    area = cnt_det['area']
    perimeter = cnt_det['perimeter']
    num_vertices = cnt_det['num_vertices']
    return True if (num_vertices < 15 and num_vertices > 4) else False

def cnt_is_board(cnt):
    cnt_det = get_cnt_details(cnt)
    if (cnt_det is None):
        return False
    area = cnt_det['area']
    num_vertices = cnt_det['num_vertices']
    approx = cv.approxPolyDP(cnt,0.01*cv.arcLength(cnt,True),True)
    return True if (area > 20000) else False

# Finds largest difference between elements in an array
#    Useful for finding if an RGB is black (similar RGB values) or not
def max_difference(xs):
    min_elem = xs[0]
    max_elem = xs[0]
    max_diff = -1

    for elem in xs[1:]:
        min_elem = min(elem, min_elem)
        if elem > max_elem:
            max_diff = max(max_diff, elem - min_elem)
            max_elem = elem

    return max_diff

# Useful for printing to stderr
def printerr(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)


# The following block will find the board contour and use that to help identify the contour of the hole in the cornhole board. 

# In[2]:


# Load original source
img = cv.imread('socpics/te0.png')

# Crop the image so we only observe the bottom half when finding contours
height, width, channels = img.shape

# Do algorithm grabCut(...) to separate foreground from background
mask = np.zeros(img.shape[:2], np.uint8)
bgdModel = np.zeros((1,65), np.float64)
fgdModel = np.zeros((1,65), np.float64)
rect = (100,200,height,width)
cv.grabCut(img, mask, rect, bgdModel, fgdModel, 5, cv.GC_INIT_WITH_RECT)

mask2 = np.where((mask==2)|(mask==0),0,1).astype('uint8')
img = img*mask2[:,:,np.newaxis]

# Find contours
graysrc = cv.cvtColor(img, cv.COLOR_BGR2GRAY)
ret,thresh = cv.threshold(graysrc, 127,255,0)
im2, contours, hierarchy = cv.findContours(thresh,cv.RETR_TREE, 1)

# Get board contour
board_cnt = None
for cnt in contours:
    if (cnt_is_board(cnt)):
        board_cnt = cnt

if (board_cnt is None):
    printerr("Could not find board contour")
    sys.exit(1)
else:
    print("board contour: "), print_cnt_details(board_cnt)
    
    

# The purpose of this block is to...
#    Create a mask of the board
#    Extract the board area to find the hole contour
#    Create a mask that doesn't include the hole contour

# Create mask for the board
mask = np.zeros(img.shape, np.uint8)
cv.drawContours(mask, [board_cnt], 0, (255,255,255), cv.FILLED) # draw the contour of the board in the empty mask

# Find contours within board
board_only = np.zeros(img.shape, np.uint8)
board_only[mask == 255] = img[mask == 255]
graysrc = cv.cvtColor(board_only, cv.COLOR_BGR2GRAY)
ret,thresh = cv.threshold(graysrc, 127,255,0)
im2, contours, hierarchy = cv.findContours(thresh,cv.RETR_TREE, 1)

# find the one that's the hole
hole_cnt  = None
hole_area = None
hole_cx = None
hole_cy = None
largest_area = -np.inf
board_dets = get_cnt_details(board_cnt)
board_area = board_dets['area']
for cnt in contours:
    dets = get_cnt_details(cnt)
    if (dets is None):
        continue
    if (dets['area'] > largest_area and dets['area'] != board_area):
        hole_cnt = cnt
        hole_area = dets['area']
        hole_cx = dets['cx']
        hole_cy = dets['cy']

if (hole_cnt is None):
    printerr("Could not find hole contour")
    sys.exit(1)
else:
    cv.drawContours(mask, [hole_cnt], -1, (0,0,0), cv.FILLED)
    print("hole contour: ")
    print_cnt_details(hole_cnt)


# In[3]:


# Isolate the board area in a new image with bags on it (excluding the hole area)
board_w_bags = cv.imread('./socpics/tr2b3.png')
board_area_isolated = board_w_bags & mask

board_masked_img = board_area_isolated
graysrc = cv.cvtColor(board_masked_img, cv.COLOR_BGR2GRAY)
ret,thresh = cv.threshold(graysrc, 200,255,3)
im2, contours, hierarchy = cv.findContours(thresh,cv.RETR_TREE, 1)

# Exclude the one with the biggest area (board contour)
biggest_area = -np.inf
cnt_idx = None
for idx,cnt in enumerate(contours):
    dets = get_cnt_details(cnt)
    if (dets is None):
        continue
    curr_area = dets['area']
    if (curr_area > biggest_area):
        cnt_idx = idx
        biggest_area = curr_area
if (cnt_idx is not None):
    del contours[cnt_idx]

# Remove the contour that was the cornhole board hole
hole_idx = None
for idx,cnt in enumerate(contours):
    dets = get_cnt_details(cnt)
    if (dets is None):
        continue
    cx = dets['cx']
    cy = dets['cy']
    if (cx in range(int(hole_cx)-12,int(hole_cx)+12) and cy in range(int(hole_cy)-10,int(hole_cy)+10)): # TODO!!! Give this some flexibility (+/- 15 pixels maybe)
        hole_idx = idx
if (hole_idx is not None):
    del contours[hole_idx]
    
# Remove extraneous contours (with "none" properties that lead 'get_cnt_details(...)' to return None)
contours = list(filter(lambda x: get_cnt_details(x) is not None, contours))


# In[6]:


bag_cnts = contours

# Make a mask for each bag and store them in an array
bag_masks = []
means = []
for cnt in bag_cnts:
    mask = np.zeros(board_w_bags.shape, np.uint8)
    cv.drawContours(mask, [cnt], 0, (255,255,255), cv.FILLED)
    masked_img = board_area_isolated & mask
    bag_masks.append(masked_img)
    meanval = cv.mean(masked_img)
    means.append(meanval)

# Find average color of each bag, if any R,G, or B significantly differ from one another, classify as red (non-black) 
num_red   = 0
num_black = 0
for bag_mask in bag_masks: # returns BGR instead of RGB
    avg_color_per_row = np.average(bag_mask, axis=0)
    avg_color = np.average(avg_color_per_row, axis=0)
    
    if (max_difference(avg_color) > .05):
        num_red = num_red + 1
    else:
        num_black = num_black + 1
        
print("num_red: ", num_red)
print("num_black: ", num_black)


# In[ ]:




