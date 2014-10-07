Preprocessing time: O(n log n)

	First row is row of indices (0 to n-1)
	For each subsequent row:
		Take two elements of previous row and compare
		Store the index

Query is constant time (although constant is rather large to be safe)

	Check for invalid inputs
	Check for special cases

	Take log(start - end) - 1 to find row
		Do not subtract 1 if it falls below 0
	
	Using the row, find the minimum on the interval [start, floor((start + 2^row)/(2^row))]
	Using the row, find the minimum on the interval [floor((end - 2^row)/(2^row)), end]

	Calculate the minimum of the preceding 2 results
