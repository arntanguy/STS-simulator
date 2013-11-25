package Core;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.StringTokenizer;

import org.apache.commons.math.optimization.fitting.CurveFitter;
import org.apache.commons.math.optimization.fitting.ParametricRealFunction;
import org.apache.commons.math.optimization.fitting.WeightedObservedPoint;
import org.apache.commons.math.optimization.general.LevenbergMarquardtOptimizer;
import org.apache.commons.math.util.FastMath;


public class CurveFitting {

	private double[] initialGuess6 = new double[6];
	private double[] initialGuess5 = new double[5];

	private int startAt;

	public void setInitialGuess6(double[] ig) {
		initialGuess6 = ig;
	}

	public void setInitialGuess5(double[] ig) {
		initialGuess5 = ig;
	}
	
	public static void main(String[] args) {
		CurveFitting cf = new CurveFitting();
		double[] initialGuess6 = new double[6];
		initialGuess6[0] = 1.14;
		initialGuess6[1] = 0.08;
		initialGuess6[2] = 4;
		initialGuess6[3] = 5;
		initialGuess6[4] = 3;
		initialGuess6[5] = -1;
		cf.setInitialGuess6(initialGuess6);
		cf.setStartAt(100);
		cf.fit6("SW.dat", "fit.dat");
	}
	
