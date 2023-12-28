# Data_Compressing_LZW
Implemented Lempel-Ziv-Welch data comrpession technique in this one to compress a text file comtaining 1501 bytes, which ultimately gets compressed to 883 bytes.

## Overview
LZW is a compression algorithm that is table-based lookup algorithm to remove duplicate data and compress original file to a smaller size.

In this I am reading data from a file named 'Sample.txt' and then using LZW algorithm compressing it and storing compressed data to 'Sample.lwza' file.

## Key features:
- Used maps as dictionary for storing new ascii(likewise ascii as ascii>255 are assumed) codes
- Bits Processed are bits given to each character
- I have added comments to all parts for clear explanation

## Flaw

At higher data length, it sometimes collapse.
