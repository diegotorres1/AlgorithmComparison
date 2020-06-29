# Algorithm Comparison

A Qt application that compares different search and sort algorithms for configurable data sets.

## How To Use
Easy to use: click run and a sample data set is ran against the algorithm that is currently selected. You can input your own data from a file or generate it through the provided prompt. Below is an example view. A time table stores the series of tests you have run against your data sets.
![image](/sample.png)
Total Time is measured in nano-seconds.
___
### Comparison View
![image](/startup.png)
#### Inputs
- **INPUT DATA** - Prompt the user to enter their own data set.

- **Sort Selection** - a drop down to select between different sort algorithms.

- **Search Selection** - a drop down to select between different search algorithms.

- **Run** - Run the currently selected algorithm either search or sort.

- **Buffer Size** - The number of outputs to store in the output table.

#### Outputs
- **Current Output** - There are two main outputs, *Total Time* and *Number of Elements* that are shown after the algorithm is complete.

- **Table Output** - Outputs are stored in the table to compare between previous search or sort calls.

___
### Input Data
![image](/datainput.png)
#### Generate
- generate a set of data.

- **Sequential** - Choose to generate a sequential set of data [..., 5,6,7,8 ...] based on the user defined *Num of Elements*, *Lower Limit Num*, *Upper Limit Num*.
- **Random** - Choose to generate a random set of data [..., 4,-3,9,8 ...] based on the user defined *Num of Elements*, *Lower Limit Num*, *Upper Limit Num*.
- **Num of Elements** - The size of the data set to be generated.
- **Lower Limit Num** - the lowest possible value that can be generated.
- **Upper Limit Num** - the largest possible value that can be generated.
- **Generate** - generate data based on user defined parameters above.

#### From File
- read data from a file.

- **Open File** - Choose a .csv file from your system to read from.