	public void fit6(String inputFile, String outputFile) {
		try {
			LevenbergMarquardtOptimizer optimizer = new LevenbergMarquardtOptimizer();
			CurveFitter fitter = new CurveFitter(optimizer);
			ParametricRealFunction f = new f6();
			
			ArrayList<String> names = new ArrayList<String>();
			ArrayList<ArrayList<Double>> values = new ArrayList<ArrayList<Double>>();
			
			BufferedReader in = new BufferedReader(new FileReader(inputFile));
			String line = in.readLine();
			StringTokenizer st = new StringTokenizer(line);
			while (st.hasMoreTokens()) {
				names.add(st.nextToken());
				values.add(new ArrayList<Double>());
			}
			while ((line = in.readLine()) != null) {
				st = new StringTokenizer(line);
				int i = 0;
				double x = Double.parseDouble(st.nextToken());
				values.get(i++).add(x);
				while (st.hasMoreTokens()) { 
					double y = Double.parseDouble(st.nextToken());
					values.get(i++).add(y);
				}
			}

			System.out.println(names.size());
			for (int i = 0; i < names.size(); ++i) {
				String name = names.get(i);
				name = name.substring(1, name.length());
				if (name.length() > 0) {
					if (startAt == Integer.parseInt(name)) {
						startAt = i;
					}
				}
			}
			System.out.println(startAt);
				
				
			
			BufferedWriter out = new BufferedWriter(new FileWriter(outputFile));

			//to the right
			for (int i = startAt; i < values.size(); ++i) {
				for (int j = 0; j < values.get(i).size(); ++j) {
					double x = values.get(0).get(j);
					double y = values.get(i).get(j);
					fitter.addObservedPoint(x, y);
				}
				double[] fitParameters = fitter.fit(f, initialGuess6);
				System.out.print(names.get(i) + "\t");
				out.write(names.get(i) + "\t");
				for (Double parameter: fitParameters) {
					System.out.print(parameter + "\t");
					out.write(String.format("%e", parameter) + "\t");
				}
				System.out.println();
				out.write("\n");
				fitter.clearObservations();
				initialGuess6 = fitParameters;
			}
			
			//to the left
			for (int i = startAt - 1; i > 0; --i) {
				for (int j = 0; j < values.get(i).size(); ++j) {
					double x = values.get(0).get(j);
					double y = values.get(i).get(j);
					fitter.addObservedPoint(x, y);
				}
				double[] fitParameters = fitter.fit(f, initialGuess6);
				System.out.print(names.get(i) + "\t");
				out.write(names.get(i) + "\t");
				for (Double parameter: fitParameters) {
					System.out.print(parameter + "\t");
					out.write(String.format("%e", parameter) + "\t");
				}
				System.out.println();
				out.write("\n");
				fitter.clearObservations();
				initialGuess6 = fitParameters;
			}

			out.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void fit5(String inputFile, String outputFile) {
		try {
			LevenbergMarquardtOptimizer optimizer = new LevenbergMarquardtOptimizer();
			CurveFitter fitter = new CurveFitter(optimizer);
			ParametricRealFunction f = new f5();

			ArrayList<String> names = new ArrayList<String>();
			ArrayList<ArrayList<Double>> values = new ArrayList<ArrayList<Double>>();

			BufferedReader in = new BufferedReader(new FileReader(inputFile));
			String line = in.readLine();
			StringTokenizer st = new StringTokenizer(line);
			while (st.hasMoreTokens()) {
				names.add(st.nextToken());
				values.add(new ArrayList<Double>());
			}
			while ((line = in.readLine()) != null) {
				st = new StringTokenizer(line);
				int i = 0;
				double x = Double.parseDouble(st.nextToken());
				values.get(i++).add(x);
				while (st.hasMoreTokens()) {
					double y = Double.parseDouble(st.nextToken());
					values.get(i++).add(y);
				}
			}

			System.out.println(names.size());
			for (int i = 0; i < names.size(); ++i) {
				String name = names.get(i);
				name = name.substring(1, name.length());
				if (name.length() > 0) {
					if (startAt == Integer.parseInt(name)) {
						startAt = i;
					}
				}
			}
			System.out.println(startAt);



			BufferedWriter out = new BufferedWriter(new FileWriter(outputFile));

			//to the right
			for (int i = startAt; i < values.size(); ++i) {
				for (int j = 0; j < values.get(i).size(); ++j) {
					double x = values.get(0).get(j);
					double y = values.get(i).get(j);
					fitter.addObservedPoint(x, y);
				}
				double[] fitParameters = fitter.fit(f, initialGuess5);
				System.out.print(names.get(i) + "\t");
				out.write(names.get(i) + "\t");
				for (Double parameter: fitParameters) {
					System.out.print(parameter + "\t");
					out.write(String.format("%e", parameter) + "\t");
				}
				System.out.println();
				out.write("\n");
				fitter.clearObservations();
				initialGuess5 = fitParameters;
			}

			//to the left
			for (int i = startAt - 1; i > 0; --i) {
				for (int j = 0; j < values.get(i).size(); ++j) {
					double x = values.get(0).get(j);
					double y = values.get(i).get(j);
					fitter.addObservedPoint(x, y);
				}
				double[] fitParameters = fitter.fit(f, initialGuess5);
				System.out.print(names.get(i) + "\t");
				out.write(names.get(i) + "\t");
				for (Double parameter: fitParameters) {
					System.out.print(parameter + "\t");
					out.write(String.format("%e", parameter) + "\t");
				}
				System.out.println();
				out.write("\n");
				fitter.clearObservations();
				initialGuess5 = fitParameters;
			}

			out.close();
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * @return the startAt
	 */
	public int getStartAt() {
		return startAt;
	}

	/**
	 * @param startAt the startAt to set
	 */
	public void setStartAt(int startAt) {
		this.startAt = startAt;
	}
	
	private class f6 implements ParametricRealFunction {
		
		public double value(double x, double ... parameters) {
			
			double A0 = parameters[0];
			double A1 = parameters[1];
			double A2 = parameters[2];
			double A3 = parameters[3];
			double A4 = parameters[4];
			double A5 = parameters[5];
			
			//Y=2*A1^2*(1+2*COS(2*(A0)*((X))+A2))*EXP((X)/A3)+A4+A5*X
			double Y = 2 * FastMath.pow(A1, 2) * (1 + 2 * FastMath.cos(2 * A0 * x + A2)) * FastMath.exp(x / A3) + A4 + A5 * x; 
			
			return Y;
		}
		
		public double[] gradient(double x, double ... parameters) {
			double A0 = parameters[0];
			double A1 = parameters[1];
			double A2 = parameters[2];
			double A3 = parameters[3];
			double A4 = parameters[4];
			double A5 = parameters[5];
			
			double[] gradient = new double[parameters.length];
			
			//derivative for A0
			gradient[0] =  -8 * FastMath.pow(A1, 2) * x * FastMath.exp(x / A3) * FastMath.sin(2 * A0 * x + A2);
			//derivative for A1			
			gradient[1] = 2 * 2 * A1 * (1 + 2 * FastMath.cos(2 * A0 * x + A2)) * FastMath.exp(x / A3);
			//derivative for A2
			gradient[2] = -4 * FastMath.pow(A1, 2) * FastMath.exp(x / A3) * FastMath.sin(2 * A0 * x + A2);
			//derivative for A3
			gradient[3] = 2 * FastMath.pow(A1, 2) * (1 + 2 * FastMath.cos(2 * A0 * x + A2)) * FastMath.exp(x / A3) * (-x / FastMath.pow(A3, 2));
			//derivative for A4
			gradient[4] = 1;
			//derivative for A5
			gradient[5] = x;			
			
			return gradient;
		}
		
	}

	private class f5 implements ParametricRealFunction {

		public double value(double x, double ... parameters) {

			double A0 = parameters[0];
			double A1 = parameters[1];
			double A2 = parameters[2];
			double A3 = parameters[3];
			double A4 = parameters[4];

			//Y=2*A1^2*(1+2*COS(2*(A0)*((X))+A2))*EXP((X)/A3)+A4+A5*X
			double Y = 2 * FastMath.pow(A1, 2) * (1 + 2 * FastMath.cos(2 * A0 * x + A2)) * FastMath.exp(x / A3) + A4;

			return Y;
		}

		public double[] gradient(double x, double ... parameters) {
			double A0 = parameters[0];
			double A1 = parameters[1];
			double A2 = parameters[2];
			double A3 = parameters[3];
			double A4 = parameters[4];

			double[] gradient = new double[parameters.length];

			//derivative for A0
			gradient[0] =  -8 * FastMath.pow(A1, 2) * x * FastMath.exp(x / A3) * FastMath.sin(2 * A0 * x + A2);
			//derivative for A1
			gradient[1] = 2 * 2 * A1 * (1 + 2 * FastMath.cos(2 * A0 * x + A2)) * FastMath.exp(x / A3);
			//derivative for A2
			gradient[2] = -4 * FastMath.pow(A1, 2) * FastMath.exp(x / A3) * FastMath.sin(2 * A0 * x + A2);
			//derivative for A3
			gradient[3] = 2 * FastMath.pow(A1, 2) * (1 + 2 * FastMath.cos(2 * A0 * x + A2)) * FastMath.exp(x / A3) * (-x / FastMath.pow(A3, 2));
			//derivative for A4
			gradient[4] = 1;

			return gradient;
		}

	}

}
