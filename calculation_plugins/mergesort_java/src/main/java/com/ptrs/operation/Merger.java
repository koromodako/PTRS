package com.ptrs.operation;

import java.util.PriorityQueue;
import java.util.Queue;

import com.google.gson.FieldNamingPolicy;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonArray;
import com.google.gson.JsonElement;
import com.google.gson.JsonParseException;
import com.google.gson.JsonParser;
import com.ptrs.util.CalculationResultBlock;

public class Merger {
	
	private static final Gson gson = new GsonBuilder()
		    .setFieldNamingPolicy(FieldNamingPolicy.LOWER_CASE_WITH_UNDERSCORES)
		    .create();
	
	public static String mergeFromJson(String json) {
		JsonElement jsonElement = null;
		try {
			jsonElement = new JsonParser().parse(json);
		} 
		catch (JsonParseException e) {
			System.err.println("Misformed JSON, can't parse it : " + e.getMessage());
			return null;
		}
		
		if(jsonElement == null || !jsonElement.isJsonArray()) {
			System.err.println("Misformed JSON, can't further proceed fragment : " + json);
			return null;
		}
		
		JsonArray resultsArray = jsonElement.getAsJsonArray();
		
		Queue<Entry> queue = new PriorityQueue<>();
		
		int finalSize = 0;
		for(int i = 0; i < resultsArray.size(); i++) {
			JsonElement resultJson = resultsArray.get(i);
			
			CalculationResultBlock calculationResultBlock = gson.fromJson(resultJson, CalculationResultBlock.class);
			if(calculationResultBlock == null || calculationResultBlock.getResult() == null) {
				System.err.println("Unexpected structure for the calculcation result block, skipping fragment");
				continue;
			}
			
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
	
	/**
	 * Nested class to provide an abstraction for the min-heap implementation, represents an entry with an array and index associated
	 * Enables the min-heap to go through the array a step at a time
	 * @author jonathan
	 *
	 */
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
			return this.array[index] - entry.getArrayValue();
		}
	}
}
