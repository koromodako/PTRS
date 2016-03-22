package com.ptrs.algorithm;

import java.util.Random;

public class MergeSortTest {
	
	public static void main(String[] args) {
		//smallArrayTest();
		//bigArrayTest();
		sortedArrayTest();
	}
	
	private static void smallArrayTest() {
		int[] arr = {6,3,2,4,5, 6, 7, 8, 9, 10};
		MergeSort.mergeSort(arr, arr, 0, arr.length);
		
		for(int i = 0; i < arr.length; i++) {
			System.out.print(arr[i] + ",");
		}
	}
	
	private static void bigArrayTest() {
		int numberOfEntries = 60000000;
		int[] arr = new int[numberOfEntries];
		Random randomNumbers = new Random();
		for (int i = 0; i < numberOfEntries; i++) {
		     arr[i] = randomNumbers.nextInt(numberOfEntries);
		}
		MergeSort.mergeSort(arr);
		
		System.out.println("Big array sorted ? : " + (MergeSort.isSorted(arr) ? "true" : "false"));
		
	}
	
	private static void sortedArrayTest() {
		int[] sorted = {4, 8, 9, 10, 10, 10, 15, 24, 98};
		MergeSort.mergeSort(sorted);
		
		for(int i = 0; i < sorted.length; i++) {
			System.out.print(sorted[i] + ",");
		}
	}
		
		

}
