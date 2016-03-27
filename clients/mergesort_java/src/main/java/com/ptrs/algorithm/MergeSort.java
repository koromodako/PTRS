package com.ptrs.algorithm;

public class MergeSort {
	
	/**
	 * Sort array src by using a mergesort algorithm. src will represent the sorted array after the execution of the method
	 * @param src
	 */
	public static void mergeSort(int[] src) {
		mergeSort(src, src, 0, src.length);
	}
	
	/**
	 * Sort array src into dest, within the range starting at low included and finishing at high excluded [low, high[. 
	 * General usage is to pass in the same array as src and dest.
	 * @param src The source array that needs to be sorted
	 * @param dest The resulting array sorted with the mergesort algorithm
	 * @param low The start index from which to sort the src array
	 * @param high The end index to which the src array needs to be sorted
	 */
	public static void mergeSort(int[] src, int[] dest, int low, int high) {
		int length = high - low;
		int dummy1, dummy2;

		// Insertion sort on smallest arrays, no need to go into unnecessary recursion
		if (length < 7) {
			for (int i = low; i < high; i++) {
				for (int j = i;j > low; j--) {
					dummy1 = dest[j-1];
					dummy2 = dest[j];
					if (dummy1 > dummy2) {
						swap(dest, j, j-1);
					}
				}
			}
			return;
		}

		// Recursively sort halves of dest into src
		int mid = (low + high) >>> 1; // Bit-shifting optimization : int mid = (low + high) / 2
		mergeSort(dest, src, low, mid);
		mergeSort(dest, src, mid, high);
		
		// Check values in the the first sorted half (mid - 1) and the first value in the second sorted half (mid)
		dummy1 = src[mid-1];
		dummy2 = src[mid];

		// If list is already sorted, just copy from src to dest. No need to go through the lists
		if (dummy1 <= dummy2) {
			System.arraycopy(src, low, dest, low, length);
			return;
		}

		// Merge sorted halves (now in src) into dest by browsing through both halves
		for (int i = low, p = low, q = mid; i < high; i++) {
			if (q < high && p < mid) {
				dummy1 = src[p];
				dummy2 = src[q];
			}
			if (q >= high || p < mid && dummy1 <= dummy2) {
				dest[i] = src[p++];
			}
			else {
				dest[i] = src[q++];
			}
		}
	}
	
	/**
	 * Merge two arrays that have already been sorted
	 * @param firstHalf A sorted array
	 * @param secondHalf A sorted array
	 * @return
	 */
	public static int[] merge(int[] firstHalf, int[] secondHalf) {
		int lastValueOfFirstHalf = firstHalf[firstHalf.length - 1];
		int firstValueOfSecondHalf = secondHalf[0];
		
		if(lastValueOfFirstHalf <= firstValueOfSecondHalf) {
			// TODO Returned merged array
			int ar[] = {1};
			return ar;
		}
		
		// TODO Fix size with both lengths
		int totalLength = firstHalf.length + secondHalf.length;
		int[] mergedArray = new int[totalLength];
		
		for (int i = 0, iFH = 0, iSH = 0; i < totalLength; i++) {
			if(iFH >= firstHalf.length || (iSH < secondHalf.length && firstHalf[iFH] > secondHalf[iSH])) {
				mergedArray[i] = secondHalf[iSH++];
			}
			else {
				mergedArray[i] = firstHalf[iFH++];
			}
		}
		
		return mergedArray;
	}
	
	/**
	 * Swap values at index i and j in array x
	 * @param x
	 * @param i
	 * @param j
	 */
	private static void swap(int[] x, int i, int j) {
		int t = x[i];
		x[i] = x[j];
		x[j] = t;
	}
	
	/**
	 * Check if arrayToCheck is sorted
	 * @param arrayToCheck
	 * @return
	 */
	public static boolean isSorted(int[] arrayToCheck) {
		for(int i = 0; i < arrayToCheck.length - 1; i++) {
			if(arrayToCheck[i] > arrayToCheck[i+1])
				return false;
		}
		return true;
	}

}
