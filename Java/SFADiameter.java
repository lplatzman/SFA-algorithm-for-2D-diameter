/*
MIT License

Copyright (c) 2026 Loren Platzman

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

public class SFADiameter {

	// Input
	public final Point _inputPoints[];
	public final float _inputEpsilon;

	// Results
	public Point _resultEndpointA, _resultEndpointB;
	public float _resultDiameter;

	// Internal
	private int _n, _k, _m;
	private Point _hub;
	private Bin[] _sorter, _packer;
	private float _resultDiameter2;

	// Classes
	public static class Point {
		public final float _x, _y;

		public Point(float x, float y) {
			_x = x;
			_y = y;
		}

		public boolean equalsPoint(Point p) {
			return p._x == _x && p._y == _y;
		}

		public String toString() {
			return "Point(" + _x + ", " + _y + ")";
		}
	}

	private static class Bin {
		Point _point;
		float _r2, _theta;

		public String toString() {
			return "Bin(" + _point + ", " + _r2 + ", " + _theta + ")";
		}
	}

	// Utilities

	// Returns the square of the distance between points a and b.
	// This avoids taking square roots when comparing distances.
	private static float d2(Point a, Point b) {
		float dx = a._x - b._x;
		float dy = a._y - b._y;
		return dx * dx + dy * dy;
	}

	// Returns the angle of the line segment from a to b.
	// The angle ranges from 0 (right) to 2*pi (just before full circle).
	// This makes it possible to compare angles in the algorithms
	// without having to do modulo operations.
	private static float theta(Point a, Point b) {
		float atan = (float) Math.atan2(b._y - a._y, b._x - a._x);
		if (atan >= 0)
			return atan;
		else
			return atan + (float) (2 * Math.PI);
	}

	// Returns the index of the slice containing point p
	private int binIndex(Point p) {
		float theta = theta(_hub, p);
		return (int) Math.floor(_k * theta / Math.PI);
	}

	// Basic operation underlying the packer.
	// It is invoked from two places and so appears in its own function.
	private void addToPacker(Point p) {
		Bin b;
		float theta1, theta2 = 42;

		while (_m > 0) {
			b = _packer[_m - 1];
			theta1 = b._theta;
			theta2 = theta(b._point, p);
			if (theta2 > theta1)
				break;
			_m--;
		}

		b = new Bin();
		b._point = p;
		b._theta = theta2;
		_packer[_m++] = b;
	}

	// Accumulates pairs of points retaining the longest.
	private void apply(int i, int j) {
		Point a = _packer[i]._point;
		Point b = _packer[j]._point;
		float d2 = d2(a, b);

		if (d2 > _resultDiameter2) {
			_resultEndpointA = a;
			_resultEndpointB = b;
			_resultDiameter2 = d2;
		}
	}

	// Constructor / engine
	public SFADiameter(Point[] inputPoints, float inputEpsilon) {
		_inputPoints = inputPoints;
		_inputEpsilon = inputEpsilon;

		if (_inputPoints == null || _inputPoints.length == 0 || _inputEpsilon <= 1E-6 || _inputEpsilon >= 1)
			throw new IllegalArgumentException();

		_n = _inputPoints.length;
		_k = (int) Math.ceil(2 * Math.PI / _inputEpsilon);

		////////////////////////////////////////////////////
		// Compute hub

		_hub = _inputPoints[0];

		for (int i = 1; i < _n; i++) {
			Point p = _inputPoints[i];
			if (p._y > _hub._y)
				continue;
			if (p._y < _hub._y || p._x < _hub._x)
				_hub = p;
		}

		////////////////////////////////////////////////////
		// Sort (bin sort / each bin has a capacity of one)

		_sorter = new Bin[_k];

		for (Point p : _inputPoints) {
			float r2 = d2(_hub, p);
			if (r2 == 0)
				continue;

			int i = binIndex(p);
			if (_sorter[i] == null) {
				Bin bin = new Bin();
				bin._point = p;
				bin._r2 = r2;
				_sorter[i] = bin;
				continue;
			}

			Bin bin = _sorter[i];
			if (bin._r2 >= r2)
				continue;

			bin._point = p;
			bin._r2 = r2;
		}

		////////////////////////////////////////////////////
		// Pack (Graham scan)

		_packer = new Bin[_k + 3];

		Bin bin = new Bin();
		bin._point = _hub;
		bin._theta = -1E-5f;
		_packer[0] = bin;

		_m = 1;

		for (int i = 0; i < _k; i++) {
			bin = _sorter[i];
			if (bin != null)
				addToPacker(bin._point);
		}

		switch (_m) {
		case 1:
			_resultEndpointA = _resultEndpointB = _hub;
			_resultDiameter = 0;
			return;

		case 2:
			_resultEndpointA = _hub;
			_resultEndpointB = _packer[1]._point;
			_resultDiameter = (float) Math.sqrt(d2(_resultEndpointA, _resultEndpointB));
			return;

		default:
			break;
		}

		addToPacker(_hub);

		_packer[0]._theta = _packer[_m - 1]._theta - 2 * (float) Math.PI;

		bin = new Bin();
		bin._point = _packer[1]._point;
		bin._theta = _packer[1]._theta + 2 * (float) Math.PI;
		_packer[_m++] = bin;

		////////////////////////////////////////////////////
		// Measure (rotating calipers)

		int i = 0;
		float theta = _packer[1]._theta + (float) Math.PI;

		int j = 2;
		while (_packer[j + 1]._theta < theta) {
			j++;
		}

		apply(i, j);

		while (j < _m - 1) {
			if (_packer[i + 1]._theta + (float) Math.PI < _packer[j + 1]._theta) {
				i++;
			} else {
				j++;
			}
			apply(i, j);
		}

		_resultDiameter = (float) Math.sqrt(_resultDiameter2);
	}

	// Bonus: Brute force exact solution for comparison
	public SFADiameter(Point[] inputPoints) {
		_inputPoints = inputPoints;
		_inputEpsilon = 0;

		float trueDiameter2 = 0;

		for (int i = 0; i < _inputPoints.length; i++) {
			for (int j = 0; j < i; j++) {
				float d2 = d2(_inputPoints[i], _inputPoints[j]);
				if (d2 > trueDiameter2) {
					trueDiameter2 = d2;
					_resultEndpointA = _inputPoints[i];
					_resultEndpointB = _inputPoints[j];
				}
			}
		}

		_resultDiameter = (float) Math.sqrt(trueDiameter2);
	}
}
