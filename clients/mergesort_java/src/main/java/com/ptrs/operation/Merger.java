package com.ptrs.operation;

import java.util.Iterator;
import java.util.PriorityQueue;
import java.util.Queue;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonParser;
import com.ptrs.algorithm.MergeSort;
import com.ptrs.util.CalculationResultBlock;

public class Merger {
	
	private static final Gson gson = new GsonBuilder()
		    .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
		    .create();
	
	public static String mergeFromJson(String json) {
		JsonArray resultsArray = new JsonParser().parse(json).getAsJsonArray();
		
		Queue<Entry> queue = new PriorityQueue<>();
		
		int finalSize = 0;
		for(int i = 0; i < resultsArray.size(); i++) {
			JsonElement resultJson = resultsArray.get(i);
			
			CalculationResultBlock calculationResultBlock = gson.fromJson(resultJson, CalculationResultBlock.class);
			int[] sortedArray = calculationResultBlock.getResult();
			
			if(sortedArray.length > 0) {
				queue.add(new Entry(sortedArray));
				finalSize += sortedArray.length;
			}
		}
		
		int[] finalSortedArray = new int[finalSize];
		int i = 0;
		
		while(!queue.isEmpty()) {
			Entry entry = queue.poll();
			int value = entry.getArrayValue();
			
			finalSortedArray[i++] = value;
			
			if (entry.readNext()) {
				queue.add(entry);
			}
			
		}
		
		return gson.toJson(new CalculationResultBlock(0, finalSortedArray));
	}
	
	private static int[] mergeTwoJsonElements(JsonElement firstResultJson, JsonElement secondResultJson) {
		CalculationResultBlock calculationFirstResultBlock = gson.fromJson(firstResultJson, CalculationResultBlock.class);
		int[] firstResult = calculationFirstResultBlock.getResult();
		
		CalculationResultBlock calculationSecondResultBlock = gson.fromJson(firstResultJson, CalculationResultBlock.class);
		int[] secondResult = calculationSecondResultBlock.getResult();
		
		return MergeSort.merge(firstResult, secondResult);
	}
	
	private static class Entry implements Comparable<Entry> {
		private int[] array;
		private int index;

		public Entry(int[] array) {
			this.array = array;
			this.index = 0;
		}

		public int[] getArray() {
			return array;
		}
		
		public int getArrayValue() {
			return array[index];
		}

		public boolean readNext() {
			if(index >= array.length - 1) {
				return false;
			}
			index++;
			return true;
		}
		
		@Override
		public int compareTo(Entry entry) {
			return this.array[index] - entry.array[index];
		}
	}
}
