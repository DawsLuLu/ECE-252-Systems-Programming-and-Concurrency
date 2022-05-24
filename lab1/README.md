# Introduction to Systems Programming in Linux Computing Environment

The purpose of this lab is to introduce system programming in a general Linux De- velopment Environment at ECE Department. After finishing this lab, students will be able to 
apply basic Linux commands to interact with the Linux system through shell, 
apply standard Linux C programming tools for system programming and
create a program to interact with Linux file systems by applying the relevant system and libray calls.

## Problem Statement

You are given a directory under which some
files are PNG images and some files are not. The
directory may contain nested sub-directories2.
All valid PNG images under the given directory are horizontal strips of a bigger whole image. They all have the same width. The height of each image might be different. The PNG images have the naming convention of *_N.png,
where N is the image strip sequence number and
N=0, 1, 2, .... However a file with .png or
.PNG extension may not be a real PNG image
file. You need to located all the real PNG image files under the given directory first. Then
you will concatenate these horizontal strip images sequentially based on the sequence number in the file name to restore the original whole
image. The sequence number indicates the order the image should be concatenated from top
to bottom. For example, file img_1.png is the first horizontal strip and img_2.png is the second horizontal strip. To concatenate these two strips, the pixel data in img_1.png should be followed immediately by the pixel data in img_2.png file. Figure 1.1 illustrates the concatenation order. To solve the problem, first you will create a tool named pnginfo to identify if a given file is a png and to provide image details about that file. Second, you will create another command findpng to search the given directory hierarchy to find all the real PNG files under it. Finally you will create an image data concatenation tool named catpng to concatenate pixel data of a set of PNG files to form a single PNG image file. The catpng only processes PNG images with the same width in dimension.
