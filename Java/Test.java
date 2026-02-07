public class Test {
	static final SFADiameter.Point[] POINTS = new SFADiameter.Point[] { //
			new SFADiameter.Point(0, 0), //
			new SFADiameter.Point(0.5001f, 0.8661f), //
			new SFADiameter.Point(0.5424f, 0.8399f), //
			new SFADiameter.Point(-0.5001f, 0.8661f), //
			new SFADiameter.Point(-0.5424f, 0.8399f), //
	};

	static final float EPSILON = 0.1f;
	
	public static void main(String[] args) {
		SFADiameter d = new SFADiameter(POINTS, EPSILON);
		float approxDiameter = d._resultDiameter;

		System.out.println("Approx diameter = " + d._resultDiameter);
		System.out.println(d._resultEndpointA);
		System.out.println(d._resultEndpointB);
		System.out.println();

		d = new SFADiameter(POINTS);
		float exactDiameter = d._resultDiameter;
		
		System.out.println("Exact diameter = " + d._resultDiameter);
		System.out.println(d._resultEndpointA);
		System.out.println(d._resultEndpointB);
		System.out.println();
		
		System.out.println("eps = " + EPSILON);
		
		float err;
		if (exactDiameter == 0)
			err = 0;
		else 
			err = 1 - approxDiameter / exactDiameter;
		System.out.println("err = " + err);		
	}
}