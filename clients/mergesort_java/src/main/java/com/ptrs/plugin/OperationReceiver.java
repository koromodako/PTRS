package com.ptrs.plugin;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import com.google.gson.JsonParseException;
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
			System.err.println("An error occured while reading user's standard input : " + e.getMessage());
		}
		
		if(action == null) {
			System.err.println("Program exited before any command was given");
			System.exit(1);
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
				jsonResult = Calculator.mergeSortFromJson(json);
				break;
			default:
				// Undefined action
				System.err.println("Undefined action, exiting. List of actions are split, calc, and join.");
				System.exit(1);
		}
		
		if(jsonResult != null) {
			System.out.println(jsonResult);
		}
		else {
			System.exit(1);
		}
		
		// Exit on success
		System.exit(0);
	}
}
