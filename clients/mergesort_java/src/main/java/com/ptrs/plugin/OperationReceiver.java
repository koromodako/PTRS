package com.ptrs.plugin;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import com.ptrs.operation.Calculator;
import com.ptrs.operation.Merger;
import com.ptrs.operation.Splitter;

public class OperationReceiver {
	
	private static final String ACTION_JOIN = "join";
	private static final String ACTION_SPLIT = "split";
	private static final String ACTION_COMPUTE = "calc";

	public static void main(String[] args) {
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		
		String line = null, action = null, json = "";
		try {
			action = reader.readLine();
			while ((line = reader.readLine()) != null) {
			    json += line;
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		String jsonResult = null;
		switch(action) {
			case ACTION_JOIN:
				jsonResult = Merger.mergeFromJson(json);
				break;
			case ACTION_SPLIT:
				jsonResult = Splitter.splitFromJson(json);
				break;
			case ACTION_COMPUTE:
				jsonResult = Calculator.mergeSortFromJson(null);
				break;
			default:
					
		}
		
		if(jsonResult != null) {
			System.out.println(jsonResult);
		}
		
		// Exit on success
		System.exit(0);
	}
}
