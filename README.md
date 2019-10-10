_____________________________________________________
Easton Kang (ekang05), Mico Theogene (tmicom01)
Professor Monroe
Comp-40
10/09/2019
_____________________________________________________

Acknowledgments: Colin Murphy

Implemented:
	Everything but uarray2b.c (map function) has been correctly implemented.
uarray2b map function only works on square (x * x) dimension images, and
fails if image is a rectangle

Architecture:
	The first layer of our image is a 1D UArray2_T object. This array stores
1D UArray2 objects, which are the blocks for blocked arrays. The UArray2
stores whatever the user gives in. 
	Also, the main in ppmtrans iterates through all the command lines, records
which were called, then runs them separately after the processing. This
includes all "-rotate/flip" commands, determining whether stdin or file is used, and
whether -time is called.

Part E:

CPU Specs:
 CPU Mhz: 879.699 MHz
 CPU Min: 800.00 MHz
 CPU Max: 3800.00 MHz

IMAGE:mobo.jpg

 Type of Process |  -major   |    Total time (s)   |  time/pixel (ns) |
________________________________________________________________
Rotated 0        | col-major | 1,502        .00 | 0.000030
Rotated 0        | row-major | 1,565        .00 | 0.000031

Rotated 90		 | col-major | 2.68             | 53.626968
Rotated 90		 | row-major | 2,028,995,687.00 | 40.629319

Rotated 180		 | col-major | 2,434,918,403.00 | 48.757657
Rotated 180      | row-major | 1,608,169,718.00 | 32.202553

Rotated 270      | col-major | 5,166,489,582.00 | 103.455594
Rotated 270      | row-major | 3,627,866,107.00 | 72.645659

Flip Horiz.      | col-major | 2,516,892,855.00 | 50.399142
Flip Horiz.      | row-major | 1,574,750,992.00 | 31.533364

Flip Verti.      | col-major | 2,536,972,097.00 | 50.801216
Flip Verti.      | row-major | 1,582,017,838.00 | 31.678878

Tranpose         | col-major | 5,499,121,847.00 | 110.116338
Tranpose         | row-major | 3,163,380,630.00 | 63.344640


IMAGE: from-wind-cave.jpg

 Type of Process |  -major   |    Total time    |  time/pixel  |
________________________________________________________________
Rotated 0        | col-major | 1,241        .00 | 0.003321
Rotated 0        | row-major | 1,858        .00 | 0.004972

Rotated 90       | col-major | 13,712,097   .00 | 36.690045
Rotated 90       | row-major | 11,137,608   .00 | 29.801374

Rotated 180      | col-major | 13,508,913   .00 | 36.146377
Rotated 180      | row-major | 11,527,225   .00 | 30.843889

Rotated 270      | col-major | 28,943,282   .00 | 77.444778
Rotated 270      | row-major | 24,531,545   .00 | 65.640105

Flip Horiz.      | col-major | 13,586,000   .00 | 36.352641
Flip Horiz.      | row-major | 12,407,540   .00 | 33.199386

Flip Verti.      | col-major | 14,602,628   .00 | 39.072877
Flip Verti.      | row-major | 13,297,277   .00 | 35.580093

Tranpose         | col-major | 28,464,676   .00 | 76.164151
Tranpose         | row-major | 22,479,539   .00 | 60.149464


IMAGE: James_square.jpg : Square Dimension (1800 * 1800)

 Type of Process |  -major   |    Total time (s)   |  time/pixel (s)  |
________________________________________________________________
Rotated 0        | col-major | 1.24 | 0.003321
Rotated 0        | row-major | 1,858        .00 | 0.004972
Rotated 0        | blk-major | 1,858        .00 | 0.004972

Rotated 90       | col-major | 13,712,097   .00 | 36.690045
Rotated 90       | row-major | 11,137,608   .00 | 29.801374
Rotated 90       | blk-major | 11,137,608   .00 | 29.801374

Rotated 180      | col-major | 13,508,913   .00 | 36.146377
Rotated 180      | row-major | 11,527,225   .00 | 30.843889
Rotated 180      | blk-major | 11,527,225   .00 | 30.843889

Rotated 270      | col-major | 28,943,282   .00 | 77.444778
Rotated 270      | row-major | 24,531,545   .00 | 65.640105
Rotated 270      | blk-major | 24,531,545   .00 | 65.640105

Flip Horiz.      | col-major | 13,586,000   .00 | 36.352641
Flip Horiz.      | row-major | 12,407,540   .00 | 33.199386
Flip Horiz.      | blk-major | 12,407,540   .00 | 33.199386

Flip Verti.      | col-major | 14,602,628   .00 | 39.072877
Flip Verti.      | row-major | 13,297,277   .00 | 35.580093
Flip Verti.      | blk-major | 13,297,277   .00 | 35.580093

Tranpose         | col-major | 28,464,676   .00 | 76.164151
Tranpose         | row-major | 22,479,539   .00 | 60.149464
Tranpose         | blk-major | 22,479,539   .00 | 60.149464


Explanation: 
    As we expected in our design Part D, the row-major is much faster than
col-major. However, strangely, Rotated 0 runs faster on col-major than
row-major. We can't explain why, but generally, row-major is faster
because we only have 1 for loop for row-major, and 2 for loops
in col-major in UArray2 map. Also, row-major maximizes spatial locality
since the data accessed is next to each other, unlike col-major which 
jumps to each col, which jumps around width elements everytime to move to next
value. Additionally, the program benefits from temporaly locality by calling
function by pointers. For instance, at method is called often, thus when it's
brought into cache, it will likely stay there because at() is constantly being
called.
    block-major is , since block-major did not work
as intended for rectangular images (Mentioned in Implemented). If you give
block-major a square image, it will probably be faster. However, if
block-major were to work as intended, we still predict it would be faster
than both col and row. 


Hours Spent: 50 Hours...