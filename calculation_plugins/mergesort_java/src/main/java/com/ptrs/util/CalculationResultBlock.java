package com.ptrs.util;

public class CalculationResultBlock {
	
	private String fragmentId;
	private int[] result;
	
	public CalculationResultBlock(String fragmentId, int[] result) {
		super();
		this.fragmentId = fragmentId;
		this.result = result;
	}

	public String getFragmentId() {
		return fragmentId;
	}

	public void setFragmentId(String fragmentId) {
		this.fragmentId = fragmentId;
	}

	public int[] getResult() {
		return result;
	}

	public void setResult(int[] result) {
		this.result = result;
	}
}
