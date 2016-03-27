package com.ptrs.algorithm;

import java.util.Random;

import com.google.gson.Gson;
import com.ptrs.operation.Calculator;
import com.ptrs.operation.Splitter;
import com.ptrs.util.CalculationBlock;

public class MergeSortTest {
	
	public static void main(String[] args) {
		//smallArrayTest();
		//bigArrayTest();
//		sortedArrayTest();
//		mergeArraysTest();
//		splitTest();
		mergeSortFromJsonTest();
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
	
	private static void mergeArraysTest() {
		int[] firstHalf = {1, 2, 3, 4, 5, 6};
		int[] secondHalf = {2, 4, 6, 8, 9, 10};
		
		int[] merged = MergeSort.merge(firstHalf, secondHalf);
		
		for(int i = 0; i < merged.length; i++) {
			System.out.print(merged[i] + ",");
		}
		
		System.out.println();
		
		merged = MergeSort.merge(secondHalf, firstHalf);
		for(int i = 0; i < merged.length; i++) {
			System.out.print(merged[i] + ",");
		}
	}
	
	private static void splitTest() {
		Gson gson = new Gson();
		int[] arrayToSplit = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		CalculationBlock cb = new CalculationBlock(1, arrayToSplit, 11);
		
		String json = gson.toJson(cb);
		System.out.println(json);
		
		String jsonResult = Splitter.splitFromJson(json);
		System.out.println(jsonResult);
	}
	
	private static void mergeSortFromJsonTest() {
		Gson gson = new Gson();
		int[] arr = {6,3,2,4,5, 6, 7, 8, 9, 10};
		CalculationBlock cb = new CalculationBlock(1, arr, 11);
		
		String json = gson.toJson(cb);
		System.out.println(json);
		
		String jsonResult = Calculator.mergeSortFromJson(json);
		System.out.println(jsonResult);
	}
		
		

}
