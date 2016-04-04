package com.ptrs.plugin;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import com.ptrs.operation.Calculator;
import com.ptrs.operation.Merger;
import com.ptrs.operation.Splitter;

public class OperationReceiver {
	
	private static final String END_OF_FILE = "EOF";

	public static void main(String[] args) {
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		
		String line = null, action = null, json = "";
		try {
			action = reader.readLine();
			if(action.equals(END_OF_FILE)) {
				System.out.println("Requested to end communication with EOF");
				System.exit(0);
			}
			while (!(line = reader.readLine()).equals(END_OF_FILE)) {
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
		action = action.toLowerCase();
		switch(action) {
			case Action.JOIN:
				jsonResult = Merger.mergeFromJson(json);
				break;
			case Action.SPLIT:
				jsonResult = Splitter.splitFromJson(json);
				break;
			case Action.COMPUTE:
				jsonResult = Calculator.mergeSortFromJson(json);
				break;
			case Action.GET_PARAMS:
				jsonResult = Action.getAcceptedParameters();
				break;
			default:
				// Undefined action
				System.err.println("Undefined action, exiting. List of actions are split, calc, join, and get_params.");
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
