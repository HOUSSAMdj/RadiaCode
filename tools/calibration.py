import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import os

def perform_quadratic_regression(x, E):
    """
    Perform quadratic regression manually.
    Returns coefficients a2, a1, a0 for the equation E = a2*x^2 + a1*x + a0
    """
    # Create the design matrix for quadratic regression
    X = np.vstack([x**2, x, np.ones(len(x))]).T
    
    # Solve the normal equations using numpy's linear algebra solver
    # (X^T * X) * beta = X^T * E
    XTX = np.dot(X.T, X)
    XTE = np.dot(X.T, E)
    
    # Solve for the coefficients
    coeffs = np.linalg.solve(XTX, XTE)
    
    # Extract coefficients
    a2, a1, a0 = coeffs
    
    return a2, a1, a0

def quadratic_function(x, a2, a1, a0):
    """Calculate y values for a quadratic function E = a2*x^2 + a1*x + a0"""
    return a2 * x**2 + a1 * x + a0

def perform_calibration(data_file):
    """
    Perform calibration using measurement data from a file.
    """
    # Check if the file exists
    if not os.path.exists(data_file):
        print(f"Error: File '{data_file}' not found.")
        return
    
    try:
        # Read the calibration data from the file
        # Assuming data is in CSV format with columns for x and E values
        data = pd.read_csv(data_file)
        
        # Extract x and E columns
        x_values = data['x'].values
        e_values = data['E'].values
        
        # Perform quadratic regression
        a2, a1, a0 = perform_quadratic_regression(x_values, e_values)
        
        # Calculate the fitted values
        e_fit = quadratic_function(x_values, a2, a1, a0)
        
        # Calculate residuals (differences between measured and fitted values)
        residuals = e_values - e_fit
        
        # Calculate the standard deviation of the residuals
        std_dev = np.sqrt(np.sum(residuals**2) / (len(residuals) - 3))  # n-p where p=3 parameters
        
        # Print calibration results
        print("\nCalibration Results:")
        print(f"Quadratic fit: E = {a2:.6f}x^2 + {a1:.6f}x + {a0:.6f}")
        print(f"Standard deviation of residuals: {std_dev:.6f}")
        
        # Create visualization
        plt.figure(figsize=(10, 6))
        
        # Plot measured data points
        plt.scatter(x_values, e_values, color='blue', label='Measurement Points')
        
        # Plot the regression curve
        x_smooth = np.linspace(min(x_values), max(x_values), 100)
        y_smooth = quadratic_function(x_smooth, a2, a1, a0)
        plt.plot(x_smooth, y_smooth, 'r-', label=f'Quadratic Regression: {a2:.4f}x^2 + {a1:.4f}x + {a0:.4f}')
        
        # Add coefficient annotation directly in the plot
        coef_text = f"a₂ = {a2:.6f}\na₁ = {a1:.6f}\na₀ = {a0:.6f}"
        plt.annotate(coef_text, 
                    xy=(0.03, 0.30),  # Position in lower left area of the plot
                    xycoords='axes fraction',
                    verticalalignment='top',
                    bbox=dict(boxstyle="round,pad=0.5", fc="white", ec="gray", alpha=0.8))
        
        plt.title('RadiaCode Calibration: Measurement Points and Quadratic Regression')
        plt.xlabel('x [channel number]')
        plt.ylabel('Energy [keV]')
        plt.grid(True)
        plt.legend()
        
        # Save the plot
        plot_file = os.path.splitext(data_file)[0] + "_plot.png"
        plt.savefig(plot_file)
        print(f"Plot saved to {plot_file}")
        
        # Show the plot
        plt.show()
        
        return (a2, a1, a0)
    
    except Exception as e:
        print(f"Error during calibration: {e}")
        return None

def main():
    """Main function to run the calibration program."""
    print("Calibration Program")
    print("===================")
    
    # Get input file from user
    data_file = input("Enter the path to the calibration data file: ")
    
    # Perform calibration
    perform_calibration(data_file)

if __name__ == "__main__":
    main()
