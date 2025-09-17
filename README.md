
# Time Complexity Analyzer

This project analyzes the time complexity of code snippets and expresses the result in **asymptotic notation**. The program processes a given code snippet, identifies the various operations, and calculates the total time complexity, $T(n)$, based on the number of operations. It is designed to handle common programming constructs such as loops, conditional statements, and basic arithmetic operations.

### Features
This analyzer determines the time complexity of code snippets based on the following:

* **Statements**: Handles declarations, assignments, arithmetic operations, and input/output statements. Each statement is counted as a unit operation.
* **Selection (if, if-else)**: For `if-else` statements, the program evaluates both branches and selects the one with the higher operation count. For a simple `if` statement, it assumes the condition is always true and counts the operations within the `if` block.
* **Loops (for-loop)**: The program specifically handles `for-loops` where the iterator is initialized and updated only within the loop's header. When a loop's bound is represented by `n`, it is assumed to be a very large positive number.
* **Expressions**: The analyzer correctly handles relational operations, including those with arithmetic expressions, but does not support compound relational operations.

---

### Constraints

The program operates under the following assumptions and constraints:

* **Input**: The input is a syntactically correct code snippet. The first line is a positive integer indicating the number of lines that follow.
* **Variables**: The time complexity is expressed in terms of `n`, where `n` is a large positive number. The base for logarithmic time is enclosed in parentheses, e.g., $log(b)$. For square and cube roots, `sqrt(n)` and `cubert(n)` are used, respectively.
* **Conditional Statements**:
    * `if`: The condition is always assumed to be true.
    * `if-else`: The branch with the greater number of operations is chosen.
    * `if` blocks can contain input/output, assignment, and arithmetic operations. A loop can be nested within a selection block but not within another loop.
* **Loops**:
    * The loop is always a `for-loop`.
    * The loop iterator is only updated in the iteration/update part of the loop header.
    * Loop bodies can contain input/output, assignment, arithmetic operations, and selections.

---

### Input Format

The input consists of a positive integer on the first line, which indicates the number of lines in the code snippet. This is followed by the code snippet itself.

---

### Sample Inputs and Outputs

Here are some examples of valid inputs and their corresponding outputs:

| Sample Input | Sample Output |
| :--- | :--- |
| 5<br>int x = 10, y = 8, z = -2;<br>x = x + y;<br>y = x - y;<br>x = x - y;<br>z = x + y; | $T(n)=5$ |
| 6<br>int x, y, min;<br>cin>>x;<br>cin>>y;<br>min = x;<br>if(y < min)<br>    min = y; | $T(n)=5$ |
| 5<br>for(int i=0; i<n; i++){<br>    sum = i;<br>    sum = sum + 2;<br>    sum += 9;<br>} | $T(n)=6n+2$ |

---

### Version History
Version 1: This version provides the different tokens found in the input code snippet. The output is categorized by the type of statement (e.g., `for`, `if`) and lists the specific operations within each block.

```
statements:
x = 10
y = 8
z = -2
x = x + y
y = x - y
x = x - y
z = x + y
```

```
statements:
cin>>x
cin>>y
min = x
if:
condition: y < min
if statements:
min = y
```

```
for:
initializer: i=0
condition: i<n
update: i++
for statements:
sum = i
sum = sum + 2
sum += 9
```

## Authors

- [@kiloumanjaro](https://github.com/kiloumanjaro)
