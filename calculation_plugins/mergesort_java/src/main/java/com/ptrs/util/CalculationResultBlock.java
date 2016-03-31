package com.ptrs.util;

public class CalculationResultBlock {
	
	private int fragmentId;
	private int[] result;
	
	public CalculationResultBlock(int fragmentId, int[] result) {
		super();
		this.fragmentId = fragmentId;
		this.result = result;
	}

	public int getFragmentId() {
		return fragmentId;
	}

	public void setFragmentId(int fragmentId) {
		this.fragmentId = fragmentId;
	}

	public int[] getResult() {
		return result;
	}

	public void setResult(int[] result) {
		this.result = result;
	}
}
